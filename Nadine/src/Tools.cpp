// Tools.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#include "Tools.hpp"

#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <math.h>

// To get the norm of a vector
GLfloat getNorm (GLfloat * a)
{
    return sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
}


// To normalize a vector
void normalize (GLfloat * a)
{
	GLfloat norm=getNorm(a);
	if (norm!=0.0)
	{
	    a[0]/=norm;
	    a[1]/=norm;
	    a[2]/=norm;
	}
}


// To get the vector product
void vectorProduct (GLfloat * a, GLfloat * b, GLfloat * result)
{
	result[0]=a[1]*b[2] - a[2]*b[1];
	result[1]=a[2]*b[0] - a[0]*b[2];
	result[2]=a[0]*b[1] - a[1]*b[0];
}

// Returns the dot product between a and b
GLfloat dotProduct(GLfloat * a, GLfloat * b)
{
    GLfloat result=0.0;
    for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
    {
        result+=a[iCoord]*b[iCoord];
    }
    return result;
}

// Returns the scalar triple product between a, b and c
GLfloat scalarTriple(GLfloat * a, GLfloat * b, GLfloat * c)
{
    GLfloat result[4];
    vectorProduct (b, c, result);
    return dotProduct(a, result);
}

//
void normalFace(GLfloat * O, GLfloat * A, GLfloat * B, GLfloat * normal, bool toNormalize)
{
	GLfloat a[] = {A[0]-O[0], A[1]-O[1], A[2]-O[2]};
	GLfloat b[] = {B[0]-O[0], B[1]-O[1], B[2]-O[2]};
	vectorProduct(a, b, normal);
	if(toNormalize) normalize(normal);
}

//
void setNormalsFlat(GLuint nbIndices, GLfloat * vertices, GLuint * indices, GLfloat * normals)
{
	for(GLuint iTriangle=0; iTriangle<nbIndices; iTriangle+=3) {
		GLuint iIndices0=indices[iTriangle+0];
		GLuint iIndices1=indices[iTriangle+1];
		GLuint iIndices2=indices[iTriangle+2];

		GLuint iVertices0=iIndices0*4;
		GLuint iVertices1=iIndices1*4;
		GLuint iVertices2=iIndices2*4;

		GLuint iNormals0=iIndices0*3;
		GLuint iNormals1=iIndices1*3;
		GLuint iNormals2=iIndices2*3;

		normalFace(&(vertices[iVertices0]), &(vertices[iVertices1]), &(vertices[iVertices2]), &(normals[iNormals0]), true);
		
		normals[iNormals1+0]=normals[iNormals0+0];
		normals[iNormals1+1]=normals[iNormals0+1];
		normals[iNormals1+2]=normals[iNormals0+2];
		normals[iNormals2+0]=normals[iNormals0+0];
		normals[iNormals2+1]=normals[iNormals0+1];
		normals[iNormals2+2]=normals[iNormals0+2];
	}
}

