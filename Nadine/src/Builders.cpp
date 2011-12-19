// Builders.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#include "Builders.hpp"
#include "Object.hpp"
#include "Tools.hpp"

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstdlib>


//______________________________________________________________________________
// Building functions


// Builds one triangle
void buildTriangle(Object * object, GLfloat * colors)
{
    object->nbVertices=3;
    object->nbIndices=3;

    // The 3 vertices of a triangle
	GLfloat vertices[]={-0.5, 0.0, 0.0, 1.0, 
	                     0.5, 0.0, 0.0, 1.0,
	                     0.0, 0.5, 0.0, 1.0};

    // The 3 indices of the vertices to draw the face
    GLuint indices[]={0, 1, 2};
    
    // Sends the data into buffers on the GPU
    object->sendPrimitives(vertices, indices);

    object->sendColors(colors);
}


// Builds one square
void buildSquare(Object * object, GLfloat * vect)
{
    object->nbVertices=4;
    object->nbIndices=6;

    // The 3 vertices of a triangle
    GLfloat l=0.3;
	GLfloat vertices[]={-l+vect[0], -l+vect[1], 0.0, 1.0, // bottom left
	                     l+vect[0], -l+vect[1], 0.0, 1.0, // bottom right
	                    -l+vect[0],  l+vect[1], 0.0, 1.0, // top left
	                     l+vect[0],  l+vect[1], 0.0, 1.0}; // top right


    // The 3 indices of the vertices to draw the face
    GLuint indices[]={0, 2, 3, // First triangle : top left
    		      0, 1, 3}; // Second triangle : bottom right

    // Sends the data into buffers on the GPU
    object->sendPrimitives(vertices, indices);
}


// Builds one lozenge
void buildLozenge(Object * object, GLfloat * vect)
{
    object->nbVertices=4;
    object->nbIndices=6;

    // The 3 vertices of a triangle
    GLfloat l=0.2;
	GLfloat vertices[]={-l+vect[0], 0+vect[1], 0.0, 1.0, // left
	                     l+vect[0], 0+vect[1], 0.0, 1.0, // right
	                     0+vect[0], l+vect[1], 0.0, 1.0, // top
	                     0+vect[0], -l+vect[1], 0.0, 1.0}; //bottom


    // The 3 indices of the vertices to draw the face
    GLuint indices[]={0, 1, 2, // First triangle : top
    		      0, 1, 3}; // Second triangle : bottom

    // Sends the data into buffers on the GPU
    object->sendPrimitives(vertices, indices);
}

// Builds one 3D square 
void build3DSquare(Object * object)
{
    object->nbVertices=12;
    object->nbIndices=12;

    // The 12 vertices of the square

	GLfloat vertices[]={ 
			 0.0,0.0,0.0,1.0, // E pour face ECD
			 0.5,0.5,-0.1,1.0, //C pour la face ECD
			-0.5,0.5,-0.1,1.0, // D pour la face ECD
			 0.0,0.0,0.0,1.0, // E pour face DEA
			-0.5,0.5,-0.1,1.0, // D pour la face DEA
			 -0.5,-0.5,-0.1,1.0, // A pour la face DEA
			0.0,0.0,0.0,1.0, // E pour face AEB
			 -0.5,-0.5,-0.1,1.0, // A pour la face AEB
			 0.5,-0.5,-0.1,1.0, // B pour la face AEB
			0.0,0.0,0.0,1.0, // E pour face BEC
			 0.5,-0.5,-0.1,1.0, // B pour la face BEC
	                 0.5,0.5,-0.1,1.0 // C pour la face BEC
	                 };

    GLfloat normals[36];
    // The indices of the vertices to draw the face
    GLuint indices[]={0, 1, 2, // First triangle : top
    		      3, 4, 5, // Second triangle : left
		      6, 7, 8, // Third triangle : bottom
		      9, 10, 11}; // Fourth triangle : right

    setNormalsFlat(object->nbIndices, vertices, indices, normals);


    // Sends the data into buffers on the GPU
    object->sendPrimitives(vertices, indices);

   // Envoie les primitives
   object->sendNormals(normals);
}

