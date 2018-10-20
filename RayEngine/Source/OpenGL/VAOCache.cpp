#include "..\..\Include\OpenGL\VAOCache.h"

#define GL_ATTRIB_POINTER(offset) (void*)((char*)(nullptr) + offset)

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VAOCache::VAOCache()
			: m_VAO(0)
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VAOCache::~VAOCache()
		{
			if (m_VAO != 0)
			{
				glDeleteVertexArrays(1, &m_VAO);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		uint32 VAOCache::GetVAO(const GLPipelineState* pPipelineState, const GLBuffer* const * ppVertexBuffers, const GLBuffer* pIndexBuffer) const
		{
			if (m_VAO == 0)
			{
				uint32 vao = 0;

				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);

				uint32 vbo = ppVertexBuffers[0]->GetGLBuffer();
				glBindBuffer(GL_ARRAY_BUFFER, vbo);

				const GLInputLayout* pInputLayout = &pPipelineState->GetGLInputLayout();
				for (uint32 i = 0; i < pInputLayout->ElementCount; i++)
				{
					glVertexAttribPointer(i, pInputLayout->pElements[i].Size, pInputLayout->pElements[i].Type, pInputLayout->pElements[i].Normalized,
						pInputLayout->pElements[i].Stride, GL_ATTRIB_POINTER(pInputLayout->pElements[i].Offset));
					glEnableVertexAttribArray(i);
				}

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);

				m_VAO = vao;
			}

			return m_VAO;
		}
	}
}