#include <cmath>
#include <cstddef>
#include <cstdint>
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
        // cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    vector<vector<char>> output;

    string line;
    while (getline(file, line)) {
        vector<char> lineVec;
        for (const char &c : line) {
            lineVec.push_back(c);
        }

        output.push_back(lineVec);
    }

    // Close the file
    file.close();

    return output;
}

unordered_set<char> findDifferentAntenas(const vector<vector<char>> &map) {
    unordered_set<char> output;
    for (const vector<char> &col : map) {
        for (const char &e : col) {
            if (e != '.') {
                output.insert(e);
            }
        }
    }

    return output;
}

vector<pair<uint, uint>> findAntenas(const char &type, const vector<vector<char>> &map) {
    size_t rows = map.size();
    size_t cols = map[0].size();

    vector<pair<uint, uint>> output;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (map[i][j] == type) {
                output.emplace_back(i, j);
            }
        }
    }

    return output;
}

vector<pair<uint, uint>> calculateAntinodes(const pair<uint, uint> &p1, const pair<uint, uint> &p2, const vector<vector<char>> &map) {
    vector<pair<uint, uint>> output;

    int distX = p1.first - p2.first;
    int distY = p1.second - p2.second;

    int antinode1_posX = p1.first + distX;
    int antinode1_posY = p1.second + distY;

    if (antinode1_posX >= 0 && antinode1_posX < map.size() && antinode1_posY >= 0 && antinode1_posY < map[0].size()) {
        output.emplace_back(antinode1_posX, antinode1_posY);
    }

    int antinode2_posX = p2.first - distX;
    int antinode2_posY = p2.second - distY;

    if (antinode2_posX >= 0 && antinode2_posX < map.size() && antinode2_posY >= 0 && antinode2_posY < map[0].size()) {
        output.emplace_back(antinode2_posX, antinode2_posY);
    }

    return output;
}

int main() {
    vector<vector<char>> input = readInput("input.txt");
    uint64_t total = 0;

    unordered_set<char> antenaTypes = findDifferentAntenas(input);

    unordered_set<pair<uint, uint>, pairHash> allAntinodes;

    for (const char &antenaType : antenaTypes) {
        vector<pair<uint, uint>> antenas = findAntenas(antenaType, input);
        for (size_t i = 0; i < antenas.size(); i++) {
            pair<uint, uint> currentAntena = antenas[i];

            for (size_t j = 0; j < antenas.size(); j++) {
                if (i == j) {
                    continue;
                }

                vector<pair<uint, uint>> antinodes = calculateAntinodes(currentAntena, antenas[j], input);

                for (const auto &p : antinodes) {
                    allAntinodes.insert(p);
                }
            }
        }
    }

    total = allAntinodes.size();

    cout << "Total: " << total << endl;
    return 0;
}