//
// Created by qiany on 2023/1/11.
//

#include <iostream>
#include <fstream>
#include "SaveToFile.h"
#include "ReadFile.h"
#include "hsm/hsm.h"
#include "cJSON/cJSON.h"

using namespace std;

#define DTE_JOIN            (HSME_START)        // 变为正常 事件
#define DTE_ERROR           (HSME_START + 1)    // 发生异常
#define DTE_MAINTAIN        (HSME_START + 2)    // 维护

#define JSON_JOIN       "normal"
#define JSON_DAMAGE      "damage"
#define JSON_MAINTAIN   "maintenance"

typedef struct DT_T {
    HSM parent;
    cJSON *cjson;
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

void Display(DT *This, const string &json_value) {
    cout << cJSON_Print(This->cjson) << "---" << json_value << endl;
}

HSM_EVENT DT_StateNormalHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == DTE_ERROR) {
        HSM_Tran(This,
                 &DTS_DAMAGE,
                 const_cast<char *>(JSON_DAMAGE),
                 [](HSM *This, void *param) { Display((DT *) param, JSON_DAMAGE); });
        return 0;
    } else if (event == DTE_MAINTAIN) {
        HSM_Tran(This,
                 &DTS_MAINTENANCE,
                 const_cast<char *>(JSON_MAINTAIN),
                 [](HSM *This, void *param) { Display((DT *) param, JSON_MAINTAIN); });
        return 0;
    }
    return event;
}

HSM_EVENT DT_StateDamageHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == DTE_JOIN) {
        HSM_Tran(This,
                 &DTS_NORMAL,
                 const_cast<char *>(JSON_JOIN),
                 [](HSM *This, void *param) { Display((DT *) param, JSON_JOIN); });
        return 0;
    } else if (event == DTE_MAINTAIN) {
        HSM_Tran(This,
                 &DTS_MAINTENANCE,
                 const_cast<char *>(JSON_MAINTAIN),
                 [](HSM *This, void *param) { Display((DT *) param, JSON_MAINTAIN); });
        return 0;
    }
    return event;
}

HSM_EVENT DT_StateMaintainanceHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == DTE_JOIN) {
        HSM_Tran(This,
                 &DTS_NORMAL,
                 const_cast<char *>(JSON_JOIN),
                 [](HSM *This, void *param) { Display((DT *) param, JSON_JOIN); });
        return 0;
    } else if (event == DTE_ERROR) {
        HSM_Tran(This,
                 &DTS_DAMAGE,
                 const_cast<char *>(JSON_DAMAGE),
                 [](HSM *This, void *param) { Display((DT *) param, JSON_DAMAGE); });
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

void DT_Init(DT *This, char *name, cJSON *cjson) {
    HSM_STATE_Create(&DTS_NORMAL, JSON_JOIN, DT_StateNormalHndlr, nullptr);
    HSM_STATE_Create(&DTS_DAMAGE, JSON_DAMAGE, DT_StateDamageHndlr, nullptr);
    HSM_STATE_Create(&DTS_MAINTENANCE, JSON_MAINTAIN, DT_StateMaintainanceHndlr, nullptr);
    This->cjson = cjson;
    
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

int main() {
    const string JsonPath = "../../assets/json-demo1.json";
    string res;
    ReadFile(res, JsonPath);
    cJSON *cjson_root = cJSON_Parse(res.c_str());
    cJSON_ReplaceItemInObject(cjson_root, "sex", cJSON_CreateString("man"));
    res = cJSON_Print(cjson_root);
    
    SaveToFile(res, JsonPath);
    
    
    DT_Init(&dt, const_cast<char *>("dt"), cjson_root);
    
    DT_Run(&dt, DTE_ERROR);
    DT_Run(&dt, DTE_MAINTAIN);
    DT_Run(&dt, DTE_JOIN);
    
    return 0;
}

