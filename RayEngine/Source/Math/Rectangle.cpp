#include "..\..\Include\Math\Rectangle.h"

namespace Math
{
	/////////////////////////////////////////////////////////////
	Rectangle::Rectangle()
		: TopLeft(),
		BottomRight()
	{
	}



	/////////////////////////////////////////////////////////////
	Rectangle::Rectangle(const Math::Vector2& topLeft, const Math::Vector2& bottomRight)
		: TopLeft(topLeft),
		BottomRight(bottomRight)
	{
	}
}