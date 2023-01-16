//
// Created by 钱昱 on 2023/1/12.
//


#ifndef CPP_JSON_READFILE_H
#define CPP_JSON_READFILE_H

#include <string>
#include <iostream>

using namespace std;

bool ReadFile(string &str,
              const string &filePath,
              ios::ios_base::openmode mode = ios::in);

#endif //CPP_JSON_READFILE_H
