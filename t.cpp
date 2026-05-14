#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <queue>

//!!!ВОПРОС!!!
// как в примере мы прошли из 4 комнаты в 3, этой вершины нет у 4 в списке смежности
//так как во всех остальных случаях если вершина u смежна с вершиной v, то и вершина v смежна с вершиной u, поэтому я решил что это опечатка во входных данных

//как вы считаете сумму коллектед 

// the number of rooms
int n;                                                                                                                                                                                                                                                                                                                                     

// graph
std::vector<std::vector<int>> adjList;

//number of eat
int m;
//targetSource
std::string targetSource;
int indexOfDoubleValue = 0;

//the number of sources in every room
std::vector<std::vector<int>> sources;

std::vector<int> visited;
//сортировка для вершин списка смежности (так как в тз необходимо зачастую идти в вершину с меньшим номером)
//сортировка вставками так как (как я понимаю из примера) они все же идут в отсортированном порядке (но в условии об этом не сказано), а вставки на отсортированном массиве работают за O(n)
void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i]; 
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
bool isCorrectVertexLine(const std::string& line) {

    std::stringstream ss(line);

    std::string roomNumber;
    std::string adjacency;
    std::string source1;
    std::string source2;
    std::string source3;
    std::string source4;
    std::string extra;

    // должно быть ровно 6 частей
    if(!(ss >> roomNumber >> adjacency >> source1 >> source2 >> source3 >> source4)) {
        return false;
    }

    // не должно быть лишних данных
    if(ss >> extra) {
        return false;
    }

    // номер комнаты только число
    for(char c : roomNumber) {
        if(!isdigit(c)) {
            return false;
        }
    }

    // проверка списка смежности
    if(adjacency.empty()) {
        return false;
    }

    for(int i = 0; i < adjacency.size(); i++) {

        char c = adjacency[i];

        // только цифры и запятые
        if(!isdigit(c) && c != ',') {
            return false;
        }

        // две запятые подряд
        if(c == ',' && i + 1 < adjacency.size() && adjacency[i + 1] == ',') {
            return false;
        }

        // запятая в начале
        if(i == 0 && c == ',') {
            return false;
        }

        // запятая в конце
        if(i == adjacency.size() - 1 && c == ',') {
            return false;
        }
    }

    std::vector<std::string> sourcesToCheck = {
        source1,
        source2,
        source3,
        source4
    };

    // проверка ресурсов
    for(const auto& s : sourcesToCheck) {

        if(s.empty()) {
            return false;
        }

        for(char c : s) {
            if(!isdigit(c)) {
                return false;
            }
        }
    }

    return true;
}

void init() {

    std::string line;
    std::ifstream in("input.txt");

    if(!in.is_open()) {
        std::cout << "File does not exist";
    }

    if(std::getline(in, line)) {
        n = std::stoi(line);
    }
    
    adjList.resize(n+1);
    sources.resize(n+1);
    visited.resize(n+1, 0);
    visited[0] = 1;
    int numberOfStr = 0;
    
    while(std::getline(in, line) && numberOfStr < n) {
        numberOfStr+=1;
        std::stringstream tempstr(line);
        //add number of the room
        int numberOfTheRoom;
        tempstr >> numberOfTheRoom;

        //add adjList for the string
        std::string adjOfStr;
        tempstr >> adjOfStr;

        std::stringstream adjDividing(adjOfStr);
        std::string subToken;
        std::vector<int> strAdjList;
        while (std::getline(adjDividing, subToken, ',')) {
            if (!subToken.empty()) {
                strAdjList.push_back(std::stoi(subToken));
            }
        }
        insertionSort(strAdjList);
        //отправить список смежности в отсортированном виде
        adjList[numberOfTheRoom] = strAdjList;

        std::vector<int> currentRoomSources;
        int numbOfSource;
        int counter = 0;
        while(tempstr >> numbOfSource) {
            currentRoomSources.push_back(numbOfSource);
            counter++;
        }
        if(counter != 4) {
            std::cout << line << " ERROR, counter != 4" << std::endl;
            continue;
        }
        sources[numberOfTheRoom] = currentRoomSources;
        

    }
    if(!isCorrectVertexLine(line)) {
    std::cout << line << std::endl;
    return;
}
    //last string
    if(std::getline(in, line)) {
        std::stringstream lastStr(line);
        lastStr >> m >> targetSource;
    }

    visited[0] = 1;
    in.close();
}

