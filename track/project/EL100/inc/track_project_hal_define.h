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

//�ж�ΪGPIO_EINT_NO_USE�Ŀ���ʡ��
//acc
#define ACC_EINT_NO             12
//SOS����
#define EXTRA_SOS_EINT_NO       GPIO_EINT_NO_USE
//β�߼���ж�
#define EXTRA_IN_1_EINT_NO        GPIO_EINT_NO_USE
#define EXTRA_IN_2_EINT_NO        GPIO_EINT_NO_USE
//��Ƭ������
#define EXTRA_WAKEUP_EINT_NO    GPIO_EINT_NO_USE
//�豸��ͨ����
#define EXTRA_A_KEY_EINT_NO     GPIO_EINT_NO_USE
//����
#define AUX_EINT_NO             GPIO_EINT_NO_USE
//��ҳ��charge in
#define EXT_CHARGEIN_EINT_NO 15

#define EXT_HALL_EINT_NO 2
#define LOCK_CHEKC_EINT1 18
#define LOCK_CHEKC_EINT2 17


/*���ĸ��Ǳ�����*/
#define EINT_E_NO LOCK_CHEKC_EINT2

#define EINT_A_NO  ACC_EINT_NO
#define EINT_B_NO  EXT_HALL_EINT_NO
#define EINT_C_NO  EXT_CHARGEIN_EINT_NO
#define EINT_D_NO  LOCK_CHEKC_EINT1
/*3���ж϶�Ӧ��IO ��*/
#define EINT_A_GPIO 14
#define EINT_B_GPIO 2
#define EINT_C_GPIO 25
#define EINT_D_GPIO 35
#define EINT_E_GPIO 34

/*4��IO�ڵ��ж�ģʽֵ�в���*/
//nc-0
#define EINT_A_GPIO_MODE    0
#define EINT_A_EINT_MODE    2
#define EINT_B_GPIO_MODE    0
#define EINT_B_EINT_MODE    1
#define EINT_C_GPIO_MODE    0
#define EINT_C_EINT_MODE    4
#define EINT_D_GPIO_MODE    0
#define EINT_D_EINT_MODE    2
#define EINT_E_GPIO_MODE    0
#define EINT_E_EINT_MODE    2

#define CUT_OIL_ELE_GPIO  GPIO_EINT_NO_USE

//��ҳ��������
#define EXT_CHEK_FULLCHARGE_GPIO  27

//��ҳ��������
#define EXT_CHEK_FULLCHARGE_GPIO  27

#define BUZZER_GPIO  36

#endif  /* _TRACK_PROJECT_HAL_DEFINE_H */

