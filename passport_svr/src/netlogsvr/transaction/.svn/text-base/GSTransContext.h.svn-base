/*******************************************************************************
 @ ��Ȩ����(C) Tencent 2013
 ********************************************************************************/
#ifndef __GAMESVR_TRANSACTION_GSTRANSCONTEXT_H__
#define __GAMESVR_TRANSACTION_GSTRANSCONTEXT_H__

#include "tranbase/Util.h"
#include <string.h>

class GSLogContext {
public:
	GSLogContext(std::string _context) {
		const char * split = "|";
		/*char *value = strtok((char*) _context.c_str(), split);
		if (value != NULL) {
			m_LogName = value;
		}
		value = strtok(NULL, split);
		while (value != NULL) {
			m_ContextMap.push_back(value);
			value = strtok(NULL, split);
		}*/
		size_t pos = 0;
		size_t next = _context.find(split,pos);
		if (next != std::string::npos) {
			m_LogName = std::move(_context.substr(pos, next - pos));
			pos = next + 1;
			next = _context.find(split, pos);
		}
		while (next != std::string::npos) {
			m_ContextMap.push_back(std::move(_context.substr(pos, next - pos)));
			pos = next + 1;
			next = _context.find(split, pos);
		}
		m_ContextMap.push_back(std::move(_context.substr(pos, next)));
	}
	~GSLogContext(){}

	std::string m_LogName;

	const std::vector<std::string>& GetContextMap(){
		return m_ContextMap;
	}
protected:


	std::vector<std::string> m_ContextMap;
};

#endif // end of __GAMESVR_TRANSACTION_GSTRANSCONTEXT_H__
