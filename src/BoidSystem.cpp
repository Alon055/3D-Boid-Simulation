#ifndef BOIDSYSTEM_CPP
#define BOIDSYSTEM_CPP

#include "BoidSystem.h"
#include "NGLScene.h"
#include "Bird.h"
#include <iostream>
#include <ngl/Random.h>
#include <ngl/Vec3.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/VAOFactory.h>

///This code was originally written by Jon Macey
BoidSystem::BoidSystem(size_t _num): m_grid()
{
    nBirds = static_cast<int>(_num);
    m_birds.resize(_num);
    for (auto& b : m_birds)
    {
        resetParticles(b);
    }
    m_vao = ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_POINTS);
}

///End of code by Jon Macey



void BoidSystem::debug() const
{
    for (auto b: m_birds)
    {
        std::cout << b.pos.m_x << ' ' << b.pos.m_y << ' ' << b.pos.m_z << "\n";
        std::cout << b.vel.m_x << ' ' << b.vel.m_y << ' ' << b.vel.m_z << "\n";
        std::cout << b.acc.m_x << ' ' << b.acc.m_y << ' ' << b.acc.m_z << "\n";
        std::cout << b.desiredVel.m_x << ' ' << b.desiredVel.m_y << ' ' << b.desiredVel.m_z << "\n";
        std::cout << b.color.m_x << ' ' << b.color.m_y << ' ' << b.color.m_z << "\n";
        std::cout << b.maxSpeed << ' ' << b.maxForce <<"\n";
    }
}

// code modified from this example
// prompt :- create a spatial partitioning system based on tiling in c++, based on my code
// chatgpt suggested the following(see the rest of the code in BoidSystem.h)
/*

// Returns the 3D grid index (tile coordinates) of a position within a partitioned environment
std::tuple<int, int, int> BoidSystem::tailIndex(const ngl::Vec3 &pos, float envSize, int nTails) const
{
    float cellSize = envSize / static_cast<float>(nTails);
    int x = static_cast<int>(pos.m_x / cellSize);
    int y = static_cast<int>(pos.m_y / cellSize);
    int z = static_cast<int>(pos.m_z / cellSize);
    return std::make_tuple(x, y, z);
}

// Finds and returns a list of neighboring birds within a given radius around a specified bird
std::vector<Bird*> BoidSystem::getNeighbors(const Bird &b, float neighborRadius) const
{
    std::vector<Bird*> neighbors;
    auto center = tailIndex(b.m_position, envSize, nTails);

    int cx, cy, cz;
    std::tie(cx, cy, cz) = center;

    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dz = -1; dz <= 1; ++dz)
            {
                auto cell = std::make_tuple(cx + dx, cy + dy, cz + dz);
                auto it = m_grid.find(cell);
                if (it != m_grid.end())
                {
                    for (Bird* other : it->second)
                    {
                        if (other != &b && (b.m_position - other->m_position).length() < neighborRadius)
                        {
                            neighbors.push_back(other);
                        }
                    }
                }
            }
        }
    }

    return neighbors;
}


*/
//Code used:

// Returns the 3D grid index (tile coordinates) a bird belongs to
std::tuple<int, int, int> BoidSystem::tailIndex(const Bird &io_b, float envSize, int nTails, double tailSize)
{
    const ngl::Vec3& pos = io_b.pos;

    int x = static_cast<int>(pos.m_x  / tailSize);
    int y = static_cast<int>(pos.m_y  / tailSize);
    int z = static_cast<int>(pos.m_z  / tailSize);
    return std::make_tuple(x, y, z);
}

// Finds and returns a list of neighboring birds within a given radius around a specified bird
std::vector<Bird*> BoidSystem::getNeighbors(const Bird &io_b, float neighborRadius) const
{
    std::vector<Bird*> neighbors;
    auto center = tailIndex(io_b, envSize, nTails, tailSize);

    int cx, cy, cz;
    std::tie(cx, cy, cz) = center;

    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dz = -1; dz <= 1; ++dz)
            {
                auto cell = std::make_tuple(cx + dx, cy + dy, cz + dz);
                auto it = m_grid.find(cell);
                if (it != m_grid.end())
                {
                    for (Bird* other : it->second)
                    {
                        if (other != &io_b && (io_b.pos - other->pos).length() < neighborRadius)
                        {
                            neighbors.push_back(other);
                        }
                    }
                }
            }
        }
    }

    return neighbors;
}
//Code ends