// Expensive matrix inversion
void getInverseGenericMatrix(float * A, float * B)
{
	B[0]  =  A[5]*A[10]*A[15] - A[5]*A[14]*A[11] - A[6]*A[9]*A[15] + A[6]*A[13]*A[11] + A[7]*A[9]*A[14] - A[7]*A[13]*A[10];
	B[1]  = -A[1]*A[10]*A[15] + A[1]*A[14]*A[11] + A[2]*A[9]*A[15] - A[2]*A[13]*A[11] - A[3]*A[9]*A[14] + A[3]*A[13]*A[10];
	B[2]  =  A[1]*A[6]*A[15]  - A[1]*A[14]*A[7]  - A[2]*A[5]*A[15] + A[2]*A[13]*A[7]  + A[3]*A[5]*A[14] - A[3]*A[13]*A[6];
	B[3]  = -A[1]*A[6]*A[11]  + A[1]*A[10]*A[7]  + A[2]*A[5]*A[11] - A[2]*A[9]*A[7]   - A[3]*A[5]*A[10] + A[3]*A[9]*A[6];
	B[4]  = -A[4]*A[10]*A[15] + A[4]*A[14]*A[11] + A[6]*A[8]*A[15] - A[6]*A[12]*A[11] - A[7]*A[8]*A[14] + A[7]*A[12]*A[10];
	B[5]  =  A[0]*A[10]*A[15] - A[0]*A[14]*A[11] - A[2]*A[8]*A[15] + A[2]*A[12]*A[11] + A[3]*A[8]*A[14] - A[3]*A[12]*A[10];
	B[6]  = -A[0]*A[6]*A[15]  + A[0]*A[14]*A[7]  + A[2]*A[4]*A[15] - A[2]*A[12]*A[7]  - A[3]*A[4]*A[14] + A[3]*A[12]*A[6];
	B[7]  =  A[0]*A[6]*A[11]  - A[0]*A[10]*A[7]  - A[2]*A[4]*A[11] + A[2]*A[8]*A[7]   + A[3]*A[4]*A[10] - A[3]*A[8]*A[6];
	B[8]  =  A[4]*A[9]*A[15]  - A[4]*A[13]*A[11] - A[5]*A[8]*A[15] + A[5]*A[12]*A[11] + A[7]*A[8]*A[13] - A[7]*A[12]*A[9];
	B[9]  = -A[0]*A[9]*A[15]  + A[0]*A[13]*A[11] + A[1]*A[8]*A[15] - A[1]*A[12]*A[11] - A[3]*A[8]*A[13] + A[3]*A[12]*A[9];
	B[10] =  A[0]*A[5]*A[15]  - A[0]*A[13]*A[7]  - A[1]*A[4]*A[15] + A[1]*A[12]*A[7]  + A[3]*A[4]*A[13] - A[3]*A[12]*A[5];
	B[11] = -A[0]*A[5]*A[11]  + A[0]*A[9]*A[7]   + A[1]*A[4]*A[11] - A[1]*A[8]*A[7]   - A[3]*A[4]*A[9]  + A[3]*A[8]*A[5];
	B[12] = -A[4]*A[9]*A[14]  + A[4]*A[13]*A[10] + A[5]*A[8]*A[14] - A[5]*A[12]*A[10] - A[6]*A[8]*A[13] + A[6]*A[12]*A[9];
	B[13] =  A[0]*A[9]*A[14]  - A[0]*A[13]*A[10] - A[1]*A[8]*A[14] + A[1]*A[12]*A[10] + A[2]*A[8]*A[13] - A[2]*A[12]*A[9];
	B[14] = -A[0]*A[5]*A[14]  + A[0]*A[13]*A[6]  + A[1]*A[4]*A[14] - A[1]*A[12]*A[6]  - A[2]*A[4]*A[13] + A[2]*A[12]*A[5];
	B[15] =  A[0]*A[5]*A[10]  - A[0]*A[9]*A[6]   - A[1]*A[4]*A[10] + A[1]*A[8]*A[6]   + A[2]*A[4]*A[9]  - A[2]*A[8]*A[5];	

	float det = A[0]*B[0] + A[4]*B[1] + A[8]*B[2] + A[12]*B[3];
	for (unsigned int i=0; i<16; ++i)
	    B[i]=B[i]/det;
}

void multVertexWithMatrix(GLfloat * vertex, GLfloat * matrix, GLfloat * result, GLuint dim)
{
	if (dim==4)
	{
		result[0] = vertex[0]*matrix[0] + vertex[1]*matrix[4] + vertex[2]*matrix[8] + vertex[3]*matrix[12];
		result[1] = vertex[0]*matrix[1] + vertex[1]*matrix[5] + vertex[2]*matrix[9] + vertex[3]*matrix[13];
		result[2] = vertex[0]*matrix[2] + vertex[1]*matrix[6] + vertex[2]*matrix[10] + vertex[3]*matrix[14];
		result[3] = vertex[0]*matrix[3] + vertex[1]*matrix[7] + vertex[2]*matrix[11] + vertex[3]*matrix[15];
	}
	else
	{
		result[0] = vertex[0]*matrix[0] + vertex[1]*matrix[4] + vertex[2]*matrix[8];
		result[1] = vertex[0]*matrix[1] + vertex[1]*matrix[5] + vertex[2]*matrix[9];
		result[2] = vertex[0]*matrix[2] + vertex[1]*matrix[6] + vertex[2]*matrix[10];
	}
	
}




