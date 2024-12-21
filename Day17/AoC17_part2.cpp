#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using namespace std;

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

enum Opcodes {
    ADV,
    BXL,
    BST,
    JNZ,
    BXC,
    OUT,
    BDV,
    CDV
};

struct Registers {
    int A = 0;
    int B = 0;
    int C = 0;
};

class CPU {
  public:
    Registers reg;
    uint ip = 0;
    vector<int> program;
    vector<int> output;

    void run() {
        while (ip < program.size()) {
            decodeOpcode();
        }
    }

    void decodeOpcode() {
        int opcode = program[ip];
        int encodedOp = program[ip + 1];

        int operand = encodedOp;
        switch (opcode) {
        case ADV:
            operand = decodeOperand(encodedOp);
            adv(operand);
            break;
        case BXL:
            bxl(operand);
            break;
        case BST:
            operand = decodeOperand(encodedOp);
            bst(operand);
            break;
        case JNZ:
            jnz(operand);
            break;
        case BXC:
            bxc(-1);
            break;
        case OUT:
            operand = decodeOperand(encodedOp);
            out(operand);
            break;
        case BDV:
            operand = decodeOperand(encodedOp);
            bdv(operand);
            break;
        case CDV:
            operand = decodeOperand(encodedOp);
            cdv(operand);
            break;
        }
        ip += 2;
    }

    int decodeOperand(int op) {
        int returnValue;
        if (op <= 3) {
            returnValue = op;
        } else if (op == 4) {
            returnValue = reg.A;
        } else if (op == 5) {
            returnValue = reg.B;
        } else {
            returnValue = reg.C;
        }

        return returnValue;
    }

    void adv(int operand) {
        reg.A = reg.A / pow(2, operand);
    }

    void bxl(int operand) {
        reg.B = reg.B ^ operand;
    }

    void bst(int operand) {
        reg.B = operand % 8;
    }

    void jnz(int operand) {
        if (reg.A != 0) {
            ip = operand;
            ip -= 2;
        }
    }

    void bxc(int operand) {
        reg.B = reg.B ^ reg.C;
    }

    void out(int operand) {
        output.push_back(operand % 8);
    }

    void bdv(int operand) {
        reg.B = reg.A / pow(2, operand);
    }

    void cdv(int operand) {
        reg.C = reg.A / pow(2, operand);
    }
};

CPU readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    CPU output;

    string line;
    for (char c = 'A'; c <= 'C'; c++) {
        getline(file, line);
        string numStr = string_split(line, ": ")[1];
        if (c == 'A') {
            output.reg.A = stoi(numStr);
        } else if (c == 'B') {
            output.reg.B = stoi(numStr);
        } else if (c == 'C') {
            output.reg.C = stoi(numStr);
        }
    }

    getline(file, line);
    getline(file, line);

    string program = string_split(line, ": ")[1];

    vector<string> strOpcodes = string_split(program, ",");

    for (uint i = 0; i < strOpcodes.size(); i++) {
        output.program.push_back(stoi(strOpcodes[i]));
    }

    // Close the file
    file.close();

    return output;
}

std::mutex foundMtx;
bool found = false;

void threadFunc(const CPU &ogCPU, uint64_t start, uint64_t stop, uint64_t *out) {
    CPU cpu = ogCPU;

    for (uint64_t regA = start; regA <= stop; regA++) {
        cpu = ogCPU;
        cpu.reg.A = regA;

        cpu.run();

        // foundMtx.lock();
        if (found) {
            // foundMtx.unlock();
            break;
        }
        // foundMtx.unlock();

        if (cpu.output == ogCPU.program) {
            *out = regA;
            // foundMtx.lock();
            found = true;
            // foundMtx.unlock();
            break;
        }
    }
}

uint64_t bruteForcer(const CPU ogCPU) {
    vector<thread *> hilos(16);
    vector<uint64_t *> results(16);

    uint64_t size = UINT64_MAX / hilos.size();

    for (uint64_t i = 0; i <= hilos.size(); i++) {
        uint64_t start = size * i;
        uint64_t stop = size * (i + 1) - 1;

        results[i] = new uint64_t(0);

        hilos[i] = new thread(threadFunc, ogCPU, start, stop, results[i]);
    }

    for (uint i = 0; i < hilos.size(); i++) {
        hilos[i]->join();
    }

    for (const auto &e : results) {
        if (*e != 0) {
            return *e;
        }
    }

    return 0;
}

int main() {
    CPU cpu = readInput("input.txt");

    cout << bruteForcer(cpu) << "\n";
    return 0;

    const CPU ogCPU = cpu;
    int regA = 0;

    while (cpu.output != cpu.program) {
        cpu = ogCPU;
        cpu.reg.A = regA;

        cpu.run();
        regA++;
    }

    for (int e : cpu.output) {
        cout << e << ",";
    }
    cout << "\n";
    cout << regA - 1 << "\n";

    return 0;
}
