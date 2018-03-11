#ifndef __Table_progress_H__
#define __Table_progress_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_progress : public AllocatedObject {
	Field_uint f_UnlockType;
	Field_uint f_SubType;
	Field_uint f_LevelId;
	Field_uint f_WaveId;
	Field_string f_Explain;
	Field_string f_UnLockTip;
	void load(DataStream &stream);
};

class Table_progress{
public:
	Table_progress();
	~Table_progress();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_progress * at(size_t index) const;

private:
	u32 m_version;
	Record_progress *m_records;
	size_t m_size;
};
}
#endif