/*
 * if (dim == 4)
	{
		GLfloat init[4] = {vertex[0], vertex[1], vertex[2], vertex[3]} ;
		vertex[0] = matrix[0]*init[0] + matrix[1]*init[1] + matrix[2]*init[2] + matrix[3]*init[3];
		vertex[1] = matrix[4]*init[0] + matrix[5]*init[1] + matrix[6]*init[2] + matrix[7]*init[3];
		vertex[2] = matrix[8]*init[0] + matrix[9]*init[1] + matrix[10]*init[2] + matrix[11]*init[3];
		vertex[3] = matrix[12]*init[0] + matrix[13]*init[1] + matrix[14]*init[2] + matrix[15]*init[3];
	}
	else
	{
		GLfloat init[3] = {vertex[0], vertex[1], vertex[2]} ;
		vertex[0] = matrix[0]*init[0] + matrix[1]*init[1] + matrix[2]*init[2];
		vertex[1] = matrix[4]*init[0] + matrix[5]*init[1] + matrix[6]*init[2];
		vertex[2] = matrix[8]*init[0] + matrix[9]*init[1] + matrix[10]*init[2];
	}
	*/

// Does the multiplication A=A*B : all the matrices are described column-major
void multMatrixBtoMatrixA(GLfloat * A, GLfloat * B)
{
    int i=0; // row index
    int j=0; // column index
    GLfloat temp[16];
    
    for (int iValue=0 ; iValue<16 ; iValue++)
    {
        temp[iValue]=0;
        //j=iValue%4; // if raw-major
        //i=iValue/4; // if raw-major
        i=iValue%4; // if column-major
        j=iValue/4; // if column-major
        for (int k=0 ; k<4 ; k++)
        {
            int indexik=k*4+i;
            int indexkj=j*4+k;
            temp[iValue]+=A[indexik]*B[indexkj];
        }
    }
    
    for (int iValue=0 ; iValue<16 ; iValue++)
        A[iValue]=temp[iValue];
}


// Sets the provided matrix to identity
void setToIdentity(GLfloat * matrix)
{
    GLfloat I[]={1.0, 0.0, 0.0, 0.0, 
                 0.0, 1.0, 0.0, 0.0, 
                 0.0, 0.0, 1.0, 0.0, 
                 0.0, 0.0, 0.0, 1.0};
    for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
        matrix[iMatrixCoord]=I[iMatrixCoord];
}


// Sets the provided matrix to a translate matrix on vector t
void setToTranslate(GLfloat * matrix, GLfloat * t)
{
    GLfloat T[]={1.0,   0.0,   0.0,   0.0,
                 0.0,   1.0,   0.0,   0.0,
                 0.0,   0.0,   1.0,   0.0,
                 t[0],  t[1],  t[2],  1.0}; 
    for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
        matrix[iMatrixCoord]=T[iMatrixCoord];
}


// Sets the provided matrix to a scale matrix by coeficients in s
void setToScale(GLfloat * matrix, GLfloat * s)
{
    GLfloat S[]={s[0], 0.0,  0.0,  0.0,
                 0.0,  s[1], 0.0,  0.0,
                 0.0,  0.0,  s[2], 0.0,
                 0.0,  0.0,  0.0,  1.0};  
    for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
        matrix[iMatrixCoord]=S[iMatrixCoord];
}


// Sets the provided matrix to a rotate matrix of angle "angle", around axis "axis"
void setToRotate(GLfloat * matrix, GLfloat angle, GLfloat * axis)
{
    GLfloat c=cos(angle);
    GLfloat s=sin(angle);
    GLfloat x=axis[0]; 
    GLfloat y=axis[1]; 
    GLfloat z=axis[2];

    if ((x==1.0) && (y==0.0) && (z==0.0))
    {
        GLfloat R[]={1.0, 0.0, 0.0, 0.0, 
                     0.0, c,   s,   0.0, 
                     0.0, -s,  c,   0.0, 
                     0.0, 0.0, 0.0, 1.0};
        for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
            matrix[iMatrixCoord]=R[iMatrixCoord];
    }
    else
    {
        if ((x==0.0) && (y==1.0) && (z==0.0))
        {                    
            GLfloat R[]={c,   0.0, -s,  0.0, 
                         0.0, 1.0, 0.0, 0.0, 
                         s,   0.0, c,   0.0, 
                         0.0, 0.0, 0.0, 1.0};
            for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
                matrix[iMatrixCoord]=R[iMatrixCoord];
        }
        else
        {

            if ((x==0.0) && (y==0.0) && (z==1.0))
            {                                          
                GLfloat R[]={c,   s,   0.0, 0.0, 
                             -s,  c,   0.0, 0.0, 
                             0.0, 0.0, 1.0, 0.0, 
                             0.0, 0.0, 0.0, 1.0};
                for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
                    matrix[iMatrixCoord]=R[iMatrixCoord];
            }
            else
            {
                GLfloat R[]={ (1.0-c)*(x*x-1.0) + 1.0, (1.0-c)*x*y + (z*s),     (1.0-c)*x*z - (y*s),      0.0, 
                              (1.0-c)*x*y - (z*s),     (1.0-c)*(y*y-1.0) + 1.0, (1.0-c)*y*z + (x*s),      0.0, 
                              (1.0-c)*x*z + (y*s),     (1.0-c)*y*z - (x*s),     (1.0-c)*(z*z-1.0) + 1.0,  0.0, 
                              0.0,                     0.0,                     0.0,                      1.0};
                for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
                    matrix[iMatrixCoord]=R[iMatrixCoord];
                std::cout<<"Rotation on non standard axis."<<std::endl;
            }
        }
    }
}


