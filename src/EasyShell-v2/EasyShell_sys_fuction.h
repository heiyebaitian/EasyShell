#ifndef __EASY_SHELL_SYS_FUCTION__H
#define __EASY_SHELL_SYS_FUCTION__H
#pragma once

#include <Arduino.h>
#include "EasyShell_driver.h"

extern eshell_cmd_list eshell_sys_static_cmd_list[];
extern uint32_t eshell_sys_static_cmd_list_num;

void cmd_test(int argc, char**argv);
void cmd_help(int argc, char**argv);

#endif