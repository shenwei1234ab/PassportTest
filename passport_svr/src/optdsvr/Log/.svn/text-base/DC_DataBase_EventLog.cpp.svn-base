#include "DC_DataBase_EventLog.h"
#include "../worker/BSTool.h"
#include "../manager/DC_DataBase.h"
void DC_DataBase_EventLog_User::Asyncsave(AsyncObjectPtr obj,std::function<void(bool ret)> doResult)
{
	//异步save
	((DC_DataBase *)&DataCenter::instance())->AsyncInsertLog(obj,strUUId,(u32)type(),strMessage,doResult);
}


void DC_DataBase_EventLog_User_Signin::Asyncsave(AsyncObjectPtr obj,std::function<void(bool ret)>doResult)
{
	strMessage = "Signin";
	DC_DataBase_EventLog_User::Asyncsave(obj,doResult);
}

void DC_DataBase_EventLog_User_Login::Asyncsave(AsyncObjectPtr obj,std::function<void(bool ret)> doResult)
{
	strMessage = "Login";
	DC_DataBase_EventLog_User::Asyncsave(obj,doResult);
}


void DC_DataBase_EventLog_User_Logout::Asyncsave(AsyncObjectPtr obj,std::function<void(bool ret)> doResult)
{
	strMessage = "Loginout";
	DC_DataBase_EventLog_User::Asyncsave(obj,doResult);
}



void	DC_DataBase_EventLog_User_BeginBattle::Asyncsave(AsyncObjectPtr obj,std::function<void(bool ret)>doResult)
{
	std::string strLevelId ;
	ConvertToStr(levelId,strLevelId);
	std::string strIntrusion = intrusion==true?"true":"false";
	if(!intrusion)
	{
		strIntrusion = "false";
	}
	strMessage = "levelId:"+	strLevelId+"intrusion:"+strIntrusion;
	DC_DataBase_EventLog_User::Asyncsave(obj,doResult);
}






void 	DC_DataBase_EventLog_User_EndBattle::Asyncsave(AsyncObjectPtr obj,std::function<void(bool ret)>doResult)
{
	std::string strLevelId ;
	std::string strIntrusion = intrusion==true?"true":"false";
	std::string strStars ;
	std::string strShovel = shovel==true?"true":"false";
	std::string strWatch = watch==true?"true":"false";
	ConvertToStr(levelId,strLevelId);
	ConvertToStr(stars,strStars);
	strMessage="levelId:"+strLevelId+"intrusion:"+strIntrusion+"stars:"+strStars+"shovel:"+strShovel+"watch:"+strWatch;
	DC_DataBase_EventLog_User::Asyncsave(obj,doResult);
}



