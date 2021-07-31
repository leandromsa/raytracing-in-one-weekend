#include "Dielectric.h"

bool Dielectric::scatter(const Ray& r_in, const Hit& hit, Color& attenuation, Ray& scattered) const
{
    attenuation = Color(1.0, 1.0, 1.0);
    double refraction_ratio = hit.front_face ? (1.0 / _ir) : _ir;

    Vector3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, hit.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Vector3 direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
        direction = reflect(unit_direction, hit.normal);
    else
        direction = refract(unit_direction, hit.normal, refraction_ratio);

    scattered = Ray(hit.point, direction);
    return true;
}

double Dielectric::reflectance(double cosine, double ref_idx)
{
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
