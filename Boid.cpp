﻿#include "Boid.h"

Boid::Boid(Vector2 posP, Vector2 dirP, float speedP,float mDisP, float aDisp, float gDisP):position(posP),direction(dirP),speed(speedP),minimumDistance(mDisP),alignDistance(aDisp),groupDistance(gDisP){}

Vector2 Boid::separate(std::array<Boid*,BOIDS_NUMBER> others){
    Vector2 res{0.0f,0.0f};
    float nBoids=0.0f;
    for (Boid* boid : others){
        if(boid==this)continue; //not self
        else{
            float otherDot = Vector2DotProduct(direction,Vector2Normalize(boid->getPosition()-position));
            if(otherDot>=0 && Vector2Distance(position,boid->getPosition())<minimumDistance){ //if in front and closer than minimum
                nBoids++;
                res= res*(1.0f-(1.0f/nBoids))+ Vector2Negate(Vector2Normalize(boid->getPosition()-position))*(1.0f/nBoids); // get direction to other -> normalize -> invert -> scale to num of too close
            }
        }
    }
    return res;
}

Vector2 Boid::avoidObstacles(std::array<Obstacle*,OBSTACLES_NUMBER> obstacles){
    Vector2 res{0.0f,0.0f};
    float nObstacles=0.0f;
    for(Obstacle* obstacle: obstacles){
        if(CheckCollisionCircleRec(position,minimumDistance,obstacle->getRectangle())){
            res= res*(1.0f-(1.0f/nObstacles))+ Vector2Negate(Vector2Normalize(Vector2{obstacle->getRectangle().x,obstacle->getRectangle().y}-position))*(1.0f/nObstacles);
        }
    }
    return res;
}

Vector2 Boid::align(std::array<Boid*,BOIDS_NUMBER> others){
    Vector2 res{0.0f,0.0f};
    float nBoids=0.0f;
    for (Boid* boid : others){
        if(boid==this)continue; //not self
        else{
            float otherDot = Vector2DotProduct(direction,Vector2Normalize(boid->getPosition()-position));
            if(otherDot>=0 && Vector2Distance(position,boid->getPosition())<alignDistance){ //if in front and closer than maximum 
                nBoids++;
                res= res*(1.0f-(1.0f/nBoids))+ boid->getDirection()*(1.0f/nBoids); // scale with direction of others in alignment radius
            }
        }
    }
    return res;
    
}

Vector2 Boid::group(std::array<Boid*,BOIDS_NUMBER> others){
    Vector2 res{0.0f,0.0f};
    float nBoids=0.0f;
    for (Boid* boid : others){
        if(boid==this)continue; //not self
        else{
            float otherDot = Vector2DotProduct(direction,Vector2Normalize(boid->getPosition()-position));
            if(otherDot>=0 && Vector2Distance(position,boid->getPosition())<groupDistance){ //if in front and closer than maximum 
                nBoids++;
                res= res*(1.0f-(1.0f/nBoids))+ Vector2Normalize(boid->getPosition()-position)*(1.0f/nBoids); // get direction to other -> normalize -> scale to num
            }
        }
    }
    return res;
}



void Boid::update(std::array<Boid*,BOIDS_NUMBER> others, std::array<Obstacle*,OBSTACLES_NUMBER> obstacles){
    Vector2 oldDirection = direction;
    direction= separate(others)*weights[0] + avoidObstacles(obstacles)*weights[1] + align(others)*weights[2] + group(others)*weights[3];
    if(abs(Vector2Angle(oldDirection,direction)*(180.f/PI))>45){
        if(Vector2Angle(oldDirection,direction)*(180.f/PI)<0){
            direction = Vector2Rotate(direction,-45.f/(180.f/PI));
        }
        else direction = Vector2Rotate(direction,45.f/(180.f/PI));
    }
    position= position+ direction*speed;
}
