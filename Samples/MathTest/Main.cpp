#include <Math/Vector4.h>
#include <System/Clock.h>

#include <iostream>

int main(int args, char** argsv)
{
	using namespace RayEngine;
	using namespace Math;

	Clock clock;
	Vector4 v1(1.0f, 1.5f, 2.0f, 2.5f);
	Vector4 v2;
	
	clock.Tick();

	for (int i = 0; i < 1000; i++)
	{

	}

	clock.Tick();

	std::cout << "Time: " << clock.GetDeltaTime().GetAsMilliSeconds() << "ms";
	return 0;
}