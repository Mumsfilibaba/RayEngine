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
#include "ILog.h"

namespace RayEngine
{
	class NullLog final : public ILog
	{
		RE_UNIQUE_OBJECT(NullLog);

	public:
		NullLog();
		~NullLog();

		void Write(LOG_SEVERITY severity, const std::string& text) const override final;

		bool HasMessageBuffer() const override final;

		void Flush() override final;

		int32 GetMessageCount() const override final;

		const LogMessage& GetMessage(int32 index) const override final;
	};
}