// Object.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "GLHeaders.hpp"
#include <vector>
#include <map>
#include <iterator>

// An object made of solid triangles
class Object
{
    public:
		GLuint vaoId;          // vertex array objet id
		GLuint vboId;          // vertices buffer id
		GLuint normalsVboId;   // normals buffer id
		GLuint uvsVboId;       // normals buffer id
		GLuint colorsVboId;    // normals buffer id
		GLuint indicesVboId;   // indices buffer id

		GLuint nbVertices;     // number of actual vertices
		GLuint nbIndices;      // number of indices necessary to draw all the triangles

        GLboolean primitives;  // Are the primitives filled ?
        GLboolean normals;     // Are the normals filled ?
        GLboolean colors;      // Are the colors filled ?
        GLboolean uvs;         // Are the uv coordinates filled ?

		GLenum primitivesType; // GL_TRIANGLES or GL_LINES

		GLuint nbTriangles;
		GLfloat* vertices;

        Object(GLenum primitivesType=GL_TRIANGLES);
        ~Object();

        void init(GLenum primitivesType);

        void sendPrimitives(GLfloat * vertices, GLuint * indices);
        void sendColors(GLfloat * colors);
		void sendNormals(GLfloat * normals);
        void sendUvs(GLfloat * uvs);

        void drawObject() const;

        std::map<GLuint, std::vector<GLfloat> > mapIndicesVertices; //indice, vertices
        std::map<GLuint, std::vector<GLuint> > mapFacesIndices; //face, indices
        std::map<GLuint, std::vector<GLfloat> > mapFacesNormals; //face, normals
};


#endif //__OBJECT_HPP__
