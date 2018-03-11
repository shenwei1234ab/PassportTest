#include "Table_levelclass.h"
#include "vnFileManager.h"

namespace tbl {
int Record_levelclass::compare(u32 _ID) const {
	return f_ID.compare(_ID);
}

void Record_levelclass::load(DataStream &stream) {
	stream >> f_ID;
	stream >> f_Reward1Type;
	stream >> f_Reward1Value;
	stream >> f_Reward2Type;
	stream >> f_Reward2Value;
	stream >> f_Reward3Type;
	stream >> f_Reward3Value;
	stream >> f_Reward4Type;
	stream >> f_Reward4Value;
	stream >> f_Reward5Type;
	stream >> f_Reward5Value;
	stream >> f_MineId;
	stream >> f_MineValue;
}

Table_levelclass::Table_levelclass()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_levelclass::~Table_levelclass() {
	fini();
}

bool Table_levelclass::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "levelclass.tbl";
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
bool Table_levelclass::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 13) {
		return false;
	}
	if(format[0].value != 131) {
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
	if(format[4].value != 3) {
		return false;
	}
	if(format[5].value != 3) {
		return false;
	}
	if(format[6].value != 3) {
		return false;
	}
	if(format[7].value != 3) {
		return false;
	}
	if(format[8].value != 3) {
		return false;
	}
	if(format[9].value != 3) {
		return false;
	}
	if(format[10].value != 3) {
		return false;
	}
	if(format[11].value != 3) {
		return false;
	}
	if(format[12].value != 3) {
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
	m_records = vnnew Record_levelclass[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_levelclass::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_levelclass::version() const {
	return m_version;
}

size_t Table_levelclass::size() const {
	return m_size;
}

const Record_levelclass * Table_levelclass::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_levelclass::find(u32 _ID) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_ID);
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
