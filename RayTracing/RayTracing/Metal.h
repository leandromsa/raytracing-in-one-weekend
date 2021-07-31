#pragma once
#include "Material.h"
#include "Vector3.h"

class Metal :
    public Material
{
public:
    Metal(const Color& albedo, double fuzz) : _albedo(albedo), _fuzz(fuzz < 1 ? fuzz : 1) {}
    virtual bool scatter(
        const Ray& r_in, const Hit& hit, Color& attenuation, Ray& scattered
    ) const override;
public:
    Color _albedo;
    double _fuzz;
};