// Builds a perspective projection matrix and stores it in mat
// l=left, r=right, b=bottom, t=top, n=near, f=far in the frustum
void setPerspective(GLfloat * mat, GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)
{
    GLfloat P[]={(2*n)/(r-l), 0.0, 0.0, 0.0,
                 0.0, (2*n)/(t-b), 0.0, 0.0,
                 (r+l)/(r-l), (t+b)/(t-b), -(f+n)/(f-n), -1.0,
                 0.0, 0.0, 0.0, 0.0};
    for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
        mat[iMatrixCoord]=P[iMatrixCoord];
}


// To print a vector of 2 values (textures 2D coordinates for example)
void printVec2(GLfloat * vect)
{
	std::cout<<"["<<vect[0]<<", "<<vect[1]<<"]"<<std::endl;
}


// To print a vector of 3 values (a normal for example)
void printVec3(GLfloat * vect)
{
	std::cout<<"["<<vect[0]<<", "<<vect[1]<<", "<<vect[2]<<"]"<<std::endl;
}


// To print a vector of 4 values (a position for example)
void printVec4(GLfloat * vect)
{
	std::cout<<"["<<vect[0]<<", "<<vect[1]<<", "<<vect[2]<<", "<<vect[3]<<"]"<<std::endl;
}


// To print a matrix (written in math form, not in column-major 16 values array)
void printMat16(GLfloat * mat)
{
    std::cout<<std::endl;
    for (GLuint iRow=0 ; iRow<4 ; iRow++)
    {
        std::cout<<"| ";
        for (GLuint iColumn=0 ; iColumn<4 ; iColumn++)
        {
            std::cout<<mat[(iColumn*4)+iRow];
            std::cout<<" ";
        }
        std::cout<<"|"<<std::endl;
    }
}


// Prints the stack of glErrors, or nothing if no error occured
void printGlErrors()
{
    GLenum error = glGetError();
    
    // The glGetError function returns one error at a time, and then unstacks it.
    // Here we call it until all the errors are shown.
    while (error!=GL_NO_ERROR)
    {
        std::cout<<"!! GL Error : ";
        if (error==GL_INVALID_ENUM)
            std::cout<<"GL_INVALID_ENUM"<<std::endl;
        if (error==GL_INVALID_VALUE)
            std::cout<<"GL_INVALID_VALUE"<<std::endl;
        if (error==GL_INVALID_OPERATION)
            std::cout<<"GL_INVALID_OPERATION"<<std::endl;
        if (error==GL_OUT_OF_MEMORY)
            std::cout<<"GL_OUT_OF_MEMORY"<<std::endl;

        error=glGetError();

        std::cout<<std::endl;
        //exit(1);
    }
}


// Loads a simple texture
GLuint loadTexture(const std::string &fileName)
{
    GLuint  w;
    GLuint  h;
    // Loads the image from a ppm file to an unsigned char array
    unsigned char *data=loadPPM(fileName, &w, &h);

    // Allocates a texture id
    GLuint textureID;
    glGenTextures(1, &textureID);
    // Selects our current texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // How to handle not normalised uvs
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // How to handle interpolation from texels to fragments
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Specifies which image will be used for this texture objet
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    return textureID;
}



