// main.c
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#include "GLHeaders.hpp"
#include "Application.hpp"
#include "Builders.hpp"
#include "Tools.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Object.hpp"

#include <iostream>
#include <vector>
#include <string>

// Entry point in the program
int main(int argc, char **argv)
{
    std::cout<<std::endl<<std::endl<<"________Template OpenGL3________"<<std::endl<<std::endl;  
    //__________________________________________________________________________ 
    
    // Application creation
    std::cout<<"  Application creation"<<std::endl;
    Application * application=new Application(800, 600);
    //__________________________________________________________________________ 

	// Scene creation
    std::cout<<"    Scene creation"<<std::endl;
    Scene * scene=new Scene(800, 600);
    application->setScene(scene);
    GLfloat red[]={1.0, 0.0, 0.0, 1.0};
    scene->setDefaultColor(red); // Color for the objects drawn in the scene
    
    //__________________________________________________________________________

    // Camera position and orientation
    std::cout<<"    Camera settings"<<std::endl;
    scene->camera->c[2]=0.0; // Position of the camera
    scene->camera->c[1]=0.5; // Position of the camera
    scene->camera->c[0]=0.0; // Position of the camera
    scene->camera->updateView();


    GLfloat c[]={-0.2, 0.3, 0.1}; // Camera position    
    GLfloat aim[]={0.0, 0.0, 0.0}; // Where we look
    GLfloat up[]={0.0, 1.0, 0.0}; // Vector pointing over the camera
    //scene->camera->lookAt(c, aim, up);

    //__________________________________________________________________________

    // Creation of the shaders
    std::cout<<"    Shaders reading, compiling and linking"<<std::endl;
    
    // Compilation and storage of the program
    std::vector<std::string> files;

    // The special shaders used for mac os are only necesary until Snow Leopard
    // From Leon on, mac os drivers version for Opengl is 3.2
    // Therefore the shaders in folder oldGLSL become out-of-date

    // One very simple shader (one color everywhere on the object)
	files.push_back("../shaders/simpleShader.glsl");
    GLuint simpleShaderID=loadProgram(files); files.pop_back();
    scene->setDefaultShaderID(simpleShaderID);
    
    // One coloring shader (colors per vertex)
	files.push_back("../shaders/colorShader.glsl");
    GLuint colorShaderID=loadProgram(files); files.pop_back();

	// One lighting shader
	#ifdef APPLE
	files.push_back("../shaders/oldGLSL/shaderTools.glsl");
	files.push_back("../shaders/oldGLSL/lightingShader.glsl");
	#else
	files.push_back("../shaders/shaderTools.glsl");
	files.push_back("../shaders/lightingShader.glsl");
	#endif
	GLuint lightingShaderID=loadProgram(files); files.pop_back();
	scene->setDefaultShaderID(lightingShaderID);
	// Uniforms filling
	glUseProgram(lightingShaderID);
	// Material creation and to shader
	GLfloat ambient[]={1.0, 1.0, 1.0, 1.0}; GLfloat ka=0.01;
	GLfloat diffuse[]={1.0, 1.0, 1.0, 1.0}; GLfloat kd=1.0;
	GLfloat specular[]={1.0, 1.0, 1.0, 1.0}; GLfloat ks=2.0; GLfloat shininess=5.0;
	setMaterialInShader(lightingShaderID, ambient, diffuse, specular, ka, kd, ks, shininess);    

	files.push_back("../shaders/persoShader.glsl");
	GLuint persoShaderID=loadProgram(files); files.pop_back();

	// Light creation
	GLfloat lightPosition[]={1.0, 1.0, 1.0, 1.0}; GLfloat lightPower=2.0;
	scene->setLight(lightPosition, lightPower);

	//__________________________________________________________________________


    // Creation of the objects to store
    std::cout<<"    Generic objects creation and storage :"<<std::endl;

    // Définition du repère
    Object * objectAxis=new Object(GL_LINES);
    GLuint storedObjectAxisID=scene->storeObject(objectAxis);
    
    // Définition du cube
    Object * objectCube=new Object(GL_TRIANGLES);
    GLuint storedObjectCubeID=scene->storeObject(objectCube);
    
    /* Environnement
    Object * objectLaby=new Object(GL_TRIANGLES);
    GLuint storedObjectLaby=scene->storeObject(objectLaby);
    bool smoothObjectFlag=true;*/
    
    // Définition de la cible
    Object * objectCible=new Object(GL_LINES);
    GLuint storedObjectCibleID=scene->storeObject(objectCible);
       

    //__________________________________________________________________________
    
    // Object building
    std::cout<<"    Generic objects building :"<<std::endl;
	
    
    // Construction du repère
    buildAxis(objectAxis);
    
    //Construction du cube
    buildCube(objectCube);

    /*environnement
    std::string fileName="../objs/labyrinthe.obj";
    buildObjectGeometryFromOBJ(objectLaby, fileName, smoothObjectFlag);*/
    
    // Construction de la cible
    buildCircle(objectCible, 0.3, 20);
    
         
    //__________________________________________________________________________
              
    // Objects we want to see
    std::cout<<"    Objects to draw setting"<<std::endl;

    
    /*Texture
       GLuint wHouseDiffuse;
       GLuint hHouseDiffuse;
       unsigned char * dataHouseDiffuse;

       dataHouseDiffuse = loadPPM("../textures/house_normal.ppm", &wHouseDiffuse, &hHouseDiffuse);
       glActiveTexture(GL_TEXTURE0);
       GLuint textureHouseDiffuseID;
       glGenTextures(1, &textureHouseDiffuseID);
       glBindTexture(GL_TEXTURE_2D, textureHouseDiffuseID);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wHouseDiffuse, hHouseDiffuse, 0, GL_RGB, GL_UNSIGNED_BYTE, dataHouseDiffuse);

       glUniform1i(glGetUniformLocation(lightingShaderID, "textureUnit0"), 0);*/

    //axis
    GLuint axisID=scene->addObjectToDraw(storedObjectAxisID);
    scene->setDrawnObjectShaderID(axisID, lightingShaderID);
    
    //cube
    GLfloat S[16];
	GLfloat s[3] = {10.0, 2.0, 10.0} ;
	setToScale(S, s);
    GLuint cubeID=scene->addObjectToDraw(storedObjectCubeID);
    scene->setDrawnObjectModel(cubeID, S);  
    scene->setDrawnObjectShaderID(cubeID, lightingShaderID);
    
    /*environnement
    GLuint labyID=scene->addObjectToDraw(storedObjectLaby);
    scene->setDrawnObjectShaderID(labyID, lightingShaderID);*/
        
        
    //cible
    GLuint cibleID=scene->addObjectToDraw(storedObjectCibleID);
    scene->setDrawnObjectShaderID(cibleID, lightingShaderID);
        
    
    GLfloat blue[4]={0.0, 0.3, 0.7, 1.0};
    scene->setDrawnObjectColor(cibleID, blue);
       
    //__________________________________________________________________________
    
    // Other informations
    // Background color creation
    GLfloat black[]={0.0, 0.0, 0.0, 1.0};
    application->setBackgroundColor(black);
    
    //__________________________________________________________________________
    
    // Errors checking
    std::cout<<"    Errors cheking before the loop"<<std::endl;
    printGlErrors(); std::cout<<std::endl;
    //__________________________________________________________________________  
      
    // Loop start !
    std::cout<<"    Looping !! "<<std::endl;
    application->lastStartTime=getTime();
    application->initTimers();
    application->loop();
    printGlErrors(); std::cout<<std::endl;
    //__________________________________________________________________________

    // Cleaning and finishing
    std::cout<<"    Cleaning before leaving"<<std::endl; 
    delete application;
   
	return 0;
}
