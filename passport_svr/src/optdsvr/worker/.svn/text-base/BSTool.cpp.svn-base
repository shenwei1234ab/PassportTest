#include "BSTool.h"

//字符串分割函数
void StrSplit(const std::string& src, const std::string& separator, std::vector<std::string>& dest)
{
	if(src == "")
	{
		return ;
	}
	std::string str = src;
	std::string substring;
	std::string::size_type start = 0, index;

	do
	{
		index = str.find_first_of(separator, start);
		if (index != std::string::npos)
		{
			substring = str.substr(start, index - start);
			dest.push_back(substring);
			start = str.find_first_not_of(separator, index);
			if (start == std::string::npos) return;
		}
	} while (index != std::string::npos);

	//the last token
	substring = str.substr(start);
	dest.push_back(substring);
}




//英雄数组变成字符串		[1,2,3,4]	=>	"1,2,3,4"
void ConvertHeroArrayToString(std::vector<vn::u32> &sorHero,std::string &strDest)
{
	strDest="";
	std::stringstream ss;
	int size=sorHero.size();
	for(int i=0;i<size;++i)
	{
		std::string strHero;
		ss.clear();
		ss<<sorHero[i];
		ss >> strHero;
		strDest=strDest+strHero;
		if(i!=size-1)
		{
			strDest=strDest+",";
		}
	}
}


//高台数组变字符串[    <1,2> <2,2> <3,3>  ]=>"1,2|2,2|3,3"
void ConvertPlatformArrayToString(std::vector<std::pair<vn::u32, vn::u32>> &sorPlatforms,std::string &strDest)
{
	strDest="";
	std::stringstream ss;
	int size=sorPlatforms.size();
	for(int i=0;i<size;++i)
	{
		std::string strPlatId;
		std::string strPlatStatus;
		ss.clear();
		ss<<sorPlatforms[i].first;
		ss >> strPlatId;
		strDest=strDest+strPlatId+",";
		ss.clear();
		ss<<sorPlatforms[i].second;
		ss	>> strPlatStatus;
		strDest=strDest+strPlatStatus;
		if(i!=size-1)
		{
			strDest=strDest+"|";
		}
	}
}


vn::u32 ConvertStrToU32(std::string& strSource)
{
	std::stringstream ss;
	vn::u32 ret;
	ss<<strSource;
	ss>>ret;
	return ret;
}

void ConvertU32ToStr(vn::u32 number,std::string &strDest)
{
	strDest = "";
	std::stringstream ss;
	ss<<number;
	ss>>strDest;
}


//解析Json获取返回值
bool ParaseGetResult(vn::s32 *nResultCode,std::string& context)
{
	vn::VariableJSON variableJSON;
	vn::RefCountedPtr<vn::Variable_object> JSONPtr = variableJSON.parse(context.c_str(), context.length());
	if (!JSONPtr.get())
	{
		return false;
	}
	*nResultCode=0;
	auto attributes = JSONPtr->attributes();
	auto iter = attributes.find("ret");
	if(iter != attributes.end())
	{
		vn::VariablePtr var = iter->second;
		if(var->type() == vn::kVT_int32)
		{
			*nResultCode= var.dyn_cast<vn::Variable_int32>()->value();
		}
	}
	return true;
}

bool ParseResult(int *outResult,std::string& context)
{
	//解析context
	//s32 nResultCode=0;
	vn::Variable_int32 type;
	std::string strKey = "ret";
	if(!ParseJson(outResult,vn::kVT_int32,type,context,strKey))
	{

		return false;
	}
	return true;
}

bool ParseDiamond(int *outDiamond,std::string & context)
{
	vn::Variable_int32 type;
		std::string strKey = "balance";
		if(!ParseJson(outDiamond,vn::kVT_int32,type,context,strKey))
		{

			return false;
		}
		return true;
}
