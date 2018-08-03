#pragma once

#include "IReferenceCounter.h"

namespace RayEngine
{
	namespace Graphics
	{
		class RefCounter : public IReferenceCounter
		{
		public:
			RefCounter(RefCounter&& other) = delete;
			RefCounter(const RefCounter& other) = delete;
			RefCounter& operator=(RefCounter&& other) = delete;
			RefCounter& operator=(const RefCounter& other) = delete;

		public:
			RefCounter();
			virtual ~RefCounter();

			virtual void Release() const override;
			virtual IReferenceCounter* QueryReference() override;

			uint32 GetReferenceCount() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			mutable uint32 m_ReferenceCounter;
		};
	}
}