#pragma once

#include "Hittable.h"
#include "Ray.h"
#include "Vector3.h"

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const Hit& hit, Color& attenuation, Ray& scaterred) const = 0;
};

