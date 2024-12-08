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

vector<pair<uint64_t, vector<uint64_t>>> readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // cout << "Error opening the file!";
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

uint64_t calculateResult(const vector<uint64_t> &numbers, const vector<bool> &mask) {
    uint64_t result = numbers[0];
    for (size_t i = 0; i < mask.size(); i++) {
        if (mask[i]) {
            result *= numbers[i + 1];
        } else {
            result += numbers[i + 1];
        }
    }
    return result;
}

void generateCombinations(size_t n, vector<vector<bool>> &combinations) {
    size_t maxMask = static_cast<size_t>(pow(2, n));
    for (size_t mask = 0; mask < maxMask; mask++) {
        vector<bool> combination;
        for (size_t i = 0; i < n; i++) {
            combination.push_back(mask & (1 << i));
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

        vector<vector<bool>> combinations;
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