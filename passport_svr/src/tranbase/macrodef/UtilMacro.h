#ifndef __UTIL_UTILMACRODEF_UTILMACRO_H__
#define __UTIL_UTILMACRODEF_UTILMACRO_H__

#include "tranbase/Util.h"
//这里只声明MacroUtil 赋值在GameSvrMacro中
#define MACRODEF(macroval) extern const uint32_t MacroUtil##macroval;


#include "tranbase/macrodef/UtilMacro.def"


#undef MACRODEF

const char* GetMacroName(uint32_t macro);
//使用的时候通过MACRUTIL名字来用
#define MACROUTIL(macroval) MacroUtil##macroval


#endif // end of __UTIL_UTILMACRODEF_UTILMACRO_H__

