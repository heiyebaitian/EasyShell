#ifndef __EASY_SHELL_CFG__H
#define __EASY_SHELL_CFG__H
#pragma once

#define ESHELL_VERSION "v2.1.0"

#define ESHELL_SERIAL Serial  // 定义EasyShell终端使用的串口
#define ESHELL_SERIAL_BPS 115200  // 定义EasyShell终端使用的串口波特率
#define ESHELL_CMD_BUFFER_MAX_LEN 512 // 最大命令缓冲区长度 可根据单片机内存大小调整此值 请勿将此值设置过小
#define ESHELL_PRINT_BUFFER_MAX_LEN 512 // 最大输出缓冲区长度 可根据单片机内存大小调整此值 请勿将此值设置过小
#define ESHELL_CMD_PARAM_MAX_NUM 512 //  最大命令参数数量 推荐设置为 ESHELL_CMD_BUFFER_MAX_LEN
#define ESHELL_DYNAMIC_CMD_COUNT_MAX 200  // EasyShell 动态表管理的最大命令数量 超出此值将无法添加新命令至表中
#define ESHELL_CMD_EDIT_DISTANCE_MATCHING_DIVISOR 6 // 命令编辑距离匹配除数(>0) 调小此值可以提高相似命令建议的模糊性 不建议将此值设置的过小    

// #define ESHELL_HISTORICAL_RECORD_MAX 50 // 命令历史记录最大记录值

#define ESHELL_LANGUAGE_SET 1 // 0--English  1--简体中文

#endif