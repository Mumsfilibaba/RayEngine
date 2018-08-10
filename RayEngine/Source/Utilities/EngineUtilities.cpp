#include "..\..\Include\Utilities\EngineUtilities.h"
#include <sstream>
#include <fstream>

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	std::string ReadFullFile(const std::string& filename, const std::string& filepath)
	{
		std::ifstream file(filepath + filename, std::ios::in);
		if (file.is_open())
		{
			std::stringstream buf;
			buf << file.rdbuf();

			file.close();

			return buf.str();
		}

		return std::string();
	}
}