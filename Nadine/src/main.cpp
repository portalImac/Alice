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
    Application * application=new Application();
    //__________________________________________________________________________ 

	// Scene creation
    std::cout<<"    Scene creation"<<std::endl;
    Scene * scene=new Scene();
    application->setScene(scene);
    GLfloat red[]={1.0, 0.0, 0.0, 1.0};
    scene->setDefaultColor(red); // Color for the objects drawn in the scene
    
    //__________________________________________________________________________

    // Camera position and orientation
    std::cout<<"    Camera settings"<<std::endl;
    scene->camera->c[2]=1.0; // Position of the camera
    scene->camera->c[1]=0.0; // Position of the camera
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

    // Définition de la sphère
    Object * objectSphere=new Object(GL_TRIANGLES);
    GLuint storedObjectSphereID=scene->storeObject(objectSphere);


    //__________________________________________________________________________
    
    // Object building
    std::cout<<"    Generic objects building :"<<std::endl;


        //An obj house (smooth edges)
           Object * objectHouse=new Object(GL_TRIANGLES);
           GLuint storedObjectHouse=scene->storeObject(objectHouse);
           bool smoothObjectFlag=true;
           std::string fileName="../objs/house.obj";
           buildObjectGeometryFromOBJ(objectHouse, fileName, smoothObjectFlag);
    
    // Construction du repère
    buildAxis(objectAxis);

    // Construction de la sphère
    GLfloat radiusSphere = 0.07;
    GLuint discLatSphere = 40;
    GLuint discLongSphere = 40;
    buildSphere_TrFlatRed(objectSphere, radiusSphere, discLatSphere, discLongSphere);


     
    //__________________________________________________________________________
              
    // Objects we want to see
    std::cout<<"    Objects to draw setting"<<std::endl;

    //house
    GLuint houseID=scene->addObjectToDraw(storedObjectHouse);
    scene->setDrawnObjectShaderID(houseID, lightingShaderID);

    //Texture
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

       glUniform1i(glGetUniformLocation(lightingShaderID, "textureUnit0"), 0);

    //axis
    GLuint axisID=scene->addObjectToDraw(storedObjectAxisID);
    scene->setDrawnObjectShaderID(axisID, lightingShaderID);


    //Soleil
    GLuint indiceSoleil [125];

    for (int x=0; x<5; ++x) {
	for (int z=0; z<25; ++z) {
		indiceSoleil [x*z+z] = scene->addObjectToDraw(storedObjectSphereID);
		scene->setDrawnObjectShaderID(indiceSoleil[x*z+z], persoShaderID);
		GLfloat orange[4]={0.7, 0.2, 0.2, 0.0};
  		//scene->setDrawnObjectColor(indiceSoleil[x*z+z], orange);
		
    		GLfloat T[16];
		GLfloat t[4] = {((GLfloat)x/5),
			0.0,
			((GLfloat)-z/5),
			1.0
			};
		setToTranslate(T, t);

    		scene->setDrawnObjectModel(indiceSoleil[x*z+z], T);
	}
    }


    
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

