//
//  vnMath.cpp
//  vnlib
//
//  Created by Wayne on 14-10-23.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnMath.h"

_vn_begin

namespace math {

	const float constant<float>::rounding_error = 0.000001f;
	const float constant<float>::pi = 3.1415926535897932f;
	const float constant<float>::half_pi = 1.5707963267948965f;
	const float constant<float>::two_pi = 6.2831853071795862f;

	const double constant<double>::rounding_error = 0.000000000000001;
	const double constant<double>::pi = 3.1415926535897932;
	const double constant<double>::half_pi = 1.5707963267948965;
	const double constant<double>::two_pi = 6.2831853071795862;
}

_vn_end