/*
 * Point3D.hpp
 *
 *  Created on: Dec 8, 2011
 *      Author: Guillaume Chatelet
 */

#ifndef VECTOR3F_HPP_
#define VECTOR3F_HPP_

#include <cassert>
#include <cmath>

namespace stein {

struct Vector3f {
    float x, y, z;

    Vector3f() :
        x(0), y(0), z(0) {
    }
    Vector3f(float x, float y, float z) :
        x(x), y(y), z(z) {
    }

    Vector3f operator+(const Vector3f &other) const {
        return Vector3f(x + other.x, y + other.y, z + other.z);
    }

    Vector3f operator/(const float value) const {
        return Vector3f(x / value, y / value, z / value);
    }

    Vector3f operator*(const float value) const {
        return Vector3f(x * value, y * value, z * value);
    }

    Vector3f operator-(const Vector3f &other) const {
        return Vector3f(x - other.x, y - other.y, z - other.z);
    }

    Vector3f operator-() const {
        return Vector3f(-x, -y, -z);
    }

    Vector3f& operator+=(const Vector3f&other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3f& operator-=(const Vector3f&other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3f& operator/=(const float value) {
        assert(value!=0);
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }

    Vector3f& operator*=(const float value) {
        x *= value;
        y *= value;
        z *= value;
        return *this;
    }

    float norm() const {
        return sqrt(x * x + y * y + z * z);
    }

    Vector3f& normalize() {
        const float n = norm();
        if (n != 0 && n != 1)
            (*this) /= n;
        return *this;
    }

    float dotP(const Vector3f &other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    Vector3f crossP(const Vector3f &other) const {
        return Vector3f( //
            y * other.z - z * other.y, //
            z * other.x - x * other.z, //
            x * other.y - y * other.x);
    }

    operator const float*() const {
        return &x;
    }
};

} // namespace stein

#endif /* VECTOR3F_HPP_ */
