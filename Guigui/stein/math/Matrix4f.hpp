/*
 * Point3D.hpp
 *
 *  Created on: Dec 8, 2011
 *      Author: Guillaume Chatelet
 */

#ifndef MATRIX4F_HPP_
#define MATRIX4F_HPP_

#include <cassert>
#include <cmath>
#include <algorithm>
#include <cassert>

namespace stein {

/**
 * Row major matrix
 */
struct Matrix4f {

    static Matrix4f identity() {
        Matrix4f identity;
        identity.setIdentity();
        return identity;
    }

    Matrix4f() {
        setZero();
    }

    Matrix4f(float _00, float _10, float _20, float _30, //
        float _01, float _11, float _21, float _31, //
        float _02, float _12, float _22, float _32, //
        float _03, float _13, float _23, float _33) {
        float values[] = { _00, _10, _20, _30, _01, _11, _21, _31, _02, _12, _22, _32, _03, _13, _23, _33 };
        std::copy(values, values + 16, m_Data);
    }

    void setZero() {
        std::fill(m_Data, m_Data + 16, 0);
    }

    void setIdentity() {
        setZero();
        for (size_t i = 0; i < 4; ++i)
            (*this)(i, i) = 1;
    }

    float& operator()(size_t col, size_t row) {
        assert(col<4);
        assert(row<4);
        return m_Data[index(col, row)];
    }

    const float& operator()(size_t col, size_t row) const {
        assert(col<4);
        assert(row<4);
        return m_Data[index(col, row)];
    }

    Matrix4f operator*(const Matrix4f& other) const {
        Matrix4f result;
        for (size_t i = 0; i < 16; ++i) {
            const size_t col = i % 4;
            const size_t row = i / 4;
            for (size_t k = 0; k < 4; ++k)
                result.m_Data[i] += m_Data[index(k, col)] * other.m_Data[index(row, k)];
        }
        return result;
    }

    void transpose() {
        std::swap((*this)(0, 1), (*this)(1, 0));
        std::swap((*this)(0, 2), (*this)(2, 0));
        std::swap((*this)(0, 3), (*this)(3, 0));
        std::swap((*this)(1, 2), (*this)(2, 1));
        std::swap((*this)(1, 3), (*this)(3, 1));
        std::swap((*this)(2, 3), (*this)(3, 2));
    }

    operator const float*() const {
        return m_Data;
    }
private:

    static inline size_t index(size_t col, size_t row) {
        return col * 4 + row;
    }

    float m_Data[16];
};

} // namespace stein

#endif /* MATRIX4F_HPP_ */
