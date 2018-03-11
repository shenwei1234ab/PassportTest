/************************************************************************ 
 * Copyright(C) Tencent 2011
 ***********************************************************************/
#ifndef SRC_UTIL_LOG_H_
#define SRC_UTIL_LOG_H_

#include "vnLog.h"

#include <stdio.h>

#define DEFAULT_LOG_LENGTH 1024

#define tlog_log(format, priority,args...)\
{\
	if(priority != _vn_ns::Log::kDebug) {\
	char buf[DEFAULT_LOG_LENGTH];\
	int ret = snprintf(buf,DEFAULT_LOG_LENGTH,format,##args);\
	if(ret < DEFAULT_LOG_LENGTH){\
		_vn_ns::Log::Content(priority, __FILE__, __LINE__) << buf;\
	} else {\
		char buf[ret];\
		snprintf(buf,ret,format,##args);\
		_vn_ns::Log::Content(priority, __FILE__ , __LINE__ ) << buf;\
	}\
	}\
}


#define LOG(fmt,priority,  args...)  tlog_log(fmt,priority, ##args)
#define LOG_FATAL(fmt, args...)      LOG(fmt,_vn_ns::Log::kFatalError, ##args)
#define LOG_ALERT(fmt, args...)      LOG(fmt,_vn_ns::Log::kFatalError, ##args)
#define LOG_CRIT(fmt, args...)       LOG(fmt,_vn_ns::Log::kFatalError, ##args)
#define LOG_ERROR(fmt, args...)      LOG(fmt,_vn_ns::Log::kError, ##args)
#define LOG_WARN(fmt, args...)       LOG(fmt,_vn_ns::Log::kWarning, ##args)
#define LOG_NOTICE(fmt, args...)     LOG(fmt,_vn_ns::Log::kInformation, ##args)
#define LOG_INFO(fmt, args...)       LOG(fmt,_vn_ns::Log::kInformation, ##args)
#define LOG_DEBUG(fmt, args...)      LOG(fmt,_vn_ns::Log::kDebug, ##args)
#define LOG_TRACE(fmt, args...)      LOG(fmt,_vn_ns::Log::kDebug, ##args)
#define LOG_NOTSET(fmt, args...)     LOG(fmt,_vn_ns::Log::kDebug, ##args)



#endif  // SRC_UTIL_LOG_H_
