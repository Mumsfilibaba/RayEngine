#include "Sandbox.h"

Sandbox::Sandbox()
	: Application(RayEngine::GRAPHICS_API_OPENGL)
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::OnUpdate()
{
}

void Sandbox::OnRender()
{
	float color[] = { 0.392f, 0.584f, 0.929f, 1.0f };
	GetContext()->ClearRendertargetView(nullptr, color);
	GetContext()->ClearDepthStencilView(nullptr, 1.0f, 0);

	GetSwapChain()->Present();
}