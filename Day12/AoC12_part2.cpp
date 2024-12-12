#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
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

    int64_t getSides(const vector<vector<char>> &garden) const {
        int64_t corners = 0;
        for (const auto &pos : positions) {
            // Bounds Checks
            char posUp = '0';
            char posDown = '0';
            char posRight = '0';
            char posLeft = '0';

            if (pos.first > 0) {
                posUp = garden[pos.first - 1][pos.second];
            }

            if (pos.first < garden.size() - 1) {
                posDown = garden[pos.first + 1][pos.second];
            }

            if (pos.second > 0) {
                posLeft = garden[pos.first][pos.second - 1];
            }

            if (pos.second < garden[0].size() - 1) {
                posRight = garden[pos.first][pos.second + 1];
            }

            // Corner checks
            if (posUp == type && posRight == type) {
                pair<uint, uint> corner = pos + pair(-1, 1);

                if (garden[corner.first][corner.second] != type) {
                    corners++;
                }
            }

            if (posUp == type && posLeft == type) {
                pair<uint, uint> corner = pos + pair(-1, -1);

                if (garden[corner.first][corner.second] != type) {
                    corners++;
                }
            }

            if (posDown == type && posLeft == type) {
                pair<uint, uint> corner = pos + pair(1, -1);

                if (garden[corner.first][corner.second] != type) {
                    corners++;
                }
            }

            if (posDown == type && posRight == type) {
                pair<uint, uint> corner = pos + pair(1, 1);

                if (garden[corner.first][corner.second] != type) {
                    corners++;
                }
            }

            if (posUp != type && posRight != type) {
                corners++;
            }

            if (posUp != type && posLeft != type) {
                corners++;
            }

            if (posDown != type && posRight != type) {
                corners++;
            }

            if (posDown != type && posLeft != type) {
                corners++;
            }
        }

        return corners;
    }
};

void exploreRegion(const vector<vector<char>> &garden, char plantType, pair<uint, uint> pos, Region &reg) {
    if (garden[pos.first][pos.second] != plantType) {
        return;
    }

    if (reg.positions.find(pos) != reg.positions.end()) {
        return;
    }

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

vector<Region> findRegions(const vector<vector<char>> &garden, char plantType) {
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
    vector<vector<char>> input = readInput("input.txt");
    unordered_set<char> plantTypes = findPlantTypes(input);

    int64_t total = 0;
    for (const char &type : plantTypes) {
        vector<Region> regionsVec = findRegions(input, type);

        for (const Region &r : regionsVec) {
            // cout << type << " Sides = " << r.getSides(input) << "\n";
            total += r.getArea() * r.getSides(input);
        }
    }

    cout << total << "\n";

    return 0;
}