#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	class AndroidAppState
	{
	private:
		AndroidAppState();
		~AndroidAppState();

	public:
		AndroidAppState(const AndroidAppState& other) = delete;
		AndroidAppState(AndroidAppState&& other) = delete;
		AndroidAppState& operator=(const AndroidAppState& other) = delete;
		AndroidAppState& operator=(AndroidAppState&& other) = delete;

	public:
		static AndroidAppState& GetInstance();
	};
}