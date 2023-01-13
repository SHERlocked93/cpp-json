//
// Created by 钱昱 on 2023/1/12.
//
#include <string>
#include <fstream>
#include <iostream>
#include "CreateFile.h"

using namespace std;

bool MvToFile(const char *str,
              const string &filePath,
              ios::ios_base::openmode mode) {
    ofstream out(filePath, mode);
    
    if (!out) {
        cout << "out file error!" << endl;
        return false;
    }
    out << str;
    
    out.close();
    
    return true;
}
