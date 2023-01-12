//
// Created by 钱昱 on 2023/1/12.
//
#include <string>
#include <fstream>
#include <iostream>
#include "CreateFile.h"

using namespace std;

bool MvToFile(const char *src, const string& filePath, ios::ios_base::openmode mode) {
    ofstream out(filePath, mode);
    
    if (!out) {
        return false;
    }
    out << src;
    
    out.close();
    
    return true;
}
