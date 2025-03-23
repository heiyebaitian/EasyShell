#include "EasyShell_driver.h"
#include "EasyShell_sys_fuction.h"

char eshell_rcv_buffer[ESHELL_CMD_PARAM_MAX_NUM + 1] = {};  //  命令接收缓冲区

/* 动态命令表 */
eshell_cmd_list* eshell_dynamic_cmd_list = NULL;
uint32_t eshell_dynamic_cmd_count = 0;



/* 合并将静态命令表添加至动态命令表
 * 注意：请勿重复添加您的静态表至动态表，此操作不会校验是否有重复命令
 * @param static_list 需要添加的命令表
 * @param static_size 需要添加的命令表命令数量
 * @return 状态标志
 */
uint32_t add_static_cmd_lists(const eshell_cmd_list* static_list, uint32_t static_size) {
    // 计算新的总大小
    uint32_t new_size = eshell_dynamic_cmd_count + static_size;
    if(new_size > ESHELL_DYNAMIC_CMD_COUNT_MAX){
      eshell_printf("EasyShell:致命错误-动态命令表空间不足，无法将静态表合并！\r\n");
      return 1;
    }

    // 动态分配新的内存
    eshell_cmd_list* new_list = (eshell_cmd_list*)realloc(eshell_dynamic_cmd_list, new_size * sizeof(eshell_cmd_list));
    if (new_list == NULL) {
        eshell_printf("EasyShell:致命错误-动态命令表创建失败！\r\n");
        return 1;
    }

    // 更新指针和计数
    eshell_dynamic_cmd_list = new_list;

    // 将静态列表的内容复制到动态列表中
    for (uint32_t i = 0; i < static_size; i++) {
        eshell_dynamic_cmd_list[eshell_dynamic_cmd_count + i] = static_list[i];
    }

    // 更新动态列表的大小
    eshell_dynamic_cmd_count = new_size;
    return 0;
}



/* EasyShell 初始化函数(有用户表型)
 * @param user_static_cmd_list 需要添加的命令表
 * @param user_static_cmd_list_num 需要添加的命令表命令数量
 * @return 状态标志
 */
uint32_t eshell_Init(eshell_cmd_list* user_static_cmd_list, uint32_t user_static_cmd_list_num){
  ESHELL_SERIAL.begin(ESHELL_SERIAL_BPS);
  add_static_cmd_lists(eshell_sys_static_cmd_list, eshell_sys_static_cmd_list_num); // 添加系统命令静态表至动态表
  add_static_cmd_lists(user_static_cmd_list, user_static_cmd_list_num); // 添加用户静态表至动态表
  return ES_RUN_NORMAL;
}

/* EasyShell 初始化函数(无用户表型)
 * @return 状态标志
 */
uint32_t eshell_Init(){
  ESHELL_SERIAL.begin(ESHELL_SERIAL_BPS);
  add_static_cmd_lists(eshell_sys_static_cmd_list, eshell_sys_static_cmd_list_num); // 添加系统命令静态表至动态表
  return ES_RUN_NORMAL;
}

