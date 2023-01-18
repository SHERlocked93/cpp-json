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

#define JSON_KEY        "dt"
#define JSON_JOIN       "normal"
#define JSON_DAMAGE     "damage"
#define JSON_MAINTAIN   "maintenance"

static const string JsonPath = "../../assets/json-demo1.json";


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

void Display(cJSON *json_root, const char *json_value) {
    cJSON *json_key = cJSON_GetObjectItem(json_root, JSON_KEY);
    if (json_key == nullptr || json_key->type == cJSON_NULL) {
        cJSON_AddStringToObject(json_root, JSON_KEY, json_value);
    } else {
        cJSON_ReplaceItemInObject(json_root, JSON_KEY, cJSON_CreateString(json_value));
    }
    string res = cJSON_Print(json_root);
    
    SaveToFile(res, JsonPath);
}

HSM_EVENT DT_StateNormalHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == DTE_ERROR) {
        HSM_Tran(This,
                 &DTS_DAMAGE,
                 (cJSON *) param,
                 [](HSM *This, void *param) { Display((cJSON *) param, JSON_DAMAGE); });
        return 0;
    } else if (event == DTE_MAINTAIN) {
        HSM_Tran(This,
                 &DTS_MAINTENANCE,
                 (cJSON *) param,
                 [](HSM *This, void *param) { Display((cJSON *) param, JSON_MAINTAIN); });
        return 0;
    }
    return event;
}

HSM_EVENT DT_StateDamageHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == DTE_JOIN) {
        HSM_Tran(This,
                 &DTS_NORMAL,
                 (cJSON *) param,
                 [](HSM *This, void *param) { Display((cJSON *) param, JSON_JOIN); });
        return 0;
    } else if (event == DTE_MAINTAIN) {
        HSM_Tran(This,
                 &DTS_MAINTENANCE,
                 (cJSON *) param,
                 [](HSM *This, void *param) { Display((cJSON *) param, JSON_MAINTAIN); });
        return 0;
    }
    return event;
}

HSM_EVENT DT_StateMaintainanceHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == DTE_JOIN) {
        HSM_Tran(This,
                 &DTS_NORMAL,
                 (cJSON *) param,
                 [](HSM *This, void *param) { Display((cJSON *) param, JSON_JOIN); });
        return 0;
    } else if (event == DTE_ERROR) {
        HSM_Tran(This,
                 &DTS_DAMAGE,
                 (cJSON *) param,
                 [](HSM *This, void *param) { Display((cJSON *) param, JSON_DAMAGE); });
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
    HSM_STATE_Create(&DTS_NORMAL, JSON_JOIN, DT_StateNormalHndlr, nullptr);
    HSM_STATE_Create(&DTS_DAMAGE, JSON_DAMAGE, DT_StateDamageHndlr, nullptr);
    HSM_STATE_Create(&DTS_MAINTENANCE, JSON_MAINTAIN, DT_StateMaintainanceHndlr, nullptr);
    
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
    string res;
    ReadFile(res, JsonPath);
    cJSON *cjson_root = cJSON_Parse(res.c_str());
    
    DT_Init(&dt, const_cast<char *>("dt"));
    
    while (true) {
        char input;
        cout << "输入 d:发生故障 m:维护 j:恢复正常 e:EXIT" << endl;
        cin >> input;
        switch (input) {
            case 'd':
                DT_Run(&dt, DTE_ERROR, cjson_root);
                break;
            case 'm':
                DT_Run(&dt, DTE_MAINTAIN, cjson_root);
                break;
            case 'j':
                DT_Run(&dt, DTE_JOIN, cjson_root);
                break;
            case 'e':
                return 0;
            default:;
        }
        cout << "当前json文件：" << cJSON_Print(cjson_root) << endl;
    }
    
    return 0;
}

