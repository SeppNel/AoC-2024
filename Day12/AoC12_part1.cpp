#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
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
        for (const char &s : line) {
            lineVec.push_back(s);
        }

        output.push_back(lineVec);
    }

    // Close the file
    file.close();

    return output;
}

unordered_set<char> findPlantTypes(const vector<vector<char>> &garden) {
    unordered_set<char> out;
    for (const auto &r : garden) {
        for (const auto &e : r) {
            out.insert(e);
        }
    }

    return out;
}

struct Region {
    unordered_set<pair<uint, uint>, pairHash> positions;
    char type;

    size_t operator==(const Region &r) const {
        return r.positions == positions;
    }

    Region(char c) {
        type = c;
    }

    uint getArea() const {
        return positions.size();
    }

    uint getPerimeter(const vector<vector<char>> &garden) const {
        size_t rows = garden.size();
        size_t cols = garden[0].size();

        vector<pair<int, int>> checkOffsets = {pair(0, 1), pair(1, 0), pair(0, -1), pair(-1, 0)};

        uint total = 0;
        for (const auto &p : positions) {
            uint posPeri = 0;
            for (const auto offset : checkOffsets) {
                pair<int, int> checkPos = p + offset;
                if (checkPos.first < 0 || checkPos.first >= rows || checkPos.second < 0 || checkPos.second >= cols) {
                    posPeri++;
                    continue;
                }

                if (garden[checkPos.first][checkPos.second] != type) {
                    posPeri++;
                }
            }
            total += posPeri;
        }

        return total;
    }
};

void exploreRegion(const vector<vector<char>> &garden, char plantType, pair<uint, uint> pos, Region &reg) {
    if (garden[pos.first][pos.second] != plantType) {
        return;
    }

    if (reg.positions.find(pos) != reg.positions.end()) {
        return;
    }

    // cout << pos << "\n";

    reg.positions.insert(pos);

    if (pos.first > 0) {
        exploreRegion(garden, plantType, pos + pair(-1, 0), reg);
    }
    if (pos.second > 0) {
        exploreRegion(garden, plantType, pos + pair(0, -1), reg);
    }
    if (pos.first < garden.size() - 1) {
        exploreRegion(garden, plantType, pos + pair(1, 0), reg);
    }
    if (pos.second < garden[0].size() - 1) {
        exploreRegion(garden, plantType, pos + pair(0, 1), reg);
    }
}

vector<Region> findRegion(const vector<vector<char>> &garden, char plantType) {
    size_t rows = garden.size();
    size_t cols = garden[0].size();

    vector<Region> regions;

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (garden[i][j] == plantType) {
                Region reg(plantType);
                exploreRegion(garden, plantType, pair(i, j), reg);

                if (!inVector(reg, regions)) {
                    regions.push_back(reg);
                }
            }
        }
    }

    return regions;
}

int main() {
    vector<vector<char>> input = readInput("exampleInput2.txt");
    unordered_set<char> plantTypes = findPlantTypes(input);

    // vector<Region> a = findRegion(input, 'O');

    uint total = 0;
    for (const char &type : plantTypes) {
        vector<Region> regionsVec = findRegion(input, type);

        for (const Region &r : regionsVec) {
            for (const auto &e : r.positions) {
                // cout << e << "\n";
            }

            total += r.getArea() * r.getPerimeter(input);

            // cout << "Area = " << r.getArea() << ", Perimeter = " << r.getPerimeter(input) << "\n";
            //  cout << "----------\n";
        }
    }

    cout << total << "\n";

    return 0;
}