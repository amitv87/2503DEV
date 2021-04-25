/******************************************************************************
 * track_WatchDog.c -
 *
 * Copyright .
 *
 * DESCRIPTION: -
 *        Watdog �� EM78P152  ����
 *
 * modification history
 * --------------------
 * v1.0   2012-07-14,  chengjun create this file
 *
 ******************************************************************************/
#if defined (__WATCH_DOG__)

/****************************************************************************
* Include Files             ����ͷ�ļ�
*****************************************************************************/
#include "track_os_timer.h"
#include "track_os_ell.h"
#include "track_os_paramete.h"
#include "track_project_hal_define.h"

/*****************************************************************************
 *  Define			    �궨��
*****************************************************************************/

#define WDT_TIME 1500


#if defined (__WDT_USE_I2C__)
#define WDT_SCL 29
#define WDT_SDA 27

#elif defined (__WDT_PLUSE__)
#ifndef WATCH_DOG_GPIO
#error "not defined  WATCH_DOG_GPIO"
#endif
#endif

/*****************************************************************************
 *  Local Functions	            ���غ���
*****************************************************************************/
static kal_int8 wdt_close_shutdown = 0;
static kal_int8  wdt_status = 1;//0�ر�1��������
/******************************************************************************

                                 ��152Э��:  ֻ�ǲ��Ϸ���ʱ���ź�ι��

 ******************************************************************************/
#if defined (__WDT_PLUSE__)

static void track_watch_dog_init(void);
static void track_watch_dog_high(void);
static void track_watch_dog_low(void);

/******************************************************************************
 *  Function    -  track_watch_dog_init
 *
 *  Purpose     -  ι���˿�����
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * ----------------------------------------
 ******************************************************************************/
static void track_watch_dog_init(void)
{
    GPIO_ModeSetup(WATCH_DOG_GPIO, 0);
    GPIO_InitIO(1, WATCH_DOG_GPIO);
}

/******************************************************************************
 *  Function    -  track_watch_dog_high
 *
 *  Purpose     -  ���Ź��ź�ι��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * ----------------------------------------
 ******************************************************************************/
static void track_watch_dog_high(void)
{
    if(track_is_timer_run(WATCH_DOG_TIMER_ID))
    {
        tr_stop_timer(WATCH_DOG_TIMER_ID);
    }
    GPIO_WriteIO(1, WATCH_DOG_GPIO);

    tr_start_timer(WATCH_DOG_TIMER_ID, WDT_TIME, track_watch_dog_low);
}


/******************************************************************************
 *  Function    -  track_watch_dog_low
 *
 *  Purpose     -  ���Ź��ź�ι����ѭ����������Ϣ
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * ----------------------------------------
 ******************************************************************************/
static void track_watch_dog_low(void)
{
    if(track_is_timer_run(WATCH_DOG_TIMER_ID))
    {
        tr_stop_timer(WATCH_DOG_TIMER_ID);
    }
    GPIO_WriteIO(0, WATCH_DOG_GPIO);
    //LOGD(L_DRV, L_V1, "");
    tr_start_timer(WATCH_DOG_TIMER_ID, WDT_TIME, track_watch_dog_high);
}

/******************************************************************************
 *  Function    -  Watch_dog_OFF
 *
 *  Purpose     -  �ر�WatchDog��ʱ��������ٸ�һ���ߵ�ƽ���Ӻ�20������
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 01-04-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
static void Watch_dog_OFF(void)
{
    tr_stop_timer(WATCH_DOG_TIMER_ID);
    GPIO_WriteIO(1, WATCH_DOG_GPIO);
    tr_stop_timer(WATCH_DOG_TIMER_ID);
}

#elif defined (__WDT_USE_I2C__)

/******************************************************************************

                                 ��152 Э��:  ͨ��IIC Э���뵥Ƭ��ͨ��

 ******************************************************************************/

/*****************************************************************************
* Local variable				�ֲ�����
*****************************************************************************/
static kal_uint8 RecBuf[3] = {0};

/*****************************************************************************
* Global variable			  ȫ�ֱ���
*****************************************************************************/
static kal_uint8 I2CTest = 0;       //��Ƭ��I2C  ͨ�Ų��Ա���λ

/*****************************************************************************
 *  Local Functions			���غ���
 *****************************************************************************/
