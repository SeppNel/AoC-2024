#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
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
bool inVector(const T &needle, const vector<T> &haystack) {
    for (const T &e : haystack) {
        if (needle == e) {
            return true;
        }
    }

    return false;
}

struct File {
    uint id;
    uint lenght;
};

struct Block {
    File *file = nullptr;
    bool free;

    Block(File *p, bool f) {
        file = p;
        free = f;
    }
};

list<Block> readInput_bck(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    list<Block> output;

    string line;
    getline(file, line);

    uint fileCount = 0;
    bool fileBlock = true;
    for (const char &c : line) {
        if (fileBlock) {
            File *tmpFile = new File;
            tmpFile->id = fileCount;
            tmpFile->lenght = c - '0';

            output.emplace_back(tmpFile, false);

            fileCount++;
            fileBlock = false;
        } else {
            uint len = c - '0';
            for (uint i = 0; i < len; i++) {
                output.emplace_back(nullptr, true);
            }
            fileBlock = true;
        }
    }

    // Close the file
    file.close();

    return output;
}

list<int> readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    list<int> output;

    string line;
    getline(file, line);

    uint fileCount = 0;
    bool fileBlock = true;
    for (const char &c : line) {
        uint len = c - '0';
        if (fileBlock) {
            for (uint i = 0; i < len; i++) {
                output.push_back(fileCount);
            }

            fileCount++;
            fileBlock = false;
        } else {
            for (uint i = 0; i < len; i++) {
                output.push_back(-1);
            }
            fileBlock = true;
        }
    }

    // Close the file
    file.close();

    return output;
}

void printList(const list<int> &l) {
    for (const int &e : l) {
        if (e == -1) {
            cout << ".";
        } else {
            cout << e;
        }
    }
    cout << "\n";
}

_List_iterator<int> getLastFile(list<int> &disk) {
    _List_iterator<int> out;
    for (auto i = disk.rbegin(); i != disk.rend(); i++) {
        if (*i != -1) {
            out = --i.base();
            break;
        }
    }

    return out;
}

_List_iterator<int> getFirstFree(list<int> &disk) {
    _List_iterator<int> out;
    for (auto i = disk.begin(); i != disk.end(); i++) {
        if (*i == -1) {
            out = i;
            break;
        }
    }

    return out;
}

bool checkDiskDefraged(const list<int> &disk) {
    bool firstFreeFound = false;
    for (const auto &e : disk) {
        if (!firstFreeFound) {
            if (e == -1) {
                firstFreeFound = true;
            }
        } else {
            if (e != -1) {
                return false;
            }
        }
    }

    return true;
}

void defragDisk(list<int> &disk) {
    while (!checkDiskDefraged(disk)) {
        auto lastFile = getLastFile(disk);
        auto firstFree = getFirstFree(disk);

        // cout << "Last File:\n";
        // cout << lastFile->file->id << " - " << lastFile->file->lenght << " - \n";
        // cin.get();

        int last = *lastFile;
        *firstFree = last;
        *lastFile = -1;

        // printList(disk);
        // cin.get();
    }
}

uint64_t calcChecksum(const list<int> &disk) {
    uint64_t out = 0;

    uint i = 0;
    for (const int &e : disk) {
        if (e == -1) {
            continue;
        }

        out += i * e;
        i++;
    }

    return out;
}

int main() {
    list<int> input = readInput("input.txt");

    // printList(input);

    defragDisk(input);

    // printList(input);

    uint64_t checksum = calcChecksum(input);

    cout << "Checksum: " << checksum << "\n";

    return 0;
}