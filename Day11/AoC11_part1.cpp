#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
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

vector<uint64_t> readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!";
        exit(EXIT_FAILURE);
    }

    vector<uint64_t> output;

    string line;
    getline(file, line);

    vector<string> numsString = string_split(line, " ");
    output.reserve(numsString.size());
    for (const string &s : numsString) {
        output.push_back(stoull(s));
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
    vector<uint64_t> input = readInput("input.txt");

    for (size_t j = 0; j < 25; j++) {

        for (size_t i = 0; i < input.size(); i++) {
            uint64_t &num = input[i];
            if (num == 0) {
                num = 1;
            } else if (count_digits(num) % 2 == 0) {
                string numString = to_string(num);
                string num1 = numString.substr(0, numString.size() / 2);
                string num2 = numString.substr(numString.size() / 2);

                num = stoull(num1);
                input.insert(input.begin() + i + 1, stoull(num2));
                i++;
            } else {
                num *= 2024;
            }
        }

        for (auto e : input) {
            cout << e << " ";
        }
        cout << "\n";
    }

    cout << input.size() << "\n";

    return 0;
}