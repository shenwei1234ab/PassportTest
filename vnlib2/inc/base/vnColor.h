//
//  vnColor.h
//  vnlib
//
//  Created by Wayne on 6/3/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnColor_h
#define vnlib_vnColor_h

#include "../vnDefine.h"

_vn_begin

struct color_t {
	union {
		struct {
			u8 red;
			u8 green;
			u8 blue;
			u8 alpha;
		};
		u32 value;
	};
    
	color_t();
	color_t(u32 _value);
	color_t(u8 r, u8 g, u8 b, u8 a = 0xFF);
	color_t(const color_t &a, const color_t &b, f32 f);
	
	operator u32() const;
	
	f32 unitRed() const;
	f32 unitGreen() const;
	f32 unitBlue() const;
	f32 unitAlpha() const;
    
	bool operator ==(const color_t &rhs) const;
	bool operator !=(const color_t &rhs) const;
    
	color_t operator +(const color_t &rhs) const;
	color_t operator -(const color_t &rhs) const;
	color_t operator *(const color_t &rhs) const;
	color_t operator *(f32 rhs) const;
    
	color_t & operator =(const color_t &rhs);
	color_t & operator +=(const color_t &rhs);
	color_t & operator -=(const color_t &rhs);
	color_t & operator *=(const color_t &rhs);
	color_t & operator *=(f32 rhs);
	
	color_t & convertFromBGRA(u32 bgra);
	u32 convertToBGRA() const;
    
	f32 luminance() const;
	
	color_t & interpolate(const color_t &a, const color_t &b, f32 f);
	
	static const color_t kWhite;
	static const color_t kBlack;
	static const color_t kRed;
	static const color_t kGreen;
	static const color_t kBlue;
	static const color_t kYellow;
	static const color_t kCyan;
	static const color_t kMegenta;
};

_vn_end

#endif
