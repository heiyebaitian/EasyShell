#include "EasyShell.h"

/* 文本输出测试 */
void cmd_hello(int argc, char**argv){
  eshell_printf("hello world\r\n");
  eshell_printf_custom(0,31,40,"hello world %d\r\n",2025);
  eshell_add_cmd("hello2025", cmd_hello, "hello2025", false);
}

/* 彩色文本输出测试 */
void cmd_colortext(int argc, char**argv)
{
  eshell_printf("这个程序演示了EasyShell支持的文本属性\r\n\r\n");
  for(int i=0;i<=8;i++)
  {
    eshell_printf("textattribute属性:%d   ",i);
    eshell_printf_custom(i,34,40,"我是演示文字Hello World!");
    eshell_printf("\r\n");
  }
  eshell_printf("\r\n");
  for(int i=30;i<=36;i++)
  {
    eshell_printf("forecolor属性:%d   ",i);
    eshell_printf_custom(1,i,40,"我是演示文字Hello World!");
    eshell_printf("\r\n");
  }
  eshell_printf("\r\n");
  for(int i=40;i<=46;i++)
  {
    eshell_printf("backgroundcolor属性:%d   ",i);
    eshell_printf_custom(1,0,i,"我是演示文字Hello World!");
    eshell_printf("\r\n");
  }
  eshell_printf("\r\n");
}

/* 进度条测试 */
void cmd_loading(int argc, char**argv){
  if (argc == 1 || strcmp(argv[1], "-h") == 0){
    eshell_printf("loading 进度条示例帮助菜单\r\n");
    eshell_printf(" -p  选择模式参数 0--正常进度条 1--自终断进度条 2--重复初始化的进度条 3--十个正常进度条\r\n");
    eshell_printf(" -h  获取帮助信息\r\n");
  }
  if (strcmp(argv[1], "-p") == 0 && strcmp(argv[2], "0") == 0){
    eshell_LoadingBar(0,0,"下载中");  // 初始化进度条
    for(int i=0;i<=100;i++){
     delay(10);
     eshell_LoadingBar(1,i,"下载中");
    }
  }
  if (strcmp(argv[1], "-p") == 0 && strcmp(argv[2], "1") == 0){
    eshell_LoadingBar(0,0,"下载中");
    for(int i=0;i<=80;i++){
     delay(10);
     eshell_LoadingBar(1,i,"下载中");
    }
    eshell_LoadingBar(2,81,"因为异常触发测试！"); // 强制中断进度条
  }
  if (strcmp(argv[1], "-p") == 0 && strcmp(argv[2], "2") == 0){
    eshell_LoadingBar(0,0,"下载中");
    for(int i=0;i<=80;i++){
     delay(10);
     eshell_LoadingBar(1,i,"下载中");
    }
    eshell_LoadingBar(0,81,"下载中");
  }
  if (strcmp(argv[1], "-p") == 0 && strcmp(argv[2], "3") == 0){
    for(int j=1;j<=10;j++)
    {
      eshell_LoadingBar(0,0,"下载中");
      for(int i=0;i<=100;i++){
        delay(10);
        eshell_LoadingBar(1,i,"下载中");
      }
    }
  }
}

/* 参数回显测试 */
void cmd_test(int argc, char**argv)
{
  int i = 0;
  eshell_printf("argc=%d ", argc);
  eshell_printf("argv=[");
  for(i=0; i<(argc-1); i++)
  {
    eshell_printf("%s,", argv[i]);
  }
  eshell_printf("%s]\r\n", argv[i]);
}

/* 用户静态命令表 */
eshell_cmd_list user_static_cmd_list[] = 
{
  {"hello", cmd_hello, "你好世界"},
  {"test", cmd_test, "显示解析的参数信息"},
  {"loading", cmd_loading, "进度条示例"},
  {"colortext", cmd_colortext, "自定义终端文字输出演示"}
};
/* 命令数量计算 */
uint32_t eshell_user_static_cmd_list_num = sizeof(user_static_cmd_list)/sizeof(eshell_cmd_list); 




void setup() {
  // put your setup code here, to run once:
  delay(2000);
  eshell_Init(user_static_cmd_list, eshell_user_static_cmd_list_num);

}

void loop() {
  // put your main code here, to run repeatedly:
  eshell_execute();
}




