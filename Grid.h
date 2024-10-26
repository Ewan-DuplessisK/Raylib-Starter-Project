#pragma once
#include <map>
#include <raylib.h>
#include <vector>

struct Node{
    std::map<Node*,float> neighbors;
    Vector2 mapPos;
    Rectangle coll = {mapPos.x-5.f,mapPos.y-5.f,10.f,10.f};
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
    int step=0;
};

class Grid{
public:
    Grid();
    ~Grid(){}
    Path solveDijKstra(std::vector<Node*>);
    Path solveAStar(std::vector<Node*>);
    void Draw();
    std::vector<Node*> getGraph(){return graph;}
private:
    std::vector<Node*> graph;
};
