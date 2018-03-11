#ifndef __Table_sub_wave_H__
#define __Table_sub_wave_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_sub_wave : public AllocatedObject {
	Field_uint f_SubWaveId;
	Field_uint f_SpawnPointId;
	Field_uint f_MonsterId;
	Field_float f_StartTime;
	Field_uint f_NumGroups;
	Field_uint f_NumMonstersPerGroup;
	Field_float f_GroupIntervalTime;
	Field_float f_MonsterIntervalTime;
	int compare(u32 _SubWaveId) const;
	void load(DataStream &stream);
};

class Table_sub_wave{
public:
	Table_sub_wave();
	~Table_sub_wave();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_sub_wave * at(size_t index) const;

	size_t find(u32 _SubWaveId);
private:
	u32 m_version;
	Record_sub_wave *m_records;
	size_t m_size;
};
}
#endif
