/*
 * Boids.cpp
 *
 *  Created on: Dec 8, 2011
 *      Author: Guillaume Chatelet
 */

#include "Boids.h"
#include <cmath>
#include <cstdlib>

using namespace std;
using namespace stein;

static inline float frand() {
    return float(rand()) / RAND_MAX;
}

Boid::Boid() :
    position(frand(), frand(), frand()) {
}

BoidSimulation::BoidSimulation(size_t boids) :
    m_Boids(boids) {
}

Vector3f BoidSimulation::cohesionForce(Boid boid) const {
    Vector3f center;
    for (size_t i = 0; i < m_Boids.size(); ++i) {
        const Boid &current = m_Boids[i];
        if (&boid == &current)
            continue;
        center += current.position;
    }assert(m_Boids.size() > 1);
    center /= m_Boids.size() - 1;
    center -= boid.position;
    return center;
}

Vector3f BoidSimulation::alignementForce(Boid boid) const {
    Vector3f velocity;
    for (size_t i = 0; i < m_Boids.size(); ++i) {
        const Boid &current = m_Boids[i];
        if (&boid == &current)
            continue;
        velocity += current.velocity;
    }assert(m_Boids.size() > 1);
    velocity /= m_Boids.size() - 1;
    velocity -= boid.velocity;
    return velocity;
}

Vector3f BoidSimulation::separationForce(Boid boid) const {
    Vector3f centroid;
    for (size_t i = 0; i < m_Boids.size(); ++i) {
        const Boid &current = m_Boids[i];
        if (&boid == &current)
            continue;
        const Vector3f delta(current.position - boid.position);
        if (delta.norm() < 3)
            centroid -= delta;
    }
    return centroid;
}

const std::vector<Boid>& BoidSimulation::boids() const {
    return m_Boids;
}

static inline void limitSpeed(Vector3f &speed) {
    const float speedLimit = 1;
    const float norm = speed.norm();
    if (norm < speedLimit || norm == 0)
        return;
    speed /= norm;
    speed *= speedLimit;
}

static inline void limitPosition(float &value, float &speed) {
    const float positionLimit = 20;
    const float bumpForce = .1;
    if (value > positionLimit)
        speed = -bumpForce;
    else if (value < -positionLimit)
        speed = bumpForce;
}

static inline void limitPosition(Vector3f &pos, Vector3f &speed) {
    limitPosition(pos.x, speed.x);
    limitPosition(pos.y, speed.y);
    limitPosition(pos.z, speed.z);
}

void BoidSimulation::update() {
    if (m_Boids.size() < 2)
        return;
    for (size_t i = 0; i < m_Boids.size(); ++i) {
        Boid &current = m_Boids[i];
        Vector3f &velocity = current.velocity;
        Vector3f &position = current.position;

        Vector3f oldVelocity = velocity;
        velocity += cohesionForce(current) / 100 + alignementForce(current) / 16 + separationForce(current) / 100;

        limitPosition(position, velocity);
        limitSpeed(velocity);

        position += (velocity + oldVelocity) * .5;
    }
}
