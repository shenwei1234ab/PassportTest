#include "Table_training.h"
#include "vnFileManager.h"

namespace tbl {
int Record_training::compare(u32 _Class,u32 _Slot,u32 _AdvancedTimes) const {
	int ret;
	ret = f_Class.compare(_Class);
	if (ret) {
		return ret;
	}
	ret = f_Slot.compare(_Slot);
	if (ret) {
		return ret;
	}
	ret = f_AdvancedTimes.compare(_AdvancedTimes);
	if (ret) {
		return ret;
	}
	return 0;
}

void Record_training::load(DataStream &stream) {
	stream >> f_Class;
	stream >> f_Slot;
	stream >> f_AdvancedTimes;
	stream >> f_TowerId;
	stream >> f_CostType;
	stream >> f_CostValue;
	stream >> f_ChipType;
	stream >> f_ChipValue;
	stream >> f_SkillId1;
	stream >> f_Skillcost1;
	stream >> f_SkillId2;
	stream >> f_Skillcost2;
	stream >> f_SkillId3;
	stream >> f_Skillcost3;
	stream >> f_SkillName1;
	stream >> f_SkillDesc1;
	stream >> f_SkillName2;
	stream >> f_SkillDesc2;
	stream >> f_SkillName3;
	stream >> f_SkillDesc3;
}

Table_training::Table_training()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_training::~Table_training() {
	fini();
}

bool Table_training::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "training.tbl";
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
bool Table_training::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 20) {
		return false;
	}
	if(format[0].value != 131) {
		return false;
	}
	if(format[1].value != 131) {
		return false;
	}
	if(format[2].value != 131) {
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
	if(format[11].value != 1) {
		return false;
	}
	if(format[12].value != 1) {
		return false;
	}
	if(format[13].value != 1) {
		return false;
	}
	if(format[14].value != 1) {
		return false;
	}
	if(format[15].value != 1) {
		return false;
	}
	if(format[16].value != 1) {
		return false;
	}
	if(format[17].value != 1) {
		return false;
	}
	if(format[18].value != 1) {
		return false;
	}
	if(format[19].value != 1) {
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
	m_records = vnnew Record_training[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_training::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_training::version() const {
	return m_version;
}

size_t Table_training::size() const {
	return m_size;
}

const Record_training * Table_training::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_training::find(u32 _Class,u32 _Slot,u32 _AdvancedTimes) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_Class,_Slot,_AdvancedTimes);
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
