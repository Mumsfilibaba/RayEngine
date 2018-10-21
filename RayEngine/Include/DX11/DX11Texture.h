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
#include "..\..\Include\Graphics\ITexture.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11Device;



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11Texture final : public ITexture
		{
			RE_IMPLEMENT_INTERFACE(DX11Texture);

		public:
			DX11Texture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureDesc& info);
			DX11Texture(IDevice* pDevice, ID3D11Texture2D* pResource);
			~DX11Texture();

			ID3D11Texture1D* GetD3D11Texture1D() const;
			
			ID3D11Texture2D* GetD3D11Texture2D() const;
			
			ID3D11Texture3D* GetD3D11Texture3D() const;

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(const ResourceData* const pInitialData, const TextureDesc& info);

		private:
			DX11Device* m_Device;
			union
			{
				ID3D11Texture1D* m_Texture1D;
				ID3D11Texture2D* m_Texture2D;
				ID3D11Texture3D* m_Texture3D;
			};

			TEXTURE_TYPE m_Type;
			
			IObject::CounterType mReferences;
		};
	}
}

#endif