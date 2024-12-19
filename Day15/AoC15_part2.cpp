#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::pair<T, T> &rhs) {
    out << rhs.first << ", " << rhs.second;
    return out;
}

template <typename T, typename U>
std::pair<T, T> operator+(const std::pair<T, T> &l, const std::pair<U, U> &r) {
    return {l.first + r.first, l.second + r.second};
}

template <typename T>
bool inVector(const T &needle, const vector<T> &haystack) {
    for (const T &e : haystack) {
        if (needle == e) {
            return true;
        }
    }

    return false;
}

void push_back_unique(vector<pair<uint, uint>> &v, pair<uint, uint> e) {
    if (!inVector(e, v)) {
        v.push_back(e);
    }
}

vector<char> readInput(string filename, vector<vector<char>> &map) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    vector<char> movements;

    string line;
    while (getline(file, line)) {
        if (line == "") {
            break;
        }

        vector<char> lineVec;

        for (char c : line) {
            if (c == '#' || c == '.') {
                lineVec.push_back(c);
                lineVec.push_back(c);
            } else if (c == '@') {
                lineVec.push_back(c);
                lineVec.push_back('.');
            } else {
                lineVec.push_back('[');
                lineVec.push_back(']');
            }
        }

        map.push_back(lineVec);
    }

    while (getline(file, line)) {
        for (char c : line) {
            movements.push_back(c);
        }
    }

    // Close the file
    file.close();

    return movements;
}

void printMap(const vector<vector<char>> &map) {
    for (const auto &row : map) {
        for (const char c : row) {
            cerr << c;
        }
        cerr << "\n";
        cerr << std::flush;
    }
}

pair<uint, uint> findRobot(const vector<vector<char>> &map) {
    for (uint i = 0; i < map.size(); i++) {
        for (uint j = 0; j < map[0].size(); j++) {
            if (map[i][j] == '@') {
                return {i, j};
            }
        }
    }
}

uint sumGPS(const vector<vector<char>> &map) {
    uint sum = 0;
    for (uint i = 0; i < map.size(); i++) {
        for (uint j = 0; j < map[0].size(); j++) {
            if (map[i][j] == '[') {
                sum += 100 * i + j;
            }
        }
    }

    return sum;
}

pair<int, int> getDirection(char c) {
    switch (c) {
    case '<':
        return {0, -1};
    case '>':
        return {0, 1};
    case '^':
        return {-1, 0};
    case 'v':
        return {1, 0};
    }
}

int main() {
    vector<vector<char>> map;
    vector<char> movements = readInput("input.txt", map);
    pair<uint, uint> robot = findRobot(map);

    for (const char move : movements) {
        if (move == '<' && robot.second <= 0) {
            continue;
        }

        if (move == '>' && robot.second >= map[0].size() - 1) {
            continue;
        }

        if (move == 'v' && robot.first >= map.size() - 1) {
            continue;
        }

        if (move == '^' && robot.first <= 0) {
            continue;
        }

        vector<pair<uint, uint>> toMove;
        queue<pair<uint, uint>> toCheck;
        bool freeSpace = true;

        toCheck.push(robot + getDirection(move));
        for (; !toCheck.empty(); toCheck.pop()) {
            pair<uint, uint> posCheck = toCheck.front();

            if (map[posCheck.first][posCheck.second] == '#') {
                freeSpace = false;
                break;
            }

            if (map[posCheck.first][posCheck.second] == '.') {
                continue;
            }

            pair<uint, uint> leftBox;
            pair<uint, uint> rightBox;
            if (map[posCheck.first][posCheck.second] == '[') {
                leftBox = posCheck;
                rightBox = posCheck + pair(0, 1);
            } else if (map[posCheck.first][posCheck.second] == ']') {
                leftBox = posCheck + pair(0, -1);
                rightBox = posCheck;
            }

            if (move == '<') {
                push_back_unique(toMove, rightBox);
                push_back_unique(toMove, leftBox);

                toCheck.push(leftBox + getDirection(move));
            } else if (move == '>') {
                push_back_unique(toMove, leftBox);
                push_back_unique(toMove, rightBox);

                toCheck.push(rightBox + getDirection(move));
            } else {
                push_back_unique(toMove, leftBox);
                push_back_unique(toMove, rightBox);

                toCheck.push(leftBox + getDirection(move));
                toCheck.push(rightBox + getDirection(move));
            }
        }

        if (!freeSpace) {
            continue;
        }

        for (auto i = toMove.rbegin(); i < toMove.rend(); i++) {
            pair<uint, uint> posToMove = *i;
            pair<uint, uint> newPos = posToMove + getDirection(move);

            char c = map[posToMove.first][posToMove.second];
            map[newPos.first][newPos.second] = c;
            map[posToMove.first][posToMove.second] = '.';
        }

        pair<uint, uint> newRobotPos = robot + getDirection(move);
        map[robot.first][robot.second] = '.';
        map[newRobotPos.first][newRobotPos.second] = '@';

        robot = newRobotPos;
    }

    cout << sumGPS(map) << "\n";

    return 0;
}