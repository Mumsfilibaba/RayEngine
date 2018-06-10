#pragma once

#include "Vector3.h"

namespace Math
{
	class Ray
	{
	public:
		Ray(const Vector3& origin = Vector3(), const Vector3& direction = Vector3());
		Ray(const Ray& other);
		~Ray();

		Vector3 GetPoint(float rayLength) const;
		bool Equals(const Ray& other) const;

		std::string ToString() const;

		Ray& operator=(const Ray& other);
		bool operator==(const Ray& other) const;
		bool operator!=(const Ray& other) const;

		void Origin(const Math::Vector3& origin);
		Vector3& Origin();
		const Vector3& Origin() const;
		void Direction(const Math::Vector3& direction);
		Vector3& Direction();
		const Vector3& Direction() const;

	private:
		Vector3 m_Origin;
		Vector3 m_Direction;
	};
}