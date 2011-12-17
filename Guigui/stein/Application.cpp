// Application.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#include "Application.hpp"
#include "Scene.hpp"
#include "Tools.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "Builders.hpp"

#include <iostream>

using namespace std;

namespace stein {

static int timerId = 1;

// Create an user event, give it the id (int) passed in "param",
// and registers it : it should now be send every "interval" set of time
static inline Uint32 genericTimer(Uint32 interval, void* param) {
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = *static_cast<int*>(param);
    event.user.data1 = 0;
    event.user.data2 = 0;
    SDL_PushEvent(&event);
    return interval;
}

// Default constructor
Application::Application(size_t width, size_t height) :
    m_pDrawContext(NULL), windowedWidth(width), windowedHeight(height), videoModeFlags(SDL_OPENGL | SDL_RESIZABLE), m_bRunning(true), m_bShowMouse(true), m_FrameCount(0) {
    
    // Mouse position, pressed position and scroll data initilaization
    // Positions : floats, origin in center, sides at +/-1 or more
    m_MouseXPos = 0.0;
    m_MouseYPos = 0.0;
    m_PressedMouseXPos = 0.0;
    m_PressedMouseYPos = 0.0;

    m_MouseScroll = 0;

    // Initialisation of SDL and creation of OpenGL context
    initSDLOpenGL();
    
    // Customize a few OpenGL and SDL states (after context creation)
    customizeStates();
}

// Cleans before the application can be closed
Application::~Application() {
    SDL_RemoveTimer(animateTimer);
    SDL_Quit();
}

// Inits SDL and OpenGL context, sets a few states
void Application::initSDLOpenGL() {
    // Initialization of SDL
    
    // Initialize timer, audio, video, CD_ROM, and joystick.
    int sdlError = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdlError < 0)
        cout << "Unable to init SDL : " << SDL_GetError() << endl;
    
    // Sets openGL parameters before opening the draw context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Double buffering
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); // Depth buffer size of 16-bit

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // Color components size of 8-bit each
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    // Operating systems uniform keyboard handling
    // (overhead, disable for performance : param->0)
    SDL_EnableUNICODE(1);

    // Capture of screen size (for fullscreen mode)
    
    const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
    fullScreenWidth = videoInfo->current_w;
    fullScreenHeight = videoInfo->current_h;

    // Creation of the openGL draw context

    // Window size
    width = windowedWidth;
    height = windowedHeight;
    // Specifies the size and other options about the window and OpenGL context
    m_pDrawContext = SDL_SetVideoMode(width, height, 0, videoModeFlags);
}

// Customize a few OPenGL states to fit the application's needs
void Application::customizeStates() {
    // Puts the window top-left corner at the following coordinates
    // Resizing doesn't impact so it is not very usefull
    //putenv("SDL_VIDEO_WINDOW_POS=100,0");

    // Glew initialisation : to register all available extentions
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
        cout << "GLEW Error : " << glewGetErrorString(glewError) << endl;

    // Mouse motion will not generate events
    // Instead we will check the position ourselves when we need them
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

    // Initialization of the m_bShowMouse position in the middle of the window

    // WarpMouse changes the mouse position and
    // generates motion events which we need to ignore.
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_WarpMouse(width / 2, height / 2);
    // After, we can reactivate the m_bShowMouse motion events
    // But we instead choose to check directly the position
    // ourselves when we need it (in the camera update)
    // It is better then to disable the unused events
    SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);

    // Depth test
    glEnable(GL_DEPTH_TEST);
    
    // Decides the background color used after this call
    setBackgroundColor(Color::BLACK);
    
    // Sets the with of the lines
    glLineWidth(2);
    
    // Disables culling
    //glDisable(GL_CULL_FACE);
}

// Initializes timers
void Application::initTimers() {
    // Creates a timer to send a move event every 60 ms    
    animateTimer = SDL_AddTimer(20, genericTimer, &timerId);
}

// Adapts the drawing to the new size of the window
// resize doesn't work on Mac os (Windows ?)
void Application::resize(GLuint w, GLuint h) {
    cout << "Window resize  : [" << w << "," << h << "]" << endl;

    width = w;
    height = h;
    
    //SDL_VideoMode update (restart the OpenGL context on windows, does not work on mac os...)
#if defined( __APPLE__ ) || defined(WIN32)
    // was not tested
    //drawContext = SDL_SetVideoMode(width, height, 0, videoModeFlags);
    //cout<<SDL_GetError()<<endl;
    //customizeStates();
#else
    m_pDrawContext = SDL_SetVideoMode(width, height, 0, videoModeFlags);
#endif
    // Viewport transformation update to fit initial window size
    glViewport(0, 0, width, height);
}

// Sets the background color
void Application::setBackgroundColor(const Color &color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

// Prints the frame duration and framerate (both averaged over 10 frames)
void Application::printFPS() {
    float milliseconds = ((float) (m_FrameDuration) / 1000.0);
    float FPS = 1000.0 / milliseconds; // Unit : [s^-1]= 1/seconds
    cout << "frame : " << milliseconds << " ms       FPS : " << FPS << endl;
}

// Distributes task for the "user" kind of events 
// For example : rendrFrame action occurs if a timer event is passed
void Application::handleUserEvent(const SDL_Event& event) {
    // In case an animate event occured      
    if (event.user.code == timerId) {
        animate();
        renderFrame();

        // Reports any possible glError
        printGlErrors();

        // Gets the average frame duration over 20 frames
        ++m_FrameCount;
        if (m_FrameCount % 20 == 0) {
            uint64_t time = getTime();
            m_FrameDuration = (time - m_LastStartTime) / 20LL;
            m_LastStartTime = time;
        }
    }
}

// Distributes task for the "key" kind of events 
// For example : cout when b key is pressed
// down is true when the key is pressed, false when released
void Application::handleKeyEvent(const SDL_keysym& keysym, bool down) {
    if (down) {
        switch (keysym.sym) {
            case SDLK_ESCAPE:
                m_bRunning = false;
                break;
            case SDLK_f:
                printFPS();
                break;
            default:
                break;
        }
    }
}

// Listens to events during the whole time of the application
// and distributes corresponding tasks
void Application::handleEvent(const SDL_Event& event) {
    switch (event.type) {
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
            // Quit event (for example sent when the window is closed)
        case SDL_QUIT:
            m_bRunning = false;
            break;
        default:
            break;
    }
}

// Clears the current frame buffer (the image on screen) 
// draws the scene in the other available frame buffer (double buffering)
// and prints the new filled frame buffer on screen
void Application::renderFrame() {
    // Clears the window with current clearing color, clears also the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draws scene
    m_Scene.drawObjectsOfScene();
    
    // Performs the buffer swap between the current shown buffer, 
    // and the one we just worked on
    SDL_GL_SwapBuffers();
}

// Listens to events during the whole time of the application
// and distributes corresponding tasks
void Application::loop() {
    initTimers();
    m_LastStartTime = getTime();
    SDL_Event event;
    
    // While the application is running (done==false),
    while (m_bRunning) {
        // SDL_PollEvent return 1 when there is still events on the stack of events
        // It takes the first event in the queue and writes it in the "event" parameter
        // Then it removes it from the stack.
        while (SDL_PollEvent(&event))
            handleEvent(event); // Checks each event for types and loach corresponding actions

        // Reports any possible glError
        printGlErrors();
    }
}

void Application::animate() {
}

size_t Application::frameCount() const {
    return m_FrameCount;
}

} // namespace stein
