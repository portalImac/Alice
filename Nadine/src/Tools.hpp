// Tools.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#ifndef __TOOLS_HPP__
#define __TOOLS_HPP__

// To include GL
#include "GLHeaders.hpp"

// C++
#include <string>
#include <vector>

GLfloat getNorm (GLfloat * a);
void normalize (GLfloat * a);
void vectorProduct (GLfloat * a, GLfloat * b, GLfloat * result);
void normalFace(GLfloat * O, GLfloat * A, GLfloat * B, GLfloat * normal, bool toNormalize);
void setNormalsFlat(GLuint nbIndices, GLfloat * vertices, GLuint * indices, GLfloat * normals);

void setToIdentity(GLfloat * matrix);
void setToTranslate(GLfloat * matrix, GLfloat * t);
void setToScale(GLfloat * matrix, GLfloat * s);
void setToRotate(GLfloat * matrix, GLfloat angle, GLfloat * axis);
void setPerspective(GLfloat * mat, GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);

void multMatrixBtoMatrixA(GLfloat * A, GLfloat * B);

void printVec2(GLfloat * vect);
void printVec3(GLfloat * vect);
void printVec4(GLfloat * vect);
void printMat16(GLfloat * mat);
void printGlErrors();

GLuint loadTexture(const std::string &fileName);

std::string * loadFile(const std::string & fileName);
void printShaderLog(GLint shaderId);
GLuint loadProgram(const std::vector<std::string> & files);
void setMatricesInShader(GLuint shaderId, GLfloat * model, GLfloat * view, GLfloat * c, GLfloat * projection);
void setLightInShader(GLuint shaderID, GLfloat * position, GLfloat power);
void setMaterialInShader(GLuint shaderID, GLfloat * ambient, GLfloat * diffuse, GLfloat * specular, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat shininess);
void changeMaterialColorInShader(GLuint shaderID, GLfloat * color);
void setFilledDataInShader(GLuint shaderID, GLboolean positions, GLboolean normals, GLboolean uvs, GLboolean colors);
void setTextureUnitsInShader(GLuint shaderID);

void changeColor(GLfloat *color);

unsigned char * loadPPM(const std::string & filename, GLuint * width, GLuint * height);

bool intersectRayTriangle(GLfloat * pos, GLfloat * dir, GLfloat * normal, GLfloat * A, GLfloat * B, GLfloat * C,
GLfloat * result);

uint64_t getTime();

#endif //  __TOOLS_HPP__
