#include "EasyShell_sys_app.h"

/* 系统命令静态表 
 * 用于维护基础的系统命令
 */
eshell_cmd_list eshell_sys_static_cmd_list[] = 
{
  {"help", cmd_help, "显示帮助菜单"}
};
/* 命令数量计算 */
uint32_t eshell_sys_static_cmd_list_num = sizeof(eshell_sys_static_cmd_list)/sizeof(eshell_cmd_list); 






/* 帮助菜单（全表打印） */
void cmd_help(int argc, char**argv)
{
  eshell_printf((char*)"\r\n-----SHELL COMMANDS-----\r\n");
  for(int i=0; i<eshell_dynamic_cmd_count; i++)
  {
    eshell_printf(" %-12s  --%s\r\n", eshell_dynamic_cmd_list[i].name, eshell_dynamic_cmd_list[i].help);
  }
  eshell_printf("\r\n");
}