#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
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

// Hash function
struct pairHash {
    size_t operator()(const pair<int,
                                 int> &x) const {
        return ((x.first + x.second) * (x.first + x.second + 1) / 2) + x.second;
    }
};

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

template <typename T>
bool inVector(T needle, vector<T> haystack) {
    for (auto e : haystack) {
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

char rotate(char c) {
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

void printPositionsInMap(const vector<vector<char>> &map, const unordered_set<pair<uint, uint>, pairHash> &visited) {
    size_t rows = map.size();
    size_t cols = map[0].size();

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (visited.find(pair(i, j)) != visited.end()) {
                cout << "X ";
            } else {
                cout << map[i][j] << " ";
            }
        }
        cout << "\n";
    }
}

int main(int argc, char *argv[]) {
    unordered_map<char, pair<int, int>> positions;
    positions['^'] = pair(-1, 0);
    positions['<'] = pair(0, -1);
    positions['>'] = pair(0, 1);
    positions['v'] = pair(1, 0);

    vector<vector<char>> map = readInput("input.txt");

    pair<uint, uint> pos = findGuard(map);
    char guard = map[pos.first][pos.second];

    size_t rows = map.size();
    size_t cols = map[0].size();

    unordered_set<pair<uint, uint>, pairHash> visitedPositions;
    // visitedPositions.insert(pos);

    pair<int, int> nextPos = pos + (pair<int, int>)positions[guard];

    while (nextPos.first >= 0 && nextPos.first < rows &&
           nextPos.second >= 0 && nextPos.second < cols) {

        if (map[nextPos.first][nextPos.second] == '#') {
            guard = rotate(guard);
        } else {
            visitedPositions.insert(pos);
            pos = nextPos;
        }

        nextPos = pos + (pair<int, int>)positions[guard];
    }

    visitedPositions.insert(pos);

    cout << visitedPositions.size() << "\n";

    printPositionsInMap(map, visitedPositions);

    return 0;
}