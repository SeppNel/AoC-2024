#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<vector<char>> readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // cout << "Error opening the file!";
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

int main(int argc, char *argv[]) {
    vector<vector<char>> input = readInput("input.txt");

    int total = 0;
    size_t rows = input.size();
    size_t cols = input[0].size();
    for (size_t i = 1; i < rows - 1; i++) {
        for (size_t j = 1; j < cols - 1; j++) {
            if (input[i][j] != 'A') {
                continue;
            }

            char topLeft = input[i - 1][j - 1];
            char topRight = input[i - 1][j + 1];
            char bottomLeft = input[i + 1][j - 1];
            char bottomRight = input[i + 1][j + 1];

            if (topLeft == bottomRight || topRight == bottomLeft) {
                continue;
            }

            if ((topLeft == 'M' || topLeft == 'S') &&
                (topRight == 'M' || topRight == 'S') &&
                (bottomLeft == 'M' || bottomLeft == 'S') &&
                (bottomRight == 'M' || bottomRight == 'S')) {
                total++;
            }
        }
    }

    cout << "Total: " << total << "\n";

    return 0;
}