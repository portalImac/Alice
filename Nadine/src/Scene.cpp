// Scene.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#include "Scene.hpp"
#include "Tools.hpp"
#include "Camera.hpp"
#include "Object.hpp"

#include <iostream>


// Default constructor
Scene::Scene()
{
    this->init();
}

// Cleans memory for Scene
Scene::~Scene()
{
    for (GLuint iStoredObjects=0 ; iStoredObjects<this->nbStoredObjects ; iStoredObjects++)
        delete this->storedObjects[iStoredObjects];

    delete this->camera;
    delete [] this->storedObjects;
    delete [] this->drawnObjects;
    delete [] this->drawnObjectsColors;
    delete [] this->drawnObjectsModels;
    delete [] this->drawnObjectsShaderIDs;
    delete [] this->drawnObjectsTexture0IDs;
    delete [] this->drawnObjectsTexture1IDs;
}


// Initializes all the pointers to NULL
// and sets the numbers of objects to 0
void Scene::init()
{
    // Fixed max sizes for arrays
    this->maxStoredObjects=50;
    this->maxDrawnObjects=200;
    
	// Initialisation of the numbers of objets filled
	this->nbStoredObjects=0;
    this->nbDrawnObjects=0;
    
    // Array creation (with fixed size)
    this->storedObjects=new Object *[this->maxStoredObjects];
    this->drawnObjects=new GLuint[this->maxDrawnObjects];
    this->drawnObjectsColors=new GLfloat[this->maxDrawnObjects*4];
    this->drawnObjectsModels=new GLfloat[this->maxDrawnObjects*16];
    this->drawnObjectsShaderIDs=new GLuint[this->maxDrawnObjects];

    this->drawnObjectsTexture0IDs=new GLuint[this->maxDrawnObjects];
    this->drawnObjectsTexture1IDs=new GLuint[this->maxDrawnObjects];
    // Default texture ID
    setDefaultTextureID(1);
           
    // Default color
    GLfloat grey[16]={0.2, 0.2, 0.2, 1.0}; setDefaultColor(grey);
    // Default model
    GLfloat I[16]; setToIdentity(I); setDefaultModel(I);
    // Default shader ID
    setDefaultShaderID(1);
    
     // Initalisation of storedObjects
	for (GLuint iStoredObjects=0 ; iStoredObjects<this->maxStoredObjects ; iStoredObjects++)
		this->storedObjects[iStoredObjects]=NULL;
		
    // Light creation
    GLfloat lightPosition[]={0.0, 5.0, 0.0, 1.0}; GLfloat lightPower=1.0;
    this->setLight(lightPosition, lightPower);

    // Creation and initialisation of a camera
    this->camera=new Camera();
}


// Adds the object in the Objects library array, 
// after the last added object, and only if the array is not full, returns the index
GLuint Scene::storeObject(Object * object)
{
	if (this->nbStoredObjects<this->maxStoredObjects)
	{
		this->storedObjects[this->nbStoredObjects]=object;
		std::cout<<"       Object stored (index: "<<this->nbStoredObjects<<")."<<std::endl;
		
		this->nbStoredObjects++;
	}
	else
	{
		std::cout<<"       There is already "<<this->maxStoredObjects<<" objects stored."<<std::endl;
	}
	return (this->nbStoredObjects-1);
}

// Sets the id of the texture to use on the drawn object of index indexDrawObject
void Scene::setDrawnObjectTextureID(GLuint indexDrawnObject, GLuint textureUnit, GLuint textureID)
{
    if (textureUnit==0) this->drawnObjectsTexture0IDs[indexDrawnObject]=textureID;
    if (textureUnit==1) this->drawnObjectsTexture1IDs[indexDrawnObject]=textureID;
}


// Adds the index of stored object to draw in the drawnObjects array, 
// after the last added, and only if the array is not full
GLuint  Scene::addObjectToDraw(GLuint indexStoredObject)
{
    if (this->nbDrawnObjects<this->maxDrawnObjects)
	{
		this->drawnObjects[this->nbDrawnObjects]=indexStoredObject;
        setDrawnObjectColor(this->nbDrawnObjects, this->defaultColor);
        setDrawnObjectModel(this->nbDrawnObjects, this->defaultModel);
        setDrawnObjectShaderID(this->nbDrawnObjects, this->defaultShaderID);
        setDrawnObjectTextureID(this->nbDrawnObjects, 0, this->defaultTextureID);
        setDrawnObjectTextureID(this->nbDrawnObjects, 1, this->defaultTextureID);

		//std::cout<<"       Object "<<this->nbDrawnObjects<<" to draw."<<std::endl;
		this->nbDrawnObjects++;
	}
	else
	{
		std::cout<<"       There is already "<<this->maxDrawnObjects<<" objects drawn."<<std::endl;
	}
    return (this->nbDrawnObjects-1);
}


