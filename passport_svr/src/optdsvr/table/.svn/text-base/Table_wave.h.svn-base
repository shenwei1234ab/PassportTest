#ifndef __Table_wave_H__
#define __Table_wave_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_wave : public AllocatedObject {
	Field_uint f_LevelId;
	Field_uint f_WaveId;
	Field_float f_BeginTime;
	Field_float f_MaxWaitingTime;
	Field_string f_SubWaves;
	Field_uint f_Music;
	int compare(u32 _LevelId,u32 _WaveId) const;
	void load(DataStream &stream);
};

class Table_wave{
public:
	Table_wave();
	~Table_wave();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_wave * at(size_t index) const;

	size_t find(u32 _LevelId,u32 _WaveId);
private:
	u32 m_version;
	Record_wave *m_records;
	size_t m_size;
};
}
#endif
