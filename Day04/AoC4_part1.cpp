#include <fstream>
#include <iostream>
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
        vector<char> lineVector;
        for (char c : line) {
            lineVector.emplace_back(c);
        }
        output.push_back(lineVector);
    }

    // Close the file
    file.close();
    return output;
}

struct PathToExplore {
    size_t i;
    size_t j;
    pair<int, int> dir;

    PathToExplore(size_t i, size_t j, pair<int, int> s) {
        this->i = i;
        this->j = j;
        this->dir = s;
    }
};

int main(int argc, char *argv[]) {
    vector<vector<char>> input = readInput("input.txt");

    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    vector<PathToExplore> paths;

    for (size_t i = 0; i < input.size(); i++) {
        for (size_t j = 0; j < input[i].size(); j++) {
            if (input[i][j] == 'X') {
                for (const auto &dir : directions) {
                    paths.emplace_back(i, j, dir);
                }
            }
        }
    }

    char charMap[3] = {'M', 'A', 'S'};
    int total = 0;
    for (const PathToExplore &p : paths) {
        size_t steps = 0;
        size_t i = p.i;
        size_t j = p.j;

        while (steps < 3) {
            i += p.dir.first;
            j += p.dir.second;

            if (i >= input.size() || j >= input[0].size()) {
                break;
            }

            if (input[i][j] != charMap[steps]) {
                break;
            }

            ++steps;
        }

        if (steps == 3) {
            ++total;
        }
    }

    cout << "Total: " << total << "\n";

    return 0;
}