#include <Arduino.h>

#include "EasyShell.h"
#include "EasyShell_driver.h"

char eshell_rcv_buffer[ESHELL_CMD_PARAM_MAX_NUM + 1] = {0};


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
  if (eshell_serial_available()){
    rcv_char_buffer = eshell_serial_read();
    if (count >= maxlen) /* 缓冲区溢出 */
    {
      buf[0] = '\0'; /*令接收的字符串无效*/
      count = 0; /*清零计数器以便后续使用*/
      return ES_GET_ERROR_BUFFER_REMOVAL;  /*返回溢出错误标志*/
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

    switch(rcv_char){
      case 0x7F:  // DEL(ASCII值)
      case 0x08:  // Backspace键
      {
        if (count > 0)  // 有输入数据
        {
          count--; // 删除上一个接收到的字符
          eshell_printf(rcv_char); // 在终端屏幕上删除一位字符
        }
      }break;

      case  0x10:  // 换行符 '\n'
      case  0x0D:  // Enter键 '\r'
      {
        buf[count] = '\0';  // 添加字符串结束符
        count = 0;  // 清零计数器*/
        return ES_GET_COMPLETED;  // 返回有效标志
      }break;

      default:
        buf[count] = rcv_char;
        count++;
    }
    eshell_printf(rcv_char);
  }
  return ES_GET_NULL_OR_UMCOMPLETED;
}



