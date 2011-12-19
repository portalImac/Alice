// Scene.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "GLHeaders.hpp"

// Forward declaration
struct Camera;
struct Object;

// A container for objects
class Scene
{
    public:
        GLuint maxStoredObjects;         // An initial limit of storable objects
        GLuint maxDrawnObjects;          // An initial limit of drawable objects
        
        Object ** storedObjects;         // Library of Objects to use from GPU
        GLuint nbStoredObjects;          // Number of stored objects
        
        GLuint * drawnObjects;           // Indices of objects from library to really draw
        GLfloat * drawnObjectsColors;    // Color for each drawn object
        GLfloat * drawnObjectsModels;    // Transformation matrix for each drawn object
        GLuint * drawnObjectsShaderIDs;  // ID of the shader to use for each drawn object  
        GLuint nbDrawnObjects;

        GLfloat defaultColor[4];         // Default color for drawn elements
        GLfloat defaultModel[16];        // Default transformation matrix for drawn elements
        GLuint defaultShaderID;          // Default shaderID for drawn elements        
        
        GLfloat lightPosition[4];        // Position of the light used in shader
        GLfloat lightPower;              // Power of the light used in shader
        
        Camera * camera;                 // Camera used to watch the scene
	Camera * cameraEntry;            // Camera used to watch the entry of a portal
	Camera * cameraExit;             // Camera used to watch the exit of a portal

        GLuint defaultTextureID;         // Default textureID for drawn elements
        GLuint * drawnObjectsTexture0IDs; // ID of the texture to use on each drawn object for unit 0
        GLuint * drawnObjectsTexture1IDs; // ID of the texture to use on each drawn object for unit 1

        Scene();
        ~Scene();

        void init();

        GLuint storeObject(Object * object);
        void setDrawnObjectTextureID(GLuint indexDrawnObject, GLuint textureUnit, GLuint textureID);
        GLuint addObjectToDraw(GLuint indexStoredObject);

        void setDrawnObjectColor(GLuint indexDrawnObject, GLfloat * color);
        void setDrawnObjectModel(GLuint indexDrawnObject, GLfloat * model);
        void setDrawnObjectShaderID(GLuint indexDrawnObject, GLuint shaderID);   
        
        void setDefaultColor(GLfloat * defaultColor);
        void setDefaultModel(GLfloat * defaultModel);
        void setDefaultShaderID(GLuint defaultShaderID);       
        
        void setLight(GLfloat * position, GLfloat power);

        void setAppearance(GLuint indexDrawnObject);

        void setDefaultTextureID(GLuint defaultTextureID);

        void drawObjectsOfScene();
};

#endif // __SCENE_HPP__
