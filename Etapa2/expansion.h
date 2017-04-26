/*
static bool operator<(const LocationScore& lsc1, const LocationScore& lsc2) {

    return (lsc1.score < lsc2.score);

}
/*
void bfs(hlt::GameMap presentMap, int* scoreMap, std::queue<hlt::Location> queue, unsigned char myID) {

    //BFS - Momentan incomplet(cred), trb analizat faza cu minimul casutelor / sau nu trb deaorece verifica daca e 0 la inceput
    while(!queue.empty()) {

        hlt::Location site = queue.front();
        queue.pop();
        
        for(unsigned char i = 1; i < 5; i++) {
            
            const hlt::Site neighborSite = presentMap.getSite({ site.x, site.y}, i);
            const hlt::Location neighborLocation = presentMap.getLocation({ site.x, site.y}, i);
            
            if((neighborSite.owner == myID) && (scoreMap[neighborLocation.y * neighborLocation.x + neighborLocation.x] == 0)) {

                scoreMap[neighborLocation.y * neighborLocation.x + neighborLocation.x] = scoreMap[site.y * site.x + site.x] + 1;
                queue.push({neighborLocation.x, neighborLocation.y});

            }

        }

    }

}
*/

struct Square {

    hlt::Location location;
    hlt::Site site;

};

void expansionStrategy(hlt::GameMap &presentMap, std::set<hlt::Move> &moves, unsigned char myID) {

    //Debug file
    //std::ofstream output("Score.txt");

    std::queue<hlt::Location> queue;
    std::list<Square> borders;
    std::list<std::pair<hlt::Location, unsigned char>> attackers; 
    int scoreMap[presentMap.height][presentMap.width];
    int movesMap[presentMap.height][presentMap.width];
    int numberOfSquares = presentMap.height * presentMap.width + 1;

    //nu reinitializa linia 0 din anumite motive
    //int scoreMap[presentMap.width][presentMap.height] = {0};
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
                
                //unsigned char plm = sendBotTo({ b, a }, {0, 0}, presentMap);
                //moves.insert({{b, a}, plm});

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
/*
    int sum = 0;

    while(!borders.empty()) {

        Square bot = borders.front();
        borders.pop_front();

        for(unsigned char i = 1; i < 5; i++) {

            const hlt::Site neighborSite = presentMap.getSite({bot.location.x, bot.location.y}, i);
            const hlt::Location neighborLocation = presentMap.getLocation({bot.location.x, bot.location.y}, i);

            if(myID == neighborSite.owner) {

                sum += neighborSite.strength;
                attackers.push_front(std::make_pair(neighborLocation, i));

                if(sum > bot.site.strength) {

                    while(!attackers.empty()) {

                        std::pair<hlt::Location, unsigned char> attackPosition = attackers.front();
                        hlt::Location attackerLocation = attackPosition.first;
                        unsigned char dir = attackPosition.second;
                        attackers.pop_front();
                        output << "Uite sumabahahahahahaha \n" << sum;
                        moves.insert({{attackerLocation.y, attackerLocation.x}, reverseCardinal(dir)});
                        movesMap[attackerLocation.y][attackerLocation.x] = 1;

                    }

                    break;

                }

            }

        }

        sum = 0;

    }
    */
/*
    output << "After doing them bfs\n";
    for(unsigned short a = 0; a < presentMap.height; a++) {

        for(unsigned short b = 0; b < presentMap.width; b++) {

            output << scoreMap[a][b] << " ";

        }

        output << '\n';
    }

    output << '\n';
    */

    for(unsigned short a = 0; a < presentMap.height; a++) {
       
        for(unsigned short b = 0; b < presentMap.width; b++) {

            if((presentMap.getSite({ b, a }).owner == myID) && (movesMap[b][a] == 0)) {
                
                const hlt::Site currentSite = presentMap.getSite({ b, a });

                if(currentSite.strength > 5 * currentSite.production) {

                    int minScore = numberOfSquares;
                    unsigned char dir = 0;

                    //We should always make it to check first points of interest - atm it prioritises to go to the WEST, because of the for startiing from 1 to 5
                    //and because in the if has  >= so when all neighbors are good, it will go to the WEST
                    //for future we will want different directions prioritization
                    for(unsigned char i = 1; i < 5; i++) {
                       
                        const hlt::Location neighborLocation = presentMap.getLocation({ b, a}, i);
                        
                        if(minScore >= scoreMap[neighborLocation.y][neighborLocation.x]) {

                            minScore = scoreMap[neighborLocation.y][neighborLocation.x];
                            dir = i;

                        }

                    }

                    if(myID != presentMap.getSite({b, a}, dir).owner) {

                        if(currentSite.strength >= presentMap.getSite({b, a}, dir).strength) {

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
