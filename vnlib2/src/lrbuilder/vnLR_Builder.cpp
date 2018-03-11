//
//  vnLR_Builder.cpp
//  vnlib
//
//  Created by Wayne on 14-5-15.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include <iostream>
#include "vnLexAnalizers.h"
#include "vnAssert.h"

#include <map>
#include <list>
#include <vector>
#include <set>
#include <sstream>

using namespace _vn_ns;

enum SymbolType {
	kMean = 1,
	kOr,
	kEnd,
	kAt
};

struct KeywordInfo {
	str8 name;
	int refCount;
	unsigned int kwId;
	KeywordInfo(const str8 &s) :name(s), refCount(0), kwId(0) {}
	void addRef() { ++refCount; }
};

typedef std::map<str8, KeywordInfo> KeywordInfoMap;

struct SymbolInfo {
	str8 name;
	int refCount;
	unsigned int sId;
	SymbolInfo(const std::string &s) : name(s), refCount(0), sId(0) {}
	void addRef() { ++refCount; }
};

typedef std::map<str8, SymbolInfo> SymbolInfoMap;

struct Input {
	str8 name;
	int refCount;
	lex::Token::Type tokenType;
	Input() : refCount(0), tokenType(lex::Token::kNull) {}
	Input(const str8 &s) : name(s), refCount(0), tokenType(lex::Token::kNull) {}
	void addRef() { ++refCount; }
};

typedef std::map<str8, Input> InputMap;

struct Node;
struct Item {
	enum Type {
		kNull,
		kNode,
		kKeyword,
		kSymbol,
		kInput
	} type;
	union {
		Node *node;
		KeywordInfo *keyword;
		SymbolInfo *symbol;
		Input *input;
	};
	Item() : type(kNull) {
		node = 0;
	}
	Item(Node *v) : type(kNode) {
		node = v;
	}
	Item(KeywordInfo *v) : type(kKeyword) {
		keyword = v;
	}
	Item(SymbolInfo *v) : type(kSymbol) {
		symbol = v;
	}
	Item(Input *v) : type(kInput) {
		input = v;
	}
	
	bool operator ==(const Item &rhs) const {
		return type == rhs.type && node == rhs.node;
	}
	
	bool operator !=(const Item &rhs) const {
		return type != rhs.type || node != rhs.node;
	}
};

typedef std::vector<Item> ItemArray;

struct Production {
	unsigned int pId;
	Node *node;
	ItemArray items;
	Production(Node *_node) : pId(++s_count), node(_node) {}
	
	static unsigned int s_count;
};

unsigned int Production::s_count = 0;

typedef std::list<Production> ProductionList;

struct Node {
	std::string name;
	unsigned nId;
	int refCount;
	ProductionList productions;
	
	Node(const std::string &s) : name(s), nId(0), refCount(0) {}
	void addRef() { ++refCount; }
	Production & addProduction() {
		productions.push_back(this);
		return productions.back();
	}
};

typedef std::map<std::string, Node> NodeMap;
//////////////////////////////////////////////////////////

struct LR_Item {
	Production *production;
	size_t pos;
	bool reserved;
	LR_Item(Production *p) : production(p), pos(0), reserved(false) {}
	LR_Item(Production *p, size_t i) : production(p), pos(i), reserved(false) {}
	bool operator <(const LR_Item &rhs) const {
		if (production < rhs.production) {
			return true;
		} else if (production == rhs.production) {
			return pos < rhs.pos;
		}
		return false;
	}
	bool operator ==(const LR_Item &rhs) const {
		return production == rhs.production && pos == rhs.pos;
	}
	bool operator !=(const LR_Item &rhs) const {
		return production != rhs.production || pos != rhs.pos;
	}
	
	struct Less {
		bool operator ()(const LR_Item *lhs, const LR_Item *rhs) const {
			return *lhs < *rhs;
		}
	};
};
typedef std::list<LR_Item> LR_ItemList;
typedef std::set<LR_Item *, LR_Item::Less> LR_ItemIndex;

