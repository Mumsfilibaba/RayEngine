#pragma once

#define PI 3.1415926535898
#define E 2.7182818284590
#define PI_F 3.141592f
#define E_f 2.718281f
#define PIx2 2.0 * PI
#define PIx2_F 2.0f * PI_F

#include <cmath>

#if defined(_ANDROID)
#define forceinline
#elif defined(_WIN32) || defined(_WIN64)
#define forceinline __forceinline
#endif

namespace Math
{
	double ToDegrees(double degrees);
	double ToRadians(double radians);

	float ToDegreesF(float degrees);
	float ToRadiansF(float radians);

	double Max(double d1, double d2);
	float MaxF(float f1, float f2);
	
	double Min(double d1, double d2);
	float MinF(float f1, float f2);
}