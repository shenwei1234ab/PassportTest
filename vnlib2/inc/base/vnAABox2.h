//
//  vnAABox2.h
//  vnlib
//
//  Created by Wayne on 14-5-31.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnAABox2_h
#define vnlib_vnAABox2_h

#include "vnVector2.h"

_vn_begin

template < typename T >
union aabox2 {
	struct {
		vector2<T> min_corner;
		vector2<T> max_corner;
	};
	T _[4];

	aabox2() : min_corner(), max_corner() {}
	aabox2(T min_x, T min_y, T max_x, T max_y) : min_corner(min_x, min_y), max_corner(max_x, max_y) {}

	template < typename P, typename Q >
	aabox2(const vector2<P> &_min, const vector2<Q> &_max) : min_corner(_min), max_corner(_max) {}

	template < typename R >
	aabox2(const aabox2<R> &rhs) : min_corner(rhs.min_corner), max_corner(rhs.max_corner) {}

	template < typename P, typename Q >
	aabox2(const aabox2<P> &a, const aabox2<Q> &b, real f)
	: min_corner(a.min_corner, b.min_corner, f)
	, max_corner(a.max_corner, b.max_corner, f) {}
	
	// binary operators
	template <typename R>
	aabox2 operator +(const vector2<R> &rhs) const {
		return aabox2(min_corner + rhs, max_corner + rhs);
	}

	template <typename R>
	aabox2 operator -(const vector2<R> &rhs) const {
		return aabox2(min_corner - rhs, max_corner - rhs);
	}

	template <typename R>
	aabox2 operator *(const vector2<R> &rhs) const {
		return aabox2(min_corner * rhs, max_corner * rhs);
	}

	aabox2 operator *(s32 s ) const {
		return aabox2(min_corner * s, max_corner * s);
	}

	aabox2 operator *(real s) const {
		return aabox2(min_corner * s, max_corner * s);
	}

	template <typename R>
	aabox2 operator /(const vector2<R> &rhs) const {
		return aabox2(min_corner / rhs, max_corner / rhs);
	}

	aabox2 operator /(s32 d) const {
		return aabox2(min_corner / d, max_corner / d);
	}

	aabox2 operator /(real d) const {
		return aabox2(min_corner / d, max_corner / d);
	}

	template <typename R>
	bool operator ==(const aabox2<R> &rhs) const {
		return min_corner == rhs.min_corner && max_corner == rhs.max_corner;
	}

	template <typename R>
	bool operator !=(const aabox2<R> &rhs) const {
		return min_corner != rhs.min_corner || max_corner != rhs.max_corner;
	}

	template <typename R>
	aabox2 & operator =(const aabox2<R> &other) {
		min_corner = other.min_corner;
		max_corner = other.max_corner;
		return *this;
	}

	template <typename R>
	aabox2 & operator +=(const vector2<R> &rhs) {
		min_corner += rhs;
		max_corner += rhs;
		return *this;
	}

	template <typename R>
	aabox2 & operator -=(const vector2<R> &rhs) {
		min_corner -= rhs;
		max_corner -= rhs;
		return *this;
	}

	template <typename R>
	aabox2 & operator *=(const vector2<R> &rhs) const {
		min_corner *= rhs;
		max_corner *= rhs;
		return *this;
	}

	aabox2 & operator *=(s32 s) {
		min_corner *= s;
		max_corner *= s;
		return *this;
	}

	aabox2 & operator *=(real s) {
		min_corner *= s;
		max_corner *= s;
		return *this;
	}

	template <typename R>
	aabox2 & operator /=(const vector2<R> &rhs) {
		min_corner /= rhs;
		max_corner /= rhs;
		return *this;
	}

	aabox2 & operator /=(s32 d) {
		min_corner /= d;
		max_corner /= d;
		return *this;
	}

	aabox2 & operator /=(real d) {
		min_corner /= d;
		max_corner /= d;
		return *this;
	}

	// functions
	bool equals(const aabox2 &other, const T tolerance = math::constant<T>::rounding_error) const {
		return min_corner.equals(other.min_corner, tolerance) && max_corner.equals(other.max_corner, tolerance);
	}

	vector2<T> size() const {
		return max_corner - min_corner;
	}
	
