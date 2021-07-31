#include "Lambertian.h"

bool Lambertian::scatter(const Ray& r_in, const Hit& hit, Color& attenuation, Ray& scattered) const
{
    Vector3 scatter_direction = hit.normal + random_unit_vector();
    scattered = Ray(hit.point, scatter_direction);

    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
        scatter_direction = hit.normal;
    
    attenuation = _albedo;
    return true;
}
