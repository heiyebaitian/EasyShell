#ifndef __EASY_SHELL_SYS_APP__H
#define __EASY_SHELL_SYS_APP__H
#pragma once

#include <Arduino.h>
#include "EasyShell_driver.h"
#include "Easyshell_cfg.h"  // 配置文件

extern eshell_cmd_list eshell_sys_static_cmd_list[];
extern uint32_t eshell_sys_static_cmd_list_num;

void cmd_help(int argc, char**argv);
void cmd_eshell(int argc, char**argv);

#endif