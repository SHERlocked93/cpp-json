//
// Created by 钱昱 on 2023/1/12.
//
#include <string>
#include <fstream>
#include <iostream>
#include "SaveToFile.h"

using namespace std;

bool SaveToFile(const char *str,
                const string &filePath,
                ios::ios_base::openmode mode ) {
    ofstream out(filePath, mode);
    
    if (!out) {
        cerr << "out file error!" << endl;
        return false;
    }
    out << str;
    
    out.close();
    
    return true;
}