// code modified from this example
// prompt :- improve the logic in my update function:
// void BoidSystem::update()
// {
//     m_grid.clear();
//     for (auto &b: m_birds)
//     {
//         auto index = tailIndex(b, envSize, nTails, tailSize);
//         m_grid[index].push_back(&b);
//         std::vector<Bird*> neighbors;
//
//         if (!m_grid[index].empty()) // Check if there are any birds in the tail
//         {
//             for (auto* neighbor : m_grid[index])  // Loop through birds in the same tail
//             {
//                 if (b.isInFOV(neighbor->pos)) // Check if the neighbor is in FOV
//                 {
//                     neighbors.push_back(neighbor);
//                 }
//             }
//
//         }else // If no birds in FOV go in a random direction
//         {
//             ngl::Vec3 acceleration = randomVecInsideCube(envSize);
//             b.applyForce(acceleration);
//             b.update();
//         }
//
//
//     }
// }
// chatgpt suggested the following
/*
void BoidSystem::update()
{
    m_grid.clear(); // Clear the grid before updating

    for (auto &b : m_birds)
    {
        // Get the index of the bird's grid cell
        auto index = tailIndex(b, envSize, nTails, tailSize);

        std::vector<Bird*> neighbors;

        // Find neighbors in same tail and within FOV
        for (auto* neighbor : m_grid[index])
        {
            if (&b != neighbor && b.isInFOV(neighbor->pos))
            {
                neighbors.push_back(neighbor);
            }
        }

        // If no birds in FOV, wander randomly
        if (neighbors.empty())
        {
            ngl::Vec3 acceleration = randomVecInsideCube(envSize);
            b.applyForce(acceleration);
            b.update();
        }
        else
        {
            // Otherwise, perform behavior (e.g., alignment, cohesion, separation)
            // You would add your alignment, cohesion, and separation forces here
            // Example (alignment):
            ngl::Vec3 alignmentForce = b.alignment(neighbors);
            b.applyForce(alignmentForce);
            b.update();
        }
    }
}
*/
//Code used:

void BoidSystem::update()
{
    m_grid.clear();

    // Reassign birds to grid
    for (auto& b : m_birds)
    {
        auto index = tailIndex(b, envSize, nTails, tailSize);
        m_grid[index].push_back(&b);
    }

    for (auto& b : m_birds)
    {
        auto index = tailIndex(b, envSize, nTails, tailSize);

        std::vector<Bird*> neighbors;

        // Find neighbors in same tail and within FOV
        for (auto* neighbor : m_grid[index])
        {
            if (&b != neighbor && b.isInFOV(neighbor->pos))
            {
                neighbors.push_back(neighbor);
            }
        }

        if (neighbors.empty())
        {
            // If there is no neighbour, wander randomly
            ngl::Vec3 acceleration = b.vel + randomVecInsideCube(0.1f);
            acceleration.normalize();
            acceleration *= b.minSpeed;
            b.applyForce(acceleration);
        }
        else
        {
            // Steering behavior
            ngl::Vec3 alignmentForce = b.alignment(neighbors) * b.alignmentForce;
            ngl::Vec3 cohesionForce  = b.seek(b.cohesion(neighbors)) * b.cohesionForce;
            ngl::Vec3 separationForce = b.separation(neighbors) * b.separationForce;

            b.applyForce(cohesionForce);
            b.applyForce(alignmentForce);
            b.applyForce(separationForce);
        }

        // Apply boundary force
        b.applyForce(b.boundaries(b.envOffset, envSize));
        b.update();
    }
}

//Code ends

void BoidSystem::resetParticles(Bird &io_b)
{
    //Emit particles inside a [-10,10] cube
    io_b.pos = randomVecInsideCube(envSize);
    io_b.vel = randomVecInsideCube(envSize);

}

