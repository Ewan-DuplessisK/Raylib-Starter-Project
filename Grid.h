#pragma once
#include <map>
#include <raylib.h>
#include <vector>

struct Node{
    std::map<Node*,float> neighbors;
    Vector2 mapPos;
};

struct AStarNode{
    Node* node;
    float gCost=0;
    float hCost=0;
    float fCost=0;
    AStarNode* parent=nullptr;
};

struct Path{
    std::vector<Node*> path;
    float estCost;
};

class Grid{
public:
    Grid();
    ~Grid(){}
    Path solveDijKstra(std::vector<Node*>);
    Path solveAStar(std::vector<Node*>);
    void Draw();
    
};
