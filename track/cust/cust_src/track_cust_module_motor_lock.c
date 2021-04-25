#if !defined(__NF2318__)
/*******************************************************************************************
 * Filename:
 * Author :
 * Date :
 * Comment:
 ******************************************************************************************/

/****************************************************************************
*  Include Files                        ����ͷ�ļ�
*****************************************************************************/
#include "track_cust.h"

/*****************************************************************************
*  Define                               �궨��
******************************************************************************/

/*****************************************************************************
*  Typedef  Enum                        ö�ٶ���
*****************************************************************************/

/*****************************************************************************
*  Struct                               ���ݽṹ����
******************************************************************************/


/*****************************************************************************
*  Local variable                       �ֲ�����
*****************************************************************************/
static kal_uint8 g_lock_status = 0;   // 0 ���������������  1 �����������  2 ׼���˳������
static kal_uint8 g_acc_status = 0;
static kal_bool  g_req_end = KAL_FALSE;
/****************************************************************************
*  Global Variable                      ȫ�ֱ���
*****************************************************************************/

/****************************************************************************
*  Global Variable - Extern             ����ȫ�ֱ���
*****************************************************************************/

/*****************************************************************************
*  Global Functions - Extern            �����ⲿ����
******************************************************************************/
extern kal_uint8 track_cust_status_acc(void);
//extern kal_uint8 track_cust_motor_lock(kal_int8 value);

/*****************************************************************************
*  Local Functions Define               ���غ�������
******************************************************************************/
static void lock_change_from_timer(void *arg);
//void track_motor_lock_change_status(track_enum_motor par);
#if 0
#endif /* 0 */
/*****************************************************************************
*  Local Functions                      ���غ���
******************************************************************************/

