#include "EasyShell.h"

char eshell_rcv_buffer[ESHELL_CMD_PARAM_MAX_NUM + 1] = {};  //  命令接收缓冲区
// char eshell_cmd_historical_record[ESHELL_HISTORICAL_RECORD_MAX][ESHELL_CMD_PARAM_MAX_NUM + 1] = {}; // 历史命令记录缓冲区
// uint32_t eshell_cmd_historical_record_length[ESHELL_HISTORICAL_RECORD_MAX] = {}; // 历史命令长度记录缓冲区
// uint32_t eshell_cmd_historical_record_count = 0; // 历史命令记录计数器
// uint32_t eshell_cmd_historical_record_count_cumulant = 0; // 历史命令记录累计计数器
// uint32_t eshell_cmd_historical_record_count_offset = 0; // 历史命令选择指针偏移量
uint32_t cmd_count = 0; // 命令长度计数器
uint32_t cmd_cursor_count = 0; // 光标位置计数器




/* 光标处删除字符函数(数组操作)
 * 此函数将自动完成于光标位置删除光标前一位字符并自动前移后方字符
 * 注意:此函数默认传入字符个数不含'\0'，请根据实际情况调整传入length
 * @param str 待操作数组
 * @param length 数组数据长度
 * @param cursor 光标位置
 * @return 状态标志
 */
uint8_t eshell_delete_char_at_cursor(char *str, uint32_t length, uint32_t cursor) {
    // 检查输入参数是否合法
    if (str == NULL || length <= 0 || cursor < 1 || cursor > length) {
        return 0; // 不合法
    }
    uint32_t i = 0;
    // 从光标位置开始，将后续字符向前移动一位
    for (i = cursor - 1; i < length - 1; i++) {
        str[i] = str[i + 1];
    }
    return 1; // 完成
}



/* 从串口获取下一位字符(用于特殊键判断)
 * @param buffer 接收变量
 * @return 状态标志
 */
bool eshell_read_next_char(char &buffer) {
    if (eshell_serial_available()) {
        buffer = eshell_serial_read();
        return true;
    }
    return false; // 没有可用字符
}


/* 从串口获取原字符串 
 * @param buf 串口字符串接收缓冲区数组
 * @param maxlen 最大命令缓冲区长度
 * @return 状态标志
 */
