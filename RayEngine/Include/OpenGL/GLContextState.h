#pragma once
#include "GLCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class GLContextState final
		{
		private:
			enum GL_STATE : int64
			{
				GL_STATE_NONE = (0 << 0),
				GL_STATE_DEPTH_TEST_ENABLED = (1 << 0),
				GL_STATE_CULL_FACE_ENABLED = (1 << 1),
				GL_STATE_BLEND_ENABLED = (1 << 2),

			};

		public:
			GLContextState(const GLContextState& other) = delete;
			GLContextState& operator=(const GLContextState& other) = delete;
			GLContextState(GLContextState&& other) = delete;
			GLContextState& operator=(GLContextState&& other) = delete;

		public:
			GLContextState();
			~GLContextState();
			
			inline bool DepthTest() const
			{
				return m_StateFlags & GL_STATE_DEPTH_TEST_ENABLED;
			}

			inline bool CullFace() const
			{
				return m_StateFlags & GL_STATE_CULL_FACE_ENABLED;
			}

			inline bool Blend() const
			{
				return m_StateFlags & GL_STATE_BLEND_ENABLED;
			}

		private:
			int64 m_StateFlags;
		};
	}
}