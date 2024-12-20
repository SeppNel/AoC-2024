#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Directions for movement (up, down, left, right)
vector<pair<int, int>> directions = {pair(-1, 0), pair(1, 0), pair(0, -1), pair(0, 1)};

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::pair<T, T> &rhs) {
    out << rhs.first << ", " << rhs.second;
    return out;
}

template <typename T, typename U>
std::pair<T, T> operator+(const std::pair<T, T> &l, const std::pair<U, U> &r) {
    return {l.first + r.first, l.second + r.second};
}

// Hash function
struct pairHash {
    size_t operator()(const pair<int,
                                 int> &x) const {
        return ((x.first + x.second) * (x.first + x.second + 1) / 2) + x.second;
    }
};

template <typename T>
bool inVector(const T &needle, const vector<T> &haystack) {
    for (const T &e : haystack) {
        if (needle == e) {
            return true;
        }
    }

    return false;
}

vector<vector<char>> readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    vector<vector<char>> output;

    string line;
    while (getline(file, line)) {
        vector<char> lineVec;

        for (char c : line) {
            lineVec.push_back(c);
        }

        output.push_back(lineVec);
    }

    // Close the file
    file.close();

    return output;
}

struct Node {
    uint distance;
    pair<uint, uint> position;
    pair<int, int> prevPosition;

    bool operator>(const Node &other) {
        return distance > other.distance;
    }
};

int dijkstra(const vector<vector<char>> &grid, pair<uint, uint> start, pair<uint, uint> end, vector<pair<uint, uint>> &path) {
    int rows = grid.size();
    int cols = grid[0].size();

    unordered_map<pair<uint, uint>, pair<uint, uint>, pairHash> predecessor;

    unordered_map<pair<uint, uint>, uint, pairHash> distanceToStart;
    priority_queue<Node, vector<Node>, greater<>> pq;

    // Initialize
    pq.push({0, start, {-1, -1}});
    distanceToStart[start] = 0;

    while (!pq.empty()) {
        Node currentNode = pq.top();
        pq.pop();

        // If the current distance is greater than the stored one, skip
        if (currentNode.distance > distanceToStart[currentNode.position]) {
            continue;
        }

        // If reached the end
        if (currentNode.position == end) {
            // Reconstruct the path
            path.clear();
            for (pair<uint, uint> at = end; at != start; at = predecessor[at]) {
                path.push_back(at);
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return currentNode.distance;
        }

        // Explore neighbors
        for (int i = 0; i < 4; ++i) {
            pair<int, int> neighbor = currentNode.position + directions[i];

            if (neighbor.first < 0 || neighbor.first >= rows || neighbor.second < 0 || neighbor.second >= cols) {
                continue;
            }

            if (grid[neighbor.first][neighbor.second] == '#') {
                continue;
            }

            uint newDist;
            if (currentNode.prevPosition == pair(-1, -1)) {

                if ((pair<uint, uint>)neighbor == start + pair(0, 1)) {
                    newDist = currentNode.distance + 1;
                } else {
                    newDist = currentNode.distance + 1001;
                }
            } else if (currentNode.prevPosition.first != neighbor.first && currentNode.prevPosition.second != neighbor.second) {
                newDist = currentNode.distance + 1001;
            } else {
                newDist = currentNode.distance + 1;
            }

            // If the neighbor has not been visited or can be relaxed
            if (distanceToStart.find(neighbor) == distanceToStart.end() || newDist < distanceToStart[neighbor]) {
                distanceToStart[neighbor] = newDist;
                pq.push({newDist, neighbor, currentNode.position});

                predecessor[neighbor] = currentNode.position;
            }
        }
    }

    return -1; // Return -1 if no path exists
}

pair<uint, uint> findStart(const vector<vector<char>> &grid) {
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == 'S') {
                return {i, j};
            }
        }
    }
}

pair<uint, uint> findEnd(const vector<vector<char>> &grid) {
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == 'E') {
                return {i, j};
            }
        }
    }
}

void printMap(const vector<vector<char>> &map, const vector<pair<uint, uint>> &path) {

    for (uint i = 0; i < map.size(); i++) {
        for (uint j = 0; j < map[0].size(); j++) {
            if (inVector(pair(i, j), path)) {
                cout << '*';
            } else {
                cout << map[i][j];
            }
        }
        cout << "\n";
    }
}

int main() {
    vector<vector<char>> grid = readInput("input.txt");

    pair<uint, uint> start = findStart(grid);
    pair<uint, uint> end = findEnd(grid);

    vector<pair<uint, uint>> path;

    int shortestPath = dijkstra(grid, start, end, path);

    printMap(grid, path);

    if (shortestPath != -1)
        cout << "Shortest path from S to E is: " << shortestPath << endl;
    else
        cout << "No path exists from S to E." << endl;

    return 0;
}
