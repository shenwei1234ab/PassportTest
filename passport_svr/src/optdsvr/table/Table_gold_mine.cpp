#include "Table_gold_mine.h"
#include "vnFileManager.h"

namespace tbl {
int Record_gold_mine::compare(u32 _Id) const {
	return f_Id.compare(_Id);
}

void Record_gold_mine::load(DataStream &stream) {
	stream >> f_Id;
	stream >> f_Type;
	stream >> f_Capacity;
	stream >> f_Factor;
}

Table_gold_mine::Table_gold_mine()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_gold_mine::~Table_gold_mine() {
	fini();
}

bool Table_gold_mine::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "gold_mine.tbl";
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
bool Table_gold_mine::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 4) {
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
	if(format[3].value != 4) {
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
	m_records = vnnew Record_gold_mine[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_gold_mine::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_gold_mine::version() const {
	return m_version;
}

size_t Table_gold_mine::size() const {
	return m_size;
}

const Record_gold_mine * Table_gold_mine::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_gold_mine::find(u32 _Id) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_Id);
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
