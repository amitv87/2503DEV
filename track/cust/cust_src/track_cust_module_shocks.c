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
#include "track_cust_platform_N07.h"

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
static struct_msg_send g_msg = {0};
#if defined(__GERMANY__)
static char* msg_alarm_vibrates_en = "Vibrationssensor Alarm!";
static char* msg_alarm_vibrates_en1 = "Vibrationssensor Alarm!";
#else
static char* msg_alarm_vibrates_en = "Vibration Alarm!";
static char* msg_alarm_vibrates_en1 = "Vibration Alarm!Please pay attention!";
#endif

static kal_uint8 msg_alarm_vibrates_cn[10] =   // �𶯱�����
{
    0x97, 0x07, 0x52, 0xA8, 0x62, 0xA5, 0x8B, 0x66, 0xFF, 0x01
};
static kal_uint8 msg_alarm_vibrates_cn1[18] =   // �𶯱��������ע!
{
    0x97, 0x07, 0x52, 0xA8, 0x62, 0xA5, 0x8B, 0x66, 0xFF, 0x01, 0x8B  , 0xF7  , 0x51  , 0x73 , 0x6C  , 0xE8  , 0xFF , 0x01

};
static track_gps_data_struct gps_data_buf = {0};
/****************************************************************************
*  Global Variable                      ȫ�ֱ���
*****************************************************************************/

/****************************************************************************
*  Global Variable - Extern             ����ȫ�ֱ���
*****************************************************************************/
extern kal_uint8 g_alarm_gps_status;//������¼�����ڼ�GPS��λ���//0,����λ;1,��λ

/*****************************************************************************
*  Global Functions - Extern            �����ⲿ����
******************************************************************************/
extern void track_instructions_get_paging(void);//20190916
extern void track_cust_vibrates_voice_alarm(void* arg);
/*****************************************************************************
*  Local Functions Define               ���غ�������
******************************************************************************/
static void alarm_vibrates_valid_lid(void);

