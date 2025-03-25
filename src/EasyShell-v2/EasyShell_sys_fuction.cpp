#include "EasyShell_sys_fuction.h"

/* 串口自定义文本属性打印函数
 * @param textattribute 0--重置所有属性 1--高亮/加粗  2--暗淡 4--下划线  5--闪烁 7--反转 8--隐藏
 * @param forecolor 0--默认 30--黑色  31--红色 32--绿色 33--黄色  34--蓝色  35--品红  36--青色
 * @param backgroundcolor 0--默认 40--黑色  41--红色  42--绿色  43--黄色 44--蓝色 45--品红  46--青色
*/
void eshell_printf_custom(int textattribute,int forecolor,int backgroundcolor,const char *str, ...){
  char buffer[ESHELL_PRINT_BUFFER_MAX_LEN];
  va_list args;
  va_start(args, str);

  // 格式化字符串到缓冲区
  vsnprintf(buffer, sizeof(buffer), str, args);

  // 设置文本属性、前景色和背景色
  eshell_printf("\033[0m\033[%d;%d;%dm", textattribute, forecolor, backgroundcolor);

  // 输出缓冲区内容
  eshell_printf("%s", buffer);

  // 重置样式
  eshell_printf("\033[0m");

  va_end(args);
}

/* 进度条模块
 * @param mode ES_LOADING_BAR_INIT--初始化进度条 ES_LOADING_BAR_RUN--进度条运行 ES_LOADING_BAR_KILL--强制中断进度条
 * @param percent 进度条当前位置（范围0-100），不可比上一次小否则会原地不动
 * @param str 提示字符串（当mode为2时为抛出错误提示）
 * @return 函数状态
*/
uint8_t eshell_LoadingBar(int mode,int percent,const char *str){
    static int state_flag;
    static int percent_tmp;
    if(mode == 0 && state_flag){ //  重复初始化抛出异常
      state_flag = 2; //将终端设置为接收模式
      eshell_printf("\r\nEasyShell:抛出异常：非法调用，重复初始化进度条控件！\r\n");
      return ES_LOADING_BAR_STATE_ERROR;
    }
    if(mode == ES_LOADING_BAR_INIT && state_flag ==0){ // 初始化
      percent_tmp = 0; //百分比缓存
      state_flag = 1; //状态标记
      eshell_printf("%s [ ",str);
      for(int i=1;i<=50;i++) eshell_printf_custom(2,0,40,"-");
      eshell_printf(" ]");
      eshell_printf(" %2d%%",0);
      return ES_LOADING_BAR_STATE_INIT;
    }
    if(mode == ES_LOADING_BAR_RUN && state_flag == 1){  // 进度条进行中
      if(percent >= percent_tmp &&percent < 100)  //  进度条未满
      {
        percent_tmp = percent;
        eshell_printf("\r%s [ ",str);
        for(int i=1;i<=percent/2;i++) eshell_printf_custom(1,31,40,"-");
        for(int i=percent/2+1;i<=50;i++) eshell_printf_custom(2,0,40,"-");
        eshell_printf(" ]");
        eshell_printf(" %2d%%",percent);
        return ES_LOADING_BAR_STATE_RUN;
      }
      if(percent >= percent_tmp && percent >= 100)  // 进度条满
      {
        percent_tmp = percent;
        eshell_printf("\r%s [ ",str);
        for(int i=1;i<=50;i++) eshell_printf_custom(1,31,40,"-");
        eshell_printf(" ]");
        eshell_printf(" %2d%%\r\n",percent);
        percent_tmp = 0;
        state_flag = 0;
        return ES_LOADING_BAR_STATE_QUIT;
      }
    }
    if(mode == ES_LOADING_BAR_KILL){ // 用户强制终止进度条
      percent_tmp = 0;
      state_flag = 0;
      eshell_printf_custom(1,0,41,"\r\n强制终止！%s",str);
      eshell_printf("\r\n");
      return ES_LOADING_BAR_STATE_ERROR;
    }
    if(state_flag == 2){  // 异常恢复
      percent_tmp = 0;
      state_flag = 0;
    }
    return ES_LOADING_BAR_STATE_ERROR;
}