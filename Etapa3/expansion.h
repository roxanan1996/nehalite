
struct Square {

    hlt::Location location;
    hlt::Site site;

};

void expansionStrategy(hlt::GameMap &presentMap, std::set<hlt::Move> &moves, unsigned char myID) {

    std::queue<hlt::Location> queue;
    std::list<Square> borders;
    std::list<std::pair<hlt::Location, unsigned char>> attackers; 
    int scoreMap[presentMap.height][presentMap.width];
    int movesMap[presentMap.height][presentMap.width];
    int numberOfSquares = presentMap.height * presentMap.width + 1;

    //Resets Scoremap
    for(unsigned short a = 0; a < presentMap.height; a++) {

        for(unsigned short b = 0; b < presentMap.width; b++) {

            scoreMap[a][b] = 0;
            movesMap[a][b] = 0;

        }

    }

    for(unsigned short a = 0; a < presentMap.height; a++) {
        
        for(unsigned short b = 0; b < presentMap.width; b++) {
            
            if(presentMap.getSite({ b, a }).owner == myID) {

                for(unsigned char i = 1; i < 5; i++) {
                   
                    const hlt::Site neighborSite = presentMap.getSite({ b, a }, i);

                    //Finding the borders
                    if(neighborSite.owner != myID) {
                       
                        const hlt::Location neighborLocation = presentMap.getLocation({ b, a }, i);
                        scoreMap[neighborLocation.y][neighborLocation.x] = 1;
                        queue.push({neighborLocation.x, neighborLocation.y});
                        borders.push_front({{neighborLocation.x, neighborLocation.y}, neighborSite});
                    
                    }

                }

            }
            
        }
        
    } 
    
    //BFS - Momentan incomplet(cred), trb analizat faza cu minimul casutelor / sau nu trb deaorece verifica daca e 0 la inceput
    while(!queue.empty()) {

        hlt::Location borderLocation = queue.front();
        queue.pop();
        
        for(unsigned char i = 1; i < 5; i++) {
            
            const hlt::Site neighborSite = presentMap.getSite({ borderLocation.x, borderLocation.y}, i);
            const hlt::Location neighborLocation = presentMap.getLocation({ borderLocation.x, borderLocation.y}, i);
            
            if((neighborSite.owner == myID) && (scoreMap[neighborLocation.y][neighborLocation.x] == 0)) {

                scoreMap[neighborLocation.y][neighborLocation.x] = scoreMap[borderLocation.y][borderLocation.x] + 1;
                queue.push(neighborLocation);

            }

        }

    }

    for(unsigned short a = 0; a < presentMap.height; a++) {
       
        for(unsigned short b = 0; b < presentMap.width; b++) {

            if((presentMap.getSite({ b, a }).owner == myID) && (movesMap[b][a] == 0)) {
                
                const hlt::Site currentSite = presentMap.getSite({ b, a });

                if(currentSite.strength > 5 * currentSite.production) {

                    int minScore = numberOfSquares;
                    unsigned char dir = 0;

                    for(unsigned char i = 1; i < 5; i++) {
                       
                        const hlt::Location neighborLocation = presentMap.getLocation({ b, a}, i);
                        
                        if(minScore >= scoreMap[neighborLocation.y][neighborLocation.x]) {

                            minScore = scoreMap[neighborLocation.y][neighborLocation.x];
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

}
