#include "EasyShell_driver.h"
#include "EasyShell_sys_app.h"



/* 动态命令表 */
eshell_cmd_list* eshell_dynamic_cmd_list = NULL;
uint32_t eshell_dynamic_cmd_count = 0;

static uint8_t eshell_init_flag = 0;  // 初始化标记
EASYSHELL_PAGE_ID eshell_page_id = ES_PAGE_MAIN;  //  当前终端页面ID
EASYSHELL_RUN_STATE eshell_run_state = ES_RUN_INIT; //  当前EasyShell运行状态

/* 终端错误打印函数 */
void eshell_printf_error(const char *str, ...){
  char buffer[ESHELL_PRINT_BUFFER_MAX_LEN];
  va_list args;
  va_start(args, str);

  // 格式化字符串到缓冲区
  vsnprintf(buffer, sizeof(buffer), str, args);
  eshell_printf("%lf ",eshell_systime_us()/1000000.0);
  eshell_printf("[EasyShell]Error:");
  // 输出缓冲区内容
  eshell_printf("%s \r\n", buffer);
  va_end(args);
}



/* 终端警告打印函数 */
void eshell_printf_warning(const char *str, ...){
  char buffer[ESHELL_PRINT_BUFFER_MAX_LEN];
  va_list args;
  va_start(args, str);

  // 格式化字符串到缓冲区
  vsnprintf(buffer, sizeof(buffer), str, args);
  eshell_printf("%lf ",eshell_systime_us()/1000000.0);
  eshell_printf("[EasyShell]Warning:");
  // 输出缓冲区内容
  eshell_printf("%s \r\n", buffer);
  va_end(args);
}



/* 终端日志打印函数 */
void eshell_printf_log(const char *str, ...){
  char buffer[ESHELL_PRINT_BUFFER_MAX_LEN];
  va_list args;
  va_start(args, str);

  // 格式化字符串到缓冲区
  vsnprintf(buffer, sizeof(buffer), str, args);
  eshell_printf("%lf ",eshell_systime_us()/1000000.0);
  eshell_printf("[EasyShell]");
  // 输出缓冲区内容
  eshell_printf("%s \r\r", buffer);
  va_end(args);
}


/* 系统时间函数(微秒)
 * @return 系统时间(微秒)
 */
uint32_t eshell_systime_us(){
  return micros();
}



/* 系统时间函数(毫秒)
 * @return 系统时间(毫秒)
 */
uint32_t eshell_systime_ms(){
  return millis();
}


/* 合并将静态命令表添加至动态命令表
 * 注意：请勿重复添加您的静态表至动态表，此操作不会校验是否有重复命令
 * @param static_list 需要添加的命令表
 * @param static_size 需要添加的命令表命令数量
 * @return 状态标志
 */
uint8_t eshell_add_static_cmd_lists(const eshell_cmd_list* static_list, uint32_t static_size) {
    // 计算新的总大小
    uint32_t new_size = eshell_dynamic_cmd_count + static_size;
    if(new_size > ESHELL_DYNAMIC_CMD_COUNT_MAX){
      eshell_printf_error("动态命令表空间不足，无法将静态表合并!");
      return 1;
    }

    // 动态分配新的内存
    eshell_cmd_list* new_list = (eshell_cmd_list*)realloc(eshell_dynamic_cmd_list, new_size * sizeof(eshell_cmd_list));
    if (new_list == NULL) {
      eshell_printf_error("致命错误:动态命令表创建失败!");
      eshell_run_state = ES_RUN_ERROR_MEMORY_ALLOCATION_ERROR;  // 将系统标志位置为内存分配错误
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
uint8_t eshell_Init(eshell_cmd_list* user_static_cmd_list, uint32_t user_static_cmd_list_num){
  if(eshell_init_flag == 0 && eshell_run_state == ES_RUN_INIT){
    ESHELL_SERIAL.begin(ESHELL_SERIAL_BPS);
    if(eshell_add_static_cmd_lists(eshell_sys_static_cmd_list, eshell_sys_static_cmd_list_num) != 0){return ES_INIT_ERROR;} // 添加系统命令静态表至动态表
    if(eshell_add_static_cmd_lists(user_static_cmd_list, user_static_cmd_list_num) != 0){return ES_INIT_ERROR;} // 添加用户静态表至动态表
    eshell_run_state = ES_RUN_NORMAL;
    eshell_init_flag = 1;
    return ES_INIT_NORMAL;
  }
  else{
    eshell_printf_error("错误:重复初始化EasyShell！");
    return ES_INIT_REPEATED_INITIALIZATION;
  }
}

/* EasyShell 初始化函数(无用户表型)
 * @return 状态标志
 */
uint8_t eshell_Init(){
  if(eshell_init_flag == 0 && eshell_run_state == ES_RUN_INIT){
    ESHELL_SERIAL.begin(ESHELL_SERIAL_BPS);
    if(eshell_add_static_cmd_lists(eshell_sys_static_cmd_list, eshell_sys_static_cmd_list_num) != 0){return ES_INIT_ERROR;} // 添加系统命令静态表至动态表
    eshell_run_state = ES_RUN_NORMAL;
    eshell_init_flag = 1;
    return ES_INIT_NORMAL;
  }
  else{
    eshell_printf_error("重复初始化EasyShell！");
    return ES_INIT_REPEATED_INITIALIZATION;
  }
}

