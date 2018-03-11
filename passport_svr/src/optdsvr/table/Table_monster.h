#ifndef __Table_monster_H__
#define __Table_monster_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_monster : public AllocatedObject {
	Field_uint f_MonsterID;
	Field_string f_MonsterName;
	Field_uint f_MonsterType;
	Field_uint f_MoveType;
	Field_uint f_Tags;
	Field_float f_Speed;
	Field_float f_PhysicalRes;
	Field_float f_MagicRes;
	Field_uint f_HP;
	Field_uint f_GoldKill;
	Field_uint f_FruitMax;
	Field_string f_MonsterSpell;
	Field_string f_StartBuff;
	Field_uint f_WaterBuff;
	Field_string f_Animation;
	Field_uint f_DieSoundID;
	Field_uint f_WaveTip;
	Field_uint f_Flags;
	Field_string f_Behavior;
	Field_string f_Content1;
	Field_string f_Content2;
	int compare(u32 _MonsterID) const;
	void load(DataStream &stream);
};

class Table_monster{
public:
	Table_monster();
	~Table_monster();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_monster * at(size_t index) const;

	size_t find(u32 _MonsterID);
private:
	u32 m_version;
	Record_monster *m_records;
	size_t m_size;
};
}
#endif
