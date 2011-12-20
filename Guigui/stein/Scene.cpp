// Scene.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#include "Scene.hpp"
#include "Tools.hpp"
#include "Camera.hpp"
#include "Object.hpp"

#include <stdexcept>
#include <iostream>

namespace stein {

// Default constructor
Scene::Scene(){
    // Light creation
    GLfloat lightPosition[] = { 0.0, 5.0, 0.0, 1.0 };
    GLfloat lightPower = 1.0;
    setLight(lightPosition, lightPower);
           defaultColor = Color::WHITE;
           defaultTransformation = Matrix4f::identity();
           defaultShaderID=1;
}

Scene::~Scene() {
    for (size_t i = 0; i < storedObjects.size(); ++i)
        delete storedObjects[i];
}

// Adds the object in the Objects library array, 
// after the last added object, and only if the array is not full, returns the index
Object& Scene::createObject(GLenum primitiveType) {
    const size_t size = storedObjects.size();
    if (size >= maxStoredObjects)
        throw std::runtime_error("maximum number of stored objects reached");
    storedObjects.push_back(new Object(size, primitiveType));
    return *storedObjects.back();
}

// Adds the index of stored object to draw in the drawnObjects array, 
// after the last added, and only if the array is not full
GLuint Scene::addObjectToDraw(GLuint indexStoredObject) {
    const size_t size = drawnObjects.size();
    if (size >= maxDrawnObjects)
        throw std::runtime_error("maximum number of drawn objects reached");
    drawnObjects.push_back(ObjectInstance(indexStoredObject, defaultShaderID, defaultTransformation, defaultColor));
    return size;
}

// Sets the color for the drawn object of index indexDrawObject
void Scene::setDrawnObjectColor(GLuint indexDrawnObject, const Color &color) {
    assert(indexDrawnObject<drawnObjects.size());
    drawnObjects[indexDrawnObject].color = color;
}

// Sets the transformation matrix for the drawn object of index indexDrawObject
void Scene::setDrawnObjectModel(GLuint indexDrawnObject, const Matrix4f &model) {
    assert(indexDrawnObject<drawnObjects.size());
    drawnObjects[indexDrawnObject].transformation = model;
}

// Sets the if of the shader to use on the drawn object of index indexDrawObject
void Scene::setDrawnObjectShaderID(GLuint indexDrawnObject, GLuint shaderID) {
    assert(indexDrawnObject<drawnObjects.size());
    drawnObjects[indexDrawnObject].shaderId = shaderID;
}

// Sets the light
void Scene::setDefaultColor(const Color &color) {
    defaultColor = color;
    for (size_t i = 0; i < drawnObjects.size(); ++i)
        drawnObjects[i].color = defaultColor;
}

// Sets default transformation matrix
void Scene::setDefaultModel(const Matrix4f &_defaultModel) {
    defaultTransformation = _defaultModel;
    for (size_t i = 0; i < drawnObjects.size(); ++i)
        drawnObjects[i].transformation = defaultTransformation;
}

// Sets default shader ID
void Scene::setDefaultShaderID(GLuint id) {
    defaultShaderID = id;
}

// Sets light data to use in shader
void Scene::setLight(GLfloat * position, GLfloat power) {
    for (GLuint iCoord = 0; iCoord < 4; iCoord++)
        lightPosition[iCoord] = position[iCoord];
    lightPower = power;
}

// Decides what will elements drawn after this call will look like
void Scene::setAppearance(const ObjectInstance &instance) {
    const size_t shaderId = instance.shaderId;

    glUseProgram(shaderId); // From now on, this shader will be used.

    // We use the specific values of model per object
    setMatricesInShader(shaderId, instance.transformation, camera.getView(), camera.getPosition(), camera.getProjection());
    glUniform4fv(glGetUniformLocation(shaderId, "color"), 1, instance.color);

    // Specifies which VBO were filled
    const Object * pObject = storedObjects[instance.objectId];
    assert(pObject);
    setFilledDataInShader(shaderId, pObject->hasPrimitives(), pObject->hasNormals(), pObject->hasUvs(), pObject->hasColors());
    
    // Sets the light in the current shader
    setLightInShader(shaderId, lightPosition, lightPower);
}

// Draw all Objects
void Scene::drawObjectsOfScene() {
    for (size_t i = 0; i < drawnObjects.size(); ++i) {
        const ObjectInstance &instance = drawnObjects[i];
        setAppearance(instance);
        storedObjects[instance.objectId]->drawObject();
    }
}

} // namespace stein
