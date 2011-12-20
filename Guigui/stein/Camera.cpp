// Camera.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#include "Camera.hpp"
#include "Tools.hpp"

#include <iostream>
#include <cmath>

namespace stein {

// Default constructor
Camera::Camera() :
    xAxis(1, 0, 0), yAxis(0, 1, 0), zAxis(0, 0, 1), position(0, 0, 1) {
    updateView();
    setPerspectiveProjection(-1, 1, -1, 1, .1, 100);
}

// Updates view
void Camera::updateView() {
    // Rotation to be aligned with correct camera axis
    Matrix4f RcInv(xAxis.x, yAxis.x, zAxis.x, 0, xAxis.y, yAxis.y, zAxis.y, 0, xAxis.z, yAxis.z, zAxis.z, 0, 0, 0, 0, 1);

    // Translation to be at the right distance from the scene
    Matrix4f TcInv(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -position.x, -position.y, -position.z, 1);

    // Initializes
    view = RcInv * TcInv;
}

void Camera::setPerspectiveProjection(float left, float right, float bottom, float top, float near, float far) {
    projection = Matrix4f( //
        (2 * near) / (right - left), 0, 0, 0, //
        0, (2 * near) / (top - bottom), 0, 0, (right + left) / (right - left), //
        (top + bottom) / (top - bottom), -(far + near) / (far - near), -1, //
        0, 0, -(2 * far * near) / (far - near), 0);
}

void Camera::setOrthoProjection(float left, float right, float bottom, float top, float near, float far) {
    projection = Matrix4f( //
        2 / (right - left), 0, 0, 0, //
        0, 2 / (top - bottom), 0, 0, //
        0, 0, -2 / (far - near), 0, //
        -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1);
}

void Camera::setPosition(const Vector3f&p) {
    position = p;
    updateView();
}

const Vector3f& Camera::getPosition() const {
    return position;
}

const Matrix4f& Camera::getView() const {
    return view;
}

const Matrix4f& Camera::getProjection() const {
    return projection;
}

} // namespace stein
