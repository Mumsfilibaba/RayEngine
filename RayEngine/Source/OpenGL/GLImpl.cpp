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

#include "..\..\Include\OpenGL\GlImpl.h"

#if defined(RE_PLATFORM_WINDOWS)
#include <typeinfo>

#define LOAD_GL_FUNC(func) (func = reinterpret_cast<decltype(func)>(LoadFunction(#func)))


/////////////////////////////
///FUNCTIONPTR DEFINITIONS///
/////////////////////////////


//WGL
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = nullptr;

//Buffers
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLISBUFFERPROC glIsBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
PFNGLBINDBUFFERBASEPROC glBindBufferBase = nullptr;
PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv = nullptr;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange = nullptr;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = nullptr;

//Textures
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;

//Shaders
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLDETACHSHADERPROC glDetachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLISPROGRAMPROC glIsProgram = nullptr;
PFNGLISSHADERPROC glIsShader = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;

//Patches
PFNGLPATCHPARAMETERIPROC glPatchParameteri = nullptr;

//Pipeline
PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages = nullptr;
PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline = nullptr;
PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines = nullptr;
PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline = nullptr;
PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv = nullptr;
PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines = nullptr;
PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline = nullptr;

//VertexArrays
PFNGLISVERTEXARRAYPROC glIsVertexArray = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLVERTEXATTRIBFORMATPROC glVertexAttribFormat = nullptr;
PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;

//Framebuffers
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = nullptr;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = nullptr;

//Draw
PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex = nullptr;

//Renderbuffers
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = nullptr;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = nullptr;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = nullptr;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = nullptr;
PFNGLISRENDERBUFFERPROC glIsRenderbuffer = nullptr;

//Clear
PFNGLCLEARCOLORPROC glClearColorf = nullptr;
PFNGLCLEARDEPTHFPROC glClearDepthf = nullptr;

//Stencil
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate = nullptr;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate = nullptr;

//Depth
PFNGLDEPTHRANGEFPROC glDepthRangef = nullptr;
PFNGLPOLYGONOFFSETCLAMPPROC glPolygonOffsetClamp = nullptr;

//GetString
PFNGLGETSTRINGIPROC glGetStringi = nullptr;

//Blend
PFNGLBLENDCOLORPROC glBlendColor = nullptr;
PFNGLCOLORMASKIPROC glColorMaski = nullptr;
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = nullptr;
PFNGLBLENDFUNCSEPARATEIPROC glBlendFuncSeparatei = nullptr;
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = nullptr;
PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei = nullptr;

//Enable
PFNGLENABLEIPROC glEnablei = nullptr;
PFNGLDISABLEIPROC glDisablei = nullptr;


namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void* LoadFunction(const char* name)
	{
		void* adress = reinterpret_cast<void*>(wglGetProcAddress(name));
		if (adress == (void*)-1 || adress == nullptr || adress == (void*)1 || adress == (void*)2 || adress == (void*)3)
		{
			HMODULE hModule = LoadLibrary("opengl32.dll");
			adress = (void*)GetProcAddress(hModule, name);
		}

		return adress;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LoadOpenGL()
	{
		static bool initialized = false;
		

		if (!initialized)
		{
			//WGL
			if (LOAD_GL_FUNC(wglChoosePixelFormatARB) == nullptr) return false;
			if (LOAD_GL_FUNC(wglCreateContextAttribsARB) == nullptr) return false;
			if (LOAD_GL_FUNC(wglSwapIntervalEXT) == nullptr) return false;
			if (LOAD_GL_FUNC(wglGetExtensionsStringARB) == nullptr) return false;


			//Buffers
			if (LOAD_GL_FUNC(glGenBuffers) == nullptr) return false;
			if (LOAD_GL_FUNC(glBindBuffer) == nullptr) return false;
			if (LOAD_GL_FUNC(glIsBuffer) == nullptr) return false;
			if (LOAD_GL_FUNC(glBufferData) == nullptr) return false;
			if (LOAD_GL_FUNC(glBindVertexBuffer) == nullptr) return false;
			if (LOAD_GL_FUNC(glDeleteBuffers) == nullptr) return false;
			if (LOAD_GL_FUNC(glBufferSubData) == nullptr) return false;
			if (LOAD_GL_FUNC(glBindBufferBase) == nullptr) return false;
			if (LOAD_GL_FUNC(glGetBufferParameteriv) == nullptr) return false;
			if (LOAD_GL_FUNC(glMapBufferRange) == nullptr) return false;
			if (LOAD_GL_FUNC(glUnmapBuffer) == nullptr) return false;
			if (LOAD_GL_FUNC(glUseProgram) == nullptr) return false;

			//Textures
			if (LOAD_GL_FUNC(glActiveTexture) == nullptr) return false;

			//Shaders
			if (LOAD_GL_FUNC(glCreateProgram) == nullptr) return false;
			if (LOAD_GL_FUNC(glCreateShader) == nullptr) return false;
			if (LOAD_GL_FUNC(glShaderSource) == nullptr) return false;
			if (LOAD_GL_FUNC(glCompileShader) == nullptr) return false;
			if (LOAD_GL_FUNC(glGetShaderiv) == nullptr) return false;
			if (LOAD_GL_FUNC(glGetShaderInfoLog) == nullptr) return false;
			if (LOAD_GL_FUNC(glDeleteProgram) == nullptr) return false;
			if (LOAD_GL_FUNC(glDeleteShader) == nullptr) return false;
			if (LOAD_GL_FUNC(glProgramParameteri) == nullptr) return false;
			if (LOAD_GL_FUNC(glAttachShader) == nullptr) return false;
			if (LOAD_GL_FUNC(glDetachShader) == nullptr) return false;
			if (LOAD_GL_FUNC(glLinkProgram) == nullptr) return false;
			if (LOAD_GL_FUNC(glGetProgramiv) == nullptr) return false;
			if (LOAD_GL_FUNC(glGetProgramInfoLog) == nullptr) return false;
			if (LOAD_GL_FUNC(glIsProgram) == nullptr) return false;
			if (LOAD_GL_FUNC(glIsShader) == nullptr) return false;
			
			//Patches
			if (LOAD_GL_FUNC(glPatchParameteri) == nullptr) return false;
			
			//Pipeline
			if (LOAD_GL_FUNC(glUseProgramStages) == nullptr) return false;
			if (LOAD_GL_FUNC(glIsProgramPipeline) == nullptr) return false;
			if (LOAD_GL_FUNC(glGenProgramPipelines) == nullptr) return false;
			if (LOAD_GL_FUNC(glBindProgramPipeline) == nullptr) return false;
			if (LOAD_GL_FUNC(glGetProgramPipelineiv) == nullptr) return false;
			if (LOAD_GL_FUNC(glDeleteProgramPipelines) == nullptr) return false;
			if (LOAD_GL_FUNC(glValidateProgramPipeline) == nullptr) return false;
			
			//VertexArrays
			if (LOAD_GL_FUNC(glIsVertexArray) == nullptr) return false;
			if (LOAD_GL_FUNC(glBindVertexArray) == nullptr) return false;
			if (LOAD_GL_FUNC(glGenVertexArrays) == nullptr) return false;
			if (LOAD_GL_FUNC(glDeleteVertexArrays) == nullptr) return false;
			if (LOAD_GL_FUNC(glVertexAttribFormat) == nullptr) return false;
			if (LOAD_GL_FUNC(glVertexAttribPointer) == nullptr) return false;
			if (LOAD_GL_FUNC(glVertexAttribBinding) == nullptr) return false;
			if (LOAD_GL_FUNC(glEnableVertexAttribArray) == nullptr) return false;
			
			//Framebuffers
			if (LOAD_GL_FUNC(glBindFramebuffer) == nullptr) return false;
			if (LOAD_GL_FUNC(glIsFramebuffer) == nullptr) return false;
			if (LOAD_GL_FUNC(glDeleteFramebuffers) == nullptr) return false;
			
			//Draw
			if (LOAD_GL_FUNC(glDrawElementsBaseVertex) == nullptr) return false;
			
			//Renderbuffers
			if (LOAD_GL_FUNC(glBindRenderbuffer) == nullptr) return false;
			if (LOAD_GL_FUNC(glDeleteRenderbuffers) == nullptr) return false;
			if (LOAD_GL_FUNC(glGenRenderbuffers) == nullptr) return false;
			if (LOAD_GL_FUNC(glRenderbufferStorage) == nullptr) return false;
			if (LOAD_GL_FUNC(glIsRenderbuffer) == nullptr) return false;
			
			//Clear
			if ((glClearColorf = reinterpret_cast<decltype(glClearColorf)>(LoadFunction("glClearColor"))) == nullptr) return false;
			if (LOAD_GL_FUNC(glClearDepthf) == nullptr) return false;
			
			//Stencil
			if (LOAD_GL_FUNC(glStencilOpSeparate) == nullptr) return false;
			if (LOAD_GL_FUNC(glStencilFuncSeparate) == nullptr) return false;

			//Depth
			if (LOAD_GL_FUNC(glDepthRangef) == nullptr) return false;
			if (LOAD_GL_FUNC(glPolygonOffsetClamp) == nullptr)
			{
				glPolygonOffsetClamp = reinterpret_cast<decltype(glPolygonOffsetClamp)>(LoadFunction("PolygonOffsetClampEXT"));
				if (glPolygonOffsetClamp == nullptr)
					return false;
			}

			//GetString
			if (LOAD_GL_FUNC(glGetStringi) == nullptr) return false;

			//Blend
			if (LOAD_GL_FUNC(glBlendColor) == nullptr) return false;
			if (LOAD_GL_FUNC(glColorMaski) == nullptr) return false;
			if (LOAD_GL_FUNC(glBlendFuncSeparate) == nullptr) return false;
			if (LOAD_GL_FUNC(glBlendFuncSeparatei) == nullptr) return false;
			if (LOAD_GL_FUNC(glBlendEquationSeparate) == nullptr) return false;
			if (LOAD_GL_FUNC(glBlendEquationSeparatei) == nullptr) return false;

			//Enable
			if (LOAD_GL_FUNC(glEnablei) == nullptr) return false;
			if (LOAD_GL_FUNC(glDisablei) == nullptr) return false;
		}

		return true;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const char* GetGLErrorString(int32 error)
	{
		switch (error)
		{
		case GL_INVALID_VALUE: return "GL_INVALID_VALUE: A numeric argument is out of range";
		case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete";
		case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command";
		case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow";
		case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow";
		case GL_INVALID_ENUM: return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument";
		case GL_NO_ERROR:
		default: return "GL_NO_ERROR: No error has been recorded";
		}
	}
}

#endif