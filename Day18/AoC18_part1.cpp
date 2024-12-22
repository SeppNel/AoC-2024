#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

#define GRID_SIZE 71

using Upair = pair<uint, uint>;
using Grid = vector<vector<char>>;

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

vector<string> string_split(string str, string del) {
    vector<string> output;
    // Find first occurrence of the delimiter
    auto pos = str.find(del);

    // While there are still delimiters in the
    // string
    while (pos != string::npos) {

        // Extracting the substring up to the
        // delimiter
        output.push_back(str.substr(0, pos));

        // Erase the extracted part from the
        // original string
        str.erase(0, pos + del.length());

        // Find the next occurrence of the
        // delimiter
        pos = str.find(del);
    }

    output.push_back(str);
    return output;
}

vector<Upair> readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    vector<Upair> output;

    string line;
    while (getline(file, line)) {
        vector<string> str = string_split(line, ",");

        output.emplace_back(stoi(str[0]), stoi(str[1]));
    }

    // Close the file
    file.close();

    return output;
}

struct NodePq {
    Upair pos;
    uint dist;

    bool operator>(const NodePq &o) {
        return dist > o.dist;
    }
};

Grid prepareGrid(const uint n, const vector<Upair> &positions) {
    Grid grid(GRID_SIZE, vector<char>(GRID_SIZE, '.'));

    for (uint i = 0; i < n; i++) {
        Upair pos = positions[i];

        grid[pos.second][pos.first] = '#';
    }

    return grid;
}

void printGrid(const Grid &g) {
    for (uint i = 0; i < GRID_SIZE; i++) {
        for (uint j = 0; j < GRID_SIZE; j++) {
            cout << g[i][j];
        }
        cout << "\n";
    }
}

void initializeDistances(unordered_map<Upair, uint, pairHash> &distances) {
    for (uint i = 0; i < GRID_SIZE; i++) {
        for (uint j = 0; j < GRID_SIZE; j++) {
            distances[pair(i, j)] = UINT32_MAX;
        }
    }
    distances[pair(0, 0)] = 0;
}

unordered_map<Upair, uint, pairHash> dijkstra(const Grid &grid, Upair start, Upair end) {
    unordered_map<Upair, uint, pairHash> distances;
    initializeDistances(distances);

    unordered_set<Upair, pairHash> visited;
    priority_queue<NodePq, vector<NodePq>, greater<>> pq;
    pq.push({start, 0});

    while (!pq.empty()) {
        NodePq curr = pq.top();
        pq.pop();

        if (visited.find(curr.pos) != visited.end()) {
            continue;
        }

        visited.insert(curr.pos);

        // Directions for movement (up, down, left, right)
        vector<pair<int, int>> directions = {pair(-1, 0), pair(1, 0), pair(0, -1), pair(0, 1)};

        for (auto neighborOffset : directions) {
            if ((curr.pos.first == 0 && neighborOffset.first == -1) ||
                (curr.pos.first == GRID_SIZE - 1 && neighborOffset.first == 1) ||
                (curr.pos.second == 0 && neighborOffset.second == -1) ||
                (curr.pos.second == GRID_SIZE - 1 && neighborOffset.second == 1)) {
                continue;
            }

            Upair neighbor = curr.pos + neighborOffset;

            if (grid[neighbor.first][neighbor.second] == '#') {
                continue;
            }

            uint tentative_distance = distances[curr.pos] + 1;

            if (tentative_distance < distances[neighbor]) {
                distances[neighbor] = tentative_distance;
                pq.push({neighbor, tentative_distance});
            }
        }
    }
    return distances;
}

int main() {
    vector<Upair> input = readInput("input.txt");

    Grid grid = prepareGrid(1024, input);

    printGrid(grid);

    unordered_map<Upair, uint, pairHash> distances = dijkstra(grid, {0, 0}, {6, 6});

    cout << distances[{
                70,
                70,
            }]
         << "\n";

    return 0;
}
