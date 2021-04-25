/******************************************************************************
 * track_project_main.c -
 *
 * Copyright .
 *
 * DESCRIPTION: -
 *        ����Ŀ��ͬ����ڴ��� ,ÿ����Ŀ������������ͬ���ļ�������������
 *
 * modification history
 * --------------------
 * v1.0   2017-06-30,  chengjun create this file
 *
 ******************************************************************************/

/****************************************************************************
* Include Files                         ����ͷ�ļ�
*****************************************************************************/
#include "track_os_data_types.h"
#include "kal_public_defs.h"
#include "track_os_log.h"
#include "track_drv_gps_decode.h"
#include "track_drv_uart.h"
#include "Track_cust.h"

/*****************************************************************************
 *  Define					�궨��
 *****************************************************************************/




/*****************************************************************************
* Typedef  Enum         ö��
*****************************************************************************/


/*****************************************************************************
 *  Struct                              ���ݽṹ����
 *****************************************************************************/



/*****************************************************************************
 *  Global Functions			ȫ�ֱ���
 *****************************************************************************/
extern nvram_parameter_struct              G_parameter ;

/*****************************************************************************
 *  Local Functions			���غ���
 *****************************************************************************/


/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/
#if defined(__OBD__)
extern void track_cust_obd_data(kal_uint8 *data, int len);
#endif
#if defined(__BMS_PHYLION__)
extern void track_cust_bms_data_receive(kal_uint8 *data,kal_uint16 len);
#endif
extern void track_cust_atech_data_receive(kal_uint8 *srcdata, kal_uint16 len);

/******************************************************************************
 *  Function    -  track_project_main_init
 *
 *  Purpose     -  ������ִ��һ�Σ�����Ŀ������ִ�г�ʼ���¼�
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-06-30,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_project_main_init(void)
{
    track_cust_LBS_upload();

}



/******************************************************************************
 *  Function    -  track_project_gpio_init
 *
 *  Purpose     -  GPIO�˿ڳ�ʼ���������������Ϊ�պ���
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-07-12,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_project_gpio_init(void)
{
    GPIO_ModeSetup(0, 0);
    GPIO_InitIO(1, 0);
    GPIO_WriteIO(0, 0);

    GPIO_ModeSetup(1, 0);
    GPIO_InitIO(1, 1);
    GPIO_WriteIO(0, 1);
}

kal_uint8 track_project_uart_init(void)
{
    return YES_GO_ON;
}

/******************************************************************************
 *  Function    -  track_project_keypad_init
 * 
 *  Purpose     -  ����Ծ������ע��
 * 
 *  Description -   
 * 
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-07-12,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_project_keypad_init()
{

}

/******************************************************************************
 *  Function    -  track_project_gps_work_and_fix_nitify
 *
 *  Purpose     -  GPS�Ӳ���λ����λ��Ϣ
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-07-12,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_project_gps_work_and_fix_nitify(track_gps_data_struct * gps_data)
{


}

/******************************************************************************
 *  Function    -  track_project_gps_work_no_fix_nitify
 *
 *  Purpose     -  GPS�Ӷ�λ������λ��Ϣ
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-07-12,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_project_gps_work_no_fix_nitify(void)
{


}

/******************************************************************************
 *  Function    -  track_project_gsensor_shocked_event
 *
 *  Purpose     -  �������¼����Ƿ����ԭ�������̣�
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-07-12,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
kal_uint8 track_project_gsensor_shocked_event(void)
{
    return YES_GO_ON;
}


/******************************************************************************
 *  Function    -  track_project_enters_quiescent_state_notify
 *
 *  Purpose     -  һ��ʱ��δ��⵽�𶯣����뾲ֹ״̬
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-07-12,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_project_enters_quiescent_state_notify(void)
{

}

/******************************************************************************
 *  Function    -  track_project_status_eint_notify
 * 
 *  Purpose     -  �ж��¼�֪ͨ
 * 
 *  Description -   
 * 
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-07-13,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
kal_uint8 track_project_status_eint_notify(U8 eint, U8 level)
{
    return YES_GO_ON;
}

/******************************************************************************
 *  Function    -  track_project_uart_receice_data_notify
 * 
 *  Purpose     -  �����յ�����֪ͨ
 * 
 *  Description -   
 * 
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-07-13,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
kal_uint8 track_project_uart_receice_data_notify(UART_PORT port,kal_uint8* data, kal_uint16 len)
{
	LOGD(L_UART, L_V5, "port:%d,len:%d", port,len);
	if(port == uart_port3)
	{
#if defined(__BMS_PHYLION__)
	track_cust_bms_data_receive(data,len);
	return 1;
#endif
#if defined(__OBD__)
		if(G_parameter.obd.sw == 1)
		{
#if defined(__ATECH__)
		//track_cust_atech_data_analyze(data,len);
		track_cust_atech_data_receive(data,len);
#else
		track_cust_obd_data(data,len);
#endif
		}
#endif
	}
    return YES_GO_ON;
}