void buildAxis(Object * object) {
    object->nbVertices=4;
    object->nbIndices=6;
    GLfloat vertices[]={ 0.0,0.0,0.0,1.0,
			 1.0,0.0,0.0,1.0,
			 0.0,1.0,0.0,1.0,
			 0.0,0.0,1.0,1.0
	                 };

    GLuint indices[]={0, 1,
    		      0, 2,
		      0, 3 };

    GLfloat colorsRepere[16] = {1.0,1.0,1.0,1.0,
				1.0,0.0,0.0,1.0,
				0.0,1.0,0.0,1.0,
				0.0,0.0,1.0,1.0}; 

    // Sends the data into buffers on the GPU
    object->sendPrimitives(vertices, indices);
    object->sendColors(colorsRepere);
}

void buildCircle(Object * object, GLfloat radius, GLuint discAngle) {
    object->nbVertices=discAngle;
    object->nbIndices=discAngle*2;

    GLuint i;
    GLfloat deltaAngle= 2 * M_PI / (GLfloat)discAngle;
    GLfloat vertices[4*object->nbVertices];
    GLuint indices[object->nbIndices];

    for(i=0; i<object->nbVertices; i++) {
	GLint indexVertices=i*4;

	vertices[indexVertices+0]=radius*cos(i*deltaAngle);
	vertices[indexVertices+1]=radius*sin(i*deltaAngle);
	vertices[indexVertices+2]=0.0;
	vertices[indexVertices+3]=1.0;

	GLint indexIndices=i*2;
	indices[indexIndices+0]=i;
	indices[indexIndices+1]=i+1;

	if(i==(discAngle-1))
	{ indices[indexIndices+1]=0;}
    }
   
    object->sendPrimitives(vertices, indices);

}

void buildDisc(Object* object, GLfloat radius, GLuint discAngle) {
    object->nbVertices=discAngle+1;
    object->nbIndices=3*discAngle;

    GLfloat deltaAngle= 2 * M_PI / (GLfloat)discAngle;
    GLfloat vertices[4*object->nbVertices];
    GLuint indices[object->nbIndices];
    GLuint i;

    GLint indexVertices = discAngle*4;
    vertices[indexVertices+0]=0.0;
    vertices[indexVertices+1]=0.0;
    vertices[indexVertices+2]=0.0;
    vertices[indexVertices+3]=1.0;

    for(i=0; i<discAngle; i++) {
	
	indexVertices=i*4;
	vertices[indexVertices+0]=radius*cos(i*deltaAngle);
	vertices[indexVertices+1]=radius*sin(i*deltaAngle);
	vertices[indexVertices+2]=0.0;
	vertices[indexVertices+3]=1.0;

	GLint indexIndices = i*3;
	indices[indexIndices+0]=discAngle;
	indices[indexIndices+1]=i;
	indices[indexIndices+2]=i+1;

	if(i==(discAngle-1)) {
		indices[indexIndices+2]=0;	
	}
    }

    object->sendPrimitives(vertices, indices);
}

void buildCylinder(Object* object, GLfloat radius, GLfloat height, GLuint discAngle, GLint discHeight, bool closed) {
    object->nbVertices=discAngle*(discHeight+1);
    object->nbIndices=discAngle*discHeight*6;

    GLuint i;
    GLfloat j;
    GLfloat deltaAngle= 2 * M_PI / (GLfloat)discAngle;
    GLfloat deltaHeight = height/discHeight;
    GLfloat vertices[4*object->nbVertices];
    GLuint indices[object->nbIndices];

    for(j=0; j<=discHeight; j++) {
		
		for(i=0; i<discAngle; i++) {
			GLint indexVertices=j*discAngle*4 + i*4;

			vertices[indexVertices+0]=radius*cos(i*deltaAngle);
			vertices[indexVertices+1]=radius*sin(i*deltaAngle);
			vertices[indexVertices+2]=-j*deltaHeight;
			vertices[indexVertices+3]=1.0;

			
			if (j<discHeight) {
				GLint indexIndices = j*discAngle*6 + i*6;
				indices[indexIndices+0]=j*discAngle+i;
				indices[indexIndices+1]=j*discAngle+i+1;
				indices[indexIndices+2]=(j+1)*discAngle+i+1;
				indices[indexIndices+3]=j*discAngle+i;
				indices[indexIndices+4]=(j+1)*discAngle+i+1;
				indices[indexIndices+5]=(j+1)*discAngle+i;

			if (i==(discAngle-1)) {
				indices[indexIndices+1]=j*discAngle;
				indices[indexIndices+2]=j*discAngle+i+1;
				indices[indexIndices+4]=j*discAngle+i+1;
			}
			}
	    	}
    }

	GLfloat normals[3*object->nbVertices];

	setNormalsFlat(object->nbIndices, vertices, indices, normals);
   
    object->sendPrimitives(vertices, indices);
	object->sendNormals(normals);

}


