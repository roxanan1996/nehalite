
using namespace std;

hlt::Location initialPosition (hlt::GameMap presentMap, unsigned char myID) {

    for(unsigned short a = 0; a < presentMap.height; a++) {
            
            for(unsigned short b = 0; b < presentMap.width; b++) {
                
                if (presentMap.getSite({ b, a }).owner == myID) {

                    // gasesc coordonatele unde am botul prima oara
                    return {b, a};

                }
            }
    }       
            
}

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


// calculeaza subsecventa de suma maxima dintr-un array
int maximumSum (int array[], unsigned short n, unsigned short &up, unsigned short &down) {

    vector <pair<int, unsigned short> > max;

    //first e suma
    // second e up

    if (array[0] > 0) {
        max.push_back(pair<int, int>(array[0], 0));
    } else {
        max.push_back(pair<int, int>(0, 1));
    }

    for (unsigned short i = 1; i < n; ++i) {
        if (array[i] + max[i - 1].first >= 0) {
            max.push_back(pair<int, int>(array[i] + max[i - 1].first, max[i - 1].second));
        } else {
            max.push_back(pair<int, int>(0, i + 1));
        }
    }

    pair<int, unsigned short> maximum = pair<int, int>(0, 0);
    for (unsigned short i = 0; i < n; ++i) {
        if (maximum.first < max[i].first) {
            maximum = max[i];
            down = i;
        }
    }

    up = maximum.second;
    return maximum.first;
}


// calculeaza subsecventa de suma maxima dintr-o matrice
int matrixSum(unsigned short lines, unsigned short columns, int matrix[100][100], unsigned short &maxLeft, unsigned short &maxRight, unsigned short &maxUp, unsigned short &maxDown) {

    int maxSum = -1;

    for (unsigned short L = 0; L < columns; ++L) {

        int array[lines] = {0};
        for (unsigned short R = L; R < columns; ++R) { 

            for (unsigned short i = 0; i < lines; ++i) {
                array[i] += matrix[i][R];
            }

            unsigned short up, down;
            int currentSum = maximumSum(array, lines, up, down);

            if (currentSum > maxSum) {
                maxSum = currentSum;
                maxLeft = L;
                maxRight = R;
                maxUp = up;
                maxDown = down;
            }

        }
    }

    return maxSum;

} 