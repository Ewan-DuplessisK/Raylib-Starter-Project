#include "Boid.h"

#include <iostream>

std::vector<Boid*> Boid::pendingKill={};

Boid::Boid(Vector2 posP, Vector2 dirP, float speedP, float mDisP,float aDisP,float gDisP,float cDist,Color teamP,Color predatorP,Color preyP,std::array<float,7> weightsP):
position(posP),direction(dirP),speed(speedP),minimumDistance(mDisP),alignDistance(aDisP),groupDistance(gDisP),chaseDistance(cDist),team(teamP),predator(predatorP),prey(preyP), weights(weightsP){}

Vector2 Boid::separate(std::vector<Boid*> others){
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

Vector2 Boid::avoidPredator(std::vector<Boid*> others){
    Vector2 res{0.0f,0.0f};
    float nBoids=0.0f;
    for (Boid* boid : others){
        if(boid!=this){//not self
            float otherDot = Vector2DotProduct(direction,Vector2Normalize(boid->getPosition()-position));
            if(boid->getTeam()==predator && otherDot>=0 && Vector2Distance(position,boid->getPosition())<minimumDistance*3.f){ //if in front and closer than minimum
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
            nObstacles++;
            res= res*(1.0f-(1.0f/nObstacles))-
                (Vector2Normalize(
                    Vector2{obstacle->getRectangle().x + obstacle->getRectangle().width,
                        obstacle->getRectangle().y+ obstacle->getRectangle().height}-position))
            *(1.0f/nObstacles);
        }
        if(position.x<minimumDistance*1.5f){
            res = res + Vector2{1.f,0.f};
        }
        if(position.x>window_size-minimumDistance*1.5f){
            res = res + Vector2{-1.f,0.f};
        }
        if(position.y<minimumDistance*1.5f){
            res = res + Vector2{0.f,1.f};
        }
        if(position.y>window_size-minimumDistance*1.5f){
            res = res + Vector2{0.f,-1.f};
        }
        res = Vector2Normalize(res);
    }
    return res;
}

Vector2 Boid::align(std::vector<Boid*> others){
    Vector2 res{0.0f,0.0f};
    float nBoids=0.0f;
    for (Boid* boid : others){
        if(boid!=this){//not self
            float otherDot = Vector2DotProduct(direction,Vector2Normalize(boid->getPosition()-position));
            if(otherDot>=0 && Vector2Distance(position,boid->getPosition())<alignDistance){ //if in front and closer than maximum 
                nBoids++;
                res= res*(1.0f-(1.0f/nBoids))+ boid->getDirection()*(1.0f/nBoids); // scale with direction of others in alignment radius
            }
        }
    }
    return res;
    
}

Vector2 Boid::group(std::vector<Boid*> others){
    Vector2 res{0.0f,0.0f};
    float nBoids=0.0f;
    for (Boid* boid : others){
        if(boid!=this){//not self
            float otherDot = Vector2DotProduct(direction,Vector2Normalize(boid->getPosition()-position));
            if(boid->getTeam()==team && otherDot>=0 && Vector2Distance(position,boid->getPosition())<groupDistance){ //if in front and closer than maximum 
                nBoids++;
                res= res*(1.0f-(1.0f/nBoids))+ Vector2Normalize(boid->getPosition()-position)*(1.0f/nBoids); // get direction to other -> normalize -> scale to num
            }
        }
    }
    return res;
}

Vector2 Boid::chasePrey(std::vector<Boid*> others){
    Vector2 res{0.0f,0.0f};
    float nBoids=0.0f;
    for (Boid* boid : others){
        if(boid!=this){//not self
            float otherDot = Vector2DotProduct(direction,Vector2Normalize(boid->getPosition()-position));
            if(boid->getTeam()==prey && otherDot>=0 && Vector2Distance(position,boid->getPosition())<chaseDistance){ //if in front and closer than maximum 
                nBoids++;
                res= res*(1.0f-(1.0f/nBoids))+ Vector2Normalize(boid->getPosition()-position)*(1.0f/nBoids); // get direction to other -> normalize -> scale to num
                if(Vector2Distance(position,boid->getPosition())<minimumDistance*0.8f){
                    pendingKill.push_back(boid);
                }
            }
        }
    }
    return res;
}

Vector2 Boid::mouseInfluence(){
    Vector2 res{0.0f,0.0f};
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        followMouse=true;
        avoidMouse=false;
    }
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        followMouse=false;
        avoidMouse=true;
    }
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))followMouse=false;
    if(IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))avoidMouse=false;

    if(followMouse) res = Vector2Normalize(GetMousePosition()-position);
    if(avoidMouse) res = Vector2Negate(Vector2Normalize(GetMousePosition()-position));

    return res;
}

void Boid::update(std::vector<Boid*> others, std::array<Obstacle*,OBSTACLES_NUMBER> obstacles){
    Vector2 influence = separate(others)*weights[0]+ avoidObstacles(obstacles)*weights[1] + align(others)*weights[2] + group(others)*weights[3] + chasePrey(others)*weights[4] + avoidPredator(others) * weights[5] + mouseInfluence() * weights[6];
    direction = direction + influence ;
    direction = Vector2Normalize(direction);
    /*if(abs(Vector2Angle(oldDirection,direction)*(180.f/PI))>45){
        if(Vector2Angle(oldDirection,direction)*(180.f/PI)<0){
            direction = Vector2Rotate(direction,-20.f/(180.f/PI));
        }
        else direction = Vector2Rotate(direction,20.f/(180.f/PI));
    }*/
    position= position+ (direction*speed);
}
