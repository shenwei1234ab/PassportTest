#include "Table_monster.h"
#include "vnFileManager.h"

namespace tbl {
int Record_monster::compare(u32 _MonsterID) const {
	return f_MonsterID.compare(_MonsterID);
}

void Record_monster::load(DataStream &stream) {
	stream >> f_MonsterID;
	stream >> f_MonsterName;
	stream >> f_MonsterType;
	stream >> f_MoveType;
	stream >> f_Tags;
	stream >> f_Speed;
	stream >> f_PhysicalRes;
	stream >> f_MagicRes;
	stream >> f_HP;
	stream >> f_GoldKill;
	stream >> f_FruitMax;
	stream >> f_MonsterSpell;
	stream >> f_StartBuff;
	stream >> f_WaterBuff;
	stream >> f_Animation;
	stream >> f_DieSoundID;
	stream >> f_WaveTip;
	stream >> f_Flags;
	stream >> f_Behavior;
	stream >> f_Content1;
	stream >> f_Content2;
}

Table_monster::Table_monster()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_monster::~Table_monster() {
	fini();
}

bool Table_monster::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "monster.tbl";
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
bool Table_monster::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 21) {
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
	if(format[3].value != 3) {
		return false;
	}
	if(format[4].value != 3) {
		return false;
	}
	if(format[5].value != 4) {
		return false;
	}
	if(format[6].value != 4) {
		return false;
	}
	if(format[7].value != 4) {
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
	if(format[11].value != 1) {
		return false;
	}
	if(format[12].value != 1) {
		return false;
	}
	if(format[13].value != 3) {
		return false;
	}
	if(format[14].value != 1) {
		return false;
	}
	if(format[15].value != 3) {
		return false;
	}
	if(format[16].value != 3) {
		return false;
	}
	if(format[17].value != 3) {
		return false;
	}
	if(format[18].value != 1) {
		return false;
	}
	if(format[19].value != 1) {
		return false;
	}
	if(format[20].value != 1) {
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
	m_records = vnnew Record_monster[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_monster::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_monster::version() const {
	return m_version;
}

size_t Table_monster::size() const {
	return m_size;
}

const Record_monster * Table_monster::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_monster::find(u32 _MonsterID) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_MonsterID);
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
