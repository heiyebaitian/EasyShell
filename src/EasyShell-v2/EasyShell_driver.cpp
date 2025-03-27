#include "EasyShell_driver.h"
#include "EasyShell_sys_app.h"
#include "EasyShell_sys_fuction.h"



/* 动态命令表 */
eshell_cmd_list* eshell_dynamic_cmd_list = NULL;
uint32_t eshell_dynamic_cmd_count = 0;

static uint8_t eshell_init_flag = 0;  // 初始化标记
EASYSHELL_PAGE_ID eshell_page_id = ES_PAGE_MAIN;  //  当前终端页面ID
EASYSHELL_RUN_STATE eshell_run_state = ES_RUN_INIT; //  当前EasyShell运行状态

/* 获取一个字符
 * @return 获取的字符
 */
char eshell_getchar(){
  char rcv_char_buffer = 0; // 接收字符缓存
  char return_char = 0; // 待返回的字符
  bool rcv_state = false; // 接收状态
  while(1){
    // 如果缓冲区有数据且函数未完成接收 则接收
    if (eshell_serial_available()){
      rcv_char_buffer = eshell_serial_read();
      if(rcv_state == false){
        return_char = rcv_char_buffer;
        rcv_state = true;
        //如果是换行符或Enter则直接返回
        if(return_char == 0x10 || return_char == 0x0D){
          eshell_printf("\r\n");
          return return_char;
        } 
      }
      // 如果收到换行符或Enter则返回
      else if(rcv_char_buffer == 0x10 || rcv_char_buffer == 0x0D){
        eshell_printf("\r\n");
        return return_char;
      }
      eshell_putchar(rcv_char_buffer);
    }
  }
}

/* 终端错误日志打印函数 */
void eshell_printf_error(const char *str, ...){
  char buffer[ESHELL_PRINT_BUFFER_MAX_LEN];
  va_list args;
  va_start(args, str);

  // 格式化字符串到缓冲区
  vsnprintf(buffer, sizeof(buffer), str, args);
  eshell_printf("%lf ",eshell_systime_us()/1000000.0);
  eshell_printf("[EasyShell]");
  // 输出缓冲区内容
  eshell_printf_custom(1,31,40,"Error:%s \r\n", buffer);
  va_end(args);
}



/* 终端警告日志打印函数 */
void eshell_printf_warning(const char *str, ...){
  char buffer[ESHELL_PRINT_BUFFER_MAX_LEN];
  va_list args;
  va_start(args, str);

  // 格式化字符串到缓冲区
  vsnprintf(buffer, sizeof(buffer), str, args);
  eshell_printf("%lf ",eshell_systime_us()/1000000.0);
  eshell_printf("[EasyShell]");
  // 输出缓冲区内容
  eshell_printf_custom(1,33,40,"Warning:%s \r\n", buffer);
  va_end(args);
}



/* 终端信息日志打印函数 */
void eshell_printf_info(const char *str, ...){
  char buffer[ESHELL_PRINT_BUFFER_MAX_LEN];
  va_list args;
  va_start(args, str);

  // 格式化字符串到缓冲区
  vsnprintf(buffer, sizeof(buffer), str, args);
  eshell_printf("%lf ",eshell_systime_us()/1000000.0);
  // 输出缓冲区内容
  eshell_printf("[EasyShell]");
  eshell_printf_custom(1,34,40,"%s \r\n", buffer);
  va_end(args);
}


