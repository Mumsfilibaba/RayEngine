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

#include "../../Include/Utilities/EngineUtilities.h"
#include <sstream>
#include <fstream>

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	float16 FloatToHalf(float32 f)
	{
		float16 result = 0;
		int32 flt32 = *reinterpret_cast<int32*>(&f);
		
		result = ((flt32 & 0x7fffffff) >> 13) - (0x38000000 >> 13);
		result |= ((flt32 & 0x80000000) >> 16);

		return result;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	float16* FloatToHalf(float32* f, int32 count)
	{
		float16* result = new float16[count];
		for (int32 i = 0; i < count; i++)
		{
			int32 flt32 = *reinterpret_cast<int32*>(f + i);

			result[i] = ((flt32 & 0x7fffffff) >> 13) - (0x38000000 >> 13);
			result[i] |= ((flt32 & 0x80000000) >> 16);
		}

		return result;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool FormatIsFloat(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_R32G32B32A32_FLOAT:
		case FORMAT_R32G32B32_FLOAT:
		case FORMAT_R16G16B16A16_FLOAT:
		case FORMAT_R32G32_FLOAT:
		case FORMAT_R16G16_FLOAT:
		case FORMAT_R32_FLOAT:
		case FORMAT_R16_FLOAT:
		case FORMAT_D32_FLOAT:
			return true;
		default:
			return false;
		} 
		
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool FormatIsSRGB(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_B8G8R8A8_UNORM_SRGB:
		case FORMAT_R8G8B8A8_UNORM_SRGB:
				return true;

		default: return false;
		}

		return false;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 FormatComponentCount(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_R32G32B32A32_FLOAT:
		case FORMAT_R32G32B32A32_UINT:
		case FORMAT_R32G32B32A32_SINT:
		case FORMAT_R16G16B16A16_FLOAT:	
		case FORMAT_R16G16B16A16_UNORM:	
		case FORMAT_R16G16B16A16_UINT:	
		case FORMAT_R16G16B16A16_SNORM:	
		case FORMAT_R16G16B16A16_SINT:	
		case FORMAT_R8G8B8A8_UNORM:		
		case FORMAT_R8G8B8A8_UNORM_SRGB:
		case FORMAT_R8G8B8A8_UINT:		
		case FORMAT_R8G8B8A8_SNORM:		
		case FORMAT_R8G8B8A8_SINT:		
		case FORMAT_B8G8R8A8_UNORM:		
		case FORMAT_B8G8R8A8_UNORM_SRGB:
			return 4;
		
		case FORMAT_R32G32B32_FLOAT:
		case FORMAT_R32G32B32_UINT:
		case FORMAT_R32G32B32_SINT:
			return 3;

		case FORMAT_R32G32_FLOAT:
		case FORMAT_R32G32_UINT:
		case FORMAT_R32G32_SINT:
		case FORMAT_R16G16_FLOAT:
		case FORMAT_R16G16_UNORM:
		case FORMAT_R16G16_UINT:
		case FORMAT_R16G16_SNORM:
		case FORMAT_R16G16_SINT:
		case FORMAT_R8G8_UNORM:
		case FORMAT_R8G8_UINT:
		case FORMAT_R8G8_SNORM:
		case FORMAT_R8G8_SINT:
			return 2;

		case FORMAT_D32_FLOAT:
		case FORMAT_R32_FLOAT:
		case FORMAT_R32_UINT:
		case FORMAT_R32_SINT:
		case FORMAT_R16_FLOAT:
		case FORMAT_D16_UNORM:
		case FORMAT_R16_UNORM:
		case FORMAT_R16_UINT:
		case FORMAT_R16_SNORM:
		case FORMAT_R16_SINT:
		case FORMAT_R8_UNORM:
		case FORMAT_R8_UINT:
		case FORMAT_R8_SNORM:
		case FORMAT_R8_SINT:
			return 1;

		default:
			return 0;
		}

		return 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 FormatStride(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_R32G32B32A32_FLOAT:
		case FORMAT_R32G32B32A32_UINT:
		case FORMAT_R32G32B32A32_SINT:
			return 16;
		
		case FORMAT_R32G32B32_FLOAT:
		case FORMAT_R32G32B32_UINT:
		case FORMAT_R32G32B32_SINT:
			return 12;

		case FORMAT_R16G16B16A16_FLOAT:	
		case FORMAT_R16G16B16A16_UNORM:	
		case FORMAT_R16G16B16A16_UINT:	
		case FORMAT_R16G16B16A16_SNORM:	
		case FORMAT_R16G16B16A16_SINT:
		case FORMAT_R32G32_FLOAT:
		case FORMAT_R32G32_UINT:
		case FORMAT_R32G32_SINT:
			return 8;

		case FORMAT_R8G8B8A8_UNORM:		
		case FORMAT_R8G8B8A8_UNORM_SRGB:
		case FORMAT_R8G8B8A8_UINT:		
		case FORMAT_R8G8B8A8_SNORM:		
		case FORMAT_R8G8B8A8_SINT:		
		case FORMAT_B8G8R8A8_UNORM:		
		case FORMAT_B8G8R8A8_UNORM_SRGB:
		case FORMAT_R16G16_FLOAT:
		case FORMAT_R16G16_UNORM:
		case FORMAT_R16G16_UINT:
		case FORMAT_R16G16_SNORM:
		case FORMAT_R16G16_SINT:
		case FORMAT_D32_FLOAT:
		case FORMAT_R32_FLOAT:
		case FORMAT_R32_UINT:
		case FORMAT_R32_SINT:
			return 4;
			
		case FORMAT_R8G8_UNORM:
		case FORMAT_R8G8_UINT:
		case FORMAT_R8G8_SNORM:
		case FORMAT_R8G8_SINT:
		case FORMAT_R16_FLOAT:
		case FORMAT_D16_UNORM:
		case FORMAT_R16_UNORM:
		case FORMAT_R16_UINT:
		case FORMAT_R16_SNORM:
		case FORMAT_R16_SINT:
			return 2;

		case FORMAT_R8_UNORM:
		case FORMAT_R8_UINT:
		case FORMAT_R8_SNORM:
		case FORMAT_R8_SINT:
			return 1;

		default:
			return 0;
		}

		return 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FORMAT_TYPE FormatType(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_R32G32B32A32_FLOAT:
		case FORMAT_R32G32B32_FLOAT:
		case FORMAT_R32G32_FLOAT:
		case FORMAT_D32_FLOAT:
		case FORMAT_R32_FLOAT:
			return FORMAT_TYPE_FLOAT32;

		case FORMAT_R32G32B32A32_UINT:
		case FORMAT_R32G32B32_UINT:
		case FORMAT_R32G32_UINT:
		case FORMAT_R32_UINT:
			return FORMAT_TYPE_UINT32;
		
		case FORMAT_R32G32B32A32_SINT:
		case FORMAT_R32G32B32_SINT:
		case FORMAT_R32G32_SINT:
		case FORMAT_R32_SINT:
			return FORMAT_TYPE_INT32;	
		
		case FORMAT_R16G16B16A16_FLOAT:	
		case FORMAT_R16G16_FLOAT:
		case FORMAT_R16_FLOAT:
			return FORMAT_TYPE_FLOAT16;

		case FORMAT_R16G16B16A16_UNORM:	
		case FORMAT_R16G16B16A16_UINT:	
		case FORMAT_R16G16_UNORM:
		case FORMAT_R16G16_UINT:
		case FORMAT_D16_UNORM:
		case FORMAT_R16_UNORM:
		case FORMAT_R16_UINT:
			return FORMAT_TYPE_UINT16;

		case FORMAT_R16G16B16A16_SNORM:	
		case FORMAT_R16G16B16A16_SINT:
		case FORMAT_R16G16_SNORM:
		case FORMAT_R16G16_SINT:
		case FORMAT_R16_SNORM:
		case FORMAT_R16_SINT:
			return FORMAT_TYPE_INT16;

		case FORMAT_R8G8B8A8_UNORM:		
		case FORMAT_R8G8B8A8_UNORM_SRGB:
		case FORMAT_R8G8B8A8_UINT:		
		case FORMAT_B8G8R8A8_UNORM:		
		case FORMAT_B8G8R8A8_UNORM_SRGB:
		case FORMAT_R8G8_UINT:
		case FORMAT_R8G8_UNORM:
		case FORMAT_R8_UNORM:
		case FORMAT_R8_UINT:
			return FORMAT_TYPE_UINT8;

		case FORMAT_R8G8B8A8_SNORM:		
		case FORMAT_R8G8B8A8_SINT:		
		case FORMAT_R8G8_SNORM:
		case FORMAT_R8G8_SINT:
		case FORMAT_R8_SNORM:
		case FORMAT_R8_SINT:
			return FORMAT_TYPE_INT8;

		default:
			return FORMAT_TYPE_UNKNOWN;
		}

		return FORMAT_TYPE_UNKNOWN;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 FormatDepthBits(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_D32_FLOAT_S8X24_UINT:
		case FORMAT_D32_FLOAT:
			return 32;
		case FORMAT_D24_UNORM_S8_UINT:
			return 24;
		case FORMAT_D16_UNORM:
			return 16;
		default: return 0;
		}

		return 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 FormatStencilBits(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_D32_FLOAT_S8X24_UINT:
		case FORMAT_D24_UNORM_S8_UINT:
			return 8;
		default: return 0;
		}

		return 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 FormatRedComponentBits(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_R32G32B32A32_FLOAT:
		case FORMAT_R32G32B32A32_UINT:
		case FORMAT_R32G32B32A32_SINT:
		case FORMAT_R32G32B32_FLOAT:
		case FORMAT_R32G32B32_UINT:
		case FORMAT_R32G32B32_SINT:
		case FORMAT_R32G32_FLOAT:
		case FORMAT_R32G32_UINT:
		case FORMAT_R32G32_SINT:
		case FORMAT_R32_FLOAT:
		case FORMAT_R32_UINT:
		case FORMAT_R32_SINT:
			return 32;
		
		case FORMAT_R16G16B16A16_FLOAT:	
		case FORMAT_R16G16B16A16_UNORM:	
		case FORMAT_R16G16B16A16_UINT:	
		case FORMAT_R16G16B16A16_SNORM:	
		case FORMAT_R16G16B16A16_SINT:
		case FORMAT_R16G16_FLOAT:
		case FORMAT_R16G16_UNORM:
		case FORMAT_R16G16_UINT:
		case FORMAT_R16G16_SNORM:
		case FORMAT_R16G16_SINT:
		case FORMAT_R16_FLOAT:
		case FORMAT_R16_UNORM:
		case FORMAT_R16_UINT:
		case FORMAT_R16_SNORM:
		case FORMAT_R16_SINT:
			return 16;

		case FORMAT_R8G8B8A8_UNORM:		
		case FORMAT_R8G8B8A8_UNORM_SRGB:
		case FORMAT_R8G8B8A8_UINT:		
		case FORMAT_B8G8R8A8_UNORM:		
		case FORMAT_B8G8R8A8_UNORM_SRGB:
		case FORMAT_R8G8B8A8_SNORM:		
		case FORMAT_R8G8B8A8_SINT:		
		case FORMAT_R8G8_UINT:
		case FORMAT_R8G8_UNORM:
		case FORMAT_R8G8_SNORM:
		case FORMAT_R8G8_SINT:
		case FORMAT_R8_UNORM:
		case FORMAT_R8_UINT:
		case FORMAT_R8_SNORM:
		case FORMAT_R8_SINT:
			return 8;

		default:
			return 0;
		}

		return 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 FormatGreenComponentBits(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_R32G32B32A32_FLOAT:
		case FORMAT_R32G32B32A32_UINT:
		case FORMAT_R32G32B32A32_SINT:
		case FORMAT_R32G32B32_FLOAT:
		case FORMAT_R32G32B32_UINT:
		case FORMAT_R32G32B32_SINT:
		case FORMAT_R32G32_FLOAT:
		case FORMAT_R32G32_UINT:
		case FORMAT_R32G32_SINT:
			return 32;
		
		case FORMAT_R16G16B16A16_FLOAT:	
		case FORMAT_R16G16B16A16_UNORM:	
		case FORMAT_R16G16B16A16_UINT:	
		case FORMAT_R16G16B16A16_SNORM:	
		case FORMAT_R16G16B16A16_SINT:
		case FORMAT_R16G16_FLOAT:
		case FORMAT_R16G16_UNORM:
		case FORMAT_R16G16_UINT:
		case FORMAT_R16G16_SNORM:
		case FORMAT_R16G16_SINT:
			return 16;

		case FORMAT_R8G8B8A8_UNORM:		
		case FORMAT_R8G8B8A8_UNORM_SRGB:
		case FORMAT_R8G8B8A8_UINT:		
		case FORMAT_B8G8R8A8_UNORM:		
		case FORMAT_B8G8R8A8_UNORM_SRGB:
		case FORMAT_R8G8B8A8_SNORM:		
		case FORMAT_R8G8B8A8_SINT:		
		case FORMAT_R8G8_UINT:
		case FORMAT_R8G8_UNORM:
		case FORMAT_R8G8_SNORM:
			return 8;

		default:
			return 0;
		}

		return 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 FormatBlueComponentBits(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_R32G32B32A32_FLOAT:
		case FORMAT_R32G32B32A32_UINT:
		case FORMAT_R32G32B32A32_SINT:
		case FORMAT_R32G32B32_FLOAT:
		case FORMAT_R32G32B32_UINT:
		case FORMAT_R32G32B32_SINT:
			return 32;
		
		case FORMAT_R16G16B16A16_FLOAT:	
		case FORMAT_R16G16B16A16_UNORM:	
		case FORMAT_R16G16B16A16_UINT:	
		case FORMAT_R16G16B16A16_SNORM:	
		case FORMAT_R16G16B16A16_SINT:
			return 16;

		case FORMAT_R8G8B8A8_UNORM:		
		case FORMAT_R8G8B8A8_UNORM_SRGB:
		case FORMAT_R8G8B8A8_UINT:		
		case FORMAT_B8G8R8A8_UNORM:		
		case FORMAT_B8G8R8A8_UNORM_SRGB:
		case FORMAT_R8G8B8A8_SNORM:		
		case FORMAT_R8G8B8A8_SINT:
			return 8;

		default:
			return 0;
		}

		return 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 FormatAlphaComponentBits(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_R32G32B32A32_FLOAT:
		case FORMAT_R32G32B32A32_UINT:
		case FORMAT_R32G32B32A32_SINT:
			return 32;
		
		case FORMAT_R16G16B16A16_FLOAT:	
		case FORMAT_R16G16B16A16_UNORM:	
		case FORMAT_R16G16B16A16_UINT:	
		case FORMAT_R16G16B16A16_SNORM:	
		case FORMAT_R16G16B16A16_SINT:
			return 16;

		case FORMAT_R8G8B8A8_UNORM:		
		case FORMAT_R8G8B8A8_UNORM_SRGB:
		case FORMAT_R8G8B8A8_UINT:		
		case FORMAT_B8G8R8A8_UNORM:		
		case FORMAT_B8G8R8A8_UNORM_SRGB:
		case FORMAT_R8G8B8A8_SNORM:		
		case FORMAT_R8G8B8A8_SINT:
			return 8;

		default:
			return 0;
		}

		return 0;
	}
}