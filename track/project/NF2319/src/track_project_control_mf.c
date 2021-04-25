#if defined(__NF2319__)
/****************************************************************************
* Include Files                         ����ͷ�ļ�
*****************************************************************************/
#include "track_cust.h"
#include "track_os_paramete.h"
#include "track_project_control_mf.h"
/*****************************************************************************
* Typedef  Enum                         ö�ٶ���
*****************************************************************************/

/*****************************************************************************
 *  Struct                              ���ݽṹ����
 *****************************************************************************/
 static ctrl_data_struct  ctrl_params = {0};
/*****************************************************************************
 *  Define					�궨��
 *****************************************************************************/

/*****************************************************************************
* Local variable                        �ֲ�����
*****************************************************************************/


/****************************************************************************
* Global Variable                       ȫ�ֱ���
*****************************************************************************/


/****************************************************************************
* Global Variable - Extern              ����ȫ�ֱ���
*****************************************************************************/

/*****************************************************************************
 *  Global Functions    - Extern        �����ⲿ����
 *****************************************************************************/

/*****************************************************************************
 *  Local Functions                     ���غ���
 *****************************************************************************/
 
/******************************************************************************
 *  Function    -  ecc_data
 *
 *  Purpose     -  У���
 *
 *  Description - �͵ĵ� 8 λ
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-11-01,  Cjj  written
 * ----------------------------------------
 ******************************************************************************/
kal_uint8 ecc_data(kal_uint8* data, kal_uint16 len)
{
    int i,tmp=0;
    for(i=0;i<len;i++)
    {
       tmp += data[i];
    }
    tmp&=0xff;
    return tmp;
}

 void control_write_data()
{
    static U8 buf[6] = {0}, len = 0;
    static kal_uint16 ecc;
    len = 5; //Ŀǰ�̶�����Ϊ5
    buf[0] = 0x59;
    buf[1] = len;
    buf[2] = G_parameter.ctrl.state;
    buf[3] = G_parameter.ctrl.speed;
    buf[4] = ecc_data(&buf[1],3);
    track_drv_uart3_write_data(buf, len);
    tr_start_timer(TRACK_RROJECT_CONTROL_UART_WRITE_ID, 500, control_write_data);//�������>200MS�����ϵ�500ms���޷�����
}

/******************************************************************************
 *  Function    -  track_ctrl_uart_rece
 *
 *  Purpose     -  ���տ����������ݴ���
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-06-19,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
 void track_ctrl_uart_rece(kal_uint8* buff, kal_uint16 len)
{
    kal_uint8* p = NULL;
   // kal_uint8 speed_data[1] = {0};
    kal_uint8 data[11] = {0};
    kal_uint8 ecc;
    int speed_Tx=0;
    float speed=0,distance=0;
    ctrl_data_struct  params = {0};
    if((buff == NULL) || (len == 0))
    {
        return;
    }

    if(len > 10)
    {
        len = 10;
    }
    memcpy(data, buff, len);

    LOGH(L_APP, L_V4, data, len);
    ecc = ecc_data(&data[1],8);
    if(ecc!=data[9])
    {
      LOGD(L_APP, L_V4,"[track_ctrl_uart_rece]error:ecc %d/%d",ecc,data[9]);
      return;
    }
    //����
   // speed_data[0] = data[3];
  //  speed_data[1] = data[2];
  //  LOGH(L_APP, L_V4, speed_data, 2);
  //  GET_UINT16_BE(speed_Tx,speed_data,0);
  speed_Tx = data[3]<<8;
  speed_Tx |=data[2];
    //�ٶ�KM/H
    LOGD(L_APP, L_V4,"����:%d",speed_Tx);    
    //speed = (speed_Tx*7200 *(16.7*2.54/100000)*3.14)/6*20*100;
    speed =(float)speed_Tx*0.079915512;
    LOGD(L_APP, L_V4,"speed:%.06f",speed);
    speed = (int)(speed*100);//�ٶȾ���ת���0.00 KM/H
    params.speed = speed/100;
    //����������
    params.iChr = data[4];
    //��������ѹ
    params.vChr = data[5];
    /*
    ����״̬ Status1 : 1���й��� 0 �� �޹���
    Bit 7 �� ����
    Bit 6 �� ����
    Bit 5 �� ����
    Bit 4 �� Ƿѹ����
    Bit 3 �� ����������
    Bit 2 �� �����������
    Bit 1 �� ɲ������
    Bit 0 �� ת�ѹ���
    */
    params.state1 = data[6];
    /*
   ����״̬ Status2: 1���й��� 0 �� �޹���
    Bit 7 �� ����
    Bit 6 �� ����
    Bit 5 �� ����
    Bit 4 �� ����
    Bit 3 �� ����
    Bit 2 �� ����
    Bit 1 �� ����
    Bit 0 �� ���� 
    */
    params.state2 = data[7];

    //���㵥λʱ����(500MS)��λ�ƣ���λM
    distance = (int)(params.speed/3.6 *0.5*100);
    params.distance = distance/100 + ctrl_params.distance;

    memcpy(&ctrl_params, &params, sizeof(ctrl_params));
    LOGD(L_APP, L_V4,"�ٶ�:%.02f ,����:%d ,��ѹ:%d ,�������:%.02f"
        ,ctrl_params.speed,ctrl_params.iChr,ctrl_params.vChr,ctrl_params.distance);
}

#endif/*__EL100_M03__*/
