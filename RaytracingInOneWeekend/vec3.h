#pragma once

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3
{
public:
	vec3() : elements{ 0,0,0 } {}
	vec3(double e0, double e1, double e2) : elements{ e0,e1,e2 } {}

	double x() const { return elements[0]; }
	double y() const { return elements[1]; }
	double z() const { return elements[2]; }

	vec3 operator-() const { return vec3(-elements[0], -elements[1], -elements[2]); }
	double operator[](int i) const { return elements[i]; }
	double& operator[](int i) { return elements[i]; }

    vec3& operator+=(const vec3& v) {
        elements[0] += v.elements[0];
        elements[1] += v.elements[1];
        elements[2] += v.elements[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        elements[0] *= t;
        elements[1] *= t;
        elements[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return ( *this ) *= 1 / t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return elements[0] * elements[0] + elements[1] * elements[1] + elements[2] * elements[2];
    }
public:
	double elements[3];
};

using point3 = vec3;
using color = vec3;

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.elements[0] << ' ' << v.elements[1] << ' ' << v.elements[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.elements[0] + v.elements[0], u.elements[1] + v.elements[1], u.elements[2] + v.elements[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.elements[0] - v.elements[0], u.elements[1] - v.elements[1], u.elements[2] - v.elements[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.elements[0] * v.elements[0], u.elements[1] * v.elements[1], u.elements[2] * v.elements[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.elements[0], t * v.elements[1], t * v.elements[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.elements[0] * v.elements[0]
        + u.elements[1] * v.elements[1]
        + u.elements[2] * v.elements[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.elements[1] * v.elements[2] - u.elements[2] * v.elements[1],
        u.elements[2] * v.elements[0] - u.elements[0] * v.elements[2],
        u.elements[0] * v.elements[1] - u.elements[1] * v.elements[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}