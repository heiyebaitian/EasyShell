#ifndef __EASY_SHELL__H
#define __EASY_SHELL__H
#pragma once

#include <Arduino.h>
#include "EasyShell_driver.h"
#include "EasyShell_sys_fuction.h"


uint8_t eshell_get(char *buf, uint8_t maxlen);
uint8_t eshell_get_param(char* msg, char*delim, char* get[], int param_max_num);
void eshell_match(int argc, char**argv, uint32_t eshell_cmd_list_num, eshell_cmd_list cmd_list[]);
void eshell_execute();


#endif