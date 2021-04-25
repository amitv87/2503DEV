#if defined __SMT_BMS__
#ifndef _TRACK_DRV_SMARTBMS_H
#define _TRACK_DRV_SMARTBMS_H

#include "stack_msgs.h"
#include "stack_ltlcom.h"
#include "drvsignals.h"
#include "kal_public_defs.h"


#define BMS_HEAD 0X3A
#define BMS_ADD 0X16
#define BMS_END1 0X0D
#define BMS_END2 0X0A
#define BMS_UART_BUFF_LEN_MAX 32

#define BMS_EVT_TEMP 0X08   //�¶�
#define BMS_EVT_FULLVOL 0X09  //��ѹ
#define BMS_EVT_ELE 0X0A //����
#define BMS_EVT_RRAT 0X0D  //��Ե�������ٷְ�
#define BMS_EVT_ABRAT 0X0E //���Ե�������ٷֱ�
#define BMS_EVT_RCAP 0X0F  //ʣ������
#define BMS_EVT_FULLCAP 0X10 //��������
#define BMS_EVT_CYCTIME 0X17 //ѭ������
#define BMS_EVT_VOL1 0X24  // 1-7�ڵ�ɵ�ѹ
#define BMS_EVT_VOL2 0X25  //8 -14
#define BMS_EVT_CURCHRINVELTIME 0X47  //��ǰ�����ʱ��
#define BMS_EVT_MAXCHRINVELTIME 0X48 //�������ʱ��

#define MAX_BMS_COMM_LEN 32

typedef struct
{
    kal_uint8 event;
    kal_uint8 data[BMS_UART_BUFF_LEN_MAX];
} bms_event_msg;

typedef struct
{
    LOCAL_PARA_HDR
    bms_event_msg bms;
} bms_event_msg_struct;


typedef struct
{
    kal_uint8   buffer[MAX_BMS_COMM_LEN];
    int         len;
    kal_uint8   packet_type;              // ���ݰ�����
} bms_vec_write_struct;
#endif
#endif
