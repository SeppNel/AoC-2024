#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

std::ostream &operator<<(std::ostream &out,
                         const std::pair<size_t, size_t> &rhs) {
    out << rhs.first << ", " << rhs.second;
    return out;
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
    pair<uint64_t, uint64_t> Price;

    Combo(pair<uint, uint> a, pair<uint, uint> b, pair<uint64_t, uint64_t> c) {
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
                // c.Price = pair(stoi(numX), stoi(numY));
                c.Price = pair(stoi(numX) + 10000000000000, stoi(numY) + 10000000000000);
            }
            getline(file, line);
        }

        output.push_back(c);
    }

    // Close the file
    file.close();

    return output;
}

void threadFunc(const Combo &combo, uint64_t start, uint64_t stop, pair<uint64_t, uint64_t> *out) {
    for (uint64_t i = start; i < stop; i++) {
        for (uint64_t j = 0; j < 1000000000; j++) {
            uint64_t posX = combo.A.first * i + combo.B.first * j;
            uint64_t posY = combo.A.second * i + combo.B.second * j;

            if (combo.Price == pair(posX, posY)) {
                *out = pair(i, j);
                return;
            }
        }
    }
}

pair<int64_t, int64_t> bruteForcer(Combo combo) {
    vector<thread *> hilos(16);
    vector<pair<uint64_t, uint64_t> *> results(16);

    uint64_t size = 1000000000 / hilos.size();

    for (uint i = 0; i < hilos.size(); i++) {
        uint64_t start = size * i;
        uint64_t stop = size * (i + 1) - 1;

        results[i] = new pair((uint64_t)0, (uint64_t)0);
        hilos[i] = new thread(threadFunc, combo, start, stop, results[i]);
    }

    for (uint i = 0; i < hilos.size(); i++) {
        hilos[i]->join();
    }

    for (const auto &e : results) {
        if (*e != pair((uint64_t)0, (uint64_t)0)) {
            return *e;
        }
    }

    return {-1, -1};
}

std::pair<int64_t, int64_t> solve(Combo c) {
    int64_t a1 = c.A.first;
    int64_t a2 = c.A.second;
    int64_t b1 = c.B.first;
    int64_t b2 = c.B.second;
    int64_t px = c.Price.first;
    int64_t py = c.Price.second;

    // Compute numerator and denominator for b
    int64_t numerator = py * a1 - px * a2;
    int64_t denominator = b2 * a1 - b1 * a2;

    // Check if denominator is 0 (no solution or infinite solutions)
    if (denominator == 0 || numerator % denominator != 0) {
        return {-1, -1}; // No solution
    }

    int64_t b = numerator / denominator;

    // Compute a using the first equation
    if ((px - b * b1) % a1 != 0) {
        return {-1, -1}; // No solution
    }

    int64_t a = (px - b * b1) / a1;

    // Ensure non-negative solutions
    if (a < 0 || b < 0) {
        return {-1, -1}; // No valid solution
    }

    return {a, b};
}

int main() {
    vector<Combo> input = readInput("input.txt");

    uint64_t total = 0;
    for (auto c : input) {
        // pair<uint64_t, uint64_t> times = bruteForcer(c);

        pair<int64_t, int64_t> sol = solve(c);

        // cout << "Combo A = " << sol.first << "\n";
        // cout << "Combo B = " << sol.second << "\n";

        if (sol != pair((int64_t)-1, (int64_t)-1)) {
            total += sol.first * 3 + sol.second;
        }
    }

    cout << "\nTotal: " << total << "\n";

    return 0;
}