uint8_t eshell_get_cmd(char *buf, uint32_t maxlen){
  char rcv_char_buffer = 0; // 接收字符缓存
  if (eshell_serial_available()){
    rcv_char_buffer = eshell_serial_read();
    if (cmd_count >= maxlen) /* 缓冲区溢出 */
    {
      buf[0] = '\0'; // 令接收的字符串无效
      cmd_count = 0; // 清零计数器以便后续使用
      cmd_cursor_count = 0;
      return ES_GET_ERROR_BUFFER_REMOVAL;  // 返回溢出错误标志
    }


    if(rcv_char_buffer == 0x1B && eshell_read_next_char(rcv_char_buffer)){  // 特殊按键1B类
      if(rcv_char_buffer == 0x5B && eshell_read_next_char(rcv_char_buffer)){ // 特殊按键1B 5B类

        if(rcv_char_buffer == 0x41){/*eshell_previous_command();*/} // 方向键上
        else if(rcv_char_buffer == 0x42){/*eshell_next_command();*/}  // 方向键下
        else if(rcv_char_buffer == 0x43){eshell_moveCursorRight(buf);}  // 方向键右
        else if(rcv_char_buffer == 0x44){eshell_moveCursorLeft(buf);}  // 方向键左
        else if(rcv_char_buffer == 0x31 && eshell_read_next_char(rcv_char_buffer)){
          if(rcv_char_buffer == 0x7E){} // Home键
          else if(rcv_char_buffer == 0x31 && eshell_read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F1键
          }
          else if(rcv_char_buffer == 0x32 && eshell_read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F2键
          }
          else if(rcv_char_buffer == 0x33 && eshell_read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F3键
          }
          else if(rcv_char_buffer == 0x34 && eshell_read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F4键
          }
          else if(rcv_char_buffer == 0x35 && eshell_read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F5键
          }
          else if(rcv_char_buffer == 0x37 && eshell_read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F6键
          }
          else if(rcv_char_buffer == 0x38 && eshell_read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F7键
          }
          else if(rcv_char_buffer == 0x39 && eshell_read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){eshell_printf("F8");} // F8键
          }
        }
        else if(rcv_char_buffer == 0x32 && eshell_read_next_char(rcv_char_buffer)){
          if(rcv_char_buffer == 0x7E){} //  Insert键
          else if(rcv_char_buffer == 0x30 && eshell_read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){eshell_printf("F9");} // F9键
          }
          else if(rcv_char_buffer == 0x31 && eshell_read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F10键
          }
          else if(rcv_char_buffer == 0x33 && eshell_read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F11键
          }
          else if(rcv_char_buffer == 0x34 && eshell_read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F12键
          }
        }
        else if(rcv_char_buffer == 0x33 && eshell_read_next_char(rcv_char_buffer)){
          if(rcv_char_buffer == 0x7E){} // DEL键
        }
        else if(rcv_char_buffer == 0x34 && eshell_read_next_char(rcv_char_buffer)){
          if(rcv_char_buffer == 0x7E){} // End键
        }
        else if(rcv_char_buffer == 0x35 && eshell_read_next_char(rcv_char_buffer)){
          if(rcv_char_buffer == 0x7E){} // PgUp键
        }
        else if(rcv_char_buffer == 0x36 && eshell_read_next_char(rcv_char_buffer)){
          if(rcv_char_buffer == 0x7E){} // PgDn键
        }
      }
      return ES_GET_NULL_OR_UMCOMPLETED;
    }
    else if(rcv_char_buffer == 0x1B){

      return ES_GET_NULL_OR_UMCOMPLETED;
    }  // ECS键

    switch(rcv_char_buffer){
      case 0x0C: {  //  Ctrl+C 清屏
        eshell_putchar(rcv_char_buffer); 
        eshell_cmd_input_prompt();
      }break;

      case 0x09:{ // Tab键
      }break;

      case 0x08:  // Backspace键 (ASCII值)
      case 0x7F:  // Backspace键  DEL(ASCII值)
      {
        if (cmd_cursor_count >= 1)  // 光标前有可删减空间
        {
          if((cmd_cursor_count >= 3) && (buf[cmd_cursor_count-3] == 0xE4 || buf[cmd_cursor_count-3] == 0xE5 || buf[cmd_cursor_count-3] == 0xE6 || buf[cmd_cursor_count-3] == 0xE7)){ // 如果是中文字符
            for(int i=0; i<3; i++){
              if(eshell_delete_char_at_cursor(buf, cmd_count, cmd_cursor_count)){
              cmd_count--; // 减少一位命令长度
              cmd_cursor_count--; // 光标前移动一位
              }
            }
            eshell_putchar(rcv_char_buffer); 
            eshell_putchar(rcv_char_buffer); // 在终端屏幕上删除一位中文字符
            // 将删除位置后的字符前移
            for(uint32_t i=cmd_cursor_count; i<cmd_count; i++){ 
              eshell_putchar(buf[i]);
            }
            eshell_printf(" ");
            eshell_printf("\033[%dD",cmd_count - cmd_cursor_count);
          }
          else if(eshell_delete_char_at_cursor(buf, cmd_count, cmd_cursor_count)){
            cmd_count--; // 减少一位命令长度
            cmd_cursor_count--; // 光标前移动一位
            eshell_putchar(rcv_char_buffer); // 在终端屏幕上删除一位字符
            // 将删除位置后的字符前移
            for(uint32_t i=cmd_cursor_count; i<cmd_count; i++){ 
              eshell_putchar(buf[i]);
            }
            eshell_printf(" ");
            eshell_printf("\033[%dD",cmd_count - cmd_cursor_count + 1);
          }
        }
      }break;

      case  0x10:  // 换行符 '\n'
      case  0x0D:  // Enter键 '\r'
      {
        buf[cmd_count] = '\0';  // 末尾添加字符串结束符
        cmd_count = 0;  // 清零计数器
        cmd_cursor_count = 0;
        return ES_GET_COMPLETED;  // 返回有效标志
      }break;

      default: // TODO:中文插入存在一些未知BUG
      {
        // 接收到中文
        if((rcv_char_buffer == 0xE4 || rcv_char_buffer == 0xE5 || rcv_char_buffer == 0xE6 || rcv_char_buffer == 0xE7) && eshell_serial_available()){
          // 将光标位置之后的字符向后移动3位
          for (uint32_t i = cmd_count; i > cmd_cursor_count + 2; i--) {
            buf[i] = buf[i - 3];
          }
          for(uint32_t i = 0; i<3; i++){
            buf[cmd_cursor_count] = rcv_char_buffer;
            eshell_putchar(rcv_char_buffer);
            cmd_count++;
            cmd_cursor_count++;
            if(i<2){rcv_char_buffer = eshell_serial_read();}
          }

          // eshell_putchar(buf[cmd_cursor_count-3]);
          // eshell_putchar(buf[cmd_cursor_count-2]);
          // eshell_putchar(buf[cmd_cursor_count-1]);
          // char send_buf[4]={};
          // send_buf[0] = buf[cmd_cursor_count-3];
          // send_buf[1] = buf[cmd_cursor_count-2];
          // send_buf[2] = buf[cmd_cursor_count-1];
          // send_buf[3] = '\0';
          // eshell_printf("%s",send_buf);
          
          
          // 移动光标至修改的位置
          if(cmd_cursor_count != cmd_count){
            // 终端将后方字符移动
            for(uint32_t i=cmd_cursor_count; i<=cmd_count; i++){ 
              eshell_putchar(buf[i]);
            }
            eshell_printf("\033[%dD",cmd_count - cmd_cursor_count - 2);
          }
          
        }
        else{
          // 将光标位置之后的字符向后移动一位
          for (uint32_t i = cmd_count; i > cmd_cursor_count; i--) {
            buf[i] = buf[i - 1];
          }
          buf[cmd_cursor_count] = rcv_char_buffer;
          //eshell_putchar(rcv_char_buffer);
          // 终端将后方字符移动
          for(uint32_t i=cmd_cursor_count; i<=cmd_count; i++){ 
            eshell_putchar(buf[i]);
          }
          // 移动光标至修改的位置
          if(cmd_cursor_count != cmd_count){
            eshell_printf("\033[%dD",cmd_count - cmd_cursor_count);
          }
          
          cmd_count++;
          cmd_cursor_count++;
        }
      }
    }
  }
  return ES_GET_NULL_OR_UMCOMPLETED;
}

