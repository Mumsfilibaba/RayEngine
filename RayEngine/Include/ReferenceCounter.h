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
#include "Defines.h"
#include "Types.h"

#if !defined(ReRelease)
#define ReRelease(pReferenferCounter) pReferenferCounter->Release(); pReferenferCounter = nullptr
#endif

#if !defined(ReRelease_S)
#define ReRelease_S(pReferenferCounter) if (pReferenferCounter != nullptr) { ReRelease(pReferenferCounter); }
#endif

namespace RayEngine
{
	class ReferenceCounter
	{
	public:
		ReferenceCounter(ReferenceCounter&& other) = delete;
		ReferenceCounter(const ReferenceCounter& other) = delete;
		ReferenceCounter& operator=(ReferenceCounter&& other) = delete;
		ReferenceCounter& operator=(const ReferenceCounter& other) = delete;

	protected:
		ReferenceCounter();
		virtual ~ReferenceCounter();

	public:
		//Adds a reference to the object
		virtual uint32 AddRef();
		//Gets a pointer to the object
		virtual ReferenceCounter* QueryReference();
		//Decreases the reference counter and releases the object
		virtual void Release();
		//Returns the reference count
		virtual uint32 GetReferenceCount() const;

		//Templated version of QueryReference
		template<typename Ref>
		inline Ref* QueryReference()
		{
			return reinterpret_cast<Ref*>(QueryReference());
		}

	protected:
		//Removes a reference to the object
		virtual uint32 DecrRef();

	private:
		mutable int32 m_References;
	};
}