#if 0
#endif /* 0 */
/*****************************************************************************
*  Local Functions                      ���غ���
******************************************************************************/
static void track_cust_senalm_lock_overtime()
{
	;//track_acc_status((void *) 0);
}
kal_uint8 track_cust_ele_lock(void* param)
{
	kal_uint8 arg = (kal_uint8)param;
	static kal_uint8 lock = 0;
	
    LOGD(L_BMS, L_V2, "arg=%d,lock=%d",arg,lock);
	if(arg != 99)
	{
		lock = arg;
	}else
	{
		return lock;
	}
	if(lock == 1)
	{
		track_acc_status((void *) 2);
		set_ele_lock = 1;
		//request_static_packet((void *)99);
		//������Ϊ�����̫�죬���������ĳ���ʱ��
		if(!track_is_timer_run(TRACK_CUST_CONTROLLER_DATA_TIMER_ID))
		track_cust_cotroller_alarm((void*)0);
		if(!track_is_timer_run(TRACK_CUST_SENALM_LOCK_ELE_TIMER_ID))
		{
			//tr_start_timer(TRACK_CUST_SENALM_LOCK_ELE_TIMER_ID, 8*1000, track_cust_senalm_lock_overtime);
			track_start_timer(TRACK_CUST_SENALM_LOCK_ELE_TIMER_ID, 8*1000, track_cust_ele_lock,(void*)0);
		}
		//play_voice(8);
	}
	else if(lock == 2)
	{
		track_start_timer(TRACK_CUST_SENALM_LOCK_ELE_TIMER_ID, 8*1000, track_cust_ele_lock,(void*)0);
	}
	else if(lock == 0)
	{
		//lock = 0;
		track_acc_status((void *)0);
		set_ele_lock = 0;//��������ΪʲôҪ�������
		tr_start_timer(TRACK_CUST_SENALM_LOCK_ELE_OVERTIME_TIMER_ID, 6*1000, track_cust_senalm_lock_overtime);
	}
	else if(lock == 3)
	{
		tr_stop_timer(TRACK_CUST_SENALM_LOCK_ELE_TIMER_ID);
		lock = 0;
		tr_stop_timer(TRACK_CUST_SENALM_LOCK_ELE_OVERTIME_TIMER_ID);
	}
	return lock;
}
void track_cust_senalm_handle(U8 isFirst)
{
    LOGD(L_BMS, L_V2, "�𶯱���");

	if(isFirst)
	{
		if(!track_is_timer_run(TRACK_CUST_MODULE_ALARM_VIBRATES_TIMER_ID))
		{// G_parameter.vibrates_alarm.alarm_interval
                    tr_start_timer(
            TRACK_CUST_MODULE_ALARM_VIBRATES_TIMER_ID,
           G_parameter.vibrates_alarm.alarm_interval*1000,
            alarm_vibrates_valid_lid);
			track_cust_driver_behaviour();//�����𶯣�ȡ��ǰ��sensor����
			//tr_stop_timer(TRACK_CUST_SENSOR_READ_TIMER_ID);
			//�𶯱����ϴ�Ҫ�о�Ĭ�����������������ÿ�ζ�����
			track_cust_N07_paket_04(ALARM_VIB, 0x01);
			track_cust_paging_command();//�ϴ���λ��
			//track_instructions_get_paging();
		}
		if(track_cust_ele_lock((void*)99) == 0 && !track_is_timer_run(TRACK_CUST_SENALM_LOCK_ELE_OVERTIME_TIMER_ID))
		{
			track_cust_ele_lock((void*)1);
		}
		//play_voice(8);

	}
	
}
#if 1
static void track_cust_void_alarm_play()
{
	//if(track_set_audio_file_play(99) == 0)
	//{
	play_voice(8);
	//}
	track_start_timer(TRACK_CUST_SENALM_VOICE_OVERTIME_TIMER_ID, 11*1000, track_cust_vibrates_voice_alarm,(void*)2);
}
void track_cust_vibrates_voice_alarm(void* arg)
{
    static U8 shock_tmp = 0;
	kal_uint8 mode  = (kal_uint8)arg;
	
    static kal_bool valid = KAL_FALSE;
	static kal_uint8 vibratesCount = 0;
	static kal_uint32 sampling_interval_tick = 0;
	kal_uint32 tick = OTA_kal_get_systicks();
	
	kal_uint8 lease = G_parameter.ste_status.lease;

	LOGD(L_BMS, L_V2, "�������� mode:%d,lease:%d,tick:%d,sampling_interval_tick:%d,vibratesCount:%d,%d",mode,lease,tick,sampling_interval_tick,vibratesCount,G_parameter.viblvoicesw);
	if(G_parameter.viblvoicesw == 0)
    {
        return;
    }
	if(mode == 1 && valid &&lease == 0 && tick>sampling_interval_tick)
	{
		if((G_realtime_data.defences_status == 1)) return;//�����������������
		#if 0//��ȥ�� 2019-7-11 Liujw
	    shock_tmp = track_cust_get_shock();
		if(((shock_tmp&2) == 2 || (shock_tmp & 8) == 8))// && track_is_timer_run(TRACK_CUST_VEHICLE_LEASE_OFF_NO_SHOCK_TIMER_ID)
		{
			LOGD(L_APP, L_V5, "3-��ʱ���ĳ����������������,%d", shock_tmp);
			return;
		}
		#endif
		sampling_interval_tick = tick + OTA_kal_secs_to_ticks(G_parameter.vibrates_alarm.sampling_interval) - KAL_TICKS_100_MSEC * 2;
		vibratesCount ++;
		if(vibratesCount == 1)
		{
			track_start_timer(TRACK_CUST_SENALM_VOICE_TIMER_ID, G_parameter.vibrates_alarm.detection_time * 1000, track_cust_vibrates_voice_alarm, (void * )2);
		}
		if(vibratesCount >= G_parameter.ste_param.senspkset && track_set_audio_file_play(99) == 0)
		{
			LOGD(L_BMS, L_V2, "����������������");
            valid = KAL_FALSE;
            vibratesCount = 0;
			//if(track_is_timer_run(TRACK_CUST_SENALM_VOICE_OVERTIME_TIMER_ID))
			//{
			//	LOGD(L_BMS, L_V2, "����������");
			//}
			track_cust_void_alarm_play();
		}
	}
	else if(mode == 2 && lease == 0)
	{
		valid = KAL_TRUE;
	    vibratesCount = 0;
	}
	else if(mode == 3)
	{
		valid = KAL_FALSE;
	    vibratesCount = 0;
	}
}
#endif
/******************************************************************************
 *  Function    -  alarm_vibrates_valid_lid
 *
 *  Purpose     -  ��Ĭʱ�䵽
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
static void alarm_vibrates_valid_lid(void)
{
    LOGD(L_BMS, L_V2, "�񶯱����ľ�Ĭʱ�� ����");
#if defined(__NF2318__)
	if(G_parameter.ste_status.lease == 0)
	{
		track_cust_module_alarm_vibrates((void *)2);
	}
#else
#if defined(__ET310__) || defined(__MT200__)
    if(G_parameter.motor_lock.sw == 0 && (G_realtime_data.defense_mode == 1 || track_cust_status_acc() == 0))
#else
    if(G_parameter.motor_lock.sw == 0 && track_cust_status_acc() == 0)
#endif
    {
        if(track_cust_status_defences() == 1 && !track_is_timer_run(TRACK_CUST_STATUS_DEFENCES_TIMER_ID))
        {
            track_cust_status_defences_change((void*)2);
        }
        else if(track_cust_status_defences() == 2)
        {
            track_cust_module_alarm_vibrates((void *)2);
        }
    }
#endif
}

/******************************************************************************
 *  Function    -  track_cus_alarm_vibrates_overTime_sendsms
 *
 *  Purpose     -  �����񶯱�������
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cus_alarm_vibrates_overTime_sendsms(void)
{
    track_gps_data_struct *gpsLastPoint = NULL;

    LOGD(L_APP, L_V5, "%d,%d", G_parameter.vibrates_alarm.sw, G_parameter.vibrates_alarm.alarm_type);
    if(G_parameter.vibrates_alarm.sw && G_parameter.vibrates_alarm.alarm_type)
    {
        memset(&g_msg, 0, sizeof(struct_msg_send));
#if defined (__GT370__)|| defined (__GT380__)|| defined(__GT740__)||defined(__GT300S__)
        LOGD(L_APP, L_V6, "g_alarm_gps_status = %d", g_alarm_gps_status);
#if (!(defined(__GT740__) || defined(__ET310__) || defined(__MT200__)))
        if(g_alarm_gps_status == 1  || track_cust_gps_status() > 2)
#endif
        {
            gpsLastPoint = track_cust_backup_gps_data(0, NULL);
            memcpy(&gps_data_buf, gpsLastPoint, sizeof(track_gps_data_struct));
            g_alarm_gps_status = 0;
        }
        if(gpsLastPoint != NULL && gpsLastPoint->gprmc.status == 1)
        {
            g_msg.addUrl = KAL_TRUE;
        }
        else
        {
            g_msg.addUrl = KAL_FALSE;
        }
#else
        g_msg.addUrl = KAL_TRUE;
#endif
        g_msg.cm_type = CM_SMS;


        if(g_msg.addUrl == KAL_TRUE)
        {
            track_fun_strncpy(g_msg.msg_en, msg_alarm_vibrates_en, CM_PARAM_LONG_LEN_MAX);
            g_msg.msg_cn_len = 10;
            memcpy(g_msg.msg_cn, msg_alarm_vibrates_cn, g_msg.msg_cn_len);
        }
        else
        {
            track_fun_strncpy(g_msg.msg_en, msg_alarm_vibrates_en1, CM_PARAM_LONG_LEN_MAX);
            g_msg.msg_cn_len = 18;
            memcpy(g_msg.msg_cn, msg_alarm_vibrates_cn1, g_msg.msg_cn_len);
        }
        track_cust_module_alarm_msg_send(&g_msg, &gps_data_buf);

        if(G_parameter.vibrates_alarm.alarm_type == 2)
        {
            track_start_timer(TRACK_CUST_ALARM_alarm_vibrates_Call_TIMER, 60000, track_cust_make_call, (void*)1);
        }
        else
        {
            track_cust_module_alarm_lock(TR_CUST_ALARM_Vibration, 0);
        }
    }
    else
    {
        track_cust_module_alarm_lock(TR_CUST_ALARM_Vibration, 0);
    }
    track_alarm_vec_queue_remove(2, 0, TR_CUST_ALARM_Vibration);
}

/******************************************************************************
 *  Function    -  alarm_vibrates_ind
 *
 *  Purpose     -  ����ʱ����ʱ�䵽����������
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
static void alarm_vibrates_ind(void)
{
	;
#ifndef __NF2318__
    if(G_parameter.vibrates_alarm.sw)
    {
        track_gps_data_struct *gpsLastPoint = NULL;

        gpsLastPoint = track_cust_gpsdata_alarm();
        memcpy(&gps_data_buf, gpsLastPoint, sizeof(track_gps_data_struct));
        if(track_is_timer_run(TRACK_CUST_MODULE_ALARM_VIBRATES_TIMER_ID))
        {
            LOGD(L_APP, L_V4, "��һ�𶯱�����Ĭʱ��δ��");
            /*if(G_parameter.gprson)
            {
                gpsLastPoint = track_cust_backup_gps_data(0, NULL);
                track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_Vibration, KAL_FALSE);
            }*/
            return;
        }
        else if(track_cust_module_alarm_lock(TR_CUST_ALARM_Vibration, 2) != 0)
        {
            LOGD(L_APP, L_V4, "��һ�𶯱�������δ���");
            if(G_parameter.vibrates_alarm.alarm_interval > 0)
            {
                LOGD(L_APP, L_V4, "�����񶯱����ľ�Ĭʱ��%d��", G_parameter.vibrates_alarm.alarm_interval);
                tr_start_timer(
                    TRACK_CUST_MODULE_ALARM_VIBRATES_TIMER_ID,
                    G_parameter.vibrates_alarm.alarm_interval * 1000,
                    alarm_vibrates_valid_lid);
            }
            return;
        }
        else
        {
            g_alarm_gps_status = 0;

            if(track_cust_gps_status() > 2)
            {
                g_alarm_gps_status = 1;
            }
            else
            {
                track_cust_gps_work_req((void*)10);
                track_cust_send_paket_16(2, gpsLastPoint, 0);
            }
            if(G_parameter.gprson)
            {
                track_cust_alarm_type(TR_CUST_ALARM_Vibration);
                if(g_alarm_gps_status == 1)
                {
                    track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_Vibration, KAL_TRUE, 1);
                    tr_start_timer(
                        TRACK_CUST_ALARM_alarm_vibrates_OVERTIME_TIMER,
                        60 * 1000,
                        track_cus_alarm_vibrates_overTime_sendsms);
                }
                else
                {
                    track_cust_paket_19(track_drv_get_lbs_data(), TR_CUST_ALARM_Vibration, KAL_TRUE, 1);
                    tr_start_timer(
                        TRACK_CUST_ALARM_alarm_vibrates_OVERTIME_TIMER,
                        5 * 60 * 1000,
                        track_cus_alarm_vibrates_overTime_sendsms);
                }
            }

            if(G_parameter.vibrates_alarm.alarm_interval > 0)
            {
                LOGD(L_APP, L_V4, "�����񶯱����ľ�Ĭʱ��%d��", G_parameter.vibrates_alarm.alarm_interval);
                tr_start_timer(
                    TRACK_CUST_MODULE_ALARM_VIBRATES_TIMER_ID,
                    G_parameter.vibrates_alarm.alarm_interval * 1000,
                    alarm_vibrates_valid_lid);
            }
        }

