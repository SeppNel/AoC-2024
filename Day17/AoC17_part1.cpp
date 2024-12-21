#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
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
        cout << operand % 8 << ",";
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

int main() {
    CPU cpu = readInput("input.txt");

    cpu.run();
    cout << "\n";

    return 0;
}
