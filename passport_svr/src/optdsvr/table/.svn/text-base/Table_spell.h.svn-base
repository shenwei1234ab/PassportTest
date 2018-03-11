#ifndef __Table_spell_H__
#define __Table_spell_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_spell : public AllocatedObject {
	Field_uint f_SpellId;
	Field_string f_SpellName;
	Field_float f_CDTime;
	Field_int f_CostMana;
	Field_uint f_TargetSource;
	Field_uint f_DamageType;
	Field_float f_DamagePercent;
	Field_int f_DamageBase;
	Field_float f_HealPercent;
	Field_int f_Heal;
	Field_string f_ObjectDest;
	Field_uint f_ObjectId;
	Field_string f_BuffId;
	Field_int f_Gold;
	Field_float f_SummonDistance;
	Field_uint f_SummonId;
	Field_float f_MoveDistance;
	Field_float f_Scan;
	Field_int f_IncDigger;
	Field_uint f_TriggeredSpellId;
	Field_uint f_PositionSource;
	Field_uint f_RangeType;
	Field_float f_RangeValue1;
	Field_float f_RangeValue2;
	Field_uint f_RangeNumTargets;
	Field_uint f_RangeTargetType;
	Field_uint f_RangeSpellId;
	Field_uint f_FilterNumTargets;
	Field_uint f_FilterTargetType;
	Field_float f_FilterRadius;
	Field_uint f_Except;
	Field_uint f_Flags;
	Field_string f_Action;
	Field_string f_explain;
	Field_uint f_CastEffect;
	Field_uint f_HitEffect;
	Field_uint f_LinkCasterEffect;
	Field_uint f_LinkTargetEffect;
	int compare(u32 _SpellId) const;
	void load(DataStream &stream);
};

class Table_spell{
public:
	Table_spell();
	~Table_spell();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_spell * at(size_t index) const;

	size_t find(u32 _SpellId);
private:
	u32 m_version;
	Record_spell *m_records;
	size_t m_size;
};
}
#endif
