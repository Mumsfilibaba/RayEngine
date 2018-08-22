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
#include "IResource.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		enum BUFFER_USAGE : int32
		{
			BUFFER_USAGE_UNKNOWN = 0,
			BUFFER_USAGE_CONSTANT = 1,
			BUFFER_USAGE_VERTEX = 2,
			BUFFER_USAGE_INDEX = 3,
		};



		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
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
		class IBuffer : public IResource
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
		};
	}
}