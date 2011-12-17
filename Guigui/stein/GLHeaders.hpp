// GLHeaders.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#ifndef __GLHEADERS_HPP__
#define __GLHEADERS_HPP__


// Windows
#ifdef WIN32
#include <windows.h>		// Must have for Windows platform builds
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // WIN32

#include "glew\glew.h"			// OpenGL Extension "autoloader"
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#endif

// Mac OS X
#ifdef __APPLE__
#include <stdlib.h>

#include <TargetConditionals.h>
#include <mach/clock.h>
#include <mach/mach.h>
#if TARGET_OS_IPHONE | TARGET_IPHONE_SIMULATOR
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define OPENGL_ES
#else
//#include <GL/glew.h> // because we want the program to choose to provided Glew, 
					   // not the one possibly installed on the system
#include "glew/glew.h"
#include <OpenGL/gl.h>		// Apple OpenGL haders (version depends on OS X SDK version)
#endif
#endif

// Linux
#ifdef linux
#define GLEW_STATIC
//#include <glew.h> // because we want the program to choose to provided Glew, 
					   // not the one possibly installed on the system
#include "glew/glew.h"
#include <GL/gl.h>
#endif

//////////////////////// TEMPORARY TEMPORARY TEMPORARY - On SnowLeopard this is suppored, but GLEW doens't hook up properly
//////////////////////// Fixed probably in 10.6.3
#ifdef __APPLE__
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glDeleteVertexArrays  glDeleteVertexArraysAPPLE
#define glBindVertexArray	glBindVertexArrayAPPLE
#ifndef OPENGL_ES
#define glGenerateMipmap    glGenerateMipmapEXT
#endif
#endif

#endif // __GLHEADERS_HPP__
