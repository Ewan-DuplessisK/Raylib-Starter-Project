#pragma once
#include <memory>
#include <vector>
#include "Obstacle.h"
#include "Util.h"

class Boid
{
public:
    Boid(Vector2 posP, Vector2 dirP, float speedP, float mDisP,float aDisP,float gDisP,float cDist,Color teamP,Color predatorP,Color preyP,std::array<float,7> weightsP);
    ~Boid(){}
    Vector2 getPosition(){return position;}
    Vector2 getDirection(){return direction;}
    Color getTeam(){return team;}
    Color getPredator(){return predator;}
    Color getPrey(){return prey;}
    void update(std::vector<Boid*> others, std::array<Obstacle*,OBSTACLES_NUMBER> obstacles);
    static std::vector<Boid*> pendingKill;

private:
    Vector2 separate(std::vector<Boid*> others);
    Vector2 avoidObstacles(std::array<Obstacle*,OBSTACLES_NUMBER> obstacles);  //Obstacles hold a position and size, only rectangles for now
    Vector2 align(std::vector<Boid*> others);
    Vector2 group(std::vector<Boid*> others);
    Vector2 chasePrey(std::vector<Boid*> others);
    Vector2 avoidPredator(std::vector<Boid*> others);
    Vector2 mouseInfluence();
    Vector2 moveTowardsTarget(Vector2 target);
    Vector2 position;
    Vector2 direction;
    float speed;
    std::array<float,7> weights= {1.f,1.f,1.f,1.f,1.f,1.f,1.f};
    float minimumDistance;
    float alignDistance;
    float groupDistance;
    float chaseDistance;
    bool followMouse = false;
    bool avoidMouse = false;
    Color team;
    Color predator;
    Color prey;
    
};
