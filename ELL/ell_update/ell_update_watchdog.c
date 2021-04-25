/*******************************************************************************************
 *
 * Filename:
 * Author :
 * Date :
 * Comment:
 *
 ******************************************************************************************/
#if defined( __WATCH_DOG__)
/****************************************************************************
*  Include Files                        ����ͷ�ļ�
*****************************************************************************/
#include "ell_update.h"
#include "track_project_hal_define.h"

/*****************************************************************************
*  Define                               �궨��
******************************************************************************/

#if defined (__WDT_USE_I2C__)
    #define WDT_SCL 29
    #define WDT_SDA 27
#else
#ifndef WATCH_DOG_GPIO
#error "not defined  WATCH_DOG_GPIO"
#endif
#endif 


/*****************************************************************************
*  Typedef  Enum                        ö�ٶ���
*****************************************************************************/

/*****************************************************************************
*  Struct                               ���ݽṹ����
******************************************************************************/

/*****************************************************************************
*  Local variable                       �ֲ�����
*****************************************************************************/

/****************************************************************************
*  Global Variable                      ȫ�ֱ���
*****************************************************************************/

/****************************************************************************
*  Global Variable - Extern             ����ȫ�ֱ���
*****************************************************************************/

/*****************************************************************************
*  Global Functions - Extern            �����ⲿ����
******************************************************************************/