//Function to return a random vector inside a cube that spans from -envSize to +envSize on each axis
ngl::Vec3 BoidSystem::randomVecInsideCube(float envSize)
{
    return {
            ngl::Random::randomNumber(envSize),
            ngl::Random::randomNumber(envSize),
            ngl::Random::randomNumber(envSize)};
}


///This code was originally written by Jon Macey
void BoidSystem::render() const
{
    m_vao->bind();
    m_vao->setData(ngl::AbstractVAO::VertexData(
            m_birds.size() * sizeof(Bird),
            m_birds[0].pos.m_x
        ));
    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(Bird),0);
    m_vao->setVertexAttributePointer(1,3,GL_FLOAT,sizeof(Bird),6);

    m_vao->setNumIndices(m_birds.size());
    m_vao->draw();
    m_vao->unbind();
}
///End of code by Jon Macey

// code modified from this example
// prompt :- how can I create a GUI widget class with qt to control the values in Bird.h in real time?
// chatgpt suggested the following(see the rest of the code in BoidControlPanel.cpp, BoidControlPanel.h, main.cpp and NGLScene.h)
/*
// Provide mutable reference access to the vector of Bird objects
std::vector<Bird>& BoidSystem::getMutableBirds()
{
    return m_birds; // Allows modification of m_birds
}

// Provide read-only access to the vector of Bird objects
const std::vector<Bird>& BoidSystem::getBirds() const
{
    return m_birds; // Prevent direct modification of m_birds
}
*/
//Code used:
std::vector<Bird>& BoidSystem::getMutableBirds()
{
    return m_birds; // Allows modification of m_birds
}

// Provide read-only access to the vector of Bird objects
const std::vector<Bird>& BoidSystem::getBirds() const
{
    return m_birds; // Prevent direct modification of m_birds
}
//Code ends


void BoidSystem::setMinSpeed(int _value)
{
    float newMinSpeed = _value / 100.0f;

    // Update the minSpeed of all birds
    for (auto& b : m_birds)
    {
        b.minSpeed = newMinSpeed;
    }
}

void BoidSystem::setMaxSpeed(int _value)
{
    float newMaxSpeed = _value / 100.0f;

    // Update the maxSpeed of all birds
    for (auto& b : m_birds)
    {
        b.maxSpeed = newMaxSpeed;
    }

}

void BoidSystem::setMaxForce(int _value)
{
    float newMaxForce = _value / 1000.0f;

    // Update the maxForce of all birds
    for (auto& b : m_birds)
    {
        b.maxForce = newMaxForce;
    }
}

void BoidSystem::setVisualRangeDistance(int _value)
{
    float newVisualRangeDistance = _value / 10.0f;

    // Update the visualRangeDistance of all birds
    for (auto& b : m_birds)
    {
        b.visualRangeDistance = newVisualRangeDistance;
    }
}

void BoidSystem::setVisualRangeAngle(int _value)
{
    float newVisualRangeAngle = _value;

    // Update the visualRangeAngle of all birds
    for (auto& b : m_birds)
    {
        b.visualRangeAngle = newVisualRangeAngle;
    }

}

void BoidSystem::setAlignmentForce(int _value)
{
    float newAlignmentForce = _value / 100.0f;

    // Update the alignmentForce for all birds
    for (auto& b : m_birds)
    {
        b.alignmentForce = newAlignmentForce;
    }
}

void BoidSystem::setSeparationForce(int _value)
{
    float newSeparationForce = _value / 100.0f;

    // Update the separationForce for all birds
    for(auto& b : m_birds)
    {
        b.separationForce = newSeparationForce;
    }
}

void BoidSystem::setCohesionForce(int _value)
{
    float newCohesionForce = _value / 100.0f;

    // Update the cohesionForce for all birds
    for (auto& b : m_birds)
    {
        b.cohesionForce = newCohesionForce;
    }
}

void BoidSystem::setEnvSize(int _value)
{
    envSize = _value;
}

void BoidSystem::setNParticles(int _value)
{
    nBirds = _value;
    m_birds.resize(_value);
    for (auto& b : m_birds)
    {
        resetParticles(b);
    }
}

#endif