struct LR_State;

struct LR_Jump {
	Item condition;
	LR_State *state;
    //	LR_Action() : state(0) { state = 0; }
	LR_Jump(const Item &c, LR_State *s) : condition(c), state(s) {}
};
typedef std::list<LR_Jump> LR_JumpList;

struct LR_State {
	unsigned int sId;
	LR_ItemList items;
	LR_ItemIndex keyItems;
	LR_JumpList jumps;
	Production *reduction;
	
	LR_State() : sId(0), reduction(0) {}
	LR_State(const LR_State &other)
	: sId(0), items(other.items), reduction(0) {
		for (LR_ItemList::iterator it = items.begin(); it != items.end(); ++it) {
			if (it->pos) {
				keyItems.insert(&*it);
			}
		}
	}
	bool operator ==(const LR_State &rhs) const {
		if (keyItems.size() != rhs.keyItems.size()) {
			return false;
		}
		for (LR_ItemIndex::iterator i0 = keyItems.begin(), i1 = rhs.keyItems.begin(); i0 != keyItems.end(); ++i0, ++i1) {
			if (**i0 != **i1) {
				return false;
			}
		}
		return true;
	}
	
	bool operator <(const LR_State &rhs) const {
		for (LR_ItemIndex::iterator i0 = keyItems.begin(), i1 = rhs.keyItems.begin(); ; ++i0, ++i1) {
			if (i0 == keyItems.end()) {
				if (i1 != rhs.keyItems.end()) {
					return true;
				}
				break;
			}
			if (i1 == rhs.keyItems.end()) {
				break;
			}
			if (**i0 < **i1) {
				return true;
			}
		}
		return false;
	}
	
	void clear() {
		sId = 0;
		items.clear();
		keyItems.clear();
		jumps.clear();
	}
	
	bool addItem(const LR_Item &item) {
		for (LR_ItemList::iterator it = items.begin(); it != items.end(); ++it) {
			if (*it == item) {
				return false;
			}
		}
		items.push_back(item);
		if (item.pos) {
			keyItems.insert(&items.back());
		}
		return true;
	}
	
	void buildClosure() {
		for (LR_ItemList::iterator it = items.begin(); it != items.end(); ++it) {
			Item &i = it->production->items[it->pos];
			if (i.type != Item::kNode) {
				continue;
			}
			for (ProductionList::iterator pit = i.node->productions.begin(); pit != i.node->productions.end(); ++pit) {
				addItem(&*pit);
			}
		}
	}
	
	struct Less {
		bool operator ()(const LR_State *lhs, const LR_State *rhs) const {
			return *lhs < *rhs;
		}
	};
	
};

typedef std::list<LR_State> LR_StateList;


//////////////////////////////////////////////////////////

KeywordInfoMap g_keywords;
SymbolInfoMap g_symbols;
InputMap g_inputs;
NodeMap g_nodes;
lex::Token *g_errorToken = 0;
Production *g_rootProduction = 0;
LR_StateList g_states;

bool isKeyword(const char *s);
bool parse(lex::TokenList &tokens);
lex::TokenList::iterator parseExpression(lex::TokenList::iterator it, lex::TokenList::iterator end, Node *node);

bool buildOtherStates();
LR_State * addState(LR_State &state);

#define V_TT(token, tt) if (token->type != tt) throw token;
#define V_SYMBOL(token, sId) if (token->type != lex::Token::kSymbol || token->int32 != sId) throw token;
void V_NEXT(lex::TokenList::iterator &it, lex::TokenList::iterator end, lex::Token *&token);

