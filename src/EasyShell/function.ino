
/* 串口打印函数 */
void eshell_print(char* str, ...)
{
  char buf[CMD_BUF_MAX_LEN] = {0};
  va_list ap;
  va_start(ap, str);
  vsnprintf(buf,CMD_BUF_MAX_LEN,str, ap);
  SERIAL.print(buf);
}

/* 串口自定义文本属性打印函数 */
/* 参数说明
  textattribute 0--重置所有属性 1--高亮/加粗  2--暗淡 4--下划线  5--闪烁 7--反转 8--隐藏
  forecolor 0--默认 30--黑色  31--红色 32--绿色 33--黄色  34--蓝色  35--品红  36--青色
  backgroundcolor 0--默认 40--黑色  41--红色  42--绿色  43--黄色 44--蓝色 45--品红  46--青色
*/
void eshell_print_color(int textattribute,int forecolor,int backgroundcolor,char* str, ...)
{
  char buf[CMD_BUF_MAX_LEN] = {0};
  va_list ap;
  va_start(ap, str);
  vsnprintf(buf,CMD_BUF_MAX_LEN,str, ap);
  SERIAL.printf("\033[0m\033[%d;%d;%dm",textattribute,forecolor,backgroundcolor);
  SERIAL.print(buf);
  SERIAL.print("\033[0m");
}


/* 从串口获取原字符串 */
/* 参数说明
  buf 串口字符串接收缓冲区数组
  maxlen 最大命令缓冲区长度
  返回值 有效标志
*/
uint8_t eshell_get(char *buf, uint8_t maxlen)
{
  char rcv_char;
  static uint8_t count = 0; //命令计数器
  buf[count] = rcv_char; /*记录数据*/
  if (SERIAL.available() && !eshell_state)  //判断串口存在数据且终端状态为0
  {
    rcv_char = SERIAL.read();
    if (count >= maxlen) /*长度超限*/
    {
      buf[0] = '\0'; /*令接收的字符串无效*/
      count = 0; /*清零计数器以便后续使用*/
      eshell_print((char*)"\r\nEasyShell:接收字符串长度不合法！");
      return 1;  /*返回有效标志*/
    }     
    buf[count] = rcv_char; /*记录数据*/
    switch (rcv_char)
    {
      case 0x08:
      case 0x7F: /*退格键或者删除键*/
      {
        if (count > 0)
        {
          count--; /*删除上一个接收到的字符*/
        }
        else if(count == 0)
        {
          eshell_moveCursorRight(1);
        }
      }
      break;
      case '\r':
      case '\n': /*接收到回车换行，命令接收结束*/
      {
        buf[count] = '\0'; /*添加字符串结束符*/
        count = 0;      /*清零计数器*/
        return 1;     /*返回有效标志*/
      }
      break;

      default:
        count++;
    }
    SERIAL.print(rcv_char); /*把收到的字符回显到终端*/
  }
  return 0;
}



/* 命令参数计算及拆分 */
/* 参数说明
  msg 串口获取的原字符串
  delim 分隔字符串的分隔字符，一般为" "单个空格
  get[] 存放拆分后的字符串
  max_num 允许的最大参数数量
  返回值 实际参数数量
*/
static int eshell_get_param(char* msg, char*delim, char* get[], int param_max_num)
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



/* 命令匹配 */
void eshell_match(int argc, char**argv)
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
    eshell_print((char*)"EasyShell:\"%s\" 命令未找到！\r\n",argv[0]);
}



/* 光标右移 */
void eshell_moveCursorRight(int n) {
    eshell_print((char*)"\033[%dC", n);
}

/* 光标左移 */
void eshell_moveCursorLeft(int n) {
    eshell_print((char*)"\033[%dD", n);
}

/* 光标上移 */
void eshell_moveCursorUp(int n) {
    eshell_print((char*)"\033[%dA", n);
}

/* 光标下移 */
void eshell_moveCursorDown(int n) {
    eshell_print((char*)"\033[%dB", n);
}


/* 进度条模块 */
/* 参数说明
  state 0--初始化进度条 1--进度条运行 2--强制中断进度条
  percent 进度条当前位置（范围0-100），不可比上一次小否则会原地不动
  str 提示字符串（当state为2时为抛出错误提示）
*/
void eshell_LoadingBar(int state,int percent,char* str){
    static int state_flag;
    static int percent_tmp;
    if(state == 0 && state_flag){ //  重复初始化抛出异常
      state_flag = 2; //将终端设置为接收模式
      eshell_print((char*)"\r\nEasyShell:抛出异常：非法调用，重复初始化进度条控件！\r\n");
    }
    if(state == 0 && state_flag ==0){ // 初始化
      percent_tmp = 0; //百分比缓存
      state_flag = 1; //状态标记
      eshell_state = 2; //将终端设置为正忙，暂停接收
      eshell_print((char*)"%s [ ",str);
      for(int i=1;i<=50;i++) eshell_print_color(2,0,40,(char*)"-");
      eshell_print((char*)" ]");
      eshell_print((char*)" %2d%%",0);
    }
    if(state == 1 && state_flag == 1){  // 进度条进行中
      if(percent >= percent_tmp &&percent < 100)  //  进度条未满
      {
        percent_tmp = percent;
        eshell_print((char*)"\r%s [ ",str);
        for(int i=1;i<=percent/2;i++) eshell_print_color(1,31,40,(char*)"-");
        for(int i=percent/2+1;i<=50;i++) eshell_print_color(2,0,40,(char*)"-");
        eshell_print((char*)" ]");
        SERIAL.printf((char*)" %2d%%",percent);
      }
      if(percent >= percent_tmp && percent >= 100)  // 进度条满
      {
        percent_tmp = percent;
        eshell_print((char*)"\r%s [ ",str);
        for(int i=1;i<=50;i++) eshell_print_color(1,31,40,(char*)"-");
        eshell_print((char*)" ]");
        eshell_print((char*)" %2d%%\r\n",percent);
        eshell_state = 0;
        percent_tmp = 0;
        state_flag = 0;
      }
    }
    if(state == 2){ // 用户强制终止进度条
      eshell_state = 0; //将终端设置为接收模式
      percent_tmp = 0;
      state_flag = 0;
      eshell_print_color(1,0,41,(char*)"\r\n强制终止！%s",str);
      eshell_print((char*)"\r\n");
    }
    if(state_flag == 2){  // 异常恢复
      eshell_state = 0; //将终端设置为接收模式
      percent_tmp = 0;
      state_flag = 0;
    }
    
}