/* 命令全为空格检查
 * @param msg 串口获取的原字符串
 * @return 如果不全为空格则返回1，否则返回0
 */
uint8_t eshell_check_blank(char* msg){
  uint32_t length =  strlen(msg);
  for(uint32_t i=0; i<length;i++){
    if(msg[i] != 0x20){return 1;}
  }
  return 0;
}

// 动态规划实现编辑距离算法
uint32_t eshell_edit_distance(const char* s1, const char* s2) {
    uint32_t len1 = strlen(s1);
    uint32_t len2 = strlen(s2);

    // 创建动态规划表
    uint32_t** dp = (uint32_t**)malloc((len1 + 1) * sizeof(uint32_t*));
    for (uint32_t i = 0; i <= len1; i++) {
        dp[i] = (uint32_t*)malloc((len2 + 1) * sizeof(uint32_t));
    }

    // 初始化边界条件
    for (uint32_t i = 0; i <= len1; i++) {
        dp[i][0] = i; // 将 s1 的前 i 个字符变为空字符串需要 i 次删除
    }
    for (uint32_t j = 0; j <= len2; j++) {
        dp[0][j] = j; // 将空字符串变为 s2 的前 j 个字符需要 j 次插入
    }

    // 填充动态规划表
    for (uint32_t i = 1; i <= len1; i++) {
        for (uint32_t j = 1; j <= len2; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                // 如果字符相同，不需要额外操作
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                // 否则取插入、删除、替换操作中的最小值加 1
                dp[i][j] = 1 + eshell_min(dp[i - 1][j],     // 删除
                                          dp[i][j - 1],     // 插入
                                          dp[i - 1][j - 1]  // 替换
                                          );
            }
        }
    }

    // 获取最终结果
    uint32_t result = dp[len1][len2];

    // 释放动态分配的内存
    for (uint32_t i = 0; i <= len1; i++) {
        free(dp[i]);
    }
    free(dp);

    return result;
}

// 辅助函数：求三个数中的最小值
uint32_t eshell_min(uint32_t a, uint32_t b, uint32_t c) {
    uint32_t min_val = a;
    if (b < min_val) min_val = b;
    if (c < min_val) min_val = c;
    return min_val;
}



/* 命令参数计算及拆分 
 * @param msg 串口获取的原字符串
 * @param delim 分隔字符串的分隔字符，一般为" "单个空格
 * @param get[] 存放拆分后的字符串
 * @param max_num 允许的最大参数数量
 * @return 实际参数数量
 */
