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
#include <vector>
#include "..\Graphics\IShader.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"
#include "..\DXBase\DXShaderBase.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX11Device;
		class DX11ShaderConstantBlock;



		/////////////////////////////////////////////////////////////
		template<typename T>
		constexpr bool IsD3D11Shader()
		{
			using namespace Type;

			return IsSame<ID3D11VertexShader, T>() ||
				IsSame<ID3D11HullShader, T>() ||
				IsSame<ID3D11DomainShader, T>() ||
				IsSame<ID3D11GeometryShader, T>() || 
				IsSame<ID3D11PixelShader, T>() || 
				IsSame<ID3D11ComputeShader, T>();
		}



		/////////////////////////////////////////////////////////////
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

			template<typename D3D11ShaderType>
			inline D3D11ShaderType* GetD3D11Shader() const
			{
				static_assert(IsD3D11Shader<D3D11ShaderType>(), "GetD3D11Shader() - T must be a valid D3D11 shadertype. For example ID3D11VertexShader");
				return reinterpret_cast<D3D11ShaderType*>(m_Shader);
			}

			SHADER_TYPE GetType() const override final;
			
			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(const ShaderInfo& info);

		private:
			DX11Device* m_Device;
			ID3D11DeviceChild* m_Shader;

			IObject::CounterType m_References;
		};
	}
}

#endif