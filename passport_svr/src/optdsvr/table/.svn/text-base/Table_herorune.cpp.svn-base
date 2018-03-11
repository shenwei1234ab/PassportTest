#include "Table_herorune.h"
#include "vnFileManager.h"

namespace tbl {
int Record_herorune::compare(u32 _RuneClass,u32 _RuneRank) const {
	int ret;
	ret = f_RuneClass.compare(_RuneClass);
	if (ret) {
		return ret;
	}
	ret = f_RuneRank.compare(_RuneRank);
	if (ret) {
		return ret;
	}
	return 0;
}

void Record_herorune::load(DataStream &stream) {
	stream >> f_RuneClass;
	stream >> f_RuneRank;
	stream >> f_Level;
	stream >> f_CostType;
	stream >> f_CostValue;
	stream >> f_ActiveSpellID;
	stream >> f_PassiveSpellID;
	stream >> f_Explain;
}

Table_herorune::Table_herorune()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_herorune::~Table_herorune() {
	fini();
}

bool Table_herorune::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "herorune.tbl";
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
bool Table_herorune::init(DataStream &stream) {
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
	if(format[0].value != 131) {
		return false;
	}
	if(format[1].value != 131) {
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
	m_records = vnnew Record_herorune[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_herorune::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_herorune::version() const {
	return m_version;
}

size_t Table_herorune::size() const {
	return m_size;
}

const Record_herorune * Table_herorune::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_herorune::find(u32 _RuneClass,u32 _RuneRank) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_RuneClass,_RuneRank);
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