static void WDT_I2C_Delay(kal_uint16 delay);
static void WDT_SerialCommStart(void);
static void WDT_SerialCommStop(void);
static void WDT_SerialCommTxByte(kal_uint8 data, kal_bool init_io);
static void WDT_SerialCommRxByte(kal_uint8 *data, kal_uint8 ack);
static void WDT_WriteReadByte(kal_uint8 data1, kal_uint8 data2, kal_uint8 *data);


static void track_ShutdownSystemOperation(void)
{
    LOGS("152�����رգ�ϵͳ(%d)���ڹػ�\r\n", wdt_close_shutdown);

    if(wdt_close_shutdown)
    {
        track_drv_sys_power_off_req();
    }

}
/******************************************************************************
 *  Function    -  WDT_I2C_Delay
 *
 *  Purpose     -  ����������ʱ
 *
 *  Description -   0.5ms
    //60ƽ̨MCU 350MHZ
*  5*2500   WDT_I2C_DELAY(5);  ʾ�������γ���CLK��2MS ����2MS
*  10*2500   WDT_I2C_DELAY(5);  ʾ�������γ���CLK��4MS ����4MS
*  15*2500   WDT_I2C_DELAY(5);  ʾ�������γ���CLK��6MS ����6MS
 * modification history
 * ----------------------------------------
* v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/
//#define WDT_DELAY_TIME              5*2500

//fota ʱ�����5*2500��Ϊ24ms��������10��֮�� 2500 == 10MS  1600=2.3MS 1800=2.4MS 1MS��Լ��200
/*��������WDT_I2C_Delay(5)����ʾʱ��20140515��֤ fota����3600Ϊ1ms*/
//#define WDT_DELAY_TIME_1MS              2500  //�����ӳ�ʱ��3000 5ms

#if defined( MT6261)
#define WDT_DELAY_TIME_1MS              2000  //����Fota 800=2ms//20151017  1600 4ms
#else
#define WDT_DELAY_TIME_1MS              800  //����Fota 500=3.2ms
#endif /* MT6261 */

//fota new fota remake ��
#define WDT_READ_DATA_DELAY      5
#define WDT_DATA_H                      5
#define WDT_DELAY_L                     3

static void  WDT_I2C_Delay(kal_uint16 delay)
{
    kal_uint16 i;
    while(delay--)
    {
        for(i = 0; i < WDT_DELAY_TIME_1MS; i++)
        {
            ;
        }
    }
}

/******************************************************************************
 *  Function    -  WDT_SerialCommStart
 *
 *  Purpose     -  IIC ͨ�ſ�ʼ����ʼλ
 *
 *  Description -  SCL һֱΪ��ʱ��SDA ��һ���½��آ{
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/
static void WDT_SerialCommStart(void)
{
    GPIO_InitIO(1, WDT_SDA);
    GPIO_WriteIO(1, WDT_SDA);
    WDT_I2C_Delay(WDT_DELAY_L);
    GPIO_WriteIO(1, WDT_SCL);
    WDT_I2C_Delay(WDT_DATA_H);
    GPIO_WriteIO(0, WDT_SDA);
    WDT_I2C_Delay(WDT_DELAY_L);
    GPIO_WriteIO(0, WDT_SCL);
    WDT_I2C_Delay(WDT_DATA_H);
}


/******************************************************************************
 *  Function    -  WDT_SerialCommStop
 *
 *  Purpose     -  һ��ͨ����ɵĽ���λ
 *
 *  Description -  SCL Ϊ��ʱ��SDA�� 0-->1
 *
 * modification history
 * ----------------------------------------
* v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/
static void WDT_SerialCommStop(void)
{
    GPIO_InitIO(1, WDT_SDA);
    GPIO_WriteIO(0, WDT_SCL);
    WDT_I2C_Delay(WDT_DELAY_L);
    GPIO_WriteIO(0, WDT_SDA);
    WDT_I2C_Delay(WDT_DATA_H);
    GPIO_WriteIO(1, WDT_SCL);
    WDT_I2C_Delay(WDT_DELAY_L);
    GPIO_WriteIO(1, WDT_SDA);
    WDT_I2C_Delay(WDT_DATA_H);
}


/******************************************************************************
 *  Function    -  WDT_SerialCommTxByte
 *
 *  Purpose     -  ��Ƭ����������
 *
 *  Description - ģ����д��1BIT���ݣ�SCL ��1-->0
 *
 *                        ��Ƭ�����SCL ������ȥ������
 *
 * modification history
 * ----------------------------------------
* v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/
static void WDT_SerialCommTxByte(kal_uint8 data, kal_bool init_io)
{
    kal_uint16 i;
    GPIO_InitIO(1, WDT_SDA);
    for(i = 8; --i > 0;)
    {
        GPIO_WriteIO((data >> i) & 0x01, WDT_SDA);
        WDT_I2C_Delay(WDT_DELAY_L);
        GPIO_WriteIO(1, WDT_SCL);  /* high */
        WDT_I2C_Delay(WDT_DATA_H);
        GPIO_WriteIO(0, WDT_SCL);  /* low */
        WDT_I2C_Delay(WDT_DELAY_L);
    }
    GPIO_WriteIO((data >> i) & 0x01, WDT_SDA);
    WDT_I2C_Delay(WDT_DELAY_L);
    GPIO_WriteIO(1, WDT_SCL);  /* high */
    WDT_I2C_Delay(WDT_DATA_H);
    GPIO_WriteIO(0, WDT_SCL);  /* low */

    if(init_io)
    {
        GPIO_InitIO(0, WDT_SDA);
    }

    WDT_I2C_Delay(10);
    GPIO_WriteIO(0, WDT_SCL);

}


