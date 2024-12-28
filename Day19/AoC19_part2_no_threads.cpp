#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

template <typename T>
bool inVector(const T &needle, const vector<T> &haystack) {
    for (const T &e : haystack) {
        if (needle == e) {
            return true;
        }
    }

    return false;
}

template <typename T>
bool inSet(const T &needle, const unordered_set<T> &haystack) {
    if (haystack.find(needle) != haystack.end()) {
        return true;
    }

    return false;
}

template <typename T, typename U>
bool inMap(const T &needle, const unordered_map<T, U> &haystack) {
    if (haystack.find(needle) != haystack.end()) {
        return true;
    }

    return false;
}

vector<string> string_split(string str, string del) {
    vector<string> output;
    // Find first occurrence of the delimiter
    auto pos = str.find(del);

    // While there are still delimiters in the string
    while (pos != string::npos) {

        // Extracting the substring up to the delimiter
        output.push_back(str.substr(0, pos));

        // Erase the extracted part from the original string
        str.erase(0, pos + del.length());

        // Find the next occurrence of the delimiter
        pos = str.find(del);
    }

    output.push_back(str);
    return output;
}

void readInput(string filename, vector<string> &primitives, vector<string> &lines) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    string line;
    getline(file, line);
    for (const string &s : string_split(line, ", ")) {
        primitives.push_back(s);
    }

    getline(file, line);
    while (getline(file, line)) {
        lines.push_back(line);
    }

    // Close the file
    file.close();
}

uint64_t solve(const string line, const vector<string> &primitives, const uint maxPrimitiveSize) {
    static unordered_map<string, uint64_t> cache;
    if (line.empty()) {
        return 1;
    }

    uint64_t count = 0;
    for (uint i = maxPrimitiveSize; i >= 1; i--) {
        if (line.size() < i) {
            continue;
        }

        if (inVector(line.substr(0, i), primitives)) {
            string remainder = line.substr(i);
            if (!inMap(remainder, cache)) {
                cache[remainder] = solve(line.substr(i), primitives, maxPrimitiveSize);
            }

            count += cache[remainder];
        }
    }

    return count;
}

uint calcMaxPrimitiveSize(const vector<string> &v) {
    uint out = 0;

    for (const string &p : v) {
        if (p.size() > out) {
            out = p.size();
        }
    }

    return out;
}

int main() {
    using namespace chrono;

    auto start = high_resolution_clock::now();

    vector<string> primitives;
    vector<string> lines;
    readInput("input.txt", primitives, lines);

    uint64_t total = 0;

    for (const string &s : lines) {
        total += solve(s, primitives, calcMaxPrimitiveSize(primitives));
    }

    cout << total << "\n";

    auto end = high_resolution_clock::now();
    cout << "Time (ms): " << duration_cast<milliseconds>(end - start).count() << "\n";

    return 0;
}
