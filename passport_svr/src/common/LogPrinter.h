//
//  LogPrinter.h
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __passport_svr__LogPrinter__
#define __passport_svr__LogPrinter__

#include "vnLog.h"

using namespace _vn_ns;

class LogPrinterFile : public LogPrinter {
public:
    LogPrinterFile(str8 path);
    virtual void print(Log::Level, const c8 *, int, str8 &&);
    
protected:
    str8 m_path;
};


#endif /* defined(__passport_svr__LogPrinter__) */
