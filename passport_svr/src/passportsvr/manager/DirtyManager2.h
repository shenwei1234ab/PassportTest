/*
 * DirtyManager2.h
 *
 *  Created on: 2015年9月27日
 *      Author: tanghailong
 */

#ifndef SRC_PASSPORTSVR_MANAGER_DIRTYMANAGER2_H_
#define SRC_PASSPORTSVR_MANAGER_DIRTYMANAGER2_H_

#include "../../passportsvr/manager/StringFilter.h"
#include "vnSingleton.h"


class DirtyManager2 : public Singleton<DirtyManager2> {
public:

	virtual ~DirtyManager2();
	bool init();
	bool check(const str8 &s);
private:
	StringFilter filter;
};

#endif /* SRC_PASSPORTSVR_MANAGER_DIRTYMANAGER2_H_ */
