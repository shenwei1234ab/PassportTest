//
//  DateTime.cpp
//  optd_srv
//
//  Created by Wayne on 1/29/15.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#include "DateTime.h"

#include <ctime>

bool Date::operator <(const Date &t) const {
//    if (year < t.year) {
//        return true;
//    }
//    if (year == t.year) {
//        if (month < t.month) {
//            return true;
//        }
//        if (month == t.month) {
//            return day < t.day;
//        }
//    }
//    return false;
    return date < t.date;
}

bool Date::operator ==(const Date &t) const {
    return date == t.date;
}

bool Date::operator !=(const Date &t) const {
    return date != t.date;
}

bool Date::convertFromString(const str8 &s) {
    u32 y, m, d;
    if (sscanf(s.c_str(), "%u-%u-%u", &y, &m, &d) != 3) {
        return false;
    }
    if (m >= 1 && m <= 12 && d >=1 && d <= 31) {
        year = y;
        month = m - 1;
        day = d;
        return true;
    }
    return false;
}

str8 Date::convertToString() const {
    c8 buf[32];
    return {buf, (size_t)sprintf(buf, "%u-%u-%u", year, month + 1, day)};
}

Date Date::today() {
    Date ret;
    time_t raw = time(nullptr);
    struct tm time;
    localtime_r(&raw, &time);
    ret.year = time.tm_year + 1900;
    ret.month = time.tm_mon;
    ret.day = time.tm_mday;
    return ret;
}

bool Time::operator <(const Time &t) const {
//    if (hour < t.hour) {
//        return true;
//    }
//    if (hour == t.hour) {
//        if (minute < t.minute) {
//            return true;
//        }
//        if (minute == t.minute) {
//            return second < t.second;
//        }
//    }
//    return false;
    return time < t.time;
}

bool Time::operator ==(const Time &t) const {
    return time == t.time;
}

bool Time::operator !=(const Time &t) const {
    return time != t.time;
}

bool Time::convertFromString(const str8 &s) {
    u32 h, m, sec;
    if (sscanf(s.c_str(), "%u:%u:%u", &h, &m, &sec) != 3) {
        return false;
    }
    if (h < 24 && m < 60 && sec < 60) {
        hour = h;
        minute = m;
        second = sec;
        return true;
    }
    return false;
}

str8 Time::convertToString() const {
    c8 buf[32];
    return {buf, (size_t)sprintf(buf, "%u:%u:%u", hour, minute, second)};
}

Time Time::now() {
    Time ret;
    time_t raw = ::time(nullptr);
    struct tm t;
    localtime_r(&raw, &t);
    ret.hour = t.tm_hour;
    ret.minute = t.tm_min;
    ret.second = t.tm_sec;
    return ret;
}

bool DateTime::operator <(const DateTime &t) const {
    if (Date::operator<(t)) {
        return true;
    }
    if (Date::operator ==(t) && Time::operator<(t)) {
        return true;
    }
    return false;
}

bool DateTime::operator ==(const DateTime &t) const {
    return date == t.date && time == t.time;
}

bool DateTime::operator !=(const DateTime &t) const {
    return date != t.date || time != t.time;
}

bool DateTime::convertFromString(const str8 &s) {
    u32 y, m, d, h, n, sec;
    if (sscanf(s.c_str(), "%u-%u-%u %u:%u:%u", &y, &m, &d, &h, &n, &sec) != 6) {
        return false;
    }
    if (m >= 1 && m <= 12 && d >=1 && d <= 31 && h < 24 && n < 60 && sec < 60) {
        year = y;
        month = m - 1;
        day = d;
        hour = h;
        minute = n;
        second = sec;
        return true;
    }
    return false;
}

str8 DateTime::convertToString() const {
    c8 buf[64];
    return {buf, (size_t)sprintf(buf, "%u-%u-%u %u:%u:%u", year, month + 1, day, hour, minute, second)};
}

DateTime DateTime::now() {
    DateTime ret;
    time_t raw = ::time(nullptr);
    struct tm t;
    localtime_r(&raw, &t);
    ret.year = t.tm_year + 1900;
    ret.month = t.tm_mon;
    ret.day = t.tm_mday;
    ret.hour = t.tm_hour;
    ret.minute = t.tm_min;
    ret.second = t.tm_sec;
    return ret;
}