uint8_t eshell_get_param(char* msg, const char* delim, char* get[], uint32_t param_max_num)
{
    if (msg == NULL || delim == NULL || get == NULL) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("eshell_get_param()-Invalid input parameters for command parameter parsing!");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("eshell_get_param()-命令参数解析的输入参数无效！");
        #endif
        return 0; // 返回错误码
    }

    if (strlen(msg) == 0) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_warning("eshell_get_param()-Empty input string detected in command parameter parsing.");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_warning("eshell_get_param()-检测到命令参数解析中的空输入字符串。");
        #endif
        return 0; // 返回参数数量为 0
    }

    // 检查输入字符串长度是否超出限制
    if (strlen(msg) >= ESHELL_CMD_BUFFER_MAX_LEN) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("eshell_get_param()-Input command exceeds the maximum allowed length!");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("eshell_get_param()-输入命令超过缓冲区允许的最大长度！");
        #endif
        return 0; // 返回错误码
    }

    // 检查参数数量限制
    if (param_max_num == 0 || param_max_num > ESHELL_CMD_PARAM_MAX_NUM) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("eshell_get_param()-The number of arguments exceeds the allowed maximum limit or the error is set to 0!");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("eshell_get_param()-参数数量超过允许的最大限制或错误设置为0！");
        #endif
        return 0; // 返回错误码
    }

    uint32_t i;
    char *ptr = NULL;
    char *saveptr = NULL; // 用于保存 strtok_r 的上下文

    ptr = strtok_r(msg, delim, &saveptr);
    for (i = 0; ptr != NULL && i < param_max_num; i++) {
        get[i] = ptr;
        ptr = strtok_r(NULL, delim, &saveptr);
    }

    if (ptr != NULL) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_warning("eshell_get_param()-Input string has more parameters than the maximum allowed (%d).", param_max_num);
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_warning("eshell_get_param()-输入字符串的参数数量超过了允许的最大值 (%d)。", param_max_num);
        #endif
    }

    return i; // 返回实际参数数量
}



/* 命令匹配并执行
 * @param argc 参数数量
 * @param argv 参数
 */
void eshell_match(uint32_t argc, char** argv, uint32_t eshell_cmd_list_num, eshell_cmd_list cmd_list[]){
    const char* similar_cmd = NULL;
    uint32_t edit_distance = 0xFFFFFFFF;
    uint32_t temp_edit_distance = 0;
    // 检查输入参数有效性
    if (cmd_list == NULL || argv == NULL) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("eshell_match()-Invalid input parameters for command matching!");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("eshell_match()-命令匹配的输入参数无效！");
        #endif
        return;
    }

    if (eshell_cmd_list_num > ESHELL_DYNAMIC_CMD_COUNT_MAX) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("eshell_match()-Command list size exceeds the maximum allowed limit!");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("eshell_match()-命令列表大小超过允许的最大限制！");
        #endif
        return;
    }

    if (argc > ESHELL_CMD_PARAM_MAX_NUM) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("eshell_match()-Parameter count exceeds the maximum allowed limit!");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("eshell_match()-参数数量超过允许的最大限制！");
        #endif
        return;
    }

    if (argv[0] == NULL) {
        #if ESHELL_LANGUAGE_SET == 0
            eshell_printf_error("eshell_match()-Invalid input command! Command name is NULL.");
        #elif ESHELL_LANGUAGE_SET == 1
            eshell_printf_error("eshell_match()-无效的输入命令！命令名称为空。");
        #endif
        return;
    }
    
    // 遍历命令表进行匹配
    for (uint32_t i = 0; i < eshell_cmd_list_num; i++) {
        if (cmd_list[i].name == NULL) {
            #if ESHELL_LANGUAGE_SET == 0
                eshell_printf_warning("eshell_match()-Command name at index %d is NULL, skipping...", i);
            #elif ESHELL_LANGUAGE_SET == 1
                eshell_printf_warning("eshell_match()-索引 %d 处的命令名称为空，跳过...", i);
            #endif
            continue;
        }

        if (strcmp(argv[0], cmd_list[i].name) == 0) {
            if (cmd_list[i].func == NULL) {
                #if ESHELL_LANGUAGE_SET == 0
                    eshell_printf_warning("eshell_match()-Callback function for command '%s' is NULL, skipping...", cmd_list[i].name);
                #elif ESHELL_LANGUAGE_SET == 1
                    eshell_printf_warning("eshell_match()-命令 '%s' 的回调函数为空，跳过...", cmd_list[i].name);
                #endif
                continue;
            }

            eshell_page_id = ES_PAGE_APP; // 将页面ID设置为应用程序
            cmd_list[i].func(argc, argv); // 调用对应的回调函数
            eshell_page_id = ES_PAGE_MAIN; // 退出应用程序恢复为主界面
            return;
        }
        else{
          // 相似命令匹配
          uint32_t max_allowed_distance = (strlen(argv[0]) + strlen(cmd_list[i].name)) / ESHELL_CMD_EDIT_DISTANCE_MATCHING_DIVISOR; // 动态阈值：允许的最大编辑距离
          temp_edit_distance = eshell_edit_distance(argv[0], cmd_list[i].name); // 计算编辑距离
          // 如果编辑距离更优 且 小于动态阈值 则 记录该命令为更优相似命令
          if(temp_edit_distance <= edit_distance && temp_edit_distance <= max_allowed_distance){
            edit_distance = temp_edit_distance;
            similar_cmd = cmd_list[i].name;
          }
        }
    }


    // 如果存在相似命令
    if(edit_distance > 0 && similar_cmd != NULL){
      #if ESHELL_LANGUAGE_SET == 0
        eshell_printf("'%s' command not found,did you mean: '%s'\r\n", argv[0], similar_cmd);
      #elif ESHELL_LANGUAGE_SET == 1
        eshell_printf("'%s' 命令没有找到，您是否想要输入 '%s'\r\n", argv[0], similar_cmd);
      #endif
    }
    else{
      // 未找到匹配命令
      #if ESHELL_LANGUAGE_SET == 0
        eshell_printf("'%s' command not found.\r\n", argv[0]);
      #elif ESHELL_LANGUAGE_SET == 1
        eshell_printf("'%s' 命令没有找到。\r\n", argv[0]);
      #endif
    }
}