void buildCone(Object* object, GLfloat radius, GLfloat height, GLuint discAngle, GLint discHeight, bool closed) {

    object->nbVertices=discAngle*(discHeight+1);
    object->nbIndices=discAngle*discHeight*6;

    GLuint i;
    GLfloat j;
    GLfloat deltaAngle= 2 * M_PI / (GLfloat)discAngle;
    GLfloat deltaHeight = height/discHeight;
    GLfloat deltaRadius = radius/discHeight;
    GLfloat vertices[4*object->nbVertices];
    GLuint indices[object->nbIndices];

    for(j=0; j<=discHeight; j++) {
		
		for(i=0; i<discAngle; i++) {
			GLint indexVertices=j*discAngle*4 + i*4;

			vertices[indexVertices+0]=radius*cos(i*deltaAngle);
			vertices[indexVertices+1]=radius*sin(i*deltaAngle);
			vertices[indexVertices+2]=-j*deltaHeight;
			vertices[indexVertices+3]=1.0;

			
			if (j<discHeight) {
				GLint indexIndices = j*discAngle*6 + i*6;
				indices[indexIndices+0]=j*discAngle+i;
				indices[indexIndices+1]=j*discAngle+i+1;
				indices[indexIndices+2]=(j+1)*discAngle+i+1;
				indices[indexIndices+3]=j*discAngle+i;
				indices[indexIndices+4]=(j+1)*discAngle+i+1;
				indices[indexIndices+5]=(j+1)*discAngle+i;

			if (i==(discAngle-1)) {
				indices[indexIndices+1]=j*discAngle;
				indices[indexIndices+2]=j*discAngle+i+1;
				indices[indexIndices+4]=j*discAngle+i+1;
			}
			}
			
			
	    	}

		radius-=deltaRadius;
	
    }

	GLfloat normals[3*object->nbVertices];

	setNormalsFlat(object->nbIndices, vertices, indices, normals);
   
    object->sendPrimitives(vertices, indices);
	object->sendNormals(normals);
}

void buildSphere_TrFlatRed(Object* object, GLfloat radius, GLuint discLat, GLuint discLong) {

    object->nbVertices=discLong*discLat*6;
    object->nbIndices=object->nbVertices;
    
    GLfloat deltaLong=2.0*M_PI/GLfloat(discLong);
    GLfloat deltaLat=M_PI/GLfloat(discLat);
    
    GLfloat localRadius=0.0;
    GLfloat bottom=-1.0;
    GLfloat localHeight=bottom;
    
    GLfloat vertices[object->nbVertices*4];
    GLfloat normals[object->nbVertices*3];
    GLuint indices[object->nbIndices];
    
    // Index of the first vertex of a pair of triangles
    int indexFirst=0;
    for (unsigned int iLat=0 ; iLat<discLat ; iLat++)
    {
        for (unsigned int iLong=0 ; iLong<discLong ; iLong++)
        {
            GLfloat latB=((iLat+0)*deltaLat)-(M_PI/2.0);  // Latitude Bottom
            GLfloat latT=((iLat+1)*deltaLat)-(M_PI/2.0);  // Latitude Top
            GLfloat longL=((iLong+0)*deltaLong);  // Longitude Left
            GLfloat longR=((iLong+1)*deltaLong);  // Longitude Right
            
            localRadius=cos(latB); 
            localHeight=sin(latB);
            GLfloat BL[4]={localRadius*cos(longL), localHeight, -localRadius*sin(longL), 1.0}; // Vertex Bottom Left
            GLfloat BR[4]={localRadius*cos(longR), localHeight, -localRadius*sin(longR), 1.0}; // Vertex Bottom Right
                           
            localRadius=cos(latT);
            localHeight=sin(latT);
            GLfloat TL[4]={localRadius*cos(longL), localHeight, -localRadius*sin(longL), 1.0}; // Vertex Top Left
            GLfloat TR[4]={localRadius*cos(longR), localHeight, -localRadius*sin(longR), 1.0}; // Vertex Top Right
            
            for (int iCoord=0 ; iCoord<4 ; iCoord++)            
            {
                GLfloat coef=radius;
                if (iCoord==3)
                    coef=1.0;
                vertices[(indexFirst+(0*4))+iCoord]=coef*BL[iCoord]; // First triangle
                vertices[(indexFirst+(1*4))+iCoord]=coef*BR[iCoord];
                vertices[(indexFirst+(2*4))+iCoord]=coef*TR[iCoord];
                vertices[(indexFirst+(3*4))+iCoord]=coef*BL[iCoord]; // Second triangle
                vertices[(indexFirst+(4*4))+iCoord]=coef*TR[iCoord];
                vertices[(indexFirst+(5*4))+iCoord]=coef*TL[iCoord];
            }
            indexFirst+=6*4;
        }
    }
    
    for (GLuint iIndices=0 ; iIndices<object->nbIndices ; iIndices++)
        indices[iIndices]=iIndices;
    
    setNormalsFlat(object->nbIndices, vertices, indices, normals);
   
    object->sendPrimitives(vertices, indices);
	object->sendNormals(normals);
}

