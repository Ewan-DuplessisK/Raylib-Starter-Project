#include "raylib.h"
#include <iostream>
#include "Boid.h"

using namespace std;

int main() {
    SetRandomSeed(time(0));
    
    array<Boid*,BOIDS_NUMBER> boids{};
    array<Obstacle*,OBSTACLES_NUMBER> obstacles{};

    //set up boids
    for(int i=0;i<BOIDS_NUMBER/3;i++){
        Vector2 posP = Vector2{float(GetRandomValue(11,589)),float(GetRandomValue(11,589))};
        Vector2 dirP = Vector2{GetRandomValue(0,100)/100.f,GetRandomValue(0,100)/100.f};
        float speedP = 2.f;
        float mDisP = 30.f;
        float aDisP = 150.f;
        float gDisP = 200.f;
        Color teamP = RED;
        Color predatorP = BLUE;
        Color preyP = YELLOW;
        boids.at(i)=new Boid(posP,dirP,speedP,mDisP,aDisP,gDisP,teamP,predatorP,preyP);
    }
    for(int i=0;i<BOIDS_NUMBER/3;i++){
        Vector2 posP = Vector2{float(GetRandomValue(11,589)),float(GetRandomValue(11,589))};
        Vector2 dirP = Vector2{GetRandomValue(0,100)/100.f,GetRandomValue(0,100)/100.f};
        float speedP = 2.f;
        float mDisP = 30.f;
        float aDisP = 150.f;
        float gDisP = 200.f;
        Color teamP = YELLOW;
        Color predatorP = RED;
        Color preyP = BLUE;
        boids.at(11+i)=new Boid(posP,dirP,speedP,mDisP,aDisP,gDisP,teamP,predatorP,preyP);
    }
    for(int i=0;i<BOIDS_NUMBER/3;i++){
        Vector2 posP = Vector2{float(GetRandomValue(11,589)),float(GetRandomValue(11,589))};
        Vector2 dirP = Vector2{GetRandomValue(0,100)/100.f,GetRandomValue(0,100)/100.f};
        float speedP = 2.f;
        float mDisP = 30.f;
        float aDisP = 150.f;
        float gDisP = 200.f;
        Color teamP = BLUE;
        Color predatorP = YELLOW;
        Color preyP = RED;
        boids.at(22+i)=new Boid(posP,dirP,speedP,mDisP,aDisP,gDisP,teamP,predatorP,preyP);
    }

    //set up walls
    obstacles.at(0)=new Obstacle(-1,-1,2,602);
    obstacles.at(1)=new Obstacle(-1,-1,602,2);
    obstacles.at(2)=new Obstacle(599,-1,2,602);
    obstacles.at(3)=new Obstacle(-1,599,602,2);

    //set up obstacles
    /*obstacles.at(4)=new Obstacle(50,50,50,50);
    obstacles.at(5)=new Obstacle(400,300,40,60);*/
    
    cout << "Hello World" << endl;

    InitWindow(600, 600, "My first Raylib window!");
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
            //cout<<boid->getDirection().x<<" "<<boid->getDirection().y<<endl;
            //cout << Vector2Angle({1.f,0.f},boid->getDirection())*(180.f/PI)<<endl;
            //DrawPoly(boid->getPosition(), 3, 20,Vector2Angle({1.f,0.f},boid->getDirection())*(180.f/PI), BLUE);
            boid->update(boids,obstacles);
            //float angle=Vector2Angle({1.f,0.f},boid->getDirection())*(180.f/PI);
            //DrawTexturePro(boidTex,{0,0,512,512},{boid->getPosition().x-5.0f,boid->getPosition().y-5.0f,10,10},{5.f,5.f},angle,WHITE);
            
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}