#pragma once
#include "Ray.h"

class Material;

struct Hit {
	Point3 point;
	Vector3 normal;
	double t;
	bool front_face;
	shared_ptr<Material> material;
	inline void set_face_normal(const Ray& r, const Vector3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable
{
public:
	virtual bool hit(const Ray& r, double t_min, double t_max, Hit& hit) const = 0;
};

