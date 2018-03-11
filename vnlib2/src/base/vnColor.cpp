//
//  vnColor.cpp
//  vnlib
//
//  Created by Wayne on 6/3/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnColor.h"

#include "vnMath.h"

_vn_begin

const color_t color_t::kBlack(0, 0, 0);
const color_t color_t::kWhite(0xFF, 0xFF, 0xFF);
const color_t color_t::kRed(0xFF, 0, 0);
const color_t color_t::kGreen(0, 0xFF, 0);
const color_t color_t::kBlue(0, 0, 0xFF);
const color_t color_t::kYellow(0xFF, 0xFF, 0);
const color_t color_t::kCyan(0, 0xFF, 0xFF);
const color_t color_t::kMegenta(0xFF, 0, 0xFF);

color_t::color_t()
: value(0xFFFFFFFF) {
    
}

color_t::color_t(u32 _value) {
    value = _value;
}

color_t::color_t(u8 r, u8 g, u8 b, u8 a) {
	red = r;
	green = g;
	blue = b;
	alpha = a;
}

color_t::color_t(const color_t &a, const color_t &b, f32 f) {
	red = (u8)math::clamp((s32)(a.red + ((s32)b.red - (s32)a.red) * f), 0, 255);
	green = (u8)math::clamp((s32)(a.green + ((s32)b.green - (s32)a.green) * f), 0, 255);
	blue = (u8)math::clamp((s32)(a.blue + ((s32)b.blue - (s32)a.blue) * f), 0, 255);
	alpha = (u8)math::clamp((s32)(a.alpha + ((s32)b.alpha - (s32)a.alpha) * f), 0, 255);
}

bool color_t::operator ==(const color_t &rhs) const {
	return value == rhs.value;
}

bool color_t::operator !=(const color_t &rhs) const {
	return value != rhs.value;
}

color_t::operator u32() const {
    return value;
}

f32 color_t::unitRed() const {
	return red / 255.f;
}

f32 color_t::unitGreen() const {
	return green / 255.f;
}

f32 color_t::unitBlue() const {
	return blue / 255.f;
}

f32 color_t::unitAlpha() const {
	return alpha / 255.f;
}

color_t color_t::operator +(const color_t &rhs) const {
	return color_t((u8)math::clamp((s32)red + (s32)rhs.red, 0, 255),
				   (u8)math::clamp((s32)green + (s32)rhs.green, 0, 255),
				   (u8)math::clamp((s32)blue + (s32)rhs.blue, 0, 255),
				   (u8)math::clamp((s32)alpha + (s32)rhs.alpha, 0, 255));
}

color_t color_t::operator -(const color_t &rhs) const {
	return color_t((u8)math::clamp((s32)red - (s32)rhs.red, 0, 255),
				   (u8)math::clamp((s32)green - (s32)rhs.green, 0, 255),
				   (u8)math::clamp((s32)blue - (s32)rhs.blue, 0, 255),
				   (u8)math::clamp((s32)alpha - (s32)rhs.alpha, 0, 255));
}

color_t color_t::operator *(const color_t &rhs) const {
	return color_t((u8)math::clamp((s32)((s32)red * rhs.red / 255.f), 0, 255),
				   (u8)math::clamp((s32)((s32)green * rhs.green / 255.f), 0, 255),
				   (u8)math::clamp((s32)((s32)blue * rhs.blue / 255.f), 0, 255),
				   (u8)math::clamp((s32)((s32)alpha * rhs.alpha / 255.f), 0, 255));
}

color_t color_t::operator *(f32 rhs) const {
	return color_t((u8)math::clamp((s32)(red * rhs), 0, 255),
				   (u8)math::clamp((s32)(green * rhs), 0, 255),
				   (u8)math::clamp((s32)(blue * rhs), 0, 255),
				   (u8)math::clamp((s32)(alpha * rhs), 0, 255));
}

color_t & color_t::operator =(const color_t &rhs) {
	value = rhs.value;
	return *this;
}

color_t & color_t::operator +=(const color_t &rhs) {
	red = (u8)math::clamp((s32)red + (s32)rhs.red, 0, 255);
	green = (u8)math::clamp((s32)green + (s32)rhs.green, 0, 255);
	blue = (u8)math::clamp((s32)blue + (s32)rhs.blue, 0, 255);
	alpha = (u8)math::clamp((s32)alpha + (s32)rhs.alpha, 0, 255);
	return *this;
}

color_t & color_t::operator -=(const color_t &rhs) {
	red = (u8)math::clamp((s32)red - (s32)rhs.red, 0, 255);
	green = (u8)math::clamp((s32)green - (s32)rhs.green, 0, 255);
	blue = (u8)math::clamp((s32)blue - (s32)rhs.blue, 0, 255);
	alpha = (u8)math::clamp((s32)alpha - (s32)rhs.alpha, 0, 255);
	return *this;
}

color_t & color_t::operator *=(const color_t &rhs) {
	red = (u8)math::clamp((s32)((s32)red * rhs.red / 255.f), 0, 255);
	green = (u8)math::clamp((s32)((s32)green * rhs.green / 255.f), 0, 255);
	blue = (u8)math::clamp((s32)((s32)blue * rhs.blue / 255.f), 0, 255);
	alpha = (u8)math::clamp((s32)((s32)alpha * rhs.alpha / 255.f), 0, 255);
	return *this;
}

color_t & color_t::operator *=(f32 rhs) {
	red = (u8)math::clamp((s32)(red * rhs), 0, 255);
	green = (u8)math::clamp((s32)(green * rhs), 0, 255);
	blue = (u8)math::clamp((s32)(blue * rhs), 0, 255);
	alpha = (u8)math::clamp((s32)(alpha * rhs), 0, 255);
	return *this;
}


color_t & color_t::convertFromBGRA(u32 argb) {
    blue = (u8)argb;
	green = (u8)(argb >> 8);
	red = (u8)(argb >> 16);
	alpha = (u8)(argb >> 24);
    return *this;
}

u32 color_t::convertToBGRA() const {
	return ((u32)alpha << 24) | ((u32)red << 16) | ((u32)green << 8) | ((u32)blue);
}


f32 color_t::luminance() const {
    return (0.299f*red + 0.587f*green + 0.114f*blue) / 255.f;
}

color_t & color_t::interpolate(const color_t &a, const color_t &b, f32 f) {
	red = (u8)math::clamp((s32)(a.red + ((s32)b.red - (s32)a.red) * f), 0, 255);
	green = (u8)math::clamp((s32)(a.green + ((s32)b.green - (s32)a.green) * f), 0, 255);
	blue = (u8)math::clamp((s32)(a.blue + ((s32)b.blue - (s32)a.blue) * f), 0, 255);
	alpha = (u8)math::clamp((s32)(a.alpha + ((s32)b.alpha - (s32)a.alpha) * f), 0, 255);
	return *this;
}

_vn_end