int main(int argc, const char *argv[]) {
	
	lex::Lexer lexer = {
		vnnew lex::LA_Space(),
		vnnew lex::LA_Comment(),
		vnnew lex::LA_Identifier(),
		vnnew lex::LA_CharacterEx()
	};
	lex::LA_Symbol *la_symbol = vnnew lex::LA_Symbol();
    la_symbol->registerSymbol(":", kMean);
	la_symbol->registerSymbol("|", kOr);
	la_symbol->registerSymbol(";", kEnd);
	la_symbol->registerSymbol("@", kAt);
    lexer.add(la_symbol);
    
	
	const char *fileName = "/Users/wayne/Documents/script.txt";
	const char *targetFile01 = "states.hpp";
	const char *targetFile02 = "productions.hpp";
	const char *targetFile03 = "state_desc.txt";
	
	if (argc > 1) {
		fileName = argv[1];
	}
	
	FILE *fp = fopen(fileName, "rb");
	if (!fp) {
		std::cout << "Error! open file failed!" << std::endl;
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	int textLength = (int)ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	char *text = vnmalloc(char, textLength);
	fread(text, textLength, 1, fp);
	fclose(fp);
	/////////////////
    lex::Token::FilePtr file = vnnew lex::Token::File();
	file->name = fileName;
    lex::TokenList tokens;
	
	ssize_t ret = lexer.scan(text, textLength, file, tokens);
	file.reset();
	
	if (ret == -1) {
		std::cout << "Error!" << std::endl;
		std::cout << lexer.error().c_str() << std::endl;
	} else if (!parse(tokens)) {
		std::cout << "Error!" << std::endl;
		std::cout << "(" << g_errorToken->row + 1 << "): near \"" << g_errorToken->text.c_str() << "\"" << std::endl;
	} else {
		
        //		std::cout << "inputs: " << g_inputs.size() << std::endl;
        //		for (InputMap::iterator it = g_inputs.begin(); it != g_inputs.end(); ++it) {
        //			Input &input = it->second;
        //			std::cout << "\t" << input.name.c_str() << "  ref: " << input.refCount << std::endl;
        //		}
        //
        //		std::cout << "keywords: " << g_keywords.size() << std::endl;
        //		for (KeywordInfoMap::iterator it = g_keywords.begin(); it != g_keywords.end(); ++it) {
        //			KeywordInfo &kw = it->second;
        //			std::cout << "\t" << kw.name.c_str() << "  ref: " << kw.refCount << std::endl;
        // 		}
        //
        //		std::cout << "symbols: " << g_symbols.size() << std::endl;
        //		for (SymbolInfoMap::iterator it = g_symbols.begin(); it != g_symbols.end(); ++it) {
        //			SymbolInfo &s = it->second;
        //			std::cout << "\t" << s.name.c_str() << "  ref: " << s.refCount << std::endl;
        //		}
        //
        //		std::cout << "nodes: " << g_nodes.size() << std::endl;
        //		for (NodeMap::iterator it = g_nodes.begin(); it != g_nodes.end(); ++it) {
        //			Node &n = it->second;
        //			std::cout << "\t" << n.name.c_str() << " ref: " << n.refCount << std::endl;
        //		}
		
		g_inputs["identifier"].tokenType = lex::Token::kIdentifier;
        g_inputs["number"].tokenType = lex::Token::kNumber;
		g_inputs["character_literal"].tokenType = lex::Token::kCharacter;
		g_inputs["integer"].tokenType = lex::Token::kInteger;
		g_inputs["long_integer"].tokenType = lex::Token::kLongInteger;
		g_inputs["float"].tokenType = lex::Token::kFloat;
		g_inputs["double"].tokenType = lex::Token::kDouble;
		g_inputs["string_literal"].tokenType = lex::Token::kString;
		
		unsigned int uId = 0;
		for (KeywordInfoMap::iterator it = g_keywords.begin(); it != g_keywords.end(); ++it) {
			it->second.kwId = ++uId;
		}
		uId = 0;
		for (SymbolInfoMap::iterator it = g_symbols.begin(); it != g_symbols.end(); ++it) {
			it->second.sId = ++uId;
		}
		uId = 0;
		for (NodeMap::iterator it = g_nodes.begin(); it != g_nodes.end(); ++it) {
			it->second.nId = ++uId;
		}
		
		std::cout << "keywords: " << std::endl;
		for (KeywordInfoMap::iterator it = g_keywords.begin(); it != g_keywords.end(); ++it) {
			std::cout << it->second.name.c_str() << std::endl;
		}
		std::cout << "symbols:" << std::endl;
		for (SymbolInfoMap::iterator it = g_symbols.begin(); it != g_symbols.end(); ++it) {
			std::cout << it->second.name.c_str() << std::endl;
		}
		std::cout << "nodes:" << std::endl;
		for (NodeMap::iterator it = g_nodes.begin(); it != g_nodes.end(); ++it) {
			std::cout << it->second.name.c_str() << ":" << it->second.productions.size() << std::endl;
		}
		
		Node *root = &g_nodes.insert(std::make_pair("", Node(""))).first->second;
		g_rootProduction = &root->addProduction();
		g_rootProduction->pId = 0;
		g_rootProduction->items.push_back(&g_nodes.find("script")->second);
		g_rootProduction->items.push_back(Item());
		
		g_states.push_back(LR_State());
		LR_State &state0 = g_states.back();
		state0.items.push_back(g_rootProduction);
		state0.keyItems.insert(&state0.items.back());
		state0.buildClosure();
		
		buildOtherStates();
		
		std::ostringstream oss;
		oss << "// automatic generated by vnlib" << std::endl;
		oss << "static _vn_ns::lex::LR_Terminal g_keywordInfo[] = {" << std::endl;
		for (KeywordInfoMap::iterator it = g_keywords.begin(); it != g_keywords.end(); ++it) {
			oss << "\t{\"" << it->second.name.c_str() << "\", " << it->second.kwId << "}," << std::endl;
		}
		oss << "\t{(const char *)0, 0}" << std::endl << "};" << std::endl << std::endl;
		
		oss << "static _vn_ns::lex::LR_Terminal g_symbolInfo[] = {" << std::endl;
		for (SymbolInfoMap::iterator it = g_symbols.begin(); it != g_symbols.end(); ++it) {
			oss << "\t{\"" << it->second.name.c_str() << "\", " << it->second.sId << "}," << std::endl;
		}
		oss << "\t{(const char *)0, 0}" << std::endl << "};" << std::endl << std::endl;
		
		for (LR_StateList::iterator it = g_states.begin(); it != g_states.end(); ++it) {
			oss << "static _vn_ns::lex::LR_Jump g_state" << it->sId << "_jumps[] = {" << std::endl;
			for (LR_JumpList::iterator jit = it->jumps.begin(); jit != it->jumps.end(); ++jit) {
				oss << "\t_vn_ns::lex::LR_Jump(";
				if (jit->condition.type == Item::kNode) {
					oss << jit->condition.node->nId << ", ";
				} else {
					int a = 0, b = 0;
					const char *t = 0;
					switch (jit->condition.type) {
						case Item::kKeyword:
							t = "kTokenWithValue";
							a = lex::Token::kKeyword;
							b = jit->condition.keyword->kwId;
							break;
						case Item::kSymbol:
							t = "kTokenWithValue";
							a = lex::Token::kSymbol;
							b = jit->condition.symbol->sId;
							break;
						case Item::kInput:
							t = "kToken";
							a = jit->condition.input->tokenType;
							b = 0;
							break;
						default:
							vnassert(0);
							break;
					}
					oss << "_vn_ns::lex::LR_Jump::" << t << ", " << a << ", " << b << ", ";
				}
				oss << jit->state->sId << ")," << std::endl;
			}
			oss << "\t_vn_ns::lex::LR_Jump()" << std::endl;
			oss << "};" << std::endl << std::endl;
			if (it->reduction) {
				oss << "static _vn_ns::lex::LR_Reduction g_state" << it->sId << "_reduction = {" << std::endl;
				oss << "\t" << it->reduction->items.size() - 1 << ", " << it->reduction->pId << ", " << it->reduction->node->nId << std::endl;
				oss << "};" << std::endl << std::endl;
			}
		}
		
		oss << "static _vn_ns::lex::LR_State g_states[] = {" << std::endl;
		for (LR_StateList::iterator it = g_states.begin(); it != g_states.end(); ++it) {
			oss << "\t{" << it->sId << ", g_state" << it->sId << "_jumps, ";
			if (it->reduction) {
				oss << "&g_state" << it->sId << "_reduction";
			} else {
				oss << "(_vn_ns::lex::LR_Reduction *)0";
			}
			oss << "}," << std::endl;
		}
		oss << "\t{0xFFFFFFFF, (_vn_ns::lex::LR_Jump *)0, (_vn_ns::lex::LR_Reduction *)0}" << std::endl;
		oss << "};" << std::endl;
		
		fp = fopen(targetFile01, "wb");
		fwrite(oss.str().c_str(), oss.str().length(), 1, fp);
		fclose(fp);
		
		oss.str("");
		
		oss << "// automatic generated by vnlib." << std::endl;
		oss << "enum {" << std::endl;
		for (NodeMap::iterator it = g_nodes.begin(); it != g_nodes.end(); ++it) {
			int idx = 0;
			for (ProductionList::iterator pit = it->second.productions.begin(); pit != it->second.productions.end(); ++pit) {
				oss << "\tPROD_" << it->second.name.c_str() << "_" << idx++ << " = " << pit->pId << ", //" << pit->node->name.c_str() << " ->";
				for (Item *pi = &pit->items.front(); pi->type; ++pi) {
					switch (pi->type) {
						case Item::kKeyword:
							oss << " \'" << pi->keyword->name.c_str() << "\'";
							break;
						case Item::kSymbol:
							oss << " \'" << pi->symbol->name.c_str() << "\'";
							break;
						case Item::kInput:
							oss << " @" << pi->input->name.c_str();
							break;
						case Item::kNode:
							oss << " " << pi->node->name.c_str();
							break;
						default:
							break;
					}
				}
				oss << std::endl;
			}
		}
		oss << "\tPROD_COUNT = " << Production::s_count << std::endl;
		oss << "};" << std::endl;
		
		fp = fopen(targetFile02, "wb");
		fwrite(oss.str().c_str(), oss.str().length(), 1, fp);
		fclose(fp);
        
		oss.str("");
		
		for (LR_StateList::iterator it = g_states.begin(); it != g_states.end(); ++it) {
			oss << "state" << it->sId << ":" << std::endl;
			for (LR_ItemList::iterator iit = it->items.begin(); iit != it->items.end(); ++iit) {
				oss << "    ";
				Production &prod = *iit->production;
				oss << prod.node->name.c_str() << " -> ";
				size_t pos = 0;
				for (Item *pi = &prod.items.front(); pi->type; ++pi, ++pos) {
					if (iit->pos == pos) {
						oss << " * ";
					}
					switch (pi->type) {
						case Item::kKeyword:
							oss << " \'" << pi->keyword->name.c_str() << "\'";
							break;
						case Item::kSymbol:
							oss << " \'" << pi->symbol->name.c_str() << "\'";
							break;
						case Item::kInput:
							oss << " @" << pi->input->name.c_str();
							break;
						case Item::kNode:
							oss << " " << pi->node->name.c_str();
							break;
						default:
							break;
					}
				}
				oss << std::endl;
			}
			oss << std::endl;
		}
		
		fp = fopen(targetFile03, "wb");
		fwrite(oss.str().c_str(), oss.str().length(), 1, fp);
		fclose(fp);
		
	}
	/////////////////
	vnfree(text);
}

bool isKeyword(const char *s) {
	return (*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || *s == '_';
}

void V_NEXT(lex::TokenList::iterator &it, lex::TokenList::iterator end, lex::Token *&token) {
    if (++it == end) {
        throw token;
    }
    token = it->get();
}

bool parse(lex::TokenList &tokens) {
	try {
        for (lex::TokenList::iterator it = tokens.begin(); it != tokens.end(); ++it) {
            lex::Token *token = it->get();
			V_TT(token, lex::Token::kIdentifier);
			Node *node = &g_nodes.insert(std::make_pair(token->text, Node(token->text))).first->second;
			V_NEXT(it, tokens.end(), token);
			V_SYMBOL(token, kMean);
			for (;;) {
				V_NEXT(it, tokens.end(), token);
				it = parseExpression(it, tokens.end(), node);
                token = it->get();
				V_TT(token, lex::Token::kSymbol);
				if (token->int32 == kEnd) {
					break;
				}
			}
		}
	} catch (lex::Token *t) {
		
		g_errorToken = t;
		return false;
	}
	return true;
}

lex::TokenList::iterator parseExpression(lex::TokenList::iterator it, lex::TokenList::iterator end, Node *node) {
	Production &exp = node->addProduction();
    lex::Token *token = it->get();
	for (;;) {
		if (token->type == lex::Token::kSymbol) {
			if (token->int32 != kAt) {
				exp.items.push_back(Item());
				break;
			}
			V_NEXT(it, end, token);
			V_TT(token, lex::Token::kIdentifier);
			Input *input = &g_inputs.insert(std::make_pair(token->text, Input(token->text))).first->second;
			input->addRef();
			exp.items.push_back(input);
		} else if (token->type == lex::Token::kString) {
			if (isKeyword(token->text.c_str())) {
				KeywordInfo *kw = &g_keywords.insert(std::make_pair(token->text, KeywordInfo(token->text))).first->second;
				kw->addRef();
				exp.items.push_back(kw);
			} else {
				SymbolInfo *symbol = &g_symbols.insert(std::make_pair(token->text, SymbolInfo(token->text))).first->second;
				symbol->addRef();
				exp.items.push_back(symbol);
			}
		} else {
			V_TT(token, lex::Token::kIdentifier);
			Node *n = &g_nodes.insert(std::make_pair(token->text, Node(token->text))).first->second;
			n->addRef();
			exp.items.push_back(n);
		}
		V_NEXT(it, end, token);
	}
	return it;
}

bool buildOtherStates() {
	for (LR_StateList::iterator sit = g_states.begin(); sit != g_states.end(); ++sit) {
		LR_State &state = *sit;
		for (LR_ItemList::iterator iit = state.items.begin(); iit != state.items.end(); ++iit) {
			if (iit->reserved) {
				continue;
			}
			Item &condition = iit->production->items[iit->pos];
			if (condition.type != Item::kNull) {
				LR_State ns;
				for (LR_ItemList::iterator biit = iit; biit != state.items.end(); ++biit) {
					LR_Item &item = *biit;
					if (condition != item.production->items[item.pos]) {
						continue;
					}
					item.reserved = true;
					ns.addItem(LR_Item(item.production, item.pos + 1));
				}
				LR_State *dest = addState(ns);
				state.jumps.push_back(LR_Jump(condition, dest));
			} else {
				iit->reserved = true;
				vnassert(state.reduction == NULL);
				state.reduction = iit->production;
			}
		}
	}
	return true;
}

LR_State * addState(LR_State &state) {
	for (LR_StateList::iterator it = g_states.begin(); it != g_states.end(); ++it) {
		if (*it == state) {
			return &*it;
		}
	}
	unsigned int sId = (unsigned int)g_states.size();
	g_states.push_back(state);
	LR_State *ret = &g_states.back();
	ret->sId = sId;
	ret->buildClosure();
	return ret;
}