struct value {
    std::string name;
    int value;
};

std::vector<value> defaultValuesWithNames = {
        {"iron", 7},
        {"gold", 11},
        {"gems", 23},
        {"exp", 1}
    };

std::vector<int> defaultValues = {7, 11, 23, 1};

std::vector<int> collectedSources = {0,0,0,0};
// std::map<std::string, int> defaultValues = {
//         {"iron", 7},
//         {"gold", 11},
//         {"gems", 23},
//         {"exp", 1}
//     };

// сделать чтобы оно init передавало все вектора (чтобы не было глобальных переменных)

int FindtheBiggestValueIndex(std::vector<int> RoomSources, std::vector<int> values, int indexOfDoubleValue) {
    if(RoomSources[indexOfDoubleValue]) {
        return indexOfDoubleValue;
    }
    int theBiggestValueIndex = 0;
    for(int i = 1; i < 4; i++) {
        if(RoomSources[i] && values[i] > values[theBiggestValueIndex]) {
            theBiggestValueIndex = i;
        }
    }
    return theBiggestValueIndex;
}

void goToTheRoom(int room, std::vector<int>& visitedStatus, int& currentPos) {
    std::cout << "go " << room << std::endl;
    if(!visitedStatus[room]) {
        visitedStatus[room] = 1;
    }
    m -= 1;
    currentPos = room;
    if(room == 0) {
        return;
    }
    std::cout << "state " << room << " ";
    for (int val : sources[room]) {
        if(val == -1) {
            std::cout << "_ ";
            continue;
        }
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

void ToCollectSource(int room, const std::vector<int>& values, int indexOfDoubleValue, int& profit, bool isFirstharvest) {
    int collectIndex = FindtheBiggestValueIndex(sources[room], values, indexOfDoubleValue);
    std::cout << "collect " << defaultValuesWithNames[collectIndex].name << std::endl;
    //отладка
    // std::cout << values[collectIndex] << " * " << sources[room][collectIndex] << std::endl;
    // отладка
    profit+=(values[collectIndex]*sources[room][collectIndex]);
    collectedSources[collectIndex]+=sources[room][collectIndex];
    sources[room][collectIndex] = -1;
    std::cout << "state " << room << " ";
    for (int j = 0; j < sources[room].size(); j++) {
        if (sources[room][j] == -1) {
            std::cout << "_ ";
        } else {
            std::cout << sources[room][j] << " ";
        }
    }
    std::cout << std::endl;
    
    if(!isFirstharvest) {
        m-=1;
    }
}

int findMin(std::vector<int> arr) {
    int min = 256; // the biggest room number
    // и равно 0 а не 1 так если вектор состоит из одного элемента то есть шанс сразу выйти за границы вектора
    for(int i = 0; i < arr.size(); i++) {
        if(arr[i] < min) {
            min  = arr[i];
        }
    }
    return min;
}

   //the biggest path is 255, because we have only 255 rooms, so 256 is a good value to determine rooms for which distance didnt evaluate
// so 256 is a good value to determine rooms for which distance didnt evaluate
const int TBP = 256;

std::vector<int> bfs(int start) {
    std::vector<int> distances(n + 1, TBP);
    std::queue<int> roomsQueue;

    distances[start] = 0;
    roomsQueue.push(start);
    while (!roomsQueue.empty()) {
        int current = roomsQueue.front();
        roomsQueue.pop();

        for (auto& room : adjList[current]) {
            if (visited[room] == 1 && distances[room] == TBP) {
                distances[room] = distances[current] + 1;
                roomsQueue.push(room);
            }
        }
        
    }

    return distances;
}

void turnBack(int room, std::vector<int>& distancesToZero, std::vector<int>& visited, std::vector<int>& values, int& profit) {
    if(room == 0) {
        return;
    }
    std::vector<int> shortestPaths;
    int sizeOfShortestPath = TBP;
     //отладка
    //         std::cout << " length " << adjList[room].size() << std::endl;
   
    // for(int i = 0; i < adjList[room].size();i++) {
    //     std::cout << i << ": " << adjList[room][i] << std::endl;
    // }
     //отладка

    int distanceFromCurrent = distancesToZero[room];
    while(m > distanceFromCurrent) {
        ToCollectSource(room, values, indexOfDoubleValue, profit, false);
    }

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

    //отладка
    // std::cout << "call next room " << std::endl;
    goToTheRoom(nextRoom, visited, room);

    turnBack(nextRoom, distancesToZero, visited, values, profit);
}

std::vector<int> findPathToNearestUnvisited(int currentRoom) {
    std::vector<int> distances = bfs(currentRoom);
    std::vector<int> parent(n + 1, -1);
    std::queue<int> q;

    std::vector<bool> used(n + 1, false);

    q.push(currentRoom);
    used[currentRoom] = true;
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        for (int next : adjList[current]) {
            if (visited[next] == 1 && !used[next]) {

                used[next] = true;
                parent[next] = current;

                q.push(next);
            }
        }
    }

    int bestUnvisited = -1;
    int bestDistance = TBP;
    int bestVisibleRoom = -1;

    for (int room = 0; room < n; room++) {
        if (visited[room] == 0) {
            for (int adj : adjList[room]) {
                if (visited[adj] == 1 &&
                    distances[adj] != TBP) {
                    int dist = distances[adj] + 1;
                    bool better = false;
                    if (dist < bestDistance) {
                        better = true;
                    }
                    else if (dist == bestDistance &&
                             room < bestUnvisited) {
                        better = true;
                    }
                    if (better) {
                        bestDistance = dist;
                        bestUnvisited = room;
                        bestVisibleRoom = adj;
                    }
                }
            }
        }
    }
    if (bestUnvisited == -1) {
        return {};
    }
    std::vector<int> path;
    int current = bestVisibleRoom;

    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }
    std::reverse(path.begin(), path.end());
    path.push_back(bestUnvisited);
    return path;
}

 void secondStage(int IndexOfCurrentRoom, std::vector<int>& visited, std::vector<int>& values, int& profit) {
    std::vector<int> valuesToStart = bfs(0);
    turnBack(IndexOfCurrentRoom, valuesToStart, visited, values, profit);
    std::cout << "result ";
    for(int i = 0; i < collectedSources.size(); i++) {
        std::cout << collectedSources[i] << " ";
    }
    std::cout << profit << std::endl;
 }

