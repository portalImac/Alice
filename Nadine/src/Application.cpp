// Application.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#include "Application.hpp"
#include "Scene.hpp"
#include "Tools.hpp"
#include "Camera.hpp"

#include <iostream>
#include <cmath>
#include <sstream>



// Default constructor
Application::Application(unsigned int width, unsigned int height)
{
    this->init(width, height);
}


// Cleans before the application can be closed
Application::~Application()
{
    SDL_RemoveTimer(this->animateTimer);
    SDL_RemoveTimer(this->moveFPSTimer);
    SDL_Quit();
    if (scene!=NULL) delete scene;
}


// Sets the application parameters and does all the initialisation
void Application::init(unsigned int width, unsigned int height)
{
    this->scene=NULL;
    
    // False as long as we don't want to quit the application
    this->done=false;
    
    // BackgroundColor
	GLfloat black[]={0.0, 0.0, 0.0, 1.0};
    this->setBackgroundColor(black);
    
	this->moveFlags[0]=0.0;
	this->moveFlags[1]=0.0;
	this->moveFlags[2]=0.0;
	
	// Times measurements initialisation
	this->cntFrame=0;
	uint64_t lastStartTime=0;
	uint64_t frameDuration=0; 
	
    // Move counter
    this->cntAnimation=0;

    // Camera FPS deplacement
    this->goingForward = this->goingBackward = false;
    this->goingLeft = this->goingRight = false;
    this->goingUp = this->goingDown = false;

    
    // Mouse is seeable at first
    this->mouse=true;
    
    // Window size initialization (for windowed mode)
	this->windowedWidth=width;
	this->windowedHeight=height;
	
	// Mouse position, pressed position and scroll data initilaization
    // Positions : floats, origin in center, sides at +/-1 or more
	this->xMousePosition=0.0;
	this->yMousePosition=0.0;
	this->xMouseLeftDownPosition=0.0; 
	this->yMouseLeftDownPosition=0.0;


	this->scroll=0;
	
	// Initialisation of SDL and creation of OpenGL context
    initSDLOpenGL();
    
    // Customize a few OpenGL and SDL states (after context creation)
    customizeStates();
}


// Inits SDL and OpenGL context, sets a few states
void Application::initSDLOpenGL()
{
    // Initialization of SDL
    
        // Initialize timer, audio, video, CD_ROM, and joystick.
    int sdlError=SDL_Init(SDL_INIT_EVERYTHING);
    if (sdlError<0) 
		std::cout<<"Unable to init SDL : "<<SDL_GetError()<<std::endl;

    	// Sets openGL parameters before opening the draw context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);    // Double buffering
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);		// Depth buffer size of 16-bit
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);    // Color components size of 8-bit each
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	// Operating systems uniform keyboard handling 
	// (overhead, disable for performance : param->0)
	SDL_EnableUNICODE(1);	

    // Capture of screen size (for fullscreen mode)
    
    const SDL_VideoInfo* videoInfo=SDL_GetVideoInfo();
	this->fullScreenWidth=videoInfo->current_w;
	this->fullScreenHeight=videoInfo->current_h;

	// Creation of the openGL draw context
	
		// Window size
	this->width=this->windowedWidth;
	this->height=this->windowedHeight;	
	    // Options about the OpenGL window for SDL_SetVideoMode(...)  
	this->videoModeFlags = SDL_OPENGL | SDL_RESIZABLE; // Resizable OpenGL display
        // Specifies the size and other options about the window and OpenGL context
     this->drawContext = SDL_SetVideoMode(this->width, this->height, 0, this->videoModeFlags);

// Désactiver le pointeur
//SDL_ShowCursor(SDL_DISABLE);


// Bloquer le pointeur dans la scène
//SDL_WM_GrabInput(SDL_GRAB_ON);

}





// Customize a few OPenGL states to fit the application's needs
void Application::customizeStates()
{
   // Puts the window top-left corner at the following coordinates 
    // Resizing doesn't impact so it is not very usefull
	//putenv("SDL_VIDEO_WINDOW_POS=100,0"); 
	 
    	// Glew initialisation : to register all available extentions
    GLenum glewError=glewInit();
    if (glewError!=GLEW_OK)
		std::cout<<"GLEW Error : "<<glewGetErrorString(glewError)<<std::endl;
	
	// Mouse motion will not generate events
	// Instead we will check the position ourselves when we need them
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
		

	// Initialization of the mouse position in the middle of the window	

	    // WarpMouse changes the mouse position and 
        // generates motion events which we need to ignore.
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_WarpMouse(this->width/2, this->height/2);
        // After, we can reactivate the mouse motion events
        // But we instead choose to check directly the position 
        // ourselves when we need it (in the camera update)
        // It is better then to disable the unused events
    SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);


    	// Depth test
    glEnable(GL_DEPTH_TEST);
    
        // Decides the background color used after this call
    GLfloat black[]={0.0, 0.0, 0.0, 1.0};
    setBackgroundColor(black);
    
        // Sets the with of the lines
    glLineWidth(2); 
    
        // Disables culling
    //glDisable(GL_CULL_FACE);
}


