#include <Arduino.h>

#ifndef __EASY_SHELL__H
#define __EASY_SHELL__H
#pragma once

#define ESHELL_SERIAL Serial  // 定义EasyShell终端使用的串口
#define ESHELL_CMD_BUFFER_MAX_LEN 128 // 最大命令缓冲区长度 可根据单片机内存大小调整此值
#define ESHELL_CMD_PARAM_MAX_NUM 64 //  最大命令参数数量

typedef enum
{
      ES_GET_NULL_OR_UMCOMPLETED=0, ES_GET_COMPLETED, ES_GET_ERROR_BUFFER_REMOVAL
} EASYSHELL_GET_RETURN_STATE;


typedef void(*eshell_cmd_func_t)(int argc, char**argv);
typedef struct /* 定义命令结构体 */
{
  char* name;
  eshell_cmd_func_t func;
  char* help;
}eshell_cmd_list;


uint8_t eshell_get(char *buf, uint8_t maxlen);


#endif