/* 终端调试日志打印函数 */
void eshell_printf_debug(const char *str, ...){
  char buffer[ESHELL_PRINT_BUFFER_MAX_LEN];
  va_list args;
  va_start(args, str);

  // 格式化字符串到缓冲区
  vsnprintf(buffer, sizeof(buffer), str, args);
  eshell_printf("%lf ",eshell_systime_us()/1000000.0);
  // 输出缓冲区内容
  eshell_printf("[EasyShell]");
  eshell_printf_custom(1,32,40,"%s \r\n", buffer);
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
    // 检查输入参数的有效性
    if (static_list == NULL || static_size == 0) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("Invalid input parameters for merging static command list!");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("合并静态命令表的输入参数无效！");
        #endif
        return 1; // 返回错误码
    }

    // 检查静态命令表内部是否有重复命令
    for (uint32_t i = 0; i < static_size; i++) {
        const char* cmd_name = static_list[i].name;
        for (uint32_t j = i + 1; j < static_size; j++) {
            if (strcmp(static_list[j].name, cmd_name) == 0) {
                #if ESHELL_LANGUAGE_SET == 0
                    eshell_printf_error("Duplicate command '%s' found in the static command table. The merge operation cannot be completed!", cmd_name);
                #elif ESHELL_LANGUAGE_SET == 1
                    eshell_printf_error("静态命令表中发现重复命令 '%s'，无法完成合并操作！", cmd_name);
                #endif
                return 1; // 返回错误码
            }
        }
    }

    // 计算新的总大小
    uint32_t new_size = eshell_dynamic_cmd_count + static_size;
    if (new_size > ESHELL_DYNAMIC_CMD_COUNT_MAX) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("Insufficient dynamic command table space to merge the static table!");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("动态命令表空间不足，无法将静态表合并!");
        #endif
        return 1; // 返回错误码
    }

    // 动态分配新的内存
    eshell_cmd_list* new_list = (eshell_cmd_list*)realloc(eshell_dynamic_cmd_list, new_size * sizeof(eshell_cmd_list));
    if (new_list == NULL) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("Fatal error - Dynamic command table creation failed!");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("致命错误-动态命令表创建失败!");
        #endif
        eshell_run_state = ES_RUN_ERROR_MEMORY_ALLOCATION_ERROR; // 将系统标志位置为内存分配错误
        return 1; // 返回错误码
    }

    // 更新指针和计数
    eshell_dynamic_cmd_list = new_list;

    // 将静态列表的内容复制到动态列表中，同时检查重复命令
    uint32_t added_count = 0; // 成功添加的命令数量
    for (uint32_t i = 0; i < static_size; i++) {
        const char* static_cmd_name = static_list[i].name;

        // 检查是否命令名称已存在
        bool is_duplicate = false;
        for (uint32_t j = 0; j < eshell_dynamic_cmd_count; j++) {
            if (strcmp(eshell_dynamic_cmd_list[j].name, static_cmd_name) == 0) {
                is_duplicate = true;
                break;
            }
        }

        if (is_duplicate) {
            #if ESHELL_LANGUAGE_SET == 0
                eshell_printf_warning("Command '%s' already exists in the dynamic command list and will be skipped.", static_cmd_name);
            #elif ESHELL_LANGUAGE_SET == 1
                eshell_printf_warning("命令 '%s' 已存在于动态命令列表中，将被跳过。", static_cmd_name);
            #endif
            continue; // 跳过重复命令
        }

        // 添加非重复命令到动态列表
        eshell_dynamic_cmd_list[eshell_dynamic_cmd_count + added_count] = static_list[i];
        added_count++;
    }

    // 更新动态列表的大小
    eshell_dynamic_cmd_count += added_count;

    // 如果没有添加任何命令，返回警告状态
    if (added_count == 0) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_warning("No new commands were added from the static list.");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_warning("未从静态表中添加任何新命令。");
        #endif
        return 2; // 返回警告状态码
    }

    return 0; // 成功添加
}


/* 命令移除函数
 * @param cmd_name 需要移除的命令名称
 * @return 状态标志
 */
uint8_t eshell_remove_cmd(const char* cmd_name) {
    if (cmd_name == NULL || eshell_dynamic_cmd_list == NULL || eshell_dynamic_cmd_count == 0) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("Invalid input or dynamic command list is empty!");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("输入无效或动态命令列表为空！");
        #endif
        return 1; // 返回错误码
    }

    uint32_t target_index = UINT32_MAX; // 初始化为目标未找到

    // 查找目标命令的索引
    for (uint32_t i = 0; i < eshell_dynamic_cmd_count; i++) {
        if (strcmp(eshell_dynamic_cmd_list[i].name, cmd_name) == 0) {
            target_index = i;
            break;
        }
    }

    if (target_index == UINT32_MAX) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("The command '%s' was not found in the dynamic command list, and therefore the deletion operation cannot be performed!", cmd_name);
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("命令 '%s' 未在动态命令列表中找到，无法执行删除操作！", cmd_name);
        #endif
        return 1; // 返回错误码
    }

    // 移除目标命令（将后续命令向前移动）
    for (uint32_t i = target_index; i < eshell_dynamic_cmd_count - 1; i++) {
        eshell_dynamic_cmd_list[i] = eshell_dynamic_cmd_list[i + 1];
    }

    // 更新动态列表的大小
    eshell_dynamic_cmd_count--;

    // 如果动态列表为空，释放内存
    if (eshell_dynamic_cmd_count == 0) {
        free(eshell_dynamic_cmd_list);
        eshell_dynamic_cmd_list = NULL;
    } else {
        // 否则重新分配内存以减少空间占用
        eshell_cmd_list* new_list = (eshell_cmd_list*)realloc(eshell_dynamic_cmd_list, eshell_dynamic_cmd_count * sizeof(eshell_cmd_list));
        if (new_list == NULL && eshell_dynamic_cmd_count > 0) {
            #if ESHELL_LANGUAGE_SET == 0
                eshell_printf_error("Fatal error - Dynamic command table reallocation failed!");
            #elif ESHELL_LANGUAGE_SET == 1
                eshell_printf_error("致命错误-动态命令表重新分配失败！");
            #endif
            eshell_run_state = ES_RUN_ERROR_MEMORY_ALLOCATION_ERROR; // 将系统标志位置为内存分配错误
            return 1; // 返回错误码
        }
        eshell_dynamic_cmd_list = new_list;
    }

    return 0; // 成功删除
}


