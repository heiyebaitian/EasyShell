#include <Arduino.h>
#include "EasyShell_driver.h"

/* 串口打印函数
 * @param format 输出字符串
 */
uint32_t eshell_printf(const char *format, ...) {
    va_list args; // 可变参数列表
    va_start(args, format); // 初始化可变参数列表
    uint32_t result = ESHELL_SERIAL.printf(format, args);
    va_end(args); // 清理可变参数列表
    return result; // 返回实际输出的字符数
}



/* 串口缓冲区有无数据检查
 * @return state 状态结果
 */
uint8_t eshell_serial_available(){
  return ESHELL_SERIAL.available();
}


/* 串口读取函数
 * 从串口读取一个字符数据
 * @return char 
 */
uint32_t eshell_serial_read(){
  return ESHELL_SERIAL.read();
}


