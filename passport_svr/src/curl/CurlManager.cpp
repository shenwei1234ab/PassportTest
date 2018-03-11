/*
 * CurlManager.cpp
 *
 *  Created on: 2015年6月17日
 *      Author: leave
 */

#include "CurlManager.h"

int CurlManager::CurlSend(HttpRequestPtr req){
	if(m_curlWrapperList.size() < m_maxNum){
		CurlWrapperPtr wrapper(new CurlWrapper);
		std::function<void(CurlWrapper *)> cb = std::bind(&CurlManager::_removeCurlWrapper,this,std::placeholders::_1);
		wrapper->Init(m_linkPool,time(NULL),cb);
		m_curlListLock.lock();
		m_curlWrapperList.push_back(wrapper);
		m_curlListLock.unlock();
		wrapper->CurlSend(req);
	} else {
		//放入等待队列
		m_requestLisLock.lock();
		m_requestList.push_back(req);
		m_requestLisLock.unlock();
	}
	return 0;
}

void CurlManager::_removeCurlWrapper(CurlWrapper *ptr){
	m_curlListLock.lock();
	for (auto it = m_curlWrapperList.begin(); it != m_curlWrapperList.end(); ++it) {
		if (it->get() == ptr) {
			m_curlWrapperList.erase(it);
			break;
		}
	}
	m_curlListLock.unlock();

	//将等待队列中的任务启动
	m_requestLisLock.lock();
	if (!m_requestList.empty()) {
		this->CurlSend(m_requestList.front());
		m_requestList.pop_front();
	}
	m_requestLisLock.unlock();
}

//todo 定时器——清理超时的任务
void CurlManager::_cleanTimeOutWrapper(){
	std::lock_guard<std::mutex> lck (m_curlListLock);
	VN_LOG_DEBUG( "CurlManager::_cleanTimeOutWrapper" << "  Execute : "<< m_curlWrapperList.size() << "  Wait : " << m_requestList.size());
	for(auto p :m_curlWrapperList){
		if(p->m_isChecked){
			p->CloseConnection();
		} else {
			p->m_isChecked = true;
		}
	}
}

void CurlManager::Destroy(){
	std::lock_guard<std::mutex> lck (m_curlListLock);
	for(auto p :m_curlWrapperList){
		if(p){
			p->CloseConnection();
		}
	}
}
