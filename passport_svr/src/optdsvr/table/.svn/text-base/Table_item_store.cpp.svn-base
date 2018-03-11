#include "Table_item_store.h"
#include "vnFileManager.h"

namespace tbl {
int Record_item_store::compare(u32 _id) const {
	return f_id.compare(_id);
}

void Record_item_store::load(DataStream &stream) {
	stream >> f_id;
	stream >> f_price;
}

Table_item_store::Table_item_store()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_item_store::~Table_item_store() {
	fini();
}

bool Table_item_store::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "item_store.tbl";
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
bool Table_item_store::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 2) {
		return false;
	}
	if(format[0].value != 131) {
		return false;
	}
	if(format[1].value != 3) {
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
	m_records = vnnew Record_item_store[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_item_store::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_item_store::version() const {
	return m_version;
}

size_t Table_item_store::size() const {
	return m_size;
}

const Record_item_store * Table_item_store::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_item_store::find(u32 _id) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_id);
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
