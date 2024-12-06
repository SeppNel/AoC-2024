#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

std::ostream &operator<<(std::ostream &out,
                         const std::pair<size_t, size_t> &rhs) {
    out << rhs.first << ", " << rhs.second;
    return out;
}

template <typename T, typename U>
std::pair<T, T> operator+(const std::pair<T, T> &l, const std::pair<U, U> &r) {
    return {l.first + r.first, l.second + r.second};
}

struct VisitedPosition {
    uint x;
    uint y;
    char dir;

    bool operator==(const VisitedPosition &o) {
        return x == o.x && y == o.y && dir == o.dir;
    }

    bool operator==(const VisitedPosition &o) const {
        return x == o.x && y == o.y && dir == o.dir;
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

pair<uint, uint> findGuard(const vector<vector<char>> &map) {
    size_t rows = map.size();
    size_t cols = map[0].size();

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (map[i][j] == '^' ||
                map[i][j] == '>' ||
                map[i][j] == '<' ||
                map[i][j] == 'v') {
                return pair(i, j);
            }
        }
    }
}

char rotate(const char c) {
    char rotated;
    if (c == '^') {
        rotated = '>';
    } else if (c == '>') {
        rotated = 'v';
    } else if (c == 'v') {
        rotated = '<';
    } else {
        rotated = '^';
    }

    return rotated;
}

void insertIfFullyUnique(vector<VisitedPosition> &v, const VisitedPosition &p) {
    for (VisitedPosition c : v) {
        if (c == p) {
            return;
        }
    }

    v.push_back(p);
}

int main(int argc, char *argv[]) {
    unordered_map<char, pair<int, int>> positions;
    positions['^'] = pair(-1, 0);
    positions['<'] = pair(0, -1);
    positions['>'] = pair(0, 1);
    positions['v'] = pair(1, 0);

    vector<vector<char>> map = readInput("input.txt");

    pair<uint, uint> initialPos = findGuard(map);
    pair<uint, uint> pos = initialPos;
    char guard = map[pos.first][pos.second];

    size_t rows = map.size();
    size_t cols = map[0].size();

    vector<VisitedPosition> visitedPath;
    visitedPath.reserve(1000);

    pair<int, int> nextPos = pos + (pair<int, int>)positions[guard];

    while (nextPos.first >= 0 && nextPos.first < rows &&
           nextPos.second >= 0 && nextPos.second < cols) {

        if (map[nextPos.first][nextPos.second] == '#') {
            guard = rotate(guard);
        } else {
            VisitedPosition p = {pos.first, pos.second, guard};
            insertIfFullyUnique(visitedPath, p);
            pos = nextPos;
        }

        nextPos = pos + (pair<int, int>)positions[guard];
    }

    // Part2 -----------------------------------------------------

    vector<pair<int, int>> triedObstacles;
    triedObstacles.reserve(visitedPath.size());

    uint total = 0;
    for (const VisitedPosition &vp : visitedPath) {
        pair<int, int> nextPos = pair(vp.x, vp.y) + (pair<int, int>)positions[vp.dir];
        if (map[nextPos.first][nextPos.second] == '#') {
            continue;
        }
        if (inVector(nextPos, triedObstacles)) {
            continue;
        }
        triedObstacles.push_back(nextPos);

        vector<VisitedPosition> newVisitedPath;
        pair<int, int> newObstacle = nextPos;

        pair<uint, uint> pos = initialPos;
        char guard = map[pos.first][pos.second];
        nextPos = pos + (pair<int, int>)positions[guard];

        while (nextPos.first >= 0 && nextPos.first < rows &&
               nextPos.second >= 0 && nextPos.second < cols) {
            VisitedPosition p = {pos.first, pos.second, guard};
            if (inVector(p, newVisitedPath)) {
                total++;
                break;
            }

            if (map[nextPos.first][nextPos.second] == '#' || nextPos == newObstacle) {
                guard = rotate(guard);
            } else {
                newVisitedPath.push_back(p);
                pos = nextPos;
            }

            nextPos = pos + (pair<int, int>)positions[guard];
        }
    }

    cout << "Total: " << total << "\n";

    return 0;
}