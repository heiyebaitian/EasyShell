typedef void(*eshell_cmd_func_t)(int argc, char**argv);
typedef struct /* 定义命令结构体 */
{
  char* name;
  eshell_cmd_func_t func;
  char* help;
}eshell_cmd_list;

/* 命令表 */
eshell_cmd_list cmd_list[] = 
{
  {(char*)"test", cmd_test, (char*)"显示解析的参数信息"},
  {(char*)"test2", cmd_test2, (char*)"显示参数2的十六进制"},
  {(char*)"info", cmd_info, (char*)"显示Shell信息"},
  {(char*)"colortext", cmd_colortext, (char*)"自定义终端文字输出演示"},
  {(char*)"loading", cmd_loading, (char*)"进度条示例"},
  {(char*)"reboot", cmd_reboot, (char*)"重启单片机"},
  {(char*)"help", cmd_help, (char*)"显示帮助菜单"}
};
/* 命令数量计算 */
int eshell_cmd_list_num = sizeof(cmd_list)/sizeof(eshell_cmd_list); 




/* 命令程序跳转区 */


void cmd_test(int argc, char**argv)
{
  int i = 0;
  eshell_print((char*)"argc=%d ", argc);
  eshell_print((char*)"argv=[");
  for(i=0; i<(argc-1); i++)
  {
    eshell_print((char*)"%s,", argv[i]);
  }
  eshell_print((char*)"%s]\r\n", argv[i]);
}

void cmd_test2(int argc, char**argv)
{
  eshell_print((char*)"argc=%d ", argc);
  eshell_print((char*)"argv[1]=[");
  eshell_print((char*)"%x", *argv[1]);
  eshell_print((char*)"]\r\n");
}


void cmd_help(int argc, char**argv)
{
  eshell_print((char*)"\r\n-----SHELL COMMANDS-----\r\n");
  for(int i=0; i<eshell_cmd_list_num; i++)
  {
    eshell_print((char*)" %-12s  --%s\r\n", cmd_list[i].name, cmd_list[i].help);
  }
  eshell_print((char*)"\r\n");
}

void cmd_info(int argc, char**argv)
{
  eshell_print((char*)"EasyShell版本:");
  eshell_print((char*)EASYSHELL_VER);
  eshell_print((char*)"\r\n");
}

void cmd_reboot(int argc, char**argv)
{
  eshell_print((char*)"\r\n\n\nReboot...\r\n\n\n");
  eshell_initializer();

}

void cmd_colortext(int argc, char**argv)
{
  eshell_print((char*)"这个程序演示了EasyShell支持的文本属性\r\n\r\n");
  for(int i=0;i<=8;i++)
  {
    eshell_print((char*)"textattribute属性:%d   ",i);
    eshell_print_color(i,34,40,(char*)"我是演示文字Hello World!");
    eshell_print((char*)"\r\n");
  }
  eshell_print((char*)"\r\n");
  for(int i=30;i<=36;i++)
  {
    eshell_print((char*)"forecolor属性:%d   ",i);
    eshell_print_color(1,i,40,(char*)"我是演示文字Hello World!");
    eshell_print((char*)"\r\n");
  }
  eshell_print((char*)"\r\n");
  for(int i=40;i<=46;i++)
  {
    eshell_print((char*)"backgroundcolor属性:%d   ",i);
    eshell_print_color(1,0,i,(char*)"我是演示文字Hello World!");
    eshell_print((char*)"\r\n");
  }
  eshell_print((char*)"\r\n");
}

void cmd_loading(int argc, char**argv){
  if (strcmp(argv[1], "-p") == 0 && strcmp(argv[2], "0") == 0){
    eshell_LoadingBar(0,0,(char*)"下载中");  // 初始化进度条
    for(int i=0;i<=100;i++){
     delay(10);
     eshell_LoadingBar(1,i,(char*)"下载中");
    }
  }
  if (strcmp(argv[1], "-p") == 0 && strcmp(argv[2], "1") == 0){
    eshell_LoadingBar(0,0,(char*)"下载中");
    for(int i=0;i<=80;i++){
     delay(10);
     eshell_LoadingBar(1,i,(char*)"下载中");
    }
    eshell_LoadingBar(2,81,(char*)"因为异常触发测试！"); // 强制中断进度条
  }
  if (strcmp(argv[1], "-p") == 0 && strcmp(argv[2], "2") == 0){
    eshell_LoadingBar(0,0,(char*)"下载中");
    for(int i=0;i<=80;i++){
     delay(10);
     eshell_LoadingBar(1,i,(char*)"下载中");
    }
    eshell_LoadingBar(0,81,(char*)"下载中");
  }
  if (strcmp(argv[1], "-p") == 0 && strcmp(argv[2], "3") == 0){
    for(int j=1;j<=10;j++)
    {
      eshell_LoadingBar(0,0,(char*)"下载中");
      for(int i=0;i<=100;i++){
        delay(10);
        eshell_LoadingBar(1,i,(char*)"下载中");
      }
    }
  }
  if (strcmp(argv[1], "-h") == 0 || argc == 1){
    eshell_print((char*)"loading 进度条示例帮助菜单\r\n");
    eshell_print((char*)" -p  选择模式参数 0--正常进度条 1--自终断进度条 2--重复初始化的进度条 3--十个正常进度条\r\n");
    eshell_print((char*)" -h  获取帮助信息\r\n");
  }
}
