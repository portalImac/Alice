/*
 * StreamUtils.cpp
 *
 *  Created on: Dec 10, 2011
 *      Author: Guillaume Chatelet
 */

#include "StreamUtils.h"

using namespace std;
using namespace stein;

namespace {

const char* SEPARATOR = ", ";

} // namespace

ostream& operator<<(ostream& stream, const Vector3f&v) {
    stream << '[' << v.x << SEPARATOR << v.y << SEPARATOR << v.z << ']';
    return stream;
}

ostream& operator<<(ostream& stream, const Matrix4f&m) {
    for (size_t row = 0; row < 4; ++row) {
        stream << '[';
        for (size_t col = 0; col < 4; ++col) {
            if (col != 0)
                stream << SEPARATOR;
            stream << m(col, row);
        }
        stream << ']';
    }
    return stream;
}
