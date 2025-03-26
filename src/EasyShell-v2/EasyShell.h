#ifndef __EASY_SHELL__H
#define __EASY_SHELL__H
#pragma once

#include <Arduino.h>
#include "EasyShell_driver.h"
#include "EasyShell_sys_fuction.h"
#include "EasyShell_sys_app.h"


bool eshell_read_next_char(char &buffer);
uint8_t eshell_get_cmd(char *buf, uint32_t maxlen);
uint8_t eshell_check_blank(char* msg);
uint8_t eshell_get_param(char* msg, const char* delim, char* get[], uint32_t param_max_num);
uint8_t eshell_delete_char_at_cursor(char *str, uint32_t length, uint32_t cursor);
void eshell_match(uint32_t argc, char**argv, uint32_t eshell_cmd_list_num, eshell_cmd_list cmd_list[]);
void eshell_execute();
void eshell_moveCursorLeft(char *buf);
void eshell_moveCursorRight(char *buf);
void eshell_cmd_input_prompt();
// void eshell_previous_command();
// void eshell_next_command();


#endif