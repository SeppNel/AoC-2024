#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <utility>
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

template <typename T>
bool inVector(T needle, vector<T> haystack) {
    for (auto e : haystack) {
        if (needle == e) {
            return true;
        }
    }

    return false;
}

void readInput(string filename, vector<pair<uint, uint>> &pairs, vector<vector<uint>> &updates) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(file, line)) {
        if (line == "") {
            break;
        }

        pair<uint, uint> p;
        auto pos = line.find('|');
        pairs.emplace_back(stoi(line.substr(0, pos)), stoi(line.substr(pos + 1, line.length() - pos)));
    }

    while (getline(file, line)) {
        vector<uint> lineVec;

        for (string s : string_split(line, ",")) {
            lineVec.push_back(stoi(s));
        }

        updates.push_back(lineVec);
    }

    // Close the file
    file.close();
}

struct invalidUpdateData {
    size_t indexI;
    size_t indexJ;

    invalidUpdateData(size_t i, size_t j) {
        indexI = i;
        indexJ = j;
    }
};

bool checkValidUpdate(const vector<uint> &update, const vector<pair<uint, uint>> &rules, invalidUpdateData &iud) {
    bool valid = true;

    uint size = update.size();
    for (size_t i = 0; i < size; i++) {
        uint n = update[i];
        vector<uint> notValid;

        for (auto rule : rules) {
            if (n != rule.first) {
                continue;
            }
            notValid.push_back(rule.second);
        }

        for (int j = i - 1; j >= 0; j--) {
            if (inVector(update[j], notValid)) {
                iud.indexI = i;
                iud.indexJ = j;
                return false;
            }
        }
    }

    return true;
}

int main(int argc, char *argv[]) {
    vector<pair<uint, uint>> rules;
    vector<vector<uint>> updates;
    readInput("input.txt", rules, updates);

    vector<vector<uint>> invalidUpdates;
    vector<invalidUpdateData> iud;

    uint total = 0;
    for (auto &update : updates) {
        bool valid = true;

        uint size = update.size();
        for (size_t i = 0; i < size; i++) {
            uint n = update[i];
            vector<uint> notValid;

            for (auto rule : rules) {
                if (n != rule.first) {
                    continue;
                }
                notValid.push_back(rule.second);
            }

            for (int j = i - 1; j >= 0; j--) {
                if (inVector(update[j], notValid)) {
                    valid = false;
                    iud.emplace_back(i, j);
                    break;
                }
            }

            if (!valid) {
                invalidUpdates.emplace_back(update);
                break;
            }
        }
    }

    for (size_t i = 0; i < invalidUpdates.size(); i++) {
        invalidUpdateData &data = iud[i];
        vector<uint> &update = invalidUpdates[i];

        do {
            uint tmp = update[data.indexI];
            update[data.indexI] = update[data.indexJ];
            update[data.indexJ] = tmp;
        } while (!checkValidUpdate(update, rules, data));

        total += update[update.size() / 2];
    }

    cout << "Total: " << total << "\n";

    return 0;
}