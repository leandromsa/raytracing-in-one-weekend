#pragma once
#include <cmath>
#include <iostream>
#include "Common.h"

class Vector3
{
	public:
		Vector3() : e{ 0, 0, 0 } {}
		Vector3(double x, double y, double z) : e{ x, y, z } {}

		double x();
		double y();
		double z();

		Vector3 operator-() const;
		double operator[](int i) const;
		double& operator[](int i);

		Vector3& operator+=(const Vector3& v);
		Vector3& operator*=(const double t);
		Vector3& operator/=(const double t);

		inline static Vector3 random() {
			return Vector3(random_double(), random_double(), random_double());
		}

		inline static Vector3 random(double min, double max) {
			return Vector3(random_double(min, max), random_double(min, max), random_double(min, max));
		}

		bool near_zero() const;

		Vector3 reflect(const Vector3& v, const Vector3& n);

		double length() const;
		double length_squared() const;

	public:
		double e[3];
};

inline Vector3 operator*(const Vector3& u, const Vector3& v)
{
	return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3 operator*(double t, const Vector3& v)
{
	return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector3 operator+(const Vector3& u, const Vector3& v)
{
	return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3 operator-(const Vector3& u, const Vector3& v)
{
	return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3& Vector3::operator/=(const double t) {
	return *this *= 1 / t;
}

inline Vector3 operator/(Vector3 v, double t)
{
	return (1 / t) * v;
}

inline std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vector3 operator*(const Vector3& v, double t)
{
	return t * v;
}

inline double dot(const Vector3& u, const Vector3& v)
{
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline Vector3 cross(const Vector3& u, const Vector3& v)
{
	return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vector3 unit_vector(Vector3 v)
{
	return v / v.length();
}

inline Vector3 random_in_unit_sphere() {
	while (true) {
		auto p = Vector3::random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

inline Vector3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

inline Vector3 random_in_hemisphere(const Vector3& normal) {
	Vector3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0)
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

inline Vector3 reflect(const Vector3& v, const Vector3& n) {
	return v - 2 * dot(v, n) * n;
}

inline Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat) {
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	Vector3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	Vector3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

inline Vector3 random_in_unit_disk() {
	while (true) {
		auto p = Vector3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

using Point3 = Vector3;
using Color = Vector3;
