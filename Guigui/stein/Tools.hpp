// Tools.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#ifndef __TOOLS_HPP__
#define __TOOLS_HPP__

// To include GL
#include "GLHeaders.hpp"

#include "math/Matrix4f.hpp"
#include "math/Vector3f.hpp"

// C++
#include <string>
#include <vector>

namespace stein {

Matrix4f translation(const Vector3f&t);
Matrix4f scale(const Vector3f&s);
Matrix4f xRotation(const float angle);
Matrix4f yRotation(const float angle);
Matrix4f zRotation(const float angle);
Matrix4f rotation(const float angle, const Vector3f &axis);

void printGlErrors();

GLuint loadTexture(const char* fileName);
GLuint loadProgram(const std::vector<std::string> & files);
GLuint loadProgram(const std::string& file);
void printShaderLog(GLint shaderId);

std::string * loadFile(const std::string & fileName);
unsigned char * loadPPM(const char* filename, GLuint &width, GLuint &height);

void setMatricesInShader(GLuint shaderId, const Matrix4f &model, const Matrix4f &view, const Vector3f &eye, const Matrix4f &projection);
void setLightInShader(GLuint shaderID, GLfloat * position, GLfloat power);
void setMaterialInShader(GLuint shaderID, GLfloat * ambient, GLfloat * diffuse, GLfloat * specular, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat shininess);
void setFilledDataInShader(GLuint shaderID, GLboolean positions, GLboolean normals, GLboolean uvs, GLboolean colors);
void setTextureUnitsInShader(GLuint shaderID);

void changeMaterialColorInShader(GLuint shaderID, GLfloat * color);

uint64_t getTime();

} // namespace stein

#endif //  __TOOLS_HPP__
