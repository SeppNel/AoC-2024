#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> list1;
vector<int> list2;

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

pair<int, int> string_split_pair(string str, string del) {
    pair<int, int> output;

    auto pos = str.find(del);
    output.first = stoi(str.substr(0, pos));
    str.erase(0, pos + del.length());
    pos = str.find(del);
    output.second = stoi(str.substr(0, pos));

    return output;
}

void readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(file, line)) {
        pair<int, int> nums = string_split_pair(line, "   ");
        list1.push_back(nums.first);
        list2.push_back(nums.second);
    }

    // Close the file
    file.close();
}

uint frequencyInVector(vector<int> &v, int n) {
    uint total = 0;
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i] == n) {
            total++;
        } else if (v[i] > n) {
            break;
        }
    }

    return total;
}

int main(int argc, char *argv[]) {
    readInput("input.txt");

    // Optimization for early exits
    sort(list2.begin(), list2.end());

    uint total = 0;
    for (size_t i = 0; i < list1.size(); i++) {
        uint freq = frequencyInVector(list2, list1[i]);

        total += list1[i] * freq;
    }

    cout << "Total: " << total << "\n";
}