//______________________________________________________________________________
// Mesh conformation functions



// Centers and normalize any mesh
void centerAndNormalizeMesh(Object * object, GLfloat * vertices)
{
    GLfloat inf=100000.0;
    // Normalizes to 1.0 the size of the mesh
    GLfloat min[]={inf, inf, inf};
    GLfloat max[]={-inf, -inf, -inf};

    for(GLuint iVerticesValues=0 ; iVerticesValues<object->nbVertices ; iVerticesValues++)
    {
        for(GLuint iCoord=0 ; iCoord<3 ; iCoord++)
        {
            GLfloat value=vertices[iVerticesValues*4+iCoord];
            if (value<min[iCoord])
                min[iCoord]=value;
            if (value>max[iCoord])
                max[iCoord]=value;
        }
    }
    GLfloat maxSize[]={max[0]-min[0], max[1]-min[1], max[2]-min[2]};
    GLuint maxCoord=0;
    if ((maxSize[1]>=maxSize[0]) && (maxSize[1]>=maxSize[2])) maxCoord=1;    
    if ((maxSize[2]>=maxSize[0]) && (maxSize[2]>=maxSize[1])) maxCoord=2;

    for(GLuint iVerticesValues=0 ; iVerticesValues<object->nbVertices ; iVerticesValues++)
    {
        for(GLuint iCoord=0 ; iCoord<3 ; iCoord++)
        {
            vertices[iVerticesValues*4+iCoord]-=min[iCoord]+(maxSize[iCoord]/2.0);
            vertices[iVerticesValues*4+iCoord]/=maxSize[maxCoord];
        }
    } 
}






//______________________________________________________________________________
// Obj file reading fuctions


// Fills a std::vector with strings from cuts at every "delim" in "string"
void split(const std::string& string, std::vector<std::string>& tokens, const std::string& delim)
{
        // Clears the vector where the results are put
        tokens.clear();
        // Goes through the string and extract the tokens
        for (std::string::size_type p1=0, p2=0; p1!=std::string::npos; )
        {
                p1=string.find_first_not_of(delim, p1);
                if (p1!=std::string::npos)
                {
                        p2=string.find_first_of(delim, p1);
                        tokens.push_back(string.substr(p1, p2-p1));
                        p1=p2;
                }
        }
}

// Fills a std::vector with three GLfloat values in a string
void readVec3(std::istringstream& line, std::vector<GLfloat> * vertices)
{
    GLfloat vec[3]={0.0, 0.0, 0.0};
    line >> vec[0] >> vec[1] >> vec[2];
    vertices->push_back(vec[0]);
    vertices->push_back(vec[1]);
    vertices->push_back(vec[2]);
}

// Fills a std::vector with two GLfloat values in a string
void readVec2(std::istringstream& line, std::vector<GLfloat> * vertices)
{
    GLfloat vec[2]={0.0, 0.0};
    line >> vec[0] >> vec[1];
    vertices->push_back(vec[0]);
    vertices->push_back(1.0-vec[1]);
}

