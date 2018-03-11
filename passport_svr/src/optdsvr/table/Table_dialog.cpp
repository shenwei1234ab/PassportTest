#include "Table_dialog.h"
#include "vnFileManager.h"

namespace tbl {
int Record_dialog::compare(u32 _LevelID,u32 _WaveID,u32 _DialogID) const {
	int ret;
	ret = f_LevelID.compare(_LevelID);
	if (ret) {
		return ret;
	}
	ret = f_WaveID.compare(_WaveID);
	if (ret) {
		return ret;
	}
	ret = f_DialogID.compare(_DialogID);
	if (ret) {
		return ret;
	}
	return 0;
}

void Record_dialog::load(DataStream &stream) {
	stream >> f_LevelID;
	stream >> f_WaveID;
	stream >> f_DialogID;
	stream >> f_Position1;
	stream >> f_Position1SP;
	stream >> f_Position1EM;
	stream >> f_Position2;
	stream >> f_Position2SP;
	stream >> f_Position2EM;
	stream >> f_Position3;
	stream >> f_Position3SP;
	stream >> f_Position3EM;
	stream >> f_Position4;
	stream >> f_Position4SP;
	stream >> f_Position4EM;
	stream >> f_Position5;
	stream >> f_Position5SP;
	stream >> f_Position5EM;
	stream >> f_Text;
	stream >> f_Namelabel;
	stream >> f_Background;
	stream >> f_Sound;
	stream >> f_Flag;
}

Table_dialog::Table_dialog()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_dialog::~Table_dialog() {
	fini();
}

bool Table_dialog::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "dialog.tbl";
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
bool Table_dialog::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 23) {
		return false;
	}
	if(format[0].value != 131) {
		return false;
	}
	if(format[1].value != 131) {
		return false;
	}
	if(format[2].value != 131) {
		return false;
	}
	if(format[3].value != 1) {
		return false;
	}
	if(format[4].value != 3) {
		return false;
	}
	if(format[5].value != 3) {
		return false;
	}
	if(format[6].value != 1) {
		return false;
	}
	if(format[7].value != 3) {
		return false;
	}
	if(format[8].value != 3) {
		return false;
	}
	if(format[9].value != 1) {
		return false;
	}
	if(format[10].value != 3) {
		return false;
	}
	if(format[11].value != 3) {
		return false;
	}
	if(format[12].value != 1) {
		return false;
	}
	if(format[13].value != 3) {
		return false;
	}
	if(format[14].value != 3) {
		return false;
	}
	if(format[15].value != 1) {
		return false;
	}
	if(format[16].value != 3) {
		return false;
	}
	if(format[17].value != 3) {
		return false;
	}
	if(format[18].value != 1) {
		return false;
	}
	if(format[19].value != 1) {
		return false;
	}
	if(format[20].value != 1) {
		return false;
	}
	if(format[21].value != 1) {
		return false;
	}
	if(format[22].value != 3) {
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
	m_records = vnnew Record_dialog[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_dialog::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_dialog::version() const {
	return m_version;
}

size_t Table_dialog::size() const {
	return m_size;
}

const Record_dialog * Table_dialog::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_dialog::find(u32 _LevelID,u32 _WaveID,u32 _DialogID) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_LevelID,_WaveID,_DialogID);
		if(ret > 0) {
			high = mid - 1;
			continue;
		}
		if (ret < 0) {
			low = mid + 1;
			continue;
		}
		return mid;
	}
	return -1;
}
}
