/*
 * Util.h
 *
 *  Created on: 2015年11月2日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_TRANBASE_UTIL_H_
#define SRC_PASSPORTSVR_TRANBASE_UTIL_H_

#include <cstdlib>
#ifndef WIN32
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdint.h>
#include <inttypes.h>
#endif
#include <ctime>

#include <assert.h>
#include <vector>
#include "tranbase/Macrodef.h"
#include <set>
#include <map>
#include <algorithm>
#include <string.h>

#define GCC_VERSION (__GNUC__ * 10000 \
    + __GNUC_MINOR__ * 100 \
    + __GNUC_PATCHLEVEL__)

#define MULTISETDEFINE(key) std::multiset<key,std::less<key>>
#define SETDEFINE(key) std::set<key,std::less<key>>
#define MAPDEFINE(key,type) std::map<key,type,std::less<key>/>
#define MULTIMAPDEFINE(key,type) std::multimap<key,type,std::less<key>>

#include <tr1/unordered_map>
#include <tr1/unordered_set>
#define HASHMAPDEFINE(key,type) std::tr1::unordered_map<key,type,std::tr1::hash<key>>
#define HASHMULTIMAPDEFINE(key,type) std::tr1::unordered_multimap<key,type,std::tr1::hash<key>,std::equal_to<key>>
#define HASHSETDEFINE(key) std::tr1::unordered_set<key,std::tr1::hash<key>,std::equal_to<key>>
#define HASHMULTISETDEFINE(key) std::tr1::unordered_multiset<key,std::tr1::hash<key>,std::equal_to<key>>


int StrListToIntArray(const char* pSrcStr, int* pDesArray, int Max, const char* cSplitChar);

#endif /* SRC_PASSPORTSVR_TRANBASE_UTIL_H_ */
