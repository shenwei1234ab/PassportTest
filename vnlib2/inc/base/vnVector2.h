//
//  vnVector2.h
//  vnlib
//
//  Created by Wayne on 14-5-31.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnVector2_h
#define vnlib_vnVector2_h

#include "vnMath.h"

_vn_begin

template <typename T>
struct vector2 {
	union {
		struct {
			T x, y;
		};
		T _[2];
	};
	
	vector2() : x(0), y(0) {}
	vector2( T _x, T _y ) : x(_x), y(_y) {}
	
	template <typename R0, typename R1>
	vector2(const vector2<R0> &a, const vector2<R1> &b, real f)
	: x((T)(a.x + (b.x - a.x) * f))
	, y((T)(a.y + (b.y - a.y) * f))
	{
	
	}
	
	template <typename R>
	vector2(const vector2<R> &rhs) : x((T)rhs.x), y((T)rhs.y) {}
	
	static const vector2 kZero;
	static const vector2 kOneX;
	static const vector2 kOneY;
	static const vector2 kOneXY;
	
	// unary operators
	vector2 operator +() const {
		return *this;
	}
	
	vector2 operator -() const {
		return vector2(-x, -y);
	}
	
	// binary operators
	vector2 operator +(const vector2 &rhs) const {
		return vector2(x + rhs.x, y + rhs.y);
	}
	
	vector2 operator -(const vector2 &rhs) const {
		return vector2(x - rhs.x, y - rhs.y);
	}
	
	vector2 operator *(const vector2 &rhs) const {
		return vector2(x * rhs.x, y * rhs.y);
	}
	
	vector2 operator /(const vector2 &rhs) const {
		return vector2(x / rhs.x, y / rhs.y);
	}
	
	vector2 operator *(real s) const {
		return vector2((T)(x * s), (T)(y * s));
	}
	
	vector2 operator *(int s) const {
		return vector2((T)(x * s), (T)(y * s));
	}
	
	vector2 operator /(real d) const {
		return vector2((T)(x / d), (T)(y / d));
	}
	
	vector2 operator /(int d) const {
		return vector2((T)(x / d), (T)(y / d));
	}
	
