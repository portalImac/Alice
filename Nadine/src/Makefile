# Makefile
# Template for OpenGL 3.*
# Nadine Dommanget dommange@univ-mlv.fr




MACHINE = $(shell uname -s)

ifeq ($(MACHINE),Darwin)
	OPENGL_INC = -FOpenGL
	OPENGL_LIB = -framework OpenGL -lm
	SDL_INC = `sdl-config --cflags`
	SDL_LIB = `sdl-config --libs`
	#CXX=g++-mp-4.5
	CXX=g++
else
	OPENGL_INC = -I/usr/X11R6/include
	OPENGL_LIB = -I/usr/lib64 -lGL -lGLU -lm -lrt
	#OPENGL_LIB = -lGL -lGLU -lm
	SDL_INC = -I/usr/include/SDL/
	SDL_LIB = `sdl-config --libs`
	CXX=g++
endif


COMPILER_FLAGS=-g -I.
COMPILER_FLAGS_WARN=-g -Wall -I.

EXE=portal

INCLUDE= $(SDL_INC) $(OPENGL_INC) -I../api
LIBS= $(SDL_LIB) $(OPENGL_LIB)

OBJS = Object.o Builders.o Camera.o Scene.o Application.o Tools.o main.o glew.o

all : $(EXE) 

$(EXE) : $(OBJS)
	$(CXX) $(COMPILER_FLAGS_WARN) $^ $(LIBS) -o $@

-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CXX) -c $(COMPILER_FLAGS_WARN) $(INCLUDE) $<
	@$(CXX)  -MM $(INCLUDE) $< > $*.d

# Glew :
glew.o : glew/glew.c glew/glew.h
	$(CXX) -c $(COMPILER_FLAGS) $(INCLUDE) $<

.PHONY : clean ultraclean

clean::
	rm -f *.o *~ *.d *.oD 

bigclean : clean
	rm -f $(EXE) $(LIB)
	rm -f ../plugins/*.so
