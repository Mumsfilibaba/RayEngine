#include "Sandbox.h"

Sandbox::Sandbox()
	: Application(RayEngine::GRAPHICS_API_D3D11)
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::OnUpdate()
{
	LOG_INFO("Update");
}

void Sandbox::OnRender()
{
	float color[] = { 0.392f, 0.584f, 0.929f, 1.0f };
	GetContext()->ClearRendertargetView(nullptr, color);
	GetContext()->ClearDepthStencilView(nullptr, 1.0f, 0);

	GetSwapChain()->Present();
}
