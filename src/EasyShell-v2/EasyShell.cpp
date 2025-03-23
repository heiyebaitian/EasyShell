#include "EasyShell.h"


/* 光标处删除字符函数
 * 此函数将自动完成于光标位置删除光标前一位字符并自动前移后方字符
 * 注意:此函数默认传入字符个数不含'\0'，请根据实际情况调整传入length
 * @param str 待操作数组
 * @param length 数组数据长度
 * @param cursor 光标位置
 * @return 状态标志
 */
uint8_t delete_char_at_cursor(char *str, int length, int cursor) {
    // 检查输入参数是否合法
    if (str == NULL || length <= 0 || cursor < 1 || cursor > length) {
        return 0; // 不合法
    }

    // 从光标位置开始，将后续字符向前移动一位
    for (int i = cursor - 1; i < length - 1; i++) {
        str[i] = str[i + 1];
    }
    return 1; // 完成
}



/* 从串口获取下一位字符(用于特殊键判断)
 * @param buffer 接收变量
 * @return 状态标志
 */
bool read_next_char(char &buffer) {
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
uint8_t eshell_get(char *buf, uint8_t maxlen){
  char rcv_char_buffer = 0; // 接收字符缓存
  static uint8_t count = 0; // 命令长度计数器
  static uint8_t cursor_count = 0; // 光标位置计数器
  if (eshell_serial_available()){
    rcv_char_buffer = eshell_serial_read();
    if (count >= maxlen) /* 缓冲区溢出 */
    {
      buf[0] = '\0'; // 令接收的字符串无效
      count = 0; // 清零计数器以便后续使用
      cursor_count = 0;
      return ES_GET_ERROR_BUFFER_REMOVAL;  // 返回溢出错误标志
    }


    if(rcv_char_buffer == 0x1B && read_next_char(rcv_char_buffer)){  // 特殊按键1B类
      if(rcv_char_buffer == 0x5B && read_next_char(rcv_char_buffer)){ // 特殊按键1B 5B类

        if(rcv_char_buffer == 0x41){} // 方向键上
        else if(rcv_char_buffer == 0x42){}  // 方向键下
        else if(rcv_char_buffer == 0x43){}  // 方向键右
        else if(rcv_char_buffer == 0x44){}  // 方向键左
        else if(rcv_char_buffer == 0x31 && read_next_char(rcv_char_buffer)){
          if(rcv_char_buffer == 0x7E){} // Home键
          else if(rcv_char_buffer == 0x31 && read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F1键
          }
          else if(rcv_char_buffer == 0x32 && read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F2键
          }
          else if(rcv_char_buffer == 0x33 && read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F3键
          }
          else if(rcv_char_buffer == 0x34 && read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F4键
          }
          else if(rcv_char_buffer == 0x35 && read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F5键
          }
          else if(rcv_char_buffer == 0x37 && read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F6键
          }
          else if(rcv_char_buffer == 0x38 && read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F7键
          }
          else if(rcv_char_buffer == 0x39 && read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){eshell_printf("F8");} // F8键
          }
        }
        else if(rcv_char_buffer == 0x32 && read_next_char(rcv_char_buffer)){
          if(rcv_char_buffer == 0x7E){} //  Insert键
          else if(rcv_char_buffer == 0x30 && read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){eshell_printf("F9");} // F9键
          }
          else if(rcv_char_buffer == 0x31 && read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F10键
          }
          else if(rcv_char_buffer == 0x33 && read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F11键
          }
          else if(rcv_char_buffer == 0x34 && read_next_char(rcv_char_buffer)){
            if(rcv_char_buffer == 0x7E){} // F12键
          }
        }
        else if(rcv_char_buffer == 0x33 && read_next_char(rcv_char_buffer)){
          if(rcv_char_buffer == 0x7E){} // DEL键
        }
        else if(rcv_char_buffer == 0x34 && read_next_char(rcv_char_buffer)){
          if(rcv_char_buffer == 0x7E){} // End键
        }
        else if(rcv_char_buffer == 0x35 && read_next_char(rcv_char_buffer)){
          if(rcv_char_buffer == 0x7E){} // PgUp键
        }
        else if(rcv_char_buffer == 0x36 && read_next_char(rcv_char_buffer)){
          if(rcv_char_buffer == 0x7E){} // PgDn键
        }
      }
      return ES_GET_NULL_OR_UMCOMPLETED;
    }
    else if(rcv_char_buffer == 0x1B){

      return ES_GET_NULL_OR_UMCOMPLETED;
    }  // ECS键

    switch(rcv_char_buffer){
      case 0x08:  // Backspace键 (ASCII值)
      case 0x7F:  // Backspace键  DEL(ASCII值)
      {
        if (cursor_count >= 1)  // 光标前有可删减空间
        {
          if((cursor_count >= 3) && (buf[cursor_count-3] == 0xE4 || buf[cursor_count-3] == 0xE5 || buf[cursor_count-3] == 0xE6 || buf[cursor_count-3] == 0xE7)){ // 如果是中文字符
            for(int i=0; i<3; i++){
              if(delete_char_at_cursor(buf, count, cursor_count)){
              count--; // 减少一位命令长度
              cursor_count--; // 光标前移动一位
              }
            }
            eshell_putchar(rcv_char_buffer); 
            eshell_putchar(rcv_char_buffer); // 在终端屏幕上删除一位中文字符
          }
          else if(delete_char_at_cursor(buf, count, cursor_count)){
            count--; // 减少一位命令长度
            cursor_count--; // 光标前移动一位
            eshell_putchar(rcv_char_buffer); // 在终端屏幕上删除一位字符
          }
        }
      }break;

      case  0x10:  // 换行符 '\n'
      case  0x0D:  // Enter键 '\r'
      {
        buf[count] = '\0';  // 末尾添加字符串结束符
        count = 0;  // 清零计数器
        cursor_count = 0;
        return ES_GET_COMPLETED;  // 返回有效标志
      }break;

      default:
      {
        buf[cursor_count] = rcv_char_buffer;
        count++;
        cursor_count++;
        eshell_putchar(rcv_char_buffer);
      }
    }
  }
  return ES_GET_NULL_OR_UMCOMPLETED;
}



