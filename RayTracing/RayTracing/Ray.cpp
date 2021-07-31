#include "Ray.h"

Point3 Ray::origin() const
{
    return _origin;
}

Vector3 Ray::direction() const
{
    return _direction;
}

Point3 Ray::at(double t) const
{
    return _origin + t*_direction;
}
