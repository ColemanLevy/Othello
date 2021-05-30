//Header file that contains all the constructors and functions for the Vector2D variable class
#pragma once
#include <iostream>

class Vector2D
{
public:
	//The x and y positions of the variable
	float x;
	float y;

	//The constructors for the variable, both default and specified
	Vector2D();
	Vector2D(float x, float y);

	//The rest of the functions shown here are for overriding math operations between two Vector2Ds. These
	//give the program insight on what to do when a and b (both Vector2D variables) are added, subtracted,
	//multiplied, or divided against each other. (Even includes the +, -, *, and / symbol operations)
	Vector2D& Add(const Vector2D& vec);
	Vector2D& Sub(const Vector2D& vec);
	Vector2D& Mult(const Vector2D& vec);
	Vector2D& Div(const Vector2D& vec);

	friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);
};