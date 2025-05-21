#ifndef BOIDSYSTEM_H
#define BOIDSYSTEM_H

#include <ngl/Vec2.h>
#include <ngl/Vec3.h>
#include "Bird.h"
#include <vector>
#include <string_view>
#include <ngl/AbstractVAO.h>
#include <memory>
#include <ngl/Mat4.h>
#include <iostream>
#include <unordered_map>

// code modified from this example
// prompt :- define a custom hash function for std::tuple<int, int, int>
// chatgpt suggested the following
/*
struct Bird;

namespace std
{
    template <>
    struct hash<std::tuple<int, int, int>>
    {
        std::size_t operator()(const std::tuple<int, int, int>& t) const
        {
            auto h1 = std::hash<int>{}(std::get<0>(t));
            auto h2 = std::hash<int>{}(std::get<1>(t));
            auto h3 = std::hash<int>{}(std::get<2>(t));
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}
*/
//Code used:

struct Bird;

template <>
struct std::hash<std::tuple<int, int, int>>
{
    std::size_t operator()(const std::tuple<int, int, int>& t) const noexcept
    {
        auto h1 = std::hash<int>{}(std::get<0>(t));
        auto h2 = std::hash<int>{}(std::get<1>(t));
        auto h3 = std::hash<int>{}(std::get<2>(t));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
//Code ends

class BoidSystem
{
public:

    BoidSystem() = default;

    explicit BoidSystem(size_t _num);
    void debug() const;
    void update();
    void render() const;
    float envSize = 35.0f;
    int nTails = 49; // Needs to be a rational number

    void setMinSpeed(int _value);
    void setMaxSpeed(int _value);
    void setMaxForce(int _value);
    void setVisualRangeDistance(int _value);
    void setVisualRangeAngle(int _value);
    void setAlignmentForce(int _value);
    void setSeparationForce(int _value);
    void setCohesionForce(int _value);
    void setEnvSize (int _value);
    void setNParticles (int _value);

    int nBirds{};

    double tailSize = sqrt(pow((envSize * 2), 2) / nTails);

    std::vector<Bird>& getMutableBirds();
    const std::vector<Bird>& getBirds() const;

    void resetParticles(Bird &io_b);


private:



    std::vector<Bird> m_birds;
    ngl::Vec3 randomVecInsideCube(float envSize);

    // code modified from this example
    // prompt :- create a spatial partitioning system based on tiling in c++
    // chatgpt suggested the following(see the rest of the code in BoidSystem.cpp)
    /*
    std::unordered_map<std::tuple<int, int, int>, std::vector<Bird*>> m_grid;
    std::tuple<int, int, int> tailIndex(const ngl::Vec3 &pos, float envSize, int nTails) const;
    std::vector<Bird*> getNeighbors(const Bird &b, float neighborRadius) const;
    */
    //Code used:
    std::unordered_map<std::tuple<int, int, int>, std::vector<Bird*>> m_grid;
    static std::tuple<int, int, int> tailIndex(const Bird &io_b, float envSize, int nTails, double tailSize);
    std::vector<Bird*> getNeighbors(const Bird &io_b, float neighborRadius) const;
    //Code ends



    std::unique_ptr<ngl::AbstractVAO> m_vao;
    ngl::Mat4 m_view;
    ngl::Mat4 m_project;


};


#endif