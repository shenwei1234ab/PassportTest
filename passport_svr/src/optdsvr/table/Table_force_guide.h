#ifndef __Table_force_guide_H__
#define __Table_force_guide_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_force_guide : public AllocatedObject {
	Field_uint f_GroupId;
	Field_uint f_StepId;
	Field_uint f_CondType;
	Field_string f_CondValue;
	Field_uint f_SceneAction;
	Field_string f_SceneArea;
	Field_uint f_UIAction;
	Field_uint f_UIAreaName;
	Field_string f_Portrait;
	Field_string f_ShowText;
	Field_string f_AddItems;
	Field_uint f_Flags;
	int compare(u32 _GroupId,u32 _StepId) const;
	void load(DataStream &stream);
};

class Table_force_guide{
public:
	Table_force_guide();
	~Table_force_guide();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_force_guide * at(size_t index) const;

	size_t find(u32 _GroupId,u32 _StepId);
private:
	u32 m_version;
	Record_force_guide *m_records;
	size_t m_size;
};
}
#endif
