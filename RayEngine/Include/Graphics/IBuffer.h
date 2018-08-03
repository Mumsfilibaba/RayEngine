#pragma once

#include "..\RefCounter.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IDevice;



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
			int32 ByteStride = 0;
		};



		/////////////////////////////////////////////////////////////
		class IBuffer : public RefCounter
		{
		public:
			IBuffer(IBuffer&& other) = delete;
			IBuffer(const IBuffer& other) = delete;
			IBuffer& operator=(IBuffer&& other) = delete;
			IBuffer& operator=(const IBuffer& other) = delete;

		public:
			IBuffer() {}
			virtual ~IBuffer() {}

			//Maps the buffer from the GPU to CPU - returns the pointer to the buffer
			virtual void* Map(int32 subresource) = 0;
			//Unmaps the buffer from the GPU to CPU
			virtual void Unmap() = 0;
			//Returns the device that created the buffer
			virtual IDevice* GetDevice() const = 0;
		};
	}
}