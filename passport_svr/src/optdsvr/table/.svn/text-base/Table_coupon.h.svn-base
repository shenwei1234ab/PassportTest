#ifndef __Table_coupon_H__
#define __Table_coupon_H__

#include "TableBase.h"
#include "vnFilePath.h"
namespace tbl {
struct Record_coupon : public AllocatedObject {
	Field_uint f_CouponId;
	Field_uint f_MinLevelType;
	Field_uint f_MinLevelId;
	Field_uint f_MaxLevelType;
	Field_uint f_MaxLevelId;
	Field_uint f_Weight;
	Field_float f_Time;
	Field_uint f_Type;
	Field_string f_Content;
	Field_uint f_ImageId;
	Field_uint f_SubImageId;
	Field_string f_Title;
	Field_string f_Desc;
	Field_uint f_Cost;
	Field_uint f_OriginalCost;
	int compare(u32 _CouponId) const;
	void load(DataStream &stream);
};

class Table_coupon{
public:
	Table_coupon();
	~Table_coupon();

	bool init(DataStream &stream);
	bool init(u32 fsId, const str8& path);
	void fini();

	u32 version() const;
	size_t size() const;
	const Record_coupon * at(size_t index) const;

	size_t find(u32 _CouponId);
private:
	u32 m_version;
	Record_coupon *m_records;
	size_t m_size;
};
}
#endif
