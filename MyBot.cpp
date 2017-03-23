#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <set>
#include <fstream>

#include <queue>
#include <climits>

#include "hlt.hpp"
#include "networking.hpp"

struct LocationScore {

    unsigned short x;
    unsigned short y;
    int score;

};

static bool operator<(const LocationScore& lsc1, const LocationScore& lsc2) {

    return (lsc1.score < lsc2.score);

}

unsigned char reverseCardinal( unsigned short cardinal) {

    switch(cardinal) {

        case NORTH: return SOUTH;
        case EAST:  return WEST;
        case SOUTH: return NORTH;
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

    //din acest moment avem 15 secunde pana sa facem sendInit, altfel Timeout
    std::priority_queue<LocationScore> heap;
    std::ofstream output("Score.txt");

    sendInit("MyC++Bot");
//std::vector< std::vector<LocationScore> > scoreMap(presentMap.height, std::vector<LocationScore>(presentMap.width));
//int scoreMap[presentMap.height][presentMap.width] = {0};
int acc = 0;
    std::set<hlt::Move> moves;
    while(true) {
        moves.clear();acc++;

        getFrame(presentMap);
        //sleep(1); //cam sleep(1) + delta, timp de executie pentru fiecare tura
        int scoreMap[presentMap.height][presentMap.width] = {0};

        for(unsigned short a = 0; a < presentMap.height; a++) {
            for(unsigned short b = 0; b < presentMap.width; b++) {
                
                if (presentMap.getSite({ b, a }).owner == myID) {
                    
                    for(unsigned char i = 1; i < 5; i++) {
                       
                        const hlt::Site neighborSite = presentMap.getSite({ b, a }, i);

                         if(neighborSite.owner != myID) {
                           
                            const hlt::Location neighborLocation = presentMap.getLocation({ b, a }, i);
                            scoreMap[neighborLocation.y][neighborLocation.x] = neighborSite.production * 5 - (neighborSite.strength * 7 / 10);
                            heap.push({neighborLocation.x, neighborLocation.y, scoreMap[neighborLocation.y][neighborLocation.x]});
                            //output << "border( " << neighborLocation.x << ", " << neighborLocation.y << "): " <<  scoreMap[neighborLocation.x][neighborLocation.y] << " ";
                            //output << "border( " << b << ", " << a << "): " <<  scoreMap[neighborLocation.y][neighborLocation.x] << " ";
                         }

                    }

                }
                output << scoreMap[a][b] << " ";

            }
            output << '\n';
        }
        output << '\n'; 

        //grija la indexare aici
        while(!heap.empty()) {

            LocationScore site = heap.top();
            heap.pop();
            output << "border( " << site.x << ", " << site.y << "): " <<  site.score << " ";
            for(unsigned char i = 1; i < 5; i++) {
                
                const hlt::Site neighborSite = presentMap.getSite({ site.x, site.y}, i);
                const hlt::Location neighborLocation = presentMap.getLocation({ site.x, site.y}, i);
                
                if((neighborSite.owner == myID) && (scoreMap[neighborLocation.y][neighborLocation.x] == 0)) {

                    scoreMap[neighborLocation.y][neighborLocation.x] = site.score - neighborSite.production - 2;
                    heap.push({neighborLocation.x, neighborLocation.y, scoreMap[neighborLocation.y][neighborLocation.x]});

                }

            }

        }

        for(unsigned short a = 0; a < presentMap.height; a++) {
            for(unsigned short b = 0; b < presentMap.width; b++) {

                if (presentMap.getSite({ b, a }).owner == myID) {
                    
                    const hlt::Site currentSite = presentMap.getSite({ b, a });

                    if(currentSite.strength > 5 * currentSite.production) {

                        int maxScore = INT_MIN;
                        unsigned char dir = 0;

                        for(unsigned char i = 1; i < 5; i++) {
                           
                            const hlt::Location neighborLocation = presentMap.getLocation({ b, a}, i);
                            if(maxScore < scoreMap[neighborLocation.y][neighborLocation.x]) {

                                maxScore = scoreMap[neighborLocation.y][neighborLocation.x];
                                dir = i;

                            }

                        }

                        moves.insert({ { b, a} , dir});

                    }

                }

            }
        }

        output << std::endl;
        output << "FRAME" <<acc;
        for(hlt::Move print : moves)

            output << print.dir + 41 << " ";
        output << std::endl;
        output << std::endl;
        output << std::endl;
/*
        for(unsigned short a = 0; a < presentMap.height; a++) {
            for(unsigned short b = 0; b < presentMap.width; b++) {
                
                hlt::Site site = presentMap.getSite({ b, a});
                if(site.owner == myID) {
                    /*
                    int maxScore = INT_MIN;
                    for(unsigned char i = 1; i < 5; i++) {

                        hlt::Site neighborSite = presentMap.getSite({ b, a}, i);
                        hlt::Location neighborLocation = presentMap.getLocation({ b, a}, i);
                        if(maxScore < scoreMap[neighborLocation.y][neighborLocation.x].score) {

                            maxScore = scoreMap[neighborLocation.y][neighborLocation.x].score;

                        }

                    }
                    //redundant?
                    if(site.strength == 0) {

                        moves.insert({ {b, a}, 0});

                    } else {

                        if(site.strength < site.production * 5) {

                            moves.insert({ { b, a}, 0});

                        } else {



                        }

                    }
                
                }

            }

        } 

for(unsigned char i = 1; i < 5; i++) {
                
                const hlt::Site neighborSite = presentMap.getSite({ site.x, site.y}, i);
                const hlt::Location neighborLocation = presentMap.getLocation({ site.x, site.y}, i);
                
                if((neighborSite.owner == myID) && (scoreMap[neighborLocation.y][neighborLocation.x] == 0)) {

                    scoreMap[neighborLocation.y][neighborLocation.x] = site.score - neighborSite.production - 2;

                    if(neighborSite.strength < neighborSite.production * 5) {

                        moves.insert({ { neighborLocation.x, neighborLocation.y} , 0});

                    }

                    else if((neighborSite.strength < presentMap.getSite({ site.x, site.y}).strength) && (presentMap.getSite({ site.x, site.y}).owner == 0)) {

                            moves.insert({ { neighborLocation.x, neighborLocation.y} , 0});

                            } 

                            else {

                            moves.insert({ { neighborLocation.x, neighborLocation.y} , reverseCardinal(i)});
                            output << "DAU MISCAREA "<< site.x << " " << site.y << " " << reverseCardinal(i) + 41 << " " << neighborSite.production + 41 << "\n" ;
                            heap.push({neighborLocation.x, neighborLocation.y, scoreMap[neighborLocation.y][neighborLocation.x]});

                            }

                    }

            }

        */

        sendFrame(moves);
    
    }

    return 0;
}
