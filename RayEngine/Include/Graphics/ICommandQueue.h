#pragma once

#include "ITexture.h"

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

			//Transition a resource from one state to another
			virtual void TransitionResource(const ITexture* resource, RESOURCE_STATE from, 
				RESOURCE_STATE to, int32 subresource) const = 0;
			//Reset allocator and list
			virtual bool Reset() const = 0;
			//Close list for commandsubmition
			virtual bool Close() const = 0;
			//Execute the commandqueue
			virtual void Execute() const = 0;
		};
	}
}