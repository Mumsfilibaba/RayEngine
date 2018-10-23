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
#include "..\Defines.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IDX11RootVariableSlot
		{
		public:
			IDX11RootVariableSlot(const IDX11RootVariableSlot& other) = delete;
			IDX11RootVariableSlot& operator=(const IDX11RootVariableSlot& other) = delete;
			IDX11RootVariableSlot(IDX11RootVariableSlot&& other) = delete;
			IDX11RootVariableSlot& operator=(IDX11RootVariableSlot&& other) = delete;

		public:
			IDX11RootVariableSlot() {}
			virtual ~IDX11RootVariableSlot() {}

			virtual void SetShaderResourceViews(ID3D11DeviceContext* pContext, ID3D11ShaderResourceView** ppSRVs, int32 count) const = 0;
			
			virtual void SetSamplers(ID3D11DeviceContext* pContext, ID3D11SamplerState** ppSamplerStates, int32 count) const = 0;
			
			virtual void SetUnorderedAccessViews(ID3D11DeviceContext* pContext, ID3D11UnorderedAccessView** ppUAVs, int32 count) const = 0;
			
			virtual void SetConstantBuffers(ID3D11DeviceContext* pContext, ID3D11Buffer** ppBuffers, int32 count) const = 0;
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		template<typename TShader>
		class DX11RootVariableSlotImpl final : public IDX11RootVariableSlot
		{
		public:
			DX11RootVariableSlotImpl(const DX11RootVariableSlotImpl& other) = delete;
			DX11RootVariableSlotImpl& operator=(const DX11RootVariableSlotImpl& other) = delete;
			DX11RootVariableSlotImpl(DX11RootVariableSlotImpl&& other) = delete;
			DX11RootVariableSlotImpl& operator=(DX11RootVariableSlotImpl&& other) = delete;

		public:
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			inline DX11RootVariableSlotImpl(int32 baseSlot)
				: m_Slot(baseSlot)
			{
			}


			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			inline ~DX11RootVariableSlotImpl()
			{
			}


			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			void SetShaderResourceViews(ID3D11DeviceContext* pContext, ID3D11ShaderResourceView** ppSRVs, int32 count) const override final;
			
			void SetSamplers(ID3D11DeviceContext* pContext, ID3D11SamplerState** ppSamplerStates, int32 count) const override final;
			
			void SetUnorderedAccessViews(ID3D11DeviceContext* pContext, ID3D11UnorderedAccessView** ppUAVs, int32 count) const override final;
			
			void SetConstantBuffers(ID3D11DeviceContext* pContext, ID3D11Buffer** ppBuffers, int32 count) const override final;

		private:
			int32 m_Slot;
		};
	}
}

#endif