#if defined (__NT13V__)
        track_drv_buzzer_sound_on(10);
#endif /* __NT13V__ */
        
        track_cust_module_alarm_lock(TR_CUST_ALARM_Vibration, 1);
        track_cust_led_alarm();
        if(G_parameter.vibrates_alarm.alarm_type > 0)
        {
            if(G_parameter.vibrates_alarm.alarm_type == 3)
            {
                tr_stop_timer(TRACK_CUST_ALARM_alarm_vibrates_OVERTIME_TIMER);
                track_alarm_vec_queue_remove(2, 0, TR_CUST_ALARM_Vibration);
                track_start_timer(TRACK_CUST_ALARM_alarm_vibrates_Call_TIMER, 10 * 1000, track_cust_make_call, (void*)1);
            }
            else
            {
                if(G_parameter.lang == 0 || G_parameter.gprson == 0)
                {
                    tr_stop_timer(TRACK_CUST_ALARM_alarm_vibrates_OVERTIME_TIMER);
                    track_os_intoTaskMsgQueue(track_cus_alarm_vibrates_overTime_sendsms);
                }
            }
        }
        else
        {
            track_cust_module_alarm_lock(TR_CUST_ALARM_Vibration, 0);
        }
    }
    if(!track_is_timer_run(TRACK_CUST_MODULE_ALARM_VIBRATES_TIMER_ID))
    {
        alarm_vibrates_valid_lid();
    }
