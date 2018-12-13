#pragma once
#include <Graphics/IRenderer.h>

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Device;

		class DX12Renderer final : public IRenderer
		{
			RE_IMPLEMENT_INTERFACE(DX12Renderer);

		public:
			DX12Renderer(DX12Device* pDevice);
			~DX12Renderer();

			void Clear(float color[4]) const;

			void Draw() const;

			void Present() const;

			CounterType AddRef();

			CounterType Release();

		private:
			CounterType m_References;
		};
	}
}