// Fills std::vectors with indices of three type for the vertices in the face
void readFace(std::istringstream& line, std::vector<GLuint> * indices,  std::vector<GLuint> * uvIndices, std::vector<GLuint> * normalsIndices)
{
    std::string face;
    while (!line.eof())
	{
        line >> face;
            std::vector<std::string> sequence;
            split(face, sequence, "/");
            size_t index=strtoul(sequence[0].c_str(), NULL, 10)-1;
            indices->push_back(index);

            size_t uvIndex=strtoul(sequence[1].c_str(), NULL, 10)-1;
            uvIndices->push_back(uvIndex);

            size_t normalIndex=strtoul(sequence[2].c_str(), NULL, 10)-1;
            normalsIndices->push_back(normalIndex);
    }
}

// Regroup in four arrays instead of six by assigning a uv coordinates and a normal to the corresponding vertex. 
// Does not create new vertices : works only if one normal is sufficient per location in space : smoothed objects
void reorderUvsAndNormalsIfSmooth(std::vector<GLfloat> * vertices, std::vector<GLfloat> * uvs, std::vector<GLfloat> * normals,
                          std::vector<GLuint> * indices, std::vector<GLuint> * uvIndices, std::vector<GLuint> * normalIndices)
{
    std::vector<GLfloat> newUvs(vertices->size(), 0.0);
    std::vector<GLfloat> newNormals(vertices->size(), 0.0);
    GLuint uvIndex;
    GLuint normalIndex;
    for(GLuint iIndices=0 ; iIndices<indices->size() ; iIndices++)
    {
        uvIndex=(*uvIndices)[iIndices];
        normalIndex=(*normalIndices)[iIndices];
        for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
        {
            newNormals[((*indices)[iIndices]*3)+iCoord]=(*normals)[(normalIndex*3)+iCoord];
            if (iCoord<2)
                newUvs[((*indices)[iIndices]*2)+iCoord]=(*uvs)[(uvIndex*2)+iCoord];
        }
    }
    
    uvs->clear();
    normals->clear();
    for(GLuint iVertices=0 ; iVertices<vertices->size() ; iVertices++)
    {
        normals->push_back(newNormals[iVertices]);
    }
    for(GLuint iUvs=0 ; iUvs<newUvs.size() ; iUvs++)
    {      
        uvs->push_back(newUvs[iUvs]);
    }
}

// Regroup in four arrays instead of six by assigning a uv coordinates and a normal to corresponding vertices. 
// Creates new vertices to be able to give several normals per location in space : non smoothed objects
void reorderUvsAndNormalsIfNonSmooth(std::vector<GLfloat> * vertices, std::vector<GLfloat> * uvs, std::vector<GLfloat> * normals,
                                     std::vector<GLuint> * indices, std::vector<GLuint> * uvIndices, std::vector<GLuint> * normalIndices)
{
    std::vector<GLfloat> newVertices;
    std::vector<GLfloat> newUvs;
    std::vector<GLfloat> newNormals;
    GLuint vertexIndex;
    GLuint uvIndex;
    GLuint normalIndex;
    for(GLuint iIndices=0 ; iIndices<indices->size() ; iIndices++)
    {
        vertexIndex=(*indices)[iIndices];
        uvIndex=(*uvIndices)[iIndices];
        normalIndex=(*normalIndices)[iIndices];
        for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
        {
            newVertices.push_back((*vertices)[(vertexIndex*3)+iCoord]);
            newNormals.push_back((*normals)[(normalIndex*3)+iCoord]);
            if (iCoord<2)
                newUvs.push_back((*uvs)[(uvIndex*2)+iCoord]);
        }
        (*indices)[iIndices]=iIndices;
    }
    
    vertices->clear();
    uvs->clear();
    normals->clear();
    for(GLuint iVertices=0 ; iVertices<newVertices.size() ; iVertices++)
    {
        vertices->push_back(newVertices[iVertices]);
        normals->push_back(newNormals[iVertices]);
    }
    for(GLuint iUvs=0 ; iUvs<newUvs.size() ; iUvs++)
    {      
        uvs->push_back(newUvs[iUvs]);
    }
}

// Adds w to every vertices
void addHomogeneousToVertices(std::vector<GLfloat> * vertices)
{
    std::vector<GLfloat> newVertices;
    for(GLuint iVertices=0 ; iVertices<(vertices->size())/3 ; iVertices++)
    {
        for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
            newVertices.push_back((*vertices)[(iVertices*3)+iCoord]);        
        newVertices.push_back(1.0); 
    }
    
    vertices->clear();
    for(GLuint iVertices=0 ; iVertices<newVertices.size() ; iVertices++)
        vertices->push_back(newVertices[iVertices]);   
}


