/******************************************************************************
 * track_led_driver.h -
 *
 * Copyright .
 *
 * DESCRIPTION: -
 *        LED ����
 *
 * modification history
 * --------------------
 * v1.0   2012-07-22,  chengjun create this file
 *
 ******************************************************************************/

#ifndef _TRACK_DRV_LED_H
#define _TRACK_DRV_LED_H


/****************************************************************************
* Include Files                   		����ͷ�ļ�
*****************************************************************************/
#include "track_os_data_types.h"
#include "dcl.h"
#include "dcl_pmu.h"
#include "track_os_timer.h"

/*****************************************************************************
 *  Define					�궨��
 *****************************************************************************/
//#define __LED_DEBUG__
#define LED_LOOP_Max 20

#if defined (__GT300S__)||defined (__QB200__)
#define GPIO_G_LED_NO 48
#define GPIO_B_LED_NO 47
#elif defined(__EL100__)
#define GPIO_R_LED_NO 50
#define GPIO_G_LED_NO 48
#define GPIO_B_LED_NO 47
#elif defined(__NT31__) || defined(__MT200__)||defined(__ET310__) ||defined(__NT36__)
#define GPIO_G_LED_NO 25
#elif defined (__GT370__)|| defined (__GT380__)
#define GPIO_R_LED_NO 27
#elif defined (__NF2319__)||defined (__NF2318__)
#define GPIO_B_LED_NO 40
#endif
/*****************************************************************************
* Typedef  Enum
*****************************************************************************/


/*****************************************************************************
 *  Global Functions	- Extern		�����ⲿ����
 *****************************************************************************/

/*****************************************************************************
 *  Struct					���ݽṹ����
 *****************************************************************************/
typedef struct
{
    kal_bool valid;
    kal_bool cycleCountVilid;
    kal_bool status;
    kal_uint32 delayStart;
    kal_uint32 timeOn;
    kal_uint32 timeOff;
    kal_uint16 cycleCount;
} LED_Flash_Struct;

typedef struct
{
    kal_bool valid;
    kal_bool status[LED_Num_Max];
    kal_uint32 delay;
} led_loop_struct;

typedef struct
{
    kal_uint32 loop;
    led_loop_struct leds[LED_LOOP_Max];
    kal_hisr_func_ptr exit_function;
} led_loop_main_struct;


/******************* ����LED  �˿� *******************

GPIO_WriteIO ()    ������ LED_Init_Hardware()  �У�
����LED_SetPort_Init_GPIO( )��ʼ���˿�

******************************************************/
#if defined (__GT300S__)||defined(__QB200__)||defined (__NF2319__)||defined (__NF2318__)
#define LED_Num1_ON()           track_drv_gpio_con_led(KAL_TRUE,GPIO_B_LED_NO)
#define LED_Num1_OFF()         track_drv_gpio_con_led(KAL_FALSE,GPIO_B_LED_NO)
#elif defined (__GT370__)
#define LED_Num1_ON()           track_drv_gpio_con_led(KAL_TRUE,GPIO_R_LED_NO)
#define LED_Num1_OFF()         track_drv_gpio_con_led(KAL_FALSE,GPIO_R_LED_NO)
#elif defined (__EL100__)
#define LED_Num1_ON()           track_drv_gpio_con_led(KAL_TRUE,GPIO_R_LED_NO)
#define LED_Num1_OFF()         track_drv_gpio_con_led(KAL_FALSE,GPIO_R_LED_NO)
#elif defined (__NT55__)||defined (__JM10__)
#define LED_Num1_ON()           pmu_drv_bl_isink(ISINK0,KAL_TRUE)
#define LED_Num1_OFF()          pmu_drv_bl_isink(ISINK0,KAL_FALSE)
#else
#define LED_Num1_ON()           pmu_drv_bl_isink(ISINK0,KAL_TRUE)
#define LED_Num1_OFF()          pmu_drv_bl_isink(ISINK0,KAL_FALSE)
#endif

#if defined(__NT55__)||defined(__NT13V__)||defined (__JM10__)
#define LED_Num2_ON()           track_drv_led_vcama(KAL_TRUE)
#define LED_Num2_OFF()         track_drv_led_vcama(KAL_FALSE)
#elif defined (__EL100__)
#define LED_Num2_ON()           track_drv_gpio_con_led(KAL_TRUE,GPIO_G_LED_NO)
#define LED_Num2_OFF()         track_drv_gpio_con_led(KAL_FALSE,GPIO_G_LED_NO)
#elif defined (__NF2319__)||defined (__NF2318__)
#define LED_Num2_ON()           pmu_drv_bl_isink(ISINK0,KAL_TRUE)
#define LED_Num2_OFF()          pmu_drv_bl_isink(ISINK0,KAL_FALSE)
#else
#define LED_Num2_ON()           pmu_drv_bl_isink(ISINK1,KAL_TRUE)
#define LED_Num2_OFF()         pmu_drv_bl_isink(ISINK1,KAL_FALSE)//null
#endif /* __GT740__ */

