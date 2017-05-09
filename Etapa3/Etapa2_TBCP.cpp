#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <set>
#include <fstream>

#include <queue>
#include <list>

#include "hlt.hpp"
#include "networking.hpp"

#include "expansion.h"
#include "tunneling.h"

unsigned char reverseCardinal( unsigned short cardinal) {

    switch(cardinal) {

        case NORTH: return SOUTH;
        case SOUTH: return NORTH;
        case EAST:  return WEST;
        case WEST:  return EAST;
        default: return STILL;

    }

}

hlt::Location minimumDistance(hlt::Location startPoint, unsigned short maxLeft,
                                unsigned short maxRight, unsigned short maxUp, unsigned short maxDown,
                                hlt::GameMap &presentMap) {

    std::vector<std::pair<float, hlt::Location> > coord;
    
    float distance0 = presentMap.getDistance(startPoint, {maxLeft, maxDown});
    float distance1 = presentMap.getDistance(startPoint, {maxRight, maxDown});
    float distance2 = presentMap.getDistance(startPoint, {maxLeft, maxUp});
    float distance3 = presentMap.getDistance(startPoint, {maxRight, maxUp});

    coord.push_back(std::pair<float, hlt::Location>(distance0, {maxLeft, maxDown}));
    coord.push_back(std::pair<float, hlt::Location>(distance1, {maxRight, maxDown}));
    coord.push_back(std::pair<float, hlt::Location>(distance2, {maxLeft, maxUp}));
    coord.push_back(std::pair<float, hlt::Location>(distance3, {maxRight, maxUp}));

   float min = coord[0].first;
    hlt::Location destination = coord[0].second;

    for (int i = 1; i < 4; ++i) {
        if (min > coord[i].first) {
            min = coord[i].first;
            destination = coord[i].second;
        }
    }

    return destination;
}

int main() {
    srand(time(NULL));

    std::cout.sync_with_stdio(0);

    unsigned char myID;
    hlt::GameMap presentMap;
    getInit(myID, presentMap);

    int scoreMap[100][100];
    int acc = 1;
    int accp = 10;

    //heuristica
    for(unsigned short a = 0; a < presentMap.height; a++) {

        for(unsigned short b = 0; b < presentMap.width; b++) {

            const hlt::Site site = presentMap.getSite({ b, a });

            if(site.strength < 50) {
                acc = 1;
            } else if(site.strength < 100) {
                acc = 3;
            } else if(site.strength < 150) {
                acc = 4;
            }

            if(site.production < 5) {
                accp = 5;
            } else if(site.production < 10) {
                accp = 10;
            } else if(site.production < 15) {
                accp = 30;
            }   

            scoreMap[a][b] = accp * site.production - acc * site.strength;

        }

    }
 
    unsigned short maxLeft, maxRight, maxUp, maxDown, middleI, middleJ;
    int maxSum = matrixSum(presentMap.height / 4, presentMap.width / 4, scoreMap, maxLeft, 
                            maxRight, maxUp, maxDown);

    // coordonatele mijlocului zonei productive
    middleI = (maxLeft + maxRight) / 2;
    middleJ = (maxUp + maxDown) / 2;
    hlt::Location close = minimumDistance(initialPosition(presentMap, myID), maxLeft, maxRight, maxUp, maxDown, presentMap);

    sendInit("MyC++Bot");

    std::set<hlt::Move> moves;
    unsigned char strategy = 1;

    while(true) {
        moves.clear();

        getFrame(presentMap);

        switch(strategy) {

            //Tunneling
            case 1: {

                //Tunneling
                //tunneling(presentMap, {middleI, middleJ}, moves, myID);
                tunneling(presentMap, close, moves, myID);

                //if destination reached, switch to expansion strategy
                if(presentMap.getSite(close).owner == myID) {

                    strategy = 2;

                }

                break;

            }

            //Expansion
            case 2: {

                expansionStrategy(presentMap, moves, myID);
                break;

            }

        }

        sendFrame(moves);
    
    }

    return 0;
}