/* 命令参数计算及拆分 
 * @param msg 串口获取的原字符串
 * @param delim 分隔字符串的分隔字符，一般为" "单个空格
 * @param get[] 存放拆分后的字符串
 * @param max_num 允许的最大参数数量
 * @return 实际参数数量
 */
uint8_t eshell_get_param(char* msg, char*delim, char* get[], int param_max_num)
{
  int i,ret;
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
void eshell_match(int argc, char**argv, uint32_t eshell_cmd_list_num, eshell_cmd_list cmd_list[])
{
  int i = 0;
  for(int i=0; i<eshell_cmd_list_num; i++)
  {
    if(strcmp(argv[0], cmd_list[i].name) == 0)
    {
      cmd_list[i].func(argc, argv);
      return;
    }
  }
    eshell_printf("EasyShell:\"%s\" 命令未找到！\r\n",argv[0]);
}



/* shell处理界面
 */
void eshell_execute(){
  if(eshell_get(eshell_rcv_buffer, ESHELL_CMD_BUFFER_MAX_LEN))
  {
    eshell_printf("\r\n");
    /* 获得到指令 */
    if(strlen(eshell_rcv_buffer)) /* 判断接受到的指令字符串是否为0 */
    {
      char *argv[ESHELL_CMD_PARAM_MAX_NUM];
      int argc = eshell_get_param(eshell_rcv_buffer, " ", argv, ESHELL_CMD_PARAM_MAX_NUM);
      eshell_match(argc, argv, eshell_dynamic_cmd_count, eshell_dynamic_cmd_list);
      
    }
    eshell_printf("->");
  }
}

