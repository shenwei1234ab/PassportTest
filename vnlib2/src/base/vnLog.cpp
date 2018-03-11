//
//  vnLog.cpp
//  vnlib
//
//  Created by Wayne on 5/19/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnLog.h"
#include "vnUnicode.h"

_vn_begin

Log::Level Log::level =
#ifdef VN_DEBUG
Log::kDebug
#else
Log::kInformation
#endif
;

Log::Content::~Content() {
    Log::instance()._output(m_level, m_file, m_line, m_context);
}

Log::Content & Log::Content::operator <<(bool value) {
    m_context += (value ? "true" : "false");
    return *this;
}

Log::Content & Log::Content::operator <<(s8 value) {
    m_context += std::to_string(value);
    return *this;
}

Log::Content & Log::Content::operator <<(u8 value) {
    m_context += std::to_string(value);
    return *this;
}

Log::Content & Log::Content::operator <<(s16 value) {
    m_context += std::to_string(value);
    return *this;
}

Log::Content & Log::Content::operator <<(u16 value) {
    m_context += std::to_string(value);
    return *this;
}

Log::Content & Log::Content::operator <<(s32 value) {
    m_context += std::to_string(value);
    return *this;
}

Log::Content & Log::Content::operator <<(u32 value) {
    m_context += std::to_string(value);
    return *this;
}

Log::Content & Log::Content::operator <<(s64 value) {
    m_context += std::to_string(value);
    return *this;
}

Log::Content & Log::Content::operator <<(u64 value) {
    m_context += std::to_string(value);
    return *this;
}

Log::Content & Log::Content::operator <<(long value) {
    m_context += std::to_string(value);
    return *this;
}

Log::Content & Log::Content::operator <<(unsigned long value) {
    m_context += std::to_string(value);
    return *this;
}

Log::Content & Log::Content::operator <<(f32 value) {
    //m_context += std::to_string(value);
    c8 buf[16];
    m_context += str8(buf, sprintf(buf, "%g", value));
    return *this;
}

Log::Content & Log::Content::operator <<(f64 value) {
    //m_context += std::to_string(value);
    c8 buf[32];
    m_context += str8(buf, sprintf(buf, "%g", value));
    return *this;
}

Log::Content & Log::Content::operator <<(const str8 &value) {
    m_context += value;
    return *this;
}

Log::Content & Log::Content::operator <<(const str16 &value) {
    str8 t;
    t.reserve(value.length());
    Unicode::convert(value.c_str(), value.length(), t);
    m_context += t;
    return *this;
}

Log::Content & Log::Content::operator <<(const str32 &value) {
    str8 t;
    t.reserve(value.length());
    Unicode::convert(value.c_str(), value.length(), t);
    m_context += t;
    return *this;
}

Log::Content & Log::Content::operator <<(const c8 *value) {
    m_context += value;
    return *this;
}

Log::Content & Log::Content::operator <<(const c16 *value) {
    str8 t;
    size_t len = str16::traits_type::length(value);
    t.reserve(len);
    Unicode::convert(value, len, t);
    m_context += t;
    return *this;
}

Log::Content & Log::Content::operator <<(const c32 *value) {
    str8 t;
    size_t len = str32::traits_type::length(value);
    t.reserve(len);
    Unicode::convert(value, len, t);
    m_context += t;
    return *this;
}

void Log::printer(LogPrinterPtr printer) {
    m_printer = std::move(printer);
}

const LogPrinterPtr & Log::printer() {
    return m_printer;
}

Log::Log()
: m_printer(vnnew LogPrinterSTDOUT()) {
    
}

void Log::_output(vn::Log::Level level, const c8 *file, int line, str8 &content) {
    if (m_printer) {
        m_printer->print(level, file, line, std::move(content));
    }
}

void LogPrinterSTDOUT::print(Log::Level level, const c8 *file, int line, str8 &&content) {
    const c8 *format = 0;
    switch (level) {
        case Log::kDebug: {
            format ="[%s:%d] D: %s\n";
            break;
        }
            
        case Log::kInformation: {
            format ="[%s:%d] I: %s\n";
            break;
        }
            
        case Log::kWarning: {
            format ="[%s:%d] W: %s\n";
            break;
        }
            
        case Log::kError: {
            format ="[%s:%d] E: %s\n";
            break;
        }
            
        case Log::kFatalError: {
            format ="[%s:%d] F: %s\n";
            break;
        }
            
        default: {
            return ;
        }
    }
    printf(format, (file ? file : "?"), line, content.c_str());
    content.clear();
}


_vn_end
