#pragma once

#include "Vector3.h"
#include "Ray.h"

class Camera
{
public:
	Camera(
        Point3 lookfrom,
        Point3 lookat,
        Vector3   vup,
        double vfov,
        double aspect_ratio,
        double aperture,
        double focus_dist);
    Ray get_ray(double s, double t);

public:
    float _focal_length;

    Point3 _origin;
    Vector3 _horizontal;
    Vector3 _vertical;
    Vector3 _lower_left_corner;
    Vector3 _w, _u, _v;
    double _lens_radius;
};

