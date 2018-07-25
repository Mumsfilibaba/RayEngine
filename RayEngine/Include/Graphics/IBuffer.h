#pragma once

#include "..\IReferenceCounter.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		enum BUFFER_USAGE : int32
		{
			BUFFER_USAGE_UNKNOWN = 0,
			BUFFER_USAGE_UNIFORM = 1,
			BUFFER_USAGE_VERTEX = 2,
			BUFFER_USAGE_INDEX = 3,
		};



		/////////////////////////////////////////////////////////////
		struct BufferInfo
		{
			std::string Name = "";
			RESOURCE_USAGE Usage = RESOURCE_USAGE_UNKNOWN;
			CPU_ACCESS_FLAG CpuAccess = CPU_ACCESS_FLAG_NONE;
			BUFFER_USAGE Type = BUFFER_USAGE_UNKNOWN;
			int32 Count = 0;
			int32 Stride = 0;
		};



		/////////////////////////////////////////////////////////////
		class IBuffer : public IReferenceCounter
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