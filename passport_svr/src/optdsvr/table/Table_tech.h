#ifndef __Table_tech_H__
#define __Table_tech_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_tech : public AllocatedObject {
	Field_uint f_Slot;
	Field_uint f_Rank;
	Field_uint f_Type;
	Field_int f_Value1;
	Field_int f_Value2;
	Field_uint f_CostType;
	Field_uint f_CostValue;
	Field_uint f_CostTime;
	Field_string f_Explain;
	Field_string f_Desc1;
	Field_string f_Desc2;
	int compare(u32 _Slot,u32 _Rank) const;
	void load(DataStream &stream);
};

class Table_tech{
public:
	Table_tech();
	~Table_tech();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_tech * at(size_t index) const;

	size_t find(u32 _Slot,u32 _Rank);
private:
	Record_tech *m_records;
	u32 m_version;
	size_t m_size;
};
}
#endif
