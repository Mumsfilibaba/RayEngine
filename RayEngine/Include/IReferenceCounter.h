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

		//Gets a pointer to the object
		virtual IReferenceCounter* QueryReference() = 0;
		//Decreases the reference counter and releases the object
		virtual void Release() const = 0;
		//Returns the reference count
		virtual uint32 GetReferenceCount() const = 0;
	protected:
		//Adds a reference to the object
		virtual uint32 AddRef() = 0;
	};
}