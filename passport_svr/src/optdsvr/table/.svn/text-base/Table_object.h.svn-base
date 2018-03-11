#ifndef __Table_object_H__
#define __Table_object_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_object : public AllocatedObject {
	Field_uint f_ObjectId;
	Field_string f_ObjectName;
	Field_uint f_ObjectType;
	Field_float f_ObjectTime;
	Field_uint f_SpellId;
	Field_uint f_BuffId;
	Field_uint f_DestType;
	Field_int f_NumExtraTargets;
	Field_float f_Speed;
	Field_float f_SpeedVar;
	Field_float f_TurningRadius;
	Field_float f_FilterRadius;
	Field_uint f_FilterTargetType;
	Field_uint f_Except;
	Field_string f_Animation;
	Field_float f_RotationSpeed;
	Field_string f_Effects;
	int compare(u32 _ObjectId) const;
	void load(DataStream &stream);
};

class Table_object{
public:
	Table_object();
	~Table_object();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_object * at(size_t index) const;

	size_t find(u32 _ObjectId);
private:
	u32 m_version;
	Record_object *m_records;
	size_t m_size;
};
}
#endif
