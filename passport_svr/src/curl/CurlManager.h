/*
 * CurlManager.h
 *
 *  Created on: 2015年6月17日
 *      Author: leave
 */

#ifndef SRC_CURL_CURLMANAGER_H_
#define SRC_CURL_CURLMANAGER_H_

#include "vnSingleton.h"
#include "common/Util.h"


#include "CurlWrapper.h"
#include <set>

#include "manager/TimerManager.h"

#define MAX_FREE_HANDLES 300


class CurlManager : public vn::Singleton<CurlManager>{
public:

	bool init(vn::LinkPool *_linkPool,vn::u32 _maxNum) {
		if (!_linkPool) {
			return false;
		}
		m_linkPool = _linkPool;
		m_maxNum = _maxNum;
		//sTimerManager.createTimer(60,std::bind(&CurlManager::_cleanTimeOutWrapper,this),-1);
		return true;
	}

	int CurlSend(HttpRequestPtr req);

/*	vn::u32 size(){
		return m_curlWrapperList.size();
	}*/

	void Destroy();
protected:
	void _removeCurlWrapper(CurlWrapper *ptr);

	void _cleanTimeOutWrapper();

	vn::LinkPool *m_linkPool ;
	vn::u32 m_maxNum;

	std::mutex m_requestLisLock;
	std::list<HttpRequestPtr> m_requestList;

	std::mutex m_curlListLock;
	std::list<CurlWrapperPtr> m_curlWrapperList;
};



#define sCurlManager CurlManager::instance()

#endif /* SRC_CURL_CURLMANAGER_H_ */
