#pragma once
#include "Material.h"
#include "Hittable.h"

class Sphere :
    public Hittable
{
public:
    Sphere(Vector3 center, double radius, shared_ptr<Material> material) : _center(center), _radius(radius), _material(material) {};

    virtual bool hit(const Ray& r, double t_min, double t_max, Hit& hit) const override;
public:
    Vector3 _center;
    double _radius;
    shared_ptr<Material> _material;
};

