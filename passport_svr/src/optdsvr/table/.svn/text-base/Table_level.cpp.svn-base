#include "Table_level.h"
#include "vnFileManager.h"

namespace tbl {
int Record_level::compare(u32 _LevelId) const {
	return f_LevelId.compare(_LevelId);
}

void Record_level::load(DataStream &stream) {
	stream >> f_LevelId;
	stream >> f_PreType;
	stream >> f_PreId;
	stream >> f_LevelGroupId;
	stream >> f_MapFile;
	stream >> f_CouponId;
	stream >> f_InitGold;
	stream >> f_NumGoldTrees;
	stream >> f_GoldTreeValue;
	stream >> f_RedTowerId;
	stream >> f_GreenTowerId;
	stream >> f_WhiteTowerId;
	stream >> f_FirstPassRewardType;
	stream >> f_FirstPassRewardValue;
	stream >> f_FirstChipType;
	stream >> f_FirstChipValue;
	stream >> f_MineId;
	stream >> f_MineValue;
	stream >> f_Task1Type;
	stream >> f_Reward1Type;
	stream >> f_Reward1Value;
	stream >> f_Reward1ChipType;
	stream >> f_Reward1ChipValue;
	stream >> f_Task2Type;
	stream >> f_Reward2Type;
	stream >> f_Reward2Value;
	stream >> f_Reward2ChipType;
	stream >> f_Reward2ChipValue;
	stream >> f_Task3Type;
	stream >> f_Reward3Type;
	stream >> f_Reward3Value;
	stream >> f_Reward3ChipType;
	stream >> f_Reward3ChipValue;
	stream >> f_Music0;
	stream >> f_Music1;
}

Table_level::Table_level()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_level::~Table_level() {
	fini();
}

bool Table_level::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "level.tbl";
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
bool Table_level::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 35) {
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
	if(format[3].value != 3) {
		return false;
	}
	if(format[4].value != 1) {
		return false;
	}
	if(format[5].value != 3) {
		return false;
	}
	if(format[6].value != 2) {
		return false;
	}
	if(format[7].value != 3) {
		return false;
	}
	if(format[8].value != 2) {
		return false;
	}
	if(format[9].value != 1) {
		return false;
	}
	if(format[10].value != 1) {
		return false;
	}
	if(format[11].value != 1) {
		return false;
	}
	if(format[12].value != 3) {
		return false;
	}
	if(format[13].value != 3) {
		return false;
	}
	if(format[14].value != 3) {
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
	if(format[18].value != 3) {
		return false;
	}
	if(format[19].value != 3) {
		return false;
	}
	if(format[20].value != 3) {
		return false;
	}
	if(format[21].value != 3) {
		return false;
	}
	if(format[22].value != 3) {
		return false;
	}
	if(format[23].value != 3) {
		return false;
	}
	if(format[24].value != 3) {
		return false;
	}
	if(format[25].value != 3) {
		return false;
	}
	if(format[26].value != 3) {
		return false;
	}
	if(format[27].value != 3) {
		return false;
	}
	if(format[28].value != 3) {
		return false;
	}
	if(format[29].value != 3) {
		return false;
	}
	if(format[30].value != 3) {
		return false;
	}
	if(format[31].value != 3) {
		return false;
	}
	if(format[32].value != 3) {
		return false;
	}
	if(format[33].value != 3) {
		return false;
	}
	if(format[34].value != 3) {
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
	m_records = vnnew Record_level[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_level::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_level::version() const {
	return m_version;
}

size_t Table_level::size() const {
	return m_size;
}

const Record_level * Table_level::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_level::find(u32 _LevelId) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_LevelId);
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
