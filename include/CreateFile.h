//
// Created by 钱昱 on 2023/1/12.
//


#ifndef CPP_JSON_CREATEFILE_H
#define CPP_JSON_CREATEFILE_H

#include <string>
#include <iostream>

using namespace std;

// 字符串存入json
bool MvToFile(const char *src,
              const string &filePath,
              ios::ios_base::openmode mode);

#endif //CPP_JSON_CREATEFILE_H
