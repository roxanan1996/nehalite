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

int main() {
    srand(time(NULL));

    std::cout.sync_with_stdio(0);

    unsigned char myID;
    hlt::GameMap presentMap;
    getInit(myID, presentMap);

    std::priority_queue<LocationScore> heap;

    sendInit("MyC++Bot");

    std::set<hlt::Move> moves;
    while(true) {
        moves.clear();

        getFrame(presentMap);
        
        int scoreMap[presentMap.height][presentMap.width] = {0};

        for(unsigned short a = 0; a < presentMap.height; a++) {
            
            for(unsigned short b = 0; b < presentMap.width; b++) {
                
                if (presentMap.getSite({ b, a }).owner == myID) {
                    
                    for(unsigned char i = 1; i < 5; i++) {
                       
                        const hlt::Site neighborSite = presentMap.getSite({ b, a }, i);

                         if(neighborSite.owner != myID) {
                           
                            const hlt::Location neighborLocation = presentMap.getLocation({ b, a }, i);
                            scoreMap[neighborLocation.y][neighborLocation.x] = neighborSite.production * 5;
                            heap.push({neighborLocation.x, neighborLocation.y, scoreMap[neighborLocation.y][neighborLocation.x]});
                        
                         }

                    }

                }
                
            }
            
        }
        
        while(!heap.empty()) {

            LocationScore site = heap.top();
            heap.pop();
            
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

                        if(myID != presentMap.getSite({b, a}, dir).owner) {

                            if(currentSite.strength > presentMap.getSite({b, a}, dir).strength) {

                                moves.insert({ { b, a} , dir});                    

                            }

                        } else {

                            moves.insert({ { b, a} , dir});

                        }

                    }

                }

            }
        }

        sendFrame(moves);
    
    }

    return 0;
}
