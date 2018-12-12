#pragma once

#include <System/Application.h>

class Sandbox final : public RayEngine::Application
{
public:
	Sandbox();
	~Sandbox();

	void OnUpdate() override final;
	void OnRender() override final;
};