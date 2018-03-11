/*
 * Log.cpp
 *
 *  Created on: 2015年11月2日
 *      Author: leave
 */




#include "tranbase/logmgr/Log.h"

#include <stdio.h>
#include "stdarg.h"

#include "vnLog.h"

/*void tlog_log(const char * format, ...){
	char buf[1024];
	va_list arg_ptr;
	va_start(arg_ptr,format);
	//sprintf(log,format,arg_ptr);
	vsprintf(buf,format,arg_ptr);
		//printf("%s",buf);
	VN_LOG_DEBUG(buf);
	va_end(arg_ptr);
}*/
