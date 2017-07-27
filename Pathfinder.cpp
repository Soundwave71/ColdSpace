//
// Created by Leonardo on 7/11/2017.
//

#include "Pathfinder.h"
#include "Map.h"

Pathfinder::Pathfinder(){}

Pathfinder::~Pathfinder(){}

sf::Vector2f Pathfinder::DestinationRandomizer() {
    unsigned int mapx = m_gamemap->GetMapSize().x;
    unsigned int mapy = m_gamemap->GetMapSize().y;
    unsigned int tileSize= m_gamemap->GetTileSize();
    sf::Vector2f target;
    target.x=rand() % (mapx-1);
    target.y=rand() % (mapy-1);
    while(m_grid.m_gamemap->GetTile(target.x,target.y,1)->m_solid){
        target.x=rand() % (mapx-1);
        target.y=rand() % (mapy-1);
    }
    target.x=target.x*tileSize;
    target.y=target.y*tileSize;
    return target;
}

std::vector<sf::Vector2f> Pathfinder::Astar(sf::Vector2f start, sf::Vector2f target) {

        //Setup
        bool found = false;
        bool ready_1 = false;
        bool ready_2 = false;

        Node *processingNode = m_grid.GetNode(start);
        if (!processingNode)
            std::cout << "StartingNode!" << std::endl;
        else {
            processingNode->m_coords = start;
            ready_1 = true;
        }

        Node *targetNode = m_grid.GetNode(target);
        if (!targetNode)
            std::cout << "TargetNode!" << std::endl;
        else {
            targetNode->m_coords = target;
            ready_2 = true;
        }

            m_open.push_back(processingNode);


        //ready check
        if (ready_1 && ready_2) {
            while (!found && m_open.size() > 0) {

                processingNode = PopBest();
                if (processingNode->m_ID == targetNode->m_ID) {
                    found = true;
                } else {

                    CheckNeighbours(processingNode, targetNode);

                }

                m_closed.push_back(processingNode);

            }

            //Build Best Path
            while (processingNode) {

                    /*std::cout<<  processingNode->m_coords.x<< " " <<processingNode->m_coords.y<< " --> ";*/
                route.push_back(processingNode->m_coords);

                processingNode = processingNode->m_leash;
            }
            //Sets Pathfinder back to default.
            ResetGrid();

            return route;
        } else
        {
            route.push_back(sf::Vector2f(-1000, -1000));

            //Sets Pathfinder back to default.
            ResetGrid();
            return route;
        }

}

void Pathfinder::CheckNeighbours(Node *currentNode, Node* targetNode) {
    Node* tempNode;
    sf::Vector2f newcoords= currentNode->m_coords;
    for(int i=0; i<8; i++ ) {
        newcoords += m_directions[i];

        if (newcoords.x >= 0 && newcoords.x < (m_grid.m_gamemap->GetMapSize().x*m_gamemap->GetTileSize()) &&
                newcoords.y >= 0 && newcoords.y < (m_grid.m_gamemap->GetMapSize().y*m_gamemap->GetTileSize()))
        {
            tempNode = m_grid.GetNode(newcoords);

            bool InOpen = IsOnOpen(tempNode);
            bool InClosed = IsOnClosed(tempNode);

            if (!tempNode->IsSolid() && !InClosed && !InOpen) {
                tempNode->m_coords = newcoords ;
                tempNode->ComputeCost(targetNode, currentNode, i >= 4);
                tempNode->m_leash = currentNode;
                m_open.push_back(tempNode);

            } else if (!tempNode->IsSolid() && InOpen) {
                if ((i >= 4 ? currentNode->m_gCost + 14 : currentNode->m_gCost + 10) < tempNode->m_gCost) {
                    currentNode->m_leash = tempNode;
                    tempNode->ComputeCost(targetNode, currentNode, i >= 4);
                }
            } else if (tempNode->IsSolid())
                m_closed.push_back(tempNode);
        }
        newcoords-=m_directions[i];
    }
}



Node* Pathfinder::PopBest() {
    int bestIndex = 0;
    int bestCost = 50000;
    Node * tempNode;
    for (int i = 0; i < m_open.size(); i++) {
        tempNode = m_open.at(i);

        if (tempNode->getCost() < bestCost ) {
            bestIndex = i;
            bestCost = tempNode->getCost();
        }
    }

    //get best node
    tempNode = m_open.at(bestIndex);

    //erase it from list
    m_open.erase(m_open.begin() + bestIndex);
    return tempNode;
}

void Pathfinder::ResetGrid(){
    for(auto itr=m_grid.m_gridData.begin(); itr!= m_grid.m_gridData.end(); itr++)
        itr->Reset();
    m_open.clear();
    m_closed.clear();
}

bool Pathfinder::IsOnClosed(Node* node ) {
    for (auto itr=m_closed.begin(); itr!=m_closed.end(); itr++) {
        if ((*itr)==node) {
            return true;
        }
    }
    return false;
}

bool Pathfinder::IsOnOpen(Node* node) {
    for (auto itr=m_open.begin(); itr!=m_open.end(); itr++) {
        if ((*itr)==node) {
            return true;
        }
    }
    return false;
}

void Pathfinder::SetMapGrid(Map *gamemap) {
    m_gamemap=gamemap;
    m_grid.SetupGrid(gamemap);
    float tils= m_gamemap->GetTileSize();
    m_directions[0]={0,-tils}; //up
    m_directions[1]={0,tils}; //down
    m_directions[2]={-tils,0}; //left
    m_directions[3]={tils,0}; //right
    m_directions[4]={tils,tils}; //bot right
    m_directions[5]={tils,-tils}; //top right
    m_directions[6]={-tils,tils}; //bot left
    m_directions[7]={-tils,-tils}; // top left
}
