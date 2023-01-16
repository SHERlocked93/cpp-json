//
// Created by qiany on 2023/1/11.
//

#include <iostream>
#include <fstream>
#include "SaveToFile.h"
#include "ReadFile.h"
#include <cJSON/cJSON.h>
#include "hsm/hsm.h"

#define HSME_NORMAL             (HSME_START)        #正常
#define HSME_DAMAGE             (HSME_STAoRT + 1)    #异常
#define HSME_MAINTENANCE        (HSME_START + 2)    #维护

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
    read();
    
    return 0;
}

// Camera HSM Events
#define HSME_PWR        (HSME_START)
#define HSME_RELEASE    (HSME_START + 1)
#define HSME_MODE       (HSME_START + 2)
#define HSME_LOWBATT    (HSME_START + 3)


typedef struct CAMERA_T {
    /*
     Class that implements the Camera HSM and inherits from the HSM class
     Creating a HSM requires the following steps:
         1) Initialize the base HSM class
         2) Define the HSM states hierarchy
         3) Set the starting state
         4) Define the state handlers
             a) State handler must return "None" if the event IS handled
             b) State handler must return "event" if the event IS NOT handled
             c) State handler may handle the ENTRY event for state setup
             d) State handler may handle the EXIT event for state teardown/cleanup
             e) State handler may handle the INIT for self transition to child state
             f) Self transition to child state MUST NOT be handled by ENTRY or EXIT event
             g) Events ENTRY, EXIT and INIT do no need to return None for brevity
    实现相机 HSM 并从 HSM 类继承的类 创建 HSM 需要执行以下步骤：
     1） 初始化基本 HSM 类
     2） 定义 HSM 状态层次结构
     3） 设置起始状态
     4） 定义状态处理程序
        a） 如果事件已处理，则状态处理程序必须返回“None”
        b） 如果未处理事件，状态处理程序必须返回“event”
        c） 状态处理程序可以处理状态设置的 ENTRY 事件
        d） 状态处理程序可以处理状态拆解清理的 EXIT 事件
        e） 状态处理程序可以处理 INIT 以自行转换到子状态
        f） 自我转换到子状态不得由进入或退出事件处理
        g） 事件 ENTRY, EXIT and INIT 不需要返回 None 为简洁起见
     */
    HSM parent;
    // Child members
    char param1;
    char param2;
} CAMERA;

CAMERA basic;
HSM_STATE CAMERA_StateOff;
HSM_STATE CAMERA_StateOn;
HSM_STATE CAMERA_StateOnShoot;
HSM_STATE CAMERA_StateOnDisp;
HSM_STATE CAMERA_StateOnDispPlay;
HSM_STATE CAMERA_StateOnDispMenu;

HSM_EVENT CAMERA_StateOffHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == HSME_ENTRY) {
        printf("\tEnter Low Power Mode\n");
    } else if (event == HSME_EXIT) {
        printf("\tExit Low Power Mode\n");
    } else if (event == HSME_PWR) {
        HSM_Tran(This, &CAMERA_StateOn, 0, nullptr);
        return 0;
    }
    return event;
}

HSM_EVENT CAMERA_StateOnHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == HSME_ENTRY) {
        printf("\tOpen Lens\n");
    } else if (event == HSME_EXIT) {
        printf("\tClose Lens\n");
    } else if (event == HSME_INIT) {
        HSM_Tran(This, &CAMERA_StateOnShoot, nullptr, nullptr);
    } else if (event == HSME_PWR) {
        HSM_Tran(This, &CAMERA_StateOff, nullptr, nullptr);
        return 0;
    } else if (event == HSME_LOWBATT) {
        printf("\tBeep low battery warning\n");
        return 0;
    }
    return event;
}

HSM_EVENT CAMERA_StateOnShootHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == HSME_ENTRY) {
        printf("\tEnable Sensor\n");
    } else if (event == HSME_EXIT) {
        printf("\tDisable Sensor\n");
    } else if (event == HSME_RELEASE) {
        printf("\tCLICK!, save photo\n");
        return 0;
    } else if (event == HSME_MODE) {
        HSM_Tran(This, &CAMERA_StateOnDispPlay, 0, nullptr);
        return 0;
    }
    return event;
}

