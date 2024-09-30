﻿#pragma once
#include "Obstacle.h"
#include "Util.h"

class Boid
{
public:
    Boid(Vector2 posP, Vector2 dirP, float speedP, float mDisP,float aDisP,float gDisP);
    ~Boid(){}
    Vector2 getPosition(){return position;}
    Vector2 getDirection(){return direction;}
    void update(std::array<Boid*,BOIDS_NUMBER> others, std::array<Obstacle*,OBSTACLES_NUMBER> obstacles);

private:
    Vector2 separate(std::array<Boid*,BOIDS_NUMBER> others);
    Vector2 avoidObstacles(std::array<Obstacle*,OBSTACLES_NUMBER> obstacles);  //Obstacles hold a position and size, only rectangles for now
    Vector2 align(std::array<Boid*,BOIDS_NUMBER> others);
    Vector2 group(std::array<Boid*,BOIDS_NUMBER> others);
    Vector2 position;
    Vector2 direction;
    float speed;
    float weights[4] = {0.40,0.30,0.25,0.05};
    float minimumDistance;
    float alignDistance;
    float groupDistance;
    
};
