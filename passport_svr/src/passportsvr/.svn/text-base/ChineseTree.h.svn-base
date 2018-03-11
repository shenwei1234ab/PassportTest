/*
 * ChineseTree.h
 *
 *  Created on: 2015年8月24日
 *      Author: 沈巍
 */
#ifndef SRC_PASSPORTSVR_CHINESETREE_H_
#define SRC_PASSPORTSVR_CHINESETREE_H_
#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "vnUnicode.h"
#include <functional>
#include "vnAllocatedObject.h"
using namespace vn;
class ChineseTree:public AllocatedObject
{
public:
	struct Node:public AllocatedObject
	{
		c32 m_data;
		std::map<c32, Node *> m_childrens;
		Node(c32 _data) :m_data(_data)
		{

		}

		Node()
		{

		}
		bool m_ifEnd = false;
	};

	ChineseTree();

	~ChineseTree();



	void Create(c32 *pArray)
	{
		_CreateNode(m_root, pArray);
	}


	void Visit(std::function<void(Node * &)> pFun)
	{
		_Visit(m_root, pFun);
	}


	void Replace(c32 *pArray);


	bool Check(c32 *pArray);



private:
	Node *m_root;

	bool _Check();

	void _Replace();

	void _Init(c32 *pArray);
	
	
	void _FreeSpace(Node * &pNode);



	unsigned int _ConvertToLowerLetter(unsigned int pLetter);


	void _CreateNode(Node *root, c32 *pArray);


	void _Visit(Node *pRoot, std::function<void(Node * &)> pFun);


	void _FreeNodeSpace(Node *&pRoot);

	void _Search(Node *pRoot, c32 *pArray);
private:
	c32 *m_currentBegin;
	std::list<c32 * > m_findingChars;
	std::list<c32 *  > m_foundedChars;

};



#endif /* SRC_SERVER_WORLDMANAGER_H_ */
