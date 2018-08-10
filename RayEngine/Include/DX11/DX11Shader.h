#pragma once

#include <vector>
#include "..\Graphics\IShader.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Device.h"
#include "..\DXBase\DXShaderBase.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11Shader final : public IShader, public DXShaderBase
		{ 
		public:
			DX11Shader(const DX11Shader& other) = delete;
			DX11Shader& operator=(const DX11Shader& other) = delete;
			DX11Shader(DX11Shader&& other) = delete;
			DX11Shader& operator=(DX11Shader&& other) = delete;

		public:
			DX11Shader(IDevice* pDevice, const ShaderInfo& info);
			~DX11Shader();

			ID3D11SamplerState* const * GetStaticSamplers() const;
			int32 GetStaticSamplerCount() const;

			ID3D11VertexShader* GetD3D11VertexShader() const;
			ID3D11HullShader* GetD3D11HullShader() const;
			ID3D11DomainShader* GetD3D11DomainShader() const;
			ID3D11GeometryShader* GetD3D11GeometryShader() const;
			ID3D11PixelShader* GetD3D11PixelShader() const;
			ID3D11ComputeShader* GetD3D11ComputeShader() const;

			SHADER_TYPE GetType() const override final;
			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const ShaderInfo& info);
			ID3D11SamplerState* CreateSampler(const StaticSampler& sampler);

		private:
			DX11Device* m_Device;
			std::vector<ID3D11SamplerState*> m_StaticSamplers;
			union
			{
				ID3D11VertexShader* m_VertexShader;
				ID3D11HullShader* m_HullShader;
				ID3D11DomainShader* m_DomainShader;
				ID3D11GeometryShader* m_GeometryShader;
				ID3D11PixelShader* m_PixelShader;
				ID3D11ComputeShader* m_ComputeShader;
			};
		};
	}
}

#endif