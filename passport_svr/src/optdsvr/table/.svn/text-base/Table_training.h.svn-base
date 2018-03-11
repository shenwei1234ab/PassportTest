#ifndef __Table_training_H__
#define __Table_training_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_training : public AllocatedObject {
	Field_uint f_Class;
	Field_uint f_Slot;
	Field_uint f_AdvancedTimes;
	Field_uint f_TowerId;
	Field_uint f_CostType;
	Field_uint f_CostValue;
	Field_uint f_ChipType;
	Field_uint f_ChipValue;
	Field_string f_SkillId1;
	Field_string f_Skillcost1;
	Field_string f_SkillId2;
	Field_string f_Skillcost2;
	Field_string f_SkillId3;
	Field_string f_Skillcost3;
	Field_string f_SkillName1;
	Field_string f_SkillDesc1;
	Field_string f_SkillName2;
	Field_string f_SkillDesc2;
	Field_string f_SkillName3;
	Field_string f_SkillDesc3;
	int compare(u32 _Class,u32 _Slot,u32 _AdvancedTimes) const;
	void load(DataStream &stream);
};

class Table_training{
public:
	Table_training();
	~Table_training();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_training * at(size_t index) const;

	size_t find(u32 _Class,u32 _Slot,u32 _AdvancedTimes);
private:
	u32 m_version;
	Record_training *m_records;
	size_t m_size;
};
}
#endif
