/*
 * CurlWrapper.cpp
 *
 *  Created on: 2015年6月17日
 *      Author: leave
 */

#include "CurlWrapper.h"
#include <sstream>

#include <netdb.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string>
#include "../framework/ServerApp.h"

int CurlWrapper::CurlSend(HttpRequestPtr req) {
	if(req->callBackFn == nullptr){
		VN_LOG_ERROR(" CurlWrapper::CurlSend HttpRequest's callBackFn is NULL!");
		return -1;
	}
	m_isHttps = req->isHttps;
	//更具host地提，替换连接地址
	struct hostent *hptr;
	char str[32];
	if((hptr = gethostbyname(req->host.c_str())) == NULL){
		 VN_LOG_ERROR("gethostbyname error for host:" << req->host);
		 return -1;
	 }
	switch (hptr->h_addrtype) {
	case AF_INET:
	case AF_INET6:
		//pptr = hptr->h_addr_list;
		req->addr.from_string( inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str)));

		break;
		default:
		VN_LOG_ERROR("unknown address type");
		return -1;
		break;
	}

	//发起连接
	if(!m_isHttps){
		if (req->addr.port == 0) {
			req->addr.port = 80;
		}
		m_connHttp = (HttpConnection*) m_linkPool->connect(
				[]() {return vnnew HttpConnection();}, req->addr);
		if (!m_connHttp) {
			VN_LOG_ERROR("connect "<<req->addr.to_string() << " error !!");
			return -1;
		}
		m_connHttp->setConnectedCallBack(std::bind(&CurlWrapper::HttpConnectedCallBack,
				this,req));
		m_connHttp->setDestroyCallBack(std::bind(&CurlWrapper::DestroyCallBack,this));
	} else {
		if (req->addr.port == 0) {
			req->addr.port = 443;
		}
		m_connHttps = (HttpsConnection*) m_linkPool->connect(
				[]() {return vnnew HttpsConnection();}, req->addr);
		if (!m_connHttps) {
			VN_LOG_ERROR("connect "<<req->addr.to_string() << " error !!");
			return -1;
		}

		m_connHttps->setConnectedCallBack(std::bind(&CurlWrapper::HttpsConnectedCallBack,
				this, req));
		m_connHttps->setDestroyCallBack(std::bind(&CurlWrapper::DestroyCallBack,this));
	}
	return 0;
}

void CurlWrapper::HttpConnectedCallBack(HttpRequestPtr req){
	_ConnectedCallBack(m_connHttp.get(),req);
}
void CurlWrapper::HttpsConnectedCallBack(HttpRequestPtr req){
	_ConnectedCallBack(m_connHttps.get(),req);
}

template<class T>
void CurlWrapper::_ConnectedCallBack(T* conn, HttpRequestPtr req) {
	//std::ostringstream oss;
	std::string oss;
	oss.reserve(1024*5);
//	std::string addr = req->addr.to_string();
//	std::size_t pos = addr.find(':');
//	std::string host = addr.substr(0,pos);
	//填充数据
	if (req->isPost) {
		//
		oss += "POST ";
		oss += req->content.c_str();
		oss += " HTTP/1.1";
		oss += req->lineBreak;

		oss += "Host: ";
		oss += req->host.c_str();
		oss += req->lineBreak;
		//oss << "Accept-Encoding: identity\r\n";
		//oss << "Content-type: text/plain\r\n";
		std::map<std::string, std::string>::iterator itrHead = req->headMap.begin();
		for (; itrHead != req->headMap.end(); itrHead++) {
			oss += itrHead->first;
			oss += ": ";
			oss += itrHead->second;
			oss += req->lineBreak;
		}
		if (req->cookie.length() > 0) {
			oss += "Cookie: ";
			oss += req->cookie;
			oss += req->lineBreak;
		}

		oss += "Content-Length: ";
		oss += std::to_string(req->postUri.length());
		oss += req->lineBreak;
		oss += req->lineBreak;
		oss += req->postUri.c_str();
		oss += req->lineBreak;
	} else {

		oss += "GET ";
		oss += req->content.c_str();
		oss += " HTTP/1.1";
		oss += req->lineBreak;

		oss += "Host: ";
		oss += req->host.c_str();
		oss += req->lineBreak;
		std::map<std::string, std::string>::iterator itrHead = req->headMap.begin();
		for (; itrHead != req->headMap.end(); itrHead++) {
			oss += itrHead->first;
			oss += ": ";
			oss += itrHead->second;
			oss += req->lineBreak;
		}
		if (req->cookie.length() > 0) {
			oss += "Cookie: ";
			oss += req->cookie;
			oss += req->lineBreak;
		}

		oss += "Accept-Encoding: identity";
		oss += req->lineBreak;
		oss += req->lineBreak;
	}
	VN_LOG_DEBUG(oss.c_str());

//	uint64_t ps = getpagesize();
//	uint64_t page = (uint64_t(&(req->callBackFn)) & ~(ps -1));
//	if(mprotect((void *)page, ps, PROT_READ | PROT_WRITE) != 0){
//		handle_error("mprotect");
//	}

	//发送
	std::string reqStr = oss.c_str();
	if (!conn->sendHttpRequest(reqStr, req->callBackFn)) {
		VN_LOG_ERROR("send HttpRequest "<<req->addr.to_string() << " error !!");
		return;
	}
}

void CurlWrapper::DestroyCallBack(){
	if (m_closecb != nullptr) {
		m_closecb(this);
	}
}

void CurlWrapper::CloseConnection(){
	if (m_isHttps && m_connHttps) {
		m_connHttps->ssl_shutdown();
		m_connHttps->close();
	} else if (!m_isHttps && m_connHttp) {
		m_connHttp->close();
	}
}
