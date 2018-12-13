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

#include <RayEngine.h>
#include <OpenGL/GlImpl.h>

#if defined(RE_PLATFORM_WINDOWS) || defined(RE_PLATFORM_LINUX)
/////////////////////////////
///FUNCTIONPTR DEFINITIONS///
/////////////////////////////

#if defined(RE_PLATFORM_WINDOWS)

/*////////////////////////////////////////////////////////////
	WGL
////////////////////////////////////////////////////////////*/
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = nullptr;
#elif defined(RE_PLATFORM_LINUX)

/*////////////////////////////////////////////////////////////
	GLX
////////////////////////////////////////////////////////////*/
PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = nullptr;
#endif

/*////////////////////////////////////////////////////////////
	Buffers
////////////////////////////////////////////////////////////*/
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

/*////////////////////////////////////////////////////////////
	Textures
////////////////////////////////////////////////////////////*/
#if defined(RE_PLATFORM_WINDOWS)
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
#endif

/*////////////////////////////////////////////////////////////
	Shaders
////////////////////////////////////////////////////////////*/
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

/*////////////////////////////////////////////////////////////
	Patches
////////////////////////////////////////////////////////////*/
PFNGLPATCHPARAMETERIPROC glPatchParameteri = nullptr;

/*////////////////////////////////////////////////////////////
	Pipeline
////////////////////////////////////////////////////////////*/
PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages = nullptr;
PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline = nullptr;
PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines = nullptr;
PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline = nullptr;
PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv = nullptr;
PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines = nullptr;
PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline = nullptr;

/*////////////////////////////////////////////////////////////
	VertexArrays
////////////////////////////////////////////////////////////*/
PFNGLISVERTEXARRAYPROC glIsVertexArray = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLVERTEXATTRIBFORMATPROC glVertexAttribFormat = nullptr;
PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;

/*////////////////////////////////////////////////////////////
	Framebuffers
////////////////////////////////////////////////////////////*/
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = nullptr;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = nullptr;

/*////////////////////////////////////////////////////////////
	Draw
////////////////////////////////////////////////////////////*/
PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex = nullptr;

/*////////////////////////////////////////////////////////////
	Renderbuffer
////////////////////////////////////////////////////////////*/
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = nullptr;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = nullptr;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = nullptr;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = nullptr;
PFNGLISRENDERBUFFERPROC glIsRenderbuffer = nullptr;

/*////////////////////////////////////////////////////////////
	Clear
////////////////////////////////////////////////////////////*/
PFNGLCLEARCOLORPROC glClearColorf = nullptr;
PFNGLCLEARDEPTHFPROC glClearDepthf = nullptr;

/*////////////////////////////////////////////////////////////
	String
////////////////////////////////////////////////////////////*/
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate = nullptr;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate = nullptr;

/*////////////////////////////////////////////////////////////
	Depth
////////////////////////////////////////////////////////////*/
PFNGLDEPTHRANGEFPROC glDepthRangef = nullptr;
PFNGLPOLYGONOFFSETCLAMPPROC glPolygonOffsetClamp = nullptr;

/*////////////////////////////////////////////////////////////
	String
////////////////////////////////////////////////////////////*/
PFNGLGETSTRINGIPROC glGetStringi = nullptr;

/*////////////////////////////////////////////////////////////
	Blend
////////////////////////////////////////////////////////////*/
#if defined(RE_PLATFORM_WINDOWS)
PFNGLBLENDCOLORPROC glBlendColor = nullptr;
#endif
PFNGLCOLORMASKIPROC glColorMaski = nullptr;
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = nullptr;
PFNGLBLENDFUNCSEPARATEIPROC glBlendFuncSeparatei = nullptr;
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = nullptr;
PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei = nullptr;

/*////////////////////////////////////////////////////////////
	Enable
////////////////////////////////////////////////////////////*/
PFNGLENABLEIPROC glEnablei = nullptr;
PFNGLDISABLEIPROC glDisablei = nullptr;


