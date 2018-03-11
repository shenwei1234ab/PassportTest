/*
 * Chinese.cpp
 *
 *  Created on: 2015年8月24日
 *      Author: 沈巍
 */

#include "../passportsvr/ChineseTree.h"


ChineseTree::ChineseTree()
{
	m_root = vnnew Node(0);
}


ChineseTree::~ChineseTree()
{
	if (m_root)
	{
		_FreeNodeSpace(m_root);
	}
}


void ChineseTree::Replace(c32 *pArray)
{
	_Init(pArray);
	//寻找关键字并保存地址
	_Search(m_root, pArray);
	//替换脏字
	_Replace();
}



bool ChineseTree::Check(c32 *pArray)
{
	_Init(pArray);
	//寻找关键字并保存地址
	_Search(m_root, pArray);
	return _Check();
}


bool ChineseTree::_Check()
{
	if (m_foundedChars.empty())
	{
		return false;
	}
	return true;
}


void ChineseTree::_Replace()
{
	if (m_foundedChars.empty())
	{
		return;
	}

	for (auto iter = m_foundedChars.begin(); iter != m_foundedChars.end(); ++iter)
	{
		*(*iter) = 0x0023;
	}
}



void ChineseTree::_Init(c32 *pArray)
{
	m_findingChars.clear();
	m_foundedChars.clear();
	m_currentBegin = pArray;
}

void ChineseTree::_FreeSpace(Node * &pNode)
{
	if (pNode)
	{
		delete pNode;
		pNode = NULL;
	}
}



unsigned int ChineseTree::_ConvertToLowerLetter(unsigned int pLetter)
{
	unsigned int ret = pLetter;
	if (ret >= 65 && ret <= 90)
	{
		ret += 32;
	}
	return ret;
}




void ChineseTree::_CreateNode(Node *root, c32 *pArray)
{
	if (*pArray == 0)
	{
		root->m_ifEnd = true;
		return;
	}
	c32 cValue = _ConvertToLowerLetter(*pArray);
	auto iter = root->m_childrens.find(cValue);
	if (iter != root->m_childrens.end())
	{
		//找到
		_CreateNode(iter->second, pArray + 1);
	}
	else
	{
		//没有找到
		Node *pNew = new Node(cValue);
		root->m_childrens.insert(std::make_pair(cValue, pNew));
		_CreateNode(pNew, pArray + 1);
	}
}




void ChineseTree::_Visit(Node *pRoot, std::function<void(Node * &)> pFun)
{
	if (!pRoot)
	{
		return;
	}
	if (pRoot->m_childrens.empty())
	{
		pFun(pRoot);
		return;
	}
	for (auto iter = pRoot->m_childrens.begin(); iter != pRoot->m_childrens.end(); ++iter)
	{
		_Visit(iter->second, pFun);
	}
	pFun(pRoot);
}






void ChineseTree::_FreeNodeSpace(Node *&pRoot)
{
	if (!pRoot)
	{
		return;
	}
	if (pRoot->m_childrens.empty())
	{
		delete pRoot;
		pRoot = NULL;
		return;
	}
	for (auto iter = pRoot->m_childrens.begin(); iter != pRoot->m_childrens.end(); ++iter)
	{
		_FreeNodeSpace(iter->second);
	}
	delete pRoot;
	pRoot = NULL;
}






void ChineseTree::_Search(Node *pRoot, c32 *pArray)
{
	if (*pArray == 0)
	{
		if (pRoot->m_ifEnd)
		{
			m_currentBegin = pArray;
			m_foundedChars.splice(m_foundedChars.end(), m_findingChars);
			m_findingChars.clear();
		}
		return;
	}

	c32 cValue = _ConvertToLowerLetter(*pArray);
	if (pRoot == m_root)
	{
		auto iter = pRoot->m_childrens.find(cValue);
		if (iter == pRoot->m_childrens.end())
		{
			m_currentBegin++;
			_Search(pRoot, pArray + 1);
			return;
		}
		else
		{
			m_findingChars.push_back(pArray);
			_Search(iter->second, pArray + 1);
			return;
		}
	}
	else
	{
		//找到模式
		if (pRoot->m_ifEnd)
		{
			m_currentBegin = pArray;
			m_foundedChars.splice(m_foundedChars.end(), m_findingChars);
			m_findingChars.clear();
			_Search(m_root, m_currentBegin);
			return;
		}
		else
		{
			auto iter = pRoot->m_childrens.find(cValue);
			if (iter == pRoot->m_childrens.end())
			{
				//没有找到
				//回退
				m_currentBegin++;
				m_findingChars.clear();
				//重头开始找
				_Search(m_root, m_currentBegin);
				return;
			}
			else
			{
				//找到继续往下找
				m_findingChars.push_back(pArray);
				_Search(iter->second, pArray + 1);
				return;
			}
		}
	}
}