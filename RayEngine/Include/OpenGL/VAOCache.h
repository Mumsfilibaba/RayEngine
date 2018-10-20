#pragma once

#include "GLCommon.h"
#include "GLPipelineState.h"
#include "GLBuffer.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VAOCache final
		{
			RE_UNIQUE_OBJECT(VAOCache);

		public:
			VAOCache();
			~VAOCache();

			uint32 GetVAO(const GLPipelineState* pPipelineState, const GLBuffer* const * ppVertexBuffers, const GLBuffer* pIndexBuffer) const;

		private:
			mutable uint32 m_VAO;
		};
	}
}