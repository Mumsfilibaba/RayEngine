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

#pragma once
#include "Vector2.h"
#include "Color.h"

namespace Math
{
	class RAYENGINE_API RandomGenerator
	{
	public:
		RandomGenerator();
		RandomGenerator(int32 seed);
		
		void SetSeed(int32 seed);
		int GetSeed() const;

		int32 GenerateInt() const;
		int32 GenerateInt(int32 min, int32 max) const;
		float GenerateFloat() const;
		float GenerateFloat(float min, float max) const;
		double GenerateDouble() const;
		double GenerateDouble(double min, double max) const;
		Vector2 GenerateVector2() const;
		Vector3 GenerateVector3() const;
		Vector4 GenerateVector4() const;
		Color GenerateColor() const;
		ColorF GenerateColorF() const;

	private:
		void SetStaticSeed() const;

	private:
		static int32 s_CurrentSeed;

	private:
		int32 m_Seed;
	};
}