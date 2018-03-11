#ifndef __Table_levelclass_H__
#define __Table_levelclass_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_levelclass : public AllocatedObject {
	Field_uint f_ID;
	Field_uint f_Reward1Type;
	Field_uint f_Reward1Value;
	Field_uint f_Reward2Type;
	Field_uint f_Reward2Value;
	Field_uint f_Reward3Type;
	Field_uint f_Reward3Value;
	Field_uint f_Reward4Type;
	Field_uint f_Reward4Value;
	Field_uint f_Reward5Type;
	Field_uint f_Reward5Value;
	Field_uint f_MineId;
	Field_uint f_MineValue;
	int compare(u32 _ID) const;
	void load(DataStream &stream);
};

class Table_levelclass{
public:
	Table_levelclass();
	~Table_levelclass();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_levelclass * at(size_t index) const;

	size_t find(u32 _ID);
private:
	u32 m_version;
	Record_levelclass *m_records;
	size_t m_size;
};
}
#endif
