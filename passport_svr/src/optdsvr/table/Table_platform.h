#ifndef __Table_platform_H__
#define __Table_platform_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_platform : public AllocatedObject {
	Field_uint f_Category;
	Field_string f_BuffId;
	Field_int f_Price1;
	Field_int f_Price2;
	Field_int f_Price3;
	Field_int f_Price4;
	Field_int f_Price5;
	Field_int f_Price6;
	Field_int f_Price7;
	Field_int f_Price8;
	Field_int f_Price9;
	Field_string f_Title;
	Field_string f_Desc;
	int compare(u32 _Category) const;
	void load(DataStream &stream);
};

class Table_platform{
public:
	Table_platform();
	~Table_platform();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_platform * at(size_t index) const;

	size_t find(u32 _Category);
private:
	u32 m_version;
	Record_platform *m_records;
	size_t m_size;
};
}
#endif
