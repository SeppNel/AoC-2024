#include <cmath>
#include <cstdint>
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

vector<pair<uint64_t, vector<uint64_t>>> readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    vector<pair<uint64_t, vector<uint64_t>>> output;

    string line;
    while (getline(file, line)) {
        pair<uint64_t, vector<uint64_t>> linePair;

        vector<string> tmp = string_split(line, ":");
        linePair.first = stoull(tmp[0]);
        vector<string> tmp2 = string_split(tmp[1], " ");
        for (const string &e : tmp2) {
            if (e == "") {
                continue;
            }
            linePair.second.push_back(stoull(e));
        }

        output.push_back(linePair);
    }

    // Close the file
    file.close();

    return output;
}

uint64_t calculateResult(const vector<uint64_t> &numbers, const vector<int> &mask) {
    uint64_t result = numbers[0];
    uint64_t concatValue = numbers[0]; // Used for concatenation

    for (size_t i = 0; i < mask.size(); i++) {
        if (mask[i] == 0) { // Addition
            result += numbers[i + 1];
        } else if (mask[i] == 1) { // Multiplication
            result *= numbers[i + 1];
        } else if (mask[i] == 2) { // Concatenation
            result = stoull(to_string(result) + to_string(numbers[i + 1]));
        }
    }

    return result;
}

void generateCombinations(size_t n, vector<vector<int>> &combinations) {
    size_t maxMask = static_cast<size_t>(pow(3, n));
    for (size_t mask = 0; mask < maxMask; mask++) {
        vector<int> combination;
        size_t tmp = mask;
        for (size_t i = 0; i < n; i++) {
            combination.push_back(tmp % 3);
            tmp /= 3;
        }
        combinations.push_back(combination);
    }
}

int main() {
    vector<pair<uint64_t, vector<uint64_t>>> input = readInput("input.txt");
    uint64_t total = 0;

    for (const auto &e : input) {
        const uint64_t &testValue = e.first;
        const vector<uint64_t> &numbers = e.second;

        if (numbers.size() == 1) {
            if (numbers[0] == testValue) {
                total++;
            }
            continue;
        }

        vector<vector<int>> combinations;
        generateCombinations(numbers.size() - 1, combinations);

        for (const auto &mask : combinations) {
            if (calculateResult(numbers, mask) == testValue) {
                total += testValue;
                break;
            }
        }
    }

    cout << "Total: " << total << endl;
    return 0;
}