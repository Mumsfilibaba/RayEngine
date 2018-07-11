#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace Graphics
	{
		class IFence
		{
		public:
			IFence(IFence&& other) = delete;
			IFence(const IFence& other) = delete;
			IFence& operator=(IFence&& other) = delete;
			IFence& operator=(const IFence& other) = delete;

		public:
			IFence() {}
			virtual ~IFence() {}
		};
	}
}