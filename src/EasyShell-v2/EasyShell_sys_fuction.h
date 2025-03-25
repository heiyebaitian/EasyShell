#ifndef __EASY_SHELL_SYS_FUCTION__H
#define __EASY_SHELL_SYS_FUCTION__H
#pragma once

#include <Arduino.h>
#include "EasyShell_driver.h"

/* eshell_LoadingBar()函数模式 */
typedef enum
{
      ES_LOADING_BAR_INIT=0,
      ES_LOADING_BAR_RUN,
      ES_LOADING_BAR_KILL
} EASYSHELL_LOADING_BAR_MOD;

/* eshell_LoadingBar()函数返回值 */
typedef enum
{
      ES_LOADING_BAR_STATE_RUN=0,
      ES_LOADING_BAR_STATE_INIT,
      ES_LOADING_BAR_STATE_QUIT,
      ES_LOADING_BAR_STATE_ERROR
} EASYSHELL_LOADING_BAR_STATE;

void eshell_printf_custom(int textattribute,int forecolor,int backgroundcolor,const char *str, ...);
uint8_t eshell_LoadingBar(int mode,int percent,const char *str);

#endif