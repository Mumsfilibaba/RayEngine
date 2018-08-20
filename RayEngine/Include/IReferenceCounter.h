#pragma once

#include "Defines.h"
#include "Types.h"

namespace RayEngine
{
	class IReferenceCounter
	{
	public:
		IReferenceCounter(IReferenceCounter&& other) = delete;
		IReferenceCounter(const IReferenceCounter& other) = delete;
		IReferenceCounter& operator=(IReferenceCounter&& other) = delete;
		IReferenceCounter& operator=(const IReferenceCounter& other) = delete;

	public:
		IReferenceCounter() {}
		virtual ~IReferenceCounter() {}

		//Adds a reference to the object
		virtual uint32 AddRef();
		//Gets a pointer to the object
		virtual IReferenceCounter* QueryReference();
		//Decreases the reference counter and releases the object
		virtual void Release() const;
		//Returns the reference count
		virtual uint32 GetReferenceCount() const;

		//Templated version of QueryReference
		template<typename Ref>
		inline Ref* QueryReference()
		{
			return reinterpret_cast<Ref*>(QueryReference());
		}

	protected:
		virtual int32& ReferenceCounter() const = 0;
	};
}