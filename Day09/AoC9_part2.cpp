#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <unordered_set>

using namespace std;

struct Block {
    int fileId = -1;
    bool free;
    uint lenght;

    Block(bool f, int id, uint len) {
        fileId = id;
        free = f;
        lenght = len;
    }
};

list<Block> readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    list<Block> output;

    string line;
    getline(file, line);

    uint fileCount = 0;
    bool fileBlock = true;
    for (const char &c : line) {
        uint len = c - '0';
        if (fileBlock) {
            output.emplace_back(false, fileCount, len);

            fileCount++;
            fileBlock = false;
        } else {
            output.emplace_back(true, -1, len);

            fileBlock = true;
        }
    }

    // Close the file
    file.close();

    return output;
}

void printList(const list<Block> &l) {
    for (const Block &e : l) {
        if (e.free) {
            for (uint i = 0; i < e.lenght; i++) {
                cout << ".";
            }

        } else {
            for (uint i = 0; i < e.lenght; i++) {
                cout << e.fileId;
            }
        }
    }
    cout << "\n";
}

_List_iterator<Block> getLastFile(list<Block> &disk, const unordered_set<int> &triedIds) {
    _List_iterator<Block> out = disk.end();
    for (auto i = disk.rbegin(); i != disk.rend(); i++) {
        if (!i->free && triedIds.find(i->fileId) == triedIds.end()) {
            out = --i.base();
            break;
        }
    }

    return out;
}

_List_iterator<Block> getFirstFree(list<Block> &disk, uint minLenght, _List_iterator<Block> lastPos) {
    _List_iterator<Block> out = disk.end();
    for (auto i = disk.begin(); i != lastPos; i++) {
        if (i->free && i->lenght >= minLenght) {
            out = i;
            break;
        }
    }

    return out;
}

void defragDisk(list<Block> &disk) {
    unordered_set<int> triedIds;
    triedIds.insert(disk.front().fileId);

    while (true) {
        auto lastFile = getLastFile(disk, triedIds);

        if (lastFile == disk.end()) {
            // cout << "LastFile error\n";
            break;
        }

        triedIds.insert(lastFile->fileId);

        auto firstFree = getFirstFree(disk, lastFile->lenght, lastFile);
        if (firstFree == disk.end()) {
            // cout << "FirstFree error\n";
            continue;
        }

        if (firstFree->lenght > lastFile->lenght) {
            auto insertPos = firstFree;
            insertPos++;
            uint newFreeLenght = firstFree->lenght - lastFile->lenght;
            disk.emplace(insertPos, true, -1, newFreeLenght);
        }

        firstFree->free = false;
        firstFree->fileId = lastFile->fileId;
        firstFree->lenght = lastFile->lenght;

        lastFile->free = true;
        lastFile->fileId = -1;
    }
}

uint64_t calcChecksum(const list<Block> &disk) {
    uint64_t out = 0;

    uint i = 0;
    for (const Block &e : disk) {
        if (e.free) {
            i += e.lenght;
            continue;
        }

        for (uint j = 0; j < e.lenght; j++) {
            out += i * e.fileId;
            i++;
        }
    }

    return out;
}

int main() {
    list<Block> input = readInput("input.txt");

    // printList(input);

    defragDisk(input);

    // printList(input);

    uint64_t checksum = calcChecksum(input);

    cout << "Checksum: " << checksum << "\n";

    return 0;
}