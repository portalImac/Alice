// Object.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "GLHeaders.hpp"

#include "math/Vector3f.hpp"
#include "Color.hpp"
#include <vector>

namespace stein {

// An object made of solid triangles
struct Object {
    const size_t id;

    Object(size_t id, GLenum primitivesType = GL_TRIANGLES);
    ~Object();

    void drawObject() const;
    void sendPrimitives(const std::vector<Vector3f> &vertices, const std::vector<GLuint> &indices);
    void sendNormals(const std::vector<Vector3f> &normals);
    void sendColors(const std::vector<Color> &colors);
    void sendUvs(const std::vector<std::pair<float, float> > &uvs);

    GLboolean hasPrimitives() const {
        return m_bPrimitives;
    }
    GLboolean hasNormals() const {
        return m_bNormals;
    }
    GLboolean hasUvs() const {
        return m_bUvs;
    }
    GLboolean hasColors() const {
        return colorsVboId;
    }
private:
    GLuint vaoId; // vertex array objet id
    GLuint verticesVboId; // vertices buffer id
    GLuint normalsVboId; // normals buffer id
    GLuint uvsVboId; // normals buffer id
    GLuint colorsVboId; // normals buffer id
    GLuint indicesVboId; // indices buffer id
    GLuint nbIndices; // number of indices necessary to draw all the triangles
    GLenum primitivesType; // GL_TRIANGLES or GL_LINES
    GLboolean m_bPrimitives; // Are the primitives filled ?
    GLboolean m_bNormals; // Are the normals filled ?
    GLboolean m_bUvs; // Are the uv coordinates filled ?
    GLboolean m_bColors; // Are the colors filled ?
};

} // namespace stein

#endif //__OBJECT_HPP__
