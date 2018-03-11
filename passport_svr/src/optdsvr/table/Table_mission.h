#ifndef __Table_mission_H__
#define __Table_mission_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_mission : public AllocatedObject {
	Field_uint f_Id;
	Field_uint f_Type;
	Field_uint f_Task;
	Field_uint f_ParameterA;
	Field_uint f_ParameterB;
	Field_uint f_RewardType;
	Field_uint f_RewardValue;
	Field_string f_Name;
	Field_string f_Desc;
	int compare(u32 _Id) const;
	void load(DataStream &stream);
};

class Table_mission{
public:
	Table_mission();
	~Table_mission();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_mission * at(size_t index) const;

	size_t find(u32 _Id);
private:
	u32 m_version;
	Record_mission *m_records;
	size_t m_size;
};
}
#endif