/*****************************************************************************
*  Local Functions                      ���غ���
******************************************************************************/
#if defined( __WDT_PLUSE__)
static void watch_dog_high(void);
static void watch_dog_low(void);

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
static void watch_dog_high(void)
{
    if(ell_is_timer_run(ELL_WATCH_DOG_TIMER_ID))
    {
        ell_stop_timer(ELL_WATCH_DOG_TIMER_ID);
    }
    GPIO_WriteIO(1, WATCH_DOG_GPIO);
    ell_start_timer(ELL_WATCH_DOG_TIMER_ID, 1500, watch_dog_low);
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
static void watch_dog_low(void)
{
    if(ell_is_timer_run(ELL_WATCH_DOG_TIMER_ID))
    {
        ell_stop_timer(ELL_WATCH_DOG_TIMER_ID);
    }
    GPIO_WriteIO(0, WATCH_DOG_GPIO);
    ell_start_timer(ELL_WATCH_DOG_TIMER_ID, 1500, watch_dog_high);
}

#else


/******************************************************************************

                                 ��152 Э��:  ͨ��IIC Э���뵥Ƭ��ͨ��

 ******************************************************************************/

/*****************************************************************************
* Local variable				�ֲ�����
*****************************************************************************/

/*****************************************************************************
* Global variable			  ȫ�ֱ���
*****************************************************************************/
/*****************************************************************************
 *  Local Functions			���غ���
 *****************************************************************************/
static void ell_I2C_Delay(kal_uint16 delay);
static void ell_WDT_SerialCommStart(void);
static void ell_WDT_SerialCommStop(void);
static void ell_WDT_SerialCommTxByte(kal_uint8 data, kal_bool init_io);
static void ell_WDT_SerialCommRxByte(kal_uint8 *data, kal_uint8 ack);
static void ell_WDT_Write2Byte(kal_uint8 CW, kal_uint8 data);
static void ell_WDT_WriteReadByte(kal_uint8 data1, kal_uint8 data2, kal_uint8 *data);
static void ell_WDT_SerialCommON(void);

//#define WDT_DELAY_TIME              5*2500

//fota ʱ�����5*2500��Ϊ24ms��������10��֮�� 2500 == 10MS  1600=2.3MS 1800=2.4MS 1MS��Լ��200
/*��������WDT_I2C_Delay(5)����ʾʱ��20140515��֤ fota����3600Ϊ1ms*/
//10*2500 ���Ϊ3.2 
#define WDT_DELAY_TIME_1MS              12*2500// 12*2500 4ms //10*2500 2ms 15*2500 6ms
//61ƽ̨ 12*2500 4.7ms
#define WDT_READ_DATA_DELAY     5
#define WDT_DATA_H     5
#define WDT_DELAY_L     3
static kal_uint8 RecBuf[3] = {0};

/******************************************************************************
 *  Function    -  ell_I2C_Delay
 *
 *  Purpose     -  ����������ʱ
 *
 *  Description -   0.5ms
    //60ƽ̨MCU 350MHZ   
*  5*2500   ell_I2C_Delay(5);  ʾ�������γ���CLK��2MS ����2MS
*  10*2500   ell_I2C_Delay(5);  ʾ�������γ���CLK��4MS ����4MS
*  15*2500   ell_I2C_Delay(5);  ʾ�������γ���CLK��6MS ����6MS
 * modification history
 * ----------------------------------------
* v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/

static void  ell_I2C_Delay(kal_uint16 delay)
{
    kal_uint16 i;
    while(delay--)
    {
        for(i = 0; i < WDT_DELAY_TIME_1MS; i++) {;} // 1msÿ��ѭ��5000 ��//modigy for(i = 0; i < 2500; i++) {} 60ƽ̨��Ҫ���5��
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
static void ell_WDT_SerialCommStart(void)
{
    GPIO_InitIO(1,WDT_SDA);
    GPIO_WriteIO(1,WDT_SDA);
    ell_I2C_Delay(WDT_DELAY_L);
    GPIO_WriteIO(1,WDT_SCL);
    ell_I2C_Delay(WDT_DATA_H);
    GPIO_WriteIO(0,WDT_SDA);
    ell_I2C_Delay(WDT_DELAY_L);
    GPIO_WriteIO(0,WDT_SCL);
    ell_I2C_Delay(WDT_DATA_H);
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
static void ell_WDT_SerialCommStop(void)
{
    GPIO_InitIO(1,WDT_SDA);
    GPIO_WriteIO(0,WDT_SCL);
    ell_I2C_Delay(WDT_DELAY_L);
    GPIO_WriteIO(0,WDT_SDA);
    ell_I2C_Delay(WDT_DATA_H);
    GPIO_WriteIO(1,WDT_SCL);
    ell_I2C_Delay(WDT_DELAY_L);
    GPIO_WriteIO(1,WDT_SDA);
    ell_I2C_Delay(WDT_DATA_H);
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
static void ell_WDT_SerialCommTxByte(kal_uint8 data, kal_bool init_io)
{
    kal_uint16 i;
    GPIO_InitIO(1,WDT_SDA);
    for(i = 8; --i>0;)
    {
        GPIO_WriteIO((data>>i)&0x01, WDT_SDA);
        ell_I2C_Delay(WDT_DELAY_L);
        GPIO_WriteIO( 1, WDT_SCL); /* high */
        ell_I2C_Delay(WDT_DATA_H);
        GPIO_WriteIO( 0, WDT_SCL); /* low */
        ell_I2C_Delay(WDT_DELAY_L);
    }
    GPIO_WriteIO((data>>i)&0x01, WDT_SDA);
    ell_I2C_Delay(WDT_DELAY_L);
    GPIO_WriteIO( 1, WDT_SCL); /* high */
    ell_I2C_Delay(WDT_DATA_H);
    GPIO_WriteIO( 0, WDT_SCL); /* low */

    if(init_io)
    {
        GPIO_InitIO(0, WDT_SDA);
    }
    ell_I2C_Delay(10);
    GPIO_WriteIO( 0, WDT_SCL);
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
static void  ell_WDT_SerialCommRxByte(kal_uint8 *data, kal_uint8 ack)
{
    kal_uint8 i = 8;
    kal_uint8 dataCache = 0;
    char Tmp[50] = {0};
    
    GPIO_InitIO(0,WDT_SDA);
    for(i = 8; i > 0; i--)
    {
        dataCache <<= 1;
        GPIO_WriteIO(1, WDT_SCL);
        ell_I2C_Delay(WDT_READ_DATA_DELAY);
        dataCache |= GPIO_ReadIO(WDT_SDA);
        GPIO_WriteIO(0, WDT_SCL);
        ell_I2C_Delay(WDT_READ_DATA_DELAY);
    }
    GPIO_InitIO(1,WDT_SDA);
    GPIO_WriteIO(ack, WDT_SDA);
    ell_I2C_Delay(WDT_READ_DATA_DELAY);
    GPIO_WriteIO(1, WDT_SCL);
    *data = (kal_uint8)dataCache;
    GPIO_WriteIO(0,WDT_SCL);
    if(dataCache != 0x75)
    {
        EllLog("ell_WDT_SerialCommRxByte:0x%x !=0x75!!!!!!152ͨѶ���쳣", dataCache);
    }
}


