/*
 * HttpsConnection.cpp
 *
 *  Created on: 2015年6月22日
 *      Author: leave
 */

#include "HttpsConnection.h"


void HttpsConnection::_init() {
}
void HttpsConnection::_destroy() {
	VN_LOG_DEBUG( "HttpsConnection _destroy");
	if(m_closecb!= nullptr){
		m_closecb();
	}

}
void HttpsConnection::_disconnected() {
	VN_LOG_DEBUG( "HttpsConnection _disconnected");
	 this->close();
}
void HttpsConnection::_ssl_disconnected() {
	VN_LOG_DEBUG( "HttpsConnection _ssl_disconnected");
}

void HttpsConnection::setConnectedCallBack(std::function<void()> cb){
	connectedCallBack = cb;
}
void HttpsConnection::setDestroyCallBack(std::function<void()> cb){
	m_closecb = cb;
}
void HttpsConnection::_ssl_connected(){
	if (connectedCallBack != nullptr) {
		connectedCallBack();
	}
}
void HttpsConnection::_ssl_received(LinkBufferPtr buffers) {
	auto tb = buffers;
	do {
		VN_LOG_DEBUG("\n" << str8((const char*)tb->lock(), tb->size()));
		tb = tb->next();
	} while (tb);
	m_parser.append(std::move(buffers));
	for (;;) {
		auto ret = m_parser.fetch();
		if (ret == HttpResponseParser::kError) {
			VN_LOG_ERROR( "HttpResponseParser::kError");
			this->close();
			break;
		} else if (ret == HttpResponseParser::kWait) {
			break;
		} else {
			callBackFn(m_parser.result());
			this->close();
		}
	}
	//存储进数据流，读到包结束，进行解析，调用回调函数
/*
	m_httpResponse.m_buffer.append((c8 *) buffer->lock(), buffer->size());
	m_httpResponse.update();
	if (m_httpResponse.isEnd()) {
		callBackFn(m_httpResponse);
		this->ssl_shutdown();
		this->close();
	}*/
}

#include "openssl/ssl.h"

bool HttpsConnection::sendHttpRequest(std::string& reqStr,std::function<void(const HttpResponse&)> callBack){
	if(!this->ssl_connected()){
		VN_LOG_ERROR("have not build connection !.")
		return false;
	}
	if(callBackFn != nullptr){
		VN_LOG_ERROR("this connection not free !.")
		return false;
	}
	callBackFn = std::move(callBack);

	this->ssl_send(reqStr.c_str(), reqStr.length());
	/*if (m_ssl_connected) {
		int ret = SSL_write((SSL *) m_ssl, reqStr.c_str(), reqStr.length());
		if(ret <= 0){
			VN_LOG_ERROR("SSL error :" <<SSL_get_error((SSL *)m_ssl, 0));
			return false;
		}
	} else {
		VN_LOG_ERROR("have not build connection !.")
		return false;
	}
*/
	return true;
}
