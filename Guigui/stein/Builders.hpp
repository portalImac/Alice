// Builders.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#ifndef __BUILDERS_HPP__
#define __BUILDERS_HPP__

namespace stein {

class Object; //Forward declaration

// Building functions
void buildTriangle(Object &object);
void buildSquare(Object &object, const float side = 0.5);
void buildSphere_TrFlatRed(Object &object, const float radius, const unsigned int discLat, const unsigned int discLong);
bool buildObjectGeometryFromOBJ(Object &object, const char* fileName, bool smoothObject);

} // namespace stein

#endif
