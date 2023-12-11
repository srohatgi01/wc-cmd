#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <iterator>

using namespace std;

void byteCount(istream &file) {
    file.seekg(0, file.end);
    cout << "\t" << file.tellg();
    file.seekg(0, std::ios::beg);
}

void lineCount(istream &file) {
    int c = 0;
    string s;
    while(getline(file, s)) {
        c++;
    }

    cout << "\t" << c;
    file.clear();
    file.seekg(0, std::ios::beg);
}

void wordCount(istream &file) {
    size_t wordCount = std::distance(istream_iterator<string>(file),
                                     istream_iterator<string>());
    
    cout << "\t" << wordCount;

    file.clear();
    file.seekg(0, std::ios::beg);
}

int main(int argc, char *argv[]) {

    string filePath = "";
    string args = "";
    // istream ss;
    ifstream inputStream;

    if(argc > 3) {
        cout << "Error: Too many arguments. Expected 2 Received " << argc-1 << endl;
        return -1;
    } else if(argc == 3) {
        args = argv[1];
        if(args[0] == '-') {
            args = args.substr(1, args.size());
        } 
        else {
            cout << "Invalid Arguments" << endl; 
            return -1;
        }

        filePath = argv[2];

        inputStream.open(filePath);
    
        if(!inputStream.good()) {
            cout << "Something wrong with the file." << endl;
            return -1;
        }
    } else if(argc == 2) {
        if(argv[1][0] == '-') {
            args = argv[1];
            args = args.substr(1, args.size());
        }
        else {
             filePath = argv[1];
    
            inputStream.open(filePath);
        
            if(!inputStream.good()) {
                cout << "Something wrong with the file." << endl;
                return -1;
            }
        }
    }

    if(filePath == "") {
        const std::string tempFilename = "tempfile.txt";
        std::ofstream tempFile(tempFilename);
        std::string line;

        while (std::getline(std::cin, line)) {
            tempFile << line << '\n';
        }
        tempFile.close();

        inputStream.open(tempFilename);
        std::remove(tempFilename.c_str());
    }

    // create a command map
    unordered_map<char, function<void(istream &)>> commandMap;
    commandMap['c'] = byteCount;
    commandMap['l'] = lineCount;
    commandMap['w'] = wordCount;


    // execute functions for all arguments
    if(args == "") {
        for(auto& f: commandMap) {
            f.second((inputStream));
        }
    } else {
        for(int i = 0; i < args.size(); i++) {
            if(commandMap.find(args[i]) != commandMap.end()) {
                commandMap[args[i]](inputStream);
            }
            else {
                cout << endl << "Error: Invalid Argument Passed!" << endl;
                return -1;
            }
        }
    }

    cout << "\t" << filePath << endl;

    return 0;
}