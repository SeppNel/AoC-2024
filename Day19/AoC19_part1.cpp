#include <fstream>
#include <iostream>
#include <string>
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

void readInput(string filename, unordered_set<string> &primitives, vector<string> &lines) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    string line;
    getline(file, line);
    for (const string &s : string_split(line, ", ")) {
        primitives.insert(s);
    }

    getline(file, line);
    while (getline(file, line)) {
        lines.push_back(line);
    }

    // Close the file
    file.close();
}

bool solve(string line, unordered_set<string> primitives, uint maxPrimitiveSize) {
    // cout << "--" << line << "\n";
    if (line.empty()) {
        return true;
    }

    for (uint i = maxPrimitiveSize; i >= 1; i--) {
        if (line.size() < i) {
            continue;
        }

        string subStr = line.substr(0, i);
        // cout << "##" << subStr << "\n";
        if (inSet(subStr, primitives)) {
            // if (inVector(line.substr(0, i), primitives)) {
            if (solve(line.substr(i, line.size() - i), primitives, maxPrimitiveSize)) {
                return true;
            }
        }
    }

    return false;
}

uint calcMaxPrimitiveSize(const unordered_set<string> &v) {
    uint out = 0;

    for (const string &p : v) {
        if (p.size() > out) {
            out = p.size();
        }
    }

    return out;
}

int main() {
    unordered_set<string> primitives;
    vector<string> lines;
    readInput("input.txt", primitives, lines);

    vector<string> to_remove;
    for (const auto &p : primitives) {
        if (solve(p, primitives, p.size() - 1)) {
            to_remove.push_back(p);
        }
    }

    for (const string &a : to_remove) {
        primitives.erase(a);
    }

    uint count = 0;

    for (const string &s : lines) {
        count += solve(s, primitives, calcMaxPrimitiveSize(primitives));
    }

    cout << count << "\n";

    return 0;
}
