//
// Created by qiany on 2023/1/11.
//

#include <cJSON/cJSON.h>
#include <iostream>
#include <fstream>
#include "CreateFile.h"
#include "ReadFile.h"

using namespace std;

void write() {
    cJSON *cjson_root = nullptr;
    cJSON *cjson_address = nullptr;
    cJSON *cjson_skill = nullptr;
    
    cjson_root = cJSON_CreateObject();
    
    cJSON_AddStringToObject(cjson_root, "name", "mcugtoer666");
    
    cJSON_AddNumberToObject(cjson_root, "age", 22);
    
    cJSON_AddNumberToObject(cjson_root, "weight", 55.5);
    
    cjson_address = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_address, "country", "China");
    cJSON_AddNumberToObject(cjson_address, "zip-code", 111111);
    cJSON_AddItemToObject(cjson_root, "address", cjson_address);
    
    cjson_skill = cJSON_CreateArray();
    cJSON_AddItemToArray(cjson_skill, cJSON_CreateString("C"));
    cJSON_AddItemToArray(cjson_skill, cJSON_CreateString("Java"));
    cJSON_AddItemToArray(cjson_skill, cJSON_CreateString("Python"));
    cJSON_AddItemToObject(cjson_root, "skill", cjson_skill);
    
    cJSON_AddFalseToObject(cjson_root, "student");
    
    char *str = nullptr;
    str = cJSON_Print(cjson_root);
    
    MvToFile(str, "../../assets/write_test1.json", ios::out | ios::trunc);
    
    free(str);
    cJSON_Delete(cjson_root);
}

void read() {
    char *str = nullptr;
    ReadFile(str, "../../assets/json-demo1.json", ios::in);
    
    cout << str << endl;
    
    str = nullptr;
}

int main() {
    //write();
    read();
    
    return 0;
}
