#include "tranbase/macrodef/UtilMacro.h"

#include <algorithm>


extern const char* MacroNameArray[];


const char* GetMacroName(uint32_t macro)
{
	if( macro >= MACROUTIL(UtilEnd) ) {return "Invalid Macro Id!";}
	return MacroNameArray[macro];
}

