/*
 * HttpConnection.cpp
 *
 *  Created on: 2015年6月18日
 *      Author: leave
 */

#include "HttpConnection.h"

void HttpConnection::_init() {

}
void HttpConnection::_destroy() {
	VN_LOG_DEBUG( "HttpConnection _destroy");
	if (m_closecb != nullptr) {
		m_closecb();
	}
}
void HttpConnection::_disconnected() {
	VN_LOG_DEBUG( "HttpConnection _disconnected");
	 this->close();
}

void HttpConnection::setConnectedCallBack(std::function<void()> cb){
	connectedCallBack = cb;
}
void HttpConnection::setDestroyCallBack(std::function<void()> cb){
	m_closecb = cb;
}
void HttpConnection::_connected(){
	if (connectedCallBack != nullptr) {
		connectedCallBack();
	}
}
void HttpConnection::_received(LinkBufferPtr buffers) {
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
	/*
	//存储进数据流，读到包结束，进行解析，调用回调函数
	while (buffers) {
		m_httpResponse.m_buffer.append((c8 *) buffers->lock(), buffers->size());
		buffers = buffers->next();
	}

	m_httpResponse.update();
	if (m_httpResponse.isEnd()) {
		callBackFn(m_httpResponse);
		this->close();
	}
	*/
}


bool HttpConnection::sendHttpRequest(std::string& reqStr,std::function<void(const HttpResponse&)> callBack){
	if(!this->connected()){
		VN_LOG_ERROR("have not build connection !.")
		return false;
	}
	if(callBackFn != nullptr){
		VN_LOG_ERROR("this connection not free !.")
		return false;
	}
	callBackFn = std::move(callBack);

	LinkBufferPtr buffers = LinkBuffer::create();
	LinkBuffer* m_tail = buffers.get();
	size_t remaining = reqStr.length();
	size_t m_offset = 0;
	const u8 *ptr = (const u8 *) reqStr.data();
	while (remaining) {
		size_t bytes = m_tail->write(m_offset, ptr, remaining);
		remaining -= bytes;
		ptr += bytes;
		m_offset += bytes;
		if (m_offset == m_tail->capacity()) {
			auto p = LinkBuffer::create().release();
			m_tail->link_next(p);
			m_tail = p;
			m_offset = 0;
		}
	}

	this->send(buffers);
	return true;
}
