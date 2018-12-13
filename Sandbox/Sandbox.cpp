#include "Sandbox.h"

Sandbox::Sandbox()
	: Application(RayEngine::GRAPHICS_API_D3D12)
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::OnUpdate()
{
	Application::OnUpdate();
}

void Sandbox::OnRender()
{
	Application::OnRender();
}