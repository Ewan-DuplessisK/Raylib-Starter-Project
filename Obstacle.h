#pragma once
#include <raylib.h>

class Obstacle
{
public:
    Obstacle(float xP,float yP,float widthP, float heightP):rec(Rectangle{xP,yP,widthP,heightP}){}
    ~Obstacle(){}
    Rectangle getRectangle(){return rec;}

private:
    Rectangle rec;
};
