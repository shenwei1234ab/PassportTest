/*
 * DirtyManager.h
 *
 *  Created on: 2015年8月24日
 *      Author: shenwei
 */

#ifndef SRC_SERVER_MANAGER_DIRTYMANGER_H_
#define SRC_SERVER_MANAGER_DIRTYMANGER_H_

#include "vnSingleton.h"
#include "common/Util.h"
#include <vector>
using namespace _vn_ns;
using namespace vn;
class ChineseTree;
class DirtyManager : public Singleton<DirtyManager>
{
public:
	DirtyManager();
	
	virtual ~DirtyManager()
	{ 
		Destory(); 
	}
	
	int init();
	

	int Check(const c8* content);

	int Replace(c8* content);

	void  Destory();



private:
	ChineseTree *m_tree;
	bool m_ifInitSuccess = false;
	int LoadChar(const char *pStr);
};

#endif /* SRC_SERVER_MANAGER_DIRTYMANGER_H_ */
