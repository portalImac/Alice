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
#include <cmath>

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
    scene->camera->c[2]=4.0; // Position of the camera
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

	// One texture and Phong shader
	#ifdef __APPLE__
	    files.push_back("../shaders/oldGLSL/lightingTexturingShader.glsl");
   	 #else
	    files.push_back("../shaders/lightingTexturingShader.glsl");
   	 #endif
   	 GLuint lightingTextureShaderID=loadProgram(files); files.pop_back();
    	glUseProgram(lightingTextureShaderID);
    	setMaterialInShader(lightingTextureShaderID, ambient, diffuse, specular, ka, kd, ks, shininess);
    	setTextureUnitsInShader(lightingTextureShaderID);

	
	//__________________________________________________________________________


    // Creation of the objects to store
    std::cout<<"    Generic objects creation and storage :"<<std::endl;

    // Définition du repère
    Object * objectAxis=new Object(GL_LINES);
    GLuint storedObjectAxisID=scene->storeObject(objectAxis);
    
    
    // Définition boundingBox
    Object * objectBB=new Object(GL_TRIANGLES);
    GLuint storedObjectBBID=scene->storeObject(objectBB);
    
    // Définition de la cible
    Object * objectCible=new Object(GL_LINES);
    GLuint storedObjectCibleID=scene->storeObject(objectCible);
  
    // Environnement
    Object * objectLaby=new Object(GL_TRIANGLES);
    GLuint storedObjectLaby=scene->storeObject(objectLaby);
    bool smoothObjectFlag=true;
    GLuint houseTextureDiffuseID=loadTexture("../textures/mur_contour.ppm");
    GLuint houseTextureSpecularID=loadTexture("../textures/mur_contour.ppm");

    //portal
    Object * objectPortal = new Object(GL_TRIANGLES);
    GLuint storedObjectPortal=scene->storeObject(objectPortal);
    //std::cout<<"IIIIIDDDDDD"<<storedObjectPortal<<std::endl;

    GLuint portalTextureDiffuseID=loadTexture("../textures/mur_contour.ppm");
    GLuint portalTextureSpecularID=loadTexture("../textures/mur_contour.ppm");

    /*Object * objectTr=new Object(GL_TRIANGLES);
    GLuint storedObjectTrID=scene->storeObject(objectTr);*/

