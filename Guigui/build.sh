#!/bin/bash
mkdir -p bin
echo "Compiling boids, this will take some time"
g++ src/*.cpp stein/*.cpp stein/glew/*.c -I. -lGL -lSDL -O3 -Wall -o bin/portal
echo "done ! Now go in the bin directory and launch ./portal"
