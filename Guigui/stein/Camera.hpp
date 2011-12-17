// Camera.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "math/Vector3f.hpp"
#include "math/Matrix4f.hpp"

namespace stein {

// A camera to watch the scene
struct Camera {
    Camera();

    void setPosition(const Vector3f&);
    void setPerspectiveProjection(float left, float right, float bottom, float top, float near, float far);
    void setOrthoProjection(float left, float right, float bottom, float top, float near, float far);

    const Vector3f& getPosition() const;
    const Matrix4f& getView() const;
    const Matrix4f& getProjection() const;
private:
    void updateView();

    const Vector3f xAxis; // Camera axis x : right side
    const Vector3f yAxis; // Camera axis y : up
    const Vector3f zAxis; // Camera axis z : backward

    Vector3f position; // Camera position
    Matrix4f view; // View matrix
    Matrix4f projection; // Projection matrix
};

} // namespace stein

#endif // __CAMERA_HPP__
