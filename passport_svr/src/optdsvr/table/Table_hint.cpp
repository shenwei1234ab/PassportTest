#include "Table_hint.h"
#include "vnFileManager.h"

namespace tbl {
void Record_hint::load(DataStream &stream) {
	stream >> f_LevelId;
	stream >> f_WaveId;
	stream >> f_DelayTime;
	stream >> f_HintType;
	stream >> f_Picture;
	stream >> f_Title;
	stream >> f_Content1;
	stream >> f_Content2;
}

Table_hint::Table_hint()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_hint::~Table_hint() {
	fini();
}

bool Table_hint::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "hint.tbl";
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
bool Table_hint::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 8) {
		return false;
	}
	if(format[0].value != 3) {
		return false;
	}
	if(format[1].value != 3) {
		return false;
	}
	if(format[2].value != 4) {
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
	if(format[6].value != 1) {
		return false;
	}
	if(format[7].value != 1) {
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
	m_records = vnnew Record_hint[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_hint::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_hint::version() const {
	return m_version;
}

size_t Table_hint::size() const {
	return m_size;
}

const Record_hint * Table_hint::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

}