/******************************************************************************
 *  Function    -  WDT_SerialCommRxByte
 *
 *  Purpose     -  ��ȡ��Ƭ������Ӧ���ݣ�ȷ���Ƿ��ͳɹ�
 *
 *  Description -  ��Ƭ�� ��SCL �½���д��1BIT����
 *
 *                          �ն���SCL Ϊ��ʱ����1BIT����
 *
 * modification history
 * ----------------------------------------
* v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/
static void  WDT_SerialCommRxByte(kal_uint8 *data, kal_uint8 ack)
{
    kal_uint8 i = 8;
    kal_uint8 dataCache = 0;
    char Tmp[50] = {0};

    GPIO_InitIO(0, WDT_SDA);
    for(i = 8; i > 0; i--)
    {
        dataCache <<= 1;
        GPIO_WriteIO(1, WDT_SCL);
        WDT_I2C_Delay(WDT_READ_DATA_DELAY);
        dataCache |= GPIO_ReadIO(WDT_SDA);
        GPIO_WriteIO(0, WDT_SCL);
        WDT_I2C_Delay(WDT_READ_DATA_DELAY);
    }
    GPIO_InitIO(1, WDT_SDA);
    GPIO_WriteIO(ack, WDT_SDA);
    WDT_I2C_Delay(WDT_READ_DATA_DELAY);
    GPIO_WriteIO(1, WDT_SCL);
    *data = (kal_uint8)dataCache;
    GPIO_WriteIO(0, WDT_SCL);
    if(I2CTest)
    {
        memset(Tmp, 0, 50);
        if(1 == I2CTest)
        {
            I2CTest = 0;
        }
        if(dataCache == 0x75)
        {
            LOGS("Receive Data:0x%x , I2C OK\r\n", dataCache);
        }
        else
        {
            LOGS("Receive Data:0x%x !=0x75\r\n", dataCache);
        }
    }
    else if(dataCache != 0x75)
    {
        LOGS("WDT_SerialCommRxByte:0x%x !=0x75!!!!!!152ͨѶ���쳣", dataCache);
    }
}



/******************************************************************************
 *  Function    -  WDT_WriteReadByte
 *
 *  Purpose     -  Prepare the SDA and SCL for sending two bytes and receive one byte
 *
 *  Description - �ն˷��������ֽ����ݣ�������һ���ֽڵ���Ӧ����
 *
 * modification history
 * ----------------------------------------
* v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/
static void WDT_WriteReadByte(kal_uint8 data1, kal_uint8 data2, kal_uint8 *data)
{
    WDT_SerialCommStart();                       // send the start sequence
    WDT_SerialCommTxByte(data1, 0);           //data to be written
    WDT_SerialCommTxByte(data2, 1);           // data to be written
    WDT_SerialCommRxByte(data, 0);        // read data and send ACK
    LOGD(L_DRV, L_V7, "write:%x,%x,read:%x", data1, data2, *data);
    WDT_SerialCommStop();                        // send the stop sequence
}

/*****************************************************************************
 *  Globall Functions		ȫ�ֺ���
 *****************************************************************************/


