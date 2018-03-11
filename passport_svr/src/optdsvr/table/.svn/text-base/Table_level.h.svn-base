#ifndef __Table_level_H__
#define __Table_level_H__

#include "TableBase.h"
#include "vnFilePath.h"
#include "vnRefCounted.h"
namespace tbl {
struct Record_level : public RefCounted {
	Field_uint f_LevelId;
	Field_uint f_PreType;
	Field_uint f_PreId;
	Field_uint f_LevelGroupId;
	Field_string f_MapFile;
	Field_uint f_CouponId;
	Field_int f_InitGold;
	Field_uint f_NumGoldTrees;
	Field_int f_GoldTreeValue;
	Field_string f_RedTowerId;
	Field_string f_GreenTowerId;
	Field_string f_WhiteTowerId;
	Field_uint f_FirstPassRewardType;
	Field_uint f_FirstPassRewardValue;
	Field_uint f_FirstChipType;
	Field_uint f_FirstChipValue;
	Field_uint f_MineId;
	Field_uint f_MineValue;
	Field_uint f_Task1Type;
	Field_uint f_Reward1Type;
	Field_uint f_Reward1Value;
	Field_uint f_Reward1ChipType;
	Field_uint f_Reward1ChipValue;
	Field_uint f_Task2Type;
	Field_uint f_Reward2Type;
	Field_uint f_Reward2Value;
	Field_uint f_Reward2ChipType;
	Field_uint f_Reward2ChipValue;
	Field_uint f_Task3Type;
	Field_uint f_Reward3Type;
	Field_uint f_Reward3Value;
	Field_uint f_Reward3ChipType;
	Field_uint f_Reward3ChipValue;
	Field_uint f_Music0;
	Field_uint f_Music1;
	int compare(u32 _LevelId) const;
	void load(DataStream &stream);
};

class Table_level{
public:
	Table_level();
	~Table_level();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_level * at(size_t index) const;

	size_t find(u32 _LevelId);
private:
	u32 m_version;
	Record_level *m_records;
	size_t m_size;
};
}
#endif
