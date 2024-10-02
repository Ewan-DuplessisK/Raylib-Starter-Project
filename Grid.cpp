#include "Grid.h"

Grid::Grid(){}

/*struct Node{
std::map<Node*,float> neighbors;
Vector2 mapPos;
};*/

bool compare_float(float x, float y, float epsilon = 0.01f){
    return(fabs(x - y) < epsilon);
}

Path Grid::solveDijKstra(std::vector<Node*> graph){
    std::vector<float> distance;
    std::vector<int> previous;
    std::vector<int> set;
    
    for(int i=0;i<graph.size();i++){
        distance.emplace_back(FLT_MAX);
        previous.emplace_back(-1);
        set.emplace_back(i);
    }

    distance[0]=0;
    int index = -1;
    
    while(!set.empty()){
        float temp = FLT_MAX;
        for(int value:set){
            if(distance.at(value)<temp){
                temp=distance.at(value);
                index = value;
            }
        }
        set.erase(std::find(set.begin(),set.end(),index));
        for(int i=0;i<graph.size();i++){
            if(!compare_float(graph.at(index)->neighbors[graph.at(i)],FLT_MAX)){
                if(std::find(set.begin(), set.end(), i) != set.end()){
                    const float tmp = distance.at(index)+graph.at(index)->neighbors[graph.at(i)];
                    if(tmp<distance.at(i)){
                        distance.at(i)=tmp;
                        previous.at(i)=index;
                    }
                }
            }
        }
    }

    Path res;
    res.estCost=distance.at(graph.size()-1);
    res.path.push_back(graph.at(graph.size()-1));
    int prev = previous.at(graph.size()-1);
    while (prev!=0){
        res.path.push_back(graph.at(prev));
        prev=previous.at(prev);
    }
    res.path.push_back(graph.at(0));
    std::reverse(res.path.begin(),res.path.end());

    return res;
}

Path Grid::solveAStar(std::vector<Node*> graph){
    std::vector<AStarNode*> openList;
    std::vector<AStarNode*> closedList;

    std::vector<AStarNode*> AStarGraph;
    for(Node* node:graph){
        AStarNode* n;
        n->node=node;
    }
    AStarGraph.at(0)->fCost=0;
    openList.push_back(AStarGraph.at(0));

    while (!openList.empty()){
        AStarNode* currNode=openList.at(0);
        for(int i = 0; i < openList.size(); i++){
            if(openList.at(i)->fCost<currNode->fCost){
                currNode=openList.at(i);
            }
        }
        closedList.push_back(currNode);
        openList.erase(std::remove(openList.begin(), openList.end(), currNode), openList.end());

        if(currNode->node==graph.at(graph.size()-1)){
            //TODO Backtrack
        }
        else{
            for(auto pair : currNode->node->neighbors){
                AStarNode* tmp;
                for(AStarNode* n:AStarGraph){ //find_if
                    if(n->node==pair.first)tmp=n;
                }
                if(std::find(closedList.begin(),closedList.end(),tmp)!=closedList.end()){//not in closed list
                    
                }
                
            }
        }
    }
    
}

