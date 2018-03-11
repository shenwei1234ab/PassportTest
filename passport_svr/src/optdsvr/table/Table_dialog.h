#ifndef __Table_dialog_H__
#define __Table_dialog_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_dialog : public AllocatedObject {
	Field_uint f_LevelID;
	Field_uint f_WaveID;
	Field_uint f_DialogID;
	Field_string f_Position1;
	Field_uint f_Position1SP;
	Field_uint f_Position1EM;
	Field_string f_Position2;
	Field_uint f_Position2SP;
	Field_uint f_Position2EM;
	Field_string f_Position3;
	Field_uint f_Position3SP;
	Field_uint f_Position3EM;
	Field_string f_Position4;
	Field_uint f_Position4SP;
	Field_uint f_Position4EM;
	Field_string f_Position5;
	Field_uint f_Position5SP;
	Field_uint f_Position5EM;
	Field_string f_Text;
	Field_string f_Namelabel;
	Field_string f_Background;
	Field_string f_Sound;
	Field_uint f_Flag;
	int compare(u32 _LevelID,u32 _WaveID,u32 _DialogID) const;
	void load(DataStream &stream);
};

class Table_dialog{
public:
	Table_dialog();
	~Table_dialog();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_dialog * at(size_t index) const;

	size_t find(u32 _LevelID,u32 _WaveID,u32 _DialogID);
private:
	u32 m_version;
	Record_dialog *m_records;
	size_t m_size;
};
}
#endif
