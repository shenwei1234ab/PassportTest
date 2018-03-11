/*******************************************************************************
@ 版权所有(C) Tencent 2012
********************************************************************************/
#ifndef __GAMESVR_MACRODEF_MACRO_H__
#define __GAMESVR_MACRODEF_MACRO_H__

#include "tranbase/macrodef/UtilMacro.h"
#include "tranbase/Util.h"

//定义MacroGameSvr的宏
#define MACRODEF(macroval) MacroGamesvr##macroval,

enum GameSvrMacro
{
//将GameSvr、RedUtil和Util的宏都定义和赋值
#include "GamesvrMacro.def"
#include "tranbase/macrodef/UtilMacro.def"
};

#undef MACRODEF
//使用的时候通过MACROGAMESVR名字来用
#define MACROGAMESVR(macroval) MacroGamesvr##macroval


#endif // end of __GAMESVR_MACRODEF_MACRO_H__
