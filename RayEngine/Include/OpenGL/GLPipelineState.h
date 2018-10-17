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
#include "..\Graphics\IPipelineState.h"
#include "GLCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class GLDevice;
		class GLShader;


		/////////////////////////////////////////////////////////////
		struct GLInputLayoutElement
		{
			int32 Type;
			int32 Size;
			int32 Stride;
			int32 Offset;
			int32 Divisor;
			bool Normalized;
		};


		/////////////////////////////////////////////////////////////
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
				glStencilOpSeparate(-, StencilFail, DepthFail, Pass)
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


		/////////////////////////////////////////////////////////////
		struct GLRasterizerState
		{

		};


		/////////////////////////////////////////////////////////////
		class GLPipelineState final : public IPipelineState
		{
			RE_IMPLEMENT_INTERFACE(GLPipelineState);
		
		public:
			GLPipelineState(IDevice* pDevice, const PipelineStateInfo& info);
			~GLPipelineState();

			PIPELINE_TYPE GetPipelineType() const override final;

			void SetName(const std::string& name) override final;

			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;

			IObject::CounterType Release() override final;

			IObject::CounterType AddRef() override final;

			inline const GLInputLayout& GetInputLayout() const
			{
				return m_InputLayout;
			}

			inline const GLDepthState& GetDepthState() const
			{
				return m_DepthState;
			}

			inline uint32 GetProgram() const
			{
				return m_Program;
			}

		private:
			void Create(const PipelineStateInfo& info);

			void CreateGraphicsPipeline(const PipelineStateInfo& info);

			void CreateComputePipeline(const PipelineStateInfo& info);

			void LinkShaders();

			void CreateInputLayout(const PipelineStateInfo& info);

			void CreateDepthState(const PipelineStateInfo& info);

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

			PIPELINE_TYPE m_Type;

			uint32 m_Program;

			IObject::CounterType m_References;
		};
	}
}