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
#include "track_cust.h"
#include "track_os_timer.h"
#include "track_ble_gatt.h"

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


/*****************************************************************************
 *  Local Functions			���غ���
 *****************************************************************************/
void track_project_hallsig_out();

/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/
extern void track_cust_key_led_sleep();

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
#if defined (__BLE_GATT__)
    //track_drv_ble_gatt_switch(1);
#endif
    track_drv_Set_VMC(KAL_TRUE, 2);
    LOGD(L_APP, L_V3, "VCM ON  (1=1.8V 2=2.8V 3=3V 4=3.3V)");
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

}

kal_uint8 track_project_uart_init(void)
{
#if defined(__SMT_BMS__)
    track_bms_queue_init();
    track_drv_uart_config(EXT_UART_PORT,MOD_MMI,EXT_UART_BAND);
#endif
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
    switch(eint)
    {
        case EXT_HALL_EINT_NO:
        {
            if(level==LEVEL_LOW)
            {
                track_project_hallsig_out();
            }
            break;
        }
        case LOCK_CHEKC_EINT1:
            break;
        case LOCK_CHEKC_EINT2:
            break;            
        default:
            return YES_GO_ON;
            break;
    }
    return NO_STOP;
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
#if defined(__SMT_BMS__)
    if(port==EXT_UART_PORT)
    {
        track_drv_bms_recdata(data,len);
    }
#endif

    return YES_GO_ON;
}
static U32 outing_time_tick=0;

void reset_hallsing()
{
    outing_time_tick=0;
}

//�����ź����
void track_project_hallsig_out()
{
    if(G_parameter.sfride.sw && G_realtime_data.lock_status && track_project_save_switch(99,sw_hallsig_alm)==0)
    {
        if(outing_time_tick==0)
        {
            outing_time_tick= kal_get_systicks();
        }
        else
        {
            if((kal_get_systicks()-outing_time_tick)/KAL_TICKS_1_SEC>=G_parameter.sfride.t)
            {
                //�Ƿ�����
                LOGS("HALLS SINGAL ALARM!!!!");
                track_project_save_switch(1,sw_hallsig_alm);
            }
        }
        tr_start_timer(TRACK_CUST_RESET_HALLSIG_TIMER_ID, 5*1000, reset_hallsing);
        LOGD(L_APP, L_V3, "�����ź����!!!!");
    }
}

/******************************************************************************
 *  Function    -  track_cust_reserve
 *
 *  Purpose     -  һ��ԤԼ������
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 02-03-2017,  ZengPing  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_reserve(void)
{
    LOGD(L_APP, L_V5, "reserve=%d", G_parameter.reserve);
    track_cust_led_sleep(1);
    tr_start_timer(TRACK_CUST_LED_RESERVE_TIMER_ID, G_parameter.reserve * 60 * 1000, track_cust_key_led_sleep);
}
void track_cust_unlock(kal_uint32 BtSerialNumber)
{

}

kal_uint16 track_cust_lock_paket_no(kal_uint16 flag, kal_uint16 no)
{

}

void track_cust_lock(kal_uint32 BtSerialNumber)
{

}

/******************************************************************************
 *  Function    -  track_project_deal_ble_msg
 *
 *  Purpose     - ����������Ϣ
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 09-10-2017,  xzq  written
 * ----------------------------------------
 ******************************************************************************/
void track_project_deal_ble_msg(void* msg)
{
    track_reply_ble_msg_struct *myMsgPtr = (track_reply_ble_msg_struct *)msg;
    switch(myMsgPtr->reply_data[0])
    {
        case BLE_MAC_ADDR:
            {
                LOGD(L_DRV, L_V5, "%02X,%02X,%02X,%02X,%02X,%02X", &myMsgPtr->reply_data[0], &myMsgPtr->reply_data[1], &myMsgPtr->reply_data[2], &myMsgPtr->reply_data[3], &myMsgPtr->reply_data[4], &myMsgPtr->reply_data[5]);
                //memcpy(&bt_addr.addr[0], &myMsgPtr->reply_data[1], BD_ADDR_SIZE);
                //HexToStr(btaddr_str, bt_addr.addr, 6);
            }
            break;
        case BLE_EVT_UNLOCK:
            G_realtime_data.lock_status=0;
            LOGD(L_APP, L_V5, "����!!!!");
            break;
        case BLE_EVT_LOCK:
            G_realtime_data.lock_status=1;
            LOGD(L_APP, L_V5, "����!!!!");
            break;
        default:
            break;
    }
}

kal_int8 track_project_control_buzzer(U8 high)
{
    track_drv_gpio_set(high,BUZZER_GPIO);
}

void track_project_didi();
void ddff()
{
    track_drv_gpio_set(0,BUZZER_GPIO);
    tr_start_timer(TRACK_CUST_BMSCOMM_DEFAULT_CHECK_TIMER_ID,G_realtime_data.tracking_time,track_project_didi);
}


void track_project_didi()
{
    track_drv_gpio_set(1,BUZZER_GPIO);
    tr_start_timer(TRACK_CUST_BMSCOMM_DEFAULT_CHECK_TIMER_ID,G_realtime_data.flashtime,ddff);
}
