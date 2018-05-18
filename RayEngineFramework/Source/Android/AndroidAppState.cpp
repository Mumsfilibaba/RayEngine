#include "..\..\Include\Android\AndroidAppState.h"

namespace RayEngine
{
	AndroidAppState::AndroidAppState()
	{
	}

	AndroidAppState::~AndroidAppState()
	{
	}

	AndroidAppState& AndroidAppState::GetInstance()
	{
		static AndroidAppState instance;
		return instance;
	}
}