// Writes content of a text file [fileName] in a returned std::string
std::string* loadFile(const std::string &fileName)
{
    std::string* result = new std::string();
    std::ifstream file(fileName.c_str());
    if (!file) 
    {
        std::cerr << "Cannot open file " << fileName << std::endl;
        throw std::exception();
    }
    std::string line;
    while (getline(file, line)) 
    {
        *result += line;
        *result += '\n';
    }
    file.close();
    return result;
}



// Prints info about potential problems which occured during compilation
void printShaderLog(GLint shaderId)
{
    GLint logLength;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        char *log=new char[logLength];
        glGetShaderInfoLog(shaderId, logLength, &logLength, log);
        std::cout << std::string(log);
        delete[] log;
    }
}



// Loads files [files] in string table, builds program object, compile shaders in shaders objects, links and return the id program object (with the executable code)
GLuint loadProgram(const std::vector<std::string> &files)
{
    GLuint programId=glCreateProgram(); /// Creates a program object which id is returned

    glBindAttribLocation(programId, 0, "vertexPosition");
    glBindAttribLocation(programId, 1, "vertexNormal");
    glBindAttribLocation(programId, 2, "vertexUvs");
    glBindAttribLocation(programId, 3, "vertexColor");


    GLuint vertexShaderId=glCreateShader(GL_VERTEX_SHADER); /// Creates a vertex shader object which id is returned
    GLuint fragmentShaderId=glCreateShader(GL_FRAGMENT_SHADER); /// Creates a fragment shader object which id is returned
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    GLint n = files.size();
    std::string **strs=new std::string*[n];
    const char** lines=new const char*[n + 1];
    std::cout << "      Loading program " << files[n - 1] << "..." << std::endl;
    
    bool geo = false;
    for (int i=0 ; i<n ; ++i) /// For every file :
    {
        std::string* s=loadFile(files[i]); /// get std::string of file
        strs[i]=s; /// Store it [std::string] in strs[i]
        lines[i+1]=s->c_str(); /// Store it [char] in lines[i+1]
        
        /// If _GEOMETRY_ is in file, geometrey shader : geo=true
        if (strstr(lines[i+1], "_GEOMETRY_") != NULL) /// strstr(a, b)-> finds firt occurence of b in a
            geo=true;
    }

    lines[0] = "#define _VERTEX_\n";
    glShaderSource(vertexShaderId, n + 1, lines, NULL); /// Loads shader source chars in shader object
    glCompileShader(vertexShaderId); /// Compile the loaded shader source code
    printShaderLog(vertexShaderId); /// Prints compilation potential problems

    if (geo) 
    {
        unsigned int geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER_EXT); /// Creates a geometry shader object which id is returned
        glAttachShader(programId, geometryShaderId);
        lines[0] = "#define _GEOMETRY_\n"; // Adds this text before the source text
        glShaderSource(geometryShaderId, n + 1, lines, NULL);
        glCompileShader(geometryShaderId);
        printShaderLog(geometryShaderId);
        glProgramParameteriEXT(programId, GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES); /// Specifies type of primitives accessible in geometry shader
    }

    lines[0] = "#define _FRAGMENT_\n";
    glShaderSource(fragmentShaderId, n + 1, lines, NULL);
    glCompileShader(fragmentShaderId);
    printShaderLog(fragmentShaderId);

    glLinkProgram(programId); /// Links the program object to build the executable code

    for (int i = 0; i < n; ++i) 
    {
        delete strs[i];
    }
    delete[] strs;
    delete[] lines;

    return programId; /// Returns the id of the program object
}


// Passing the matrices to the shader
void setMatricesInShader(GLuint shaderID, GLfloat * model, GLfloat * view, GLfloat * c, GLfloat * projection)
{
    GLenum toTranspose=GL_FALSE;
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, toTranspose, model);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, toTranspose, view);
    glUniform4fv(glGetUniformLocation(shaderID, "eye"), 1, c);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, toTranspose, projection);
}

// Passing the light to the shader
void setLightInShader(GLuint shaderID, GLfloat * position, GLfloat power)
{
    // Passing the light to the shader
    glUniform4fv(glGetUniformLocation(shaderID, "light.position"), 1, position);
    glUniform1f(glGetUniformLocation(shaderID, "light.power"), power);
}