/* shell处理界面 */
void eshell_execute(){
  if(eshell_page_id == ES_PAGE_MAIN && eshell_run_state == ES_RUN_NORMAL){
    uint8_t get_state = eshell_get_cmd(eshell_rcv_buffer, ESHELL_CMD_BUFFER_MAX_LEN);  // 获取命令读取状态
    // 如果命令获取完成则解析命令
    if(get_state == ES_GET_COMPLETED){  
      /* 获得到命令 */
      eshell_printf("\r\n");
      if(strlen(eshell_rcv_buffer) && eshell_check_blank(eshell_rcv_buffer)){/* 判断接受到的指令字符串是否为0 */
        // strncpy(eshell_cmd_historical_record[eshell_cmd_historical_record_count], eshell_rcv_buffer, ESHELL_CMD_PARAM_MAX_NUM); // 拷贝命令至历史记录数组
        // eshell_cmd_historical_record[eshell_cmd_historical_record_count][ESHELL_CMD_PARAM_MAX_NUM] = '\0'; // 确保字符串以 '\0' 结尾
        // eshell_cmd_historical_record_length[eshell_cmd_historical_record_count] = cmd_count; // 将命令的长度写入历史记录数组
        // if(eshell_cmd_historical_record_count < ESHELL_HISTORICAL_RECORD_MAX){ eshell_cmd_historical_record_count++; } // 如果未超过历史记录数量最大限制
        // else if(eshell_cmd_historical_record_count >= ESHELL_HISTORICAL_RECORD_MAX){ eshell_cmd_historical_record_count = 0;} // 如果达到最大限制则指向数组头部
        // eshell_cmd_historical_record_count_cumulant++; // 历史命令累计计数器增加
        // eshell_cmd_historical_record_count_offset = 0; // 清除偏移量

        char *argv[ESHELL_CMD_PARAM_MAX_NUM];
        uint32_t argc = eshell_get_param(eshell_rcv_buffer, " ", argv, ESHELL_CMD_PARAM_MAX_NUM);
        eshell_match(argc, argv, eshell_dynamic_cmd_count, eshell_dynamic_cmd_list);
        
      }
      eshell_cmd_input_prompt();
    }
    // 如果发生命令溢出缓冲区
    else if(get_state == ES_GET_ERROR_BUFFER_REMOVAL){
      memset(eshell_rcv_buffer, 0, ESHELL_CMD_PARAM_MAX_NUM + 1); // 清空缓冲区
      eshell_printf("\r\n");
      #if ESHELL_LANGUAGE_SET == 0
        eshell_printf_error("Input command exceeds the maximum length allowed by the buffer!");
      #elif ESHELL_LANGUAGE_SET == 1
        eshell_printf_error("输入命令超过缓冲区允许的最大长度！");
      #endif
      eshell_cmd_input_prompt();
    }
  }
}


