
using namespace std;

// afla coordonatele initiale
pair<unsigned short, unsigned short> initialPosition (hlt::GameMap presentMap, unsigned char myID) {
    pair<unsigned short, unsigned short> coord;
    for(unsigned short a = 0; a < presentMap.height; a++) {
            
            for(unsigned short b = 0; b < presentMap.width; b++) {
                
                if (presentMap.getSite({ b, a }).owner == myID) {

                    //Do Nothing 
                    // gasesc coordonatele unde am botul prima oara
                    coord = pair<unsigned short, unsigned short>(a, b);
                    return coord;
                }
            }
    }

    return coord;        
            
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

            // afisare vector
            /*cout << "afisare vector coloana\n";
            for (int i = 0; i < lines; ++i) {
                cout << array[i] << " ";
            }
            cout << "\n";
            */
            unsigned short up, down;
            int currentSum = maximumSum(array, lines, up, down);
            
            // cout << "currentSum = " << currentSum << "\n";
            // cout << "up and down " << up << down << "\n";
            // cout << "suma maxima " << maxSum << "\n";
            if (currentSum > maxSum) {
                maxSum = currentSum;
                maxLeft = L;
                maxRight = R;
                maxUp = up;
                maxDown = down;
            }

            // cout << "coordonatele daca se schimba ceva\n";
            // cout << maxLeft << " " << maxRight << " " << maxUp << " " << maxDown << "\n";
        }
    }

    return maxSum;

} 