// Passing the material to the shader
void setMaterialInShader(GLuint shaderID, GLfloat * ambient, GLfloat * diffuse, GLfloat * specular, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat shininess)
{
    glUniform4fv(glGetUniformLocation(shaderID, "material.ambient"), 1, ambient);
    glUniform4fv(glGetUniformLocation(shaderID, "material.diffuse"), 1, diffuse);
    glUniform4fv(glGetUniformLocation(shaderID, "material.specular"), 1, specular);
    glUniform1f(glGetUniformLocation(shaderID, "material.ka"), ka);
    glUniform1f(glGetUniformLocation(shaderID, "material.kd"), kd);
    glUniform1f(glGetUniformLocation(shaderID, "material.ks"), ks);
    glUniform1f(glGetUniformLocation(shaderID, "material.shininess"), shininess);
}

// Change color in the material in the shader (diffuse component)
void changeMaterialColorInShader(GLuint shaderID, GLfloat * color)
{
    glUniform4fv(glGetUniformLocation(shaderID, "material.diffuse"), 1, color);
}

// Set filledData in the shader (flag saying with data is provided as attribute)
void setFilledDataInShader(GLuint shaderID, GLboolean positions, GLboolean normals, GLboolean uvs, GLboolean colors)
{
    glUniform4i(glGetUniformLocation(shaderID, "filledData"), positions, normals, uvs, colors);
}

// Sets the units of the textures to use for diffuse and specular as a uniform samplers in shader
void setTextureUnitsInShader(GLuint shaderID)
{
    glUniform1i(glGetUniformLocation(shaderID, "textureUnitDiffuse"), 0);
    glUniform1i(glGetUniformLocation(shaderID, "textureUnitSpecular"), 1);
}

void changeColor(GLfloat *color) {
	if(color[0]==0.0 && color[1]==0.0 && color[2]==0.0) {
		color[0]=1.0;
	}
	else if(color[0]==1.0) {
		color[0]=0.0;
		color[1]=1.0;
		color[2]=0.0;
	}
	else if(color[1]==1.0) {
		color[0]=0.0;
		color[1]=0.0;
		color[2]=1.0;
	}
	else  {
		color[0]=0.0;
		color[1]=0.0;
		color[2]=0.0;
	}
}


//______________________________________________________________________________
// PPM file reading fuction

// Loads an image encoded in ppm format (P6)
unsigned char * loadPPM(const std::string & filename, GLuint * width, GLuint * height)
{	
    unsigned char * data;
    // Reads header
	std::ifstream file(filename.c_str());
	std::string line;
	
	// Formats
	std::getline(file,line);
	
	// Skips comments
	std::getline(file,line);
	while(line[0]=='#')
		std::getline(file,line);

	// Reads dimensions
	std::istringstream ist(line);
    GLuint w, h;
	ist >> w >> h;
    *width=w;
    *height=h;

    // Reads the data
	std::getline(file,line);

    /* May be necessary on windows	   
    // 	unsigned int dataStart = file.tellg();	
    // 	file.close();
    // 	file.open(filename.c_str(),std::ios::in | std::ios::binary); 
    // 	file.seekg(dataStart);
	*/

	data = new unsigned char[w*h*3];
	file.read((char*) data, w*h*sizeof(unsigned char)*3);

	// Closes the file
	file.close();

    return data;
}


// needs -lrt (real-time lib)
// 1970-01-01 epoch UTC time, 1 microsecond resolution (divide by 1M to get time_t)
uint64_t getTime()
{
    timespec ts;
    
    // apple version was not tested and windows version is to be done
    #ifdef __APPLE__
        clock_serv_t cclock;
        mach_timespec_t mts;
        host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
        clock_get_time(cclock, &mts);
        mach_port_deallocate(mach_task_self(), cclock);
        ts.tv_sec = mts.tv_sec;
        ts.tv_nsec = mts.tv_nsec;
    #else
        clock_gettime(CLOCK_REALTIME, &ts);
    #endif

    return ((uint64_t)ts.tv_sec * 1000000LL) + ((uint64_t)ts.tv_nsec / 1000LL);
}

