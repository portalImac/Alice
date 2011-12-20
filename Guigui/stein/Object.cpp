// Object.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#include "Object.hpp"
#include <iostream>

using namespace std;

namespace stein {

struct ArrayBinder {
    ArrayBinder(GLuint id) :
        id(id) {
        glBindVertexArray(id);
    }
    ~ArrayBinder() {
        glBindVertexArray(0);
    }
private:
    const GLuint id;
};

// Default constructor
Object::Object(size_t id, GLenum primitivesType) :
    id(id), nbIndices(0), primitivesType(primitivesType), m_bPrimitives(GL_FALSE), m_bNormals(GL_FALSE), m_bUvs(GL_FALSE), m_bColors(GL_FALSE) {
    // Creation of ids for the buffers on GPU.
    // We store them in the structure for clarity
    // Creates a VAO id to handle the vao for objectTr
    glGenVertexArrays(1, &vaoId);
    // Creates a VBO id for a VBO to store the vertices
    glGenBuffers(1, &verticesVboId);
    // Creates a VBO id for a VBO to store the normals
    glGenBuffers(1, &normalsVboId);
    // Creates a VBO id for a VBO to store the uv coordinates (for textures)
    glGenBuffers(1, &uvsVboId);
    // Creates a VBO id for a VBO to store the colors
    glGenBuffers(1, &colorsVboId);
    // Creates a VBO id for a VBO to store the indices of the vertices
    glGenBuffers(1, &indicesVboId);
}

// Cleans memory for Object
Object::~Object() {
    // Cleans by deleting the buffers
    glDeleteBuffers(1, &indicesVboId);
    glDeleteBuffers(1, &colorsVboId);
    glDeleteBuffers(1, &uvsVboId);
    glDeleteBuffers(1, &normalsVboId);
    glDeleteBuffers(1, &verticesVboId);
    glDeleteVertexArrays(1, &vaoId);
}

static inline void pushArray(const GLenum attribute, const GLuint vboId, const GLfloat* pData, const size_t elementCount, const GLuint floatPerElement) {
    // Deactivates the generic attributes in shader
    // (specific with the use of our shader tool)
    glEnableVertexAttribArray(attribute);
    // Binds the vbo "objectTr->vboId" as a buffer for vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    // Fills the vbo with the vertices data, in a GPU memory special for "static draw" access
    // (data should be used to draw frequently, but not be changed often)
    glBufferData(GL_ARRAY_BUFFER, elementCount * floatPerElement * sizeof(GLfloat), pData, GL_STATIC_DRAW);
    // Specifies from where and how to read the data in the bound buffer
    glVertexAttribPointer(attribute, floatPerElement, GL_FLOAT, GL_FALSE, 0, (const GLvoid *) 0);
}

// Sends an array of vertices and an array of indices in buffers on the GPU
void Object::sendPrimitives(const vector<Vector3f> &vertices, const vector<GLuint> &indices) {
    ArrayBinder binder(vaoId);

    const GLenum attributePosition = 0; // First attribute in the shader is position
    pushArray(attributePosition, verticesVboId, (GLfloat*) vertices.data(), vertices.size(), 3);

//    cout << "vertices sent " << vertices.size() << endl;

    // Binds the other vbo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVboId);
    // Loads up the indices of the vertices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

//    cout << "face     sent " << indices.size() << endl;
    nbIndices = indices.size();
    m_bPrimitives = true;
}

// Sends an array of normals in buffers on the GPU
void Object::sendNormals(const vector<Vector3f> &values) {
    ArrayBinder binder(vaoId);

    const GLenum attributeNormal = 1; // Second attribute in the shader is the normal
    pushArray(attributeNormal, normalsVboId, (GLfloat*) values.data(), values.size(), 3);

//    cout << "normal sent " << values.size() << endl;

    m_bNormals = true;
}

// Sends an array of normals in buffers on the GPU
void Object::sendUvs(const vector<pair<float, float> > &values) {
    ArrayBinder binder(vaoId);

    const GLenum attributeUV = 2; // Third attribute in the shader is the uv
    pushArray(attributeUV, uvsVboId, (GLfloat*) values.data(), values.size(), 2);

//    cout << "UV sent " << values.size() << endl;

    m_bUvs = true;
}

// Sends an array of colors in buffers on the GPU
void Object::sendColors(const vector<Color> &values) {
    ArrayBinder binder(vaoId);

    const GLenum attributeColor = 3; // Fourth attribute in the shader is the color
    pushArray(attributeColor, colorsVboId, (GLfloat*) values.data(), values.size(), 4);

    m_bColors = true;
}

// Draw the object with the vao
void Object::drawObject() const {
    ArrayBinder binder(vaoId);

    // Draw the elements stored by the vao
    glDrawElements(primitivesType, nbIndices, GL_UNSIGNED_INT, 0);
}

} // namespace stein
