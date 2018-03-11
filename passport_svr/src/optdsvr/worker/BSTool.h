#ifndef __optd_srv__BSTool__
#define __optd_srv__BSTool__
#include "vector"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include "vnRefCounted.h"
#include "vnVariableJSON.h"
void StrSplit(const std::string& src, const std::string& separator, std::vector<std::string>& dest);
void ConvertHeroArrayToString( std::vector<vn::u32> &sorHero,std::string &strDest);
void ConvertPlatformArrayToString(std::vector<std::pair<vn::u32, vn::u32>> &sorPlatforms,std::string &strDest);
vn::u32 ConvertStrToU32(std::string& strSource);
void ConvertU32ToStr(vn::u32 number,std::string &strDest);
bool ParaseGetResult(vn::s32 *nResultCode,std::string& context);
bool ParseResult(int *outResult,std::string& context);
bool ParseDiamond(int *outDiamond,std::string & context);
template <typename T>
void ConvertToStr(T value,std::string &strDest)
{
	strDest = "";
	std::stringstream ss;
	ss<<value;
	ss>>strDest;
}


template <typename T1,typename T2>
bool ParseJson(T1 *value,vn::VariableType type,T2 valType ,std::string& context,std::string &key)
{
	vn::VariableJSON variableJSON;
	vn::RefCountedPtr<vn::Variable_object> JSONPtr = variableJSON.parse(context.c_str(), context.length());
	if (!JSONPtr.get())
	{
		return false;
	}
	*value=0;
	auto attributes = JSONPtr->attributes();
	auto iter = attributes.find(key);
	if(iter != attributes.end())
	{
		vn::VariablePtr var = iter->second;
		if(var->type() == type)
		{
			*value= var.dyn_cast<T2>()->value();
		}
	}
	return true;
}





#endif
