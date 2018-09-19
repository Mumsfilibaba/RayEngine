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
#include "..\Types.h"


#if defined (RE_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>


///////////////
///FUNCTIONS///
///////////////

namespace RayEngine
{
#if defined(RE_PLATFORM_WINDOWS)
	typedef HDC GLNativeDevice;
#define RE_GL_NULL_NATIVE_DEVICE 0
	typedef HGLRC GLNativeContext;
#define RE_GL_NULL_NATIVE_CONTEXT 0
#else
	typedef void* GLNativeDevice;
#define GL_NULL_NATIVE_DEVICE nullptr
	typedef void* GLNativeContext;
#define GL_NULL_NATIVE_CONTEXT nullptr
#endif


	bool LoadOpenGL();
	void* LoadFunction(const char* name);
	const char* GetGLErrorString(int32 error);
}


/////////////
///DEFINES///
/////////////


/*////////////////////////////////////////////////////////////
	WGL
////////////////////////////////////////////////////////////*/

//PixelformatAttributes
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_SWAP_METHOD_ARB 0x2007
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_SWAP_EXCHANGE_ARB 0x2028
#define WGL_SWAP_COPY_ARB 0x2029
#define WGL_SWAP_UNDEFINED_ARB 0x202A
#define WGL_TYPE_RGBA_ARB 0x202B
#define WGL_STEREO_ARB 0x2012
#define WGL_COLORSPACE_EXT 0x309D
#define WGL_COLORSPACE_SRGB_EXT 0x3089
#define WGL_COLORSPACE_LINEAR_EXT 0x308A
#define WGL_SAMPLE_BUFFERS_ARB 0x2041
#define WGL_SAMPLES_ARB 0x2042

//Context Attributes
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x00000001
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001


/*////////////////////////////////////////////////////////////
	CORE OPENGL
////////////////////////////////////////////////////////////*/

//glGet
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#define GL_MAX_DRAW_BUFFERS 0x8824
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C

//Errortypes
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506

//Buffers
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_ARRAY_BUFFER 0x8892
#define GL_UNIFORM_BUFFER 0x8A11
#define GL_ELEMENT_ARRAY_BUFFER 0x8893

//Usage
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8

//Internal format
#define GL_R8 0x8229
#define GL_R32F 0x822E
#define GL_RGBA16F 0x881A
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT32 0x81A7

//Sampler parameter
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D

//Texture units
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF

//GL-Enable
#define GL_TEXTURE_CUBE_MAP_SEAMLESS 0x884F
#define GL_DEPTH_CLAMP 0x864F

//Cube-Map
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A

//Shaders
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_TESS_EVALUATION_SHADER 0x8E87
#define GL_TESS_CONTROL_SHADER 0x8E88
#define GL_VERTEX_SHADER_BIT 0x00000001
#define GL_FRAGMENT_SHADER_BIT 0x00000002
#define GL_GEOMETRY_SHADER_BIT 0x00000004
#define GL_TESS_CONTROL_SHADER_BIT 0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT 0x00000010
#define GL_ALL_SHADER_BITS 0xFFFFFFFF
#define GL_PROGRAM_SEPARABLE 0x8258

//Primitive types
#define GL_PATCHES 0x000E
#define GL_PATCH_VERTICES 0x8E72

//Framebuffer attachment
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_COLOR_ATTACHMENT16 0x8CF0
#define GL_COLOR_ATTACHMENT17 0x8CF1
#define GL_COLOR_ATTACHMENT18 0x8CF2
#define GL_COLOR_ATTACHMENT19 0x8CF3
#define GL_COLOR_ATTACHMENT20 0x8CF4
#define GL_COLOR_ATTACHMENT21 0x8CF5
#define GL_COLOR_ATTACHMENT22 0x8CF6
#define GL_COLOR_ATTACHMENT23 0x8CF7
#define GL_COLOR_ATTACHMENT24 0x8CF8
#define GL_COLOR_ATTACHMENT25 0x8CF9
#define GL_COLOR_ATTACHMENT26 0x8CFA
#define GL_COLOR_ATTACHMENT27 0x8CFB
#define GL_COLOR_ATTACHMENT28 0x8CFC
#define GL_COLOR_ATTACHMENT29 0x8CFD
#define GL_COLOR_ATTACHMENT30 0x8CFE
#define GL_COLOR_ATTACHMENT31 0x8CFF
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20

//Pipelines
#define GL_VALIDATE_STATUS 0x8B83

//Stencil
#define GL_INCR_WRAP 0x8507
#define GL_DECR_WRAP 0x8508

//Usage
#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_COPY 0x88EA


//////////////
///TYPEDEFS///
//////////////


//Types
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;
typedef char GLchar;

//WGL
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
typedef BOOL(WINAPI* PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int *piFormats, UINT* nNumFormats);
typedef const char *(WINAPI* PFNWGLGETEXTENSIONSSTRINGARBPROC) (HDC hdc);