/* 命令添加函数
 * @param name 命令名称
 * @param func 函数地址
 * @param help 命令简介
 * @param hide 是否隐藏命令不在help中显示
 * @return 状态标志
 */
uint8_t eshell_add_cmd(const char* name, eshell_cmd_func_t func, const char* help, bool hide) {
    // 检查输入参数的有效性
    if (name == NULL || func == NULL || help == NULL) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("Invalid input parameters for adding a command!");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("添加命令的输入参数无效！");
        #endif
        return 1; // 返回错误码
    }

    // 检查是否超过最大命令数量限制
    if (eshell_dynamic_cmd_count >= ESHELL_DYNAMIC_CMD_COUNT_MAX) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("Dynamic command table is full! Cannot add more commands.");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("动态命令表已满！无法添加更多命令。");
        #endif
        return 1; // 返回错误码
    }

    // 检查是否命令名称已存在
    for (uint32_t i = 0; i < eshell_dynamic_cmd_count; i++) {
        if (strcmp(eshell_dynamic_cmd_list[i].name, name) == 0) {
            #if ESHELL_LANGUAGE_SET == 0
                eshell_printf_error("The command '%s' already exists in the dynamic command list and cannot be added to the dynamic command table!", name);
            #elif ESHELL_LANGUAGE_SET == 1
                eshell_printf_error("命令 '%s' 已存在于动态命令列表中,无法添加至动态命令表！", name);
            #endif
            return 1; // 返回错误码
        }
    }

    // 分配或扩展内存
    eshell_cmd_list* new_list = (eshell_cmd_list*)realloc(eshell_dynamic_cmd_list, (eshell_dynamic_cmd_count + 1) * sizeof(eshell_cmd_list));
    if (new_list == NULL) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("Fatal error - Dynamic command table reallocation failed!");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("致命错误-动态命令表重新分配失败！");
        #endif
        eshell_run_state = ES_RUN_ERROR_MEMORY_ALLOCATION_ERROR; // 将系统标志位置为内存分配错误
        return 1; // 返回错误码
    }

    // 更新动态命令表指针
    eshell_dynamic_cmd_list = new_list;

    // 添加新命令到动态命令表
    eshell_dynamic_cmd_list[eshell_dynamic_cmd_count].name = name;
    eshell_dynamic_cmd_list[eshell_dynamic_cmd_count].func = func;
    eshell_dynamic_cmd_list[eshell_dynamic_cmd_count].help = help;
    eshell_dynamic_cmd_list[eshell_dynamic_cmd_count].hide = hide;

    // 更新动态命令表的大小
    eshell_dynamic_cmd_count++;

    return 0; // 成功添加
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
    #if ESHELL_LANGUAGE_SET == 0
      eshell_printf_error("Repeated initialization of EasyShell!");
    #elif ESHELL_LANGUAGE_SET == 1
      eshell_printf_error("重复初始化EasyShell！");
    #endif
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
    #if ESHELL_LANGUAGE_SET == 0
      eshell_printf_error("Repeated initialization of EasyShell!");
    #elif ESHELL_LANGUAGE_SET == 1
      eshell_printf_error("重复初始化EasyShell！");
    #endif
    return ES_INIT_REPEATED_INITIALIZATION;
  }
}

