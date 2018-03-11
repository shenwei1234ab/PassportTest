#include "Table_wave.h"
#include "vnFileManager.h"

namespace tbl {
int Record_wave::compare(u32 _LevelId,u32 _WaveId) const {
	int ret;
	ret = f_LevelId.compare(_LevelId);
	if (ret) {
		return ret;
	}
	ret = f_WaveId.compare(_WaveId);
	if (ret) {
		return ret;
	}
	return 0;
}

void Record_wave::load(DataStream &stream) {
	stream >> f_LevelId;
	stream >> f_WaveId;
	stream >> f_BeginTime;
	stream >> f_MaxWaitingTime;
	stream >> f_SubWaves;
	stream >> f_Music;
}

Table_wave::Table_wave()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_wave::~Table_wave() {
	fini();
}

bool Table_wave::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "wave.tbl";
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
bool Table_wave::init(DataStream &stream) {
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
	if(format[0].value != 131) {
		return false;
	}
	if(format[1].value != 131) {
		return false;
	}
	if(format[2].value != 4) {
		return false;
	}
	if(format[3].value != 4) {
		return false;
	}
	if(format[4].value != 1) {
		return false;
	}
	if(format[5].value != 3) {
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
	m_records = vnnew Record_wave[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_wave::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_wave::version() const {
	return m_version;
}

size_t Table_wave::size() const {
	return m_size;
}

const Record_wave * Table_wave::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_wave::find(u32 _LevelId,u32 _WaveId) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_LevelId,_WaveId);
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
