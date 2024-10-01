#pragma once
#include <array>
#include "raylib.h"
#include "raymath.h"

#define BOIDS_NUMBER 33
#define OBSTACLES_NUMBER 4

static Vector2 operator+ (Vector2 left,Vector2 right){
    return Vector2Add(left,right);
}
static Vector2 operator* (Vector2 left,float right){
    return Vector2Scale(left,right);
}
static Vector2 operator- (Vector2 left,Vector2 right){
    return Vector2Subtract(left,right);
}

static bool operator== (Color left, Color right){
    return left.a==right.a && left.b==right.b && left.g==right.g &&left.b==right.b;
}

