// Scene.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "GLHeaders.hpp"
#include "Camera.hpp"
#include "Color.hpp"

#include <vector>

namespace stein {

// Forward declaration
struct Object;

// A container for objects
class Scene {
    struct ObjectInstance {
        size_t objectId;
        size_t shaderId;
        Matrix4f transformation;
        Color color;

        ObjectInstance(size_t objectId, size_t shaderId, const Matrix4f& transformation, const Color&color) :
            objectId(objectId), shaderId(shaderId), transformation(transformation), color(color) {
        }
    };

    std::vector<Object*> storedObjects; // Library of Objects to use from GPU
    std::vector<ObjectInstance> drawnObjects;

    Color defaultColor; // Default color for drawn elements
    Matrix4f defaultTransformation; // Default transformation matrix for drawn elements
    GLuint defaultShaderID; // Default shaderID for drawn elements
    GLfloat lightPosition[4]; // Position of the light used in shader
    GLfloat lightPower; // Power of the light used in shader
public:
    const static size_t maxStoredObjects = 50; // An initial limit of storable objects
    const static size_t maxDrawnObjects = 200; // An initial limit of drawable objects

    Scene();
    ~Scene();

    Camera camera; // Camera used to watch the scene

    Object& createObject(GLenum primitiveType);
    GLuint addObjectToDraw(GLuint indexStoredObject);

    void setDrawnObjectColor(GLuint indexDrawnObject, const Color &color);
    void setDrawnObjectModel(GLuint indexDrawnObject, const Matrix4f &model);
    void setDrawnObjectShaderID(GLuint indexDrawnObject, GLuint shaderID);

    void setDefaultColor(const Color &defaultColor);
    void setDefaultModel(const Matrix4f &defaultModel);
    void setDefaultShaderID(GLuint defaultShaderID);

    void setLight(GLfloat * position, GLfloat power);

    void drawObjectsOfScene();
private:
    void setAppearance(const ObjectInstance &);
};

} // namespace stein

#endif // __SCENE_HPP__
