#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

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

unordered_map<uint64_t, uint64_t> readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    unordered_map<uint64_t, uint64_t> output;

    string line;
    getline(file, line);

    vector<string> numsString = string_split(line, " ");
    for (const string &s : numsString) {
        output[stoull(s)] = 1;
    }

    // Close the file
    file.close();

    return output;
}

uint count_digits(uint64_t num) {
    uint count = 0;
    while (num != 0) {
        count++;
        num /= 10;
    }
    return count;
}

int main() {
    unordered_map<uint64_t, uint64_t> input = readInput("input.txt");

    for (size_t j = 0; j < 75; j++) {

        unordered_map<uint64_t, uint64_t> copy = input;
        for (auto stone = copy.begin(); stone != copy.end(); stone++) {
            uint64_t num = stone->first;
            uint64_t count = stone->second;

            if (count == 0) {
                continue;
            }

            if (num == 0) {
                input[1] += count;
                input[0] -= count;
            } else if (count_digits(num) % 2 == 0) {
                string numString = to_string(num);
                string num1 = numString.substr(0, numString.size() / 2);
                string num2 = numString.substr(numString.size() / 2);

                input[stoull(num1)] += count;
                input[stoull(num2)] += count;
                input[num] -= count;
            } else {
                uint64_t n = num * 2024;
                input[n] += count;
                input[num] -= count;
            }
        }
    }

    uint64_t total = 0;
    for (auto e : input) {
        total += e.second;
    }

    cout << total << "\n";

    return 0;
}