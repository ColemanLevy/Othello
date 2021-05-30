//Cpp file that holds the constructors and what all of the operand functions do for Vector2D variables
#include "Headers/Vector2D.h"

//Base construction for a Vector2D variable
Vector2D::Vector2D()
{
	x = 0.0f;
	y = 0.0f;
}
//Constructor that sets the x and y variables for the Vector2D instance
Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}
//Math operand for adding two Vector2Ds together
Vector2D& Vector2D::Add(const Vector2D& vec)
{
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}
//Math operand for subtracting two Vector2Ds
Vector2D& Vector2D::Sub(const Vector2D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}
//Math operand for multiplying two Vector2Ds
Vector2D& Vector2D::Mult(const Vector2D& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}
//Math operand for dividing two Vector2Ds
Vector2D& Vector2D::Div(const Vector2D& vec)
{
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}
//Operator override the determines what to do when + is used between two Vector2Ds
Vector2D& operator+(Vector2D& v1, const Vector2D& v2)
{
	return v1.Add(v2);
}
//Operator override the determines what to do when - is used between two Vector2Ds
Vector2D& operator-(Vector2D& v1, const Vector2D& v2)
{
	return v1.Sub(v2);
}
//Operator override the determines what to do when * is used between two Vector2Ds
Vector2D& operator*(Vector2D& v1, const Vector2D& v2)
{
	return v1.Mult(v2);
}
//Operator override the determines what to do when / is used between two Vector2Ds
Vector2D& operator/(Vector2D& v1, const Vector2D& v2)
{
	return v1.Div(v2);
}
//Operator override the determines what to do when += is used between two Vector2Ds
Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	return this->Add(vec);
}
//Operator override the determines what to do when -= is used between two Vector2Ds
Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
	return this->Sub(vec);
}
//Operator override the determines what to do when *= is used between two Vector2Ds
Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
	return this->Mult(vec);
}
//Operator override the determines what to do when /= is used between two Vector2Ds
Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
	return this->Div(vec);
}