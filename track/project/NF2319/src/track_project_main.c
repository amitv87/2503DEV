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
#include "track_project_control_mf.h"

/*****************************************************************************
 *  Define					�궨��
 *****************************************************************************/
#define MCU_UART_PORT               uart_port3
#define MCU_UART_BAND               9600



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
static void track_uart_read_mcu_version(void);
static void track_drv_set_protection_voltage(void);
void track_drv_mcu_send_data(kal_uint8 cmd);
void track_eint_WHEEL_registration(void);
static void track_eint_WHEEL_hisr(void);
/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/
extern void track_cust_key_led_sleep();
extern void track_project_eint_init_registration(U8 eint, U8 level);
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

}

/******************************************************************************
 *  Function    -  track_drv_gpio50_init
 *
 *  Purpose     -  GPIO50,�����������
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-07-12,  CJJ  written
 * ----------------------------------------
 ******************************************************************************/
void track_drv_gpio50_init(void)
{
    GPIO_ModeSetup(EXBAT_CONTROL_GPIO, 0);
    GPIO_InitIO(1, EXBAT_CONTROL_GPIO);
    GPIO_WriteIO(0, EXBAT_CONTROL_GPIO);
}

/******************************************************************************
 *  Function    -  track_drv_gpio48_init
 *
 *  Purpose     -  GPIO48,��������ʼ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-07-12,  CJJ  written
 * ----------------------------------------
 ******************************************************************************/
void track_drv_gpio48_init(void)
{
    GPIO_ModeSetup(SADDLE_LOCK_GPIO, 0);
    GPIO_InitIO(1, SADDLE_LOCK_GPIO);
    GPIO_WriteIO(0, SADDLE_LOCK_GPIO);
}

/******************************************************************************
 *  Function    -  track_drv_gpio47_init
 *
 *  Purpose     -  GPIO47,��������ʼ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-07-12,  CJJ  written
 * ----------------------------------------
 ******************************************************************************/
void track_drv_gpio47_init(void)
{
    GPIO_ModeSetup(47, 0);
    GPIO_InitIO(1, 47);
    GPIO_WriteIO(0, 47);
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
    track_drv_enable_vsim2_power_domain();
    
#if defined (__WHEEL_CHECK__)
    //�ֶ��ж��ź�
    track_eint_WHEEL_registration();
#endif /* __WHEEL_CHECK__ */

    track_drv_gpio50_init();

    track_drv_gpio48_init();

    track_drv_gpio47_init();
}

