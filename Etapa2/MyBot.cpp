#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <set>
#include <fstream>

#include <queue>
#include <list>

//for INT_MAX or INT_MIN
//#include <climits>

#include "hlt.hpp"
#include "networking.hpp"

#include "expansion.h"
#include "tunneling.h"

unsigned char sendBotTo(hlt::Location botLocation, hlt::Location destinationLocation, hlt::GameMap &presentMap) {

    int distance;
    int minDistance = presentMap.getDistance(presentMap.getLocation(botLocation, 1), destinationLocation);
    unsigned char currentMove = 1;

    for(unsigned char i = 2; i < 5; i++) {
        
        distance = presentMap.getDistance(presentMap.getLocation(botLocation, i), destinationLocation);

        if(minDistance > distance) {

            minDistance = distance;
            currentMove = i;

        }

    }

    return currentMove;

}

void tunneling(hlt::GameMap &presentMap, hlt::Location zone, std::set<hlt::Move> &moves, unsigned char myID) {

    unsigned char dir;

    for(unsigned short a = 0; a < presentMap.height; a++) {
        
        for(unsigned short b = 0; b < presentMap.width; b++) {
            
            const hlt::Site currentSite = presentMap.getSite({b, a});

            if(currentSite.owner == myID) {
                
                //TODO
                //dont forget to give certain location
                dir = sendBotTo({ b, a }, zone, presentMap);
                const hlt::Site directionSite = presentMap.getSite({b, a}, dir);

                if(myID != directionSite.owner) {

                    if(currentSite.strength > directionSite.strength) {

                        moves.insert({ { b, a} , dir});                    

                    }

                } else {

                    if(currentSite.strength > 5 * currentSite.production) {

                        moves.insert({ { b, a} , dir});

                    }

                }

            }

        }

    }

}

unsigned char reverseCardinal( unsigned short cardinal) {

    switch(cardinal) {

        case NORTH: return SOUTH;
        case SOUTH: return NORTH;
        case EAST:  return WEST;
        case WEST:  return EAST;
        default: return STILL;

    }

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
 
    //Debug file
    std::ofstream output("Tunnel.txt");
    output << "ScoreMap\n";
    for(unsigned short a = 0; a < presentMap.height; a++) {

        for(unsigned short b = 0; b < presentMap.width; b++) {

            output << scoreMap[a][b] << " ";

        }

        output << '\n';
    }

    output << '\n';

    unsigned short maxLeft, maxRight, maxUp, maxDown, middleI, middleJ;

    int maxSum = matrixSum(presentMap.height, presentMap.width, scoreMap, maxLeft, 
                            maxRight, maxUp, maxDown);

    // coordonatele mijlocului zonei productive
    middleI = (maxLeft + maxRight) / 2;
    middleJ = (maxUp + maxDown) / 2;

    pair<unsigned short, unsigned short> coord = initialPosition(presentMap, myID);

    output << "poz initiala: " << coord.first << " " << coord.second << "\n";
    output << '\n'; 
    output << "mijlocul " << middleI << " " << middleJ << "\n";
    output << maxLeft << " " << maxRight << " " << maxUp << " " << maxDown << "\n";
    output.close();

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
                tunneling(presentMap, {middleJ, middleI}, moves, myID);

                //if destinatian reached, switch to expansion strategy
                if(presentMap.getSite({middleJ, middleI}).owner == myID) {

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