#pragma once
#include "Vector3.h"

class Ray
{
public:
	Ray() {};
	Ray(const Point3& origin, const Vector3& direction) : _origin(origin), _direction(direction) {};

	Point3 origin() const;
	Vector3 direction() const;

	Point3 at(double t) const;

public:
	Point3 _origin;
	Vector3 _direction;
};

