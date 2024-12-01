#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

vector<int> list1;
vector<int> list2;

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

unordered_map<int, uint> allFrequencies(vector<int> &v) {
    unordered_map<int, uint> output;

    for (auto e : v) {
        output[e]++;
    }

    return output;
}

int main(int argc, char *argv[]) {
    readInput("input.txt");
    unordered_map<int, uint> freqs = allFrequencies(list2);

    uint total = 0;
    for (size_t i = 0; i < list1.size(); i++) {
        total += list1[i] * freqs[list1[i]];
    }

    cout << "Total: " << total << "\n";
}