#ifndef __Table_Registration_H__
#define __Table_Registration_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_Registration : public AllocatedObject {
	Field_uint f_Id;
	Field_uint f_StepId;
	Field_uint f_Theprops;
	Field_int f_Number;
	int compare(u32 _Id,u32 _StepId) const;
	void load(DataStream &stream);
};

class Table_Registration{
public:
	Table_Registration();
	~Table_Registration();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_Registration * at(size_t index) const;

	size_t find(u32 _Id,u32 _StepId);
private:
	u32 m_version;
	Record_Registration *m_records;
	size_t m_size;
};
}
#endif