	T width() const {
		return max_corner.x - min_corner.x;
	}
	
	T height() const {
		return max_corner.y - min_corner.y;
	}

	T area() const {
		return (max_corner.x - min_corner.x) * (max_corner.y - min_corner.y);
	}

	vector2<T> center() const {
		return (min_corner + max_corner) * 0.5f;
	}

	bool empty(const T tolerance = math::constant<T>::rounding_error) const {
		return max_corner.equals(min_corner, tolerance);
	}

	template <typename R>
	bool contains(const vector2<R> &point) const {
		return point >= min_corner && point <= max_corner;
	}

	template <typename R>
	bool contains(const aabox2<R> &box) const {
		return box.min_corner >= min_corner && box.max_corner <= max_corner;
	}

	bool valid() const {
		return min_corner <= max_corner;
	}


	aabox2 & reset(const vector2<T> &init) {
		min_corner = max_corner = init;
		return *this;
	}

	aabox2 & add_point(const vector2<T> &point) {
		if (point.x < min_corner.x) {
			min_corner.x = point.x;
		} else if (point.x > max_corner.x) {
			max_corner.x = point.x;
		}
		if (point.y < min_corner.y) {
			min_corner.y = point.y;
		} else if (point.y > max_corner.y) {
			max_corner.y = point.y;
		}
		return *this;
	}

	aabox2 & add_box(const aabox2 &box) {
		if (box.min_corner.x < min_corner.x) {
			min_corner.x = box.min_corner.x;
		}
		if (box.max_corner.x > max_corner.x) {
			max_corner.x = box.max_corner.x;
		}
		if (box.min_corner.y < min_corner.y) {
			min_corner.y = box.min_corner.y;
		}
		if (box.max_corner.y > max_corner.y) {
			max_corner.y = box.max_corner.y;
		}
		return *this;
	}

	template <typename R>
	void build_corners(vector2<R> *corners) const {
		corners[0] = min_corner;
		corners[1].set((R)max_corner.x, (R)min_corner.y);
		corners[2] = max_corner;
		corners[3].set((R)min_corner.x, (R)max_corner.y);
	}
	
	template <typename R>
	void build_corners(vector2<R> *corners, size_t stride) const {
		*corners = min_corner;
		corners = (vector2<R> *)((u8 *)corners + stride);
		corners->set((R)max_corner.x, (R)min_corner.y);
		corners = (vector2<R> *)((u8 *)corners + stride);
		*corners = max_corner;
		corners = (vector2<R> *)((u8 *)corners + stride);
		corners->set((R)min_corner.x, (R)max_corner.y);
	}

	const vector2<T> & corner_0() const {
		return min_corner;
	}

	vector2<T> corner_1() const {
		return vector2<T>(max_corner.x, min_corner.y);
	}

	const vector2<T> & corner_2() const {
		return max_corner;
	}

	vector2<T> corner_3() const {
		return vector2<T>(min_corner.x, max_corner.y);
	}

	aabox2 & repair() {
		if (min_corner.x > max_corner.x) {
			math::swap(min_corner.x, max_corner.x);
		}
		if (min_corner.y > max_corner.y) {
			math::swap(min_corner.y, max_corner.y);
		}
		return *this;
	}

	template < typename P, typename Q >
	aabox2 & interpolate(const aabox2<P> &a, const aabox2<Q> &b, real f) {
		min_corner.interpolate(a.min_corner, b.min_corner, f);
		max_corner.interpolate(a.max_corner, b.max_corner, f);
		return *this;
	}

	template < typename P >
	bool intersects(const aabox2<P> &other) const {
		if (min_corner.x > other.max_corner.x || max_corner.x < other.min_corner.x) {
			return false;
		}
		if (min_corner.y > other.max_corner.y || max_corner.y < other.min_corner.y) {
			return false;
		}
		return true;
	}

	aabox2 intersection(const aabox2 &box) const {
		return aabox2(math::max_(min_corner.x, box.min_corner.x), math::max_(min_corner.y, box.min_corner.y),
					  math::min_(max_corner.x, box.max_corner.x), math::min_(max_corner.y, box.max_corner.y));
	}
};

typedef aabox2<s32> aabox2i;
typedef aabox2<real> aabox2f;

_vn_end

#endif