// Sets one scene for the application to draw
void Application::setScene(Scene * scene)
{
	this->scene=scene;
	resize(this->scene->width, this->scene->height);
}


// Initializes timers
void Application::initTimers()
{
    //Each timer event has an arbitrairy int ID
    
    this->animateEventID=3; // Identifies our move loop event
    this->moveFPSEventID=2;
    
    // Creates a timer to send a move event every 60 ms    
    this->animateTimer=SDL_AddTimer(60, genericTimer, (void*)(&(this->animateEventID)));
    this->moveFPSTimer=SDL_AddTimer(60, genericTimer, (void*)(&(this->moveFPSEventID)));
}


// Create an user event, give it the id (int) passed in "param",
// and registers it : it should now be send every "interval" set of time
Uint32 genericTimer(Uint32 interval, void* param)
{
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = *(int *)param; 
    event.user.data1 = 0;
    event.user.data2 = 0;
    SDL_PushEvent(&event);
    return interval;
}


// Adapts the drawing to the new size of the window
// resize doesn't work on Mac os (Windows ?)
void Application::resize(GLuint w, GLuint h)
{
    //std::cout<<"Window resize  : ["<<w<<","<<h<<"]"<<std::endl;
    
    this->width=w;
    this->height=h;

	//SDL_VideoMode update (restart the OpenGL context on windows, does not work on mac os...)
	//#ifdef __APPLE__ | WIN32 // was not tested
        //this->drawContext = SDL_SetVideoMode(this->width, this->height, 0, this->videoModeFlags);
        //std::cout<<SDL_GetError()<<std::endl;
        //this->customizeStates();
    //#else
        this->drawContext = SDL_SetVideoMode(this->windowedWidth, this->windowedHeight, 0, this->videoModeFlags);
    //#endif
	
    
    // Viewport transformation update to fit initial window size
    glViewport(0, 0, this->width, this->height);

    // Projection transformation update
        // Keeping the angle constant
    GLfloat fovy=M_PI/3.0;
    GLfloat aspectRatio=this->width/GLfloat(this->height);
    this->scene->camera->setPerspectiveFromAngle(fovy, aspectRatio);
}

//
void Application::switchFullScreen (){
	if (this->videoModeFlags == (SDL_OPENGL | SDL_RESIZABLE)) {
		this->videoModeFlags = SDL_OPENGL | SDL_FULLSCREEN;
		this->width = this->fullScreenWidth;
		this->height = this->fullScreenHeight;
	}
	else {
		this->videoModeFlags = SDL_OPENGL | SDL_RESIZABLE;
		this->width = this->windowedWidth;
		this->height = this->windowedHeight;
	}
	resize(this->width, this->height);
}

// Sets the background color
void Application::setBackgroundColor(GLfloat * color)
{
    this->backgroundColor[0]=color[0];
    this->backgroundColor[1]=color[1];
    this->backgroundColor[2]=color[2];
    this->backgroundColor[3]=color[3];
    glClearColor(color[0], color[1], color[2], color[3]);
}


// Prints the frame duration and framerate (both averaged over 10 frames)
void Application::printFPS()
{
    float milliseconds=((float)(this->frameDuration)/1000.0);
    float FPS=1000.0/milliseconds; // Unit : [s^-1]= 1/seconds
    std::cout<<"frame : "<<milliseconds<<" ms       FPS : "<<FPS<<std::endl;
    
    // To print fps in the title bar
    //std::ostringstream title; title.precision(3); title.width(5);
    //title<<"frame : "<<milliseconds<<" ms       FPS : "<<FPS;
    //SDL_WM_SetCaption(title.str().data(), 0);
}


// Distributes task for the "user" kind of events 
// For example : rendrFrame action occurs if a timer event is passed
void Application::handleUserEvent(SDL_Event& event)
{
    int eventID=event.user.code;
    
    // In case an animate event occured      
    if (eventID==animateEventID)
        animate();

    if(eventID==moveFPSEventID)
	moveFPS();
}

