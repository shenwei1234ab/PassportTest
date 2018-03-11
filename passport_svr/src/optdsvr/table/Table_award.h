#ifndef __Table_award_H__
#define __Table_award_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_award : public AllocatedObject {
	Field_uint f_AwardId;
	Field_uint f_GroupId;
	Field_uint f_PoolWeight;
	Field_uint f_Rank;
	Field_uint f_Weight;
	Field_uint f_ExtraWeight;
	Field_int f_Money;
	Field_int f_Diamonds;
	Field_string f_Items;
	Field_string f_Desc;
	Field_string f_AfterWin;
	Field_uint f_ImageId;
	int compare(u32 _AwardId) const;
	void load(DataStream &stream);
};

class Table_award{
public:
	Table_award();
	~Table_award();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_award * at(size_t index) const;

	size_t find(u32 _AwardId);
private:
	u32 m_version;
	Record_award *m_records;
	size_t m_size;
};
}
#endif
