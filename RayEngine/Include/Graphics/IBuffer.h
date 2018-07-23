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
			std::string Name = "";
			BUFFERTYPE Type = BUFFERTYPE_UNKNOWN;
			RESOURCE_USAGE Usage = RESOURCE_USAGE_UNKNOWN;
			int32 Count = 0;
			int32 Stride = 0;
		};



		/////////////////////////////////////////////////////////////
		class IBuffer
		{
		public:
			IBuffer(IBuffer&& other) = delete;
			IBuffer(const IBuffer& other) = delete;
			IBuffer& operator=(IBuffer&& other) = delete;
			IBuffer& operator=(const IBuffer& other) = delete;

		public:
			IBuffer() {}
			virtual ~IBuffer() {}
		};
	}
}