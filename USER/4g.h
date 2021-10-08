#ifndef _4G_H
#define _4G_H
#include "stm8l15x_gpio.h"

#define RSTIO GPIO_Pin_5        //RST 管脚号


#define AT_AT           0     
#define AT_CPIN         1
#define AT_CSQ          2
#define AT_CGREG        3
#define AT_XIIC0        4
#define AT_XIIC1        5
#define AT_GPSPWR       6
#define AT_GPSSTA       7
#define AT_POS          8
#define AT_MQTTJQ       9
#define AT_MQTTSET      10
#define AT_MQTTCON      11
#define AT_MQTTSUB      12
#define AT_MQTTPUB      13   //定义接收标志位

struct AT
{
  char *at;     //等待模组响应
  char *cpin;   //查询卡是否插好
  char *csq;    //查询信号质量，只有信号质量在17-31 之间，模块才能稳定工作
  char *cgreg;  /*查询网络状态，直到返回为1 表示附着了网络，如果一直没附着，
                肯卡停机了*/
  char *xiic0;  //激活网络，这个命令只能发一次，并且等待时间最长30 秒
  char *xiic1;  //查询是否分配到IP，只有分配到IP后才能上网
/*  char *gpspwr; //打开gps
  char *gpssta;  //查询gps状态
  char *gpspos; //获取gps定位信息
  char *mqttjq; //进行matt鉴权,鉴权成功后才能进行订阅和发布*/
  char mqttset[50];//设置MQTT连接参数 client_id usrname password
  char *mqttcon;//MQTT连接 ip：port
  char *mqttsub; //订阅MQTT
  char *mqttpub; //推送MQTT信息
  char *mqttdiscon;
  
  //阿里云
  /*char *mqttjqrec; //云端返回鉴权信息帧头
  char *mqttpubrec;//云端返回发布信息帧头
  char *mqttsubrec;//云端返回订阅信息帧头*/
  
  char *mqttsetrec;//设置MQTT连接参数返回帧头
  char *mqttsetconrec;//MQTT连接返回帧头
  char *mqttsetsubrec;//MQTT订阅返回帧头
};

struct AT_REC_FLAG
{
  u8 at;     
  u8 cpin;   
  u8 csq;    
  u8 cgreg;                 
  u8 xiic0;  
  u8 xiic1;  
  u8 gpspwr; 
  u8 gpssta;  
  u8 gpspos; 
  u8 mqttjq; 
  u8 mqttset;
  u8 mqttcon;
  u8 mqttsub; 
  u8 mqttpub; 
};

struct YF4G_SEND_DATA
{
  char *latitude;      //纬度
  char *longitude;     //经度
};
void yf4g_init(void);
void send_yf4g(void);
void pub_data(char *data, u8 flag);
void send_4gdata(char *fun);
u8 which_rec_at(char *rec_str);
void manage_rec_at(char *rec_str, u8 rec_flag);
void uart2_irq(void);


#endif