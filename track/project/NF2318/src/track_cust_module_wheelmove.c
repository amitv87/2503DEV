/*******************************************************************************************
 * Filename:track_cust_module_wheelmove.c.c
 * Author :Liujw
 * Date :2018-01-20
 * Comment:7�ŵ絥���ֶ�����
 *
 ******************************************************************************************/
/**����ͷ�ļ�**/
#include "track_cust.h"
#include "track_os_ell.h"
#include "track_cust_platform_N07.h"

/****************************************************************************
***��̬����
****************************************************************************/
static kal_bool wheel_moving = KAL_FALSE;
static U8 wheel_confirm = 0;
/****************************************************************************
***���ñ���
****************************************************************************/

/****************************************************************************
***���ú���
****************************************************************************/
extern void play_voice(U8 ring);
extern void track_cust_gate_on_gpio(void* arg);
extern void track_cust_gate_off_gpio(void* arg);
//20190916
extern void track_instructions_get_paging(void);

void track_cust_wheel_stop()
{
    static U8 shock_tmp = 0;
    //wheel_confirm = 0;
    wheel_moving = KAL_FALSE;
    //--------------
    track_cust_vehicle_moving(45);
    //---------------
    shock_tmp = track_cust_get_shock();
    LOGD(L_APP, L_V1, "shock_tmp:%d", shock_tmp);
    if((shock_tmp & 1) == 1) //����Ϊ���𶯻����𶯴�����û�л�
    {
        LOGD(L_APP, L_V1, "not shock or sensor fail! %d", shock_tmp);
    }
    else
    {
        tr_start_timer(TRACK_CUST_VEHICLE_LEASE_OFF_NO_SHOCK_TIMER_ID, 10 * 1000, track_cust_shock_overtime);
    }
}
kal_bool track_cust_wheel_state()
{
    return wheel_moving;
}
void track_cust_wheel_lock_ele(void* param)
{
    kal_uint8 lock = (kal_uint8)param;
    LOGD(L_APP, L_V1, "%d", lock);
    if(lock)
    {
        set_ele_lock = 1;
    }
    else
    {
        //set_ele_lock = 0;//ΪʲôҪ�򿪵����
    }
    if(G_parameter.ste_status.lease == 0)
    {
        if(!track_is_timer_run(TRACK_CUST_CONTROLLER_DATA_TIMER_ID))
        {
            track_cust_cotroller_alarm((void*)0);
        }
    }
    //request_static_packet((void*)99);
    if(!lock)
    {
        track_acc_status((void *)0);//GPIO_WriteIO(0, ELE_LOCK_GPIO);//�ر�acc
        set_ele_lock = 0;
    }

}
void track_cust_wheel_alarm()
{

    LOGD(L_BMS, L_V5, "�ֶ�����");
    //20191009 ȡ���ֶ�����������Ĵ��˰�ACCҲ����ȥ��
    #if 0
    track_acc_status((void *)2);//GPIO_WriteIO(1, ELE_LOCK_GPIO);//��acc
    play_voice(8);
    #endif
    track_acc_status((void *)2);
    //track_cust_gate_on_gpio((void*)1);//������
    track_cust_wheel_lock_ele((void*)1);

    track_start_timer(TRACK_CUST_WHEEL_ALARM_LOCK_ELE_TIMER_ID, 6000, track_cust_wheel_lock_ele, (void *) 0);
    //GPIO_WriteIO(0, 50);//�����ϵ�
    track_cust_N07_paket_04(ALARM_MOVING, 0x01);
	//track_instructions_get_paging();//20190916
    track_cust_paging_command();//�ϴ���λ��,�ֶ�����û������
}
void track_cust_wheel_signal_reception(kal_uint8 signal)
{
    static kal_uint8 counts = 0;
    static kal_uint32 start_ticks = 0;
    static kal_uint32 ticks = 0;
    kal_uint32 seconds = 0;
    static U8 level = 99;
    static U8 shock_tmp = 0;

    LOGD(L_BMS, L_V5, "�ֶ����%d,%d,��������:%d,acc:%d,lease:%d,disarm:%d", G_parameter.ste_param.wheelmovtime, signal, G_parameter.ste_status.vechile_sw, track_cust_status_acc(), G_parameter.ste_status.lease, G_realtime_data.defences_status);
    //if(G_parameter.ste_status.vechile_sw) return;
    wheel_moving = KAL_TRUE;
    //track_cust_wheel_set();//���ñ�ǣ���wheel_moving����������
    tr_start_timer(TRACK_CUST_WIFI_WHEELMOVING_OVERTIME_TIMER_ID, 2 * 1000, track_cust_wheel_stop);
#if 0//2019-4-10 �ر���δ��룬���������ִ����ϣ���ܱ��������ֶ��ź�Ӱ������������
    if(track_is_timer_run(TRACK_CUST_SENALM_LOCK_ELE_TIMER_ID))
    {
        //track_cust_senalm_handle(0);
        track_cust_ele_lock((void*) 2);
    }
#endif
    /*
    if(wheel_confirm < 2)
    {
    	wheel_confirm ++;
    	return;
    }*/
    track_cust_vehicle_moving(2);
    if((track_cust_get_lease_desired() == 1) || track_cust_status_acc() || (G_realtime_data.defences_status == 1) || G_parameter.ste_status.lease || track_is_timer_run(TRACK_CUST_LEASE_ON_PRECONDITION_TIMER_ID))
    {
        //�˴���һ�����⣬�����⵽�𶯣����acc����ô��Ҫ��Ҫ������ߣ��Ȳ����
        LOGD(L_BMS, L_V6, "�ֶ����������");
        return; //����״̬�£�����ⱨ��
    }

#if 0//��ȥ�� 2019-7-11 Liujw
    shock_tmp = track_cust_get_shock();
    if(((shock_tmp & 2) == 2 || (shock_tmp & 8) == 8)) /* && track_is_timer_run(TRACK_CUST_VEHICLE_LEASE_OFF_NO_SHOCK_TIMER_ID)*/
    {
        LOGD(L_APP, L_V5, "1-��ʱ���ĳ���������ֶ�����,%d", shock_tmp);
        return;
    }
#endif
    if(G_parameter.ste_param.wheelmovtime <= 0)
    {
        return;
    }
    if(track_is_timer_run(TRACK_CUST_WHEEL_ALARM_LOCK_ELE_TIMER_ID))
    {
        LOGD(L_BMS, L_V5, "���ڴ����ϴ��ֶ�����");
        return;
    }
    ticks = OTA_kal_get_systicks();
    if(counts == 0)
    {
        //level = signal;
        start_ticks = OTA_kal_get_systicks();
    }
    counts++;


    if(counts-1 >= G_parameter.ste_param.wheelmov)//���������һ�Σ��д�����
    {
        seconds = OTA_kal_ticks_to_secs(ticks - start_ticks);
        if(seconds <= G_parameter.ste_param.wheelmovtime)
        {
            track_cust_wheel_alarm();
        }
        counts = 0;
        //level = 99;
    }
}
void track_cust_acc_fault_alarm(void * arg)
{
	kal_bool iswrite = KAL_FALSE;
	LOGD(L_APP, L_V5, "ACC���ϱ������:expect acc:%d  reality acc: %d  acc_fault_alarm :%d", (U8)arg,track_cust_acc_status_confirm(),G_realtime_data.acc_fault_alarm);
	if((U8)arg != track_cust_acc_status_confirm())
	{	
		if(G_realtime_data.acc_fault_alarm == 0)
		{
			track_cust_N07_paket_04(ALARM_ACC_FAULT, 0x01);
			G_realtime_data.acc_fault_alarm = 1;
			iswrite = KAL_TRUE;
		}
	}
	else
	{
		if(G_realtime_data.acc_fault_alarm == 1)
		{
			track_cust_N07_paket_04(ALARM_ACC_FAULT, 0x00);
			G_realtime_data.acc_fault_alarm = 0;
			iswrite = KAL_TRUE;
		}
	}
    if(iswrite)
    {
    	track_cust_paging_command();//20190916
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
    }
	return;
}
//2019_________________-acc�쳣����
void track_cust_acc_alarm(U8 arg)
{
    kal_bool iswrite = KAL_FALSE;

    LOGD(L_BMS, L_V5, "ACC�쳣�����������:%d,%d,%d,%d", arg, iswrite, G_realtime_data.acc_alarm, G_parameter.ste_status.lease);
    if(arg == 1 && track_acc_status((void*)3))
    {
        LOGD(L_APP, L_V5, "������ACC,������ACC�쳣����");
        return;
    }
    if(!G_parameter.ste_status.lease && arg == 1 && !G_realtime_data.acc_alarm)
    {
        track_cust_N07_paket_04(ALARM_OPEN_ACC, 0x01);
        G_realtime_data.acc_alarm = 1;
        iswrite = KAL_TRUE;
    }
    else if(arg == 2 && G_realtime_data.acc_alarm)
    {
        track_cust_N07_paket_04(ALARM_OPEN_ACC, 0x00);
        G_realtime_data.acc_alarm = 0;
        iswrite = KAL_TRUE;
    }
    if(iswrite)
    {
        track_cust_paging_command();//�ϴ���λ��,�ֶ�����û������
        //track_instructions_get_paging();//20190916
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
    }
}
void track_cust_controller_alarm(U8 arg)
{
    kal_bool iswrite = KAL_FALSE;
    LOGD(L_BMS, L_V7, "�����������������:%d,%d,%d,%d", arg, iswrite, G_realtime_data.controller_alarm, G_parameter.ste_status.lease);
    if(arg == 1 && !G_realtime_data.controller_alarm)
    {
        G_realtime_data.controller_alarm = 1;
        track_cust_N07_paket_04(ALARM_CONTROLER_WARNING, 0x01);
        iswrite = KAL_TRUE;
    }
    else if(arg == 0 && G_realtime_data.controller_alarm)
    {
        G_realtime_data.controller_alarm = 0;
        track_cust_N07_paket_04(ALARM_CONTROLER_WARNING, 0x00);
        iswrite = KAL_TRUE;
    }
    if(iswrite)
    {
        track_cust_paging_command();//�ϴ���λ��,�ֶ�����û������
        //track_instructions_get_paging();//20190916
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
    }
}
//---------------
extern kal_uint8 track_get_lesea_off_sta(kal_uint8 value);
//-----------------
void track_cust_rearwheel_fault_alarm(U8 arg)
{
    kal_uint8 position = 0;
    LOGD(L_BMS, L_V5, "wheel fault:%d,%d", arg, G_realtime_data.rearwheelalarm);

    if(arg == 1 && G_realtime_data.rearwheelalarm == 0)
    {
        track_cust_N07_paket_04(ALARM_REARWHEEL_LOCK_WARNING, 0x01);
        position = 1;
        G_realtime_data.rearwheelalarm = 1;
    }
    else if(arg == 0 && G_realtime_data.rearwheelalarm == 1)
    {
        track_cust_N07_paket_04(ALARM_REARWHEEL_LOCK_WARNING, 0x00);
        position = 1;
        G_realtime_data.rearwheelalarm = 0;
    }
    if(position == 1)
    {
        track_cust_paging_command();//�ϴ���λ��,�ֶ�����û������
        //track_instructions_get_paging();//20190916
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
    }
}

void track_cust_exbatlock_fault_alarm(U8 arg)
{
    kal_uint8 position = 0;
    LOGD(L_BMS, L_V5, "exbat fault:%d,%d", arg, G_realtime_data.elelockalarm);

    if(arg == 1&& G_realtime_data.elelockalarm == 0)
    {
        track_cust_N07_paket_04(ALARM_EXBAT_LOCK_WARNING, 0x01);
        position = 1;
        G_realtime_data.elelockalarm = 1;
    }
    else if(arg == 0 && G_realtime_data.elelockalarm == 1)
    {
        track_cust_N07_paket_04(ALARM_EXBAT_LOCK_WARNING, 0x00);
        position = 1;
        G_realtime_data.elelockalarm = 0;
    }
    if(position == 1)
    {
        track_cust_paging_command();//�ϴ���λ��,�ֶ�����û������
        //track_instructions_get_paging();//20190916
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
    }
}

