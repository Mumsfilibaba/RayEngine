#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		enum BUFFERTYPE : int32
		{
			BUFFERTYPE_UNKNOWN = 0,
			BUFFERTYPE_UNIFORMBUFFER = 1,
			BUFFERTYPE_VERTEXBUFFER = 2,
			BUFFERTYPE_INDEXBUFFER = 3,
		};



		/////////////////////////////////////////////////////////////
		struct BufferInfo
		{
			BUFFERTYPE Type = BUFFERTYPE_UNKNOWN;
			RESOURCE_USAGE Usage = RESOURCE_USAGE_UNKNOWN;
			int32 Count = 0;
			int32 Stride = 0;
		};



		/////////////////////////////////////////////////////////////
		class IBuffer
		{
		public:
			IBuffer() {}
			virtual ~IBuffer() {}
		};
	}
}