/******************************************************************************
 *  Function    -  WDT_SerialCommInit
 *
 *  Purpose     -  ��ʼ���˿ڣ�SDA /SCL ��Ϊͨ�������
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
* v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/
void WDT_SerialCommInit(void)
{
    LOGD(L_DRV, L_V5, "wdt_close_shutdown:%d", wdt_close_shutdown);

    GPIO_ModeSetup(WDT_SCL, 0);
    GPIO_ModeSetup(WDT_SDA, 0);
    GPIO_InitIO(1, WDT_SCL);
}


/******************************************************************************
 *  Function    -  WDT_SerialCommRelease
 *
 *  Purpose     -  ͨ�ſ���ʱ�˿��øߵ�ƽ
 *
 *  Description - (  ����) ����IIC ��Ҫ��ʵ����Ŀ�費�ϸ�ι���ź�
 *
 * modification history
 * ----------------------------------------
* v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/
void WDT_SerialCommRelease(void)
{
    GPIO_WriteIO(1, WDT_SCL);
    GPIO_WriteIO(1, WDT_SDA);
}


/******************************************************************************
 *  Function    -  WDT_SerialCommON
 *
 *  Purpose     -  ��������
 *
 *  Description - �ն�����������Ƭ������WDT  (0xAA . 0x56)
 *
 *                         ѭ����CLK �ź�ι��
 *
 * modification history
 * ----------------------------------------
* v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/
void WDT_SerialCommON(void)
{
    LOGD(L_DRV, L_V7, "");

    WDT_WriteReadByte(0XAA, 0X56, &RecBuf[0]);
    tr_start_timer(WATCH_DOG_TIMER_ID, 5000, WDT_SerialCommON);
}


/******************************************************************************
 *  Function    -  WDT_SerialCommDisable
 *
 *  Purpose     -  �͵�ػ�( �͵�Diable�ź�)
 *
 *  Description - �ն˵͵�ػ�ǰ��Ƭ������Disable �ź�(0xAD . 0x53)
 *
 *                        Ҫ��Ƭ�����ټ��WDT
 * modification history
 * ----------------------------------------
* v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/
void WDT_SerialCommDisable(void)
{
    if(track_is_timer_run(WATCH_DOG_TIMER_ID))
    {
        track_stop_timer(WATCH_DOG_TIMER_ID);
    }
    memset(&RecBuf, 0, sizeof(RecBuf));
    WDT_WriteReadByte(0XAD, 0X53, &RecBuf[0]);

    LOGD(L_DRV, L_V5, "RecBuf[0]=0x%x shutdown:%d", RecBuf[0], wdt_close_shutdown);

    if(RecBuf[0] != 0x75)
    {

        LOGS("\r\n WDT_SerialCommDisable Receive Data:0x%x !=0x75!!!!!!152ͨѶ���쳣\r\n", RecBuf[0]);
        tr_start_timer(WATCH_DOG_ENABLE_TIMER_ID, 2000, WDT_SerialCommDisable);
    }
    else
    {
        wdt_status = 0;
        tr_start_timer(SYSTEM_SHUTDOWN_TIMER_ID, 1000, track_ShutdownSystemOperation);
    }
}


/******************************************************************************
 *  Function    -  WDT_SerialCommEnable
 *
 *  Purpose     -  �ػ����( ��� Enable �ź�)
 *
 *  Description - �ն˹ػ�����������Ƭ������0xAC . 0x54
 *
 *                        Ҫ��Ƭ����� WDT ����λ�ն˲�����
 *
 * modification history
 * ----------------------------------------
* v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/
void WDT_SerialCommEnable(void)
{
    if(track_is_timer_run(WATCH_DOG_TIMER_ID))
    {
        track_stop_timer(WATCH_DOG_TIMER_ID);
    }

    memset(&RecBuf, 0, sizeof(RecBuf));
    WDT_WriteReadByte(0XAC, 0X54, &RecBuf[0]);
    if(RecBuf[0] != 0x75)
    {
        tr_start_timer(WATCH_DOG_ENABLE_TIMER_ID, 2000, WDT_SerialCommEnable);
    }
}


/******************************************************************************
 *  Function    -  WDT_SystemOn_Init
 *
 *  Purpose     -   �ն˿�������WDT�ź�
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
* v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/
void WDT_SystemOn_Init(void)
{
    LOGD(L_DRV, L_V5, "wdt_close_shutdown:%d", wdt_close_shutdown);
    WDT_SerialCommInit();
    tr_start_timer(WATCH_DOG_TIMER_ID, 2000, WDT_SerialCommON);
}


/*---------------------------------------------------------------------------------------
 * FUNCTION			WDT_SystemOFF_Init  ( X )
 *
 * DESCRIPTION	       �ն˹ػ�����ʼ��WDT IO��
 *
 * PARAMETERS		NULL
 * RETURNS			NULL
 *
 *---------------------------------------------------------------------------------------*/
