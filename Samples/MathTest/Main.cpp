#include <Math/Vector4.h>
#include <Math/Matrix4.h>
#include <System/Clock.h>

#include <iostream>
#include <fstream>
#include <string>

#pragma comment(lib, "RayEngine.lib")

int main(int args, char** argsv)
{
	using namespace RayEngine;
	using namespace Math;

	int32 iterations = 0;
	for (int32 i = 0; i < args; i++)
	{
		if (strstr(argsv[i], "i-"))
			iterations = atoi(argsv[i] + 3);
	}

	std::string output;
	output += "Starting test with '" + std::to_string(iterations) + "' number of iterations\n";

	Clock clock;
	for (int32 j = 0; j < 10; j++)
	{
		Vector4 v1(1.0f, 1.0f, 1.0f, 1.0f);
		Vector4 v2(2.0f, 2.0f, 2.0f, 2.0f);
		Vector4 v3(3.0f, 3.0f, 3.0f, 3.0f);

		Matrix4 m1(2.0f);
		Matrix4 m2(1000.1f);

		std::cout << m1.InverseOf().ToString() << "\n\n";

		float len = 0.0f;
		clock.Reset();
		
		clock.Tick();
		for (int32 i = 0; i < iterations; i++)
		{
			m1 *= 1.1f;
			m2 = m1.Invert();
		}
		clock.Tick();

		output += "Result:\n" + m2.ToString();
		output += "\nTime: " + std::to_string(clock.GetDeltaTime().GetAsNanoSeconds()) + "ns\n";
	}

	std::cout << output;

	std::ofstream outputFile("Log.txt", std::ofstream::out);
	if (outputFile.is_open())
	{
		outputFile << output;
		outputFile.close();
	}

	std::cin.ignore();
	return 0;
}