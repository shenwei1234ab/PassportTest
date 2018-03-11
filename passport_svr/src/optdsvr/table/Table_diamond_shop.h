#ifndef __Table_diamond_shop_H__
#define __Table_diamond_shop_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_diamond_shop : public AllocatedObject {
	Field_uint f_Id;
	Field_uint f_DiamondCost;
	Field_uint f_GoldValue;
	int compare(u32 _Id) const;
	void load(DataStream &stream);
};

class Table_diamond_shop{
public:
	Table_diamond_shop();
	~Table_diamond_shop();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_diamond_shop * at(size_t index) const;

	size_t find(u32 _Id);
private:
	u32 m_version;
	Record_diamond_shop *m_records;
	size_t m_size;
};
}
#endif
