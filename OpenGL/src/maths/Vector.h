#pragma once


class Vector4
{
public:
	Vector4
	(float x, float y, float z, float w) 
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	explicit Vector4(float f) : x(f), y(f), z(f), w(f) // member init list
	{}
	float x{ 0 };
	float y{ 0 };
	float z{ 0 };
	float w{ 0 };
};