// Returns true if intersection between ray (from position pos with direction dir)
// and triangle ABC, if true : intersection point in result.
bool intersectRayTriangle(GLfloat * pos, GLfloat * dir, GLfloat * normal, GLfloat * A, GLfloat * B, GLfloat * C,
GLfloat * result)
{
    GLfloat t=(dotProduct(normal, A) - dotProduct(normal, pos))/dotProduct(normal, dir);
    if (t<0.0) return false;

    GLfloat pa[4]; GLfloat pb[4]; GLfloat pc[4];
    for (GLuint iCoord=0 ; iCoord<4 ; iCoord++)
    {
        pa[iCoord]=A[iCoord]-pos[iCoord];
        pb[iCoord]=B[iCoord]-pos[iCoord];
        pc[iCoord]=C[iCoord]-pos[iCoord];
    }

    // Test intersection against triangle ABC
    GLfloat u=scalarTriple(dir, pc, pb);
    if (u<0.0) return false;

    GLfloat v=scalarTriple(dir, pa, pc);
    if (v<0.0) return false;

    GLfloat w=scalarTriple(dir, pb, pa);
    if (w<0.0) return false;

    // Compute r, r=u*a+v*b+w*c, from barycentric coordinates (u, v, w)
    GLfloat denom=1.0/(u+v+w);
    u*=denom;
    v*=denom;
    w*=denom; // w=1.0f-u-v;
    for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
        result[iCoord]=u*A[iCoord]+v*B[iCoord]+w*C[iCoord];

    return true;
}

//======================== X-tests ========================
bool axisTestX01(GLfloat a, GLfloat b, GLfloat fa, GLfloat fb, GLfloat * boxHalfSize, GLfloat * v0, GLfloat * v1, GLfloat * v2)
{
	GLfloat p0=a*v0[1]-b*v0[2];
	GLfloat p2=a*v2[1]-b*v2[2];	
	
	GLfloat min, max;
    if (p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;}
    
	GLfloat rad=fa*boxHalfSize[1]+fb*boxHalfSize[2];
	if (min>rad || max<-rad) return false;
	return true;
}
bool axisTestX2(GLfloat a, GLfloat b, GLfloat fa, GLfloat fb, GLfloat * boxHalfSize, GLfloat * v0, GLfloat * v1, GLfloat * v2)
{
	GLfloat p0=a*v0[1]-b*v0[2];
	GLfloat p1=a*v1[1]-b*v1[2];	
	
	GLfloat min, max;
    if (p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;}
    
	GLfloat rad=fa*boxHalfSize[1]+fb*boxHalfSize[2];
	if (min>rad || max<-rad) return false;
	return true;
}

//======================== Y-tests ========================
bool axisTestY02(GLfloat a, GLfloat b, GLfloat fa, GLfloat fb, GLfloat * boxHalfSize, GLfloat * v0, GLfloat * v1, GLfloat * v2)
{
	GLfloat p0=-a*v0[0]+b*v0[2];
	GLfloat p2=-a*v2[0]+b*v2[2];	
	
	GLfloat min, max;
    if (p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;}
    
	GLfloat rad=fa*boxHalfSize[0]+fb*boxHalfSize[2];
	if (min>rad || max<-rad) return false;
	return true;
}
bool axisTestY1(GLfloat a, GLfloat b, GLfloat fa, GLfloat fb, GLfloat * boxHalfSize, GLfloat * v0, GLfloat * v1, GLfloat * v2)
{
	GLfloat p0=-a*v0[0]+b*v0[2];
	GLfloat p1=-a*v1[0]+b*v1[2];	
	
	GLfloat min, max;
    if (p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;}
    
	GLfloat rad=fa*boxHalfSize[0]+fb*boxHalfSize[2];
	if (min>rad || max<-rad) return false;
	return true;
}

//======================== Z-tests ========================
bool axisTestZ12(GLfloat a, GLfloat b, GLfloat fa, GLfloat fb, GLfloat * boxHalfSize, GLfloat * v0, GLfloat * v1, GLfloat * v2)
{
	GLfloat p1=a*v1[0]-b*v1[1];
	GLfloat p2=a*v2[0]-b*v2[1];
		
	GLfloat min, max;
    if (p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;}
    
	GLfloat rad=fa*boxHalfSize[0]+fb*boxHalfSize[1];
	if (min>rad || max<-rad) return false;
	return true;
}
bool axisTestZ0(GLfloat a, GLfloat b, GLfloat fa, GLfloat fb, GLfloat * boxHalfSize, GLfloat * v0, GLfloat * v1, GLfloat * v2)
{
	GLfloat p0=a*v0[0]-b*v0[1];
	GLfloat p1=a*v1[0]-b*v1[1];
		
	GLfloat min, max;
    if (p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;}
    
	GLfloat rad=fa*boxHalfSize[0]+fb*boxHalfSize[1];
	if (min>rad || max<-rad) return false;
	return true;
}


