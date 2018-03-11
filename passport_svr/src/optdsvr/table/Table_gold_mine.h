#ifndef __Table_gold_mine_H__
#define __Table_gold_mine_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_gold_mine : public AllocatedObject {
	Field_uint f_Id;
	Field_uint f_Type;
	Field_uint f_Capacity;
	Field_float f_Factor;
	int compare(u32 _Id) const;
	void load(DataStream &stream);
};

class Table_gold_mine{
public:
	Table_gold_mine();
	~Table_gold_mine();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_gold_mine * at(size_t index) const;

	size_t find(u32 _Id);
private:
	u32 m_version;
	Record_gold_mine *m_records;
	size_t m_size;
};
}
#endif
