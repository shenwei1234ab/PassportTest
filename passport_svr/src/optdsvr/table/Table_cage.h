#ifndef __Table_cage_H__
#define __Table_cage_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_cage : public AllocatedObject {
	Field_uint f_Id;
	Field_string f_Name;
	Field_int f_HP;
	Field_float f_CloseRange;
	Field_uint f_CloseSpellId;
	Field_uint f_CloseBuffId;
	Field_float f_OpenRange;
	Field_uint f_OpenSpellId;
	Field_uint f_OpenBuffId;
	Field_uint f_Flags;
	Field_string f_Animation;
	Field_string f_CloseExplain;
	Field_string f_OpenExplain;
	int compare(u32 _Id) const;
	void load(DataStream &stream);
};

class Table_cage{
public:
	Table_cage();
	~Table_cage();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_cage * at(size_t index) const;

	size_t find(u32 _Id);
private:
	u32 m_version;
	Record_cage *m_records;
	size_t m_size;
};
}
#endif
