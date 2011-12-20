// Object.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#include "Object.hpp"



// Default constructor
Object::Object(GLenum primitivesType)
{
    this->init(primitivesType);
}

// Cleans memory for Object
Object::~Object()
{
    // Cleans by deleting the buffers
    glDeleteBuffers(1, &(this->vboId));
    glDeleteBuffers(1, &(this->normalsVboId));
    glDeleteBuffers(1, &(this->colorsVboId));
    glDeleteBuffers(1, &(this->uvsVboId));
    glDeleteBuffers(1, &(this->indicesVboId));
    glDeleteVertexArrays(1, &(this->vaoId));
}


// Inits parameters for the object, 
// and values necessary for the storage of the object on the GPU
void Object::init(GLenum primitivesType)
{
    this->nbVertices=0;
    this->nbIndices=0;
    this->primitivesType=primitivesType;

    this->primitives=GL_FALSE;
    this->normals=GL_FALSE;
    this->uvs=GL_FALSE;
    this->colors=GL_FALSE;

	// Creation of ids for the buffers on GPU.
	// We store them in the structure for clarity
    	// Creates a VAO id to handle the vao for objectTr
    glGenVertexArrays(1, &(this->vaoId));
    	// Creates a VBO id for a VBO to store the vertices
    glGenBuffers(1, &(this->vboId));
    	// Creates a VBO id for a VBO to store the normals
    glGenBuffers(1, &(this->normalsVboId));
    	// Creates a VBO id for a VBO to store the colors
    glGenBuffers(1, &(this->colorsVboId)); 
    	// Creates a VBO id for a VBO to store the uv coordinates (for textures)
    glGenBuffers(1, &(this->uvsVboId));        
    	// Creates a VBO id for a VBO to store the indices of the vertices
    glGenBuffers(1, &(this->indicesVboId));
}


// Sends an array of vertices and an array of indices in buffers on the GPU
void Object::sendPrimitives(GLfloat * vertices, GLuint * indices)
{
    // Binds the vao
    glBindVertexArray(this->vaoId);
    
    // Deactivates the generic attributes in shader 
    // (specific with the use of our shader tool)
    GLenum attributePosition=0; // First attribute in the shader is position
    glEnableVertexAttribArray(attributePosition);

    // Binds the vbo "objectTr->vboId" as a buffer for vertices
    glBindBuffer(GL_ARRAY_BUFFER, this->vboId);
    // Fills the vbo with the vertices data, in a GPU memory special for "static draw" access 
    // (data should be used to draw frequently, but not be changed often)
    glBufferData(GL_ARRAY_BUFFER, this->nbVertices*4*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    // Specifies from where and how to read the data in the bound buffer
    glVertexAttribPointer(attributePosition, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);

    // Binds the other vbo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesVboId); 
    // Loads up the indices of the vertices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nbIndices*sizeof(GLuint), indices, GL_STATIC_DRAW);

    // Unbinds the vao
    glBindVertexArray(0);
    
    this->primitives=true;
}


// Sends an array of colors in buffers on the GPU
void Object::sendColors(GLfloat * colors)
{
    // Binds the vao
    glBindVertexArray(this->vaoId);
    
    // Activates the generic attribute of normal in shader
    GLenum attributeColor=3; // Fourth attribute in the shader is the color
    glEnableVertexAttribArray(attributeColor);

    // Binds the vbo "object->colorsVboId" as a buffer for normals
    glBindBuffer(GL_ARRAY_BUFFER, this->colorsVboId);
    // Fills the vbo with the colors data, in a GPU memory special for "static draw" access 
    // (data should be used to draw frequently, but not be changed often)
    glBufferData(GL_ARRAY_BUFFER, this->nbVertices*4*sizeof(GLfloat), colors, GL_STATIC_DRAW);
    // Specifies from where and how to read the data in the bound buffer
    glVertexAttribPointer(attributeColor, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);

    // Unbinds the vao
    glBindVertexArray(0);
    
    this->colors=true;
}

// Sends an array of normals in buffers on the GPU
void Object::sendNormals(GLfloat * normals)
{
	// Binds the vao
	glBindVertexArray(this->vaoId);

	// Activates the generic attribute of normal in shader
	GLenum attributeNormal=1; // Second attribute in the shader is the normal
	glEnableVertexAttribArray(attributeNormal);

	// Binds the vbo "object->normalsVboId" as a buffer for normals
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsVboId);
	// Fills the vbo with the normals data, in a GPU memory special for "static draw" access
	// (data should be used to draw frequently, but not be changed often)
	glBufferData(GL_ARRAY_BUFFER, this->nbVertices*3*sizeof(GLfloat), normals,GL_STATIC_DRAW);
	// Specifies from where and how to read the data in the bound buffer
	glVertexAttribPointer(attributeNormal, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);

	// Unbinds the vao
	glBindVertexArray(0);

	this->normals=true;
}

// Sends an array of colors in buffers on the GPU
void Object::sendUvs(GLfloat * uvs)
{
     // Binds the vao
    glBindVertexArray(this->vaoId);

    // Activates the generic attribute of normal in shader
    GLenum attributeUvs=2; // Third attribute in the shader is the texture coordinates
    glEnableVertexAttribArray(attributeUvs);

    // Binds the vbo "object->colorsVboId" as a buffer for normals
    glBindBuffer(GL_ARRAY_BUFFER, this->uvsVboId);
    // Fills the vbo with the colors data, in a GPU memory special for "static draw" access
    // (data should be used to draw frequently, but not be changed often)
    glBufferData(GL_ARRAY_BUFFER, this->nbVertices*2*sizeof(GLfloat), uvs, GL_STATIC_DRAW);
    // Specifies from where and how to read the data in the bound buffer
    glVertexAttribPointer(attributeUvs, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);

    // Unbinds the vao
    glBindVertexArray(0);

    this->uvs=true;
}

// Draw the object with the vao
void Object::drawObject() const
{
	// Binds the vao to draw (the vbos binds are stored in the vao, no need to redo them)
    glBindVertexArray(this->vaoId); 
    // Draw the elements stored by the vao               
    glDrawElements(this->primitivesType, this->nbIndices, GL_UNSIGNED_INT, 0);
}

