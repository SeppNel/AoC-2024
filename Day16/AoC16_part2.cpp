#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

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

struct Move {
    pair<int, int> pos;
    int direction;
    int score;

    bool operator>(const Move &other) {
        return score > other.score;
    }
};

enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

int mod(int a, int b) {
    int result = a % b;
    if (result < 0) {
        result += b;
    }
    return result;
}

int markPathsOfScore(pair<int, int> pos, int direction, int score, int target, const vector<vector<char>> &map, vector<vector<vector<int>>> &minDistance,
                     vector<vector<int>> &seatMap) {
    int r = pos.first;
    int c = pos.second;

    // cout << score << " | " << target << " | " << map[r][c] << "\n";
    if (score == target && map[r][c] == 'E') {
        seatMap[r][c] = 1;
        return 1;
    }
    if (score >= target) {
        return 0;
    }
    if (map[r][c] == '#') {
        return 0;
    }
    // in Dijkstra, we calculated the minimum score for a row, column, direction so if we are above that, we are not on a best path
    if (score > minDistance[r][c][direction]) {
        return 0;
    }
    minDistance[r][c][direction] = score;
    int isBestPath = 0;
    if (direction == NORTH) {
        isBestPath = markPathsOfScore({r - 1, c}, direction, score + 1, target, map, minDistance, seatMap);
    }
    if (direction == EAST) {
        isBestPath = markPathsOfScore({r, c + 1}, direction, score + 1, target, map, minDistance, seatMap);
    }
    if (direction == SOUTH) {
        isBestPath = markPathsOfScore({r + 1, c}, direction, score + 1, target, map, minDistance, seatMap);
    }
    if (direction == WEST) {
        isBestPath = markPathsOfScore({r, c - 1}, direction, score + 1, target, map, minDistance, seatMap);
    }
    if (markPathsOfScore({r, c}, mod(direction + 1, 4), score + 1000, target, map, minDistance, seatMap)) {
        isBestPath = 1;
    }
    if (markPathsOfScore({r, c}, mod(direction - 1, 4), score + 1000, target, map, minDistance, seatMap)) {
        isBestPath = 1;
    }

    if (isBestPath) {
        seatMap[r][c] = 1;
    }
    return isBestPath;
}

int main() {

    vector<vector<char>> map = readInput("input.txt");
    uint size = map.size();
    vector<vector<vector<int>>> minDistance(size, vector<vector<int>>(size, vector<int>(4, __INT_MAX__)));
    vector<vector<int>> seatMap(size, vector<int>(size, 0));

    pair<uint, uint> start;

    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            if (map[r][c] == 'S') {
                start = {r, c};
            }
        }
    }

    priority_queue<Move, vector<Move>, greater<>> pq;

    pq.push({start, EAST, 0});

    int score;

    while (!pq.empty()) {
        Move bestMove = pq.top();
        pq.pop();

        int r = bestMove.pos.first;
        int c = bestMove.pos.second;
        int direction = bestMove.direction;
        score = bestMove.score;

        if (map[r][c] == 'E') {
            break;
        }
        if (map[r][c] == '#') {
            continue;
        }
        if (score >= minDistance[r][c][direction]) {
            continue;
        }

        minDistance[r][c][direction] = score;
        pq.push({{r, c}, mod(direction + 1, 4), score + 1000});
        pq.push({{r, c}, mod(direction - 1, 4), score + 1000});

        if (direction == NORTH) {
            pq.push({{r - 1, c}, direction, score + 1});
        }
        if (direction == EAST) {
            pq.push({{r, c + 1}, direction, score + 1});
        }
        if (direction == SOUTH) {
            pq.push({{r + 1, c}, direction, score + 1});
        }
        if (direction == WEST) {
            pq.push({{r, c - 1}, direction, score + 1});
        }
    }

    // Part 2 starts here
    int numSeats = 0;
    markPathsOfScore(start, EAST, 0, score, map, minDistance, seatMap);
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            if (seatMap[r][c]) {
                cout << "O";
                numSeats++;
            } else {
                cout << map[r][c];
            }
        }
        cout << "\n";
    }
    cout << numSeats << "\n";
}