// Sets the color for the drawn object of index indexDrawObject
void Scene::setDrawnObjectColor(GLuint indexDrawnObject, GLfloat * color)
{
    for (int iColorComp=0 ; iColorComp<4 ; iColorComp++)
        this->drawnObjectsColors[(indexDrawnObject*4)+iColorComp]=color[iColorComp];
}


// Sets the transformation matrix for the drawn object of index indexDrawObject
void Scene::setDrawnObjectModel(GLuint indexDrawnObject, GLfloat * model)
{
    for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
        this->drawnObjectsModels[(indexDrawnObject*16)+iMatrixCoord]=model[iMatrixCoord];
}


// Sets the if of the shader to use on the drawn object of index indexDrawObject
void Scene::setDrawnObjectShaderID(GLuint indexDrawnObject, GLuint shaderID)
{
    this->drawnObjectsShaderIDs[indexDrawnObject]=shaderID;
}


// Sets the light
void Scene::setDefaultColor(GLfloat * defaultColor)
{
    for (int iCoord=0 ; iCoord<4 ; iCoord++)
        this->defaultColor[iCoord]=defaultColor[iCoord];

	for (GLuint iDrawnObjects=0 ; iDrawnObjects<this->maxStoredObjects ; iDrawnObjects++)
		setDrawnObjectColor(iDrawnObjects, this->defaultColor);
}


// Sets default transformation matrix
void Scene::setDefaultModel(GLfloat * defaultModel)
{
    for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
        this->defaultModel[iMatrixCoord]=defaultModel[iMatrixCoord];

	for (GLuint iDrawnObjects=0 ; iDrawnObjects<this->maxStoredObjects ; iDrawnObjects++)
	    setDrawnObjectModel(iDrawnObjects, this->defaultModel);
}


// Sets default shader ID
void Scene::setDefaultShaderID(GLuint defaultShaderID)
{
    this->defaultShaderID=defaultShaderID;
}

 
// Sets light data to use in shader
void Scene::setLight(GLfloat * position, GLfloat power)
{
    for (GLuint iCoord=0 ; iCoord<4 ; iCoord++)
        this->lightPosition[iCoord]=position[iCoord];
    this->lightPower=power;
}


// Decides what will elements drawn after this call will look like
void Scene::setAppearance(GLuint indexDrawnObject)
{
    glUseProgram(this->drawnObjectsShaderIDs[indexDrawnObject]); // From now on, this shader will be used.

    // We use the specific values of model per object
    GLfloat * model=&(this->drawnObjectsModels[indexDrawnObject*16]);
    setMatricesInShader(this->drawnObjectsShaderIDs[indexDrawnObject], model, this->camera->view, this->camera->c, this->camera->projection);

    // We use the specific values of color per object
    GLfloat * color=&(this->drawnObjectsColors[indexDrawnObject*4]);
    changeMaterialColorInShader(this->drawnObjectsShaderIDs[indexDrawnObject], color);
    //glUniform4fv(glGetUniformLocation(this->drawnObjectsShaderIDs[indexDrawnObject], "color"), 1, color);

    // Specifies which VBO were filled
    Object * object=this->storedObjects[this->drawnObjects[indexDrawnObject]];
    setFilledDataInShader(this->drawnObjectsShaderIDs[indexDrawnObject], object->primitives, object->normals, object->uvs, object->colors);
    
    // Sets the light in the current shader
    setLightInShader(this->drawnObjectsShaderIDs[indexDrawnObject], this->lightPosition, this->lightPower);

    // Selects our current texture for unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->drawnObjectsTexture0IDs[indexDrawnObject]);

    // Selects our current texture for unit 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->drawnObjectsTexture1IDs[indexDrawnObject]);
}

// Sets default texture ID
void Scene::setDefaultTextureID(GLuint defaultTextureID)
{
    this->defaultTextureID=defaultTextureID;
}


// Draw all Objects
void Scene::drawObjectsOfScene()
{
    
    GLuint indexStoredObjects=0;
    for (GLuint iDrawnObjects=0 ; iDrawnObjects<this->nbDrawnObjects; iDrawnObjects++)
    {
        indexStoredObjects=this->drawnObjects[iDrawnObjects];
	    if ((indexStoredObjects<this->maxStoredObjects) && (this->storedObjects[indexStoredObjects]!=NULL))
	    {
	        setAppearance(iDrawnObjects);
		GLfloat vDecalage[3] = {0.5,0.5,0}; 
    	    this->storedObjects[indexStoredObjects]->drawObject();
        }
    }
}

