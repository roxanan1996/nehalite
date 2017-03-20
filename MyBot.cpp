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

    std::set<hlt::Move> moves;
    while(true) {
        moves.clear();

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
                            heap.push({neighborLocation.y, neighborLocation.x, scoreMap[neighborLocation.y][neighborLocation.x]});
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

        while(!heap.empty()) {

            LocationScore site = heap.top();
            heap.pop();
            output << "border( " << site.y << ", " << site.x << "): " <<  site.score << " ";
            for(unsigned char i = 1; i < 5; i++) {
                //problema e la indexare
                const hlt::Site neighborSite = presentMap.getSite({ site.y, site.x}, i);
                const hlt::Location neighborLocation = presentMap.getLocation({ site.y, site.x}, i);
                
                if((neighborSite.owner == myID) && (scoreMap[neighborLocation.y][neighborLocation.x] == 0)) {
                    //rendundant
                    if(neighborSite.strength == 0) {

                        moves.insert({ { neighborLocation.x, neighborLocation.y} , 0});

                    } else if(neighborSite.strength < neighborSite.production * 5) {

                            moves.insert({ { neighborLocation.x, neighborLocation.y} , 0});

                        } else if((neighborSite.strength < presentMap.getSite({ neighborLocation.x, neighborLocation.y} , reverseCardinal(i)).strength) && (myID != presentMap.getSite({ neighborLocation.x, neighborLocation.y} , reverseCardinal(i)).owner)) {

                                moves.insert({ { neighborLocation.x, neighborLocation.y} , 0});

                                } else {

                            scoreMap[neighborLocation.y][neighborLocation.x] = site.score - neighborSite.production - 2;
                            moves.insert({ { neighborLocation.x, neighborLocation.y} , reverseCardinal(i)});
                            output << "DAU MISCAREA "<< neighborLocation.y << " " << neighborLocation.x << " " << reverseCardinal(i) + 41 << " " << neighborSite.production + 41 << "\n" ;
                            heap.push({neighborLocation.y, neighborLocation.x, scoreMap[neighborLocation.y][neighborLocation.x]});

                    }

                }

            }

        }/*
        output << std::endl;
        for(hlt::Move print : moves)

            output << print.loc.x << " " << print.loc.y << " " << print.dir + 41 << " ";
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

        } */

        sendFrame(moves);
    
    }

    return 0;
}
