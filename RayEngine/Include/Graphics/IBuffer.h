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
			BUFFER_USAGE_SHADER_RESOURCE = 4,
			BUFFER_USAGE_UNORDERED_ACCESS = 5,
		};



		/*////////////////////////////////////////////////////////////

			Contains info of an bufferresource. Is used for creating
			a buffer.

			Name - Name of the buffer

			Usage - How the resource will be used. Should based
			on how many times per frame a resource will be updated.

			CpuAccess - What access that the cpu will have to the
			resource.

			Type - What type of buffer, or what the buffer will be 
			used as.

			Count - Count of elements in the buffer.

			ByteStride - Size in bytes of one element in the 
			buffer. The total size is equal to Count * ByteStride
			number of bytes.

		////////////////////////////////////////////////////////////*/
		struct BufferInfo
		{
			std::string Name;
			RESOURCE_USAGE Usage;
			CPU_ACCESS_FLAG CpuAccess;
			BUFFER_USAGE Type;
			int32 Count;
			int32 ByteStride;
		};



		/////////////////////////////////////////////////////////////
		class IBuffer : public IResource
		{
			RE_INTERFACE(IBuffer);

		public:
			IBuffer() {}
			~IBuffer() {}

			/*////////////////////////////////////////////////////////////
				
				Returns a CPU pointer to the resource. This makes it 
				possible for the CPU to read or write to the resource.
				It does however require the resource to have 
				RESOURCE_USAGE_DYNAMIC set and the appropriate CpuAccess
				flags set.

				subresource - The subreource.

				flag - How the resource should be mapped.

			////////////////////////////////////////////////////////////*/
			virtual void* Map(int32 subresource, RESOURCE_MAP_FLAG flag) = 0;


			/*////////////////////////////////////////////////////////////
				Unmaps the resource from the CPU. This makes the pointer
				returned by Map invalid.
			////////////////////////////////////////////////////////////*/
			virtual void Unmap() = 0;
		};
	}
}