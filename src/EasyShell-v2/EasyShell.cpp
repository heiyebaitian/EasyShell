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


/* 命令参数计算及拆分 
 * @param msg 串口获取的原字符串
 * @param delim 分隔字符串的分隔字符，一般为" "单个空格
 * @param get[] 存放拆分后的字符串
 * @param max_num 允许的最大参数数量
 * @return 实际参数数量
 */
uint8_t eshell_get_param(char* msg, const char* delim, char* get[], uint32_t param_max_num)
{
  uint32_t i,ret;
  char *ptr = NULL;
  ptr = strtok(msg, delim);
  for(i=0; ptr!=NULL &&i<param_max_num; i++)
  {
    get[i] = ptr;
    ptr = strtok(NULL, delim);
  }
  ret = i;
  return ret;
}



/* 命令匹配并执行
 * @param argc 参数数量
 * @param argv 参数
 */
void eshell_match(uint32_t argc, char**argv, uint32_t eshell_cmd_list_num, eshell_cmd_list cmd_list[])
{
  uint32_t i = 0;
  for(i=0; i<eshell_cmd_list_num; i++)
  {
    // 命令匹配
    if(strcmp(argv[0], cmd_list[i].name) == 0)
    {
      eshell_page_id = ES_PAGE_APP; // 将页面ID设置为应用程序
      cmd_list[i].func(argc, argv); // 跳转到对应的回调函数
      eshell_page_id = ES_PAGE_MAIN;  // 退出应用程序恢复为主界面
      return;
    }
  }
  #if ESHELL_LANGUAGE_SET == 0
    eshell_printf("\"%s\" command not found.\r\n",argv[0]);
  #elif ESHELL_LANGUAGE_SET == 1
    eshell_printf("\"%s\" 不是一个有效的命令。\r\n",argv[0]);
  #endif
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
