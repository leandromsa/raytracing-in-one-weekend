#pragma once
#include "Material.h"
#include "Vector3.h"
class Dielectric :
    public Material
{
public:
    Dielectric(double inde_of_refraction) : _ir(inde_of_refraction) {};
    virtual bool scatter(
        const Ray& r_in, const Hit& hit, Color& attenuation, Ray& scattered
    ) const override;
private:
    static double reflectance(double cosine, double ref_idx);
public:
    double _ir;
};