void algorithm() {
    init();
    if(adjList.empty()) {
        return;
    }
    int profit = 0;
    int indexOfDoubleValue = 0;
    if(targetSource == "iron") {
        indexOfDoubleValue = 0;
    }
    else if(targetSource == "gold") {
        indexOfDoubleValue = 1;
    }
    else if(targetSource == "gems") {
        indexOfDoubleValue = 2;
    }
    else if(targetSource == "exp") {
        indexOfDoubleValue = 3;
    }
    std::vector<int> values = defaultValues;
    values[indexOfDoubleValue] *= 2;

    int mconst = m;
    int IndexOfCurrentRoom = 0;

    for(int i = 0; i < mconst / 2; i++) {
        bool wasStep = false;
        for(const auto& room : adjList[IndexOfCurrentRoom]) {
            if(!visited[room]) {
                goToTheRoom(room, visited, IndexOfCurrentRoom);
                ToCollectSource(room, values, indexOfDoubleValue, profit, true);
                wasStep = true;
                break;
            }
        }
        if(!wasStep) {  
            std::vector<int> path = findPathToNearestUnvisited(IndexOfCurrentRoom);
            if (path.empty()) {
                // а что в этом случае делать, в условии этот случай не прописан
                //поэтому идем назад и как только дошли потом делаем return (так как вообще идем назад в цикле, но return спасет)
                secondStage(IndexOfCurrentRoom,visited, values, profit);
                return;
            }
            else {
                int mInTheBeggin = m;
                for(int j = 1; j < path.size(); j++) {
                    bool wasVisited = visited[path[j]];
                    goToTheRoom(path[j], visited, IndexOfCurrentRoom);
                    if(m <= mconst/2) {
                        secondStage(IndexOfCurrentRoom,visited, values, profit);
                        return;
                    }
                    ToCollectSource(path[j], values, indexOfDoubleValue, profit, wasVisited);
                    if(m <= mconst/2) {
                        secondStage(IndexOfCurrentRoom,visited, values, profit);
                        return;
                    }
                }
                int differenceInM = mInTheBeggin - m;
                i+=differenceInM-1;
            }
        }
            
    }

    //2 стадия
    secondStage(IndexOfCurrentRoom,visited, values, profit);
}

int main() {
    std::ofstream out("result.txt");

    if(!out.is_open()) {
        return 1;
    }

    std::streambuf* oldCoutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    algorithm();

    std::cout.rdbuf(oldCoutBuffer);

    out.close();

    return 0;
}