//__________________________________________________________________________ 

    // Object building
    std::cout<<"    Generic objects building :"<<std::endl;
	
    
    // Construction du repère
    buildAxis(objectAxis);
    
    //Construction boundingBox
    //buildCube(objectBB);

	// Construction de la cible
    buildCircle(objectCible, 0.3, 20);
    
	//environnement
    std::string fileName="../objs/murlaby.obj";
    std::vector<GLfloat> verticesObj;
    std::vector<GLuint> indicesObj;
    std::vector<GLfloat> normalsObj;
    buildObjectGeometryFromOBJ(objectLaby, fileName, smoothObjectFlag, verticesObj, indicesObj, normalsObj);
  
    application->nbTriangles = indicesObj.size() / 3;
    application->nbVertices = verticesObj.size() / 4;
    
    application->objVertices = new GLfloat[application->nbTriangles * 12];
    application->objIndices = new GLuint[indicesObj.size()];
    application->objNormals = new GLfloat[application->nbTriangles * 3];
    
    
    
    for (int i=0; i<application->nbTriangles; ++i)

	{
      application->objVertices[12*i] = verticesObj[4*indicesObj[3*i]];
	  application->objVertices[12*i+1] = verticesObj[4*indicesObj[3*i]+1];
	  application->objVertices[12*i+2] = verticesObj[4*indicesObj[3*i]+2];
	  application->objVertices[12*i+3] = verticesObj[4*indicesObj[3*i]+3];
	  
	  application->objVertices[12*i+4] = verticesObj[4*indicesObj[3*i+1]];
	  application->objVertices[12*i+5] = verticesObj[4*indicesObj[3*i+1]+1];
	  application->objVertices[12*i+6] = verticesObj[4*indicesObj[3*i+1]+2];
	  application->objVertices[12*i+7] = verticesObj[4*indicesObj[3*i+1]+3];
	  
	  application->objVertices[12*i+8] = verticesObj[4*indicesObj[3*i+2]];
	  application->objVertices[12*i+9] = verticesObj[4*indicesObj[3*i+2]+1];
	  application->objVertices[12*i+10] = verticesObj[4*indicesObj[3*i+2]+2];
	  application->objVertices[12*i+11] = verticesObj[4*indicesObj[3*i+2]+3];
	}
	
	std::cout << indicesObj[3] << " " << indicesObj[4] << indicesObj[5] << std::endl;
	
	std::cout << verticesObj[11*4] << " " << verticesObj[11*4+1] << " " << verticesObj[11*4+2] << " " << verticesObj[11*4+3] << std::endl;
	std::cout << verticesObj[10*4] << " " << verticesObj[10*4+1] << " " << verticesObj[10*4+2] << " " << verticesObj[10*4+3] << std::endl;
	std::cout << verticesObj[4] << " " << verticesObj[4+1] << " " << verticesObj[4+2] << " " << verticesObj[4+3] << std::endl << std::endl;
	
	std::cout << verticesObj[4] << " " << verticesObj[4+1] << " " << verticesObj[4+2] << " " << verticesObj[4+3] << std::endl;
	std::cout << verticesObj[0] << " " << verticesObj[1] << " " << verticesObj[2] << " " << verticesObj[3] << std::endl;
	std::cout << verticesObj[11*4] << " " << verticesObj[11*4+1] << " " << verticesObj[11*4+2] << " " << verticesObj[11*4+3] << std::endl << std::endl;
	
	for (int i=0; i<24; ++i)
		std::cout << application->objVertices[i] << " " ;
	    
    std::cout << std::endl;

    std::string fileNamePortal="../objs/portail.obj";
    std::vector<GLfloat> verticesObjPortal;
    std::vector<GLuint> indicesObjPortal;
    std::vector<GLfloat> normalsObjPortal;
    buildObjectGeometryFromOBJ(objectPortal, fileNamePortal, smoothObjectFlag, verticesObjPortal, indicesObjPortal, normalsObjPortal);

    
    //__________________________________________________________________________
              
    // Objects we want to see
    std::cout<<"    Objects to draw setting"<<std::endl;


    //axis
    GLuint axisID=scene->addObjectToDraw(storedObjectAxisID);
    scene->setDrawnObjectShaderID(axisID, lightingShaderID);
    
    //BoundingBox
    GLuint BBID=scene->addObjectToDraw(storedObjectBBID);
    //scene->setDrawnObjectShaderID(BBID, lightingShaderID);
    
    GLfloat yellow[4]={0.0, 0.8, 0.4, 1.0};
    scene->setDrawnObjectColor(BBID, yellow);
    
        
    //cible
    GLuint cibleID=scene->addObjectToDraw(storedObjectCibleID);
    scene->setDrawnObjectShaderID(cibleID, lightingShaderID);
    GLfloat blue[4]={0.0, 0.3, 0.7, 1.0};
    scene->setDrawnObjectColor(cibleID, blue);
    
    //environnement
    GLfloat S[16];
	GLfloat s[3] = {3.0, 2.0, 3.0} ;
	setToScale(S, s);
		
    GLuint labyID=scene->addObjectToDraw(storedObjectLaby);
    //scene->setDrawnObjectModel(labyID, S);
    scene->setDrawnObjectColor(labyID, red);
	scene->setDrawnObjectShaderID(labyID, lightingShaderID);
        //std::cout<<"ID SHADER :"<<lightingShaderID<<std::endl;
	/*cible
    GLuint trID=scene->addObjectToDraw(storedObjectTrID);
    scene->setDrawnObjectShaderID(trID, lightingShaderID);
    GLfloat zarb[4]={0.4, 0.7, 0.2, 1.0};
    scene->setDrawnObjectColor(trID, zarb);*/

        std::cout<<scene->nbStoredObjects<<std::endl;
    
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

