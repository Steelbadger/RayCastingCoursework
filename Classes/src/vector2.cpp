//////////////////////------------------//////////////////////
/*			
						By Ross Davies
															*/
//////////////////////------------------//////////////////////

#include <stdio.h>
#include "vector2.h"

Vector2::Vector2()
{
}

Vector2::Vector2(const float _x, const float _y):
	x(_x),
	y(_y)
{
}

Vector2::Vector2(const Vector2 &rhs):
	x(rhs.x),
	y(rhs.y)
{
}

Vector2::~Vector2()
{
}

Vector2& Vector2::operator +=(const Vector2 &rhs)
{
	x += rhs.x;
	y += rhs.y;

	return *this;
}

Vector2& Vector2::operator -=(const Vector2 &rhs)
{
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

float Vector2::Dot(const Vector2 &rhs) const
{
	return (x * rhs.x + y * rhs.y);
}

Vector2 Vector2::Rotate(float theta)
{
	float xcalc = x;
	x = xcalc * Cos(theta) - y * Sin(theta);
	y = xcalc * Sin(theta) + y * Cos(theta);
	return *this;
}

Vector2 Vector2::Translate(Vector2 dir, float dist)
{
	dir.NormaliseSelf();
	dir *= dist;
	x += dir.x;
	y += dir.y;
	return *this;
}

Vector2& Vector2::operator *=(const float s)
{
	x *= s;
	y *= s;

	return *this;
}

Vector2& Vector2::operator /=(const float s)
{
	x /= s;
	y /= s;

	return *this;
}

float Vector2::Length() const
{
	return Sqrt(x * x + y * y);
}

float Vector2::LengthSqr() const
{
	return (x * x + y * y);
}


bool Vector2::operator ==(const Vector2 &rhs) const
{
	return ((x == rhs.x) && (y == rhs.y));
}

Vector2 Vector2::Normalise()
{
	return (*this / this->Length());
}

void Vector2::NormaliseSelf()
{
	*this /= this->Length();
}