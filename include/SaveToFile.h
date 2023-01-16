//
// Created by 钱昱 on 2023/1/12.
//


#ifndef CPP_JSON_SAVETOFILE_H
#define CPP_JSON_SAVETOFILE_H

#include <string>
#include <iostream>

using namespace std;

// 字符串存入json
bool SaveToFile(const char *str,
                const string &filePath,
                ios::ios_base::openmode mode = ios::out | ios::trunc);

#endif //CPP_JSON_SAVETOFILE_H
