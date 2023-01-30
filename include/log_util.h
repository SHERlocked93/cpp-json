//
// Created by 钱昱 on 2023/1/29.
//


#ifndef CPP_JSON_LOG_UTIL_H
#define CPP_JSON_LOG_UTIL_H

#include <string>

void ELOGLIB_Init();

void ELOGLIB_Deinit();

void logi(const string &msg);

void loge(const string &msg);

string getTime();

#endif //CPP_JSON_LOG_UTIL_H
