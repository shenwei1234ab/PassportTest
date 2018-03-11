/*
 * Curl.h
 *
 *  Created on: 2015年6月17日
 *      Author: leave
 */

#ifndef SRC_CURL_CURL_H_
#define SRC_CURL_CURL_H_

#include <vector>
#include <functional>
#include <sstream>

#include "vnLog.h"
#include "vnNetAddress.h"
#include "vnHttpResponse.h"
#include <map>

//请求
class HttpRequestC{
public:
	HttpRequestC(){

	}
	~HttpRequestC(){

	}
	vn::NetAddress addr;
	std::string host = "";
	bool isHttps = false;
	std::string content = "";
	bool isPost = false;
	//	int cookielen;
	std::string cookie = "";
	//int uri_len;
	std::string postUri = "";
	std::map<std::string,std::string> headMap;
	std::string lineBreak = "\n";
	//std::function<void(HttpResponse)> callBackFn;
	std::function<void(const vn::HttpResponse&)> callBackFn;
};

typedef std::shared_ptr<HttpRequestC> HttpRequestPtr;



/*
 //响应
class HttpResponse{
public:
	void print_state (const std::ios& stream) {
		VN_LOG_DEBUG(  " good()=" << stream.good());
		VN_LOG_DEBUG(  " eof()=" << stream.eof());
		VN_LOG_DEBUG(  " fail()=" << stream.fail());
		VN_LOG_DEBUG(  " bad()=" << stream.bad());
	}

	HttpResponse():m_isEnd(false),m_isReadHead(true),m_state(0),m_contentLength(0),m_haveReadLength(0){

	}
	HttpResponse(HttpResponse&& _Response){
		m_isEnd = _Response.m_isEnd;
		m_isReadHead = _Response.m_isReadHead;
		m_state = _Response.m_state;
		m_contentLength = _Response.m_contentLength;
		m_buffer.swap(_Response.m_buffer);

		m_haveReadLength = _Response.m_haveReadLength;
		m_variableJSON = _Response.m_variableJSON;
		m_vJSONPtr = _Response.m_vJSONPtr;
		m_vJSONStr.swap(_Response.m_vJSONStr);
	}
	void update(){
		//是否有完整的行
		std::istringstream ios;
		ios.str(m_buffer);
		m_isEnd = true; //可能没有"Content-Length"字段
		std::string line;
		std::getline(ios, line);
		while(!line.empty()){
//			print_state(ios);
			if(ios.eof() && m_contentLength != m_haveReadLength + line.size()){
				break;//不是完整的一行
			}
//			VN_LOG_DEBUG( "收到： " << line);
			if(m_isReadHead){
				if(0 == m_state){
					std::string http = "HTTP/";
					size_t  size = http.size();
					if(0 != line.compare(0,size,http)){
						VN_LOG_ERROR("http read error!");
					}
					std::string sState = line.substr(9,3);
					m_state = atoi(sState.c_str());
				} else {
					size_t pox = line.find(':');
					if(pox == std::string::npos){
						if (0 == line.compare("\r")) {
							m_isReadHead = false;
						} else {
							VN_LOG_ERROR("http read error!");
						}
					}
					std::string contentlen ="Content-Length";
					size_t  size = contentlen.size();
					if( 0 == line.compare(0,size,contentlen)){
						pox++; //跳过冒号
						if(line[pox+1] == ' '){
							pox++;
						}
						std::string cntLen = line.substr(pox,line.size()-pox-1);
						m_contentLength = atoi(cntLen.c_str());
						m_isEnd= false;
					}
				}
			} else {
//				VN_LOG_DEBUG( "收到： " << line);
				m_vJSONStr += line;
				m_haveReadLength += line.size() + 1; //加上换行符
				if (m_contentLength == m_haveReadLength - 1) {
					m_haveReadLength--; //减去eof的大小
					m_isEnd = true;

					try {
						m_vJSONPtr = m_variableJSON.parse(m_vJSONStr.data(),
								m_vJSONStr.size());
//						for (int i = 0; i < (int) m_vJSONPtr->size(); i++) {
//							vn::VariablePtr var = m_vJSONPtr->at(i);
//
//							VN_LOG_INFO(
//									"Variable_string : "<<var.dyn_cast< vn::Variable_string>()->value());
//						}

//						VN_LOG_DEBUG( "JSON data is : \n" << m_vJSONStr.data());
					} catch (...) {
						VN_LOG_ERROR( "JSON parse error! \n" << m_vJSONStr.data());
					}

				}
			}
			if (ios.eof()) {
				break;
			}
			std::getline(ios, line);

		}
		m_buffer = line;

	}
	bool isEnd(){
		return m_isEnd;
	}
	std::string m_buffer;
	bool m_isEnd;

	bool m_isReadHead;

	//报文信息

	vn::u16 m_state;//状态码
	vn::u32 m_contentLength;//内容长度
	vn::u32 m_haveReadLength;
	vn::VariableJSON m_variableJSON;
	vn::RefCountedPtr<vn::Variable_object> m_vJSONPtr;

	std::string m_vJSONStr;

};*/

#endif /* SRC_CURL_CURL_H_ */
