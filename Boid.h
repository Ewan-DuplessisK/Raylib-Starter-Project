#pragma once
#include <memory>
#include <vector>
#include "Obstacle.h"
#include "Util.h"
#include "Grid.h"

class Boid
{
public:
    Boid(Vector2 posP, Vector2 dirP, float speedP, float mDisP,float aDisP,float gDisP,std::array<float,7> weightsP, std::vector<Node*> graph,Grid pGrid);
    ~Boid(){}
    Vector2 getPosition(){return position;}
    Vector2 getDirection(){return direction;}
    bool update(std::vector<Boid*> others, std::array<Obstacle*,OBSTACLES_NUMBER> obstacles);
    static std::vector<Boid*> pendingKill;

private:
    bool updateStep();
    Vector2 separate(std::vector<Boid*> others);
    //Vector2 avoidObstacles(std::array<Obstacle*,OBSTACLES_NUMBER> obstacles);  //Obstacles hold a position and size, only rectangles for now
    Vector2 align(std::vector<Boid*> others);
    Vector2 group(std::vector<Boid*> others);
    //Vector2 mouseInfluence();
    Vector2 moveTowardsTarget(Vector2 target);
    Vector2 stayOnRoad();
    Vector2 position;
    Vector2 direction;
    float speed;
    std::array<float,7> weights= {1.f,1.f,1.f,1.f,1.f,1.f,1.f};
    float minimumDistance;
    float alignDistance;
    float groupDistance;
    //bool followMouse = false;
    //bool avoidMouse = false;
    Vector2 signalDirection;
    Node start;
    Node goal;
    Path path;
    Path rPath;
    std::vector<Node*>bGraph;
    Vector2 currRoad[2];
    Grid currGrid;
    
};
