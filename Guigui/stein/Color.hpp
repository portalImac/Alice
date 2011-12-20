/*
 * Point3D.hpp
 *
 *  Created on: Dec 8, 2011
 *      Author: Guillaume Chatelet
 */

#ifndef COLOR_HPP_
#define COLOR_HPP_

#include <cassert>
#include <cmath>

namespace stein {

struct Color {
    float r, g, b, a;

    Color() :
        r(0), g(0), b(0), a(0) {
    }
    Color(float r, float g, float b) :
        r(r), g(g), b(b), a(1) {
    }
    Color(float r, float g, float b, float a) :
        r(r), g(g), b(b), a(a) {
    }

    operator const float*() const {
        return &r;
    }

    static Color BLACK;
    static Color WHITE;
    static Color GRAY;
    static Color RED;
    static Color GREEN;
    static Color BLUE;
};

} // namespace stein

#endif /* COLOR_HPP_ */