/******************************************************************************
 *  Function    -  WDT_Write2Byte
 *
 *  Purpose     -  Prepare the SDA and SCL for sending two bytes
 *
 *  Description - �ն˷��������ֽ�����
 *
 * modification history
 * ----------------------------------------
* v1.0  , 2011-2-28,  guojie  written
 * ----------------------------------------
 ******************************************************************************/
static void ell_WDT_Write2Byte(kal_uint8 CW, kal_uint8 data)
{
    ell_WDT_SerialCommStart();                       // send the start sequence
    ell_WDT_SerialCommTxByte(CW, 0);               // data to be written
    ell_WDT_SerialCommTxByte(data, 1);             // data to be written
    //EllLog("ELL write:%x,%x", CW, data);

    ell_WDT_SerialCommStop();                        // send the stop sequence
}


/******************************************************************************
 *  Function    -  ell_WDT_WriteReadByte
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
static void ell_WDT_WriteReadByte(kal_uint8 data1, kal_uint8 data2, kal_uint8 *data)
{
    ell_WDT_SerialCommStart();                       // send the start sequence
    ell_WDT_SerialCommTxByte(data1, 0);           //data to be written
    ell_WDT_SerialCommTxByte(data2, 1);           // data to be written
    ell_WDT_SerialCommRxByte(data, 0);        // read data and send ACK
    //EllLog( "ELL write:%x,%x,read:%x", data1, data2, *data);
    ell_WDT_SerialCommStop();                        // send the stop sequence
}
/******************************************************************************
 *  Function    -  ell_WDT_SerialCommON
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
static void ell_WDT_SerialCommON(void)
{
    EllLog("ell_WDT_SerialCommON");
    ell_WDT_WriteReadByte(0XAA, 0X56, &RecBuf[0]);
    ell_start_timer(ELL_WATCH_DOG_TIMER_ID, 5000, ell_WDT_SerialCommON);
}


kal_uint8 ell_WDT_SerialCommEnable(void)
{
    static kal_uint8 enRecBuf = 0;
    enRecBuf = 0;
    ell_WDT_WriteReadByte(0XAC, 0X54, &enRecBuf);
    return enRecBuf;
}
void ell_WDT_SerialCommInit(void)
{
    GPIO_ModeSetup(WDT_SCL, 0);
    GPIO_ModeSetup(WDT_SDA, 0);
    GPIO_InitIO(1, WDT_SCL);
    //ell_start_timer(ELL_WATCH_DOG_TIMER_ID, 2000, ell_WDT_SerialCommON);
}
#endif /* __WDT_PLUSE__ */
#endif /* __WATCH_DOG__ */
/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/

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
void ell_watch_dog_main(U8 par)
{
#if defined( __WDT_PLUSE__)
    if(par == 0)
    {
        ell_stop_timer(ELL_WATCH_DOG_TIMER_ID);
    }
    else
    {
        GPIO_ModeSetup(WATCH_DOG_GPIO, 0);
        GPIO_InitIO(1, WATCH_DOG_GPIO);
        watch_dog_high();
    }
#elif defined(__WDT_USE_I2C__)
    EllLog("ell_WDT_SystemOn_Init ELL");
    GPIO_ModeSetup(WDT_SCL, 0);
    GPIO_ModeSetup(WDT_SDA, 0);
    GPIO_InitIO(1, WDT_SCL);
    ell_start_timer(ELL_WATCH_DOG_TIMER_ID, 2000, ell_WDT_SerialCommON);
#endif /* __WATCH_DOG__ */
}