void WDT_SystemOFF_Init()
{
    WDT_SerialCommInit();
}

/******************************************************************************
 *  Function    -  Watch_dog_Charging_Enable
 *
 *  Purpose     -  �ն˹ػ���磬���Ϳ����Ź�ʹ���ź�
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * ----------------------------------------
 ******************************************************************************/
void Watch_dog_Charging_Enable(void)
{
    WDT_SerialCommEnable();
}

#endif /* __WDT_PLUSE__ */


/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/


/******************************************************************************
 *  Function    -  track_drv_watdog_init
 *
 *  Purpose     -  ���Ź���ʼ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 27-07-2012,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_drv_watdog_init(void)
{
#if defined (__WDT_PLUSE__)
    track_watch_dog_init();
    track_watch_dog_high();
#elif defined (__WDT_USE_I2C__)
    WDT_SystemOn_Init();
#else
#error "track_drv_watdog_init"
#endif /* __WDT_PLUSE__ */
}

void track_drv_watdog_close(kal_uint8 shutdown)
{
#if defined (__WDT_PLUSE__)
    Watch_dog_OFF();
#elif defined (__WDT_USE_I2C__)

    if(shutdown == 1)
    {
        wdt_close_shutdown = 1;
        WDT_SerialCommDisable();
    }
    else if(shutdown == 0)
    {
        wdt_close_shutdown = 0;
        WDT_SerialCommON();
        WDT_SerialCommRelease();
        track_stop_timer(WATCH_DOG_TIMER_ID);
    }
    else if(shutdown == 2)
    {
        wdt_close_shutdown = 0;
        WDT_SerialCommDisable();
    }
#else
#error 
#endif /* __WDT_PLUSE__ */

}
/******************************************************************************
 *  Function    -  track_drv_wdt_staus
 *
 *  Purpose     -  ��ѯ���Ź�����״̬
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 27-07-2012,  wangqi  written
 * ----------------------------------------
 ******************************************************************************/
kal_uint8 track_drv_wdt_staus(void)
{
    return wdt_status;
}

void track_drv_watch_test(void)
{
#if defined (__WDT_PLUSE__)

#elif defined (__WDT_USE_I2C__)
    I2CTest = 1;
#endif
}
void track_drv_watch_receive_display(void)
{
#if  defined (__WDT_PLUSE__)

#elif defined (__WDT_USE_I2C__)
    I2CTest = 2;
#endif
}

void track_drv_watchdog_enable(void)
{
#if defined (__WDT_PLUSE__)

#elif defined (__WDT_USE_I2C__)
    WDT_SerialCommInit();

    WDT_SerialCommEnable();
#endif
}

void WDT_Worng_Data_Test(kal_uint8 type, kal_uint16 data)
{
#if defined( __WDT_USE_I2C__)
    if(type == 1)
    {
        if(track_is_timer_run(WATCH_DOG_TIMER_ID))
        {
            track_stop_timer(WATCH_DOG_TIMER_ID);
        }
        tr_start_timer(WATCH_DOG_TIMER_ID, 5000, WDT_SerialCommON);
    }

    memset(&RecBuf, 0, sizeof(RecBuf));
    WDT_WriteReadByte(data >> 8 & 0x00ff, data & 0x00ff, &RecBuf[0]);

    LOGS("\r\n WDT wrong data test write 0X%x0X%x and receive data:0x%x\r\n", data >> 8 & 0x00ff, data & 0x00ff, RecBuf[0]);
#endif /* __WDT_USE_I2C__ */
}

#endif /* __WATCH_DOG__ */

