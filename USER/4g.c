#include "4g.h"
#include "string.h"
#include "stdio.h"

struct AT at;
struct AT_REC_FLAG at_rec_flag;
struct YF4G_SEND_DATA yf4g_send_data;
char yf4g_rec[300];//接收数据标志
static u8 uart2_offset = 0; //uart2接收字符串偏移量
u8 yf4g_init_done; //4g模块初始化完成标志
char *dev_id = "hue_wellcover1"; //井盖编号
u8 getdata_flag = 0;    //上位机查询标志位

void yf4g_init(void)
{
  
  GPIO_Init(GPIOF, RSTIO, GPIO_Mode_Out_PP_Low_Fast);
  
  //memset(&at, 0, sizeof(struct AT));
  memset(&at_rec_flag, 0, sizeof(struct AT_REC_FLAG));
  
  at.at = "AT";
  at.cpin = "AT+CPIN?";
  at.csq = "AT+CSQ";
  at.cgreg = "AT+CGREG?";
  at.xiic0 = "AT+XIIC=1";
  at.xiic1 = "AT+XIIC?";
  /*at.gpspwr = "AT$MYGPSPWR=1";
  at.gpssta = "AT$MYGPSSTATE";
  at.gpspos = "AT$MYGPSPOS=0,1";
  at.mqttjq = "AT+CLOUDHDAUTH=a16JA7WMR2L,test1,5b51acf43198b36706f9b3b1eee590b5"; */
  sprintf(at.mqttset,"AT+MQTTCONNPARAM=\"%s\",\"\",\"\"",dev_id);
  at.mqttcon = "AT+MQTTCONN=\"47.98.108.7:1883\",0,60";
  at.mqttsub = "AT+MQTTSUB=\"data\",0";
  at.mqttpub = "AT+MQTTPUB";
  at.mqttdiscon = "AT+MQTTDISCONN";
    
/*  at.mqttjqrec = "+CLOUDHDAUTH";  
  at.mqttpubrec = "+CLOUDPUBACK";
  at.mqttsubrec = "+CLOUDSUBACK";*/
  at.mqttsetrec = "AT+MQTTCONNPARAM";
  at.mqttsetconrec = "AT+MQTTCONN";
  at.mqttsetsubrec = "+MQTTSUB";
  
   
  
  
}

void send_yf4g(void)
{ 
  
  if(yf4g_init_done == 0)
  {
    //确认是否开机
    if(at_rec_flag.at == 0)
      send_4gdata(at.at);
    
    //确定卡是否插好
    if(at_rec_flag.at == 1 && at_rec_flag.cpin == 0)
      send_4gdata(at.cpin);
    
    //检测信号质量
    if(at_rec_flag.cpin == 1 && at_rec_flag.csq == 0)
      send_4gdata(at.csq);
    
    //查询GPRS网络注册状态
    if(at_rec_flag.csq == 1 && at_rec_flag.cgreg == 0)
      send_4gdata(at.cgreg);
    
    //连接网络
    if(at_rec_flag.cgreg == 1 && at_rec_flag.xiic0 == 0)
      send_4gdata(at.xiic0);  
    
    //查询是否分配到IP，只有分配到IP后才能上网
    if(at_rec_flag.xiic0 == 1 && at_rec_flag.xiic1 == 0)
      send_4gdata(at.xiic1);
    
   /* //打开gps
    if(at_rec_flag.xiic1 == 1 && at_rec_flag.gpspwr == 0)
      send_4gdata(at.gpspwr);
    
    //进行鉴权,鉴权成功后才能进行订阅和发布  
    if(at_rec_flag.xiic1 == 1 && at_rec_flag.mqttjq == 0 && at_rec_flag.gpspwr == 1)
      send_4gdata(at.mqttjq);*/
    
    //设置MQTT连接参数
    if(at_rec_flag.xiic1 == 1 && at_rec_flag.mqttset == 0)
      send_4gdata(at.mqttset);
    
      //设置MQTT连接ip：port
    if(at_rec_flag.mqttset == 1 && at_rec_flag.mqttcon == 0)
      send_4gdata(at.mqttcon);
    
    //订阅MQTT
    if(at_rec_flag.mqttcon == 1 && at_rec_flag.mqttsub == 0)
    {
      send_4gdata(at.mqttsub);
    }
  }
   delay_ms(200);
   
   manage_rec_at(yf4g_rec, which_rec_at(yf4g_rec));
   memset(yf4g_rec, 0, sizeof(yf4g_rec));
   uart2_offset = 0;
}

void pub_data(char *data, u8 flag)
{
  if(getdata_flag)
  {
    send_4gdata(data);
    getdata_flag = 0;
  }
}

void send_4gdata(char *fun)
{
   char *s = fun;
   
   while(*s)
   {   
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
      USART_SendData8(USART2,*s);
      
    s++;
   }
   
   while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
      USART_SendData8(USART2,0x0d);
}


