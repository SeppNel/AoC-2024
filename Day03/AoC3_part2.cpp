#include <fstream>
#include <iostream>
#include <regex>
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

string readInput(string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening the file!\n";
        exit(EXIT_FAILURE);
    }

    string output = "";
    string line;
    while (getline(file, line)) {
        output += line;
    }

    // Close the file
    file.close();
    return output;
}

int main(int argc, char *argv[]) {
    string input = readInput("input.txt");

    regex regexp("mul\\(\\d+,\\d+\\)");

    bool process = true;
    size_t last_match_end = 0;
    int total = 0;
    for (sregex_iterator it(input.begin(), input.end(), regexp), end_it; it != end_it; ++it) {
        // Find the position of the current mul() match
        size_t match_start = it->position();
        size_t match_end = match_start + it->str().length();

        // Check the text between the previous match and the current one
        string between_matches = input.substr(last_match_end, match_start - last_match_end);
        if (regex_search(between_matches, regex("do\\(\\)"))) {
            cout << "Found 'do()' between matches: " << between_matches << endl;
            process = true;
        } else if (regex_search(between_matches, regex("don't\\(\\)"))) {
            cout << "Found 'don't()' between matches: " << between_matches << endl;
            process = false;
        }

        if (process) {
            string str_minus_mul = string_split(it->str(), "(")[1];
            vector<string> str_nums = string_split(str_minus_mul, ",");
            int num1 = stoi(str_nums[0]);
            int num2 = stoi(str_nums[1].substr(0, str_nums[1].length() - 1));

            total += num1 * num2;
        }

        last_match_end = match_end;
    }

    cout << "Total: " << total << "\n";

    return 0;
}