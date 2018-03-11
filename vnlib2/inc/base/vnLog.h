//
//  vnLog.h
//  vnlib
//
//  Created by Wayne on 5/19/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnLog_h
#define vnlib_vnLog_h

#include "vnSingleton.h"
#include "vnRefCounted.h"
#include "vnSpinLock.h"
#include <vector>

#define VN_LOG_LEVEL_DEBUG         0
#define VN_LOG_LEVEL_INFORMATION   1
#define VN_LOG_LEVEL_WARNING       2
#define VN_LOG_LEVEL_ERROR         3
#define VN_LOG_LEVEL_FATAL_ERROR   4
#define VN_LOG_LEVEL_NONE          5

#ifndef VN_LOG_LEVEL
#ifdef VN_DEBUG
#   define VN_LOG_LEVEL VN_LOG_LEVEL_DEBUG
#else
#   define VN_LOG_LEVEL VN_LOG_LEVEL_INFORMATION
#endif
#endif

#if VN_LOG_LEVEL > VN_LOG_LEVEL_DEBUG
#   define VN_LOG_DEBUG(x)
#else
#   define VN_LOG_DEBUG(x) if (_vn_ns::Log::level <= _vn_ns::Log::kDebug) { _vn_ns::Log::Content(_vn_ns::Log::kDebug, __FILE__, __LINE__) << x; }
#endif

#if VN_LOG_LEVEL > VN_LOG_LEVEL_INFORMATION
#   define VN_LOG_INFO(x)
#else
#   define VN_LOG_INFO(x) if (_vn_ns::Log::level <= _vn_ns::Log::kInformation) { _vn_ns::Log::Content(_vn_ns::Log::kInformation, __FILE__, __LINE__) << x; }
#endif

#if VN_LOG_LEVEL > VN_LOG_LEVEL_WARNING
#   define VN_LOG_WARN(x)
#else
#   define VN_LOG_WARN(x) if (_vn_ns::Log::level <= _vn_ns::Log::kWarning) { _vn_ns::Log::Content(_vn_ns::Log::kWarning, __FILE__, __LINE__) << x; }
#endif

#if VN_LOG_LEVEL > VN_LOG_LEVEL_ERROR
#   define VN_LOG_ERROR(x)
#else
#   define VN_LOG_ERROR(x) if (_vn_ns::Log::level <= _vn_ns::Log::kError) { _vn_ns::Log::Content(_vn_ns::Log::kError, __FILE__, __LINE__) << x; }
#endif

#if VN_LOG_LEVEL > VN_LOG_LEVEL_FATAL_ERROR
#   define VN_LOG_FATAL(x)
#else
#   define VN_LOG_FATAL(x) if (_vn_ns::Log::level <= _vn_ns::Log::kFatalError) { _vn_ns::Log::Content(_vn_ns::Log::kFatalError, __FILE__, __LINE__) << x; }
#endif

_vn_begin

class LogPrinter;
typedef RefCountedPtr<LogPrinter> LogPrinterPtr;

class Log : public Singleton<Log> {
    friend struct CreateUsingNew<Log>;
public:
    enum Level {
        kDebug = 0,
        kInformation,
        kWarning,
        kError,
        kFatalError,
        kNone
    };
    
    class Content {
    public:
        Content(Level level, const c8 *file, int line) : m_level(level), m_file(file), m_line(line) {}
        ~Content();
        Content & operator <<(bool value);
        Content & operator <<(s8 value);
        Content & operator <<(u8 value);
        Content & operator <<(s16 value);
        Content & operator <<(u16 value);
        Content & operator <<(s32 value);
        Content & operator <<(u32 value);
        Content & operator <<(s64 value);
        Content & operator <<(u64 value);
        Content & operator <<(long value);
        Content & operator <<(unsigned long value);
		Content & operator <<(f32 value);
		Content & operator <<(f64 value);
        Content & operator <<(const str8 &value);
        Content & operator <<(const str16 &value);
        Content & operator <<(const str32 &value);
        Content & operator <<(const c8 *value);
        Content & operator <<(const c16 *value);
        Content & operator <<(const c32 *value);
    private:
        Level m_level;
        const c8 *m_file;
        int m_line;
        str8 m_context;
    };
    
    static Level level;
    
    void printer(LogPrinterPtr printer);
    const LogPrinterPtr & printer();
    
private:
    Log();
    void _output(Level level, const c8 *, int, str8 &context);
    
    LogPrinterPtr m_printer;
    
    friend class Context;
};

class LogPrinter : public RefCounted {
public:
    virtual void print(Log::Level, const c8 *, int, str8 &&) = 0;
};

class LogPrinterSTDOUT : public LogPrinter {
public:
    virtual void print(Log::Level, const c8 *, int, str8 &&);
};


_vn_end

#endif
