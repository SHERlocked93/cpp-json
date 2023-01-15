//
// Created by 钱昱 on 2023/1/12.
//
#include <string>
#include <fstream>
#include <iostream>
#include "ReadFile.h"

using namespace std;

static const int bufferLen = 2048;

bool ReadFile(string &str, const string &filePath, ios::ios_base::openmode mode) {
    ifstream in(filePath, mode);
    
    if (!in) {
        cout << "read file error!" << endl;
        return false;
    }
    
    
    char temp[bufferLen];
    while (!in.eof()) {
        in.read(temp, bufferLen);
        str.append(temp);
    }
    
    in.close();
    
    return true;
}
