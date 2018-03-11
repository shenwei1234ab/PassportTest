//
//  vnMath.h
//  vnlib
//
//  Created by Wayne on 14-5-31.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnMath_h
#define vnlib_vnMath_h

#include "../vnDefine.h"

#include <cmath>

_vn_begin

namespace math {
	
	template <typename T>
	struct constant;
	
	template <>
	struct constant<int> {
		static const int rounding_error = 0;
	};
	

	template <>
	struct constant<float> {
		static const float rounding_error;
		static const float pi;
		static const float half_pi;
		static const float two_pi;
	};

	template <>
	struct constant<double> {
		static const double rounding_error;
		static const double pi;
		static const double half_pi;
		static const double two_pi;
	};
    
//	template <>
//	struct constant<float> {
//		constexpr static const float rounding_error = 0.000001f;
//		constexpr static const float pi =       3.1415926535897932f;
//        constexpr static const float half_pi =  1.5707963267948965f;
//        constexpr static const float two_pi =   6.2831853071795862f;
//	};
//	
//	template <>
//	struct constant<double> {
//		constexpr static const double rounding_error = 0.000000000000001;
//        constexpr static const double pi =       3.1415926535897932;
//        constexpr static const double half_pi =  1.5707963267948965;
//        constexpr static const double two_pi =   6.2831853071795862;
//	};
	
	inline float reciprocal(float x) {
		return 1.f / x;
	}
	
	inline double reciprocal(double x) {
		return 1.0 / x;
	}
	
	inline float sqrt(float x) {
		return ::sqrtf(x);
	}
	
	inline double sqrt(double x) {
		return ::sqrt(x);
	}
	
	inline float reciprocal_sqrt(float x) {
		return 1.f / ::sqrtf(x);
	}
	
	inline double reciprocal_sqrt(double x) {
		return 1.0 / ::sqrt(x);
	}
	
	inline int abs(int x) {
		return (x >= 0 ? x : -x);
	}
	
	inline float abs(float x) {
		return ::fabsf(x);
	}
	
	inline double abs(double x) {
		return ::fabs(x);
	}
	
	inline int mod(int x, int y) {
		return x % y;
	}
	
	inline float mod(float x, float y) {
		return ::fmodf(x, y);
	}
	
	inline double mod(double x, double y) {
		return ::fmod(x, y);
	}
	
	template <typename T>
	bool equals(const T a, const T b, const T tolerance = constant<T>::rounding_error) {
		return abs(a - b) <= tolerance;
	}
	
	template <typename T>
	bool is_zero(const T a, const T tolerance = constant<T>::rounding_error) {
		return abs(a) <= tolerance;
	}
	
	template <typename T>
	T min_(T a, T b) {
		return a < b ? a : b;
	}
	
	template <typename T>
    T max_(T a, T b) {
		return a > b ? a : b;
	}
    
	template <typename T>
    T clamp(T value, T _min, T _max) {
		return max_(_min, min_(_max, value));
	}
    
    template <typename T>
	T deg2rad(T x) {
		return x / 180 * constant<T>::pi;
	}
	
	template <typename T>
	T rad2deg(T x) {
		return x * 180 / constant<T>::pi;
	}
    
    template <typename T>
    void swap(T &a, T &b) {
        T t = a; a = b; b = t;
    }
	
	inline void sincos(float x, float &s, float &c) {
		s = ::sinf(x);
		c = ::cosf(x);
	}
	
	inline void sincos(double x, double &s, double &c) {
		s = ::sin(x);
		c = ::cos(x);
	}
	
	inline float sin(float x) {
		return ::sinf(x);
	}
	
	inline double sin(double x) {
		return ::sin(x);
	}
	
	inline float cos(float x) {
		return ::cosf(x);
	}
	
	inline double cos(double x) {
		return ::cos(x);
	}
	
    inline float asin(float x) {
        return ::asinf(x);
    }
    
    inline double asin(double x) {
        return ::asin(x);
    }
    
    inline float asin_s(float x) {
        return ::asinf(x < -1.f ? -1.f : (x > 1.f ? 1.f : x));
    }
    
    inline double asin_s(double x) {
        return ::asin(x < -1.0 ? -1.0 : (x > 1.0 ? 1.0 : x));
    }
    
	inline float acos(float x) {
		return ::acosf(x);
	}
	
	inline double acos(double x) {
		return ::acos(x);
	}
    
    inline float acos_s(float x) {
        return ::acosf(x < -1.f ? -1.f : (x > 1.f ? 1.f : x));
    }
    
    inline double acos_s(double x) {
        return ::acos(x < -1.0 ? -1.0 : (x > 1.0 ? 1.0 : x));
    }
	
	inline float atan2(float y, float x) {
		return ::atan2f(y, x);
	}
	
	inline double atan2(double y, double x) {
		return ::atan2(y, x);
	}
	
	inline float ceil(float x) {
		return ::ceilf(x);
	}
	
	inline double ceil(double x) {
		return ::ceil(x);
	}
	
	inline float floor(float x) {
		return ::floorf(x);
	}
	
	inline double floor(double x) {
		return ::floor(x);
	}
};

_vn_end

#endif
