/*
 * CurlWrapper.h
 *
 *  Created on: 2015年6月17日
 *      Author: leave
 */

#ifndef SRC_CURL_CURLWRAPPER_H_
#define SRC_CURL_CURLWRAPPER_H_

#include "Curl.h"
#include "HttpConnection.h"
#include "HttpsConnection.h"


class CurlWrapper;
typedef RefCountedPtr<CurlWrapper> CurlWrapperPtr;

class CurlWrapper: public RefCounted {
public:

	bool Init(vn::LinkPool *_linkPool,vn::u64 _startTime,std::function<void(CurlWrapper *)> closecb){
		if (!_linkPool) {
			return false;
		}
		m_linkPool = _linkPool;
		m_startTime = _startTime;
		m_closecb = std::move(closecb);
		m_isChecked = false;
		m_connHttp = nullptr;
		m_connHttps = nullptr;
		return true;
	}
	void SetCloseCallBack(std::function<void(CurlWrapper *)> closecb){
		m_closecb = std::move(closecb);
	}
	int CurlSend(HttpRequestPtr req);


	void HttpConnectedCallBack(HttpRequestPtr req);
	void HttpsConnectedCallBack(HttpRequestPtr req);

	template <class T>
	void _ConnectedCallBack(T* conn,HttpRequestPtr req);

	void DestroyCallBack();
	void CloseConnection();

	bool m_isChecked;
protected:
	vn::LinkPool *m_linkPool ;

	vn::u64 m_startTime;//超时处理使用
	std::function<void(CurlWrapper *)> m_closecb;

	RefCountedPtr<HttpConnection> m_connHttp;
	RefCountedPtr<HttpsConnection> m_connHttps;
	bool m_isHttps;
};



#endif /* SRC_CURL_CURLWRAPPER_H_ */
