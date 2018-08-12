#pragma once

#include "..\RefCounter.h"

namespace RayEngine
{
	namespace Graphics
	{
		class IRootVariableSlot : public RefCounter
		{
		public:
			IRootVariableSlot(IRootVariableSlot&& other) = delete;
			IRootVariableSlot(const IRootVariableSlot& other) = delete;
			IRootVariableSlot& operator=(IRootVariableSlot&& other) = delete;
			IRootVariableSlot& operator=(const IRootVariableSlot& other) = delete;

		public:
			IRootVariableSlot() {}
			virtual ~IRootVariableSlot() {}
		};
	}
}