#ifndef BIRD_CPP
#define BIRD_CPP

#include "BoidSystem.h"
#include "Bird.h"
#include <cmath>
#include <ngl/Vec3.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <vector>

// Function to check if candidate bird is in the field of view of current birdconst Bird &io_b,
bool Bird::isInFOV(const ngl::Vec3& candidate) const
{
    ngl::Vec3 directionToCandidate = candidate - pos;
    float distanceToTarget = directionToCandidate.length();

    if(distanceToTarget < visualRangeDistance)
    {
        ngl::Vec3 forward = vel;
        forward.normalize();
        directionToCandidate.normalize();
        float dotProduct = forward.dot(directionToCandidate);

        // The inverse cosine of the dot product between the velocity(direction) vector of the bird(vel) and the
        // direction vector to the candidate(directionToCandidate) gives you the angle between those vectors

        float angleRad = std::acos(dotProduct); // Radians
        float angle = angleRad * (180/ M_PI);

        if (angle < visualRangeAngle)
        {
            return true;
        }else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


// Apply a steering force on bird towards a target position (once the target has been found)
ngl::Vec3 Bird::seek(const ngl::Vec3& target) const
{
    ngl::Vec3 desiredVel = target - pos;

    // If distance between birds is less than distanceCoefficient the visualRangeDistance, set speed to minimum
    if(desiredVel.length() < (visualRangeDistance * distanceCoefficient))
    {
        desiredVel *= minSpeed;
    }else
    {
        desiredVel.normalize();
        desiredVel *= maxSpeed;
    }

    ngl::Vec3 steeringForce = desiredVel - vel;

    // If the steeringForce magnitude is greater than the max force, cap it at maxForce
    if(steeringForce.length() > maxForce)
    {
        steeringForce.normalize();
        steeringForce *= maxForce;
    }

    return steeringForce;
}



// Function to steer the birds towards the average direction(vel) of its neighbours
ngl::Vec3 Bird::alignment(const std::vector<Bird*>& targets) const
{
    ngl::Vec3 velTarget(0.0f, 0.0f, 0.0f);
    int nTargets = 0;

    for (Bird* target : targets)
    {
        velTarget += target -> vel;
        ++nTargets;
    }

    ngl::Vec3 avgVel = velTarget/static_cast<float>(nTargets);

    avgVel.normalize();
    avgVel *= maxSpeed;


    ngl::Vec3 steeringForce = avgVel - vel;
    return steeringForce;
}

// Function to steer the birds towards the center(average position) of its neighbours
ngl::Vec3 Bird::cohesion(const std::vector<Bird*>& targets) const
{
    ngl::Vec3 posTarget(0.0f, 0.0f, 0.0f);
    int nTargets = 0;

    for (Bird* target : targets)
    {
        posTarget += target -> pos;
        ++nTargets;
    }

    ngl::Vec3 avgPosTarget = posTarget/static_cast<float>(nTargets);

    return avgPosTarget;
}

// Function to steer the bird away from the average vector pointing away from nearby birds
ngl::Vec3 Bird::separation(const std::vector<Bird*>& targets) const
{
    if(targets.size() > 1)
    {
        ngl::Vec3 avgSteeringForce(0.0f, 0.0f, 0.0f);
        int nTargets = 0;

        for (Bird* target : targets)
        {
            ngl::Vec3 desiredVel = pos - target->pos;

            // If distance between birds is less than distanceCoefficient the visualRangeDistance, set speed to minimum
            if(desiredVel.length() < (visualRangeDistance * distanceCoefficient))
            {
                desiredVel *= minSpeed;
            }else
            {
                desiredVel.normalize();
                desiredVel *= maxSpeed;
            }

            ngl::Vec3 steeringForce = desiredVel - vel;

            // If the steeringForce magnitude is greater than the max force, cap it at maxForce
            if(steeringForce.length() > maxForce)
            {
                steeringForce.normalize();
                steeringForce *= maxForce;
            }

            avgSteeringForce += steeringForce;
            ++nTargets;
        }

    return (avgSteeringForce/static_cast<float>(nTargets));


    } else
    {
        Bird* target = targets[0];
        ngl::Vec3 desiredVel = pos - target->pos ;

        // If distance between birds is less than distanceCoefficient the visualRangeDistance, set speed to minimum
        if(desiredVel.length() < (visualRangeDistance * distanceCoefficient))
        {
            desiredVel *= minSpeed;
        }else
        {
            desiredVel.normalize();
            desiredVel *= maxSpeed;
        }

        ngl::Vec3 steeringForce = desiredVel - vel;

        // If the steeringForce magnitude is greater than the max force, cap it at maxForce
        if(steeringForce.length() > maxForce)
        {
            steeringForce.normalize();
            steeringForce *= maxForce;
        }

        return steeringForce;
    }
}

// Check if bird goes beyond environment. If so, apply a steering force on the opposite direction
ngl::Vec3 Bird::boundaries(float envOffset, float envSize) const
{
    ngl::Vec3 desiredVel(0.0f, 0.0f, 0.0f);

    if(pos.m_x < -envSize + envOffset)
    {
        desiredVel.set(maxSpeed ,vel.m_y, vel.m_z);
    }else if (pos.m_x > envSize - envOffset)
    {
        desiredVel.set(-maxSpeed ,vel.m_y, vel.m_z);
    }

    if(pos.m_y < -envSize + envOffset)
    {
        desiredVel.set( vel.m_x, maxSpeed, vel.m_z);
    }else if (pos.m_y > envSize - envOffset)
    {
        desiredVel.set(vel.m_x, -maxSpeed, vel.m_z);
    }

    if(pos.m_z < -envSize + envOffset)
    {
        desiredVel.set( vel.m_x, vel.m_y, maxSpeed);
    }else if (pos.m_z > envSize - envOffset)
    {
        desiredVel.set(vel.m_x, vel.m_y, -maxSpeed);
    }

    if(desiredVel != ngl::Vec3 (0.0f, 0.0f, 0.0f))
    {
        ngl::Vec3 steeringForce = desiredVel - vel;

        // If the steeringForce magnitude is greater than the max force, cap it at maxForce
        if(steeringForce.length() > maxForce)
        {
            steeringForce.normalize();
            steeringForce *= maxForce;
        }
        return steeringForce;
    } else
    {
        return ngl::Vec3 (0.0f, 0.0f, 0.0f);
    }
}




// Apply force to acceleration
void Bird::applyForce(const ngl::Vec3& force)
{
    acc += force;
}

// Update the position of the bird based on acceleration
void Bird::update()
{
    vel += acc;

    // If the velocity magnitude is greater than the max speed, cap it at maxSpeed
    if(vel.length() > maxSpeed)
    {
        vel.normalize();
        vel *= maxSpeed;
    }

    pos += vel;

    acc.null(); // Reset acceleration
}

#endif


