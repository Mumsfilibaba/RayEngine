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
#include "..\Defines.h"
#include "..\Types.h"

#if !defined(ReRelease)
#define ReRelease(pObject) pObject->Release(); pObject = nullptr
#endif

#if !defined(ReRelease_S)
#define ReRelease_S(pObject) if (pObject != nullptr) { ReRelease(pObject); }
#endif

namespace RayEngine
{
	class IObject
	{
		RE_INTERFACE(IObject)

	protected:
		IObject() {}
		~IObject() {}

	public:
		typedef uint32 CounterType;

		/*////////////////////////////////////////////////////////////
			Increments objects reference counter and returns the 
			new referencecount.
		////////////////////////////////////////////////////////////*/
		virtual CounterType AddRef() = 0;
		
		/*////////////////////////////////////////////////////////////
			Returns the current referencecount
		////////////////////////////////////////////////////////////*/
		virtual CounterType GetReferenceCount() const = 0;
		
		/*////////////////////////////////////////////////////////////
			Decrements the referencecounter and returns the new 
			referencecount. The object is deleted if the number 
			of referencec reaches zero.
		////////////////////////////////////////////////////////////*/
		virtual CounterType Release() = 0;

		/*////////////////////////////////////////////////////////////
			Returns a new pointer to the object and increments the
			referencecounter. Casts to the wanted type.
		////////////////////////////////////////////////////////////*/
		template<typename Ref>
		inline Ref* QueryReference()
		{
			AddRef();
			return dynamic_cast<Ref*>(this);
		}
	};
}