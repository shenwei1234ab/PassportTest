#ifndef __Table_buff_H__
#define __Table_buff_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_buff : public AllocatedObject {
	Field_uint f_BuffID;
	Field_string f_BuffName;
	Field_uint f_Flag;
	Field_float f_Duration;
	Field_uint f_SpellID;
	Field_uint f_EffectType;
	Field_uint f_SubEffType;
	Field_int f_Value;
	Field_uint f_Trigger;
	Field_int f_TriggerRate;
	Field_string f_TriggerSpell;
	Field_uint f_BuffClass;
	Field_int f_BuffLevel;
	Field_uint f_RemoveFlags;
	Field_uint f_Effect;
	Field_string f_Action;
	int compare(u32 _BuffID) const;
	void load(DataStream &stream);
};

class Table_buff{
public:
	Table_buff();
	~Table_buff();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_buff * at(size_t index) const;

	size_t find(u32 _BuffID);
private:
	u32 m_version;
	Record_buff *m_records;
	size_t m_size;
};
}
#endif
