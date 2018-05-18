#pragma once

#include "Vector3.h"

namespace Math
{
	class Vector4
	{
	public:
		Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
		Vector4(const Vector4& other);
		explicit Vector4(const Vector3& other);
		~Vector4() {}

		Vector4& Add(const Vector4& other);
		Vector4& Add(float scalar);
		Vector4& Subtract(const Vector4& other);
		Vector4& Subtract(float scalar);
		Vector4& Multiply(float scalar);
		Vector4& Divide(float scalar);

		bool IsUnitVector() const;
		bool Equals(const Vector4& other) const;

		float Dot(const Vector4& other) const;
		float LengthSqrd() const;
		float Length() const;
		Vector4& Normalize();
		Vector4 UnitVector() const;
		Vector4 Project(const Vector4& other) const;
		Vector4 Reflect(const Vector4& normal) const;

		std::string ToString() const;

		bool operator==(const Vector4& other) const;
		bool operator!=(const Vector4& other) const;

		Vector4 operator-() const;
		Vector4& operator=(const Vector4& other);
		Vector4& operator=(const Vector3& vector3);
		Vector4& operator-=(const Vector4& other);
		Vector4& operator+=(const Vector4& other);
		Vector4& operator-=(float scalar);
		Vector4& operator+=(float scalar);
		Vector4& operator*=(float scalar);
		Vector4& operator/=(float scalar);

		explicit operator Vector3&();
		explicit operator const Vector3&() const;


		friend Vector4 operator-(Vector4 left, const Vector4& right);
		friend Vector4 operator+(Vector4 left, const Vector4& right);
		friend Vector4 operator-(Vector4 left, float right);
		friend Vector4 operator+(Vector4 left, float right);
		friend Vector4 operator*(Vector4 left, float right);
		friend Vector4 operator/(Vector4 left, float right);
		friend Vector4 operator*(float left, Vector4 right);

	public:
		static Vector4 Nan();

	public:
		union
		{
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};

			float v[4];
			Vector3 xyz;
		};
	};
}