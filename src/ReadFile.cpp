//
// Created by 钱昱 on 2023/1/12.
//
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "ReadFile.h"

using namespace std;

bool ReadFile(string &str,
              const string &filePath,
              ios::ios_base::openmode mode) {
    ifstream in(filePath, mode);
    
    if (!in) {
        cout << "read file error!" << endl;
        return false;
    }
    
    stringstream buffer;
    buffer << in.rdbuf();
    str = buffer.str();
    
    in.close();
    
    return true;
}
