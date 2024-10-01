#include "raylib.h"
#include <iostream>
#include <vector>
#include "Boid.h"

using namespace std;

int main() {
    SetRandomSeed(time(0));
    int spawnBuffer = 11;

    vector<Boid*> boids;
    //array<Boid*,BOIDS_NUMBER> boids{};
    array<Obstacle*,OBSTACLES_NUMBER> obstacles{};

    //set up boids
    for(int i=0;i<BOIDS_NUMBER/3;i++){
        Vector2 posP = Vector2{float(GetRandomValue(spawnBuffer,window_size-spawnBuffer)),float(GetRandomValue(spawnBuffer,window_size-spawnBuffer))};
        Vector2 dirP = Vector2{GetRandomValue(0,100)/100.f,GetRandomValue(0,100)/100.f};
        float speedP = 2.f;
        float mDisP = 30.f;
        float aDisP = 150.f;
        float gDisP = 200.f;
        float cDisP = 150.f;
        std::array<float,7> weightsP = {0.40,0.30,0.25,0.05,0.6,1.f,0.3}; //separate, avoidObstacles, align, group, chasePrey, avoidPredator, mouse influence
        Color teamP = RED;
        Color predatorP = BLUE;
        Color preyP = YELLOW;
        boids.push_back(new Boid(posP,dirP,speedP,mDisP,aDisP,gDisP,cDisP,teamP,predatorP,preyP,weightsP));
    }
    for(int i=0;i<BOIDS_NUMBER/3;i++){
        Vector2 posP = Vector2{float(GetRandomValue(spawnBuffer,window_size-spawnBuffer)),float(GetRandomValue(spawnBuffer,window_size-spawnBuffer))};
        Vector2 dirP = Vector2{GetRandomValue(0,100)/100.f,GetRandomValue(0,100)/100.f};
        float speedP = 2.f;
        float mDisP = 30.f;
        float aDisP = 150.f;
        float gDisP = 200.f;
        float cDisP = 150.f;
        std::array<float,7> weightsP = {0.40,0.30,0.25,0.05,1.f,0.6,0.3}; //separate, avoidObstacles, align, group, chasePrey, avoidPredator, mouse influence
        Color teamP = YELLOW;
        Color predatorP = RED;
        Color preyP = BLUE;
        boids.push_back(new Boid(posP,dirP,speedP,mDisP,aDisP,gDisP,cDisP,teamP,predatorP,preyP,weightsP));
    }
    for(int i=0;i<BOIDS_NUMBER/3;i++){
        Vector2 posP = Vector2{float(GetRandomValue(spawnBuffer,window_size-spawnBuffer)),float(GetRandomValue(spawnBuffer,window_size-spawnBuffer))};
        Vector2 dirP = Vector2{GetRandomValue(0,100)/100.f,GetRandomValue(0,100)/100.f};
        float speedP = 2.f;
        float mDisP = 30.f;
        float aDisP = 150.f;
        float gDisP = 200.f;
        float cDisP = 150.f;
        std::array<float,7> weightsP = {0.40,0.30,0.25,0.05,0.6,0.6,0.3}; //separate, avoidObstacles, align, group, chasePrey, avoidPredator, mouse influence
        Color teamP = BLUE;
        Color predatorP = YELLOW;
        Color preyP = RED;
        boids.push_back(new Boid(posP,dirP,speedP,mDisP,aDisP,gDisP,cDisP,teamP,predatorP,preyP,weightsP));
    }

    //set up walls
    obstacles.at(0)=new Obstacle(-1,-1,2,window_size+2);
    obstacles.at(1)=new Obstacle(-1,-1,window_size+2,2);
    obstacles.at(2)=new Obstacle(window_size-1,-1,2,window_size+2);
    obstacles.at(3)=new Obstacle(-1,window_size-1,window_size+2,2);

    //set up obstacles
    /*obstacles.at(4)=new Obstacle(50,50,50,50);
    obstacles.at(5)=new Obstacle(400,300,40,60);*/
    
    cout << "Hello World" << endl;

    InitWindow(window_size, window_size, "My first Raylib window!");
    Texture2D boidTex = LoadTexture("tropical-fish.png");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGREEN);
        for (Obstacle* obstacle : obstacles){
            DrawRectangleRec(obstacle->getRectangle(),BLACK);
        }
        for(Boid* boid:boids){
            DrawCircle(boid->getPosition().x,boid->getPosition().y,10,boid->getTeam());
            boid->update(boids,obstacles);
            
        }
        for(Boid* toKill: Boid::pendingKill){
            boids.erase(std::remove(boids.begin(), boids.end(), toKill), boids.end());
        }
        Boid::pendingKill={};
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
//= {0.40,0.30,0.25,0.05}