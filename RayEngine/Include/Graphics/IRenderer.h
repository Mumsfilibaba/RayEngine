#pragma once
#include <RayEngine.h>
#include <Interfaces/IObject.h>

namespace RayEngine
{
	namespace Graphics
	{
		class IRenderer : public IObject
		{
			RE_INTERFACE(IRenderer);

		public:
			IRenderer() {}
			~IRenderer() {};

			virtual void Clear(float color[4]) const = 0;

			virtual void Draw() const = 0;
			
			virtual void Present() const = 0;
		};
	}
}