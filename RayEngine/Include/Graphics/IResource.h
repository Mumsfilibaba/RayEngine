#pragma once

#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{
		class IResource : public IDeviceObject
		{
		public:
			IResource(IResource&& other) = delete;
			IResource(const IResource& other) = delete;
			IResource& operator=(IResource&& other) = delete;
			IResource& operator=(const IResource& other) = delete;

		public:
			IResource() {}
			virtual ~IResource() {}
		};
	}
}