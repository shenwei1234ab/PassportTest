#include "Table_coupon.h"
#include "vnFileManager.h"

namespace tbl {
int Record_coupon::compare(u32 _CouponId) const {
	return f_CouponId.compare(_CouponId);
}

void Record_coupon::load(DataStream &stream) {
	stream >> f_CouponId;
	stream >> f_MinLevelType;
	stream >> f_MinLevelId;
	stream >> f_MaxLevelType;
	stream >> f_MaxLevelId;
	stream >> f_Weight;
	stream >> f_Time;
	stream >> f_Type;
	stream >> f_Content;
	stream >> f_ImageId;
	stream >> f_SubImageId;
	stream >> f_Title;
	stream >> f_Desc;
	stream >> f_Cost;
	stream >> f_OriginalCost;
}

Table_coupon::Table_coupon()
:m_version(0)
,m_records(0)
,m_size(0) {
}

Table_coupon::~Table_coupon() {
	fini();
}

bool Table_coupon::init(u32 fsId, const str8& path) {
	FilePath name(fsId, path);
	if(!name.fileName.empty() && name.fileName.back() != '/') {
		name.fileName += '/';
	}
	name.fileName += "coupon.tbl";
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
bool Table_coupon::init(DataStream &stream) {
	u32 head;
	stream >> head;
	if(head != kTableFileHeader) {
		return false;
	}
	RecordFormat format;
	stream >> format;
	if(format.size() != 15) {
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
	if(format[4].value != 3) {
		return false;
	}
	if(format[5].value != 3) {
		return false;
	}
	if(format[6].value != 4) {
		return false;
	}
	if(format[7].value != 3) {
		return false;
	}
	if(format[8].value != 1) {
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
	if(format[14].value != 3) {
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
	m_records = vnnew Record_coupon[m_size];
	for(size_t i = 0; i < m_size; ++i) {
		m_records[i].load(stream);
	}
	return true;
}

void Table_coupon::fini() {
	VN_SAFE_DELETE_ARRAY(m_records);
	m_size = 0;
}

u32 Table_coupon::version() const {
	return m_version;
}

size_t Table_coupon::size() const {
	return m_size;
}

const Record_coupon * Table_coupon::at(size_t index) const{
	if(index < m_size) {
		return m_records + index;
	}
	return 0;
}

size_t Table_coupon::find(u32 _CouponId) {
	ssize_t low = 0, high = (ssize_t)m_size - 1, mid;
	while(low <= high) {
		mid = (high + low)/2;
		int ret = m_records[mid].compare(_CouponId);
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
