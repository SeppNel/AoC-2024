#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<vector<int>> reports;

vector<int> string_split(string str, string del) {
    vector<int> output;
    // Find first occurrence of the delimiter
    auto pos = str.find(del);

    // While there are still delimiters in the
    // string
    while (pos != string::npos) {

        // Extracting the substring up to the
        // delimiter
        output.push_back(stoi(str.substr(0, pos)));

        // Erase the extracted part from the
        // original string
        str.erase(0, pos + del.length());

        // Find the next occurrence of the
        // delimiter
        pos = str.find(del);
    }

    output.push_back(stoi(str));
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
        vector<int> nums = string_split(line, " ");
        reports.emplace_back(nums);
    }

    // Close the file
    file.close();
}

int main(int argc, char *argv[]) {
    readInput("input.txt");

    uint safeReports = 0;
    for (auto report : reports){
        bool life = true;
        bool safe = true;
        char state = 'x';
        for (size_t i = 0; i < report.size() - 1; i++){
            if (i == 0){
                if(report[i] > report[i+1]){
                    state = 'd';
                }
                else{
                    state = 'i';
                }
            }

            if(state == 'd' && report[i] < report[i+1]){
                if(life){
                    report.erase(report.begin() + i);
                    i--;
                    life = false;
                    continue;
                }
                safe = false;
                break;
            }
            
            if(state == 'i' && report[i] > report[i+1]){
                if(life){
                    report.erase(report.begin() + i);
                    i--;
                    life = false;
                    continue;
                }
                safe = false;
                break;
            }

            uint diff = abs(report[i] - report[i+1]);
            if (diff > 3 || diff < 1){
                if(life){
                    report.erase(report.begin() + i);
                    i--;
                    life = false;
                    continue;
                }
                safe = false;
                break;
            }
        }
        
        if(safe){
            safeReports++;
        }
    }

    cout << safeReports << "\n";
}