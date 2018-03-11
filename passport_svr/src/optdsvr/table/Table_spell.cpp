#include "Table_spell.h"
#include "vnFileManager.h"

namespace tbl {
int Record_spell::compare(u32 _SpellId) const {
	return f_SpellId.compare(_SpellId);
}

void Record_spell::load(DataStream &stream) {
	stream >> f_SpellId;
	stream >> f_SpellName;
	stream >> f_CDTime;
	stream >> f_CostMana;
	stream >> f_TargetSource;
	stream >> f_DamageType;
	stream >> f_DamagePercent;
	stream >> f_DamageBase;
	stream >> f_HealPercent;
	stream >> f_Heal;
	stream >> f_ObjectDest;
	stream >> f_ObjectId;
	stream >> f_BuffId;
	stream >> f_Gold;
	stream >> f_SummonDistance;
	stream >> f_SummonId;
	stream >> f_MoveDistance;
	stream >> f_Scan;
	stream >> f_IncDigger;
	stream >> f_TriggeredSpellId;
	stream >> f_PositionSource;
	stream >> f_RangeType;
	stream >> f_RangeValue1;
	stream >> f_RangeValue2;
	stream >> f_RangeNumTargets;
	stream >> f_RangeTargetType;
	stream >> f_RangeSpellId;
	stream >> f_FilterNumTargets;
	stream >> f_FilterTargetType;
	stream >> f_FilterRadius;
	stream >> f_Except;
	stream >> f_Flags;
	stream >> f_Action;
	stream >> f_explain;
	stream >> f_CastEffect;
	stream >> f_HitEffect;
	stream >> f_LinkCasterEffect;
	stream >> f_LinkTargetEffect;
}

Table_spell::Table_spell()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_spell::~Table_spell() {
	fini();
}

bool Table_spell::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "spell.tbl";
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
bool Table_spell::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 38) {
		return false;
	}
	if(format[0].value != 131) {
		return false;
	}
	if(format[1].value != 1) {
		return false;
	}
	if(format[2].value != 4) {
		return false;
	}
	if(format[3].value != 2) {
		return false;
	}
	if(format[4].value != 3) {
		return false;
	}
	if(format[5].value != 3) {
		return false;
	}
	if(format[6].value != 4) {
		return false;
	}
	if(format[7].value != 2) {
		return false;
	}
	if(format[8].value != 4) {
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
	if(format[12].value != 1) {
		return false;
	}
	if(format[13].value != 2) {
		return false;
	}
	if(format[14].value != 4) {
		return false;
	}
	if(format[15].value != 3) {
		return false;
	}
	if(format[16].value != 4) {
		return false;
	}
	if(format[17].value != 4) {
		return false;
	}
	if(format[18].value != 2) {
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
	if(format[22].value != 4) {
		return false;
	}
	if(format[23].value != 4) {
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
	if(format[29].value != 4) {
		return false;
	}
	if(format[30].value != 3) {
		return false;
	}
	if(format[31].value != 3) {
		return false;
	}
	if(format[32].value != 1) {
		return false;
	}
	if(format[33].value != 1) {
		return false;
	}
	if(format[34].value != 3) {
		return false;
	}
	if(format[35].value != 3) {
		return false;
	}
	if(format[36].value != 3) {
		return false;
	}
	if(format[37].value != 3) {
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
	m_records = vnnew Record_spell[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_spell::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_spell::version() const {
	return m_version;
}

size_t Table_spell::size() const {
	return m_size;
}

const Record_spell * Table_spell::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_spell::find(u32 _SpellId) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_SpellId);
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
