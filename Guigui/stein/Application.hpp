// Application.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include "Scene.hpp"

#include "GLHeaders.hpp"

// Windowing system SDL
#include <SDL/SDL.h>

namespace stein {

// All the initialization of states and events for SDL and OpenGL
class Application {
public:
    Application(size_t width, size_t height);
    virtual ~Application();

    void loop();

protected:
    Scene m_Scene;

    void resize(GLuint w, GLuint h);
    void setBackgroundColor(const Color &color);
    void printFPS();

    virtual void renderFrame();
    virtual void animate();

    virtual void handleUserEvent(const SDL_Event& event);
    virtual void handleKeyEvent(const SDL_keysym& keysym, bool down);
    virtual void handleEvent(const SDL_Event& event);

    size_t frameCount() const;

private:
    void customizeStates();
    void initSDLOpenGL();
    void initTimers();

    SDL_TimerID animateTimer; // Timer for the animation

    SDL_Surface* m_pDrawContext;

    size_t windowedWidth; // Window dimensions when not fullscreen - horizontal axis
    size_t windowedHeight; // Window dimensions when not fullscreen - vertical axis
    size_t fullScreenWidth; // Screen dimensions - horizontal axis
    size_t fullScreenHeight; // Screen dimensions - vertical axis
    size_t width; // Window actual dimensions - horizontal axis
    size_t height; // Window actual dimensions - vertical axis

    const uint32_t videoModeFlags;

    GLfloat m_MouseXPos; // Mouse position - horizontal axis (-1 to 1)
    GLfloat m_MouseYPos; // mouse position - vertical axis (-1 to 1)
    GLfloat m_PressedMouseXPos; // Mouse position - updated only when left button down
    GLfloat m_PressedMouseYPos; // mouse position - updated only when left button down
    int m_MouseScroll; // scroll value (up : ++, down : --)
    bool m_bRunning; // True when the window is closed to end the application
    bool m_bShowMouse; // True if mouse is seeable
    GLuint m_FrameCount; // Frame counter
    uint64_t m_LastStartTime; // Time updated every 10 frames
    uint64_t m_FrameDuration; // Frame time duration
};

} // namespace stein

#endif //__APPLICATION_HPP__ 