#endif
}

/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/


/******************************************************************************
 *  Function    -  track_cust_module_alarm_defences
 *
 *  Purpose     -  ���״̬�仯
 *
 *  Description -  defences   0��������1��Ԥ�����2�����
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 27-10-2012,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_module_alarm_defences(kal_uint8 defences)
{
    switch(defences)
    {
        case 0:
            LOGD(L_APP, L_V4, "����");
            track_cust_module_alarm_vibrates((void *)3);
            tr_stop_timer(TRACK_CUST_MODULE_ALARM_VIBRATES_DELAY_TIMER_ID);
            tr_stop_timer(TRACK_CUST_MODULE_ALARM_VIBRATES_TIMER_ID);  // ����ͬʱҲ������Ĭʱ���ʱ
            if(G_realtime_data.defences_status != 1)
            {
                G_realtime_data.defences_status = 1;
                Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
            }
            break;

        case 1:
            LOGD(L_APP, L_V4, "Ԥ���");
            if(G_realtime_data.defences_status != 2)
            {
                G_realtime_data.defences_status = 2;
                Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
            }
            break;

        case 2:
            LOGD(L_APP, L_V4, "���");
            if(G_realtime_data.defences_status != 3)
            {
                G_realtime_data.defences_status = 3;
                Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
            }
            track_cust_module_alarm_vibrates((void *)2);
            break;
    }
#if defined(__PARAM_CHANGE_UPDATE_SERVER__)
    track_cust_paket_9404_handle();
#endif
}

/******************************************************************************
 *  Function    -  track_cust_module_alarm_vibrates
 *
 *  Purpose     -  ��⵽��
 *
 *  Description -  mode=1����⵽��
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 27-10-2012,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_module_alarm_vibrates(void *arg)
{
    static U8 shock_tmp = 0;
    static kal_uint16 vibratesCount = 0;
    static kal_bool valid = KAL_FALSE;
    static U32 sampling_interval_tick = 0;
    U32 mode = (kal_uint32)arg;
    U32 tick = OTA_kal_get_systicks();
    U8 defences;
    if((G_parameter.motor_lock.sw == 0 && G_parameter.vibrates_alarm.sw == 0) || G_parameter.vibrates_alarm.detection_count == 0)
    {
        return;
    }
    if(track_cust_get_lease_desired() == 1) 
	{
		LOGD(L_APP, L_V6, "desired on");
		return;
	}

	defences = G_parameter.ste_status.lease;
	if(defences == 0) defences = 2;

    if(mode == 1)
    {
        LOGD(L_BMS, L_V2, "mode:%d, valid:%d, sampling_interval_tick:%d/%d, defences:%d, sw:%d, alarm_delay_time:%d",
             mode, valid, tick, sampling_interval_tick, defences, G_parameter.vibrates_alarm.sw, G_parameter.vibrates_alarm.alarm_delay_time);
    }
    else
    {
        LOGD(L_APP, L_V6, "mode:%d, valid:%d, sampling_interval_tick:%d/%d, defences:%d, sw:%d, alarm_delay_time:%d",
             mode, valid, tick, sampling_interval_tick, defences, G_parameter.vibrates_alarm.sw, G_parameter.vibrates_alarm.alarm_delay_time);
    }
    if(mode == 1 && valid && tick > sampling_interval_tick && defences == 2)
    {
    
		if(G_realtime_data.defences_status == 1) return;//����״̬�£�����ⱨ��
		
		#if 0//��ȥ�� 2019-7-11 Liujw
    	shock_tmp = track_cust_get_shock();
	    if(((shock_tmp&2) == 2 || (shock_tmp & 8) == 8))// && track_is_timer_run(TRACK_CUST_VEHICLE_LEASE_OFF_NO_SHOCK_TIMER_ID)
		{
			LOGD(L_APP, L_V5, "2-��ʱ���ĳ���������𶯱���,%d", shock_tmp);
			return;
		}
		#endif
        vibratesCount++;
        sampling_interval_tick = tick + OTA_kal_secs_to_ticks(G_parameter.vibrates_alarm.sampling_interval) - KAL_TICKS_100_MSEC * 2;
        LOGD(L_BMS, L_V3, "vibratesCount:%d / %d",
             vibratesCount, G_parameter.vibrates_alarm.detection_count);
        if(vibratesCount == 1)
        {
            track_start_timer(
                TRACK_CUST_MODULE_ALARM_VIBRATES_CHECK_TIMER_ID,
                G_parameter.vibrates_alarm.detection_time * 1000,
                track_cust_module_alarm_vibrates, (void*)2);
        }
        if(vibratesCount == G_parameter.vibrates_alarm.detection_count)
        {
            LOGD(L_BMS, L_V2, "����������");
            valid = KAL_FALSE;
            vibratesCount = 0;

            if(G_parameter.vibrates_alarm.sw
                    && G_parameter.vibrates_alarm.alarm_delay_time > 0
                    && !track_is_timer_run(TRACK_CUST_MODULE_ALARM_VIBRATES_DELAY_TIMER_ID))
            {
                {
                    LOGD(L_APP, L_V4, "����%d����ʱ����", G_parameter.vibrates_alarm.alarm_delay_time);

					track_cust_senalm_handle(1);
                    if(G_parameter.motor_lock.sw == 0)
                    {
                        track_cust_status_defences_change((void*)1);
                    }

                }
            }
        }
    }
    else if(mode == 2 && G_parameter.ste_status.lease == 0)  // �����������Ҫ���������ǰ��
    {
        valid = KAL_TRUE;
        vibratesCount = 0;
    }
    else if(mode == 3)
    {
        valid = KAL_FALSE;
        vibratesCount = 0;
    }
}

/******************************************************************************
 *  Function    -  track_cust_defences_cmd_update
 *
 *  Purpose     -  ָ����±���״̬
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_defences_cmd_update(void)
{
#if defined(__NF2318__)
	;
#else
#if defined(__NT55__)
	if(G_realtime_data.defense_mode == 0 && G_parameter.acct.type < 3 && G_parameter.acct.sw)
#else
    if(G_realtime_data.defense_mode == 0)   /* �Զ����ģʽ */
