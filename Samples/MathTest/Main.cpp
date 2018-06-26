#include <Math/Vector4.h>
#include <Math/Matrix4.h>
#include <Math/Matrix2.h>
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
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
				m1.GetElement(x, y) = (x * 4) + y + 1;
		}

		Matrix4 m2(3.0f);
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
				m2.GetElement(x, y) = (x * 4) + y + 16;
		}


		Matrix2 m(Vector2(1, 2), Vector2(3, 4));
		Matrix2 mm(Vector2(5, 6), Vector2(7, 8));

		mm *= m;

		output += "mm:\n" + mm.ToString() + '\n';

		clock.Reset();
		
		clock.Tick();
		for (int32 i = 0; i < iterations; i++)
		{
			mm *= m;
		}
		clock.Tick();

		output += "Result:\n" + mm.ToString();
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