#ifndef __EASY_SHELL_DRIVER__H
#define __EASY_SHELL_DRIVER__H
#pragma once

#include <Arduino.h> // 引入uint，String等变量类型支持，请根据平台更换为其他库
#include <stdio.h>
#include <stdlib.h> // 动态内存分配
#include "Easyshell_cfg.h"  // 配置文件







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
      ES_PAGE_MAIN=0, // 主界面-命令输入界面
      ES_PAGE_APP // 应用程序界面
} EASYSHELL_PAGE_ID;

/* eshell_Init()函数返回值 */
typedef enum
{
      ES_INIT_NORMAL=0,
      ES_INIT_REPEATED_INITIALIZATION,
      ES_INIT_ERROR
} EASYSHELL_INIT_RETURN_STATE;

/* eshell_get()函数返回值 */
typedef enum
{
      ES_GET_NULL_OR_UMCOMPLETED=0,
      ES_GET_COMPLETED,
      ES_GET_ERROR_BUFFER_REMOVAL
} EASYSHELL_GET_RETURN_STATE;



/* 命令表结构体
 * name 命令名称
 * func 函数地址
 * help 命令简介 
 * hide 隐藏命令不在help中显示
 */
typedef void(*eshell_cmd_func_t)(int argc, char**argv);
typedef struct /* 定义命令结构体 */
{
  const char *name;
  eshell_cmd_func_t func;
  const char *help;
  bool hide;
}eshell_cmd_list;



extern eshell_cmd_list* eshell_dynamic_cmd_list; // 动态命令表
extern uint32_t eshell_dynamic_cmd_count;  // 动态命令表当前维护的命令数量
extern EASYSHELL_PAGE_ID eshell_page_id;  //  当前终端页面ID
extern EASYSHELL_RUN_STATE eshell_run_state; //  当前EasyShell运行状态



/* 终端打印函数
 * @param format 输出字符串
 */
#define eshell_printf(...) ESHELL_SERIAL.printf(__VA_ARGS__)

/* 终端字符输出函数
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

/* 获取一个字符
 * @return 获取的字符
 */
char eshell_getchar();

/* 终端错误日志打印函数 */
void eshell_printf_error(const char *str, ...);

/* 终端警告日志打印函数 */
void eshell_printf_warning(const char *str, ...);

/* 终端信息日志打印函数 */
void eshell_printf_info(const char *str, ...);

/* 终端调试日志打印函数 */
void eshell_printf_debug(const char *str, ...);

/* 系统时间函数(微秒)
 * @return 系统时间(微秒)
 */
uint32_t eshell_systime_us();

/* 系统时间函数(毫秒)
 * @return 系统时间(毫秒)
 */
uint32_t eshell_systime_ms();

/* 合并将静态命令表添加至动态命令表
 * 注意：请勿重复添加您的静态表至动态表，此操作不会校验是否有重复命令
 * @param static_list 需要添加的命令表
 * @param static_size 需要添加的命令表命令数量
 * @return 状态标志
 */
uint8_t eshell_add_static_cmd_lists(const eshell_cmd_list* static_list, uint32_t static_size);

/* 命令移除函数
 * @param cmd_name 需要移除的命令名称
 * @return 状态标志
 */
uint8_t eshell_remove_cmd(const char* cmd_name);

/* 命令添加函数
 * @param name 命令名称
 * @param func 函数地址
 * @param help 命令简介
 * @param hide 是否隐藏命令不在help中显示
 * @return 状态标志
 */
uint8_t eshell_add_cmd(const char* name, eshell_cmd_func_t func, const char* help, bool hide);

/* EasyShell 初始化函数
 * @return 状态标志
 */
uint8_t eshell_Init(eshell_cmd_list* user_static_cmd_list, uint32_t user_static_cmd_list_num);
uint8_t eshell_Init();

#endif