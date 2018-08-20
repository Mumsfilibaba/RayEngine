#pragma once

#include "..\ReferenceCounter.h"

namespace RayEngine
{
	namespace Graphics
	{
		class IRootVariableSlot : public ReferenceCounter
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