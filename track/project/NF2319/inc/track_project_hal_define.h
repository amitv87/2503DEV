/******************************************************************************
 * track_project_hal_define.h - 
 * 
 * Copyright .
 * 
 * DESCRIPTION: - 
 *        ����Ŀ��ͬ��Ӳ�����ú궨���ļ�,ÿ����Ŀ������������ͬ���ļ������������� 
 * 
 * modification history
 * --------------------
 * v1.0   2017-07-11,  chengjun create this file
 * 
 ******************************************************************************/
#ifndef _TRACK_PROJECT_HAL_DEFINE_H
#define _TRACK_PROJECT_HAL_DEFINE_H


/*****************************************************************************
 *  Define					�궨��
 *****************************************************************************/
/*��ʹ�õ��жϻ�GPIO�ڣ�����Ϊ0xFF��������Զ����ζ����Ĳ���      --    chengjun  2017-07-10*/
#define GPIO_EINT_NO_USE    0xFF


//152���Ź�
#define WATCH_DOG_GPIO  28
#define SADDLE_LOCK_GPIO  48
//������
#define EXBAT_CONTROL_GPIO  50

//�ж�ΪGPIO_EINT_NO_USE�Ŀ���ʡ��
//acc
#define ACC_EINT_NO             GPIO_EINT_NO_USE
//SOS����
#define EXTRA_SOS_EINT_NO       GPIO_EINT_NO_USE
//β�߼���ж�
#define EXTRA_IN_1_EINT_NO        2
#define EXTRA_IN_2_EINT_NO        12
//��Ƭ������
#define EXTRA_WAKEUP_EINT_NO    GPIO_EINT_NO_USE
//�豸��ͨ����
#define EXTRA_A_KEY_EINT_NO     GPIO_EINT_NO_USE
//����
#define AUX_EINT_NO             GPIO_EINT_NO_USE
//��ҳ��charge in
#define EXT_CHARGEIN_EINT_NO    GPIO_EINT_NO_USE
#if defined (__WHEEL_CHECK__)
//�ֶ��ź��ж�
#define EXTRA_WHEEL_EINT_NO        20
#endif /* __WHEEL_CHECK__ */

/*���ĸ��Ǳ�����*/
#define EINT_A_NO  EXTRA_IN_1_EINT_NO
#define EINT_B_NO  EXTRA_IN_2_EINT_NO
#define EINT_C_NO  GPIO_EINT_NO_USE
#define EINT_D_NO  GPIO_EINT_NO_USE

/*3���ж϶�Ӧ��IO ��*/
#define EINT_A_GPIO 2
#define EINT_B_GPIO 14
#define EINT_C_GPIO GPIO_EINT_NO_USE
#if defined (__WHEEL_CHECK__)
#define EXTRA_WHEEL_GPIO 46
#endif /* __WHEEL_CHECK__ */
/*4��IO�ڵ��ж�ģʽֵ�в���*/
//nc-0
#define EINT_A_GPIO_MODE    0
#define EINT_A_EINT_MODE    1
#define EINT_B_GPIO_MODE    0
#define EINT_B_EINT_MODE    2
#define EINT_C_GPIO_MODE    0
#define EINT_C_EINT_MODE    0
#if defined (__WHEEL_CHECK__)
#define EINT_WHEEL_GPIO_MODE    0
#define EINT_WHEEL_EINT_MODE    2
#endif /* __WHEEL_CHECK__ */

#define CUT_OIL_ELE_GPIO  GPIO_EINT_NO_USE

#define BUZZER_GPIO  GPIO_EINT_NO_USE

#endif  /* _TRACK_PROJECT_HAL_DEFINE_H */

