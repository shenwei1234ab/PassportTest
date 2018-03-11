/*
 * HttpsConnection.h
 *
 *  Created on: 2015年6月22日
 *      Author: leave
 */

#ifndef SRC_CURL_HTTPSCONNECTION_H_
#define SRC_CURL_HTTPSCONNECTION_H_


#include "vnLinkSSL.h"
#include "vnPacket.h"
#include "vnPackingStream.h"
#include "vnUnpackingStream.h"
#include "Curl.h"
#include "vnHttpResponseParser.h"

using namespace _vn_ns;


class HttpsConnection : public Link_TCP_Connection_SSL_Client, public RefCounted  {
public:

	void setConnectedCallBack(std::function<void()> cb);
	void setDestroyCallBack(std::function<void()> cb);

	bool sendHttpRequest(std::string& reqStr,std::function<void(const HttpResponse&)> callBack);
protected:
    virtual void _init();
    virtual void _ssl_connected();
    virtual void _destroy();
    virtual void _disconnected();
    virtual void _ssl_disconnected();
    virtual void _ssl_received(LinkBufferPtr buffer);

    std::function<void()> connectedCallBack;

    std::function<void(const HttpResponse&)> callBackFn;

    std::function<void()> m_closecb;

    //HttpResponse m_httpResponse;
    HttpResponseParser m_parser;
};

#endif /* SRC_CURL_HTTPSCONNECTION_H_ */