//Buffers
typedef void (APIENTRY* PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void (APIENTRY* PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY* PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (APIENTRY* PFNGLBINDVERTEXBUFFERPROC) (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (APIENTRY* PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);
typedef void (APIENTRY* PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
typedef void (APIENTRY* PFNGLBINDBUFFERBASEPROC) (GLenum target, GLuint index, GLuint buffer);
typedef GLboolean(APIENTRY* PFNGLISBUFFERPROC) (GLuint buffer);

//Textures
typedef void (APIENTRY* PFNGLACTIVETEXTUREPROC) (GLenum texture);

//Shaders
typedef void (APIENTRY* PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (APIENTRY* PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (APIENTRY* PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (APIENTRY* PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (APIENTRY* PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint* params);
typedef void (APIENTRY* PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY* PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY* PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* params);
typedef void (APIENTRY* PFNGLPROGRAMPARAMETERIPROC) (GLuint program, GLenum pname, GLint value);
typedef void (APIENTRY* PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (APIENTRY* PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar* const*string, const GLint* length);
typedef void (APIENTRY* PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef GLboolean(APIENTRY* PFNGLISPROGRAMPROC) (GLuint program);
typedef GLboolean(APIENTRY* PFNGLISSHADERPROC) (GLuint shader);
typedef GLuint(APIENTRY* PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint(APIENTRY* PFNGLCREATESHADERPROC) (GLenum type);

//Patches
typedef void (APIENTRY* PFNGLPATCHPARAMETERIPROC) (GLenum pname, GLint value);

//Pipeline
typedef void (APIENTRY* PFNGLGENPROGRAMPIPELINESPROC) (GLsizei n, GLuint *pipelines);
typedef void (APIENTRY* PFNGLDELETEPROGRAMPIPELINESPROC) (GLsizei n, const GLuint *pipelines);
typedef void (APIENTRY* PFNGLUSEPROGRAMSTAGESPROC) (GLuint pipeline, GLbitfield stages, GLuint program);
typedef void (APIENTRY* PFNGLGETPROGRAMPIPELINEIVPROC) (GLuint pipeline, GLenum pname, GLint *params);
typedef void (APIENTRY* PFNGLVALIDATEPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void (APIENTRY* PFNGLBINDPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef GLboolean(APIENTRY* PFNGLISPROGRAMPIPELINEPROC) (GLuint pipeline);

//VertexArray
typedef void (APIENTRY* PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (APIENTRY* PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef void (APIENTRY* PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint* arrays);
typedef void (APIENTRY* PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY* PFNGLVERTEXATTRIBBINDINGPROC) (GLuint attribindex, GLuint bindingindex);
typedef void (APIENTRY* PFNGLVERTEXATTRIBFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef GLboolean(APIENTRY* PFNGLISVERTEXARRAYPROC) (GLuint array);

//Framebuffers
typedef void (APIENTRY* PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void (APIENTRY* PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint* framebuffers);
typedef GLboolean(APIENTRY* PFNGLISFRAMEBUFFERPROC) (GLuint framebuffer);

//Draw
typedef void (APIENTRY* PFNGLDRAWELEMENTSBASEVERTEXPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);

//Renderbuffers
typedef void (APIENTRY* PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void (APIENTRY* PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void (APIENTRY* PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint *renderbuffers);
typedef void (APIENTRY* PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef GLboolean(APIENTRY* PFNGLISRENDERBUFFERPROC) (GLuint renderbuffer);

//Clear
typedef void (APIENTRY* PFNGLCLEARCOLORPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRY* PFNGLCLEARDEPTHFPROC) (GLfloat d);

//Stencil
typedef void (APIENTRY* PFNGLSTENCILOPSEPARATEPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (APIENTRY* PFNGLSTENCILFUNCSEPARATEPROC) (GLenum face, GLenum func, GLint ref, GLuint mask);

//Depth
typedef void (APIENTRY* PFNGLDEPTHRANGEFPROC) (GLfloat n, GLfloat f);

//////////////////
///FUNCTIONPTRS///
//////////////////

//WGL
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
extern PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;

//Buffers
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLISBUFFERPROC glIsBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLBINDBUFFERBASEPROC glBindBufferBase;

//Textures
extern PFNGLACTIVETEXTUREPROC glActiveTexture;

//Shaders
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLISPROGRAMPROC glIsProgram;
extern PFNGLISSHADERPROC glIsShader;

//Patches
extern PFNGLPATCHPARAMETERIPROC glPatchParameteri;

//Pipeline
extern PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages;
extern PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline;
extern PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines;
extern PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline;
extern PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv;
extern PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines;
extern PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline;

//VertexArrays
extern PFNGLISVERTEXARRAYPROC glIsVertexArray;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLVERTEXATTRIBFORMATPROC glVertexAttribFormat;
extern PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;

//Framebuffers
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLISFRAMEBUFFERPROC glIsFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;

//Draw
extern PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex;

//Renderbuffers
extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
extern PFNGLISRENDERBUFFERPROC glIsRenderbuffer;

//Clear
extern PFNGLCLEARCOLORPROC glClearColorf;
extern PFNGLCLEARDEPTHFPROC glClearDepthf;

//Stencil
extern PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;
extern PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate;

//Depth
extern PFNGLDEPTHRANGEFPROC glDepthRangef;
#endif