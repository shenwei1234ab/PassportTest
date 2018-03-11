//
//  LogPrinter.cpp
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "LogPrinter.h"

LogPrinterFile::LogPrinterFile(str8 path)
: m_path(std::move(path)) {
    if (!m_path.empty() && m_path.back() != '/') {
        m_path.push_back('/');
    }
}

void LogPrinterFile::print(Log::Level l, const c8 *f, int n, str8 &&s) {
    time_t tt;
    time(&tt);
    struct tm *ti = ::localtime(&tt);
    c8 fileName[1024];
    sprintf(fileName, "%s%04d_%02d_%02d_%02d.log", m_path.c_str(), ti->tm_year + 1900, ti->tm_mon + 1, ti->tm_mday, ti->tm_hour);
    
    c8 t;
    switch (l) {
        case Log::kDebug:
            t = 'D';
            break;
        case Log::kInformation:
            t = 'I';
            break;
        case Log::kWarning:
            t = 'W';
            break;
        case Log::kError:
            t = 'E';
            break;
        case Log::kFatalError:
            t = 'F';
            break;
        default:
            t = 'U';
            break;
    }
    
    FILE *fp = fopen(fileName, "a");
    if (fp) {
        fprintf(fp, "[%02d:%02d][%s:%d] %c: %s\n", ti->tm_min, ti->tm_sec, (f ? f : "?"), n, t, s.c_str());
        fclose(fp);
    }
    s.clear();
}