HSM_EVENT CAMERA_StateOnDispHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == HSME_ENTRY) {
        printf("\tTurn on LCD\n");
    } else if (event == HSME_EXIT) {
        printf("\tTurn off LCD\n");
    }
    return event;
}

HSM_EVENT CAMERA_StateOnDispPlayHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == HSME_ENTRY) {
        printf("\tDisplay Pictures\n");
    } else if (event == HSME_MODE) {
        HSM_Tran(This, &CAMERA_StateOnDispMenu, 0, nullptr);
        return 0;
    }
    return event;
}

HSM_EVENT CAMERA_StateOnDispMenuHndlr(HSM *This, HSM_EVENT event, void *param) {
    if (event == HSME_ENTRY) {
        printf("\tDisplay Menu\n");
    } else if (event == HSME_MODE) {
        HSM_Tran(This, &CAMERA_StateOnShoot, 0, nullptr);
        return 0;
    }
    return event;
}

const char *HSM_Evt2Str(uint32_t event) {
    switch (event) {
        case HSME_PWR:
            return "HSME_PWR";
        case HSME_RELEASE:
            return "HSME_RELEASE";
        case HSME_MODE:
            return "HSME_MODE";
        case HSME_LOWBATT:
            return "HSME_LOWBATT";
    }
}

void CAMERA_Init(CAMERA *This, char *name) {
    // Step 1: 创建 HSM 状态
    HSM_STATE_Create(&CAMERA_StateOff, "Off", CAMERA_StateOffHndlr, nullptr);
    HSM_STATE_Create(&CAMERA_StateOn, "On", CAMERA_StateOnHndlr, nullptr);
    HSM_STATE_Create(&CAMERA_StateOnShoot, "On.Shoot", CAMERA_StateOnShootHndlr, &CAMERA_StateOn);
    HSM_STATE_Create(&CAMERA_StateOnDisp, "On.Disp", CAMERA_StateOnDispHndlr, &CAMERA_StateOn);
    HSM_STATE_Create(&CAMERA_StateOnDispPlay, "On.Disp.Play", CAMERA_StateOnDispPlayHndlr, &CAMERA_StateOnDisp);
    HSM_STATE_Create(&CAMERA_StateOnDispMenu, "On.Disp.Menu", CAMERA_StateOnDispMenuHndlr, &CAMERA_StateOnDisp);
    // Step 2: 初始化 HSM 和启动状态
    HSM_Create((HSM *) This, name, &CAMERA_StateOff);
    // Step 3: [可选]启用 HSM 调试
    HSM_SET_PREFIX((HSM *) This, "[DBG] ");
    HSM_SET_DEBUG((HSM *) This, HSM_SHOW_ALL);
    // Step 4: 相机成员初始化
    This->param1 = 0;
    This->param2 = 1;
}

void CAMERA_Run(CAMERA *This, HSM_EVENT event, void *param) {
    HSM_Run((HSM *) This, event, param);
}

//int main(void) {
//    // Instantiate Camera
//    CAMERA_Init(&basic, const_cast<char *>("Canon"));
//    // Turn on the Power
//    CAMERA_Run(&basic, HSME_PWR, nullptr);
//    // Take a picture
//    CAMERA_Run(&basic, HSME_RELEASE, nullptr);
//    // Take another picture
//    CAMERA_Run(&basic, HSME_RELEASE, nullptr);
//    // Playback the photo
//    CAMERA_Run(&basic, HSME_MODE, nullptr);
//    // Oops, pushed the release button by accident
//    CAMERA_Run(&basic, HSME_RELEASE, nullptr);
//    // Go to menu settings
//    CAMERA_Run(&basic, HSME_MODE, nullptr);
//    // Uh oh, low battery
//    CAMERA_Run(&basic, HSME_LOWBATT, nullptr);
//    // Time to turn it off
//    CAMERA_Run(&basic, HSME_PWR, nullptr);
//    return 0;
//}
