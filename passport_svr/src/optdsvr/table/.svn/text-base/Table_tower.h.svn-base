#ifndef __Table_tower_H__
#define __Table_tower_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_tower : public AllocatedObject {
	Field_uint f_TowerID;
	Field_string f_TowerName;
	Field_uint f_TowerClass;
	Field_float f_RangeRd;
	Field_uint f_TargetNum;
	Field_uint f_TargetFilter;
	Field_uint f_Except;
	Field_int f_Cost;
	Field_uint f_BuildType;
	Field_string f_UpgradeID;
	Field_uint f_Level;
	Field_uint f_AtkType;
	Field_int f_MinAtk;
	Field_int f_MaxAtk;
	Field_int f_MinAtk2;
	Field_int f_MaxAtk2;
	Field_int f_MaxMana;
	Field_float f_ManaRegRate;
	Field_float f_CritRate;
	Field_float f_CritFactor;
	Field_uint f_CommonAttackID;
	Field_uint f_Flags;
	Field_string f_Animation;
	Field_uint f_Debutsound;
	Field_string f_explain;
	int compare(u32 _TowerID) const;
	void load(DataStream &stream);
};

class Table_tower{
public:
	Table_tower();
	~Table_tower();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_tower * at(size_t index) const;

	size_t find(u32 _TowerID);
private:
	u32 m_version;
	Record_tower *m_records;
	size_t m_size;
};
}
#endif
