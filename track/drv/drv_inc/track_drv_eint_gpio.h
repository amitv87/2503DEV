/******************************************************************************#ifndef  EXT_CHEK_FULLCHARGE_GPIO
#define EXT_CHEK_FULLCHARGE_GPIO GPIO_EINT_NO_USE
#endif
 * track_interrupt.h -
 *
 * Copyright .
 *
 * DESCRIPTION: -
 *        �жϿ�/GPIO  ����
 *
 * modification history
 * --------------------
 * v1.0   2012-07-26,  chengjun create this file
 *
 ******************************************************************************/

#ifndef _TRACK_DRV_EINT_GPIO_H
#define _TRACK_DRV_EINT_GPIO_H

/****************************************************************************
* Include Files                   		����ͷ�ļ�
*****************************************************************************/
#include "Eint.h"
#include "intrCtrl.h"
#include "kal_public_defs.h"
#include "track_os_data_types.h"
#include "track_project_hal_define.h"

/****************************************************************************
* Global Variable - Extern          ����ȫ�ֱ���
*****************************************************************************/


/*****************************************************************************
 *  Define					�궨��
 *****************************************************************************/

/*Ĭ��Ϊ��ƽ����ģʽ*/
/*����: �޸�ΪEDGE_SENSITIVE ģʽ������ϸ�鿴���룬��Ҫ�޸�����*/
#define EINT_SENSITIVE_MODE  LEVEL_SENSITIVE

/*Ĭ���������200ms  */
//unit 10ms
#define EINT_SW_DEBOUNCE_TIME  20          
#define EINT_SW_DEBOUNCE_20MS  2

/*-----  GPIO ����end----*/

#ifndef  EINT_A_NO
#define EINT_A_NO GPIO_EINT_NO_USE
#endif

#ifndef  EINT_B_NO
#define EINT_B_NO GPIO_EINT_NO_USE
#endif

#ifndef  EINT_C_NO
#define EINT_C_NO GPIO_EINT_NO_USE
#endif

#ifndef  EINT_D_NO
#define EINT_D_NO GPIO_EINT_NO_USE
#endif

#ifndef  EINT_E_NO
#define EINT_E_NO GPIO_EINT_NO_USE
#endif

#ifndef  CUT_OIL_ELE_GPIO
#define CUT_OIL_ELE_GPIO GPIO_EINT_NO_USE
#endif

#ifndef  CUST_OUT1_GPIO
#define CUST_OUT1_GPIO GPIO_EINT_NO_USE
#endif

#ifndef  CTRL_5V_OUT_GPIO
#define CTRL_5V_OUT_GPIO GPIO_EINT_NO_USE
#endif

#ifndef  EXT_POWER_GPIO
#define EXT_POWER_GPIO GPIO_EINT_NO_USE
#endif

#if defined(__NT55__)
#ifndef OBD_POWER_GPIO
#define OBD_POWER_GPIO 25
#endif
#endif

#ifndef  BUZZER_GPIO
#define BUZZER_GPIO GPIO_EINT_NO_USE
#endif

/*****************************************************************************
* Typedef  Enum
*****************************************************************************/


/*****************************************************************************
 *  Struct					���ݽṹ����
 *****************************************************************************/
typedef struct
{
    LOCAL_PARA_HDR
    U8 eint;
    U8 level;
} eint_status_struct;

/*****************************************************************************
* Local variable				�ֲ�����
*****************************************************************************/


/*****************************************************************************
* Golbal variable				ȫ�ֱ���
*****************************************************************************/




/*****************************************************************************
 *  Global Functions	- Extern		�����ⲿ����
 *****************************************************************************/


/*****************************************************************************
 *  Local Functions			���غ���
 *****************************************************************************/
extern void track_eint_B_registration(void);


/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/
extern void track_drv_eint_init_registration(void);
extern kal_int8 track_drv_gpio_set(kal_int8 state, kal_int8 port);

extern void track_drv_set_vcama(kal_bool ON,/* kal_uint32 ldo,*/ kal_uint32 ldo_volt);

#endif  /* _TRACK_DRV_EINT_GPIO_H */