/* 光标右移一单位 */
void eshell_moveCursorRight(char *buf) {

  if(cmd_cursor_count < cmd_count){
    if((cmd_count - cmd_cursor_count >= 3) && (buf[cmd_cursor_count] == 0xE4 || buf[cmd_cursor_count] == 0xE5 || buf[cmd_cursor_count] == 0xE6 || buf[cmd_cursor_count] == 0xE7)){ // 如果是中文字符
      cmd_cursor_count = cmd_cursor_count + 3;
      eshell_printf("\033[2C");
    }
    else{
      cmd_cursor_count++;
      eshell_printf("\033[1C");
    }
  } 
}

/* 光标左移一单位 */
void eshell_moveCursorLeft(char *buf) {
  if(cmd_cursor_count >= 1){
    if((cmd_cursor_count >= 3) && (buf[cmd_cursor_count-3] == 0xE4 || buf[cmd_cursor_count-3] == 0xE5 || buf[cmd_cursor_count-3] == 0xE6 || buf[cmd_cursor_count-3] == 0xE7)){ // 如果是中文字符
      cmd_cursor_count = cmd_cursor_count - 3;
      eshell_printf("\033[2D");
    }
    else{
    cmd_cursor_count--;
    eshell_printf("\033[1D");
    }
  }
}


/* 命令输入提示符 */
void eshell_cmd_input_prompt(){
  eshell_printf("\r->");
}

// /* 显示上一个历史命令
//  */
// void eshell_previous_command(){
//   // 清空当前显示内容
//   eshell_printf("\r");
//   for(int i=0; i<cmd_count; i++){eshell_printf(" ");}
//   eshell_cmd_input_prompt();

//   strncpy(eshell_rcv_buffer, eshell_cmd_historical_record[eshell_cmd_historical_record_count - eshell_cmd_historical_record_count_offset], ESHELL_CMD_PARAM_MAX_NUM); // 拷贝历史命令至命令缓冲区
//   cmd_count = eshell_cmd_historical_record_length[eshell_cmd_historical_record_count - eshell_cmd_historical_record_count_offset];  // 设置命令长度
//   cmd_cursor_count = cmd_count; // 移动光标
//   eshell_printf(eshell_rcv_buffer); // 终端显示

//   if(eshell_cmd_historical_record_count - eshell_cmd_historical_record_count_offset > 0){eshell_cmd_historical_record_count_offset++;} // 上偏移量增加
//   else if( (eshell_cmd_historical_record_count - eshell_cmd_historical_record_count_offset) <= 0 && eshell_cmd_historical_record_count_cumulant > ESHELL_HISTORICAL_RECORD_MAX){  // 循环历史记录显示
//     eshell_cmd_historical_record_count_offset = -(ESHELL_HISTORICAL_RECORD_MAX - 1 - eshell_cmd_historical_record_count);
//     }
// }


// /* 显示下一个历史命令
//  */
// void eshell_next_command(){
//   // 清空当前显示内容
//   eshell_printf("\r");
//   for(int i=0; i<cmd_count; i++){eshell_printf(" ");}
//   eshell_cmd_input_prompt();

//   strncpy(eshell_rcv_buffer, eshell_cmd_historical_record[eshell_cmd_historical_record_count - eshell_cmd_historical_record_count_offset], ESHELL_CMD_PARAM_MAX_NUM); // 拷贝历史命令至命令缓冲区
//   cmd_count = eshell_cmd_historical_record_length[eshell_cmd_historical_record_count - eshell_cmd_historical_record_count_offset];  // 设置命令长度
//   cmd_cursor_count = cmd_count; // 移动光标
//   eshell_printf(eshell_rcv_buffer); // 终端显示

//   if(eshell_cmd_historical_record_count - eshell_cmd_historical_record_count_offset < ESHELL_HISTORICAL_RECORD_MAX-1){eshell_cmd_historical_record_count_offset--;} // 上偏移量增加
//   else if( (eshell_cmd_historical_record_count - eshell_cmd_historical_record_count_offset) >= ESHELL_HISTORICAL_RECORD_MAX-1 && eshell_cmd_historical_record_count_cumulant > ESHELL_HISTORICAL_RECORD_MAX){  // 循环历史记录显示
//     eshell_cmd_historical_record_count_offset =  - eshell_cmd_historical_record_count;
//     }
// }
