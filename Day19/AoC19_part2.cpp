#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

using namespace std;

#define NUM_HILOS 6

std::mutex outMtx;

template <typename T>
bool inVector(const T &needle, const vector<T> &haystack) {
    for (const T &e : haystack) {
        if (needle == e) {
            return true;
        }
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
        lines.emplace_back(line);
    }

    // Close the file
    file.close();
}

uint64_t solve(const string line, const vector<string> &primitives, const uint maxPrimitiveSize) {
    thread_local unordered_map<string, uint64_t> cache; // Thread-local storage
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
                cache[remainder] = solve(remainder, primitives, maxPrimitiveSize);
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

void threadSolve(const vector<string> &lines, const vector<string> &primitives, const uint maxPrimitiveSize, uint64_t *out) {
    uint64_t localOut = 0;
    for (const string &line : lines) {
        localOut += solve(line, primitives, maxPrimitiveSize);
    }

    outMtx.lock();
    *out += localOut;
    outMtx.unlock();
}

int main() {
    auto start = chrono::high_resolution_clock::now();

    vector<string> primitives;
    vector<string> lines;
    readInput("input.txt", primitives, lines);

    vector<thread> hilos;
    hilos.reserve(NUM_HILOS);

    uint64_t total = 0;

    uint linesPerThread = lines.size() / NUM_HILOS;
    uint maxP = calcMaxPrimitiveSize(primitives);

    for (uint i = 0; i < NUM_HILOS; i++) {
        uint start = i * linesPerThread;
        uint end = (i + 1) * linesPerThread;
        if (i == NUM_HILOS - 1) {
            end = lines.size();
        }

        vector<string> tv(&lines[start], &lines[end]);
        hilos.emplace_back(threadSolve, tv, primitives, maxP, &total);
    }

    for (thread &t : hilos) {
        t.join();
    }

    cout << total << "\n";

    auto end = chrono::high_resolution_clock::now();
    cout << "Time (ms): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "\n";

    return 0;
}
