#include "Metal.h"

bool Metal::scatter(const Ray& r_in, const Hit& hit, Color& attenuation, Ray& scattered) const
{
    Vector3 reflected = reflect(unit_vector(r_in.direction()), hit.normal);
    scattered = Ray(hit.point, reflected + _fuzz * random_in_unit_sphere());
    attenuation = _albedo;
    return (dot(scattered.direction(), hit.normal) > 0);
}