kal_uint8 track_project_uart_init(void)
{
    track_drv_uart_config(MCU_UART_PORT, MOD_MMI, MCU_UART_BAND);

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
kal_uint8 track_project_uart_receice_data_notify(UART_PORT port, kal_uint8* data, kal_uint16 len)
{
    char temp[] = {0};
    if(port == MCU_UART_PORT)
    {
        if(track_is_testmode())
        {
            memcpy(temp, data, len);
            LOGH(L_DRV, L_V4, temp, len);
            if(temp[0]==0x59)
            LOGS("UART3_OK");
        }
        else
        {
            //����ȥ����
            track_ctrl_uart_rece(data, len);
            LOGH(L_DRV, L_V4, data, len);
            LOGH2(99, 0, "Uart Recv", data, len);
            return NO_STOP;
        }        
    }
    return YES_GO_ON;
}
#if defined(__WHEEL_CHECK__)
/******************************************************************************
 *  Function    -  track_eint_WHEEL_registration
 *
 *  Purpose     -  �ж�WHEEL��ʼ��
 *
 *  Description - �ֶ��ź� ���ش���ģʽ
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-12-7,  cjj  written
 * ----------------------------------------
 ******************************************************************************/
void track_eint_WHEEL_registration(void)
{
    if(EXTRA_WHEEL_EINT_NO == 0xFF) return;
    EINT_Mask(EXTRA_WHEEL_EINT_NO);

    LOGD(L_DRV, L_V6, "EINT WHEEL %d, GPIO:%d, M:%d, %d", EXTRA_WHEEL_EINT_NO, EXTRA_WHEEL_GPIO, EINT_WHEEL_EINT_MODE, kal_get_active_module_id());

    GPIO_ModeSetup(EXTRA_WHEEL_GPIO, EINT_WHEEL_EINT_MODE);


    if(track_status_test(2) == KAL_TRUE)
    {
        LOGD(L_DRV, L_V6, "EINT WHEEL LV");
        EINT_SW_Debounce_Modify(EXTRA_WHEEL_EINT_NO, EINT_SW_DEBOUNCE_TIME);
        EINT_Set_Sensitivity(EXTRA_WHEEL_EINT_NO, EINT_SENSITIVE_MODE);
        if(GPIO_ReadIO(EXTRA_WHEEL_GPIO))
        {
            track_project_eint_init_registration(EXTRA_WHEEL_EINT_NO, LEVEL_HIGH);
            EINT_Registration(EXTRA_WHEEL_EINT_NO, KAL_TRUE, LEVEL_LOW, track_eint_WHEEL_hisr, KAL_TRUE);
        }
        else
        {
            track_project_eint_init_registration(EXTRA_WHEEL_EINT_NO, LEVEL_LOW);
            EINT_Registration(EXTRA_WHEEL_EINT_NO, KAL_TRUE, LEVEL_HIGH, track_eint_WHEEL_hisr, KAL_TRUE);
        }
        EINT_UnMask(EXTRA_WHEEL_EINT_NO);
    }
    else
    {
        EINT_Set_Sensitivity(EXTRA_WHEEL_EINT_NO, EDGE_SENSITIVE);
        EINT_Registration(EXTRA_WHEEL_EINT_NO, KAL_FALSE, LEVEL_HIGH, track_eint_WHEEL_hisr, KAL_TRUE);
        EINT_UnMask(EXTRA_WHEEL_EINT_NO);
    }
}

/******************************************************************************
 *  Function    -  track_eint_WHEEL_hisr
 *
 *  Purpose     -  �ж�WHEEL ��Ӧ����
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-12-7,  cjj  written
 * ----------------------------------------
 ******************************************************************************/
static void track_eint_WHEEL_hisr(void)
{
    kal_bool eint_A_status;
    if(EXTRA_WHEEL_EINT_NO == 0xFF) return;
    EINT_Mask(EXTRA_WHEEL_EINT_NO);
    if(track_status_test(2) == KAL_TRUE)
    {
        if(GPIO_ReadIO(EXTRA_WHEEL_GPIO))
        {
            track_project_eint_init_registration(EXTRA_WHEEL_EINT_NO, LEVEL_HIGH);
            eint_A_status = LEVEL_LOW;
        }
        else
        {
            track_project_eint_init_registration(EXTRA_WHEEL_EINT_NO, LEVEL_LOW);
            eint_A_status = LEVEL_HIGH;
        }
        LOGD(L_DRV, L_V9, "EINTB %d,%d", eint_A_status, kal_get_active_module_id());
        EINT_SW_Debounce_Modify(EXTRA_WHEEL_EINT_NO, EINT_SW_DEBOUNCE_TIME);
        EINT_Set_Polarity(EXTRA_WHEEL_EINT_NO, eint_A_status);
        EINT_UnMask(EXTRA_WHEEL_EINT_NO);
    }
    else
    {
        track_project_eint_init_registration(EXTRA_WHEEL_EINT_NO, LEVEL_HIGH);
        EINT_SW_Debounce_Modify(EXTRA_WHEEL_EINT_NO, 20);
        EINT_Set_Polarity(EXTRA_WHEEL_EINT_NO, eint_A_status);
        EINT_UnMask(EXTRA_WHEEL_EINT_NO);
    }
}

/******************************************************************************
 *  Function    -  track_cust_car_wheel_check
 *
 *  Purpose     -  �˶����
 *
 *  Description - 1S�����Ĵ������ֶ��źţ����ж�Ϊ�˶�
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-12-7,  cjj  written
 * ----------------------------------------
 ******************************************************************************/
kal_bool track_cust_car_wheel_check(void *arg)
{
    static kal_uint32 tick, last_tick;
    static bool turn;
    static kal_uint8 i = 0,count = 0;//i���������ж��Ѿ���ֹ
    switch((kal_uint16)arg)
    {
        case 1:
            if(count == 0)
            {
                tick = kal_get_systicks();
            }            
            count ++;
          //  track_cust_motor_lock_change_xq(MOTOR_WHEEL_IND);
         //   track_xq_work_status_change(xq_sport_msg);
        case 2:
            last_tick = kal_get_systicks();
         //   LOGD(L_APP, L_V5, "cartweel_check:tick/last_tick(%d / %d)time:%d ,count=%d ,turn =%d ",tick,last_tick,kal_ticks_to_secs(last_tick - tick),count,turn);
            //һ���ڣ���������4����˵���˶�״̬��
            if(kal_ticks_to_secs(last_tick - tick) >= 1 && count >= 4)
            {
                if(i != 0)i = 0;
                if(turn == 0)turn = 1;
            }

            //�˶�����£�һ�����޴������ӣ��жϳ�����ת����
            if(turn == 1 && kal_ticks_to_secs(last_tick - tick) >= 1 && count < 1)
            {   
                i++;
                if(i == 3)                    
                turn = 0;
            }

            //�˶�״̬�£�ÿ��1�뿪ʼˢ��TICK��
            if(turn == 1 && kal_ticks_to_secs(last_tick - tick) >= 1)
            {
                if(count >= 1)
                {
                    count = 0;
                }
                tick = kal_get_systicks();
            } 
           
            track_start_timer(TRACK_CUST_CARTWHEEL_CHECK_TIMER_ID, 500, track_cust_car_wheel_check, (void*)2);
            if(turn == 0 && kal_ticks_to_secs(last_tick - tick) >= 2)
            {
               track_stop_timer(TRACK_CUST_CARTWHEEL_CHECK_TIMER_ID);
               i = 0;
               count = 0;
            }
            
        case 3:
            return turn;    
            break;
    }

}
#endif

