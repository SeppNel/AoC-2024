#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define spaceX 101
#define spaceY 103

using namespace std;

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::pair<T, T> &rhs) {
    out << rhs.first << ", " << rhs.second;
    return out;
}

template <typename T, typename U>
std::pair<T, T> operator+(const std::pair<T, T> &l, const std::pair<U, U> &r) {
    return {l.first + r.first, l.second + r.second};
}

template <typename T>
std::pair<T, T> operator*(const std::pair<T, T> &l, const int64_t &r) {
    return {l.first * r, l.second * r};
}

int64_t min(const vector<int64_t> &v) {
    int64_t min = INT64_MAX;

    for (const int64_t &e : v) {
        if (e < min) {
            min = e;
        }
    }

    return min;
}

int64_t max(const vector<int64_t> &v) {
    int64_t max = INT64_MIN;

    for (const int64_t &e : v) {
        if (e < max) {
            max = e;
        }
    }

    return max;
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

struct Robot {
    pair<int64_t, int64_t> position;
    pair<int64_t, int64_t> velocity;

    Robot(pair<int64_t, int64_t> p, pair<int64_t, int64_t> v) {
        position = p;
        velocity = v;
    }

    Robot(pair<int64_t, int64_t> p) {
        position = p;
    }

    Robot() {
        position = pair(0, 0);
        velocity = pair(0, 0);
    }
};

vector<Robot> readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    vector<Robot> output;

    string line;
    while (getline(file, line)) {
        Robot r;

        line = line.substr(2);

        vector<string> split = string_split(line, " v=");
        string pos = split[0];
        string vel = split[1];

        vector<string> posSplit = string_split(pos, ",");
        r.position = {stoull(posSplit[0]), stoull(posSplit[1])};

        vector<string> velSplit = string_split(vel, ",");
        r.velocity = {stoll(velSplit[0]), stoll(velSplit[1])};

        output.push_back(r);
    }

    // Close the file
    file.close();

    return output;
}

int64_t mod(int64_t k, int64_t n) {
    return ((k %= n) < 0) ? k + n : k;
}

pair<int64_t, int64_t> solve(Robot r, uint64_t sec) {
    pair<int64_t, int64_t> velInSec = r.velocity * sec;
    pair<int64_t, int64_t> posInSec = r.position + velInSec;

    posInSec = {mod(posInSec.first, spaceX), mod(posInSec.second, spaceY)};

    return posInSec;
}

vector<int64_t> getCenter(int64_t space) {
    vector<int64_t> out;
    if (space % 2 == 0) {
        out.push_back((space / 2) - 1);
    }

    out.push_back(space / 2);

    return out;
}

void displayRobots(const vector<Robot> &robots) {
    for (size_t i = 0; i < spaceX; i++) {
        for (size_t j = 0; j < spaceY; j++) {
            bool found = false;
            for (const Robot &r : robots) {
                if (r.position == pair((int64_t)i, (int64_t)j)) {
                    cout << "R";
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << ".";
            }
        }
        cout << "\n";
    }
}

int main() {
    vector<Robot> input = readInput("input.txt");

    vector<int64_t> centerX = getCenter(spaceX);
    vector<int64_t> centerY = getCenter(spaceY);

    int64_t q1 = 0;
    int64_t q2 = 0;
    int64_t q3 = 0;
    int64_t q4 = 0;

    uint64_t total = 0;

    uint64_t i = 0;
    while (true) {
        vector<Robot> newRobots;
        newRobots.reserve(input.size());

        for (Robot &r : input) {
            newRobots.emplace_back(solve(r, i));
        }

        cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
        cout << "Sec: " << i << "\n";
        displayRobots(newRobots);

        cin.get();
        i++;
    }
}