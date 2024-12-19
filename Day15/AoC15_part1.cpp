#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>

#define spaceX 101
#define spaceY 103

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
std::pair<T, T> operator*(const std::pair<T, T> &l, const int64_t &r) {
    return {l.first * r, l.second * r};
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

int64_t min(const vector<int64_t> &v) {
    int64_t min = INT64_MAX;

    for (const int64_t &e : v) {
        if (e < min) {
            min = e;
        }
    }

    return min;
}

int64_t max(const vector<int64_t> &v) {
    int64_t max = INT64_MIN;

    for (const int64_t &e : v) {
        if (e < max) {
            max = e;
        }
    }

    return max;
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

struct Robot {
    pair<int64_t, int64_t> position;
    pair<int64_t, int64_t> velocity;

    Robot(pair<int64_t, int64_t> p, pair<int64_t, int64_t> v) {
        position = p;
        velocity = v;
    }

    Robot() {
        position = pair(0, 0);
        velocity = pair(0, 0);
    }
};

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
            lineVec.push_back(c);
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
            cout << c;
        }
        cout << "\n";
    }
}

void printMovements(const vector<char> &movements) {
    for (const char c : movements) {
        cout << c;
    }
    cout << "\n";
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
            if (map[i][j] == 'O') {
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

    printMap(map);
    printMovements(movements);

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

        pair<int, int> firstBox = {-1, -1};
        pair<uint, uint> newPos = robot;
        while (true) {
            newPos = newPos + getDirection(move);
            if (map[newPos.first][newPos.second] == '#') {
                break;
            }

            if (map[newPos.first][newPos.second] == '.') {
                if (firstBox != pair(-1, -1)) {
                    map[newPos.first][newPos.second] = 'O';
                }

                map[robot.first][robot.second] = '.';
                robot = robot + getDirection(move);
                map[robot.first][robot.second] = '@';
                break;
            }

            if (map[newPos.first][newPos.second] == 'O' && firstBox == pair(-1, -1)) {
                firstBox = newPos;
            }
        }

        printMap(map);
    }

    cout << sumGPS(map) << "\n";

    return 0;
}