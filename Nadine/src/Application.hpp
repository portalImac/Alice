// Application.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include "GLHeaders.hpp"

// Windowing system SDL
#include <SDL.h>

// Forward declaration
struct Scene;

// All the initialisation of states and events for SDL and OpenGL
class Application
{
    public:
    
        int animateEventID;                     // ID of the animation timer
	int moveFPSEventID;
        SDL_TimerID animateTimer;               // Timer for the animation
	SDL_TimerID moveFPSTimer;   

	bool goingForward;
	bool goingBackward;
	bool goingLeft;
	bool goingRight;
	bool goingUp;
	bool goingDown;
        
	    SDL_Surface* drawContext;

	    unsigned int windowedWidth;             // Window dimentions when not fullscreen - horizontal axis
	    unsigned int windowedHeight;            // Window dimentions when not fullscreen - vertical axis
	    unsigned int fullScreenWidth;           // Screen dimentions - horizontal axis
	    unsigned int fullScreenHeight;          // Screen dimentions - vertical axis	
	    unsigned int width;                     // Window actual dimentions - horizontal axis
	    unsigned int height;                    // Window actual dimentions - vertical axis
	    
        Uint32 videoModeFlags;
        
	    GLfloat xMousePosition;                 // Mouse position - horizontal axis (-1 to 1)
	    GLfloat yMousePosition;                 // mouse position - vertical axis (-1 to 1)
	    GLfloat xMouseLeftDownPosition;         // Mouse position - updated only when left button down
	    GLfloat yMouseLeftDownPosition;         // mouse position - updated only when left button down
	    int scroll;                             // scroll value (up : ++, down : --)
	    
	    GLfloat moveFlags[3];
	    bool mouse;                             // True if mouse is seeable
	    
	    GLuint cntFrame;                        // Frame counter
	    uint64_t lastStartTime;                 // Time updated every 10 frames
	    uint64_t frameDuration;                 // Frame time duration
	    
	    GLuint cntAnimation;                         // Move counter
	    bool done;                              // True when the window is closed to end the application
        Scene * scene;                          // Scene to draw
        
        GLfloat backgroundColor[4];             // Color of the background;

		GLuint nbVertices;
		GLuint nbTriangles;
		GLfloat* objVertices;
		GLuint* objIndices;
		GLfloat* objNormals;
		GLfloat boxHalfSize[3];
		
        Application(unsigned int width, unsigned int height);
        ~Application();
        
        void init(unsigned int width, unsigned int height);
        void initSDLOpenGL();
	void switchFullScreen ();
        void customizeStates();
             
        void setScene(Scene * scene);
        
        void initTimers();

        void resize(GLuint w, GLuint h);
        void setBackgroundColor(GLfloat * color);
        void printFPS();

	void switchWireframe();
	
	void changeBackground();
        
        void handleUserEvent(SDL_Event& event);
        void handleKeyEvent(SDL_keysym& keysym, bool down);
        void handleEvent(SDL_Event& event);
        
        void renderFrame();
        
        void loop();

        void animate();
	void moveFPS();

};

Uint32 genericTimer(Uint32 interval, void* param);


#endif //__APPLICATION_HPP__ 
