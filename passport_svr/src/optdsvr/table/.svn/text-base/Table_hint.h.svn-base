#ifndef __Table_hint_H__
#define __Table_hint_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_hint : public AllocatedObject {
	Field_uint f_LevelId;
	Field_uint f_WaveId;
	Field_float f_DelayTime;
	Field_uint f_HintType;
	Field_string f_Picture;
	Field_string f_Title;
	Field_string f_Content1;
	Field_string f_Content2;
	void load(DataStream &stream);
};

class Table_hint{
public:
	Table_hint();
	~Table_hint();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_hint * at(size_t index) const;

private:
	u32 m_version;
	Record_hint *m_records;
	size_t m_size;
};
}
#endif