#if defined(__NT55__)||defined (__NF2319__)||defined (__NF2318__)
#define LED_Num3_ON()          track_drv_kpled(ISINK2,KAL_TRUE)
#define LED_Num3_OFF()          track_drv_kpled(ISINK2,KAL_FALSE)
#elif defined (__EL100__)
#define LED_Num3_ON()           track_drv_gpio_con_led(KAL_TRUE,GPIO_B_LED_NO)
#define LED_Num3_OFF()         track_drv_gpio_con_led(KAL_FALSE,GPIO_B_LED_NO)
#elif defined (__NT33__)||defined(__NT13V__)
#define LED_Num3_ON()          track_drv_kpled(ISINK2,KAL_TRUE)
#define LED_Num3_OFF()          track_drv_kpled(ISINK2,KAL_FALSE)
#elif defined(__NT31__) ||defined(__MT200__)||defined(__ET310__) ||defined(__NT36__)||defined(__QB200__)
#define LED_Num3_ON()          track_drv_gpio_con_led(KAL_TRUE,GPIO_G_LED_NO)
#define LED_Num3_OFF()          track_drv_gpio_con_led(KAL_FALSE,GPIO_G_LED_NO)
#else
#define LED_Num3_ON()          pmu_drv_bl_isink(ISINK2,KAL_TRUE)
#define LED_Num3_OFF()          pmu_drv_bl_isink(ISINK2,KAL_FALSE)
#endif
#define LED_Num4_ON()
#define LED_Num4_OFF()
#define LED_Num5_ON()
#define LED_Num5_OFF()

/******************LED  �� *******************/
#if defined(__MT200__)||defined(__ET310__)||defined(__EL100__)
#define LED_PWR             LED_Num2
#define LED_GSM             LED_Num3
#define LED_GPS             LED_Num1
#elif defined(__NT55__)
#define LED_PWR             LED_NULL
#define LED_GPS             LED_Num1
#define LED_BT              LED_Num2
#define LED_GSM             LED_Num3
#elif defined(__NT13V__)
#define LED_PWR             LED_Num2
#define LED_GSM             LED_Num3
#define LED_GPS             LED_Num1
#elif defined(__NT17__)
#define LED_PWR             LED_Num2
#define LED_GSM             LED_Num1
#define LED_GPS             LED_Num3
#elif defined(__QB200__)
#define  LED_GPS            LED_Num1
#define  LED_GSM            LED_NULL
#define  LED_PWR            LED_Num3
#elif defined(__EL100__)
#define LED_PWR             LED_Num1
#define LED_GSM             LED_Num2
#define LED_PWR             LED_Num3
//led define error  -- chengjun
#elif defined(__NF2319__)||defined (__NF2318__)
#define LED_PWR             LED_Num3
#define LED_GSM             LED_Num2
#define LED_GPS             LED_Num1
#elif defined (__JM10__)
#define LED_PWR             LED_Num2
#define LED_GSM             LED_Num1
#define LED_GPS             LED_NULL

#else
#error "please define project_null"
#define LED_SOS             LED_NULL
#define LED_Family1         LED_NULL
#define LED_Family2         LED_NULL
#define LED_Family3         LED_NULL
#define LED_Family4         LED_NULL
#endif


#define LED_GREEN     LED_NULL
#define LED_RED     LED_NULL
#define LED_YELLOW     LED_NULL


#ifndef LED_BT
#define LED_BT     LED_NULL
#endif

#ifndef LED_OBD
#define LED_OBD     LED_NULL
#endif

typedef enum
{
    LedMode_OFF,                //����
    LedMode_ON,                 //LED_ON �ѱ�ϵͳʹ��
    LedMode_FastFlash,       //��0.2s  :   ��0.2s
    LedMode_MedFlash,       //��1s    :  ��1s
    LedMode_SlowFlash,      //��0.2s    :  ��2s
    LedMode_Custom,         //�Զ���
    LedMode_Max,
} LED_Mode;



/*****************************************************************************
 *  Global Functions			ȫ�ֱ���
 *****************************************************************************/


/*****************************************************************************
 *  Local Functions			���غ���
 *****************************************************************************/
extern void track_drv_led_switch(LED_Type Type, kal_bool on);
extern void LED_Set_Flash(LED_Type Type, kal_uint32 timeOn, kal_uint32 timeOff);

/*****************************************************************************
 *  Global Functions			ȫ�ֺ���
 *****************************************************************************/
extern void LED_SetPort_Init_GPIO(U8 port);
extern void track_drv_led_set_status(LED_Type Type, LED_Mode Mode);
extern void track_drv_led_set_all_off(void);
extern void track_drv_led_set_all_on(void);
extern void track_drv_led_by_power(LED_Type led, kal_bool on);
#endif  /* _TRACK_DRV_LED_H */

