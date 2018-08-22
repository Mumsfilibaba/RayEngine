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

#include "..\..\Include\Math\Point.h"

#if defined(__ANDROID__)
#include <sstream>
#endif

namespace Math
{
	/////////////////////////////////////////////////////////////
	Point::Point(int x, int y)
		: x(x),
		y(y)
	{
	}



	/////////////////////////////////////////////////////////////
	Point::Point(const Point& other)
		: x(0),
		y(0)
	{
		*this = other;
	}



	/////////////////////////////////////////////////////////////
	Point& Point::Add(const Point& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Point& Point::Add(int scalar)
	{
		x += scalar;
		y += scalar;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Point& Point::Subtract(const Point& other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Point& Point::Subtract(int scalar)
	{
		x -= scalar;
		y -= scalar;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Point& Point::Multiply(int scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Point& Point::Divide(int scalar)
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	bool Point::Equals(const Point & other) const
	{
		return (x == other.x) && (y == other.y);
	}



	/////////////////////////////////////////////////////////////
	std::string Point::ToString() const
	{
#if defined(_ANDROID)
		std::stringstream stream;
		stream << '[' << x << ", " << y << ']';
		return stream.str();
#else
		using namespace std;
		return string('[' + to_string(x) + ", " + to_string(y) + ']');
#endif
	}



	/////////////////////////////////////////////////////////////
	Point Point::operator-() const
	{
		return Point(-x, -y);
	}



	/////////////////////////////////////////////////////////////
	Point& Point::operator=(const Point& other)
	{
		if (this != &other)
		{
			x = other.x;
			y = other.y;
		}

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Point& Point::operator+=(const Point& right)
	{
		return Add(right);
	}



	/////////////////////////////////////////////////////////////
	Point& Point::operator-=(const Point& right)
	{
		return Subtract(right);
	}



	/////////////////////////////////////////////////////////////
	Point& Point::operator+=(int right)
	{
		return Add(right);
	}



	/////////////////////////////////////////////////////////////
	Point& Point::operator-=(int right)
	{
		return Subtract(right);
	}



	/////////////////////////////////////////////////////////////
	Point& Point::operator*=(int right)
	{
		return Multiply(right);
	}



	/////////////////////////////////////////////////////////////
	Point& Point::operator/=(int right)
	{
		return Divide(right);
	}



	/////////////////////////////////////////////////////////////
	bool Point::operator==(const Point& other) const
	{
		return Equals(other);
	}



	/////////////////////////////////////////////////////////////
	bool Point::operator!=(const Point& other) const
	{
		return !Equals(other);
	}



	/////////////////////////////////////////////////////////////
	Point operator+(Point left, const Point& right)
	{
		return left.Add(right);
	}



	/////////////////////////////////////////////////////////////
	Point operator-(Point left, const Point& right)
	{
		return left.Subtract(right);
	}



	/////////////////////////////////////////////////////////////
	Point operator+(Point left, int right)
	{
		return left.Add(right);
	}



	/////////////////////////////////////////////////////////////
	Point operator-(Point left, int right)
	{
		return left.Subtract(right);
	}



	/////////////////////////////////////////////////////////////
	Point operator*(Point left, int right)
	{
		return left.Multiply(right);
	}



	/////////////////////////////////////////////////////////////
	Point operator*(int left, Point right)
	{
		return right.Multiply(left);
	}



	/////////////////////////////////////////////////////////////
	Point operator/(Point left, int right)
	{
		return left.Divide(right);
	}
}