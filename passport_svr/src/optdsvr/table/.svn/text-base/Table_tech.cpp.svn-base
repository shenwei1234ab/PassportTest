#include "Table_tech.h"
#include "vnFileManager.h"

namespace tbl {
int Record_tech::compare(u32 _Slot,u32 _Rank) const {
	int ret;
	ret = f_Slot.compare(_Slot);
	if (ret) {
		return ret;
	}
	ret = f_Rank.compare(_Rank);
	if (ret) {
		return ret;
	}
	return 0;
}

void Record_tech::load(DataStream &stream) {
	stream >> f_Slot;
	stream >> f_Rank;
	stream >> f_Type;
	stream >> f_Value1;
	stream >> f_Value2;
	stream >> f_CostType;
	stream >> f_CostValue;
	stream >> f_CostTime;
	stream >> f_Explain;
	stream >> f_Desc1;
	stream >> f_Desc2;
}

Table_tech::Table_tech()
:m_records(0)
,m_version(0)
,m_size(0) {
}

Table_tech::~Table_tech() {
	fini();
}

bool Table_tech::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "tech.tbl";
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
bool Table_tech::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 11) {
		return false;
	}
	if(format[0].value != 131) {
		return false;
	}
	if(format[1].value != 131) {
		return false;
	}
	if(format[2].value != 3) {
		return false;
	}
	if(format[3].value != 2) {
		return false;
	}
	if(format[4].value != 2) {
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
	if(format[8].value != 1) {
		return false;
	}
	if(format[9].value != 1) {
		return false;
	}
	if(format[10].value != 1) {
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
	m_records = vnnew Record_tech[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_tech::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_tech::version() const {
	return m_version;
}

size_t Table_tech::size() const {
	return m_size;
}

const Record_tech * Table_tech::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_tech::find(u32 _Slot,u32 _Rank) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_Slot,_Rank);
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
