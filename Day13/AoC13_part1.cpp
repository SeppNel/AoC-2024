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

struct Combo {
    pair<uint, uint> A;
    pair<uint, uint> B;
    pair<uint, uint> Price;

    Combo(pair<uint, uint> a, pair<uint, uint> b, pair<uint, uint> c) {
        A = a;
        B = b;
        Price = c;
    }

    Combo() {
        A = pair(0, 0);
        B = pair(0, 0);
        Price = pair(0, 0);
    }
};

vector<Combo> readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    vector<Combo> output;

    string line;
    while (getline(file, line)) {
        if (line == "") {
            continue;
        }

        Combo c;
        for (uint i = 0; i < 3; i++) {
            vector<string> split = string_split(line, ",");

            string del = "+";
            if (i == 2) {
                del = '=';
            }
            vector<string> split2 = string_split(split[0], del);
            // cout << split2[0] << "\n";
            string numX = split2[1];
            vector<string> split3 = string_split(split[1], del);
            string numY = split3[1];

            if (i == 0) {
                c.A = pair(stoi(numX), stoi(numY));
            }
            if (i == 1) {
                c.B = pair(stoi(numX), stoi(numY));
            }
            if (i == 2) {
                c.Price = pair(stoi(numX), stoi(numY));
            }
            getline(file, line);
        }

        output.push_back(c);
    }

    // Close the file
    file.close();

    return output;
}

pair<uint, uint> bruteForcer(Combo combo) {
    for (uint i = 0; i < 100; i++) {
        for (uint j = 0; j < 100; j++) {
            uint posX = combo.A.first * i + combo.B.first * j;
            uint posY = combo.A.second * i + combo.B.second * j;

            if (combo.Price == pair(posX, posY)) {
                return pair(i, j);
            }
        }
    }

    return pair(0, 0);
}

int main() {
    vector<Combo> input = readInput("input.txt");

    uint total = 0;
    for (auto c : input) {
        // cout << c.A << " | " << c.B << " | " << c.Price << "\n";
        pair<uint, uint> times = bruteForcer(c);
        if (times == pair((uint)0, (uint)0)) {
            cout << "No sol\n";
        } else {
            total += times.first * 3 + times.second;
            cout << "Combo = " << times.first * 3 + times.second << "\n";
        }
    }

    cout << "\nTotal: " << total << "\n";

    return 0;
}