u8 which_rec_at(char *rec_str)
{
  char *s = rec_str;

  if(strstr(s, at.at) != NULL && at_rec_flag.at == 0)
      return AT_AT;
  
  if(strstr(s, at.cpin) != NULL)
      return AT_CPIN;
  
  if(strstr(s, at.csq) != NULL)
      return AT_CSQ;
  
  if(strstr(s, at.cgreg) != NULL)
      return AT_CGREG;
  
  if(strstr(s, at.xiic0) != NULL)
      return AT_XIIC0;
  
  if(strstr(s, at.xiic1) != NULL)
      return AT_XIIC1;
  
/*  if(strstr(s, at.gpspwr) != NULL)
      return AT_GPSPWR;
  
  if(strstr(s, at.gpssta) != NULL)
      return AT_GPSSTA;
  
  if(strstr(s, at.gpspos) != NULL)
      return AT_POS;
  
  if(strstr(s, at.mqttjqrec) != NULL)
      return AT_MQTTJQ;  */ 
  
  if(strstr(s, at.mqttsetrec) != NULL)
      return AT_MQTTSET;
  
  if(strstr(s, at.mqttsetconrec) != NULL)
      return AT_MQTTCON;
  
  if(strstr(s, at.mqttsetsubrec) != NULL)
      return AT_MQTTSUB; 
  
  if(strstr(s, at.mqttpub) != NULL)
      return AT_MQTTPUB; 
  
 
  
  return -1;
}

void manage_rec_at(char *rec_str, u8 rec_flag)
{
  char *s = rec_str;
  char *s_offset;
  switch(rec_flag)
  {
  case AT_AT:
    if(strstr(s, "OK") != NULL)
      at_rec_flag.at = 1;
    break;
    
  case AT_CPIN:
    if(strstr(s, "READY") != NULL)
      at_rec_flag.cpin = 1;
    break;
       
  case AT_CSQ:
    s_offset = strstr(s, ":");
    ++s_offset;
    char csq_data[2];
    if((u8)strncpy(csq_data, s_offset, 2) >= 17 
       && (u8)strncpy(csq_data, s_offset, 2) <= 31);
      at_rec_flag.csq = 1;
    break;
    
  case AT_CGREG:
    if(strstr(s, "1") != NULL)
      at_rec_flag.cgreg = 1;
    break;
    
  case AT_XIIC0:
    if(strstr(s, "OK") != NULL)
      at_rec_flag.xiic0 = 1;    
    break;
    
  case AT_XIIC1:
    if(strstr(s, ":") != NULL)
      at_rec_flag.xiic1 = 1;    
    break;
    
  case AT_GPSPWR:
    if(strstr(s, "OK") != NULL)
      at_rec_flag.gpspwr = 1;     
    break;
    
  case AT_GPSSTA:
    if(strstr(s, "open") != NULL)
      at_rec_flag.gpssta = 1;     
    break;
    
  case AT_POS:
    if(strstr(s, "N") != NULL)
    {
      s_offset = strstr(s, "N");
      strncpy(yf4g_send_data.latitude,s_offset - 11, 10);
      strncpy(yf4g_send_data.longitude,s_offset + 2, 11);
      at_rec_flag.gpspos = 1;
    }
    else
      at_rec_flag.gpspos = 0;
    break;
    
  case AT_MQTTJQ:
    if(strstr(s, "OK") != NULL)
      at_rec_flag.mqttjq = 1;
    
    break;
    
  case AT_MQTTSET:
    if(strstr(s, "OK") != NULL)
      at_rec_flag.mqttset = 1;
    
    break;
   
  case AT_MQTTCON:
     if(strstr(s, "OK") != NULL)
      at_rec_flag.mqttcon = 1;
     
     break;
    
  case AT_MQTTSUB:
    if(strstr(s, "OK") != NULL)
    {
      at_rec_flag.mqttsub = 1;
      yf4g_init_done = 1;
    }
    if(strstr(s, "getdata") != NULL)
      getdata_flag = 1;
    /*if(strstr(s, "ERROR") != NULL)
    {
      getdata_flag = 0;
      yf4g_init_done = 0;
    }*/
    break;
    
  case AT_MQTTPUB:
    if(strstr(s, "OK") != NULL)
      at_rec_flag.mqttpub = 1;
    else
    {
      send_4gdata(at.mqttdiscon);
      at_rec_flag.mqttset = 0;
      at_rec_flag.mqttcon = 0;
      at_rec_flag.mqttsub = 0;
      yf4g_init_done = 0; 
    }
    
    break;
    
  default :
    break;
  
  }
}

void uart2_irq(void)
{
  char res;
  res = USART2->DR;
  
  yf4g_rec[uart2_offset++] = res;
   
 
}