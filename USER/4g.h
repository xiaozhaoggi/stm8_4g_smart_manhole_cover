#ifndef _4G_H
#define _4G_H
#include "stm8l15x_gpio.h"

#define RSTIO GPIO_Pin_5        //RST �ܽź�


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
#define AT_MQTTPUB      13   //������ձ�־λ

struct AT
{
  char *at;     //�ȴ�ģ����Ӧ
  char *cpin;   //��ѯ���Ƿ���
  char *csq;    //��ѯ�ź�������ֻ���ź�������17-31 ֮�䣬ģ������ȶ�����
  char *cgreg;  /*��ѯ����״̬��ֱ������Ϊ1 ��ʾ���������磬���һֱû���ţ�
                �Ͽ�ͣ����*/
  char *xiic0;  //�������磬�������ֻ�ܷ�һ�Σ����ҵȴ�ʱ���30 ��
  char *xiic1;  //��ѯ�Ƿ���䵽IP��ֻ�з��䵽IP���������
/*  char *gpspwr; //��gps
  char *gpssta;  //��ѯgps״̬
  char *gpspos; //��ȡgps��λ��Ϣ
  char *mqttjq; //����matt��Ȩ,��Ȩ�ɹ�����ܽ��ж��ĺͷ���*/
  char mqttset[50];//����MQTT���Ӳ��� client_id usrname password
  char *mqttcon;//MQTT���� ip��port
  char *mqttsub; //����MQTT
  char *mqttpub; //����MQTT��Ϣ
  char *mqttdiscon;
  
  //������
  /*char *mqttjqrec; //�ƶ˷��ؼ�Ȩ��Ϣ֡ͷ
  char *mqttpubrec;//�ƶ˷��ط�����Ϣ֡ͷ
  char *mqttsubrec;//�ƶ˷��ض�����Ϣ֡ͷ*/
  
  char *mqttsetrec;//����MQTT���Ӳ�������֡ͷ
  char *mqttsetconrec;//MQTT���ӷ���֡ͷ
  char *mqttsetsubrec;//MQTT���ķ���֡ͷ
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
  char *latitude;      //γ��
  char *longitude;     //����
};
void yf4g_init(void);
void send_yf4g(void);
void pub_data(char *data, u8 flag);
void send_4gdata(char *fun);
u8 which_rec_at(char *rec_str);
void manage_rec_at(char *rec_str, u8 rec_flag);
void uart2_irq(void);


#endif