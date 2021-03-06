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
#include "DX12Common.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Resource
		{
			RE_UNIQUE_OBJECT(DX12Resource);

		protected:
			inline DX12Resource()
				: m_Resource(nullptr),
				m_State(D3D12_RESOURCE_STATE_COMMON)
			{
			}

			inline ~DX12Resource()
			{
			}

		public:
			inline ID3D12Resource* GetD3D12Resource() const
			{
				return m_Resource.Get();
			}

			inline D3D12_RESOURCE_STATES GetD3D12State() const
			{
				return m_State;
			}

			inline void SetD3D12State(D3D12_RESOURCE_STATES state) const
			{
				m_State = state;
			}

		protected:
			Microsoft::WRL::ComPtr<ID3D12Resource> m_Resource;

			mutable D3D12_RESOURCE_STATES m_State;
		};
	}
}

#endif