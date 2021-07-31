#ifndef LAMBERTIANH
#define LAMBERTIANH

#include "material.h"

class lambertian : public material {
public:
    __device__ lambertian(const vec3 &a) : albedo(a) {}
    __device__ virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered, curandState *local_rand_state) const {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere(local_rand_state);
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }

    vec3 albedo;
};

#endif // !LAMBERTIANH

