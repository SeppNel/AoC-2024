#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
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
    uint64_t A = 0;
    uint64_t B = 0;
    uint64_t C = 0;
};

class CPU {
  public:
    Registers reg;
    uint ip = 0;
    vector<uint64_t> program;
    vector<uint64_t> output;

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

    uint64_t decodeOperand(int op) {
        uint64_t returnValue;
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

    void adv(uint64_t operand) {
        reg.A = reg.A / pow(2, operand);
    }

    void bxl(uint64_t operand) {
        reg.B = reg.B ^ operand;
    }

    void bst(uint64_t operand) {
        reg.B = operand % 8;
    }

    void jnz(uint64_t operand) {
        if (reg.A != 0) {
            ip = operand;
            ip -= 2;
        }
    }

    void bxc(uint64_t operand) {
        reg.B = reg.B ^ reg.C;
    }

    void out(uint64_t operand) {
        output.push_back(operand % 8);
    }

    void bdv(uint64_t operand) {
        reg.B = reg.A / pow(2, operand);
    }

    void cdv(uint64_t operand) {
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

bool solve(uint64_t n, const CPU &ogCPU) {
    CPU cpu = ogCPU;
    cpu.reg.A = n;
    cpu.run();

    if (cpu.output == ogCPU.program) {
        std::cout << n << '\n';
        return true;
    }

    n = n << 3;

    for (int i = 0; i < 8; i++) {
        cpu = ogCPU;
        cpu.reg.A = n;
        cpu.run();
        bool same = true;
        for (int i = 0; i < cpu.output.size(); i++) {
            same = same && cpu.output[i] == ogCPU.program[ogCPU.program.size() - cpu.output.size() + i];
        }
        if (same && solve(n, ogCPU))
            return true;
        n++;
    }
    return false;
}

int main() {
    const CPU ogCPU = readInput("input.txt");

    solve(0, ogCPU);

    return 0;
}
