#ifndef BIRD_H
#define BIRD_H

#include <ngl/Vec2.h>
#include <ngl/Vec3.h>

struct Bird
{
    ngl::Vec3 pos;
    ngl::Vec3 vel;
    ngl::Vec3 acc; // Rate of change of velocity not speed
    ngl::Vec3 color = {0.0f,0.0f,0.0f};



    ngl::Vec3 desiredVel;
    float minSpeed = 0.1f;
    float maxSpeed = 0.25f;
    float maxForce = 0.01f;
    float visualRangeDistance = 3.5f;
    float visualRangeAngle = 240.0f; // 240.0f;
    //float size = 0.5f;
    float envOffset = 3.0f;

    float distanceCoefficient = 0.05f;

    float alignmentForce = 0.5f;
    float separationForce = 0.5f;
    float cohesionForce = 0.4f;

    bool isInFOV(const ngl::Vec3& candidate) const;//Field of view(FOV)

    ngl::Vec3 alignment(const std::vector<Bird*>& targets) const;
    ngl::Vec3 separation(const std::vector<Bird*>& neighbours) const;
    ngl::Vec3 cohesion(const std::vector<Bird*>& target) const;
    ngl::Vec3 boundaries(float envOffset, float envSize) const;

    void update();
    void applyForce(const ngl::Vec3& force);
    ngl::Vec3 seek(const ngl::Vec3& target) const;

};

#endif