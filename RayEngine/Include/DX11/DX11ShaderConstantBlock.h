#pragma once

#include <vector>
#include "..\Graphics\IDeviceObject.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX11Device;



		/////////////////////////////////////////////////////////////
		class DX11ShaderConstantBlock final : public IDeviceObject
		{
		public:
			DX11ShaderConstantBlock(const DX11ShaderConstantBlock& other) = delete;
			DX11ShaderConstantBlock& operator=(const DX11ShaderConstantBlock& other) = delete;
			DX11ShaderConstantBlock(DX11ShaderConstantBlock&& other) = delete;
			DX11ShaderConstantBlock& operator=(DX11ShaderConstantBlock&& other) = delete;

		public:
			DX11ShaderConstantBlock(DX11Device* pDevice, int32 numConstants);
			~DX11ShaderConstantBlock();

			void SetConstant(uint32 constant, uint32 index);

			ID3D11Buffer* GetD3D11Buffer() const;
			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(int32 numConstants);

		private:
			DX11Device* m_Device;
			ID3D11Buffer* m_ConstantBuffer;
			std::vector<uint32> m_Constants;
		};
	}
}

#endif