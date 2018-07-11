#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct CommanQueueInfo
		{

		};



		/////////////////////////////////////////////////////////////
		class ICommandQueue
		{
		public:
			ICommandQueue(ICommandQueue&& other) = delete;
			ICommandQueue(const ICommandQueue& other) = delete;
			ICommandQueue& operator=(ICommandQueue&& other) = delete;
			ICommandQueue& operator=(const ICommandQueue& other) = delete;

		public:
			ICommandQueue() {}
			virtual ~ICommandQueue() {}

			//Close list for commandsubmition
			virtual void Close() const = 0;
		};
	}
}