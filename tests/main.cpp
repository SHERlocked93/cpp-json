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
#define DTE_JOIN_N1         (HSME_START+1)        // 变为正常_试运行 事件
#define DTE_JOIN_N2         (HSME_START+2)        // 变为正常_正式运行 事件
#define DTE_ERROR           (HSME_START + 3)    // 发生异常
#define DTE_MAINTAIN        (HSME_START + 4)    // 维护

#define JSON_KEY        "dt"
#define JSON_JOIN       "normal"
#define JSON_JOIN_N1    "normal.n1"
#define JSON_JOIN_N2    "normal.n2"
#define JSON_DAMAGE     "damage"
#define JSON_MAINTAIN   "maintenance"

static const string JsonPath = "../../assets/json-demo1.json";

typedef struct DT_T {
    HSM parent{};
} DT;

DT dt;

//     error       ┌──────────┐ maintain
//   ┌─────────────┤  Normal  ├────────────┐
//   │             │          │            │
//   │  ┌─────────►│ n1 - n2  │◄────────┐  │
//   │  │ join     └──────────┘  join   │  │
//┌──▼──┴────┐                    ┌─────┴──▼─────┐
//│          │◄───────────────────┤              │
//│  Damage  │       error        │ Maintainance │
//│          ├────────────────────►              │
//└──────────┘      maintain      └──────────────┘
HSM_STATE DTS_NORMAL;        // 正常
HSM_STATE DTS_NORMAL_N1;     // 正常 N1 试运行
HSM_STATE DTS_NORMAL_N2;     // 正常 N2 正式运行
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

void State2Error(HSM *This, void *param) {
    HSM_Tran(This,
             &DTS_DAMAGE,
             (cJSON *) param,
             [](HSM *This, void *param) { Display((cJSON *) param, JSON_DAMAGE); });
}

void State2Normal(HSM *This, void *param) {
    HSM_Tran(This,
             &DTS_NORMAL,
             (cJSON *) param,
             [](HSM *This, void *param) { Display((cJSON *) param, JSON_JOIN); });
}

void State2NormalN1(HSM *This, void *param) {
    HSM_Tran(This,
             &DTS_NORMAL_N1,
             (cJSON *) param,
             [](HSM *This, void *param) { Display((cJSON *) param, JSON_JOIN_N1); });
}

void State2NormalN2(HSM *This, void *param) {
    HSM_Tran(This,
             &DTS_NORMAL_N2,
             (cJSON *) param,
             [](HSM *This, void *param) { Display((cJSON *) param, JSON_JOIN_N2); });
}

void State2Maintain(HSM *This, void *param) {
    HSM_Tran(This,
             &DTS_MAINTENANCE,
             (cJSON *) param,
             [](HSM *This, void *param) { Display((cJSON *) param, JSON_MAINTAIN); });
}

HSM_EVENT DT_StateNormalHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == DTE_ERROR) {
        State2Error(This, param);
        return EXIT_SUCCESS;
    } else if (event == DTE_MAINTAIN) {
        State2Maintain(This, param);
        return EXIT_SUCCESS;
    } else if (event == DTE_JOIN_N1) {
        State2NormalN1(This, param);
        return EXIT_SUCCESS;
    } else if (event == DTE_JOIN_N2) {
        State2NormalN2(This, param);
        return EXIT_SUCCESS;
    }
    return event;
}

HSM_EVENT DT_StateNormalN1Hndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == HSME_EXIT) {
        State2Normal(This, param);
        return EXIT_SUCCESS;
    } else if (event == DTE_JOIN_N2) {
        State2NormalN2(This, param);
        return EXIT_SUCCESS;
    }
    return event;
}

HSM_EVENT DT_StateNormalN2Hndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == DTE_ERROR) {
        State2Error(This, param);
        return EXIT_SUCCESS;
    } else if (event == DTE_MAINTAIN) {
        State2Maintain(This, param);
        return EXIT_SUCCESS;
    }
    return event;
}

HSM_EVENT DT_StateDamageHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == DTE_JOIN) {
        State2Normal(This, param);
        return EXIT_SUCCESS;
    } else if (event == DTE_MAINTAIN) {
        State2Maintain(This, param);
        return EXIT_SUCCESS;
    }
    return event;
}

HSM_EVENT DT_StateMaintainanceHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == DTE_JOIN) {
        State2Normal(This, param);
        return EXIT_SUCCESS;
    } else if (event == DTE_ERROR) {
        State2Error(This, param);
        return EXIT_SUCCESS;
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
    HSM_STATE_Create(&DTS_NORMAL_N1, JSON_JOIN_N1, DT_StateNormalN1Hndlr, &DTS_NORMAL);
    HSM_STATE_Create(&DTS_NORMAL_N2, JSON_JOIN_N2, DT_StateNormalN2Hndlr, &DTS_NORMAL);
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
        cout << "输入 d:发生故障 \nm:维护  \nn:恢复正常 \n\t1:试运行 \n\t2:正式运行 \ne:退出当前状态 \nE:EXIT" << endl;
        cin >> input;
        switch (input) {
            case 'd':
                DT_Run(&dt, DTE_ERROR, cjson_root);
                break;
            case 'm':
                DT_Run(&dt, DTE_MAINTAIN, cjson_root);
                break;
            case 'n':
                DT_Run(&dt, DTE_JOIN, cjson_root);
                break;
            case '1':
                DT_Run(&dt, DTE_JOIN_N1, cjson_root);
                break;
            case '2':
                DT_Run(&dt, DTE_JOIN_N2, cjson_root);
                break;
            case 'e':
                DT_Run(&dt, HSME_EXIT, cjson_root);
                break;
            case 'E':
                return EXIT_SUCCESS;
            default:;
        }
        cout << "当前json文件：" << cJSON_Print(cjson_root) << endl;
    }
    
    return EXIT_SUCCESS;
}

