#ifndef __Table_herorune_H__
#define __Table_herorune_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_herorune : public AllocatedObject {
	Field_uint f_RuneClass;
	Field_uint f_RuneRank;
	Field_uint f_Level;
	Field_uint f_CostType;
	Field_uint f_CostValue;
	Field_uint f_ActiveSpellID;
	Field_uint f_PassiveSpellID;
	Field_string f_Explain;
	int compare(u32 _RuneClass,u32 _RuneRank) const;
	void load(DataStream &stream);
};

class Table_herorune{
public:
	Table_herorune();
	~Table_herorune();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_herorune * at(size_t index) const;

	size_t find(u32 _RuneClass,u32 _RuneRank);
private:
	u32 m_version;
	Record_herorune *m_records;
	size_t m_size;
};
}
#endif
