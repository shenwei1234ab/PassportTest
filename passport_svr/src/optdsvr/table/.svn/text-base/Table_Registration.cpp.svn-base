#include "Table_Registration.h"
#include "vnFileManager.h"

namespace tbl {
int Record_Registration::compare(u32 _Id,u32 _StepId) const {
	int ret;
	ret = f_Id.compare(_Id);
	if (ret) {
		return ret;
	}
	ret = f_StepId.compare(_StepId);
	if (ret) {
		return ret;
	}
	return 0;
}

void Record_Registration::load(DataStream &stream) {
	stream >> f_Id;
	stream >> f_StepId;
	stream >> f_Theprops;
	stream >> f_Number;
}

Table_Registration::Table_Registration()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_Registration::~Table_Registration() {
	fini();
}

bool Table_Registration::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "Registration.tbl";
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
bool Table_Registration::init(DataStream &stream) {
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
	if(format[1].value != 131) {
		return false;
	}
	if(format[2].value != 3) {
		return false;
	}
	if(format[3].value != 2) {
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
	m_records = vnnew Record_Registration[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_Registration::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_Registration::version() const {
	return m_version;
}

size_t Table_Registration::size() const {
	return m_size;
}

const Record_Registration * Table_Registration::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_Registration::find(u32 _Id,u32 _StepId) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_Id,_StepId);
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