void minMax(GLfloat x0, GLfloat x1, GLfloat x2, GLfloat * min, GLfloat * max)
{
    (*min)=x0; if (x1<(*min)) (*min)=x1; if (x2<(*min)) (*min)=x2;
    (*max)=x0; if (x1>(*max)) (*max)=x1; if (x2>(*max)) (*max)=x2;
}


bool planeBoxOverlap(GLfloat * normal, GLfloat * vert, GLfloat * maxbox)
{
    float vmin[3], vmax[3], v;
    for(GLuint iCoord=0 ; iCoord<3 ; iCoord++)
    {
        v=vert[iCoord];
        if(normal[iCoord]>0.0)
        {
            vmin[iCoord]=-maxbox[iCoord]-v;
            vmax[iCoord]= maxbox[iCoord]-v;
        }
        else
        {
            vmin[iCoord]= maxbox[iCoord]-v;
            vmax[iCoord]=-maxbox[iCoord]-v;
        }
    }
    if (dotProduct(normal, vmin)>0.0) return false;
    if (dotProduct(normal, vmax)>=0.0) return true;
    return false;
}


bool intersectAABBTriangle(GLfloat * boxHalfSize, GLfloat * normal, GLfloat *
A, GLfloat * B, GLfloat * C)
{
    GLfloat fex, fey, fez;
    GLfloat e0[3], e1[3], e2[3];
    for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
    {
        // Computes triangle edges
        e0[iCoord]=B[iCoord]-A[iCoord]; // tri edge 0
        e1[iCoord]=C[iCoord]-B[iCoord]; // tri edge 1
        e2[iCoord]=A[iCoord]-C[iCoord]; // tri edge 2
    }

    // 3)
    //  Tests the 9 tests first (this was faster)
    fex=fabsf(e0[0]); fey=fabsf(e0[1]); fez=fabsf(e0[2]);
    if (!axisTestX01(e0[2], e0[1], fez, fey, boxHalfSize, A, B, C)) return false;
    if (!axisTestY02(e0[2], e0[0], fez, fex, boxHalfSize, A, B, C)) return false;
    if (!axisTestZ12(e0[1], e0[0], fey, fex, boxHalfSize, A, B, C)) return false;

    fex=fabsf(e1[0]); fey=fabsf(e1[1]); fez=fabsf(e1[2]);
    if (!axisTestX01(e1[2], e1[1], fez, fey, boxHalfSize, A, B, C)) return false;
    if (!axisTestY02(e1[2], e1[0], fez, fex, boxHalfSize, A, B, C)) return false;
    if (!axisTestZ0 (e1[1], e1[0], fey, fex, boxHalfSize, A, B, C)) return false;

    fex=fabsf(e2[0]); fey=fabsf(e2[1]); fez=fabsf(e2[2]);
    if (!axisTestX2 (e2[2], e2[1], fez, fey, boxHalfSize, A, B, C)) return false;
    if (!axisTestY1 (e2[2], e2[0], fez, fex, boxHalfSize, A, B, C)) return false;
    if (!axisTestZ12(e2[1], e2[0], fey, fex, boxHalfSize, A, B, C)) return false;


    // 1)
    // First tests overlap in the {x,y,z}-directions
    // Finds min, max of the triangle each direction, and test for overlap in
    // That direction -- this is equivalent to testing a minimal AABB around
    // the triangle against the AABB
    GLfloat min, max;
    // Tests in the 3 directions
    for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
    {
        minMax(A[iCoord], B[iCoord], C[iCoord], &min, &max);
        if (min>boxHalfSize[iCoord] || max<-boxHalfSize[iCoord]) return false;
    }

    // 2)
    // Tests if the box intersects the plane of the triangle
    // Computes plane equation of triangle: normal*x+d=0
    if (!planeBoxOverlap(normal, A, boxHalfSize)) return false;

    return true;
}

