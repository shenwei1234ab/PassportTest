#include "Table_platform.h"
#include "vnFileManager.h"

namespace tbl {
int Record_platform::compare(u32 _Category) const {
	return f_Category.compare(_Category);
}

void Record_platform::load(DataStream &stream) {
	stream >> f_Category;
	stream >> f_BuffId;
	stream >> f_Price1;
	stream >> f_Price2;
	stream >> f_Price3;
	stream >> f_Price4;
	stream >> f_Price5;
	stream >> f_Price6;
	stream >> f_Price7;
	stream >> f_Price8;
	stream >> f_Price9;
	stream >> f_Title;
	stream >> f_Desc;
}

Table_platform::Table_platform()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_platform::~Table_platform() {
	fini();
}

bool Table_platform::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "platform.tbl";
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
bool Table_platform::init(DataStream &stream) {
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
	if(format[1].value != 1) {
		return false;
	}
	if(format[2].value != 2) {
		return false;
	}
	if(format[3].value != 2) {
		return false;
	}
	if(format[4].value != 2) {
		return false;
	}
	if(format[5].value != 2) {
		return false;
	}
	if(format[6].value != 2) {
		return false;
	}
	if(format[7].value != 2) {
		return false;
	}
	if(format[8].value != 2) {
		return false;
	}
	if(format[9].value != 2) {
		return false;
	}
	if(format[10].value != 2) {
		return false;
	}
	if(format[11].value != 1) {
		return false;
	}
	if(format[12].value != 1) {
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
	m_records = vnnew Record_platform[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_platform::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_platform::version() const {
	return m_version;
}

size_t Table_platform::size() const {
	return m_size;
}

const Record_platform * Table_platform::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_platform::find(u32 _Category) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_Category);
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
