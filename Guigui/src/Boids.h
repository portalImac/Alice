/*
 * Boids.h
 *
 *  Created on: Dec 8, 2011
 *      Author: Guillaume Chatelet
 */

#ifndef BOIDS_H_
#define BOIDS_H_

#include <stein/math/Vector3f.hpp>
#include <vector>

/**
 * A boid with a random position and null velocity by default
 */
struct Boid {
    Boid();
    stein::Vector3f position, velocity;
};

/**
 * The whole simulation
 * Call update to perform a simulation step and use the
 * boids() method to update the display
 */
class BoidSimulation {
public:
    BoidSimulation(std::size_t boids);

    void update();

    const std::vector<Boid>& boids() const;
private:
    stein::Vector3f cohesionForce(Boid boid) const;
    stein::Vector3f alignementForce(Boid boi) const;
    stein::Vector3f separationForce(Boid boid) const;

    std::vector<Boid> m_Boids;
};

#endif /* BOIDS_H_ */
