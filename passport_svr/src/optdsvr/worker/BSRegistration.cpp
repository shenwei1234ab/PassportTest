#include "BSRegistration.h"
#include "../manager/DC_DataBase.h"
#include "LoggedUser.h"

void BSRegistration::save(DataStream &stream) const{
    stream << DataStream::ccu(m_firstDate.date) << DataStream::ccu(m_lastDate.date) << DataStream::ccu(m_nums) << DataStream::ccu(m_group);

}

void BSRegistration::load(u8 version, DataStream &stream) {
    stream >> DataStream::cu(m_firstDate.date) >> DataStream::cu(m_lastDate.date) >> DataStream::cu(m_nums) >> DataStream::cu(m_group);
}

void BSRegistration::save(Variable_object &vobj) const {
    auto p = vnnew Variable_object();
    vobj.setAttribute("login_rewards", p);
    p->setAttribute("first_date",vnnew Variable_string( m_firstDate.convertToString()));
    p->setAttribute("last_date", vnnew Variable_string(m_lastDate.convertToString()) );
    p->setAttribute("count", vnnew Variable_int32(m_nums));
    p->setAttribute("group", vnnew Variable_int32(m_group));
}

void BSRegistration::Asyncsave(std::function<void(bool ret)> doResult)
{
	std::string strUUId= m_user->GetUUId();
	std::string firstDate =m_firstDate.convertToString();
	std::string lastDate = m_lastDate.convertToString();
	 ((DC_DataBase *)&DataCenter::instance())->AsyncUpdateBaseInfo_Registion({m_user,true},strUUId,firstDate,lastDate,m_nums,m_group,doResult);
}


void BSRegistration::load(Variable_object &vobj) {
    auto p = vobj.queryAttributeObject("login_rewards");
    if (!p) {
        return ;
    }
    auto vstr = p->queryAttributeStringV("first_date");
    if (vstr) {
        m_firstDate.convertFromString(vstr->value());
    }
    vstr= p->queryAttributeStringV("last_date");
    if (vstr) {
        m_lastDate.convertFromString(vstr->value());
    }
    m_nums = p->queryAttributeInt32("count");
    m_group = p->queryAttributeInt32("group");
}


void BSRegistration::load(PreparedQueryResult baseinfo)
{
	m_nums=(*baseinfo)[19].GetUInt32();
	m_firstDate.convertFromString((*baseinfo)[20].GetString());
	m_group=(*baseinfo)[21].GetUInt32();
	m_lastDate.convertFromString((*baseinfo)[22].GetString());
}













void BSRegistration::reset() {
    m_firstDate.year = 0;
    m_lastDate.year = 0;
    m_nums = 0;
    m_group = 0;
}