/******************************************************************************
 *  Function    -  lock_change
 *
 *  Purpose     -  �����������
 *
 *  Description -  
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
static void lock_change(track_enum_motor par)
{
    LOGD(L_APP, L_V6, "�����״̬�仯 %d,%d", par, g_lock_status);
    switch(par)
    {
        case MOTOR_UNLOCK:
        {
            if(g_lock_status == 0)    
            {
                return;
            }
            if(track_cust_status_charger() == 0)
            {
                LOGD(L_APP, L_V4, "ERROR: ��类�Ͽ������������ �˳�");
                track_cust_motor_lock(0);
                return;
            }
            if(g_acc_status == 0)
            {
                LOGD(L_APP, L_V5, "�����������ȡ������������������̣���Ϊ��ǰ��ƽ�Ѿ��ǵ͵�ƽ�����ڽ���״̬");
            }
            else
            {
                LOGD(L_APP, L_V5, "�������ȴ� ACC�� ״̬ȷ�� �����ɹ�");
                // �������ȴ� ACC�� ״̬ȷ�� (�����Ƿ���ֽ�����ACC����Ϊ�͵����)
                track_cust_motor_lock(0);
                track_start_timer(
                    TRACK_CUST_MODULE_MOTOR_LOCK_STATUS_OVERTIME_TIMER_ID, 
                    2000, 
                    lock_change_from_timer, 
                    (void *)MOTOR_END1);
            }
        }
        break;
        
        case MOTOR_LOCK:
        {
            if(g_lock_status == 0)    
            {
                return;
            }
            if(track_cust_status_charger() == 0)
            {
                LOGD(L_APP, L_V4, "ERROR: ��类�Ͽ������������ �˳�");
                track_cust_motor_lock(0);
                return;
            }
            if(g_acc_status == 1)
            {
                LOGD(L_APP, L_V5, "���������ȡ������������������̣���Ϊ��ǰ��ƽ�Ѿ��Ǹߵ�ƽ�����������״̬");
                lock_change(MOTOR_END2);
            }
            else
            {
                LOGD(L_APP, L_V5, "��������ȴ� ACC�� ״̬ȷ�� ���ɹ�");
                // ����������ڵ�����ܳ�ʱ����������ᵼ�µ���𻵣������������������
                track_cust_motor_lock(1);
                track_start_timer(
                    TRACK_CUST_MODULE_MOTOR_LOCK_STATUS_OVERTIME_TIMER_ID, 
                    2000, 
                    lock_change_from_timer, 
                    (void *)MOTOR_END3);
            }
        }
        break;
        
        case MOTOR_END1: // ����ʧ�ܣ������ж��ǵ�ǰԿ�׿����������˳����������
        {
            if(track_cust_status_charger() != 0)
            {
                track_cust_motor_lock(0);
                // ��ֹ���������
                track_stop_timer(TRACK_CUST_MODULE_MOTOR_LOCK_STATUS_OVERTIME_TIMER_ID);
                track_stop_timer(TRACK_CUST_MODULE_MOTOR_LOCK_CINTINUE_TIMER_ID);
                track_stop_timer(TRACK_CUST_MODULE_MOTOR_LOCK_TIMER_ID);
                g_lock_status = 0;
                LOGD(L_APP, L_V5, "Կ�ײ��� ��������� ����");
                acc_status_high();
                return;
            }
        }
        case MOTOR_END2: // ������֮ǰ�Ѿ���⵽ACC�ߣ������ж���ǰԿ�׿���
        case MOTOR_END3: // ������쳣���ж��Ƿ�δ�����
        case MOTOR_END4: // �񶯴��������������ʱ��������������̽���
        case MOTOR_END5: // ACC�� ��ǰ����������Ѿ������Ϊ��Ҫ�˳�
        case MOTOR_END6: // ACC�� ��ǰ����������Ѿ������Ϊ��Ҫ�˳�
        {
            if(track_cust_motor_lock(99) == 1)
            {
                track_cust_motor_lock(0);
                g_req_end = KAL_TRUE;
            }
        }
        case MOTOR_END7: // ������������̣����̽����������������δ�ٴμ�⵽�񶯱���ʱ����
        {
            if(g_acc_status)
            {
                LOGD(L_APP, L_V5, "���� ��������̽��� %d", par);
                g_req_end = KAL_TRUE;
            }
            else
            {
                LOGD(L_APP, L_V5, "��������� ����");
                g_lock_status = 0;
            }
            track_cust_motor_lock(0);
            // ��ֹ���������
            track_stop_timer(TRACK_CUST_MODULE_MOTOR_LOCK_STATUS_OVERTIME_TIMER_ID);
            track_stop_timer(TRACK_CUST_MODULE_MOTOR_LOCK_CINTINUE_TIMER_ID);
            track_stop_timer(TRACK_CUST_MODULE_MOTOR_LOCK_TIMER_ID);
        }
        break;
    }
}

/******************************************************************************
 *  Function    -  lock_change_from_timer
 *
 *  Purpose     -  ��ʱ�ĺ�������
 *
 *  Description -  
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
static void lock_change_from_timer(void *arg)
{
    kal_uint32 par = (kal_uint32)arg;
    LOGD(L_APP, L_V5, "%d", par);
    lock_change((track_enum_motor)par);
}

/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/

/******************************************************************************
 *  Function    -  track_motor_lock_change_status
 *
 *  Purpose     -  ������������ýӿ�
 *
 *  Description -  
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_motor_lock_change_status(track_enum_motor par)
{
    if(G_parameter.motor_lock.sw == 0)
    {
        return;
    }

    LOGD(L_APP, L_V6, "������ⲿ���� %d,%d", par, g_lock_status);

    switch(par)
    {
        case MOTOR_SENSOR_IND:
        {
            if(g_lock_status == 0)
            {
                if(G_parameter.motor_lock.mode == 1 && g_acc_status == 0 && track_cust_status_defences() == 2)
                {   // ���״̬�£��쵽�����������
                    track_motor_lock_change_status(MOTOR_REQ);
                }
            }
        }
        break;
        
        case MOTOR_ACC_LOW:
        {
            g_acc_status = 0;
            if(g_lock_status == 0)
            {
                return;
            }
            if(g_req_end)
            {
                g_lock_status = 0;
                lock_change(MOTOR_END5);
                LOGD(L_APP, L_V5, "���������ʱ�䵽 �˳�");
                return;
            }
            if(track_cust_status_charger() == 0)
            {
                LOGD(L_APP, L_V4, "ERROR: ��类�Ͽ������������ �˳�");
                track_cust_motor_lock(0);
                return;
            }
            // �ȴ� ACC�� ֪ͨ��������ʱ���
            LOGD(L_APP, L_V5, "�����ɹ�����ǰ�յ�ACC�͵��źţ���ʱ%d���������", G_parameter.motor_lock.lock_off_time);
            track_stop_timer(TRACK_CUST_MODULE_MOTOR_LOCK_STATUS_OVERTIME_TIMER_ID);
            track_start_timer(
                TRACK_CUST_MODULE_MOTOR_LOCK_TIMER_ID, 
                G_parameter.motor_lock.lock_off_time, 
                lock_change_from_timer, 
                (void *)MOTOR_LOCK);
        }
        break;

        case MOTOR_ACC_HIGH:
        {
            g_acc_status =1;
            if(g_lock_status == 0)
            {
                return;
            }
            if(g_req_end)
            {
                g_lock_status = 0;
                lock_change(MOTOR_END6);
                LOGD(L_APP, L_V5, "���������ʱ�䵽 �˳�");
                return;
            }
            if(track_cust_status_charger() == 0)
            {
                LOGD(L_APP, L_V4, "ERROR: ��类�Ͽ������������ �˳�");
                track_cust_motor_lock(0);
                return;
            }

            // �ڼ��ʱ���ڳ���ACC�ߣ�ȷ�����û�Կ�׽�����ֹͣ����������Ķ���
            LOGD(L_APP, L_V5, "���ɹ�����ǰ�յ�ACC�ߵ��ź� ��ʱ%d����������", G_parameter.motor_lock.lock_on_time);
            // �����������״̬�£�����ACC�����
            track_stop_timer(TRACK_CUST_MODULE_MOTOR_LOCK_STATUS_OVERTIME_TIMER_ID);
            track_start_timer(
                TRACK_CUST_MODULE_MOTOR_LOCK_TIMER_ID, 
                G_parameter.motor_lock.lock_on_time, 
                lock_change_from_timer, 
                (void *)MOTOR_UNLOCK);
        }
        break;

        case MOTOR_REQ:
        {
            // �������������
            if(track_cust_status_charger() == 0)
            {
                LOGD(L_APP, L_V4, "ERROR: ���δ����");
                return;
            }
            else if(track_cust_status_defences() != 2)
            {
                LOGD(L_APP, L_V4, "ERROR: δ�������״̬");
                return;
            }
            else if(g_acc_status == 1)
            {
                LOGD(L_APP, L_V4, "ERROR: ACC���ڸ�");
                return;
            }
            if(g_lock_status == 0)
            {
                g_lock_status = 1;
                g_req_end = KAL_FALSE;
                LOGD(L_APP, L_V5, "�������������");
                lock_change(MOTOR_LOCK);
                track_start_timer(
                    TRACK_CUST_MODULE_MOTOR_LOCK_CINTINUE_TIMER_ID, 
                    G_parameter.motor_lock.lock_continue_time * 1000, 
                    lock_change_from_timer, 
                    (void *)MOTOR_END7);
            }
            else
            {
                LOGD(L_APP, L_V5, "Sensor �ӳ� ���������");
                // ����������У���⵽�񶯣����������ʱ��˳��
                track_start_timer(
                    TRACK_CUST_MODULE_MOTOR_LOCK_CINTINUE_TIMER_ID, 
                    G_parameter.motor_lock.lock_continue_time * 1000, 
                    lock_change_from_timer, 
                    (void *)MOTOR_END4);
            }
        }
        break;

        case MOTOR_END_REQ:
        {
            LOGD(L_APP, L_V5, "������������̽���");
            lock_change(MOTOR_END7);
        }
        break;
    }
}

/******************************************************************************
 *  Function    -  track_cust_motor_lock_status_check
 *
 *  Purpose     -  �����״̬��ѯ
 *
 *  Description -  
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
kal_bool track_cust_motor_lock_status_check(void)
{
    if(g_lock_status == 0 || G_parameter.motor_lock.sw == 0)
    {
        return KAL_FALSE;
    }
    else
    {
        return KAL_TRUE;
    }
}
#endif
