/*
 * StreamUtils.h
 *
 *  Created on: Dec 10, 2011
 *      Author: Guillaume Chatelet
 */

#ifndef STREAMUTILS_H_
#define STREAMUTILS_H_

#include <iostream>
#include "Vector3f.hpp"
#include "Matrix4f.hpp"

std::ostream& operator<<(std::ostream&, const stein::Vector3f&);
std::ostream& operator<<(std::ostream&, const stein::Matrix4f&);

#endif /* STREAMUTILS_H_ */
