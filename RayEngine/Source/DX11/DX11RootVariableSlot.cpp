#include "..\..\Include\DX11\DX11RootVariableSlot.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11VertexShader>::SetShaderResourceViews(ID3D11DeviceContext* pContext, ID3D11ShaderResourceView** ppSRVs, int32 count) const
		{
			pContext->VSSetShaderResources(m_Slot, count, ppSRVs);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11VertexShader>::SetSamplers(ID3D11DeviceContext* pContext, ID3D11SamplerState** ppSamplerStates, int32 count) const
		{
			pContext->VSSetSamplers(m_Slot, count, ppSamplerStates);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11VertexShader>::SetUnorderedAccessViews(ID3D11DeviceContext* pContext, ID3D11UnorderedAccessView** ppUAVs, int32 count) const
		{
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11VertexShader>::SetConstantBuffers(ID3D11DeviceContext* pContext, ID3D11Buffer** ppBuffers, int32 count) const
		{
			pContext->VSSetConstantBuffers(m_Slot, count, ppBuffers);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11HullShader>::SetShaderResourceViews(ID3D11DeviceContext* pContext, ID3D11ShaderResourceView** ppSRVs, int32 count) const
		{
			pContext->HSSetShaderResources(m_Slot, count, ppSRVs);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11HullShader>::SetSamplers(ID3D11DeviceContext* pContext, ID3D11SamplerState** ppSamplerStates, int32 count) const
		{
			pContext->HSSetSamplers(m_Slot, count, ppSamplerStates);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11HullShader>::SetUnorderedAccessViews(ID3D11DeviceContext* pContext, ID3D11UnorderedAccessView** ppUAVs, int32 count) const
		{
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11HullShader>::SetConstantBuffers(ID3D11DeviceContext* pContext, ID3D11Buffer** ppBuffers, int32 count) const
		{
			pContext->HSSetConstantBuffers(m_Slot, count, ppBuffers);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11DomainShader>::SetShaderResourceViews(ID3D11DeviceContext* pContext, ID3D11ShaderResourceView** ppSRVs, int32 count) const
		{
			pContext->DSSetShaderResources(m_Slot, count, ppSRVs);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11DomainShader>::SetSamplers(ID3D11DeviceContext* pContext, ID3D11SamplerState** ppSamplerStates, int32 count) const
		{
			pContext->DSSetSamplers(m_Slot, count, ppSamplerStates);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11HullShader>::SetUnorderedAccessViews(ID3D11DeviceContext* pContext, ID3D11UnorderedAccessView** ppUAVs, int32 count) const
		{
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11HullShader>::SetConstantBuffers(ID3D11DeviceContext* pContext, ID3D11Buffer** ppBuffers, int32 count) const
		{
			pContext->DSSetConstantBuffers(m_Slot, count, ppBuffers);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11GeometryShader>::SetShaderResourceViews(ID3D11DeviceContext* pContext, ID3D11ShaderResourceView** ppSRVs, int32 count) const
		{
			pContext->GSSetShaderResources(m_Slot, count, ppSRVs);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11GeometryShader>::SetSamplers(ID3D11DeviceContext* pContext, ID3D11SamplerState** ppSamplerStates, int32 count) const
		{
			pContext->GSSetSamplers(m_Slot, count, ppSamplerStates);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11GeometryShader>::SetUnorderedAccessViews(ID3D11DeviceContext* pContext, ID3D11UnorderedAccessView** ppUAVs, int32 count) const
		{
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11GeometryShader>::SetConstantBuffers(ID3D11DeviceContext* pContext, ID3D11Buffer** ppBuffers, int32 count) const
		{
			pContext->GSSetConstantBuffers(m_Slot, count, ppBuffers);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11PixelShader>::SetShaderResourceViews(ID3D11DeviceContext* pContext, ID3D11ShaderResourceView** ppSRVs, int32 count) const
		{
			pContext->PSSetShaderResources(m_Slot, count, ppSRVs);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11PixelShader>::SetSamplers(ID3D11DeviceContext* pContext, ID3D11SamplerState** ppSamplerStates, int32 count) const
		{
			pContext->PSSetSamplers(m_Slot, count, ppSamplerStates);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11PixelShader>::SetUnorderedAccessViews(ID3D11DeviceContext* pContext, ID3D11UnorderedAccessView** ppUAVs, int32 count) const
		{
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11PixelShader>::SetConstantBuffers(ID3D11DeviceContext* pContext, ID3D11Buffer** ppBuffers, int32 count) const
		{
			pContext->PSSetConstantBuffers(m_Slot, count, ppBuffers);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11ComputeShader>::SetShaderResourceViews(ID3D11DeviceContext* pContext, ID3D11ShaderResourceView** ppSRVs, int32 count) const
		{
			pContext->CSSetShaderResources(m_Slot, count, ppSRVs);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11ComputeShader>::SetSamplers(ID3D11DeviceContext* pContext, ID3D11SamplerState** ppSamplerStates, int32 count) const
		{
			pContext->CSSetSamplers(m_Slot, count, ppSamplerStates);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11ComputeShader>::SetUnorderedAccessViews(ID3D11DeviceContext* pContext, ID3D11UnorderedAccessView** ppUAVs, int32 count) const
		{
			pContext->CSSetUnorderedAccessViews(m_Slot, count, ppUAVs, nullptr);
		}



		/////////////////////////////////////////////////////////////
		template<>
		void DX11RootVariableSlotImpl<ID3D11ComputeShader>::SetConstantBuffers(ID3D11DeviceContext* pContext, ID3D11Buffer** ppBuffers, int32 count) const
		{
			pContext->CSSetConstantBuffers(m_Slot, count, ppBuffers);
		}
	}
}

#endif