namespace RayEngine
{
#if defined(RE_PLATFORM_WINDOWS)
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
#elif defined(RE_PLATFORM_LINUX)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void* LoadFunction(const char* name)
	{
		void (*adress)() = glXGetProcAddressARB(reinterpret_cast<const GLubyte*>(name)); 
		return reinterpret_cast<void*>(adress);
	}
#endif


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LoadOpenGL()
	{
		static bool initialized = false;
		
		if (!initialized)
		{
#if defined(RE_PLATFORM_WINDOWS)
			/*////////////////////////////////////////////////////////////
				WGL
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(wglChoosePixelFormatARB);
			LOAD_GL_FUNC(wglCreateContextAttribsARB);
			LOAD_GL_FUNC(wglSwapIntervalEXT);
			LOAD_GL_FUNC(wglGetExtensionsStringARB);
#elif defined (RE_PLATFORM_LINUX)
			/*////////////////////////////////////////////////////////////
				GLX
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glXCreateContextAttribsARB);
#endif

			/*////////////////////////////////////////////////////////////
				Buffers
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glGenBuffers);
			LOAD_GL_FUNC(glBindBuffer);
			LOAD_GL_FUNC(glIsBuffer);
			LOAD_GL_FUNC(glBufferData);
			LOAD_GL_FUNC(glBindVertexBuffer);
			LOAD_GL_FUNC(glDeleteBuffers);
			LOAD_GL_FUNC(glBufferSubData);
			LOAD_GL_FUNC(glBindBufferBase);
			LOAD_GL_FUNC(glGetBufferParameteriv);
			LOAD_GL_FUNC(glMapBufferRange);
			LOAD_GL_FUNC(glUnmapBuffer);
			LOAD_GL_FUNC(glUseProgram);

			/*////////////////////////////////////////////////////////////
				Textures
			////////////////////////////////////////////////////////////*/
#if defined(RE_PLATFORM_WINDOWS)
			LOAD_GL_FUNC(glActiveTexture);
#endif

			/*////////////////////////////////////////////////////////////
				Shaders
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glCreateProgram);
			LOAD_GL_FUNC(glCreateShader);
			LOAD_GL_FUNC(glShaderSource);
			LOAD_GL_FUNC(glCompileShader);
			LOAD_GL_FUNC(glGetShaderiv);
			LOAD_GL_FUNC(glGetShaderInfoLog);
			LOAD_GL_FUNC(glDeleteProgram);
			LOAD_GL_FUNC(glDeleteShader);
			LOAD_GL_FUNC(glProgramParameteri);
			LOAD_GL_FUNC(glAttachShader);
			LOAD_GL_FUNC(glDetachShader);
			LOAD_GL_FUNC(glLinkProgram);
			LOAD_GL_FUNC(glGetProgramiv);
			LOAD_GL_FUNC(glGetProgramInfoLog);
			LOAD_GL_FUNC(glIsProgram);
			LOAD_GL_FUNC(glIsShader);
			
			/*////////////////////////////////////////////////////////////
				Patches
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glPatchParameteri);
			
			/*////////////////////////////////////////////////////////////
				Pipelines
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glUseProgramStages);
			LOAD_GL_FUNC(glIsProgramPipeline);
			LOAD_GL_FUNC(glGenProgramPipelines);
			LOAD_GL_FUNC(glBindProgramPipeline);
			LOAD_GL_FUNC(glGetProgramPipelineiv);
			LOAD_GL_FUNC(glDeleteProgramPipelines);
			LOAD_GL_FUNC(glValidateProgramPipeline);
			
			/*////////////////////////////////////////////////////////////
				VertexArrays
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glIsVertexArray);
			LOAD_GL_FUNC(glBindVertexArray);
			LOAD_GL_FUNC(glGenVertexArrays);
			LOAD_GL_FUNC(glDeleteVertexArrays);
			LOAD_GL_FUNC(glVertexAttribFormat);
			LOAD_GL_FUNC(glVertexAttribPointer);
			LOAD_GL_FUNC(glVertexAttribBinding);
			LOAD_GL_FUNC(glEnableVertexAttribArray);
			
			/*////////////////////////////////////////////////////////////
				Framebuffers
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glBindFramebuffer);
			LOAD_GL_FUNC(glIsFramebuffer);
			LOAD_GL_FUNC(glDeleteFramebuffers);
			
			/*////////////////////////////////////////////////////////////
				Draw
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glDrawElementsBaseVertex);
			
			/*////////////////////////////////////////////////////////////
				Renderbuffers
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glBindRenderbuffer);
			LOAD_GL_FUNC(glDeleteRenderbuffers);
			LOAD_GL_FUNC(glGenRenderbuffers);
			LOAD_GL_FUNC(glRenderbufferStorage);
			LOAD_GL_FUNC(glIsRenderbuffer);
			
			/*////////////////////////////////////////////////////////////
				Clear
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glClearDepthf);
			LOAD_GL_FUNC_NAMED(glClearColorf, "glClearColor");

			/*////////////////////////////////////////////////////////////
				Stencil
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glStencilOpSeparate);
			LOAD_GL_FUNC(glStencilFuncSeparate);

			/*////////////////////////////////////////////////////////////
				Depth
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glDepthRangef);
			LOAD_GL_FUNC(glPolygonOffsetClamp);
			if (glPolygonOffsetClamp == nullptr) 
			{ 
				LOAD_GL_FUNC_NAMED(glPolygonOffsetClamp, "PolygonOffsetClampEXT");
			}

			/*////////////////////////////////////////////////////////////
				String
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glGetStringi);

			/*////////////////////////////////////////////////////////////
				Blend
			////////////////////////////////////////////////////////////*/
#if defined(RE_PLATFORM_WINDOWS)
			LOAD_GL_FUNC(glBlendColor);
#endif
			LOAD_GL_FUNC(glColorMaski);
			LOAD_GL_FUNC(glBlendFuncSeparate);
			LOAD_GL_FUNC(glBlendFuncSeparatei);
			LOAD_GL_FUNC(glBlendEquationSeparate);
			LOAD_GL_FUNC(glBlendEquationSeparatei);

			/*////////////////////////////////////////////////////////////
				Enable
			////////////////////////////////////////////////////////////*/
			LOAD_GL_FUNC(glEnablei);
			LOAD_GL_FUNC(glDisablei);
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