// Changes the array to conform our Object's vbos formats
void conformToObject(std::vector<GLfloat> * vertices, std::vector<GLfloat> * uvs, std::vector<GLfloat> * normals)
{
    addHomogeneousToVertices(vertices);
    // Normalizes normals
    for(GLuint iNormals=0 ; iNormals<(normals->size())/3 ; iNormals++)
    {
        normalize(&((*normals)[iNormals*3]));
    }
}


// Builds an object made from an OBJ file, taking only geometry into account (not materials)
bool buildObjectGeometryFromOBJ(Object * object, const std::string& fileName, bool smoothObject)
{
    std::ifstream file(fileName.c_str(), std::ios_base::in);
    if(!file)
    {
        std::cout<<"       Error while loading object from .obj file : "<<fileName<<"."<<std::endl;
        return false;
    }
    std::cout<<"       Loading object from .obj file : "<<fileName<<"."<<std::endl; 

    bool hasVt=false;
    bool hasVn=false;
	std::vector<GLfloat> vertices;
    std::vector<GLfloat> uvs;
    std::vector<GLfloat> normals;
    std::vector<GLuint> indices;
    std::vector<GLuint> uvIndices;
    std::vector<GLuint> normalIndices;
    std::string buf, key, name, MTLFileName;

    while (getline(file, buf))
	{
            std::istringstream line(buf);
            line >> key;    
            if (key=="o")
      		    line >> name;
            else if(key=="v")
      		     readVec3(line, &vertices);
      		else if(key == "vt")
            {    readVec2(line, &uvs); hasVt=true; }
            else if(key=="vn")
      		{    readVec3(line, &normals); hasVn=true; }
            else if(key=="f")
      		    readFace(line, &indices,  &uvIndices, &normalIndices);
            else if (key=="mltlib")
      		    line >> MTLFileName;
    }

    std::cout<<"       Obj mesh "<<name<<" loading..."<<std::endl;
    std::cout<<"       Obj meshes should only be made of triangles (for this loader), make sure this is correct in file."<<std::endl;
    if (!hasVt) 
        std::cout<<"       Obj file "<<name<<" has no texture coordinates, add some in modeler."<<std::endl;

    if (!hasVn) 
    {
        std::cout<<"       Obj file "<<name<<" has no normals, add some in modeler."<<std::endl;
    }
    else 
    {
        if (normals.size()*3==(indices.size()*3)) 
        {
            std::cout<<"       Obj file "<<name<<" was not smoothed in modeler."<<std::endl;
            if (smoothObject==true)
                std::cout<<"       Normals will be wrong with smoothObject parameter as true, change it to false."<<std::endl;
        }
    }
    bool onenormalPerTriangle=(normals.size()*3==(indices.size()*3));

    if (smoothObject)
        reorderUvsAndNormalsIfSmooth(&vertices, &uvs, &normals, &indices, &uvIndices, &normalIndices);
    else
        reorderUvsAndNormalsIfNonSmooth(&vertices, &uvs, &normals, &indices, &uvIndices, &normalIndices);
    conformToObject(&vertices, &uvs, &normals);

    object->nbVertices=vertices.size()/4;
    object->nbIndices=indices.size();
    
    // Normalizes to 1.0 the size of the mesh and centers it
    centerAndNormalizeMesh(object, vertices.data());


    object->sendPrimitives(vertices.data(), indices.data());
    if (!hasVt) 
        std::cout<<"       WARNING : Obj file "<<name<<" has no texture coordinates, add some in modeler."<<std::endl;
//    else
//        object->sendUvs(uvs.data());

    if (!hasVn) 
    {
        std::cout<<"       WARNING : Obj file "<<name<<" has no normals, add some in modeler."<<std::endl;
    }
    else 
    {
        if (onenormalPerTriangle) 
        {
            std::cout<<"       Obj file "<<name<<" was not smoothed in modeler."<<std::endl;
            if (smoothObject)
                std::cout<<"       WARNING : smoothObject==true. Normals will be wrong : change it to false."<<std::endl;
        }
        object->sendNormals(normals.data());
    }
    std::cout<<"       Material files are not taken into account by this loader."<<std::endl;
    
    return true;
}