
    for(auto& r : adjList[room]) {

        if(distancesToZero[r] != TBP && distancesToZero[r] < sizeOfShortestPath) {
            sizeOfShortestPath = distancesToZero[r];
            // std::cout << r << " - distancesToZero[r] != TBP && distancesToZero[r] < sizeOfShortestPath";
            shortestPaths.clear();
            shortestPaths.push_back(r);
        } else if (distancesToZero[r] != TBP && distancesToZero[r] == sizeOfShortestPath) {
            // std::cout << r << " - distancesToZero[r] != TBP && distancesToZero[r] == sizeOfShortestPath";

            shortestPaths.push_back(r);
        }

    }



    //найти номер минимальной комнаты
    int nextRoom = findMin(shortestPaths);
    //перенести понижение 