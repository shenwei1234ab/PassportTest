#include "Table_buff.h"
#include "vnFileManager.h"

namespace tbl {
int Record_buff::compare(u32 _BuffID) const {
	return f_BuffID.compare(_BuffID);
}

void Record_buff::load(DataStream &stream) {
	stream >> f_BuffID;
	stream >> f_BuffName;
	stream >> f_Flag;
	stream >> f_Duration;
	stream >> f_SpellID;
	stream >> f_EffectType;
	stream >> f_SubEffType;
	stream >> f_Value;
	stream >> f_Trigger;
	stream >> f_TriggerRate;
	stream >> f_TriggerSpell;
	stream >> f_BuffClass;
	stream >> f_BuffLevel;
	stream >> f_RemoveFlags;
	stream >> f_Effect;
	stream >> f_Action;
}

Table_buff::Table_buff()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_buff::~Table_buff() {
	fini();
}

bool Table_buff::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "buff.tbl";
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
bool Table_buff::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 16) {
		return false;
	}
	if(format[0].value != 131) {
		return false;
	}
	if(format[1].value != 1) {
		return false;
	}
	if(format[2].value != 3) {
		return false;
	}
	if(format[3].value != 4) {
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
	if(format[7].value != 2) {
		return false;
	}
	if(format[8].value != 3) {
		return false;
	}
	if(format[9].value != 2) {
		return false;
	}
	if(format[10].value != 1) {
		return false;
	}
	if(format[11].value != 3) {
		return false;
	}
	if(format[12].value != 2) {
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
	m_records = vnnew Record_buff[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_buff::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_buff::version() const {
	return m_version;
}

size_t Table_buff::size() const {
	return m_size;
}

const Record_buff * Table_buff::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_buff::find(u32 _BuffID) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_BuffID);
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
