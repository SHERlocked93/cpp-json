//
// Created by qiany on 2023/1/11.
//

#include <iostream>
#include <fstream>
#include "SaveToFile.h"
#include "ReadFile.h"
#include <hsm/hsm.h>
#include <cJSON/cJSON.h>

using namespace std;

#define DTE_JOIN            (HSME_START)        // 变为正常
#define DTE_ERROR           (HSME_START + 1)    // 发生异常
#define DTE_MAINTAIN        (HSME_START + 2)    // 维护

typedef struct DT_T {
    HSM parent;
} DT;

DT dt;

//     error       ┌──────────┐ maintain
//   ┌─────────────┤          ├────────────┐
//   │             │  Normal  │            │
//   │  ┌─────────►│          │◄────────┐  │
//   │  │ join     └──────────┘  join   │  │
//┌──▼──┴────┐                    ┌─────┴──▼─────┐
//│          │◄───────────────────┤              │
//│  Damage  │       error        │ Maintainance │
//│          ├────────────────────►              │
//└──────────┘      maintain      └──────────────┘
HSM_STATE DTS_NORMAL;        // 正常
HSM_STATE DTS_DAMAGE;        // 异常
HSM_STATE DTS_MAINTENANCE;   // 维护

void Display(HSM *This, void *param) {
    cout << This->name << "\t-> " << (char *) param << endl;
}

HSM_EVENT DT_StateNormalHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == DTE_ERROR) {
        HSM_Tran(This, &DTS_DAMAGE, const_cast<char *>("damage"), Display);
        return 0;
    } else if (event == DTE_MAINTAIN) {
        HSM_Tran(This, &DTS_MAINTENANCE, const_cast<char *>("maintenance"), Display);
        return 0;
    }
    return event;
}

HSM_EVENT DT_StateDamageHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == DTE_JOIN) {
        HSM_Tran(This, &DTS_NORMAL, const_cast<char *>("normal"), Display);
        return 0;
    } else if (event == DTE_MAINTAIN) {
        HSM_Tran(This, &DTS_MAINTENANCE, const_cast<char *>("maintenance"), Display);
        return 0;
    }
    return event;
}

HSM_EVENT DT_StateMaintainanceHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == DTE_JOIN) {
        HSM_Tran(This, &DTS_NORMAL, const_cast<char *>("normal"), Display);
        return 0;
    } else if (event == DTE_ERROR) {
        HSM_Tran(This, &DTS_DAMAGE, const_cast<char *>("maintenance"), Display);
        return 0;
    }
    return event;
}

const char *HSM_Evt2Str(uint32_t event) {
    switch (event) {
        case DTE_ERROR:
            return "DTE_ERROR";
        case DTE_JOIN:
            return "DTE_JOIN";
        case DTE_MAINTAIN:
            return "DTE_MAINTAIN";
        default:
            return "";
    }
}

void DT_Init(DT *This, char *name) {
    HSM_STATE_Create(&DTS_NORMAL, "normal", DT_StateNormalHndlr, nullptr);
    HSM_STATE_Create(&DTS_DAMAGE, "damage", DT_StateDamageHndlr, nullptr);
    HSM_STATE_Create(&DTS_MAINTENANCE, "maintain", DT_StateMaintainanceHndlr, nullptr);
    HSM_Create((HSM *) This, name, &DTS_NORMAL);
    HSM_SET_PREFIX((HSM *) This, "[DBG] ");
    HSM_SET_DEBUG((HSM *) This, HSM_SHOW_ALL);
}

//电台，正常，json数据里面写 dt：normal
//     异常，json数据里面写 dt：damage
//     维护，json数据里面写 dt：maintenance
void DT_Run(DT *This, HSM_EVENT event, void *param = nullptr) {
    HSM_Run((HSM *) This, event, param);
}

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
    
    SaveToFile(str, "../../assets/write_test1.json");
    
    free(str);
    cJSON_Delete(cjson_root);
}

void read() {
    string str;
    ReadFile(str, "../../assets/json-demo1.json");
    
    cout << str << endl;
    
    str = nullptr;
}

int main() {
    //write();
    //read();
    DT_Init(&dt, const_cast<char *>("dt"));
    
    DT_Run(&dt, DTE_ERROR);
    DT_Run(&dt, DTE_MAINTAIN);
    DT_Run(&dt, DTE_JOIN);
    
    return 0;
}

