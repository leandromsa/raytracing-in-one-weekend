#pragma once
#include "Ray.h"
#include "Material.h"
#include "Vector3.h"

class Lambertian : public Material
{
public:
	Lambertian(const Color& albedo) : _albedo(albedo) {};

    virtual bool scatter(
        const Ray& r_in, const Hit& hit, Color& attenuation, Ray& scattered
    ) const override;


public:
	Color _albedo;
};

