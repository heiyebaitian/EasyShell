#ifndef __EASY_SHELL_DRIVER__H
#define __EASY_SHELL_DRIVER__H
#pragma once

#include <Arduino.h> // 引入uint，String等变量类型支持，请根据平台更换为其他库
#include <stdio.h>
#include <stdlib.h> // 动态内存分配




#define ESHELL_SERIAL Serial  // 定义EasyShell终端使用的串口
#define ESHELL_SERIAL_BPS 115200  // 定义EasyShell终端使用的串口波特率
#define ESHELL_CMD_BUFFER_MAX_LEN 128 // 最大命令缓冲区长度 可根据单片机内存大小调整此值
#define ESHELL_CMD_PARAM_MAX_NUM 64 //  最大命令参数数量
#define ESHELL_DYNAMIC_CMD_COUNT_MAX 200  // EasyShell 动态表管理的最大命令数量 超出此值将无法添加新命令至表中



/* EasyShell运行状态标记 */
typedef enum
{
      ES_RUN_NORMAL=0,  // 正常运行
      ES_RUN_INIT,  //  初始化
      ES_RUN_UNKNOWN_ERROR, // 未知错误
      ES_RUN_ERROR_MEMORY_ALLOCATION_ERROR  // 内存分配错误
} EASYSHELL_RUN_STATE;

/* EasyShell界面ID标记 */
typedef enum
{
      ES_PAGE_MAIN=0
} EASYSHELL_PAGE_ID;

/* eshell_get()函数返回值 */
typedef enum
{
      ES_GET_NULL_OR_UMCOMPLETED=0, ES_GET_COMPLETED, ES_GET_ERROR_BUFFER_REMOVAL
} EASYSHELL_GET_RETURN_STATE;



/* 命令表结构体 */
typedef void(*eshell_cmd_func_t)(int argc, char**argv);
typedef struct /* 定义命令结构体 */
{
  const char *name;
  eshell_cmd_func_t func;
  const char *help;
}eshell_cmd_list;


extern char eshell_rcv_buffer[]; //  命令接收缓冲区
extern eshell_cmd_list* eshell_dynamic_cmd_list; // 动态命令表
extern uint32_t eshell_dynamic_cmd_count;  // 动态命令表当前维护的命令数量



/* 串口打印函数
 * @param format 输出字符串
 */
#define eshell_printf(...) ESHELL_SERIAL.printf(__VA_ARGS__)

/* 串口字符输出函数
 * @param in_char 输出字符
 */
#define eshell_putchar(in_char) ESHELL_SERIAL.printf("%c", in_char)

/* 串口缓冲区有无数据检查
 * @return state 状态结果
 */
#define eshell_serial_available() ESHELL_SERIAL.available()

/* 串口读取函数
 * 从串口读取一个字符数据
 * @return char 
 */
#define eshell_serial_read() ESHELL_SERIAL.read()

/* EasyShell 初始化函数
 * @return 状态标志
 */
uint32_t eshell_Init(eshell_cmd_list* user_static_cmd_list, uint32_t user_static_cmd_list_num);
uint32_t eshell_Init();

#endif