#ifndef __Table_item_store_H__
#define __Table_item_store_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_item_store : public AllocatedObject {
	Field_uint f_id;
	Field_uint f_price;
	int compare(u32 _id) const;
	void load(DataStream &stream);
};

class Table_item_store{
public:
	Table_item_store();
	~Table_item_store();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_item_store * at(size_t index) const;

	size_t find(u32 _id);
private:
	u32 m_version;
	Record_item_store *m_records;
	size_t m_size;
};
}
#endif
