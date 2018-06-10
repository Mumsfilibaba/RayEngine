#include "..\..\Include\Math\Ray.h"

namespace Math
{
	Ray::Ray(const Vector3& origin, const Vector3& direction)
		: m_Origin(origin),
		m_Direction(direction)
	{
	}

	Ray::Ray(const Ray& other)
		: m_Origin(other.m_Origin),
		m_Direction(other.m_Direction)
	{
	}

	Ray::~Ray()
	{
	}

	Vector3 Ray::GetPoint(float rayLength) const
	{
		return m_Origin + (rayLength * m_Direction);
	}

	bool Ray::Equals(const Ray& other) const
	{
		return (m_Origin == other.m_Origin && m_Direction == other.m_Direction);
	}

	std::string Ray::ToString() const
	{
		return m_Origin.ToString() + " t * " + m_Direction.ToString();
	}

	Ray& Ray::operator=(const Ray& other)
	{
		if (this != &other)
			memcpy(this, &other, sizeof(Ray));

		return *this;
	}

	bool Ray::operator==(const Ray& other) const
	{
		return Equals(other);
	}

	bool Ray::operator!=(const Ray& other) const
	{
		return !Equals(other);
	}

	void Ray::Origin(const Math::Vector3& origin)
	{
		m_Origin = origin;
	}

	Vector3& Ray::Origin()
	{
		return m_Origin;
	}
	
	const Vector3& Ray::Origin() const
	{
		return m_Origin;
	}

	void Ray::Direction(const Math::Vector3& direction)
	{
		m_Direction = direction;
	}

	Vector3& Ray::Direction()
	{
		return m_Direction;
	}

	const Vector3& Ray::Direction() const
	{
		return m_Direction;
	}
}