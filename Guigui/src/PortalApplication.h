/*
 * PortalApplication.h
 *
 *  Created on: Dec 10, 2011
 *      Author: Guillaume Chatelet
 */

#ifndef PortalApplication_H_
#define PortalApplication_H_

//#include "Boids.h"

#include <stein/Application.hpp>

class PortalApplication : public stein::Application {
public:
    PortalApplication();
    virtual void animate();
private:
    //BoidSimulation m_Simulation;
};

#endif /* PortalApplication_H_ */
