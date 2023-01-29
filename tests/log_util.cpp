//
// Created by 钱昱 on 2023/1/29.
//

#include <string>
#include "elog.h"
#include "elog_file.h"

using namespace std;


void ELOGLIB_Init() {
    setbuf(stdout, nullptr);
    elog_init();
    elog_start();
    elog_file_init();
    
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
}

void ELOGLIB_Deinit(){
    elog_file_deinit();
}

void logi(const string &msg) {
    elog_file_write(msg.c_str(), msg.length());
    log_i(msg.c_str());
}

void loge(const string &msg) {
    elog_file_write(msg.c_str(), msg.length());
    log_e(msg.c_str());
}
