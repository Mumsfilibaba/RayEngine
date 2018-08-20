#pragma once

#include "Defines.h"
#include "Types.h"

#if !defined(ReRelease)
#define ReRelease(pReferenferCounter) pReferenferCounter->Release; pReferenferCounter = nullptr
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
		virtual void Release() const;
		//Returns the reference count
		virtual uint32 GetReferenceCount() const;

		//Templated version of QueryReference
		template<typename Ref>
		inline Ref* QueryReference()
		{
			return reinterpret_cast<Ref*>(QueryReference());
		}

	private:
		mutable int32 m_References;
	};
}