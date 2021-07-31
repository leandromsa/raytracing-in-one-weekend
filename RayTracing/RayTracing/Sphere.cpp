#include "Sphere.h"

bool Sphere::hit(const Ray& r, double t_min, double t_max, Hit& hit) const
{
    Vector3 oc = r.origin() - _center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - _radius * _radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    hit.t = root;
    hit.point = r.at(hit.t);
    Vector3 outward_normal = (hit.point - _center) / _radius;
    hit.set_face_normal(r, outward_normal);
    hit.material = _material;

    return true;
}
