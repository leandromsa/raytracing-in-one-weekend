#include "Vector3.h"

double Vector3::x() {
	return e[0];
}

double Vector3::y() {
	return e[1];
}

double Vector3::z() {
	return e[2];
}

Vector3 Vector3::operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
double Vector3::operator[](int i) const { return e[i]; }
double& Vector3::operator[](int i) { return e[i]; }

Vector3& Vector3::operator+=(const Vector3& v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

Vector3& Vector3::operator*=(const double t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

bool Vector3::near_zero() const
{
	// Return true if the vector is close to zero in all dimensions.
	const auto s = 1e-8;
	return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
}

Vector3 Vector3::reflect(const Vector3& v, const Vector3& n)
{
	return v - 2 * dot(v, n) * n;
}

double Vector3::length() const {
	return sqrt(length_squared());
}

double Vector3::length_squared() const {
	return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}