#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


#define CMD_BUF_MAX_LEN 128 //最大命令缓冲区长度
#define CMD_PARAM_MAX_NUM 32 //最大命令参数数量
#define SERIAL Serial //选择用于EasyShell的串口
#define EASYSHELL_VER "0.1.3" //选择用于EasyShell的串口

char eshll_cmd_buf[CMD_BUF_MAX_LEN+1] = {0};
extern int eshell_state;
int eshell_state = 0; // 0--接收模式  1--拒绝接收模式  2--终端正忙




void setup() {
  // put your setup code here, to run once:
  eshell_initializer();
}

void loop() {
  // put your main code here, to run repeatedly:
  eshell_execute();
}
