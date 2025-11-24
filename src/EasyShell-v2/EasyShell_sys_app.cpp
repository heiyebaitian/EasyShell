#include "EasyShell_sys_app.h"

/* 系统命令静态表 
 * 用于维护基础的系统命令
 */
eshell_cmd_list eshell_sys_static_cmd_list[] = 
{
  {"eshell", cmd_eshell, "EasyShell服务程序"},
  {"help", cmd_help, "显示帮助菜单"}
};
/* 命令数量计算 */
uint32_t eshell_sys_static_cmd_list_num = sizeof(eshell_sys_static_cmd_list)/sizeof(eshell_cmd_list); 






/* 帮助菜单（全表打印） */
void cmd_help(int argc, char**argv){
  eshell_printf((char*)"\r\n-----SHELL COMMANDS-----\r\n");
  for(int i=0; i<eshell_dynamic_cmd_count; i++)
  {
    if(eshell_dynamic_cmd_list[i].hide == false){
      eshell_printf(" %-12s  --%s\r\n", eshell_dynamic_cmd_list[i].name, eshell_dynamic_cmd_list[i].help);
    }
  }
  eshell_printf("\r\n");
}

/* EasyShell工具箱 */
void cmd_eshell(int argc, char**argv){
  bool valid_param_flag = true;
  if(argc == 1){
    #if ESHELL_LANGUAGE_SET == 0
      eshell_printf("eshell:Missing valid parameters.\r\n");
    #elif ESHELL_LANGUAGE_SET == 1
      eshell_printf("eshell:缺少有效参数.\r\n");
    #endif
    valid_param_flag = false; // 参数无效
  }
  else for(int i = 1; i<argc; i++){
    if(strcmp(argv[i], "-d") == 0 || strcmp(argv[1], "--delete") == 0){
      if(argc > i+1){
        char getchar_buffer = 0;
        #if ESHELL_LANGUAGE_SET == 0
          eshell_printf("eshell:Are you sure you want to delete the command '%s'? [y/n]:",argv[i+1]);
        #elif ESHELL_LANGUAGE_SET == 1
          eshell_printf("eshell:你确定要删除命令 '%s' 吗[y/n]:",argv[i+1]);
        #endif
        getchar_buffer = eshell_getchar();
        if(getchar_buffer == 0x79 || getchar_buffer == 0x59){
          if(eshell_remove_cmd(argv[i+1]) == 0){
            #if ESHELL_LANGUAGE_SET == 0
              eshell_printf("eshell:The command '%s' has been successfully removed from the dynamic command list.\r\n",argv[i+1]);
            #elif ESHELL_LANGUAGE_SET == 1
              eshell_printf("eshell:命令 '%s' 已成功从动态命令列表中删除。\r\n",argv[i+1]);
            #endif
          }
        }
        i++;
      }
      else{
        #if ESHELL_LANGUAGE_SET == 0
          eshell_printf("eshell:Missing 'COMMAND' parameter.\r\n");
        #elif ESHELL_LANGUAGE_SET == 1
          eshell_printf("eshell:缺少' COMMAND' 参数.\r\n");
        #endif
      }
    }
    
    else if(strcmp(argv[i], "-v") == 0 || strcmp(argv[1], "--version") == 0){
      eshell_printf("\r\n-----Version-----\r\n");
      eshell_printf("EasyShell %s (%s)\r\n", ESHELL_VERSION, __DATE__);
      eshell_printf("作者:参宿四\r\n");
      eshell_printf("Github项目地址:https://github.com/heiyebaitian/EasyShell\r\n");
      eshell_printf("© 2025 参宿四的小木屋。保留所有权利。\r\n");
      eshell_printf("-----------------\r\n");
    }

    else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){valid_param_flag = false;}

    else{
    eshell_printf("eshell:\"%s\" is not a valid parameters.\r\n", argv[i]);
    valid_param_flag = false; // 参数无效
    }
  }
  if(valid_param_flag == false){
    #if ESHELL_LANGUAGE_SET == 0
      eshell_printf(" %-25s  %s","-v, --version","Display EasyShell version.\r\n");
      eshell_printf(" %-25s  %s", "-d, --delete 'COMMAND'", "Delete the specified command from the dynamic command table.\r\n");
      eshell_printf(" %-25s  %s","-h, --help","Display this help.\r\n");
    #elif ESHELL_LANGUAGE_SET == 1
      eshell_printf(" %-25s  %s", "-v, --version", "显示EasyShell版本。\r\n");
      eshell_printf(" %-25s  %s", "-d, --delete 'COMMAND'", "从动态命令表中删除指定的命令。\r\n");
      eshell_printf(" %-25s  %s", "-h, --help", "显示此帮助信息。\r\n");
    #endif
  }
  return;
}