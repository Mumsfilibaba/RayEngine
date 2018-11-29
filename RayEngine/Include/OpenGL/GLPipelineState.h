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
#include "../Graphics/IPipelineState.h"
#include "GLCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class GLDevice;
		class GLShader;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct GLInputLayoutElement
		{
			int32 Type;
			int32 Size;
			int32 Stride;
			int32 Offset;
			int32 Divisor;
			bool Normalized;
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct GLInputLayout
		{
			GLInputLayoutElement* pElements;
			uint32 ElementCount;
		};


		/*///////////////////////////////////////////////////////////
			
			DepthEnable - true	glEnable(GL_DEPTH_TEST)
						- false glDiable(GL_DEPTH_TEST)

			DepthMask	- GL_FALSE or GL_TRUE - glDepthMask(DepthMask)

			DepthFunc	- glDepthFunc(DepthFunc)

			StencilEnable	- true	glEnable(GL_STENCIL_TEST)
							- false glDiable(GL_STENCIL_TEST)

			WriteMask	- glStencilMask(WriteMask)

			GLStencilFace:
				glStencilOpSeparate(-, StencilFailOp, DepthFailOp, PassOp)
				glStencilFuncSeparate(-, StencilFunc, 0, ReadMask)

		///////////////////////////////////////////////////////////*/
		struct GLDepthState
		{
			struct GLStencilFace
			{
				uint32 StencilFailOp;
				uint32 DepthFailOp;
				uint32 PassOp;
				
				uint32 ReadMask;
				uint32 StencilFunc;
			};

			bool DepthEnable;
			int32 DepthMask;
			int32 DepthFunc;

			bool StencilEnable;
			uint32 WriteMask;
			GLStencilFace FrontFace;
			GLStencilFace BackFace;
		};


		/*///////////////////////////////////////////////////////////
			
			ConservativeRasterizerEnable
				true	-	glEnable(CONSERVATIVE_RASTERIZATION_NV)
				false	-	glDiable(CONSERVATIVE_RASTERIZATION_NV)
							GL_NV_conservative_raster must be present

			PolygonMode - glPolygonMode(GL_FRONT_AND_BACK, PolygonMode)

			CullMode - glCullFace(CullMode)

			FrontFace - glFrontFace(FrontFace)

			DepthClipEnable
				false	- glEnable(GL_DEPTH_CLAMP)
				true	- glDisable(GL_DEPTH_CLAMP)

			DepthBias, DepthBiasClamp, SlopeScaleDepthBias -
				PolygonOffsetClampEXT(SlopeScaleDepthBias, DepthBias, DepthBiasClamp)

			AntialiasedLineEnable
				true -  glEnable(GL_LINE_SMOOTH)
						glHint(GL_LINE_SMOOTH_HINT, GL_NICEST)
						Blending must be enabled
				false - glDisable(GL_LINE_SMOOTH)

			ScissorEnable 
				true	- glEnable(GL_SCISSOR_TEST)
				false	- glDisable(GL_SCISSOR_TEST)

			MultisampleEnable 
				true	- glEnable(GL_MULTISAMPLE)
				false	- glDisable(GL_MULTISAMPLE)

		///////////////////////////////////////////////////////////*/
		struct GLRasterizerState
		{
			bool ConservativeRasterizerEnable;
			uint32 PolygonMode;
			uint32 CullMode;
			uint32 FrontFace;

			bool DepthClipEnable;
			float DepthBias;
			float DepthBiasClamp;
			float SlopeScaleDepthBias;

			bool AntialiasedLineEnable;
			bool ScissorEnable;
			bool MultisampleEnable;
		};


		/*///////////////////////////////////////////////////////////

			AlphaToCoverageEnable
				true	- glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE)
				false	- glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE)

			IndependentBlendEnable 
				true - RenderTargets index 1 - 7 are used if supported
				false - RenderTargets[0] is only used

			LogicOpEnable 
				true  - glEnable(GL_LOGIC_OP)
				false - glDisable(GL_LOGIC_OP)

			BlendFactor - glBlendColor(BlendFactor[0], BlendFactor[1], BlendFactor[2], BlendFactor[3])

			RenderTargets 
				if IndependentBlendEnable == true
					if glBlendFuncSeparatei != nullptr
						glBlendFuncSeparatei(i, RenderTargets[i].SrcBlend, RenderTargets[i].DstBlend, RenderTargets[i].SrcAlphaBlend, RenderTargets[i].DstAlphaBlend)
					else
						//Same as IndependentBlendEnable == false
						glBlendFuncSeparate(RenderTargets[0].SrcBlend, RenderTargets[0].DstBlend, RenderTargets[0].SrcAlphaBlend, RenderTargets[0].DstAlphaBlend)
					
					if glBlendEquationSeparatei != nullptr
						glBlendEquationSeparatei(i, RenderTargets[i].BlendOperation, RenderTargets[i].AlphaBlendOperation)
					else
						//Same as IndependentBlendEnable == false
						glBlendEquationSeparate(RenderTargets[0].BlendOperation, RenderTargets[0].AlphaBlendOperation)

					glColorMaski(i, RenderTargets[i].WriteMask[0], RenderTargets[i].WriteMask[1], RenderTargets[i].WriteMask[2], RenderTargets[i].WriteMask[3])

					if RenderTargets[i].blendEnable == true
						glEnablei(GL_BLEND, i)
					else
						glDisblei(GL_BLEND, i)

				else
					glBlendFuncSeparate(RenderTargets[0].SrcBlend, RenderTargets[0].DstBlend, RenderTargets[0].SrcAlphaBlend, RenderTargets[0].DstAlphaBlend)
					glBlendEquationSeparate(RenderTargets[0].BlendOperation, RenderTargets[0].AlphaBlendOperation)
					glColorMask(RenderTargets[0].WriteMask[0], RenderTargets[0].WriteMask[1], RenderTargets[0].WriteMask[2], RenderTargets[0].WriteMask[3])

					if RenderTargets[0].blendEnable == true
						glEnable(GL_BLEND)
					else
						glDisble(GL_BLEND)

		///////////////////////////////////////////////////////////*/
		struct GLBlendState
		{
			struct GLRenderTargetBlendInfo
			{
				bool blendEnable;
				uint32 SrcBlend;
				uint32 DstBlend;
				uint32 BlendOperation;
				uint32 SrcAlphaBlend;
				uint32 DstAlphaBlend;
				uint32 AlphaBlendOperation;
				GLboolean WriteMask[4];
			};

			bool AlphaToCoverageEnable;
			bool IndependentBlendEnable;
			bool LogicOpEnable;
			float BlendFactor[4];
			GLRenderTargetBlendInfo RenderTargets[RE_MAX_RENDERTARGETS];
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class GLPipelineState final : public IPipelineState
		{
			RE_IMPLEMENT_INTERFACE(GLPipelineState);

		public:
			GLPipelineState(IDevice* pDevice, const PipelineStateDesc* pDesc);
			~GLPipelineState();

			inline const GLInputLayout& GetGLInputLayout() const
			{
				return m_InputLayout;
			}

			inline const GLDepthState& GetGLDepthState() const
			{
				return m_DepthState;
			}

			inline const GLRasterizerState& GetGLRasterizerState() const
			{
				return m_RasterizerState;
			}

			inline const GLBlendState& GetGLBlendState() const
			{
				return m_BlendState;
			}

			inline uint32 GetGLProgram() const
			{
				return m_Program;
			}

			inline PIPELINE_TYPE GetPipelineType() const
			{
				return m_Desc.Type;
			}

			void SetName(const std::string& name) override final;

			void QueryDevice(IDevice** ppDevice) const override final;

			void GetDesc(PipelineStateDesc* pDesc) const;

			IObject::CounterType GetReferenceCount() const override final;

			IObject::CounterType Release() override final;

			IObject::CounterType AddRef() override final;

		private:
			void Create(const PipelineStateDesc* pDesc);

			void CreateGraphicsPipeline();

			void CreateComputePipeline();

			void LinkShaders();

			void CreateInputLayout();

			void CreateDepthState();

			void CreateRasterizerState();

			void CreateBlendState();

		private:
			GLDevice* m_Device;
			GLShader* m_VS;
			GLShader* m_HS;
			GLShader* m_DS;
			GLShader* m_GS;
			GLShader* m_PS;
			GLShader* m_CS;

			GLInputLayout m_InputLayout;
			GLDepthState m_DepthState;
			GLRasterizerState m_RasterizerState;
			GLBlendState m_BlendState;

			PipelineStateDesc m_Desc;

			uint32 m_Program;

			IObject::CounterType m_References;
		};
	}
}