// Permet de changer de mode (plein ou fil de fer)
void Application::switchWireframe()
{
	// Tableau params avec case 0 face avant et case 1 face arrière
	GLint params[2];
	glGetIntegerv(GL_POLYGON_MODE, params);
	// Si la face avant est pleine, on passe en mode fil de fer
	if(params[0]==GL_FILL)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Permet de changer la couleur de fond
void Application::changeBackground()
{
	// Selon la couleur de fond, remplis un tableau avec une nouvelle couleur
	changeColor(this->backgroundColor);
	// Change la couleur de fond
	setBackgroundColor(this->backgroundColor);
}


// Distributes task for the "key" kind of events 
// For example : std::cout when b key is pressed
// down is true when the key is pressed, false when released



void Application::handleKeyEvent(SDL_keysym& keysym, bool down)
{
    // unit is at 1 when the key is pressed and at -1 when released
    float unit=-1.0;
    if (down) unit=1.0;
    
    if (down)
    {
        switch(keysym.sym)
        {
        
            std::cout<<"down"<<std::endl;
        	case SDLK_ESCAPE:
          		this->done=true;
          	break;
          	
        	case SDLK_c:
			if(this->scene->camera->perspectiveProjection==true)
				this->scene->camera->perspectiveProjection=false;
			else
				this->scene->camera->perspectiveProjection=true;
			this->scene->camera->updateProjection();
		break;
          	
        	case SDLK_w :
                std::cout<<"Key \"w\" was pressed."<<std::endl;
          		switchWireframe();
          	break;
          	
          	case SDLK_f :
          	    std::cout<<"Key \"f\" was pressed."<<std::endl;
          	    printFPS();
          	break;
          	
         	case SDLK_F5 :
                    std::cout<<"Key \"F5\" was pressed."<<std::endl;
          		switchFullScreen();
                break;
          	    	
          	
          	default:
      	    break;
      	}
    }

    switch(keysym.sym)
    {
        // Camera controls
      	// Keys are associated with x, y, z direction for move.
      	// moveFlags holds a 3D vector of direction
      	// each coordinates of moveFlags is in the end : -1, 0 or 1.   	
        
        case SDLK_UP :
        case SDLK_z :
            //std::cout<<"Forward."<<std::endl;
      		this->moveFlags[2]-=unit;
      	break;
      	
    	case SDLK_DOWN :
    	case SDLK_s :
            //std::cout<<"Backward."<<std::endl;
      		this->moveFlags[2]+=unit;
      	break;
      	
      	case SDLK_LEFT :
      	case SDLK_q :
            //std::cout<<"Left."<<std::endl;
      		this->moveFlags[0]-=unit;
      	break;
      	
      	case SDLK_RIGHT :
      	case SDLK_d :
            //std::cout<<"Right."<<std::endl;
      		this->moveFlags[0]+=unit;
      	break;
      	
      	case SDLK_PAGEUP :
            //std::cout<<"Up."<<std::endl;
      		this->moveFlags[1]+=unit;
      	break;
      	
      	case SDLK_PAGEDOWN :
            //std::cout<<"Down."<<std::endl;
      		this->moveFlags[1]-=unit;
      	break;  

    	default:
      	break;
  	}
}


// Listens to events during the whole time of the application
// and distributes corresponding tasks
void Application::handleEvent(SDL_Event& event)
{
    switch(event.type) 
    {
        // User events
        case SDL_USEREVENT:
            handleUserEvent(event);
            break;
                
        // Key presses
        case SDL_KEYDOWN:
            handleKeyEvent(event.key.keysym, true);
            break;
                
        // Key releases    
        case SDL_KEYUP:
            handleKeyEvent(event.key.keysym, false);
            break;

	//Mouse

	case SDL_MOUSEMOTION:
		this->xMousePosition += 2.0*event.motion.xrel/(GLfloat)this->width;
		this->yMousePosition += -2.0*event.motion.xrel/(GLfloat)this->height;
	break;
	

	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
                	this->scene->cameraEntry = new Camera();
			//this->scene->cameraEntry->c[2] = -2.0;
                        this->scene->throwRay(this->xMousePosition, this->yMousePosition);
		}
            	else if (event.button.button == SDL_BUTTON_RIGHT)
                	this->scene->cameraExit = new Camera();
	break;
	               
            
        // Quit event (for example sent when the window is closed)
        case SDL_QUIT:
            this->done=true;
            break;

    }
}




// Clears the current frame buffer (the image on screen) 
// draws the scene in the other available frame buffer (double buffering)
// and prints the new filled frame buffer on screen
void Application::renderFrame()
{
	// Clears the window with current clearing color, clears also the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draws this->scene
	if (this->scene!=NULL)
    	this->scene->drawObjectsOfScene();
    
    // Performs the buffer swap between the current shown buffer, 
    // and the one we just worked on
    SDL_GL_SwapBuffers();
    
    // Gets the average frame duration over 20 frames
    this->cntFrame++;
    if (this->cntFrame%20==0)
    {
        uint64_t time=getTime();
        this->frameDuration=(time-this->lastStartTime)/20LL;
        this->lastStartTime=time;
    }
    
    // Reports any possible glError
    //std::cout<<"renderFrame error"<<std::endl;
    printGlErrors();
}