	template <typename R>
	bool operator ==(const vector2<R> &rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	
	template <typename R>
	bool operator !=(const vector2<R> &rhs) const {
		return x != rhs.x || y != rhs.y;
	}
	
	template <typename R>
	bool operator <(const vector2<R> &rhs) const {
		return x < rhs.x && y < rhs.y;
	}
	
	template <typename R>
	bool operator <=(const vector2<R> &rhs) const {
		return x <= rhs.x && y <= rhs.y;
	}
	
	template <typename R>
	bool operator >(const vector2<R> &rhs) const {
		return x > rhs.x && y > rhs.y;
	}
	
	template <typename R>
	bool operator >=(const vector2<R> &rhs) const {
		return x >= rhs.x && y >= rhs.y;
	}
	
	template <typename R>
	vector2 & operator =(const vector2<R> &rhs) {
		x = (T)rhs.x;
		y = (T)rhs.y;
		return *this;
	}
	
	template <typename R>
	vector2 & operator +=(const vector2<R> &rhs) {
		x = (T)(x + rhs.x);
		y = (T)(y + rhs.y);
		return *this;
	}
	
	template <typename R>
	vector2 & operator -=(const vector2<R> &rhs) {
		x = (T)(x - rhs.x);
		y = (T)(y - rhs.y);
		return *this;
	}
	
	template <typename R>
	vector2 & operator *=(const vector2<R> &rhs) {
		x = (T)(x * rhs.x);
		y = (T)(y * rhs.y);
		return *this;
	}
	
	template <typename R>
	vector2 & operator /=(const vector2<R> &rhs) {
		x = (T)(x / rhs.x);
		y = (T)(y / rhs.y);
		return *this;
	}
	
	vector2 & operator *=(real s) {
		x = (T)(x * s);
		y = (T)(y * s);
		return *this;
	}
	
	vector2 & operator *=(int s) {
		x = (T)(x * s);
		y = (T)(y * s);
		return *this;
	}
	
	vector2 & operator /=(real d) {
		x = (T)(x / d);
		y = (T)(y / d);
		return *this;
	}
	
	vector2 & operator /=(int d) {
		x = (T)(x / d);
		y = (T)(y / d);
		return *this;
	}
	
	// functions
	bool equals(const vector2 &rhs, const T tolerance = math::constant<T>::rounding_error) const {
		return math::equals<T>(x, rhs.x, tolerance) && math::equals<T>(y, rhs.y, tolerance);
	}
	
	T length_square() const {
		return x * x + y * y;
	}
	
	real length() const {
		return math::sqrt((real)(x * x + y * y));
	}
	
	T area() const {
		return x * y;
	}
	
	template <typename R>
	real distance(const vector2<R> &other) const {
		return (*this - other).length();
	}
	
	template < typename R >
	real dot_product(const vector2<R> &rhs) const {
		return x * rhs.x + y * rhs.y;
	}
	
	template <typename R>
	real cross_product(const vector2<R> &rhs) const {
		return x * rhs.y - y * rhs.x;
	}
	
	vector2 rotate(real theta) const {
		real s, c;
		math::sincos(theta, s, c);
		return vector2(c * x - s * y, s * x + c * y);
	}
	
	vector2 rotate(real cos_theta, real sin_theta) const {
		return vector2(cos_theta * x - sin_theta * y, sin_theta * x + cos_theta * y);
	}
	
	vector2 rotate(real theta, const vector2 &center) const {
		return (*this - center).rotate(theta) + center;
	}
	
	vector2 rotate(real cos_theta, real sin_theta, const vector2 &center) const {
		return (*this - center).rotate(cos_theta, sin_theta) + center;
	}
	
	template < typename R >
	vector2<R> cast() const {
		return vector2<R>( (R)x, (R)y );
	}
	
    std::pair<real, bool> normalize(const T tolerance = math::constant<T>::rounding_error) {
        std::pair<real, bool> ret {length(), false};
		ret.first = length();
		if (!math::is_zero(ret.first, tolerance)) {
            *this /= ret.first;
            ret.second = true;
		}
		return ret;
	}
	
	vector2 & set(T _x, T _y) {
		x = _x;
		y = _y;
		return *this;
	}
	
	vector2 & invert() {
		x = -x;
		y = -y;
		return *this;
	}
	
	template <typename R0, typename R1>
	vector2 & interpolate(const vector2<R0> &a, const vector2<R1> &b, real f) {
		x = (T)(a.x + (b.x - a.x) * f);
		y = (T)(a.y + (b.y - a.y) * f);
		return *this;
	}
	
	template <typename R0, typename R1, typename R2>
	vector2 & interpolate(const vector2<R0> &a, const vector2<R1> &b, const vector2<R2> &c, real f) {
		T tx = (T)(a.x + (b.x - a.x) * f);
		T ty = (T)(a.y + (b.y - a.y) * f);
		x = (T)(tx + (c.x - tx) * f);
		y = (T)(ty + (c.y - ty) * f);
		return *this;
	}
};

template <typename T>
const vector2<T> vector2<T>::kZero;

template <typename T>
const vector2<T> vector2<T>::kOneX(1, 0);

template <typename T>
const vector2<T> vector2<T>::kOneY(0, 1);

template <typename T>
const vector2<T> vector2<T>::kOneXY(1, 1);


template < typename T >
vector2<T> operator *(real s, const vector2<T> &rhs) {
	return vector2<T>( s * rhs.x, s * rhs.y );
}

template < typename T >
vector2<T> operator *(int s, const vector2<T> &rhs) {
	return vector2<T>( s * rhs.x, s * rhs.y );
}

template < typename T >
vector2<T> operator /(real d, const vector2<T> &rhs) {
	return vector2<T>( d / rhs.x, d / rhs.y );
}

template < typename T >
vector2<T> operator /(int d, const vector2<T> &rhs) {
	return vector2<T>( d / rhs.x, d / rhs.y );
}

typedef vector2<int> vector2i;
typedef vector2<real> vector2f;

inline vector2f operator +(const vector2i &a, const vector2f &b) {
	return vector2f( a.x + b.x, a.y + b.y );
}

inline vector2f operator -(const vector2i &a, const vector2f &b) {
	return vector2f( a.x - b.x, a.y - b.y );
}

inline vector2f operator *(const vector2i &a, const vector2f &b) {
	return vector2f( a.x * b.x, a.y * b.y );
}

inline vector2f operator /(const vector2i &a, const vector2f &b) {
	return vector2f( a.x / b.x, a.y / b.y );
}

_vn_end

#endif
