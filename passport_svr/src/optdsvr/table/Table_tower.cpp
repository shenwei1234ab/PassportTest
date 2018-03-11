#include "Table_tower.h"
#include "vnFileManager.h"

namespace tbl {
int Record_tower::compare(u32 _TowerID) const {
	return f_TowerID.compare(_TowerID);
}

void Record_tower::load(DataStream &stream) {
	stream >> f_TowerID;
	stream >> f_TowerName;
	stream >> f_TowerClass;
	stream >> f_RangeRd;
	stream >> f_TargetNum;
	stream >> f_TargetFilter;
	stream >> f_Except;
	stream >> f_Cost;
	stream >> f_BuildType;
	stream >> f_UpgradeID;
	stream >> f_Level;
	stream >> f_AtkType;
	stream >> f_MinAtk;
	stream >> f_MaxAtk;
	stream >> f_MinAtk2;
	stream >> f_MaxAtk2;
	stream >> f_MaxMana;
	stream >> f_ManaRegRate;
	stream >> f_CritRate;
	stream >> f_CritFactor;
	stream >> f_CommonAttackID;
	stream >> f_Flags;
	stream >> f_Animation;
	stream >> f_Debutsound;
	stream >> f_explain;
}

Table_tower::Table_tower()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_tower::~Table_tower() {
	fini();
}

bool Table_tower::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "tower.tbl";
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
bool Table_tower::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 25) {
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
	if(format[9].value != 1) {
		return false;
	}
	if(format[10].value != 3) {
		return false;
	}
	if(format[11].value != 3) {
		return false;
	}
	if(format[12].value != 2) {
		return false;
	}
	if(format[13].value != 2) {
		return false;
	}
	if(format[14].value != 2) {
		return false;
	}
	if(format[15].value != 2) {
		return false;
	}
	if(format[16].value != 2) {
		return false;
	}
	if(format[17].value != 4) {
		return false;
	}
	if(format[18].value != 4) {
		return false;
	}
	if(format[19].value != 4) {
		return false;
	}
	if(format[20].value != 3) {
		return false;
	}
	if(format[21].value != 3) {
		return false;
	}
	if(format[22].value != 1) {
		return false;
	}
	if(format[23].value != 3) {
		return false;
	}
	if(format[24].value != 1) {
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
	m_records = vnnew Record_tower[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_tower::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_tower::version() const {
	return m_version;
}

size_t Table_tower::size() const {
	return m_size;
}

const Record_tower * Table_tower::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_tower::find(u32 _TowerID) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_TowerID);
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
