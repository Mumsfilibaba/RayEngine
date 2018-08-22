/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#include "..\..\Include\Math\MathUtilities.h"

namespace Math
{
	/////////////////////////////////////////////////////////////
	double ToDegrees(double radians)
	{
		return (180.0 * radians) / PI;
	}



	/////////////////////////////////////////////////////////////
	double ToRadians(double degrees)
	{
		return (PI * degrees) / 180.0;
	}



	/////////////////////////////////////////////////////////////
	float ToDegreesF(float radians)
	{
		return (180.0f * radians) / PI_F;
	}



	/////////////////////////////////////////////////////////////
	float ToRadiansF(float degrees)
	{
		return (PI_F * degrees) / 180.0f;
	}



	/////////////////////////////////////////////////////////////
	float MaxF(float f1, float f2)
	{
		return (f1 > f2) ? f1 : f2;
	}



	/////////////////////////////////////////////////////////////
	float MinF(float f1, float f2)
	{
		return (f1 < f2) ? f1 : f2;
	}




	/////////////////////////////////////////////////////////////
	double Max(double d1, double d2)
	{
		return (d1 > d2) ? d1 : d2;
	}



	/////////////////////////////////////////////////////////////
	double Min(double d1, double d2)
	{
		return (d1 < d2) ? d1 : d2;
	}
}