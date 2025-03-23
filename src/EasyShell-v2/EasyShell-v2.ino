#include "EasyShell.h"

void cmd_hello(int argc, char**argv){
  eshell_printf("hello world\r\n");
}

/* 用户静态命令表 */
eshell_cmd_list user_static_cmd_list[] = 
{
  {"hello", cmd_hello, "你好世界"},
};
/* 命令数量计算 */
uint32_t eshell_user_static_cmd_list_num = sizeof(user_static_cmd_list)/sizeof(eshell_cmd_list); 




void setup() {
  // put your setup code here, to run once:
  eshell_Init(user_static_cmd_list, eshell_user_static_cmd_list_num);

}

void loop() {
  // put your main code here, to run repeatedly:
  eshell_execute();
}




