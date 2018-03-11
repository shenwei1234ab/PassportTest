#include "Table_progress.h"
#include "vnFileManager.h"

namespace tbl {
void Record_progress::load(DataStream &stream) {
	stream >> f_UnlockType;
	stream >> f_SubType;
	stream >> f_LevelId;
	stream >> f_WaveId;
	stream >> f_Explain;
	stream >> f_UnLockTip;
}

Table_progress::Table_progress()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_progress::~Table_progress() {
	fini();
}

bool Table_progress::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "progress.tbl";
	FileStream* fs = FileManager::instance().open(name);
	if(!fs) {
		return false;
	}
	bool ret = false;
	try {
		ret = init(*fs);
	} catch (DataStream::Exception& ) {

	}
	fs->close();
	return ret;
}
bool Table_progress::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 6) {
		return false;
	}
	if(format[0].value != 3) {
		return false;
	}
	if(format[1].value != 3) {
		return false;
	}
	if(format[2].value != 3) {
		return false;
	}
	if(format[3].value != 3) {
		return false;
	}
	if(format[4].value != 1) {
		return false;
	}
	if(format[5].value != 1) {
		return false;
	}
	stream >> m_version;
#ifdef VN_2D
	DataStream::z_u32 numRecords;
	stream >> numRecords;
	m_size = numRecords.value;
#else
	_read_z_u32(stream, m_size);
#endif
	if(m_size == 0) {
		return true;
	}
	m_records = vnnew Record_progress[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_progress::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_progress::version() const {
	return m_version;
}

size_t Table_progress::size() const {
	return m_size;
}

const Record_progress * Table_progress::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

}
