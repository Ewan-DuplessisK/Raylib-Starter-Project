#include "Boid.h"
#include "Grid.h"
#include <iostream>

std::vector<Boid*> Boid::pendingKill={};

Boid::Boid(Vector2 posP, Vector2 dirP, float speedP, float mDisP,float aDisP,float gDisP,std::array<float,7> weightsP,std::vector<Node*> graph,Grid pGrid):
position(posP),direction(dirP),speed(speedP),minimumDistance(mDisP),alignDistance(aDisP),groupDistance(gDisP), weights(weightsP),currGrid(pGrid){
    //calculate route //if non changing destination && no runtime road change
    bGraph.emplace_back(start);
    bGraph.emplace_back(graph);
    bGraph.emplace_back(goal);
    path = currGrid.solveAStar(bGraph);
    bGraph.at(0) = &goal;
    bGraph.at(bGraph.size() - 1) = &start;
    rPath = currGrid.solveAStar(bGraph);
    
}

Vector2 Boid::stayOnRoad() {
    if (FloatEquals(currRoad[0].x, currRoad[1].x)) {//vertical road
        if (direction.x > 0.f) {//going right
            if (abs(currRoad[0].x - position.x) < abs((direction * speed).x * 2.f)) {//if would hit right wall within 2 ticks
                return Vector2{ -1.0f / (abs(currRoad[0].x - position.x) / abs((direction * speed).x)),0.f };//vector left scaled by closeness [1.0 -> 2.0]
            }
        }
        else {//going left
            if (abs(currRoad[0].x - position.x) < abs((direction * speed).x * 2.f)) {//if would hit left wall within 2 ticks
                return Vector2{ 1.0f / (abs(currRoad[0].x - position.x) / abs((direction * speed).x)),0.f };//vector right scaled by closeness [1.0 -> 2.0]
            }
        }
        
    }
    else {//horizontal road
        if (direction.y > 0.f) {//going down
            if (abs(currRoad[0].y - position.y) < abs((direction * speed).y * 2.f)) {//if would hit down wall within 2 ticks
                return Vector2{0.f, -1.0f / (abs(currRoad[0].y - position.y) / abs((direction * speed).y)) };//vector up scaled by closeness [1.0 -> 2.0]
            }
        }
        else {//going up
            if (abs(currRoad[0].y - position.y) < abs((direction * speed).y * 2.f)) {//if would hit up wall within 2 ticks
                return Vector2{0.f, 1.0f / (abs(currRoad[0].y - position.y) / abs((direction * speed).y)) };//vector down scaled by closeness [1.0 -> 2.0]
            }
        }
    }
}

bool Boid::updateStep(){
    if(CheckCollisionCircleRec(position,5.f,goal.coll)){
        path.step++;
        
        if(path.step==path.path.size()-1){//reached last
            return true;
        }
        if(FloatEquals(path.path[path.step]->mapPos.x,path.path[path.step+1]->mapPos.x)){//"bounding box" of the road
            if(path.path[path.step]->mapPos.y>path.path[path.step+1]->mapPos.y){
                currRoad[0]=path.path[path.step+1]->mapPos+Vector2{-5.f,-5.f};
                currRoad[1]=path.path[path.step]->mapPos+Vector2{5.f,5.f};
            }
            else{
                currRoad[0]=path.path[path.step]->mapPos+Vector2{-5.f,-5.f};
                currRoad[1]=path.path[path.step+1]->mapPos+Vector2{5.f,5.f};
            }
        }else{
            if(path.path[path.step]->mapPos.x>path.path[path.step+1]->mapPos.x){
                currRoad[0]=path.path[path.step+1]->mapPos+Vector2{-5.f,-5.f};
                currRoad[1]=path.path[path.step]->mapPos+Vector2{5.f,5.f};
            }
            else{
                currRoad[0]=path.path[path.step]->mapPos+Vector2{-5.f,-5.f};
                currRoad[1]=path.path[path.step+1]->mapPos+Vector2{5.f,5.f};
            }
        }
    }
    return false;
}

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

/*Vector2 Boid::avoidPredator(std::vector<Boid*> others){
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
}*/

/*Vector2 Boid::avoidObstacles(std::array<Obstacle*,OBSTACLES_NUMBER> obstacles){
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
}*/

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
            float otherDot = Vector2DotProduct(direction,boid->getDirection());
            if(otherDot>=0 && Vector2Distance(position,boid->getPosition())<groupDistance){ //if other going roughly in the same direction and closer than maximum 
                nBoids++;
                res= res*(1.0f-(1.0f/nBoids))+ Vector2Normalize(boid->getPosition()-position)*(1.0f/nBoids); // get direction to other -> normalize -> scale to num
            }
        }
    }
    return res;
}

/*Vector2 Boid::chasePrey(std::vector<Boid*> others){
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
}*/

/*Vector2 Boid::mouseInfluence(){
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
}*/

Vector2 Boid::moveTowardsTarget(Vector2 target){
    return Vector2Normalize(target-position);
}

bool Boid::update(std::vector<Boid*> others, std::array<Obstacle*,OBSTACLES_NUMBER> obstacles){
    bool end = updateStep();
    if(end){
        
    }
    else{
        Vector2 influence = moveTowardsTarget(goal.mapPos) + stayOnRoad() + separate(others);
        direction = direction + influence;
        direction = Vector2Normalize(direction);
        position = position + (direction * speed);
    }
    //Vector2 influence = separate(others)*weights[0]+ avoidObstacles(obstacles)*weights[1] + align(others)*weights[2] + group(others)*weights[3] + /*chasePrey(others)*weights[4] + avoidPredator(others) * weights[5] + mouseInfluence() * weights[6]*/ moveTowardsTarget();
    //direction = direction + influence ;
    //direction = Vector2Normalize(direction);
    /*if(abs(Vector2Angle(oldDirection,direction)*(180.f/PI))>45){
        if(Vector2Angle(oldDirection,direction)*(180.f/PI)<0){
            direction = Vector2Rotate(direction,-20.f/(180.f/PI));
        }
        else direction = Vector2Rotate(direction,20.f/(180.f/PI));
    }*/
    //position= position+ (direction*speed);
    return end;
}