#endif
    {
        kal_uint8 defences = track_cust_status_defences();
        kal_uint8 acc_status = track_cust_status_acc();
        LOGD(L_APP, L_V5, "motor_lock.sw:%d, vibrates_alarm.sw:%d, defences=%d, acc_status:%d",
             G_parameter.motor_lock.sw, G_parameter.vibrates_alarm.sw, defences, acc_status);
        if(acc_status || (G_parameter.motor_lock.sw == 0 && G_parameter.vibrates_alarm.sw == 0 && defences != 0))
        {
            track_cust_status_defences_change((void*)0);
        }
        else if(acc_status == 0 && (G_parameter.motor_lock.sw == 1 || G_parameter.vibrates_alarm.sw == 1) && defences == 0)
        {
            track_cust_status_defences_change((void*)1);
            track_start_timer(
                TRACK_CUST_STATUS_DEFENCES_TIMER_ID,
                G_parameter.defense_time * 1000,
                track_cust_status_defences_change, (void*)2);
        }
    }
    else
    {
        /* �ֶ����ģʽ */

    }
#endif
}

/******************************************************************************
 *  Function    -  track_cust_shocks_quiet_time_update
 *
 *  Purpose     -  ָ����±���״̬2
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_shocks_quiet_time_update(U32 alarm_interval)
{
    if(track_is_timer_run(TRACK_CUST_MODULE_ALARM_VIBRATES_TIMER_ID))
    {
        LOGD(L_APP, L_V5, "");
        tr_start_timer(
            TRACK_CUST_MODULE_ALARM_VIBRATES_TIMER_ID,
            alarm_interval * 1000,
            alarm_vibrates_valid_lid);
    }
}