// Listens to events during the whole time of the application
// and distributes corresponding tasks
void Application::loop()
{
    SDL_Event event;
    
    // While the application is running (this->done==false), 
    while (!this->done)
    {
        // SDL_PollEvent return 1 when there is still events on the stack of events
        // It takes the first event in the queue and writes it in the "event" parameter
        // Then it removes it from the stack.
        while (SDL_PollEvent(&event)) {
            handleEvent(event); // Checks each event for types and loach corresponding actions
	}
        // Renders the frame
        renderFrame();
    }
}


void Application::animate()
{

	this->cntAnimation++;
}

// Moves with a FPS camera
void Application::moveFPS()
{
    // Motion motion information retrieval
    // Comment if mouse motion events are enabled
    SDL_PumpEvents();
    int mouseRelX, mouseRelY;
	#ifdef __APPLE__
        int mystery=0;
        SDL_GetRelativeMouseState(mystery, &mouseRelX, &mouseRelY);
    #else
        SDL_GetRelativeMouseState(&mouseRelX, &mouseRelY);
    #endif
    
	this->xMousePosition+=2.0*mouseRelX/(GLfloat)this->width;
	this->yMousePosition+=-2.0*mouseRelY/(GLfloat)this->height;



	GLfloat moveStep=0.02;
	GLfloat cameraNewPos[3];
	GLfloat moveOnX=this->moveFlags[0]*moveStep;
	GLfloat moveOnY=this->moveFlags[1]*moveStep;
	GLfloat moveOnZ=this->moveFlags[2]*moveStep;
	for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
	{
		cameraNewPos[iCoord]=this->scene->camera->c[iCoord]
							+this->scene->camera->x[iCoord]*moveOnX
							+this->scene->camera->y[iCoord]*moveOnY
							+this->scene->camera->z[iCoord]*moveOnZ;
	}

	GLfloat angleForWindowWidth=M_PI;
	GLfloat angleForWindowHeight=M_PI/2.0;
	GLfloat angleLong=this->xMousePosition*angleForWindowWidth;
	GLfloat angleLat=this->yMousePosition*angleForWindowHeight;


	// Method with rotates

	GLfloat xAxis[]={1.0, 0.0, 0.0}; GLfloat yAxis[]={0.0, 1.0, 0.0};
	GLfloat rotateAroundX[16]; setToRotate(rotateAroundX, -angleLat, xAxis);
	GLfloat rotateAroundY[16]; setToRotate(rotateAroundY, angleLong, yAxis);
	GLfloat t[]={-cameraNewPos[0], -cameraNewPos[1], -cameraNewPos[2]};
	GLfloat translate[16]; setToTranslate(translate, t);

	setToIdentity(this->scene->camera->view);
	multMatrixBtoMatrixA(this->scene->camera->view, rotateAroundX);
	multMatrixBtoMatrixA(this->scene->camera->view, rotateAroundY);
	multMatrixBtoMatrixA(this->scene->camera->view, translate);

	
	GLfloat Tr[16];
	GLfloat tr[3] = {0.0, 0.0, -(scene->camera->near) - 0.1} ;
	setToTranslate(Tr, tr);
	
	GLfloat Sc[16];
	GLfloat sc[3] = {0.05, 0.05, 0.0} ;
	setToScale(Sc, sc);
	
	GLfloat invView[16];
	getInverseGenericMatrix(scene->camera->view, invView);
	
	multMatrixBtoMatrixA(Tr, Sc);
	multMatrixBtoMatrixA(invView, Tr);
		
	scene->setDrawnObjectModel(2, invView);  

	for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
	{
		// Updates the axis with values in view
		this->scene->camera->x[iCoord]=this->scene->camera->view[iCoord*4+0];
		this->scene->camera->y[iCoord]=this->scene->camera->view[iCoord*4+1];
		this->scene->camera->z[iCoord]=this->scene->camera->view[iCoord*4+2];
	}
	// Updates the position of the camera c with cub constraints
		if ( cameraNewPos[0] > -5.0 && cameraNewPos[0] < 5.0)
	    this->scene->camera->c[0]=cameraNewPos[0];
	if ( cameraNewPos[2] > -5.0 && cameraNewPos[2] < 5.0)
	    this->scene->camera->c[2]=cameraNewPos[2];


}
