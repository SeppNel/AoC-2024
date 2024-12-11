#include <fstream>
#include <iostream>
#include <string>
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

vector<vector<uint>> readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    vector<vector<uint>> output;

    string line;
    while (getline(file, line)) {
        vector<uint> lineVec;
        for (const char &c : line) {
            if (c == '.') {
                lineVec.push_back(9000);
                continue;
            }
            uint num = c - '0';
            lineVec.push_back(num);
        }

        output.push_back(lineVec);
    }

    // Close the file
    file.close();

    return output;
}

vector<pair<uint, uint>> findStarts(const vector<vector<uint>> &map) {
    vector<pair<uint, uint>> out;
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[0].size(); j++) {
            if (map[i][j] == 0) {
                out.emplace_back(i, j);
            }
        }
    }

    return out;
}

uint recursiveSolve(const vector<vector<uint>> &map, pair<uint, uint> pos, uint expected, uint &paths) {
    if (expected == 9 && map[pos.first][pos.second] == 9) {
        return 1;
    }

    uint score = 0;

    if (map[pos.first][pos.second] == expected) {
        if (pos.first > 0 && recursiveSolve(map, pos + pair(-1, 0), expected + 1, paths)) {
            score++;
        }
        if (pos.second > 0 && recursiveSolve(map, pos + pair(0, -1), expected + 1, paths)) {
            score++;
        }
        if (pos.first < map.size() - 1 && recursiveSolve(map, pos + pair(1, 0), expected + 1, paths)) {
            score++;
        }
        if (pos.second < map[0].size() - 1 && recursiveSolve(map, pos + pair(0, 1), expected + 1, paths)) {
            score++;
        }
    }

    if (score > 0) {
        paths += score - 1;
    }

    return score;
}

int main() {
    vector<vector<uint>> input = readInput("input.txt");

    vector<pair<uint, uint>> starts = findStarts(input);

    uint total = 0;

    for (pair<uint, uint> e : starts) {
        uint paths = 1;

        recursiveSolve(input, e, 0, paths);

        total += paths;
    }

    cout << "Total paths = " << total << "\n";

    return 0;
}