#include "..\..\Include\Math\Color.h"

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	const Color Color::BLACK(ColorF::BLACK);
	const Color Color::WHITE(ColorF::WHITE);
	const Color Color::WARMWHITE(ColorF::WARMWHITE);
	const Color Color::GRAY(ColorF::GRAY);
	const Color Color::DARKGRAY(ColorF::DARKGRAY);
	const Color Color::SOFTGRAY(ColorF::SOFTGREEN);
	const Color Color::RED(ColorF::RED);
	const Color Color::DARKRED(ColorF::DARKRED);
	const Color Color::SOFTRED(ColorF::SOFTRED);
	const Color Color::GREEN(ColorF::GREEN);
	const Color Color::SOFTGREEN(ColorF::SOFTGREEN);
	const Color Color::DARKGREEN(ColorF::DARKGREEN);
	const Color Color::BLUE(ColorF::BLUE);
	const Color Color::DARKBLUE(ColorF::DARKBLUE);
	const Color Color::SOFTBLUE(ColorF::SOFTBLUE);
	const Color Color::CORNFLOWERBLUE(ColorF::CORNFLOWERBLUE);
	const Color Color::YELLOW(ColorF::YELLOW);
	const Color Color::SOFTYELLOW(ColorF::SOFTYELLOW);
	const Color Color::DARKYELLOW(ColorF::DARKYELLOW);
	const Color Color::MAGENTA(ColorF::MAGENTA);
	const Color Color::PURPLE(ColorF::PURPLE);
	const Color Color::PINK(ColorF::PINK);



	/////////////////////////////////////////////////////////////
	Color::Color(uint8 r, uint8 g, uint8 b, uint8 a)
		: r(r),
		g(g),
		b(b),
		a(a)
	{
	}



	/////////////////////////////////////////////////////////////
	Color::Color(const Color& other)
		: color(0)
	{
		*this = other;
	}



	/////////////////////////////////////////////////////////////
	Color::Color(const ColorF& colorf)
		: color(0)
	{
		*this = colorf;
	}



	/////////////////////////////////////////////////////////////
	Color::Color(const Math::Vector4& vector4)
		: color(0)
	{
		*this = vector4;
	}



	/////////////////////////////////////////////////////////////
	Color& Color::Add(const Color& other)
	{
		r += other.r;
		g += other.g;
		b += other.b;
		a += other.a;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Color& Color::Add(uint8 scalar)
	{
		r += scalar;
		g += scalar;
		b += scalar;
		a += scalar;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Color& Color::Subtract(const Color& other)
	{
		r -= other.r;
		g -= other.g;
		b -= other.b;
		a -= other.a;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Color& Color::Subtract(uint8 scalar)
	{
		r -= scalar;
		g -= scalar;
		b -= scalar;
		a -= scalar;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Color& Color::Multiply(const Color& other)
	{
		r *= other.r;
		g *= other.g;
		b *= other.b;
		a *= other.a;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Color& Color::Multiply(uint8 scalar)
	{
		r *= scalar;
		g *= scalar;
		b *= scalar;
		a *= scalar;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Color& Color::Divide(const Color& other)
	{
		r /= other.r;
		g /= other.g;
		b /= other.b;
		a /= other.a;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Color& Color::Divide(uint8 scalar)
	{
		r /= scalar;
		g /= scalar;
		b /= scalar;
		a /= scalar;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	bool Color::Equals(const Color& other) const
	{
		return (color == other.color);
	}



	/////////////////////////////////////////////////////////////
	std::string Color::ToString() const
	{
		using namespace std;
		return string("[Color][" + to_string(r) + ", " + to_string(g) + ", " + to_string(b) + ", " + to_string(a) + ']');
	}



	/////////////////////////////////////////////////////////////
	bool Color::operator==(const Color& other) const
	{
		return Equals(other);
	}



	/////////////////////////////////////////////////////////////
	bool Color::operator!=(const Color& other) const
	{
		return !Equals(other);
	}



	/////////////////////////////////////////////////////////////
	Color& Color::operator=(const Math::Vector4& other)
	{
		r = (uint8)(other.x * 255);
		g = (uint8)(other.y * 255);
		b = (uint8)(other.z * 255);
		a = (uint8)(other.w * 255);

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Color& Color::operator=(const ColorF& other)
	{
		r = (uint8)(other.r * 255);
		g = (uint8)(other.g * 255);
		b = (uint8)(other.b * 255);
		a = (uint8)(other.a * 255);

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Color& Color::operator=(const Color& other)
	{
		if (this != &other)
			color = other.color;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Color& Color::operator+=(const Color& other)
	{
		return Add(other);
	}



	/////////////////////////////////////////////////////////////
	Color& Color::operator-=(const Color& other)
	{
		return Subtract(other);
	}



	/////////////////////////////////////////////////////////////
	Color& Color::operator*=(const Color& other)
	{
		return Multiply(other);
	}



	/////////////////////////////////////////////////////////////
	Color& Color::operator/=(const Color& other)
	{
		return Divide(other);
	}



	/////////////////////////////////////////////////////////////
	Color& Color::operator+=(uint8 scalar)
	{
		return Add(scalar);
	}



	/////////////////////////////////////////////////////////////
	Color& Color::operator-=(uint8 scalar)
	{
		return Subtract(scalar);
	}



	/////////////////////////////////////////////////////////////
	Color& Color::operator*=(uint8 scalar)
	{
		return Multiply(scalar);
	}



	/////////////////////////////////////////////////////////////
	Color& Color::operator/=(uint8 scalar)
	{
		return Divide(scalar);
	}



	/////////////////////////////////////////////////////////////
	Color::operator ColorF()
	{
		ColorF result;

		result.r = r / 255.0f;
		result.g = g / 255.0f;
		result.b = b / 255.0f;
		result.a = a / 255.0f;

		return result;
	}



	/////////////////////////////////////////////////////////////
	Color operator-(Color left, const Color& right)
	{
		return left.Subtract(right);
	}



	/////////////////////////////////////////////////////////////
	Color operator+(Color left, const Color& right)
	{
		return left.Add(right);
	}



	/////////////////////////////////////////////////////////////
	Color operator*(Color left, const Color& right)
	{
		return left.Multiply(right);
	}



	/////////////////////////////////////////////////////////////
	Color operator/(Color left, const Color& right)
	{
		return left.Divide(right);
	}



	/////////////////////////////////////////////////////////////
	Color operator-(Color left, uint8 right)
	{
		return left.Subtract(right);
	}



	/////////////////////////////////////////////////////////////
	Color operator+(Color left, uint8 right)
	{
		return left.Add(right);
	}



	/////////////////////////////////////////////////////////////
	Color operator*(Color left, uint8 right)
	{
		return left.Multiply(right);
	}



	/////////////////////////////////////////////////////////////
	Color operator/(Color left, uint8 right)
	{
		return left.Divide(right);
	}



	/////////////////////////////////////////////////////////////
	Color operator*(uint8 left, Color right)
	{
		return right.Multiply(left);
	}
}