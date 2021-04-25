/******************************************************************************
 * track_cust_module_mcu.c -
 *
 * Copyright .
 *
 * DESCRIPTION: -
 *        GSM��MCUͨ��
 *        �ļ��ṹ���⣬����Ϊÿ����Ŀ��Э�鲻ͬ���ִ���һ���Ĺ��ԣ��ṹ����

        ��Ƭ���������ò���
        GT740Э�鴦����
        GT300SЭ�鴦����
        GT370AЭ�鴦����
 *
 *
 * modification history
 * --------------------
 * v1.0   2017-03-22,  chengjun create this file
 *
 ******************************************************************************/
//��Ƭ���������ò���
#if defined(__GT740__) || defined(__GT370__)|| defined (__GT380__) || defined(__GT300S__)
/****************************************************************************
*  Include Files                        ����ͷ�ļ�
*****************************************************************************/
#include "track_cust.h"
#include "track_drv_uart.h"
#include "track_drv_pmu.h"

/*****************************************************************************
*  Define                               �궨��
******************************************************************************/
#define PARAMETER_MAX   16


/*****************************************************************************
*  Typedef  Enum                        ö�ٶ���
*****************************************************************************/
typedef void (*Func_CustRecvPtr)(void);

typedef struct
{
    const char           *cmd_string;
    Func_CustRecvPtr     func_ptr;
} cust_recv_struct;


/*****************************************************************************
*  Struct                               ���ݽṹ����
******************************************************************************/
static char *blank = {""}, *s_ON = {"ON"}, *s_OFF = {"OFF"}, *s_Error = {"ERROR"};

#define ON_OFF(value) (value==0?s_OFF:(value==1?s_ON:s_Error))


void track_cust_module_power_data(kal_uint8 *data, int len);

/******************************************************************************
 * track_cust_module_extpower.c -
 *
 * Copyright .
 *
 * DESCRIPTION: -
 *        GSMģ���뵥Ƭ��ģ��ͨ��
 *
 * modification history
 * --------------------
 * v1.0   2016-10-14,  chengjun create this file
 *
 ******************************************************************************/
#if defined (__MCU_SW_UPDATE__)
kal_uint8 mcu_sw_version_string[32] = {0};
kal_uint8* track_get_mcu_sw_version(void)
{
    return mcu_sw_version_string;
}

void track_cust_module_uart_update_data(kal_uint8 *data, int len)
{
    if((data == NULL) || (len <= 0))
    {
        return;
    }

    LOGD(L_DRV, L_V1, "����2���� len=%d", len);
    LOGH(L_DRV, L_V6, data, len);

    if((len == 1) && (*data == 0 || *data == 0xFF))
    {
        return;
    }

    if(strstr(data, "AT^GT_CM=") != NULL)
    {
        track_cust_module_power_data(data, len);
    }
    else if(track_is_in_update_bootload_cmd())
    {
        track_drv_urat_mcu_updata_ymode_decode(data, len);
    }
    else if(strstr(data, "BB") != NULL)
    {
        track_drv_urat_mcu_updata_ymode_decode(data, len);
    }
    else
    {
        track_cust_module_power_data(data, len);
    }
}

void track_cust_ota_cmd(void)
{
    char buf[100] = {0};
    sprintf(buf, "AT^GT_CM=07,15,240#\r\n");
    track_drv_write_exmode(buf, strlen(buf));
    LOGD(L_UART, L_V5, "");
}

#endif /* __MCU_SW_UPDATE__ */


#endif

//��ͬ��Ŀ��ͬЭ����Դ���
#if defined (__GT740__)

#define TRACKING_TIME 20 //׷��ģʽ��������
#define BOOT_TIME 3 //��������������𱨾� ����
#define CAL_ELETRCITY 600 //  10*60�������Ƶ��
#define BOOT_ELE 70 //  ����2.85-2.95������ʼֵ

#define BAT_SWT_LOW_VALUE 275 // �����������
#define BAT_SWT_HIGH_VALUE 290 // �����������

#define BAT_DYN_LOW_VALUE  275// ��̬�����������
#define BAT_DYN_DIFFENCE_VALUE  20// ������ֵ����0.2v

#define HISTORY_ELE_FOR_LOW 60 //��Ӧ��ʷ�������ĵ���ֵ�ٷֱ�,
#define HISTORY_ELE_FOR_DYN 20 //��Ӧ��ʷ�������ĵ���ֵ�ٷֱ�,

#define RESEND_COMMAND_TIMEOUT_SEC 2000
typedef enum
{
    ST_VERSION = 1,
    ST_VOL,
    ST_TIME,
    ST_UPTIME, //Զ�������ӳٹػ�ʱ��
    ST_LOWVOL,
    ST_HANDSHAKE,
    ST_KEYSTATUS,
    ST_RST
} st_parameter;

typedef enum
{
    sthour = 0,
    stmin,
    styear,
    stmonth,
    stday,
} st_time_sort;
/*****************************************************************************
*  Local variable                       �ֲ�����
*****************************************************************************/
static char *cmd_parameters[PARAMETER_MAX] = {0};
static int cmd_parameters_count = 0;
static U32 g_tick_new = 0;
static cmd_return_struct g_return_sms = {0};
static kal_bool g_02_ok = KAL_FALSE;
static kal_uint8 g_mode = 0,  g_return_sms_valid = 0;
static kal_bool ensleep = FALSE;
static kal_bool b_track_mode = KAL_FALSE;//׷��ģʽ
//static U8 Arithmetic_logic = 1;
static U8 first_syn = 0; //�״�ͬ����ѹ
static U8 wakeup2_syn = 0; // ģʽ������
static U16 first_volt = 0;
//static U16 first_calibration_ele=0;//��ѹУ׼�ĵ����ٷֱ�
static kal_uint8 single_rst = 0xff;// ��Ƭ���״��ϵ��־
static kal_uint8 gsm_ele = 0; // �ĵ��߼��õ�ʣ������ٷֱ�
static 	kal_uint32 single_ele_value = 0; // �����ڵ�Ƭ��������ĵ���ֵ
static kal_uint32 first_10_value = 1; // �״�10���������
static U8 RESETBAT = 0; // �Ƿ񻻵��
static kal_uint32 gpsboottick = 0, gpsclosetick = 0, gpssleeptick = 0, gpsclosesleeptick = 0, gpsworksec = 0, gpssleepsec = 0;
static const float InitElet = 6000000; // 2500*3600 2500mas��λ�ĳ���
static const kal_uint32  mode2_pwrconsumption = 4320; // 1.2*3600 mas
static const kal_uint32  mode1_gps_pwrcons = 50;
static const kal_uint32  mode1_gps_sleeppwrcons = 6;
static kal_uint8 st_time[10] = {0};
static kal_uint8 wakeup_mode = 0; // 0 ������ʱ����, 1 �������ѣ� 2 ����AMS����
static kal_uint8 st_rst = 99;

/****************************************************************************
*  Global Variable                      ȫ�ֱ���
*****************************************************************************/


/****************************************************************************
*  Global Variable - Extern             ����ȫ�ֱ���
*****************************************************************************/


/*****************************************************************************
*  Global Functions - Extern            �����ⲿ����
******************************************************************************/
extern void track_cust_teardown_alarm();

extern void track_drv_sys_power_off_req(void);
extern void track_cust_sendlbs_limit(void);
/*****************************************************************************
*  Local Functions Define               ���غ�������
******************************************************************************/
void track_cust_module_update_mode(void);
void track_cust_module_auto_sw_mode(void);
void track_cust_module_status_inquiry(void);
void track_cust_module_track_mode_timeout();
void track_cust_upload_param();
void auto_mode_callback(void* par);
void track_cust_module_extchip_shutdown(void);
void track_cust_module_delay_close(U8 op);
void track_cust_module_gps_boot_time();
void track_cust_module_cal_electricity();
void track_cust_module_boot_cal();
void track_cust_resetbat();
void track_cust_module_clearup();
void track_cust_save_logfile(kal_uint8 *data, kal_uint16 len);
#if 0
#endif /* 0 */
/*****************************************************************************
*  Local Functions                      ���غ���
******************************************************************************/



kal_uint8 track_cust_module_get_syn_vol()
{
    return first_syn;
}


void track_cust_pre_remove_alarm()
{
    static kal_uint8 i = 1;
    track_gps_data_struct *gpsLastPoint = NULL;
    if(!track_is_timer_run(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID))
    {
        gpsLastPoint = track_cust_gpsdata_alarm();
        if(gpsLastPoint->gprmc.status == 2)
        {
            track_cust_paket_19(track_drv_get_lbs_data(), TR_CUST_ALARM_PRE_REMOVE, KAL_TRUE, 0);
        }
        else
        {
            track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_PRE_REMOVE, KAL_TRUE, 0);
        }
        LOGD(L_UART, L_V2, "%d", i);
        i++;
        if(i <= 3) //��������,��ʱ����������
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID, 60 * 1000, track_cust_pre_remove_alarm);
        }
        else
        {
            track_stop_timer(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID);
            i = 1;
        }

    }
    else
    {
        //��ֹ��ͻ֮ǰ�ı���
        LOGD(L_UART, L_V2, "��ͻ,�ӳ�5��!");
        if(!track_is_timer_run(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID))
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_SEND_REMOVE_ALARM_TIMER_ID, 5000, track_cust_pre_remove_alarm);
        }
    }

}

U8 track_cust_module_tracking_mode(U8 op)
{
    if(op == 99)
    {
        return G_realtime_data.tracking_mode;
    }
    if(op != G_realtime_data.tracking_mode)
    {
        G_realtime_data.tracking_mode = op;
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
    }
    LOGD(L_UART, L_V5, "%d", op);
    return 0;
}
U8 track_cust_remove_alarm_lock(U8 lock)
{
    static U8 lockup = 0;
    if(lock != 99)
    {
        lockup = lock;
    }
    return lockup;
}

//�տ����ӳ�30�뷢����
static void track_cust_delay_send_msg()
{
#if 0
    if(track_cust_gps_status() > 2)
    {
        g_alarm_gps_status = 1;
    }
    if(G_parameter.gprson)
    {
        track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_Vibration, KAL_TRUE, 1);
        tr_start_timer(
            TRACK_CUST_ALARM_alarm_vibrates_OVERTIME_TIMER,
            2 * 60 * 1000,
            track_cus_alarm_vibrates_overTime_sendsms);
    }
#endif
    track_os_intoTaskMsgQueue(track_cust_teardown_alarm);
    track_cust_remove_alarm_lock(1);//����
}
static void confirm(char *str)
{
    if((track_is_testmode() == KAL_TRUE))
    {
        LOGS("%s", str);
        return;
    }
    if(g_return_sms_valid)
    {
        track_cust_cmd_reply(&g_return_sms, str, strlen(str));
        g_return_sms_valid = 0;
    }
    else LOGD(L_UART, L_V5, "%s", str);
}


kal_uint8 track_cust_05_status(U8 in)
{
    static kal_uint8 rlt = 0;
    if(in == 1 || in == 0)
    {
        rlt = in;
    }
    LOGD(L_UART, L_V6, "rlt %d", rlt);
    return rlt;
}


//�����Ƿ�������׷��ģʽ
kal_uint8 track_cust_judge_tracking_time(nvram_ext_chip_struct* chip)
{
    applib_time_struct tmp = {0};
    kal_uint32 m1time = 0;
    memcpy(chip, &G_parameter.extchip, sizeof(nvram_ext_chip_struct));
    OTA_applib_dt_get_rtc_time(&tmp);
    m1time = OTA_applib_dt_mytime_2_utc_sec(&tmp, 0) + G_realtime_data.tracking_time * 60; // ���㴥������ʱһСʱ�����ֵ
    if(G_parameter.extchip.mode != 1 || (m1time > G_parameter.extchip.mode1_time))
    {
        if(chip->mode != 1)
        {
            chip->premode = chip->mode;
        }
        chip->mode = 1;
        chip->mode1_par1 = 20;
        chip->mode1_par2 = G_realtime_data.tracking_time;
        chip->mode1_time = m1time;
        LOGD(L_UART, L_V7, "time: %0.2d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d    sec:%d",
             tmp.nYear, tmp.nMonth, tmp.nDay,
             tmp.nHour, tmp.nMin, tmp.nSec);
        LOGD(L_UART, L_V3, "trtime %d, mod1time %d", chip->mode1_time, G_parameter.extchip.mode1_time);

        return 1;
    }
    else
    {
        LOGD(L_UART, L_V6, "����׷��ģʽ����ָ���ʱ�䷶Χ��!");
        return 0;
    }

}

U16 track_cust_calibration_table(U16 v)
{
    U8 i = 0;
    U16 v_table[39][2] =
    {
        {285, 37}, {284, 36}, {283, 34}, {282, 32}, {281, 30}, {280, 29}, {279, 27}, {278, 26},
        {277, 25}, {276, 23}, {275, 22}, {274, 21}, {273, 19}, {272, 19}, {271, 18}, {270, 17},
        {269, 16}, {268, 15}, {267, 14}, {266, 13}, {265, 13}, {264, 12}, {263, 11}, {262, 10},
        {261, 10}, {260, 9}, {259, 9}, {258, 8}, {257, 7}, {256, 7}, {255, 6}, {254, 6}, {253, 5},
        {252, 5}, {249, 4}, {245, 3}, {241, 2}, {237, 1}, {200, 0}
    };
    for(; i < 39; i++)
    {
        if(v >= v_table[i][0])
        {
            return v_table[i][1];
        }
    }
}

/**************************************************
* function : track_cust_calibration_volt
*
* Paramer : ??ʱ��ת��������
*
* Purpose :  ͳ�� Vbat ����2.85��ʱ��
*
*
***************************************************/
void track_cust_calibration_volt(U32 timep)
{
    track_work_time tmp_struct = {0};
    applib_time_struct tmp_time = {0};
    U8 c_oount = 0, ele_per = 0; //ʣ��ĵ����ٷֱ�
    U32 volt2 = 0;
    memcpy(&tmp_struct, &G_parameter.cal_wk_time_struct, sizeof(track_work_time));
    c_oount = tmp_struct.gsm_volt_low_calibration_count;
    LOGD(L_UART, L_V5, "%d, %d", first_volt, c_oount);
    if(tmp_struct.gsm_calibration_status == 0)
    {
        if(first_volt < 285)
        {
            if(c_oount == 0)
            {
                tmp_struct.gsm_calibration_time[c_oount] = timep;
                tmp_struct.gsm_volt_low_calibration_count++;
            }
            else if(c_oount >= 4)
            {
                if(((timep - tmp_struct.gsm_calibration_time[0]) / 3600) < 24)
                {
                    if(G_realtime_data.netLogControl & 32)
                    {
                        char buf[50];
                        sprintf(buf, "history data interval not more than one day");
                        cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
                    }
                    LOGD(L_UART, L_V5, "��ʷ���ݼ��������һ��%d, %d,interval %d", tmp_struct.gsm_calibration_time[0], timep, ((timep - tmp_struct.gsm_calibration_time[0]) / 3600));
                }
                else
                {
                    tmp_struct.gsm_calibration_status = 1; //��У׼
                    tmp_struct.gsm_calibration_time[4] = timep;
                    tmp_struct.gsm_volt_low_calibration_count = 5;
                    ele_per = (U8)(((InitElet - G_parameter.cal_wk_time_struct.comsultion_ele) / InitElet) * 100.0);

                    tmp_struct.first_calibration_ele = track_cust_calibration_table(first_volt);
                    LOGD(L_UART, L_V5, "У׼��������� %d, �߼�����%d, %d", tmp_struct.first_calibration_ele, ele_per, gsm_ele);
                    if(G_realtime_data.netLogControl & 32)
                    {
                        char buf[50];
                        sprintf(buf, "power calibration %d, logic computing %d", tmp_struct.first_calibration_ele, gsm_ele);
                        cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
                    }
                    if(ele_per - tmp_struct.first_calibration_ele > 10)
                    {
                        ele_per -= 10;
                        tmp_struct.comsultion_ele = (100 - ele_per) * InitElet / 100.0;
                        memcpy(&G_parameter.cal_wk_time_struct, &tmp_struct, sizeof(track_work_time));
                        Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
                        LOGD(L_UART, L_V5, "ele_per %d, %.0f", ele_per, tmp_struct.comsultion_ele);
                        return;
                    }
                }
            }
            else
            {
                tmp_struct.gsm_calibration_time[c_oount] = timep;
                tmp_struct.gsm_volt_low_calibration_count += 1;
            }
        }
        else
        {
            LOGD(L_UART, L_V5, "volt over calibration clear history data!");
            tmp_struct.gsm_calibration_status = 0;
            tmp_struct.gsm_volt_low_calibration_count = 0;
            memset(&tmp_struct.gsm_calibration_time, 0, sizeof(tmp_struct.gsm_calibration_time));
        }
        LOGD(L_UART, L_V5, "%d, %d", tmp_struct.gsm_calibration_status, tmp_struct.gsm_volt_low_calibration_count);
        //OTA_applib_dt_sec_2_mytime(tmp_struct.gsm_calibration_time[0], &tmp_time);
        //LOGD(L_UART, L_V5,"%d, %d-%d %d:%d:%d",tmp_struct.gsm_calibration_time[0],tmp_time.nMonth,tmp_time.nDay,tmp_time.nHour,tmp_time.nMin,tmp_time.nSec);
        memcpy(&G_parameter.cal_wk_time_struct, &tmp_struct, sizeof(track_work_time));
        Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
    }
    else
    {
        ele_per = (U8)(((InitElet - G_parameter.cal_wk_time_struct.comsultion_ele) / InitElet) * 100.0);
        if(ele_per - tmp_struct.first_calibration_ele > 10)
        {
            ele_per -= 10;
            tmp_struct.comsultion_ele = (100 - ele_per) * InitElet / 100.0;
            memcpy(&G_parameter.cal_wk_time_struct, &tmp_struct, sizeof(track_work_time));
            Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
            LOGD(L_UART, L_V5, "��ֹ�����½� ele_per %d, %.0f", ele_per, tmp_struct.comsultion_ele);
            if(G_realtime_data.netLogControl & 32)
            {
                char buf[50];
                sprintf(buf, "prevent decline %d, %d", ele_per, tmp_struct.first_calibration_ele);
                cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
            }
            return;
        }
    }
}

void track_count_low_volt_times(U32 volt)
{
#if 0
    track_work_time tmp_struct = {0};
    U32 volt2 = 0;
    memcpy(&tmp_struct, &G_parameter.cal_wk_time_struct, sizeof(track_work_time));
    LOGD(L_UART, L_V5, "%d", volt);
    if(volt < 240)
    {
        tmp_struct.gsm_volt_low_time += 1;
        volt2 = track_cust_get_battery_volt();
        if(tmp_struct.gsm_volt_low_time >= 3 && volt2 > 2400000)
        {
            tmp_struct.gsm_volt_low_time = 3;
            tmp_struct.gsm_alrigothm_logic = 1;
            track_cust_set_battery_volt(volt);
        }
        LOGD(L_UART, L_V5, "v:%d, ��ѹ�ۼƴ����ۼ�:%d, ����ֵ%d", volt, G_parameter.cal_wk_time_struct.gsm_volt_low_time, volt2);
    }
    else
    {
        tmp_struct.gsm_volt_low_time = 0;
        tmp_struct.gsm_alrigothm_logic = 0;
        if(!track_is_timer_run(TRACK_CUST_EXTCHIP_CAL_ELETICITY_TIMER_ID))
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_CAL_ELETICITY_TIMER_ID, 2000, track_cust_module_cal_electricity);
        }
    }
    if(!memcmp(&G_parameter.cal_wk_time_struct, &tmp_struct, sizeof(track_work_time)))
    {
        return;
    }
    else
    {
        memcpy(&G_parameter.cal_wk_time_struct, &tmp_struct, sizeof(track_work_time));
        Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
    }
#endif
}

kal_bool track_cust_existence_history_Date()
{
    if(G_parameter.cal_wk_time_struct.comsultion_ele == 0 && single_ele_value == 0)
    {
        return KAL_FALSE;
    }
    if(G_parameter.cal_wk_time_struct.comsultion_ele == 0 && single_ele_value != 0)
    {
        LOGD(L_UART, L_V5, "����ʷ���ݣ�ȡ��Ƭ����������� %d", single_ele_value);
        if(G_realtime_data.netLogControl & 32)
        {
            char buf[50];
            sprintf(buf, "NO HISTORY DATA,GETCH BY SINGLE %d", single_ele_value);
            cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
        }
        G_parameter.cal_wk_time_struct.comsultion_ele = single_ele_value;
    }
    return KAL_TRUE;
}


/******************************************************************************
 *  Function    -  track_cust_statistics_volt_for_Swtbat
 *
 *  Purpose     - ��������жϽӿ�
 *
 *  Description - volt ��ѹֵ
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 2016-05-04 xzq
 * ----------------------------------------
*******************************************************************************/
kal_bool track_cust_statistics_volt_for_Swtbat(int volt, U32 timep)
{
    U8 c_count = 0, c_count1 = 0;
    int time_interval = 0; //Сʱ
    c_count = G_parameter.cal_wk_time_struct.bat_low_time;
    if(G_parameter.cal_wk_time_struct.bat_lowtimep != 0)
    {
        time_interval = (timep - G_parameter.cal_wk_time_struct.bat_lowtimep) / 3600;
        LOGD(L_UART, L_V6, "lowtime interval %d, firsr timep %d", time_interval, G_parameter.cal_wk_time_struct.bat_lowtimep);

    }
    if(volt < BAT_SWT_LOW_VALUE)
    {
        if(c_count == 0)
        {
            G_parameter.cal_wk_time_struct.bat_lowtimep = timep;
        }
        G_parameter.cal_wk_time_struct.bat_low_time += 1;
        LOGD(L_UART, L_V5, "����2.75 volt%d������ %d,timep %d", volt, G_parameter.cal_wk_time_struct.bat_low_time, timep);

        if(G_realtime_data.netLogControl & 32)
        {
            char buf[50];
            sprintf(buf, "low than 2.75 volt %d,times %d", volt, G_parameter.cal_wk_time_struct.bat_low_time);
            cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
        }
    }
    else if(volt > BAT_SWT_HIGH_VALUE && G_parameter.cal_wk_time_struct.bat_low_time >= 5 && (time_interval > 24))
    {
        LOGD(L_UART, L_V5, "comsultion_ele %.0f, %.0f", G_parameter.cal_wk_time_struct.comsultion_ele, G_parameter.cal_wk_time_struct.comsultion_ele / InitElet * 100.0);
        if(G_realtime_data.netLogControl & 32)
        {
            char buf[50];
            sprintf(buf, "comsultion_ele per %.0f", G_parameter.cal_wk_time_struct.comsultion_ele / InitElet * 100.0);
            cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
        }
        if(G_parameter.cal_wk_time_struct.comsultion_ele / InitElet * 100.0 > HISTORY_ELE_FOR_LOW)
        {
            LOGD(L_UART, L_V5, "���㻻��ص�ѹ��������%d,times %d", volt, G_parameter.cal_wk_time_struct.bat_low_time);
            if(G_realtime_data.netLogControl & 32)
            {
                char buf[50];
                sprintf(buf, "satisfy change bat condition %d, times %d", volt, G_parameter.cal_wk_time_struct.bat_low_time);
                cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
            }
            return KAL_TRUE;
        }

        //track_cust_resetbat();
    }
    else
    {
        G_parameter.cal_wk_time_struct.bat_low_time = 0;
        G_parameter.cal_wk_time_struct.bat_lowtimep = 0;
    }

    // ��̬��������߼� ȥ��
#if 0
    if(volt > BAT_DYN_LOW_VALUE)
    {
        U8 i = 0, j;
        int all_volt = 0, aver_volt = 0;
        if(G_parameter.cal_wk_time_struct.bat_average[4] == 0)
        {
            for(i = 0; i < 5; i++)
            {
                if(G_parameter.cal_wk_time_struct.bat_average[i] == 0)
                {
                    LOGD(L_UART, L_V5, "��%d������2.75v dyn bat %d", i, volt);
                    if(G_realtime_data.netLogControl & 32)
                    {
                        char buf[50];
                        sprintf(buf, "number %d bigger than 2.75v dyn bat %d", i, volt);
                        cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
                    }
                    G_parameter.cal_wk_time_struct.bat_average[i] = volt;
                    break;
                }
            }
        }
        else
        {
            for(j = 0; j < 5; j++)
            {
                all_volt += G_parameter.cal_wk_time_struct.bat_average[j];
            }
            aver_volt = all_volt / 5;
            LOGD(L_UART, L_V5, "�ۼ�5��DYNֵ%d, ƽ��ֵ%d, ��ֵ %d", all_volt, aver_volt, volt - aver_volt);
            if(G_realtime_data.netLogControl & 32)
            {
                char buf[50];
                sprintf(buf, "five DYN value %d,average value %d, interval %d", all_volt, aver_volt, volt - aver_volt);
                cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
            }
            if((volt - aver_volt) > BAT_DYN_DIFFENCE_VALUE)
            {
                LOGD(L_UART, L_V5, "comsultion_ele per %.0f", G_parameter.cal_wk_time_struct.comsultion_ele / InitElet * 100.0);
                if(G_parameter.cal_wk_time_struct.comsultion_ele / InitElet * 100.0 > HISTORY_ELE_FOR_DYN)
                {
                    LOGD(L_UART, L_V5, "��ֵ����5���ۼ�ƽ��ֵ0.2v");
                    if(G_realtime_data.netLogControl & 32)
                    {
                        char buf[50];
                        sprintf(buf, "dyn bat interval more than 0.2v");
                        cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
                    }
                    return KAL_TRUE;
                }
            }
            i = 0;
            while(i < 4)
            {
                G_parameter.cal_wk_time_struct.bat_average[i] = G_parameter.cal_wk_time_struct.bat_average[i+1];
                i++;
            }
            G_parameter.cal_wk_time_struct.bat_average[4] = volt;
            LOGD(L_UART, L_V6, "%d, %d��%d, %d��%d", G_parameter.cal_wk_time_struct.bat_average[0], G_parameter.cal_wk_time_struct.bat_average[1],
                 G_parameter.cal_wk_time_struct.bat_average[2], G_parameter.cal_wk_time_struct.bat_average[3],
                 G_parameter.cal_wk_time_struct.bat_average[4]);

        }

    }
    else
    {
        memset(G_parameter.cal_wk_time_struct.bat_average, 0x00, sizeof(G_parameter.cal_wk_time_struct.bat_average));
    }
#endif
    Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);

    return KAL_FALSE;


}

/******************************************************************************
 *  Function    -  track_cust_ext_teardown_alm
 *
 *  Purpose     -  ���𱨾�����
 *
 *  Description - par 1��������,2��ʱ���ص�
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 2016-01-30 xzq
 * ----------------------------------------
*******************************************************************************/

void track_cust_ext_teardown_alm(void* par)
{
    LBS_Multi_Cell_Data_Struct *lbs_strcut = track_drv_get_lbs_data();
    applib_time_struct tmp = {0};
    nvram_ext_chip_struct chip = {0}, chip2 = {0};
    kal_uint32 tick = 0;
    static kal_uint32 cur_tick = 0;
    kal_uint8 parameter = (kal_uint8)par;
    kal_uint8 tr_mode = 0;
    LOGD(L_UART, L_V5, "parameter %d", parameter);
    if(G_parameter.teardown_str.sw == 1 || G_parameter.extchip.mode == 1) //����ģʽһ��������ػ�
    {
        if(G_parameter.teardown_str.sw == 0)
        {
            return;
        }
        if(parameter == 1)
        {
            //���ź�ģʽ�л�������������
            if(G_parameter.extchip.mode != 1)
            {
                track_cust_module_gps_boot_time();// �˴��ǿ��ǿ����󴥷��ķ���
                track_cust_gps_work_req((void *)18);
            }
            if(cur_tick == 0)
            {
                cur_tick = kal_get_systicks();
            }
            else
            {
                tick = kal_get_systicks();
                if(tick - cur_tick > KAL_TICKS_1_MIN * 2)
                {
                    cur_tick = 0;
                }
                else
                {
                    LOGD(L_UART, L_V5, "���𱨾��������δ����2����!");
                    return;
                }
            }
            if(G_parameter.teardown_str.alarm_type > 0 && G_parameter.teardown_str.alarm_type < 3) // 1��2
            {
                //����
                tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_SEND_ALARM_MSG_TIMER_ID, 30 * 1000, track_cust_delay_send_msg);
            }
            track_cust_remove_alarm_lock(0);//����
            tr_mode = track_cust_judge_tracking_time(&chip);
            LOGD(L_UART, L_V5, "mode %d, m1 %d,m2 %d, m1time %d", chip.mode, chip.mode1_par1, chip.mode1_par2, chip.mode1_time);
            if(tr_mode && chip.mode != 0)
            {
                track_cust_module_tracking_mode(1);
                memcpy(&G_parameter.extchip, &chip, sizeof(nvram_ext_chip_struct));
                if(Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE))
                {
                    track_cust_module_update_mode();
                    //track_cust_upload_param();//����״̬�ı��ϴ�ֻ��01��ѯʱ��һ���ϴ�
                    tr_start_timer(TRACK_CUST_EXTCHIP_TRACK_MODE_TIMEROUT_TIMER_ID, G_realtime_data.tracking_time * 60 * 1000, track_cust_module_track_mode_timeout);
                }
                else
                {
                    LOGD(L_UART, L_V5, "error write nvram 05!");
                }
            }
            else
            {
                LOGD(L_UART, L_V7, "���������׷��ģʽ %d", chip.mode);
            }
        }
        if(lbs_strcut->MainCell_Info.cell_id == 0)
        {
            LOGD(L_APP, L_V5, "cell_id is Zero, cannot upload delay 5s! ");
            track_start_timer(TRACK_CUST_EXTCHIP_DELAY_FOR_LBS_TIMER_ID, 5000, track_cust_ext_teardown_alm, (void*)2);
            cur_tick = kal_get_systicks();//û������һ��������,������������
            return;
        }
        //tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_SEND_ALARM_TIMER_ID, 30*1000, track_cust_delay_alarm);
        track_os_intoTaskMsgQueue(track_cust_pre_remove_alarm);

    }
    else if(G_parameter.extchip.mode != 1)
    {
        track_cust_module_delay_close(0);
    }
}

void track_cust_send03(void* delay_off_sec)
{
    const int sec = (int)delay_off_sec;
    if(G_parameter.extchip.mode != 1)
    {
        char buf[100] = {0};
        sprintf(buf, "AT^GT_CM=03,%d#\r\n", sec);
        track_drv_write_exmode(buf, strlen(buf));
        LOGD(L_UART, L_V5, "delay shutoff %d sec", sec);
        track_start_timer(TRACK_CUST_EXTCHIP_WAIT03_TIMER_ID, RESEND_COMMAND_TIMEOUT_SEC, track_cust_send03, delay_off_sec);
    }
}

static void track_cust_send02()
{
    U8 buf[50];
    applib_time_struct time = {0};
    track_fun_get_time(&time, KAL_FALSE, &G_parameter.zone.zone);
    snprintf(buf, 50, "AT^GT_CM=02,%d,%d,%d,%d,%d#\r\n", time.nHour, time.nMin, time.nYear - 2000, time.nMonth, time.nDay);
    track_drv_write_exmode(buf, strlen(buf));
    tr_start_timer(TRACK_CUST_EXTCHIP_WAIT02_TIMER_ID, RESEND_COMMAND_TIMEOUT_SEC, track_cust_send02);
    LOGD(L_UART, L_V5, "[%s]", buf);
}


static void track_cust_send04(void* par)
{
    U8 buf[100];
    kal_uint8 mode = (kal_uint32)par;
    if(mode == 1)
    {
        sprintf(buf, "AT^GT_CM=04,1,1,1800,0#\r\n");
    }
    else if(mode == 2)
    {
        if(G_parameter.extchip.mode2_par2 == 0) G_parameter.extchip.mode2_par2 = 24;
        sprintf(buf, "AT^GT_CM=04,1,2,%d,%d#\r\n", G_parameter.extchip.mode2_par2, G_parameter.extchip.mode2_par1);
    }
    else if(3 == mode)
    {
        sprintf(buf, "AT^GT_CM=04,1,3,%d,%d#\r\n", G_parameter.extchip.mode3_par1, G_parameter.extchip.mode3_par2);
    }
    else if(4 == mode)
    {
        char m_buf[100] = {0};
        U8 vle = G_parameter.extchip.mode4_par1, i = 0;
        for(; i < vle; i++)
        {
            if(0 == i)
            {
                snprintf(m_buf + strlen(m_buf), 99, "%d", G_parameter.extchip.mode4_par2[i]);
            }
            else
            {
                snprintf(m_buf + strlen(m_buf), 99, ":%d", G_parameter.extchip.mode4_par2[i]);
            }
        }
        sprintf(buf, "AT^GT_CM=04,1,4,%d,%s#\r\n", G_parameter.extchip.mode4_par1, m_buf);
    }
    else if(0 == mode)
    {
        sprintf(buf, "AT^GT_CM=04,0,0,0,0#\r\n");
    }
    else
    {
        LOGD(L_UART, L_V5, "��������!==mode %d==", mode);
        return;
    }
    track_drv_write_exmode(buf, strlen(buf));
    track_start_timer(TRACK_CUST_EXTCHIP_WAIT_TIMER_ID, RESEND_COMMAND_TIMEOUT_SEC, track_cust_send04, par);
    LOGD(L_UART, L_V5, "[%s]", buf);
}

static void track_cust_send05(void * par)
{
    U8 stbuf[30] = {0}, op = (U8)par;
    sprintf(stbuf, "AT^GT_CM=05,%d#\r\n", op);
    track_drv_write_exmode(stbuf, sizeof(stbuf));
    track_start_timer(TRACK_CUST_EXTCHIP_WAIT05_TIMER_ID, RESEND_COMMAND_TIMEOUT_SEC, track_cust_send05, par);
    LOGD(L_UART, L_V5, "[%s]", stbuf);
}

//��Ƭ���洢���ĵĵ���ֵ
static void track_cust_send10(U8 op, float value)
{
    U8 buf[30];
    if(op == 1)
    {
        sprintf(buf, "AT^GT_CM=10,1,%.0f#\r\n", value); // д
    }
    else
    {
        sprintf(buf, "AT^GT_CM=10,0,0#\r\n");
    }
    track_drv_write_exmode(buf, strlen(buf));
    //track_start_timer(TRACK_CUST_EXTCHIP_WAIT10_TIMER_ID, 5000, track_cust_send10, par);
    LOGD(L_UART, L_V5, "[%s]", buf);
}
static void track_cust_send11()
{
    U8 buf[30];
    sprintf(buf, "AT^GT_CM=11,OK#\r\n");
    track_drv_write_exmode(buf, strlen(buf));
    //track_start_timer(TRACK_CUST_EXTCHIP_WAIT_TIMER_ID, 5000, track_cust_send04, par);
    LOGD(L_UART, L_V5, "[%s]", buf);
}

static void track_cust_send13(void* pt)
{
    U8 buf[30];
    U8 par = (U8)pt;
    if(par == 4)
    {
        sprintf(buf, "AT^GT_CM=13,%d#\r\n", par);
        track_start_timer(TRACK_CUST_EXTCHIP_WAIT13_TIMER_ID, RESEND_COMMAND_TIMEOUT_SEC, track_cust_send13, pt);
    }
    else
    {
        sprintf(buf, "AT^GT_CM=13,%d,OK#\r\n", par);
    }
    track_drv_write_exmode(buf, strlen(buf));
    LOGD(L_UART, L_V5, "[%s]", buf);
}

static void track_cust_send99()
{
    char buf[30] = {0};
    sprintf(buf, "AT^GT_CM=99#\r\n");
    tr_start_timer(TRACK_CUST_EXTCHIP_WAIT99_TIMER_ID, RESEND_COMMAND_TIMEOUT_SEC, track_cust_send99);
    track_drv_write_exmode(buf, strlen(buf));
    LOGD(L_UART, L_V5, "[%s]", buf);
}

static void cmd_01(void)
{
    //AT^GT_CM=01,KKS_GT700_V3.0_1.12,286,0:0,2,60,575,6,270,0,1199#
    //AT^GT_CM=01,GT700_V1.01,258,13:05,01,6,300,10,275,0,1,CRC#
    int mode, i, sum = 0;
    char *p, tmp[200] = {0};
    static U8 t_j = 0, recode_time = 0;
    LOGD(L_UART, L_V1, "");
    if(track_is_timer_run(TRACK_CUST_CALCULATE_ELE_TIMER_ID))
    {
        tr_stop_timer(TRACK_CUST_CALCULATE_ELE_TIMER_ID);
    }
    if(track_is_timer_run(TRACK_CUST_EXTCHIP_BATTERY_VOLTAGE_TIMER_ID))
    {
        tr_stop_timer(TRACK_CUST_EXTCHIP_BATTERY_VOLTAGE_TIMER_ID);
    }
    for(i = 0; i < cmd_parameters_count; i++)
    {
        if(i >= 2 && i < cmd_parameters_count - 1)
        {
            char *p2;
            sum += atoi(cmd_parameters[i]);
            p = strchr(cmd_parameters[i], ':');
            while(p != NULL)
            {
                if(t_j == 0)
                {
                    st_time[t_j++] = atoi(cmd_parameters[i]);
                    LOGD(L_UART, L_V8, "%d", st_time[t_j-1]);
                }
                if(t_j < sizeof(st_time))
                {
                    st_time[t_j++] = atoi(p + 1);
                    LOGD(L_UART, L_V8, "%d", st_time[t_j-1]);
                }
                sum += atoi(p + 1);
                p++;//��ǰ��һλ
                if(strchr(p, ':') != NULL)
                {
                    p = strchr(p, ':');
                }
                else
                {
                    break;
                }
            }

        }
        LOGD(L_UART, L_V6, "[%d,%d] %s -> %d,%d\r\n", i, t_j, cmd_parameters[i], sum, atoi(cmd_parameters[i]));
    }
    t_j = 0;
    if(sum != atoi(cmd_parameters[cmd_parameters_count - 1]))
    {
        LOGD(L_UART, L_V1, "ERROR: У�鲻ͨ��. %d,%d", sum, atoi(cmd_parameters[cmd_parameters_count - 1]));
        tr_start_timer(TRACK_CUST_EXTCHIP_BATTERY_VOLTAGE_TIMER_ID, 5000, track_cust_module_status_inquiry);
        return;
    }
    //if(first_syn && (G_parameter.cal_wk_time_struct.gsm_alrigothm_logic && G_parameter.cal_wk_time_struct.gsm_volt_low_time>=3))
#if defined (__MCU_SW_UPDATE__)
    memset(mcu_sw_version_string, 0x00, sizeof(mcu_sw_version_string));
    snprintf(mcu_sw_version_string, sizeof(mcu_sw_version_string) - 1, "%s", cmd_parameters[1]);
#endif /* __MCU_SW_UPDATE__ */
    if(G_parameter.cal_time_sw == 0 || (track_is_testmode() == KAL_TRUE))
    {
        track_cust_set_battery_volt(atoi(cmd_parameters[ST_VOL]));
    }
    track_cust_set_real_battery_volt(atoi(cmd_parameters[ST_VOL]));
    if(track_is_testmode())
    {
        snprintf(tmp, 150, "version:(%s)\r\nvoltage:(%s);\r\ntime:%s;  update time:%s; low voltage:%s; key_satus:%s; rst %s",
                 cmd_parameters[ST_VERSION], cmd_parameters[ST_VOL], cmd_parameters[ST_TIME],
                 cmd_parameters[ST_UPTIME], cmd_parameters[ST_LOWVOL], cmd_parameters[ST_KEYSTATUS],
                 cmd_parameters[ST_RST]);
    }
    else
    {
        snprintf(tmp, 200, "version:%s\r\nvoltage:%s;\r\ntime:%s; update time:%s; low voltage:%s; key_satus:%s; rst %s",
                 cmd_parameters[ST_VERSION], cmd_parameters[ST_VOL], cmd_parameters[ST_TIME],
                 cmd_parameters[ST_UPTIME], cmd_parameters[ST_LOWVOL], cmd_parameters[ST_KEYSTATUS],
                 cmd_parameters[ST_RST]);
    }
    if(recode_time == 0)
    {
        applib_time_struct sttime = {0};
        applib_time_struct sttime2 = {0};
        nvram_gps_time_zone_struct  zone1 = {0};
        applib_time_struct *currTime = NULL;
        memcpy(&zone1, &G_parameter.zone, sizeof(nvram_gps_time_zone_struct));
        zone1.zone = 'W'; //��ǰ
        currTime = &sttime2;
        if(st_time[styear] != 0)
        {
            sttime.nYear = st_time[styear] + 2000;
            sttime.nMonth = st_time[stmonth];
            sttime.nDay = st_time[stday];
            sttime.nHour = st_time[sthour];
            sttime.nMin = st_time[stmin];
            sttime.nSec = 0;
            track_drv_utc_timezone_to_rtc(currTime, &sttime, zone1);
            track_fun_update_time_to_system(4, currTime);
        }
        if(G_parameter.extchip.mode != 1 && G_realtime_data.ams_log == 1)
        {
            char* sttime_str = NULL;
            sttime_str = (kal_uint8*)Ram_Alloc(2, strlen(cmd_parameters[ST_TIME]) + 3);
            if(sttime_str == NULL)
            {
                LOGD(L_UART, L_V1, "��̬�����ڴ����!");
            }
            else
            {
                snprintf(sttime_str, strlen(cmd_parameters[ST_TIME]) + 3, "%s,", cmd_parameters[ST_TIME]);
                track_cust_save_logfile(sttime_str, strlen(cmd_parameters[ST_TIME]) + 3);
                Ram_Free(sttime_str);
            }

        }

    }
    recode_time++;

    confirm(tmp);
    track_cust_send04((void*)0);
    if(G_realtime_data.switch_down != atoi(cmd_parameters[ST_KEYSTATUS]))
    {
        G_realtime_data.switch_down = atoi(cmd_parameters[ST_KEYSTATUS]);
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
        track_cust_upload_param();
    }
    st_rst = atoi(cmd_parameters[ST_RST]);
    if(G_parameter.cal_time_sw == 1)
    {
        if(first_syn != 1)
        {
            U32 v;
            first_volt = atoi(cmd_parameters[ST_VOL]);
            if(first_volt > 370 || first_volt < 180)
            {
                LOGD(L_UART, L_V1, "����ѹ������ֵ��(1.8-3.7) %dv", first_volt);
                return;
            }
            if(atoi(cmd_parameters[ST_RST]) == 1)
            {
                single_rst = 1;
                LOGD(L_UART, L_V1, "��Ƭ���״��ϵ�, ��ʷ���ĵ���ֵ %.0f!!!!", G_parameter.cal_wk_time_struct.comsultion_ele);
                if(G_realtime_data.netLogControl & 32)
                {
                    char buf[100];
                    sprintf(buf, "single first power on %d  history comsule%.0f!!!!", first_volt, G_parameter.cal_wk_time_struct.comsultion_ele);
                    cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
                }
                // ��ʼ������
                if(!track_cust_existence_history_Date())
                {
                    {
                        LOGD(L_UART, L_V5, "��ʼ���� volt %d", first_volt);

                        if(first_volt >= 295)
                        {
                            track_cust_set_battery_volt(first_volt);
                            G_parameter.cal_wk_time_struct.comsultion_ele = 0;
                            Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
                        }
                        else if(first_volt <= 285)
                        {
                            U16 ele;
                            ele = track_cust_calibration_table(first_volt);
                            G_parameter.cal_wk_time_struct.comsultion_ele = (100 - ele) * InitElet / 100.0; // ���ĵĵ���ֵ
                            if(ele > 100)
                            {
                                LOGD(L_UART, L_V3, "�����ٷֱȴ��� ele %d", ele);
                                return;
                            }
                            LOGD(L_UART, L_V6, "ele %d", ele);
                            Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
                        }
                        else
                        {
                            G_parameter.cal_wk_time_struct.comsultion_ele = (100 - BOOT_ELE) * InitElet / 100.0;
                            Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
                        }
                    }
                }
            }
            if(G_realtime_data.netLogControl & 32)
            {
                char buf[10];
                sprintf(buf, "volt %d ", first_volt);
                cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
            }

            tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_SHOWELE_TIMER_ID, 5000, track_cust_module_cal_electricity);
            //����У׼
            if(first_syn == 0xff)
            {
                applib_time_struct time = {0};
                U32 timep;
                track_fun_get_time(&time, KAL_FALSE, &G_parameter.zone.zone);
                timep = OTA_applib_dt_mytime_2_utc_sec(&time, 0);

                // ������ж�
                if(track_cust_statistics_volt_for_Swtbat(first_volt, timep) && single_rst == 1)
                {
                    track_cust_resetbat();
                }
                else
                {
                    track_cust_calibration_volt(timep);
                }


            }
            first_syn = 1;
        }
    }
}

static void cmd_02(void)
{
    U8 buf[100] = {0};
    LOGD(L_UART, L_V5, "");
    confirm("02 OK!");
    track_stop_timer(TRACK_CUST_EXTCHIP_WAIT02_TIMER_ID);

    if(!g_02_ok) return;
    track_cust_module_update_mode();
}

static void cmd_03(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("03 OK!");
    if(track_is_timer_run(TRACK_CUST_EXTCHIP_WAIT03_TIMER_ID))
    {
        track_stop_timer(TRACK_CUST_EXTCHIP_WAIT03_TIMER_ID);
        return;
    }
    if((track_is_testmode() == KAL_TRUE))
    {
        tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_SHUTDOWN_TIMER_ID, 500, track_drv_sys_power_off_req);
    }
    else
    {
        track_stop_timer(TRACK_POWER_RESET_TIMER_ID);
        track_drv_sys_power_off_req();
    }
}

static void cmd_04(void)
{
    U8 i = 0;
    U8 mode = atoi(cmd_parameters[2]);
    LOGD(L_UART, L_V5, "A %d,B %d,C %d", mode, atoi(cmd_parameters[3]), atoi(cmd_parameters[4]));
    confirm("04 OK!");
    track_stop_timer(TRACK_CUST_EXTCHIP_WAIT_TIMER_ID);
    if(G_parameter.extchip.mode != mode)
    {
        track_cust_module_update_mode();
    }
    else if(mode == 2)
    {
        if(atoi(cmd_parameters[3]) != G_parameter.extchip.mode2_par2)
        {
            track_cust_module_update_mode();
        }
        else if(G_parameter.extchip.mode2_par1 != 0 && G_parameter.extchip.mode2_par1 != atoi(cmd_parameters[4]))
        {
            track_cust_module_update_mode();
        }
    }
    else if(mode == 3)
    {
        if(atoi(cmd_parameters[3]) != G_parameter.extchip.mode3_par1)
        {
            track_cust_module_update_mode();
        }
        else if(G_parameter.extchip.mode3_par2 != 0 && G_parameter.extchip.mode3_par2 != atoi(cmd_parameters[4]))
        {
            track_cust_module_update_mode();
        }

    }
    else if(mode == 4)
    {
        U8 imode = 0;
        char m_buf[100] = {0};
        U8 vle = G_parameter.extchip.mode4_par1, i = 0;
        if(atoi(cmd_parameters[3]) != G_parameter.extchip.mode4_par1)
        {
            track_cust_module_update_mode();
        }
        for(; i < vle; i++)
        {
            if(0 == i)
            {
                snprintf(m_buf + strlen(m_buf), 99, "%d", G_parameter.extchip.mode4_par2[i]);
            }
            else
            {
                snprintf(m_buf + strlen(m_buf), 99, ":%d", G_parameter.extchip.mode4_par2[i]);
            }
        }
        LOGD(L_UART, L_V7, "%s==%s", m_buf, cmd_parameters[4]);
        if(G_parameter.extchip.mode4_par2 != 0 && memcmp(&m_buf[0], cmd_parameters[4], strlen(cmd_parameters[4])))
        {
            track_cust_module_update_mode();
        }

    }
}

static void cmd_05(void)
{
    U8 buf[100] = {0};
    confirm("(Short press)!");
    if(cmd_parameters_count == 3)
    {
        tr_stop_timer(TRACK_CUST_EXTCHIP_WAIT05_TIMER_ID);
        return;
    }
    LOGD(L_UART, L_V5, "���𱨾�����!");
    sprintf(buf, "AT^GT_CM=05,OK#\r\n");
    track_drv_write_exmode(buf, strlen(buf));

    if((track_is_testmode() == KAL_TRUE))
    {
        return;
    }
    track_cust_ext_teardown_alm((void*)1);
    if(track_cust_gps_status() >= GPS_STAUS_2D)
    {
        track_gps_data_struct *p_gpsdata = track_cust_backup_gps_data(0, NULL);
        if(p_gpsdata->gprmc.status != 0 && (p_gpsdata->gprmc.Latitude != 0 || p_gpsdata->gprmc.Longitude != 0))
        {
            track_cust_paket_position(p_gpsdata, track_drv_get_lbs_data(), TR_CUST_POSITION_KEY, 0);
        }
    }
    else
    {
        track_cust_sendlbs_limit();
    }
}

static void cmd_06(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("(Long press)!");
}

static void cmd_07(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("07 OK!");
}

static void cmd_08(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("08 OK!");
    track_cust_send10(0, 0);
    track_cust_module_status_inquiry();
}

static void cmd_09(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("09 OK!");
}

static void cmd_10(void)
{
    static U8 status = 0;
    confirm("10 OK!");
    if((track_is_testmode() == KAL_TRUE))
    {
        return;
    }
    single_ele_value = atoi(cmd_parameters[1]);
    LOGD(L_UART, L_V5, "single_ele_value %d", single_ele_value);
}

static void cmd_11(void)
{
    U8 status = 0, p1, p2, p3, singlebuff[100] = {0};
    confirm("11 OK!");
    snprintf(singlebuff, 99, "SC LOG %s ", cmd_parameters[1]);
#if 0//defined(__AMS_LOG__)
    track_cust_cache_netlink(singlebuff, strlen(singlebuff));
    track_cust_cache_sucssful_log(singlebuff, strlen(singlebuff));
#endif
    LOGD(L_UART, L_V5, "SC LOG %s", cmd_parameters[1]);
    if(G_realtime_data.netLogControl & 32)
    {
        cust_packet_log_data(8, (kal_uint8 *)singlebuff, strlen((char *)singlebuff));
    }
}
static void cmd_12(void)
{
    U8 buf[20];
    LOGD(L_UART, L_V5, "");
    confirm("12 OK!");
    sprintf(buf, "AT^GT_CM=12,OK#\r\n");
    track_drv_write_exmode(buf, strlen(buf));
}

static void cmd_13(void)
{
    U8 par = atoi(cmd_parameters[1]);
    char buf[20] = {0};
    LOGD(L_UART, L_V5, "");
    sprintf(&buf[0], "13,%d OK!", par);
    confirm(&buf[0]);
    if(4 == par)
    {
        track_stop_timer(TRACK_CUST_EXTCHIP_WAIT13_TIMER_ID);
    }
    else
    {
        wakeup_mode = par;
        track_cust_send13((void*)par);
        switch(par)
        {
            case 1:
                LOGD(L_UART, L_V1, "��������GSM");
                track_cust_gps_off_req((void*)6);
                break;
            case 2:
                LOGD(L_UART, L_V1, "AMS���ӻ���GSM");
                track_cust_gps_off_req((void*)6);
                break;
            default:
                LOGD(L_UART, L_V1, "��������");
                break;
        }
    }
}

static void cmd_99(void)
{
#if defined (__MCU_SW_UPDATE__)
    track_drv_mcu_sw_update_rsp();
#endif /* __MCU_SW_UPDATE__ */
    LOGD(L_UART, L_V5, "");
    confirm("99 OK!");
}

static cust_recv_struct CMD_Data[] =
{
    {"01",        cmd_01},
    {"02",        cmd_02},
    {"03",        cmd_03},
    {"04",        cmd_04},
    {"05",        cmd_05},
    {"06",        cmd_06},
    {"07",        cmd_07},
    {"08",        cmd_08},
    {"08 OK",     cmd_08},
    {"09 OK",     cmd_09},
    {"09",        cmd_09},
    {"10",        cmd_10},
    {"11",        cmd_11},
    {"12",        cmd_12},
    {"13",        cmd_13},
    {"99",        cmd_99}
};

//�����λ��Э��
void track_cust_module_resetpro()
{
    G_realtime_data.TPRO_COUNT = 0;
    G_parameter.new_position_sw = 0;
    Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
    Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
    LOGD(L_UART, L_V1, "�������ö�λ��Э��!");
}

//׷��ģʽ��ʱ20����δ���������
void track_cust_module_track_mode_timeout()
{
    track_stop_timer(TRACK_CUST_EXTCHIP_TRACK_MODE_TIMEROUT_TIMER_ID);
    if(G_realtime_data.tracking_mode == 1 && !track_is_testmode())
    {
        auto_mode_callback((void*)98);
        track_cust_gps_control(0);
        LOGD(L_UART, L_V5, "==׷��ģʽ����b_track_mode %d==", G_realtime_data.tracking_mode);
        //track_cust_module_extchip_shutdown();
    }
}

void track_cust_modeule_Supplements_data()
{
    LOGD(L_UART, L_V2, "had supplements data, notify st!");
    track_cust_send13((void*)4);
}

kal_uint8 track_cust_modeule_get_wakemode()
{
    return wakeup_mode;
}

void track_cust_upload_param(void)
{
    kal_uint8 *sendData;
    char data[300] = {0}, sos[68] = {0}, center[26] = {0}, fence[70] = {0}, m_buf[100] = {0};
    char in_out[10] = {0};
    kal_uint16 p1, p2, p3;

    sendData = (U8*)Ram_Alloc(5, 5);
    if(sendData == NULL)
    {
        return;
    }
#if defined(__MQ_24PAKET__)||defined(__SERVER_GTSX__)||defined(__SERVER_GPSLUCKLY__)||defined(__JIMISHARE__)
    return;//04�Ӱ���ʱ��ET130��ET200��׼���У�CJJ 20151009
#endif
    memset(sendData, 0, 5);
    /*
    ��7   Bit,�𶯱���   ����
    ��6   Bit,���籨��
    ��5   Bit,�绰����
    ��4   Bit,���ű���

    ��3   Bit,λ�Ʊ���   ����
    ��2   Bit,���籨��
    ��1   Bit,�绰����
    ��0   Bit,���ű���
    */
    if(G_parameter.displacement_alarm.alarm_type == 0)
    {
        sendData[0] |= 0x04;
    }
    if(G_parameter.displacement_alarm.alarm_type == 1)
    {
        sendData[0]   |= 0x05;
    }
    if(G_parameter.displacement_alarm.alarm_type == 2)
    {
        sendData[0]   |= 0x07;
    }
    if(G_parameter.displacement_alarm.alarm_type == 3)
    {
        sendData[0]   |= 0x06;
    }
    if(G_parameter.displacement_alarm.sw == 1)
    {
        sendData[0]   |= 0x08 ;
    }
    if(G_parameter.vibrates_alarm.alarm_type == 0)
    {
        sendData[0]  |= 0x40;
    }
    if(G_parameter.vibrates_alarm.alarm_type == 1)
    {
        sendData[0]   |= 0x50 ;
    }
    if(G_parameter.vibrates_alarm.alarm_type == 2)
    {
        sendData[0]   |= 0x70;
    }
    if(G_parameter.vibrates_alarm.alarm_type == 3)
    {
        sendData[0]   |= 0x60;
    }
    if(G_parameter.vibrates_alarm.sw == 1)
    {
        sendData[0]   |= 0x80;
    }
    LOGD(L_APP, L_V5, "ALM1=%x", sendData[0]);
    /*
    ��7   Bit,�ڲ��͵籨��   ����
    ��6   Bit,���籨��
    ��5   Bit,�绰����
    ��4   Bit,���ű���

    ��3   Bit,�ⲿ�͵籨��   ����
    ��2   Bit,���籨��
    ��1   Bit,�绰����
    ��0   Bit,���ű���
        */
    if(G_parameter.power_extbatalm.alarm_type == 0)
    {
        sendData[1] |= 0x04;
    }
    if(G_parameter.power_extbatalm.alarm_type == 1)
    {
        sendData[1]   |= 0x05;
    }
    if(G_parameter.power_extbatalm.alarm_type == 2)
    {
        sendData[1]   |= 0x07;
    }
    if(G_parameter.power_extbatalm.alarm_type == 3)
    {
        sendData[0]   |= 0x06;
    }
    if(G_parameter.power_extbatalm.sw == 1)
    {
        sendData[1]   |= 0x08 ;
    }
    if(G_parameter.low_power_alarm.alarm_type == 0)
    {
        sendData[1]  |= 0x40;
    }
    if(G_parameter.low_power_alarm.alarm_type == 1)
    {
        sendData[1]   |= 0x50 ;
    }
    if(G_parameter.low_power_alarm.alarm_type == 2)
    {
        sendData[1]   |= 0x70;
    }
    if(G_parameter.low_power_alarm.alarm_type == 3)
    {
        sendData[0]   |= 0x60;
    }
    if(G_parameter.low_power_alarm.sw == 1)
    {
        sendData[1]   |= 0x80;
    }

    /*
    ��7   Bit,���ٱ���   ����
    ��6   Bit,���籨��
    ��5   Bit,�绰����
    ��4   Bit,���ű���

    ��3   Bit,�ϵ籨��   ����
    ��2   Bit,���籨��
    ��1   Bit,�绰����
    ��0   Bit,���ű���
        */
    if(G_parameter.power_fails_alarm.alarm_type == 0)
    {
        sendData[2] |= 0x04;
    }
    if(G_parameter.power_fails_alarm.alarm_type == 1)
    {
        sendData[2]   |= 0x05;
    }
    if(G_parameter.power_fails_alarm.alarm_type == 2)
    {
        sendData[2]   |= 0x07;
    }
    if(G_parameter.power_fails_alarm.alarm_type == 3)
    {
        sendData[0]   |= 0x06;
    }
    if(G_parameter.power_fails_alarm.sw == 1)
    {
        sendData[2]   |= 0x08 ;
    }
    if(G_parameter.speed_limit_alarm.alarm_type == 0)
    {
        sendData[2]  |= 0x40;
    }
    if(G_parameter.speed_limit_alarm.alarm_type == 1)
    {
        sendData[2]   |= 0x50 ;
    }
    if(G_parameter.speed_limit_alarm.alarm_type == 2)
    {
        sendData[2]   |= 0x70;
    }
    if(G_parameter.speed_limit_alarm.alarm_type == 3)
    {
        sendData[0]   |= 0x60;
    }
    if(G_parameter.speed_limit_alarm.sw == 1)
    {
        sendData[2]   |= 0x80;
    }

    /*
    ��7   Bit,���״̬  �����1
    ��6   Bit,�Զ����
    ��5   Bit,�ֶ����
    ��4   Bit,���𿪹�  ���� 1

    ��3   Bit,
    ��2   Bit,
    ��1   Bit,
    ��0   Bit,
    */
    if(G_realtime_data.defense_mode == 1)
    {
        sendData[3]   |= 0x20 ; //�ֶ�
    }
    else if(G_realtime_data.defense_mode == 2)
    {
        sendData[3]   |= 0x10 ; //�Զ�����ֶ�����
    }
    else
    {
        sendData[3]   |= 0x40 ; //�Զ�
    }
    if(G_realtime_data.defences_status == 3 || G_realtime_data.defences_status == 2)
    {
        sendData[3]   |= 0x80 ;
    }
    if(G_realtime_data.switch_down == 1)
    {
        sendData[3] |= 0x02;
    }
    if(G_parameter.teardown_str.sw == 1) //���𱨾�����
    {
        sendData[3] |= 0x01;
    }
    /*
    ��7   Bit,
    ��6   Bit,
    ��5   Bit,
    ��4   Bit,

    ��3   Bit,����Ͽ�������δ��
    ��2   Bit,����Ͽ���δ��λ
    ��1   Bit,�Ͽ��͵�
    ��0   Bit,��ͨ�͵�
        */
    if(G_realtime_data.oil_status == 0)
    {
        sendData[4] |= 0x01;
    }
    if(G_realtime_data.oil_status == 1)
    {
        sendData[4]   |= 0x02;
    }
    if(G_realtime_data.oil_status == 2)
    {
        sendData[4]   |= 0x04;
    }
    if(G_realtime_data.oil_status == 3)
    {
        sendData[4]   |= 0x08 ;
    }
    /*      if(G_realtime_data.oil_status == 4)
          {
              sendData  |= 0x10;
          } */

    //SOS����
    sprintf(sos, "SOS=%s,%s,%s", G_parameter.sos_num[0], G_parameter.sos_num[1], G_parameter.sos_num[2]);
    //���ĺ���
    sprintf(center, "CENTER=%s",  G_parameter.centerNumber);
    //Χ��
    if(G_parameter.fence[0].in_out == 0)
        sprintf(in_out, "IN");
    else if(G_parameter.fence[0].in_out == 1)
        sprintf(in_out, "OUT");
    else if(G_parameter.fence[0].in_out == 2)
        sprintf(in_out, "IN or OUT");
    if(G_parameter.fence[0].square == 0)
    {
        sprintf(fence, "Fence,%s,%d,%.6f,%.6f,%d,%s,%d",
                ON_OFF(G_parameter.fence[0].sw),
                G_parameter.fence[0].square,
                G_parameter.fence[0].lat,
                G_parameter.fence[0].lon,
                G_parameter.fence[0].radius * 100,
                in_out, G_parameter.fence[0].alarm_type);
    }
    else
    {
        sprintf(fence, "Fence,%s,%d,%.6f,%.6f,%.6f,%.6f,%s,%d",
                ON_OFF(G_parameter.fence[0].sw),
                G_parameter.fence[0].square,
                G_parameter.fence[0].lat,
                G_parameter.fence[0].lon,
                G_parameter.fence[0].lat2,
                G_parameter.fence[0].lon2,
                in_out, G_parameter.fence[0].alarm_type);
    }
    if(G_parameter.extchip.mode == 1)
    {
        sprintf(m_buf, "MODE=MODE,%d,%d,%d", G_parameter.extchip.mode, G_parameter.gps_work.Fti.interval_acc_off, G_parameter.extchip.mode1_par2);
    }
    else if(G_parameter.extchip.mode == 2)
    {
        if(G_parameter.extchip.mode2_par2 % 60 == 0)
        {
            sprintf(m_buf, "MODE=MODE,%d,%0.2d:%0.2d,%d", G_parameter.extchip.mode, G_parameter.extchip.mode2_par1 / 60, G_parameter.extchip.mode2_par1 % 60, G_parameter.extchip.mode2_par2 / 60);
        }
        else
        {
            sprintf(m_buf, "MODE=MODE,%d,%0.2d:%0.2d,%d(MIN)", G_parameter.extchip.mode, G_parameter.extchip.mode2_par1 / 60, G_parameter.extchip.mode2_par1 % 60, G_parameter.extchip.mode2_par2);
        }
    }
    else if(G_parameter.extchip.mode == 3)
    {
        sprintf(m_buf, "MODE=MODE,%d,%d,%0.2d:%0.2d", G_parameter.extchip.mode, G_parameter.extchip.mode3_par1,
                G_parameter.extchip.mode3_par2 / 60, G_parameter.extchip.mode3_par2 % 60);
    }
    else if(G_parameter.extchip.mode == 4)
    {
        U8 vle = G_parameter.extchip.mode4_par1, i = 0;
        char m_buf1[100] = {0};
        for(; i < vle; i++)
        {
            if(0 == i)
            {
                snprintf(m_buf1 + strlen(m_buf1), 99, "%0.2d:%0.2d", G_parameter.extchip.mode4_par2[i] / 60, G_parameter.extchip.mode4_par2[i] % 60);
            }
            else
            {
                snprintf(m_buf1 + strlen(m_buf1), 99, ",%0.2d:%0.2d", G_parameter.extchip.mode4_par2[i] / 60, G_parameter.extchip.mode4_par2[i] % 60);
            }
        }
        snprintf(m_buf, 99, "MODE=MODE,%d,%d,%s", G_parameter.extchip.mode, G_parameter.extchip.mode4_par1, m_buf1);
    }
    snprintf(data, sizeof(data) - 1, "ALM1=%0.2X;ALM2=%0.2X;ALM3=%0.2X;STA1=%0.2X;DYD=%0.2X;%s;%s;FENCE=%s;ICCID=%s;%s;RST=%d;", sendData[0], sendData[1], sendData[2], sendData[3], sendData[4], sos, center, fence, OTA_ell_get_iccid(), m_buf, st_rst);
    LOGD(L_APP, L_V5, "%s", data);
    track_cust_paket_94(04, data, strlen(data));
    Ram_Free(sendData);

}

void track_cust_module_power_cmd(cmd_return_struct *return_sms, kal_uint8 *data, int len)
{
    memcpy(&g_return_sms, return_sms, sizeof(cmd_return_struct));
    g_return_sms_valid = 1;
    track_drv_write_exmode(data, len);
}

void track_cust_syn_st_time(kal_uint32 arg, kal_uint8 app)
{
    U32 timep = 0;
    applib_time_struct time = {0};
    track_fun_get_time(&time, KAL_FALSE, &G_parameter.zone.zone);
    LOGD(L_UART, L_V5, "%d", arg);
    LOGD(L_UART, L_V5, "time: %0.2d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d",
         time.nYear,
         time.nMonth,
         time.nDay,
         time.nHour,
         time.nMin,
         time.nSec);
    if(time.nYear > (2000 + (*OTA_DEFAULT_HARDWARE_YEAR)))
    {
        U8 buf[100] = {0}, valid = 0;
        timep = OTA_applib_dt_mytime_2_utc_sec(&time, 0);
        if(app != 4)
        {
            track_cust_send02();
        }
        if(!g_02_ok)
        {
            g_02_ok = KAL_TRUE;
            if(first_volt == 0)
            {
                first_syn = 0XFF;// ��ѹδͬ��
            }
            else
            {
                // ������ж�
                if(track_cust_statistics_volt_for_Swtbat(first_volt, timep) && single_rst == 1)
                {
                    track_cust_resetbat();
                }
                else
                {
                    track_cust_calibration_volt(timep);
                }
            }
        }
        if(G_parameter.extchip.mode == 1 && G_parameter.extchip.mode1_time < DEFAULT_FEB_TIMESEC && G_parameter.extchip.mode1_time + arg >= DEFAULT_FEB_TIMESEC)
        {
            G_parameter.extchip.mode1_time += arg;
            valid = 1;
        }
        if(G_parameter.extchip.mode2_par1 == 0)
        {
            G_parameter.extchip.mode2_par1 = time.nHour * 60 + time.nMin;
            if(G_parameter.extchip.mode == 2) track_cust_module_update_mode();
            valid = 1;
        }
        if(valid) Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
        track_cust_module_auto_sw_mode();
    }
}

void track_cust_module_Handshake(void)
{
    char buf[30] = {0};
    sprintf(buf, "AT^GT_CM=08#\r\n");
    track_drv_write_exmode(buf, strlen(buf));
    LOGD(L_UART, L_V5, "");
    tr_start_timer(TRACK_CUST_EXTCHIP_BATTERY_VOLTAGE_TIMER_ID, 2000, track_cust_module_Handshake);
}

/******************************************************************************
 *  Function    -  track_cust_module_clearup
 *
 *  Purpose     -  �����ص���������ر���
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 2016-05-04 xzq
 * ----------------------------------------
*******************************************************************************/
void track_cust_module_clearup()
{
    gpsworksec = 0;
    gpssleepsec = 0;
    gsm_ele = 100;
    single_ele_value = 0;
    RESETBAT = 1;
}

void track_cust_resetbat()
{
    if(G_parameter.cal_time_sw)
    {
        track_work_time tmp_struct = {0};
        memcpy(&G_parameter.cal_wk_time_struct, &tmp_struct, sizeof(track_work_time));
        Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
        LOGD(L_UART, L_V2, "��������");
        track_cust_module_clearup();
        track_cust_set_battery_volt(293);
        track_cust_send10(1, 0);
        track_os_intoTaskMsgQueue(heartbeat_packets);
    }
}

void track_cust_printf_time()
{
    //LOGD(L_UART, L_V1, "GSM �͵�ѹ���� %d,%d, GPS����ʱ�� %d, GPS����ʱ�� %d",G_parameter.cal_wk_time_struct.gsm_volt_low_time,G_parameter.cal_wk_time_struct.gsm_alrigothm_logic,gpsworksec,gpssleepsec);
}

void track_cust_module_gps_sleep_time(void)
{
    if((track_is_testmode() == KAL_TRUE)|| G_parameter.extchip.mode == 2)
    {
        return;
    }
    if(G_parameter.cal_time_sw)
    {
        gpssleeptick = kal_get_systicks();
        LOGD(L_UART, L_V1, "GPS ���߿�ʼ���������ʱ�� %d", gpssleeptick);
        if(G_realtime_data.netLogControl & 32)
        {
            char buf[50];
            sprintf(buf, "GPS sleep time boot or reset %d", gpssleeptick);
            cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
        }
    }
}

void track_cust_module_gps_cal_sleep_time(void)
{
    kal_uint32 gsmtick;
    if((track_is_testmode() == KAL_TRUE))
    {
        return;
    }
    if(G_parameter.extchip.mode == 2 || G_parameter.cal_time_sw == 0)
    {
        return;
    }
    gpsclosesleeptick = kal_get_systicks();
    gsmtick = gpsclosesleeptick - gpssleeptick;
    gsmtick = gsmtick / KAL_TICKS_1_SEC;
    //G_parameter.cal_wk_time_struct.gps_sleep_time += gsmtick;
    gpssleepsec += gsmtick;
    //Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
    LOGD(L_UART, L_V1, "GPS ����ʱ��boot %d, %d, work sec %d, ���� %d", gpssleeptick, gpsclosesleeptick, gsmtick, gpssleepsec);
    if(G_realtime_data.netLogControl & 32)
    {
        char buf[100];
        sprintf(buf, "GPS cal sleep time, boottime %d, %d, work sec %d, thistime %d", gpssleeptick, gpsclosesleeptick, gsmtick, gpssleepsec);
        cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
    }
}
void track_cust_module_gps_boot_time(void)
{
    if((track_is_testmode() == KAL_TRUE) || G_parameter.extchip.mode == 2)
    {
        return;
    }
    if(G_parameter.cal_time_sw)
    {
        gpsboottick = kal_get_systicks();
        LOGD(L_UART, L_V1, "GPS�������������ʱ�� %d", gpsboottick);
        if(G_realtime_data.netLogControl & 32)
        {
            char buf[50];
            sprintf(buf, "GPS open boot or reset%d", gpsboottick);
            cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
        }
    }
}

void track_cust_module_gps_cal_time(void)
{
    kal_uint32 gsmtick;
    if((track_is_testmode() == KAL_TRUE))
    {
        return;
    }
    if(G_parameter.extchip.mode == 2 || G_parameter.cal_time_sw == 0)
    {
        return;
    }
    gpsclosetick = kal_get_systicks();
    gsmtick = gpsclosetick - gpsboottick;
    gsmtick = gsmtick / KAL_TICKS_1_SEC;
    //G_parameter.cal_wk_time_struct.gps_work_time += gsmtick;
    gpsworksec += gsmtick;
    //Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
    LOGD(L_UART, L_V1, "GPS����ʱ�� boot %d, %d, work sec %d, ����%d", gpsboottick, gpsclosetick, gsmtick, gpsworksec);
    if(G_realtime_data.netLogControl & 32)
    {
        char buf[100];
        sprintf(buf, "GPS work time cal, boottime %d, %d, work sec %d, this time%d", gpsboottick, gpsclosetick, gsmtick, gpsworksec);
        cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
    }
}

// 10����𱨾�delay
void track_cust_module_boot_cal()
{
    if((track_is_testmode() == KAL_TRUE))
    {
        return;
    }
    if(G_parameter.extchip.mode != 1 && mmi_Get_PowerOn_Type() != ABNRESET)
    {
        G_parameter.cal_wk_time_struct.comsultion_ele += (float)mode2_pwrconsumption;
        LOGD(L_UART, L_V1, "wake up times %.0f", G_parameter.cal_wk_time_struct.comsultion_ele);
        Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
    }
}

void track_cust_module_cal_electricity()
{
    kal_uint32 tracking_pwrcons = 0, volt;
    float ele_per = 0;
    static U8 fir = 0;
    if(G_parameter.cal_time_sw)
    {
        //if(!G_parameter.cal_wk_time_struct.gsm_alrigothm_logic || (G_parameter.cal_wk_time_struct.gsm_volt_low_time<3))
        if(fir == 0)
        {
            fir = 1;
            if(RESETBAT == 0 && G_parameter.cal_wk_time_struct.comsultion_ele == 0)
            {
                if(single_ele_value != 0 && single_ele_value != 1)
                {
                    G_parameter.cal_wk_time_struct.comsultion_ele = single_ele_value;
                    LOGD(L_UART, L_V1, "GSM ���ݶ�ʧ, �ӵ�Ƭ��������ĵ��� %d", single_ele_value);
                }
            }
        }
        {
            if(track_cust_get_battery_volt() == 0)
            {
                LOGD(L_UART, L_V1, "�״ο����������");
                //tracking_pwrcons = G_parameter.cal_wk_time_struct.gps_work_time * mode1_gps_pwrcons;
                //tracking_pwrcons += G_parameter.cal_wk_time_struct.gps_sleep_time * mode1_gps_sleeppwrcons;
                //tracking_pwrcons += G_parameter.cal_wk_time_struct.awakentimes * mode2_pwrconsumption;
                tracking_pwrcons = 0;
            }
            else if(track_cust_gps_status() == 0)
            {
                tracking_pwrcons = gpsworksec * mode1_gps_pwrcons;
                track_cust_module_gps_cal_sleep_time();//  ��������ʱ��
                tracking_pwrcons += gpssleepsec * mode1_gps_sleeppwrcons;
                //tracking_pwrcons += G_parameter.cal_wk_time_struct.awakentimes * mode2_pwrconsumption;
                track_cust_module_gps_sleep_time();
            }
            else
            {
                //tracking_pwrcons = G_parameter.cal_wk_time_struct.gps_sleep_time * mode1_gps_sleeppwrcons;
                tracking_pwrcons = gpssleepsec * mode1_gps_sleeppwrcons;
                track_cust_module_gps_cal_time();// ���㹤��ʱ��
                tracking_pwrcons += gpsworksec * mode1_gps_pwrcons;
                track_cust_module_gps_boot_time();
            }
            G_parameter.cal_wk_time_struct.comsultion_ele += (float)tracking_pwrcons;
            Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
            if(G_parameter.cal_wk_time_struct.comsultion_ele < InitElet)
            {
                gsm_ele = (U8)(((InitElet - G_parameter.cal_wk_time_struct.comsultion_ele) / InitElet) * 100.0);

            }
            else
            {
                gsm_ele = 0;
            }
            gpsworksec = 0;
            gpssleepsec = 0;
            LOGD(L_UART, L_V1, "���ĵ��� %.0f, �������� %d, ʣ������ٷֱ� %d", G_parameter.cal_wk_time_struct.comsultion_ele, tracking_pwrcons, gsm_ele);
            if(G_realtime_data.netLogControl & 32)
            {
                char buf[100];
                sprintf(buf, "full %.0f, thistime %d, leftele %d", G_parameter.cal_wk_time_struct.comsultion_ele, tracking_pwrcons, gsm_ele);
                cust_packet_log_data(8, (kal_uint8 *)buf, strlen((char *)buf));
            }
            volt = track_cust_conversion_volt(gsm_ele);
            //track_cust_send10(1,G_parameter.cal_wk_time_struct.comsultion_ele);
            track_cust_set_battery_volt(volt / 10000);
            tr_start_timer(TRACK_CUST_EXTCHIP_CAL_ELETICITY_TIMER_ID, CAL_ELETRCITY * 1000, track_cust_module_cal_electricity);
        }
    }
}

void track_cust_module_status_inquiry(void)
{
    char buf[100] = {0};
    sprintf(buf, "AT^GT_CM=01#\r\n");
    track_drv_write_exmode(buf, strlen(buf));
    LOGD(L_UART, L_V1, "");
    tr_start_timer(TRACK_CUST_EXTCHIP_BATTERY_VOLTAGE_TIMER_ID, 5000, track_cust_module_status_inquiry);
    //tr_start_timer(TRACK_CUST_EXTCHIP_BATTERY_VOLTAGE_TIMER_ID, 60000, track_cust_module_status_inquiry);
}

void track_cust_module_extchip_shutdown(void)
{
    if(G_parameter.extchip.mode != 1)
    {
        char buf[100] = {0};
        sprintf(buf, "AT^GT_CM=03,2#\r\n");
        track_drv_write_exmode(buf, strlen(buf));
        LOGD(L_UART, L_V5, "�·� 03ָ��");
        tr_start_timer(TRACK_POWER_RESET_TIMER_ID, 5000, track_cust_module_extchip_shutdown);
    }
}

static void track_cust_delay_timeout_close()
{
    //LOGD(L_UART, L_V5, "30����ʱ�ػ�ʱ�䵽!");

    LOGS("\r\n\r\n!! Delay time out!\r\n!! Shutdown...\r\n");
    if(track_soc_position_queue() != 0)
    {
        track_soc_bakup_data((void*)3);
        tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_CLOSE_TIMER_ID, 2000, track_cust_module_extchip_shutdown);
    }
    else
    {
        track_cust_module_extchip_shutdown();
    }
}

void track_cust_module_delay_close(U8 sec)
{
    kal_uint32 dla = sec * 1000;
    LOGD(L_UART, L_V5, "��ʱ(%d)��ػ�!", sec);
    if(10 == sec) //δ���������
    {
        track_cust_netlog_cache(0);//��־�����ļ�
    }
    track_cust_module_cal_electricity();
    if(G_parameter.cal_wk_time_struct.comsultion_ele >= 4320.0)
    {
        track_cust_send10(1, G_parameter.cal_wk_time_struct.comsultion_ele);
    }
    if(dla == 0)dla = 1000;
    tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_CLOSE_TIMER_ID, dla, track_cust_delay_timeout_close);
}

void track_cust_module_online_update_delay(void)
{
    char buf[100] = {0};
    sprintf(buf, "AT^GT_CM=07#\r\n");
    track_drv_uart2_write_data(buf, strlen(buf));
    LOGD(L_UART, L_V5, "");
}

void track_cust_module_update_mode(void)
{
    U8 buf[100] = {0};
    LOGD(L_UART, L_V5, "mode1:%d,%d, mode2:%d,%d",
         G_parameter.extchip.mode1_par1, G_parameter.extchip.mode1_par2,
         G_parameter.extchip.mode2_par1, G_parameter.extchip.mode2_par2);
    if(G_parameter.extchip.mode == 1)
    {
        //sprintf(buf, "AT^GT_CM=04,1,1800,0#\r\n");
        track_cust_module_auto_sw_mode();
        G_parameter.gps_work.Fti.interval_acc_off = G_parameter.extchip.mode1_par1;
        G_parameter.gps_work.Fti.interval_acc_on = G_parameter.extchip.mode1_par1;
    }
    else if(G_parameter.extchip.mode == 2)
    {
        //sprintf(buf, "AT^GT_CM=04,2,%d,%d#\r\n", G_parameter.extchip.mode2_par2, G_parameter.extchip.mode2_par1);
    }
    track_cust_send04((void*)G_parameter.extchip.mode);
    //track_drv_uart2_write_data(buf, strlen(buf));
    LOGD(L_UART, L_V5, "[%s]", buf);
}

void auto_mode_callback(void* par)
{
    char buf[30] = {0};
    U8 parameter = (U8)par;
    kal_bool t1 = track_is_timer_run(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID);
    applib_time_struct tmp = {0};
    track_fun_get_time(&tmp, KAL_FALSE, &G_parameter.zone.zone);
    LOGD(L_UART, L_V4, "parameter %d,%d", parameter, t1);
    track_cust_module_cal_electricity();
    if(parameter == 99 || parameter == 98 || parameter == 100)
    {
        if(parameter == 99)
        {
            /* 1420070400 = 2015-01-01 00:00:00*/ /* 1422748800 = 2015-02-01 00:00:00*/
            U32 timep;
            timep = OTA_applib_dt_mytime_2_utc_sec(&tmp, 0);
            LOGD(L_UART, L_V5, "mode1_time:%d, mode1_par2:%d, timep:%d", G_parameter.extchip.mode1_time, G_parameter.extchip.mode1_par2, timep);
            if(G_parameter.extchip.mode1_time >= 1422748800 && timep >= 1422748800)
            {
                if(G_parameter.extchip.mode1_time > timep)
                {
                    timep = G_parameter.extchip.mode1_time - timep;
                    LOGD(L_UART, L_V4, "ģʽһ����%d����%d��֮���Զ��л���ģʽ%d", timep / 60, timep % 60, G_parameter.extchip.premode);
                    track_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, timep * 1000, auto_mode_callback, (void*)99);
                    return;
                }
            }
        }

        if(G_parameter.extchip.premode == 0 || G_parameter.extchip.premode == 1)
        {
            G_parameter.extchip.mode = 2;
        }
        else
        {
            G_parameter.extchip.mode = G_parameter.extchip.premode;
        }

        if(G_parameter.extchip.mode == 2)
        {
            if(G_parameter.extchip.mode2_par1 == 0)
            {
                G_parameter.extchip.mode2_par1 = tmp.nHour * 60 + tmp.nMin;
            }
        }
        Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
        LOGD(L_UART, L_V4, "ģʽһʱ�䵽���Զ��л���ģʽ%d", G_parameter.extchip.mode);
        track_cust_module_tracking_mode(0);
        track_cust_send04((void*) G_parameter.extchip.mode);
        if(!track_is_timer_run(TRACK_CUST_EXTCHIP_DELAY_CLOSE_TIMER_ID))
        {
            track_cust_module_delay_close(30);
        }
    }
}

void track_cust_module_auto_sw_mode(void)
{
    /* 1420070400 = 2015-01-01 00:00:00*/ /* 1422748800 = 2015-02-01 00:00:00*/
    if(G_parameter.extchip.mode == 1)
    {
        U32 timep;
        static applib_time_struct tmp = {0};
        OTA_applib_dt_get_rtc_time(&tmp);
        timep = OTA_applib_dt_mytime_2_utc_sec(&tmp, 0);
        LOGD(L_UART, L_V5, "mode1_time:%d, mode1_par2:%d, timep:%d", G_parameter.extchip.mode1_time, G_parameter.extchip.mode1_par2, timep);
        if(G_parameter.extchip.mode1_time >= 1422748800 && timep >= 1422748800)
        {
            if(G_parameter.extchip.mode1_time >= timep)
            {
                timep = G_parameter.extchip.mode1_time - timep;
                LOGD(L_UART, L_V4, "ģʽһ����%d����%d��֮���Զ��л���ģʽ��", timep / 60, timep % 60);
                if(0 == timep)
                {
                    track_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, 1000, auto_mode_callback, (void*)100);
                }
                else
                {
                    track_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, timep * 1000, auto_mode_callback, (void*)99);
                }
            }
            else
            {
                timep -= G_parameter.extchip.mode1_time;
                LOGD(L_UART, L_V4, "WARN: ģʽһ�Ѿ���ʱ����%d����%d�룬�����Զ��л���ģʽ��", timep / 60, timep % 60);
                track_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, 1000, auto_mode_callback, (void*)100);
            }
        }
        else if(timep < 1422748800)
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, 30000, track_cust_module_auto_sw_mode);
            return;
        }
        else
        {
            LOGD(L_UART, L_V5, "ERROR 1");
        }
    }
}

void track_got_st_keep_time(kal_uint8* timep)
{
    LOGD(L_UART, L_V5, "st_time %d %d %d %d %d", st_time[0], st_time[1], st_time[2], st_time[3], st_time[4]);
    if(st_time[2] > 15) //13+05+16+08+12   ʱ��������
    {
        timep[0] = st_time[styear];
        timep[1] = st_time[stmonth];
        timep[2] = st_time[stday];
        timep[3] = st_time[sthour];
        timep[4] = st_time[stmin];
        timep[5] = 0;
    }
}
void track_cust_module_sc_fireware_file(U8 *data, U32 len)
{
    LOGD(L_UART, L_V5, "SC����������� %d ", len);
}

/**************************************************/
/* track_cust_module_gps_upload_status
/*
/* par  1 �ѷ���  2 �ײ㷴�����ͳ�ȥ  3 ���ͳɹ�(ƽ̨�ѻظ�)
/***************************************************/
kal_uint8 track_cust_module_gps_upload_status(U8 par)
{
    static kal_uint8 send_status = 0;
    if(par != 99)
    {
        send_status = par;
    }

    LOGD(L_UART, L_V6, "%d, send %d", par, send_status);

    return send_status;
}

kal_uint8 track_cust_backup_gpsdata_noreply()
{
    track_gps_data_struct *gpsLastPoint = NULL;
    BACKUP_GPS_DATA databuf;
    gpsLastPoint = track_cust_backup_gps_data(0, NULL);
}

void track_cust_notify_st_remove_alarm_modify(U8 op)
{
    track_cust_send05((void *)op);
}

void track_cust_nw_reset()
{
    if(G_parameter.extchip.mode != 1 && !track_is_testmode())
    {
        int v_size = 0;
        v_size = track_soc_position_queue();
        if(v_size)
        {
            track_soc_bakup_data((void*)3);
        }
        track_cust_send03((void*)240);
        LOGD(L_UART, L_V3, "δ�ѵ����� RESET NOW! %d", G_realtime_data.nw_reset_sign);
        if(G_realtime_data.nw_reset_sign >= 3)
        {
            char ams_buf[30] = {0};
            snprintf(ams_buf, sizeof(ams_buf), "(Q)(%d)%d", RUNING_TIME_SEC, G_realtime_data.nw_reset_sign);
            track_cust_cache_netlink(ams_buf, strlen(ams_buf));
            G_realtime_data.nw_reset_sign = 0;
            G_realtime_data.nw_sign = 0;
            G_realtime_data.MainCellId = 0;
            G_realtime_data.ams_push = 1;
            Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
            track_os_intoTaskMsgQueueExt(track_cust_send_lbs, (void*)3);
            return;
        }
        else if(G_realtime_data.nw_reset_sign == 0)
        {
            LBS_Multi_Cell_Data_Struct *lbs_strcut = track_drv_get_lbs_data();
            G_realtime_data.MainCellId = lbs_strcut->MainCell_Info.cell_id;
        }
        track_cust_netlog_cache(0);//��־�����ļ�
        G_realtime_data.nw_sign++;
        G_realtime_data.nw_reset_sign++;
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
        track_cust_restart(128, 5);
    }
}


/****************************************
* Function    track_cust_poweron_send_lbs

*

* Parameter


* Purpose     ����ȡ�õ�Ƭ��ʱ��LBS

*

* Modification
******************************************/
kal_uint8 track_cust_poweron_send_lbs(U8 op)
{
    static U8 sendlbs_en = 0;
    LOGD(L_UART, L_V3, "op %d");
}


void track_cust_module_power_data(kal_uint8 *data, int len)
{
    static kal_uint8  buf[2000] = {0};
    static kal_uint32 flag = 0;
    static kal_uint32 tick = 0;
    static int last_len = 0;
    kal_uint8 *start = NULL, *end = NULL;
    int j = 0, x;

    if(len <= 0 || (len == 1 && (*data == 0 || *data == 0xFF))) return;

    if(tick > 0 && OTA_kal_get_systicks() - tick > OTA_kal_secs_to_ticks(20))
    {
        flag = 0;
    }
    tick = OTA_kal_get_systicks();

    if(flag + len < 1999)
    {
        for(j = 0; j < len; j++)
        {
            if(data[j] > 0 && data[j] < 128)
            {
                buf[flag++] = data[j];
            }
        }
        buf[flag] = 0;
    }
    else
    {
        if(len > 1999)
        {
            LOGD(L_UART, L_V1, "ERROR : Data too long!");
            return;
        }
        for(j = 0, flag = 0; j < len; j++)
        {
            if(data[j] > 0 && data[j] <= 126)
            {
                buf[flag++] = data[j];
            }
        }
        buf[flag] = 0;
    }
    LOGD(L_UART, L_V7, "[recv](%d,%d,%d): %s", strlen(buf), flag, len, data);
    LOGH(L_UART, 7, buf, flag);
    if(flag <= 0) return;
    buf[flag] = 0;

    for(j = 0; j < flag - 5; j++)
    {
        if(buf[j] == 'A' && buf[j+1] == 'T' && buf[j+2] == '^' && buf[j+3] == 'G' && buf[j+4] == 'T'
                && buf[j+5] == '_' && buf[j+6] == 'C' && buf[j+7] == 'M' && buf[j+8] == '=')
        {
            start = &buf[j];
            //end = strstr(start, "\r\n");
            end = strstr(start, "#");
            if(end != NULL)
            {
                int size, i, ret;
                j = end - buf;
                if(j > flag)
                {
                    LOGD(L_UART, L_V1, "ERROR");
                    memset(buf, 0, 2000);
                    flag = 0;
                    return;
                }
                if(G_realtime_data.netLogControl & 256)
                {
                    cust_packet_log_data_ext2(8, "[Recv]", (kal_uint8 *)buf, strlen((char *)buf));
                }
                cmd_parameters_count = track_fun_str_analyse(start + 9, cmd_parameters, PARAMETER_MAX, NULL, "#", ',');

                size = sizeof(CMD_Data) / sizeof(cust_recv_struct);
                LOGD(L_UART, L_V1, "[%d %d] %s,%s", cmd_parameters_count, size, start, end);

                LOGH(L_UART, 6, cmd_parameters, 15);

                for(i = 0; i < size; i++)
                {
                    ret = strcmp(cmd_parameters[0], CMD_Data[i].cmd_string);
                    LOGD(L_UART, L_V9, "ret=%d|%s,%s", ret, cmd_parameters[0], CMD_Data[i].cmd_string);
                    if(!ret)
                    {
                        LOGD(L_UART, L_V9, "[%d], %s", i, CMD_Data[i].cmd_string);
                        if(CMD_Data[i].func_ptr != NULL)
                        {
                            CMD_Data[i].func_ptr();
                            LOGD(L_UART, L_V8, "run [%d], %s", i, CMD_Data[i].cmd_string);
                            g_tick_new = OTA_kal_get_systicks();
                        }
                        break;
                    }
                }
                //LOGD(L_UART, L_V1, "flag:%d,%d", flag, end - buf);
            }
        }
    }
    //LOGD(L_UART, L_V2, "2 flag:%d, buf:%p, start:%p, end:%p", flag, buf, start, end);
    if(start == NULL)
    {
        start = strstr(buf, "08 OK\r\n");
        if(start)
        {
            end = start + 7;
            cmd_08();
        }
        else
        {
            start = strstr(buf, "02 OK\r\n");
            if(start)
            {
                end = start + 7;
                cmd_02();
            }
        }
        if(start == NULL)
        {
            return;
        }
    }
    if(end == 0 && start > buf)
    {
        end = start;
    }
    if(end > buf)
    {
        kal_uint8 *bb;
        if(*(end + 1) == 0x0a) end += 2;
        if(flag == (end - buf))
        {
            flag = 0;
        }
        else if(flag > (end - buf))
        {
            flag -= (end - buf);
            if(flag > 2000)
            {
                LOGD(L_UART, L_V1, "error");
                return;
            }
            bb = (U8*)Ram_Alloc(6, flag);
            if(bb == NULL)
            {
                LOGD(L_UART, L_V1, "error 2");
                return;
            }
            memcpy(bb, end, flag);
            memset(buf, 0, 2000);
            memcpy(buf, bb, flag);
            Ram_Free(bb);
        }
    }
    LOGD(L_UART, L_V7, "3 flag:%d, buf:%p, start:%p, end:%p", flag, buf, start, end);
}

#elif defined(__GT300S__)


/*****************************************************************************
*  Local variable                       �ֲ�����
*****************************************************************************/
static char *cmd_parameters[PARAMETER_MAX] = {0};
static int cmd_parameters_count = 0;
static U32 g_tick_new = 0;
static cmd_return_struct g_return_sms = {0};
static kal_bool g_02_ok = KAL_FALSE;
static kal_uint8 g_mode = 0,  g_return_sms_valid = 0;
static kal_bool ensleep = FALSE;
static kal_bool b_track_mode = KAL_FALSE;//׷��ģʽ
static kal_uint8 st_file_name[20] = {"ST.txt"};

/****************************************************************************
*  Global Variable                      ȫ�ֱ���
*****************************************************************************/


/****************************************************************************
*  Global Variable - Extern             ����ȫ�ֱ���
*****************************************************************************/


/*****************************************************************************
*  Global Functions - Extern            �����ⲿ����
******************************************************************************/
extern void track_cust_teardown_alarm();
extern void track_spi_write(kal_uint8 *data, kal_uint8 len);
extern void track_drv_sys_power_off_req(void);
extern void teardown_alarm_overtime(void);

/*****************************************************************************
*  Local Functions Define               ���غ�������
******************************************************************************/


/*****************************************************************************
*  Local Functions                      ���غ���
******************************************************************************/
void track_cust_save_logfile(kal_uint8 *data, kal_uint16 len)
{
    kal_int8 i = 1, j = 1;
    U32 filelen = 1024 * 2, readsize = 0;
    FS_HANDLE ret = NULL;
    kal_uint16 ret2 = 0;
    WCHAR tmpfile[40] = {0}, tmpfile2[40] = {0};
    kal_uint8 tmpname[20] = {0};
    int close_ret = 0, close_ret2 = 0, delefault = 99, fs_result = 0;
    int wSize = 0;
    kal_uint8 *g_rec_data2 = NULL;
    kal_wsprintf(tmpfile, st_file_name);
    ret = FS_Open(tmpfile, FS_READ_WRITE | FS_CREATE);
    if(g_rec_data2 == NULL)
    {
        g_rec_data2 = (U8*)Ram_Alloc(6, 2 * 1024);
        if(g_rec_data2 == NULL)
        {
            LOGD(L_UART, L_V1, "ERROR: Ram_Alloc");
            track_cust_restart(64, 0);
            return;
        }
    }
    if(ret < 0)
    {
        FS_Close(ret);
        ret = 0;
        LOGD(L_APP, L_V5, "Can not create this file! ret:%d", ret);
        return;
    }
    FS_Read(ret, g_rec_data2, filelen, &readsize);
    LOGD(L_UART, L_V3, "==file %d %d %d", filelen, readsize, strlen(g_rec_data2));
    if(readsize >= 5 * 1024)
    {
        LOGD(L_UART, L_V5, "���ݴ���5K,ɾ��!");
        FS_Close(ret);
        ret2 = FS_Delete(tmpfile);
        LOGD(L_APP, L_V5, "FS_Delete=%d", ret2);
        return;
    }
    fs_result = FS_Seek(ret, 0, FS_FILE_END);
    if(fs_result < FS_NO_ERROR)
    {
        FS_Close(ret);
        LOGD(L_APP, L_V5, "FS_Seek %d", fs_result);
        return;
    }
    ret2 = FS_Write(ret, (void *)data, len , &wSize);
    if(ret2 < FS_NO_ERROR)
    {
        FS_Close(ret2);
        LOGD(L_APP, L_V5, "ret2:%d", ret2);
        ret2 = FS_Delete(tmpfile);
        LOGD(L_APP, L_V5, "FS_Delete=%d", ret2);
    }
    FS_Commit(ret2);
    close_ret = FS_Close(ret2);
    close_ret2 = FS_Close(ret);

    LOGD(L_UART, L_V6, "==[DVT] %d %d %d %d %d==delefault=%d", ret, close_ret, ret2, wSize, delefault, close_ret2);

}

void track_cust_pre_remove_alarm()
{
    kal_uint8 send_packet16_counts = 1;
    static kal_uint8 i = 1;
    track_gps_data_struct *gpsLastPoint = NULL;
    LOGD(L_UART, L_V2, "index:%d,timer is:%d", i, track_is_timer_run(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID));
    if(!track_is_timer_run(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID))
    {
        gpsLastPoint = track_cust_gpsdata_alarm();
        track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_PRE_REMOVE, KAL_TRUE, 0);
        i++;
        if(i <= send_packet16_counts) //��������,��ʱ����������
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID, 60 * 1000, track_cust_pre_remove_alarm);
        }
        else
        {
            track_stop_timer(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID);
            i = 1;
        }

    }
    else
    {
        //��ֹ��ͻ֮ǰ�ı���
        LOGD(L_UART, L_V2, "��ͻ,�ӳ�2��!");
        if(!track_is_timer_run(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID))
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_SEND_REMOVE_ALARM_TIMER_ID, 2000, track_cust_pre_remove_alarm);
        }
    }

}

U8 track_cust_module_tracking_mode(U8 op)
{
    if(op == 99)
    {
        return G_realtime_data.tracking_mode;
    }
    if(op != G_realtime_data.tracking_mode)
    {
        G_realtime_data.tracking_mode = op;
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
    }
    LOGD(L_UART, L_V5, "%d", op);
    return 0;
}
//�տ����ӳ�30�뷢����
static void track_cust_delay_send_msg()
{
    track_os_intoTaskMsgQueue(track_cust_teardown_alarm);
}


static void confirm(char *str)
{
    if((track_is_testmode() == KAL_TRUE))
    {
        LOGS("%s", str);
        return;
    }
    if(g_return_sms_valid)
    {
        track_cust_cmd_reply(&g_return_sms, str, strlen(str));
        g_return_sms_valid = 0;
    }
    else LOGD(L_UART, L_V5, "%s", str);
}

kal_bool track_cust_get_mode_status(kal_bool status)
{
    static kal_bool tmp = 0;
    LOGD(L_UART, L_V5, "%d,%d", status, tmp);
    if(status == 0XFF)
    {
        return tmp;
    }
    tmp = status;
}

static void track_cust_module_gt300s_sw_mode(void)
{
    char buf[100] = {0};
    LOGD(L_UART, L_V5, "mode:%d,m1:%d,dt:%d", G_parameter.work_mode.mode, G_parameter.work_mode.mode1_timer, G_parameter.work_mode.datetime);
    if(G_parameter.work_mode.mode == 3)
    {
        sprintf(buf, "AT^GT_CM=04,2,%d,%d#", G_parameter.work_mode.mode3_timer, G_parameter.work_mode.datetime);
        track_spi_write(buf, strlen(buf));
    }
    else if(G_parameter.work_mode.mode == 2 || G_parameter.work_mode.mode == 1)
    {
        sprintf(buf, "AT^GT_CM=04,1,%d,%d#", G_parameter.work_mode.mode1_timer, G_parameter.work_mode.datetime);
        track_spi_write(buf, strlen(buf));
    }
    tr_start_timer(TRACK_CUST_GT300S_MODE_AUTO_SW_TIMER_ID, 6000, track_cust_module_gt300s_sw_mode);
}

void track_cust_module_extchip_shutdown(void)
{
    char buf[100] = {0};
    LOGD(L_APP, L_V5, "mode:%d", G_parameter.work_mode.mode);
    if(G_parameter.work_mode.mode != 3)
    {
        return;
    }
    track_soc_bakup_data((void*)3);
    if(track_cust_get_mode_status(0xff))
    {
        sprintf(buf, "AT^GT_CM=03#");
        track_spi_write(buf, strlen(buf));
        LOGD(L_UART, L_V5, "׼���ػ�");
    }
    else
    {
        LOGD(L_APP, L_V1, "ģʽδ�ĳɹ���ģ�鲻�����ػ�");
    }

}

void track_mode2_to_mode1(void)
{
    LOGD(L_UART, L_V5, "mode:%d,t_status_mode:%d", G_parameter.work_mode.mode, G_realtime_data.tracking_status_mode);
    if(G_realtime_data.tracking_status_mode < 4 && G_realtime_data.tracking_status_mode > 0)
    {
        G_parameter.work_mode.mode = G_realtime_data.tracking_status_mode;

        G_parameter.gps_work.Fti.interval_acc_on = G_parameter.extchip.mode1_par1;
        G_parameter.gps_work.Fti.interval_acc_off = G_parameter.extchip.mode1_par2;
    }
    G_realtime_data.tracking_mode = 0;
    track_cust_module_gt300s_sw_mode();
    if(G_parameter.work_mode.mode == 3)
    {
        tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, 10000, track_cust_module_extchip_shutdown);
    }
    else if(G_parameter.work_mode.mode == 1)
    {
        track_stop_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID);
        track_cust_wake_mode();
    }
    Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
    Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
}

#define START_UP_UTC_TIME_MS  1422748800    /* 1420070400 = 2015-01-01 00:00:00*/ /* 1422748800 = 2015-02-01 00:00:00*/
void track_cust_module_gt300s_auto_sw_mode(void)
{
#if defined(__GT300S__)
    if(G_parameter.work_mode.mode == 2 && G_realtime_data.tracking_mode == 1)
    {
        kal_uint32 timep, deadtime;
        static applib_time_struct tmp = {0};
        static kal_uint8 cnt = 0;
        OTA_applib_dt_get_rtc_time(&tmp);
        timep = OTA_applib_dt_mytime_2_utc_sec(&tmp, 0);
        LOGD(L_UART, L_V5, "tracking_time:%d, mode:%d.t_status_mode=%d,ms:%d", G_realtime_data.tracking_time, \
             G_parameter.work_mode.mode, G_realtime_data.tracking_status_mode, timep);
        if(G_realtime_data.tracking_time >= START_UP_UTC_TIME_MS && timep >= START_UP_UTC_TIME_MS)
        {
            deadtime = G_realtime_data.tracking_time + G_parameter.work_mode.teardown_time * 60;
            if(deadtime >= timep)
            {
                timep = deadtime - timep;
                LOGD(L_UART, L_V4, "ģʽһ����%d����%d��֮���Զ��л���ģʽ��", timep / 60, timep % 60);
                tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, timep * 1000, track_mode2_to_mode1);
            }
            else
            {
                LOGD(L_UART, L_V4, "WARN: ģʽһ�Ѿ���ʱ����%d����%d�룬�����Զ��л���ģʽ��", timep / 60, timep % 60);
                tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, 1000, track_mode2_to_mode1);
            }
        }
        else if(timep < START_UP_UTC_TIME_MS)
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, 30000, track_cust_module_gt300s_auto_sw_mode);
            cnt++;
            if(cnt > 30)
            {
                cnt = 0;
                track_mode2_to_mode1();
                LOGD(L_UART, L_V5, "15���Ӷ���ȡ����ʱ�䰡timep:%d,G_realtime_data.tracking_time=%d ", timep, G_realtime_data.tracking_time);
            }
            return;
        }
        else
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, G_parameter.work_mode.teardown_time * 60 * 1000, track_mode2_to_mode1);
            LOGD(L_UART, L_V5, "ʱ�����Ϊʲô��������timep:%d,G_realtime_data.tracking_time=%d ", timep, G_realtime_data.tracking_time);
        }
    }

#endif /* __GT300S__ */

}

void track_cust_module_sw_init(void)
{
    char buf[100] = {0};
    //��ʼ������
    LOGD(L_UART, L_V5, "");

    sprintf(buf, "AT^GT_CM=01#");
    track_spi_write(buf, strlen(buf));
    tr_start_timer(TRACK_CUST_GT300S_MODE_INIT_SW_TIMER_ID, 10000, track_cust_module_sw_init);
}

void track_cust_module_delay_close(kal_uint8 op)
{
    LOGD(L_UART, L_V5, "�ػ�ʱ�䵽!:%d", op);
    LOGS("\r\n\r\n!! track_cust_module_delay_close GPS positioning has been completed, \
		Location uploaded!\r\n!! Shutdown...\r\n");
    track_cust_module_extchip_shutdown();
    tr_start_timer(TRACK_POWER_RESET_TIMER_ID, 1000, track_drv_sys_power_off_req);
}

void track_cust_work_off(void)
{
    LOGD(L_UART, L_V5, "����׼���ػ�");
    if(track_cust_get_work_mode() == WORK_MODE_3)
    {
        LOGD(L_UART, L_V5, "����׼���ػ�");
        track_soc_bakup_data((void*)3);
        track_cust_module_delay_close(10);
    }
}

void track_cust_power_mode_set(void)
{
    char buf[100] = {0};
    LOGD(L_UART, L_V5, "");
    sprintf(buf, "AT^GT_CM=14,%d#", G_parameter.pwrlimit_sw);
    track_spi_write(buf, strlen(buf));
    if(!track_is_timer_run(TRACK_CUST_GT300S_MODE_INIT_SW_TIMER_ID))
    {
        tr_start_timer(TRACK_CUST_GT300S_MODE_INIT_SW_TIMER_ID, 30000, track_cust_module_sw_init);
    }
}


void cmd_gt300s_01(void)
{
    //AT^GT_CM=01,KKS_GT700_V3.0_1.12,286,0:0,2,60,575,6,270,0,1199#
    //AT^GT_CM=01,GT700_V1.01,258,13:05,01,6,300,10,275,0,1,CRC#
    int mode, i, sum = 0;
    char *p, tmp[150] = {0};
    kal_uint16 datetime;
    kal_uint16 update_time, cnt, timer, status;
    static   kal_uint8 flg = 0;
    static U8 singlebuf[100] = {0};

    tr_stop_timer(TRACK_CUST_GT300S_MODE_INIT_SW_TIMER_ID);

    //����������
    for(i = 0; i < cmd_parameters_count; i++)
    {
        if(i >= 2 && i < cmd_parameters_count - 1)
        {
            sum += atoi(cmd_parameters[i]);
            p = strchr(cmd_parameters[i], ':');
            if(p != NULL)
            {
                sum += atoi(p + 1);
            }
        }
        LOGD(L_UART, L_V5, "[%d] %s -> %d,%d\r\n", i, cmd_parameters[i], sum, atoi(cmd_parameters[i]));
    }
    if(sum != atoi(cmd_parameters[cmd_parameters_count - 1]))
    {
        LOGD(L_UART, L_V1, "ERROR: У�鲻ͨ��. %d,%d", sum, atoi(cmd_parameters[cmd_parameters_count - 1]));
        return;
    }
#if defined (__MCU_SW_UPDATE__)
    memset(mcu_sw_version_string, 0x00, sizeof(mcu_sw_version_string));
    snprintf(mcu_sw_version_string, sizeof(mcu_sw_version_string) - 1, "%s", cmd_parameters[1]);
#endif /* __MCU_SW_UPDATE__ */


    snprintf(tmp, 150, "version:(%s)\r\n��ѹ:(%s);\r\n��ǰʱ��:%s; ģʽ�����:%s,%s; ��ʼʱ��:%s; ���� ʱ��:%s; cnt:%s,���ƹػ�:%s;",
             cmd_parameters[1], cmd_parameters[2], cmd_parameters[3],
             cmd_parameters[4], cmd_parameters[5], cmd_parameters[6],
             cmd_parameters[7], cmd_parameters[8], cmd_parameters[9]);
    LOGD(L_UART, L_V1, "%s", tmp);

    confirm(tmp);
    mode = atoi(cmd_parameters[4]);
    datetime = atoi(cmd_parameters[6]);
    update_time = atoi(cmd_parameters[7]);
    cnt = atoi(cmd_parameters[8]);
    timer = atoi(cmd_parameters[5]);
    status = atoi(cmd_parameters[9]);
    if(flg == 0)
    {
        flg = 1;
        snprintf(singlebuf, 99, "ver:%s,bat:%s,timer:%s,mode:%s,type:%d\n\r", cmd_parameters[1], cmd_parameters[2], cmd_parameters[3], cmd_parameters[4], mmi_Get_PowerOn_Type());
        track_cust_save_logfile(singlebuf, strlen(singlebuf));
    }
    if(mode == 0) mode++;

    LOGD(L_APP, L_V5, "mode:%d,work mode:%d,mode3 timer:%d,work datetime:[%d,%d]", mode, G_parameter.work_mode.mode, G_parameter.work_mode.mode3_timer, \
         G_parameter.work_mode.datetime, datetime);

    if((mode == 1) && (G_parameter.work_mode.mode != 1 && G_parameter.work_mode.mode != 2))
    {
        LOGD(L_UART, L_V1, "ģʽ1 ��һ��,%d,%d,%d", G_parameter.work_mode.mode1_timer, timer, G_parameter.work_mode.mode);
        track_cust_module_gt300s_sw_mode();
    }
    else if(mode == 2)
    {
        if(G_parameter.work_mode.mode3_timer != 0 && G_parameter.work_mode.mode3_timer != timer || G_parameter.work_mode.mode != 3)
        {
            LOGD(L_UART, L_V1, "ģʽ2 ��һ��,%d,%d,%d", G_parameter.work_mode.mode3_timer, timer, G_parameter.work_mode.mode);
            track_cust_module_gt300s_sw_mode();
            track_cust_get_mode_status(0);

        }
        else
        {
            track_cust_get_mode_status(1);
            LOGD(L_UART, L_V1, "ģʽ����һ��");
        }

    }
    if(datetime != G_parameter.work_mode.datetime && mode == 2)
    {
        LOGD(L_UART, L_V1, "��ʼʱ�䲻һ%d,%d", datetime, G_parameter.work_mode.datetime);
        track_cust_module_gt300s_sw_mode();
        track_cust_get_mode_status(0);
    }
    else
    {
        LOGD(L_UART, L_V1, "��ʼʱ��һ��");

    }

    if(status != G_parameter.pwrlimit_sw)
    {
        track_cust_power_mode_set();
    }
    track_cust_module_gt300s_auto_sw_mode();//׷��ģʽ
    if(G_realtime_data.switch_down != atoi(cmd_parameters[10]))
    {
        G_realtime_data.switch_down = atoi(cmd_parameters[10]);
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
        //  track_cust_upload_param();
    }
}

static void cmd_02(void)
{
    U8 buf[100] = {0};
    LOGD(L_UART, L_V5, "");
    confirm("02 OK!");
    if(!g_02_ok) return;
    track_cust_module_gt300s_sw_mode();
}

static void cmd_03(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("03 OK!");
    track_drv_sys_power_off_req();
}
void cmd_gt300s_04(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("04 OK!");
    track_stop_timer(TRACK_CUST_GT300S_MODE_AUTO_SW_TIMER_ID);
    track_cust_get_mode_status(1);
    //ensleep = KAL_TRUE;
    /*if(G_parameter.extchip.mode == 1)
    {
        char buf[100] = {0};
        applib_time_struct time = {0};
        track_fun_get_time(&time, KAL_FALSE, &G_parameter.zone.zone);
        if(time.nYear > 2014)
        {
            sprintf(buf, "AT^GT_CM=02,%d,%d#\r\n", time.nHour, time.nMin);
            track_drv_uart2_write_data(buf, strlen(buf));
            LOGD(L_UART, L_V5, "[%s]", buf);
        }
    }*/
    /*sprintf(buf, "AT^GT_CM=01#\r\n");
    track_drv_uart2_write_data(buf, strlen(buf));*/
}

kal_uint8  track_is_updata(kal_uint8 type)
{
    static kal_uint8 status = 0;
    if(type == 0xff)
    {
        return status;
    }
    else
    {
        status = type;
        return 0xff;
    }
}
static void cmd_05(void)
{
    U8 buf[100] = {0};
    if(!strcmp(cmd_parameters[1], "02"))
    {

        confirm("(Short press)!key=5,0");
        sprintf(buf, "AT^GT_CM=05#\r\n");
        track_spi_write(buf, strlen(buf));
    }
    else if(!strcmp(cmd_parameters[1], "03"))
    {
        confirm("(Short press)!key=5,1");
        sprintf(buf, "AT^GT_CM=05#\r\n");
        track_spi_write(buf, strlen(buf));
    }
    if((track_is_testmode() == KAL_TRUE))
    {
        return;
    }
//   track_cust_05_status(1);
//track_cust_ext_teardown_alm((void*)1);
#if defined(__GT300S__)
    if(!strcmp(cmd_parameters[1], "02"))
    {
        track_cust_key_control_led();
        track_is_updata(1);
        if(track_cust_gps_status() >= GPS_STAUS_2D)
        {
            track_gps_data_struct *p_gpsdata = track_cust_backup_gps_data(0, NULL);
            if(p_gpsdata->gprmc.status != 0 && (p_gpsdata->gprmc.Latitude != 0 || p_gpsdata->gprmc.Longitude != 0))
            {
                track_cust_paket_position(p_gpsdata, track_drv_get_lbs_data(), TR_CUST_POSITION_KEY, 0);
            }
        }
        else
        {
            track_cust_gps_work_req((void *)12);
        }
    }
#endif

}
static void cmd_06(void)
{
    U8 buf[100] = {0};

    LOGD(L_UART, L_V5, "");
    confirm("(Long press)!key=6");
    sprintf(buf, "AT^GT_CM=06#\r\n");
    track_spi_write(buf, strlen(buf));
}

static void cmd_07(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("07 OK!");
    tr_stop_timer(TRACK_CUST_07_TIMEOUT_TIMER);
}

void track_send_07(void)
{
    U8 buf[100] = {0};
    static kal_uint8 cnt = 0;
    LOGD(L_UART, L_V5, "");
    sprintf(buf, "AT^GT_CM=07#");
    track_spi_write(buf, strlen(buf));
    if(cnt < 3)
    {
        cnt++;
        tr_start_timer(TRACK_CUST_07_TIMEOUT_TIMER, 1000, track_send_07);
    }

}
static void cmd_10(void)
{
    U8 buf[100] = {0};
    LOGD(L_UART, L_V5, "");
    confirm("10 OK!");

    sprintf(buf, "AT^GT_CM=10#");
    track_spi_write(buf, strlen(buf));
}

static void cmd_08(void)
{
    U8 buf[100] = {0};
    LOGD(L_UART, L_V5, "");
    confirm("08 OK!");

    sprintf(buf, "AT^GT_CM=08#");
    track_spi_write(buf, strlen(buf));
}

static void cmd_12(void)
{
    applib_time_struct tmp = {0};
    nvram_work_mode_struct chip = {0};
    kal_uint32 tick = 0;
    static kal_uint32 cur_tick = 0;
    U8 buf[100] = {0};

    LOGD(L_UART, L_V5, "(Short press)teardown_sw:%d,tm:%d,tracking:%d,mode:%d,teardown:%d", \
         G_parameter.teardown_str.sw, G_realtime_data.tracking_status_mode, G_realtime_data.tracking_mode, \
         G_parameter.work_mode.mode,	G_parameter.work_mode.teardown_time);

    confirm("(Short press)!key=13,1");
    sprintf(buf, "AT^GT_CM=12#");
    track_spi_write(buf, strlen(buf));
    if((track_is_testmode() == KAL_TRUE))
    {
        return;
    }
    if(G_parameter.teardown_str.sw == 1)
    {
        if(cur_tick == 0)
        {
            cur_tick = kal_get_systicks();
        }
        else
        {
            tick = kal_get_systicks();
            if(tick - cur_tick > KAL_TICKS_1_MIN * 2)
            {
                cur_tick = tick;
            }
            else
            {
                LOGD(L_UART, L_V5, "���𱨾��������δ����2����!");
                return;
            }
        }
        track_cust_gps_work_req((void *)3);
        //tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_SEND_ALARM_TIMER_ID, 30*1000, track_cust_delay_alarm);
        track_os_intoTaskMsgQueue(track_cust_pre_remove_alarm);
        if(G_parameter.teardown_str.alarm_type > 0 && G_parameter.teardown_str.alarm_type < 3) //1��2
        {
            //����
            tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_SEND_ALARM_MSG_TIMER_ID, 60 * 1000, teardown_alarm_overtime);
        }
        if(G_parameter.teardown_str.alarm_type > 1) //2��3
        {
            //call
            track_start_timer(TRACK_CUST_ALARM_TEAR_DOWN_CALL_TIMER, 10 * 1000, track_cust_make_call, (void*)1);
        }
#ifdef __GT300S__
        if(G_parameter.work_mode.mode == 2 && G_realtime_data.tracking_mode == 0)
        {
            LOGD(L_UART, L_V5, "ģʽ%d,�Ƿ���׷��%d ����", G_parameter.work_mode.mode, G_realtime_data.tracking_mode);
            return;
        }
#else
if(G_parameter.work_mode.mode == 1 || G_parameter.work_mode.mode == 2 && G_realtime_data.tracking_mode == 0)
{
    LOGD(L_UART, L_V5, "ģʽ%d,�Ƿ���׷��%d ����", G_parameter.work_mode.mode, G_realtime_data.tracking_mode);
    return;
}
#endif /* __GT300S__ */

        track_cust_module_tracking_mode(1);

        memcpy(&chip, &G_parameter.work_mode, sizeof(nvram_work_mode_struct));
        chip.mode = G_parameter.work_mode.teardown_mode;

        OTA_applib_dt_get_rtc_time(&tmp);
        G_realtime_data.tracking_time = OTA_applib_dt_mytime_2_utc_sec(&tmp, 0);
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);

        LOGD(L_UART, L_V5, "%d time: %0.2d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d    sec:%d", G_parameter.work_mode.teardown_time,
             tmp.nYear, tmp.nMonth, tmp.nDay, tmp.nHour, tmp.nMin, tmp.nSec, G_realtime_data.tracking_time);

        memcpy(&chip, &G_parameter.work_mode, sizeof(nvram_work_mode_struct));
        chip.mode = G_parameter.work_mode.teardown_mode;

        memcpy(&G_parameter.work_mode, &chip, sizeof(nvram_work_mode_struct));
        //Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
        if(Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE))
        {
            //track_cust_upload_param();//����״̬�ı��ϴ�ֻ��01��ѯʱ��һ���ϴ�
            //track_cust_module_gt300s_auto_sw_mode();
            tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, \
                           G_parameter.work_mode.teardown_time * 60 * 1000 + 5000, track_mode2_to_mode1);
        }
        else
        {
            LOGD(L_UART, L_V5, "error write nvram 05!");
        }

        track_cust_module_gt300s_sw_mode();
        track_cust_wake_mode();
    }
    /*
    if(track_cust_gps_status() >= GPS_STAUS_2D)
    {
        track_gps_data_struct *p_gpsdata = track_cust_backup_gps_data(0, NULL);
        if(p_gpsdata->gprmc.status != 0 && (p_gpsdata->gprmc.Latitude != 0 || p_gpsdata->gprmc.Longitude != 0))
        {
            track_cust_paket_position(p_gpsdata, track_drv_get_lbs_data(), TR_CUST_POSITION_KEY, 0);
        }
    }
    else
    {
        track_cust_sendlbs_limit();
    }
    */
}

static void cmd_13(void)
{
    applib_time_struct tmp = {0};
    nvram_work_mode_struct chip = {0};
    kal_uint32 tick = 0;
    static kal_uint32 cur_tick = 0;
    U8 buf[100] = {0};
    if(!strcmp(cmd_parameters[1], "0"))
    {
        LOGD(L_UART, L_V5, "(Short press)");
        confirm("(Short press)!key=13,0");
        sprintf(buf, "AT^GT_CM=13#");
        track_spi_write(buf, strlen(buf));
    }
    else if(!strcmp(cmd_parameters[1], "1"))
    {
        LOGD(L_UART, L_V5, "(Short press)");
        confirm("(Short press)!key=13,1");
        sprintf(buf, "AT^GT_CM=13#");
        track_spi_write(buf, strlen(buf));
    }
}


static void cmd_15(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("15 OK!");
}

static void cmd_16(void)
{
    LOGD(L_UART, L_V5, "%s", cmd_parameters[1]);
}
static void cmd_99(void)
{
#if defined (__MCU_SW_UPDATE__)
    track_drv_mcu_sw_update_rsp();
#endif /* __MCU_SW_UPDATE__ */
    LOGD(L_UART, L_V5, "");
    confirm("99 OK!");
}

static cust_recv_struct CMD_Data[] =
{
    {"01",        cmd_gt300s_01},
    {"02",        cmd_02},
    {"03",        cmd_03},
    {"04",        cmd_gt300s_04},
    {"05",        cmd_05},
    {"06",        cmd_06},
    {"07",        cmd_07},
    {"08",        cmd_08},
    {"10",        cmd_10},
    {"12",    	  cmd_12},
    {"13",    	  cmd_13},
    {"15",    	  cmd_15},
    {"16",		  cmd_16},
    {"99",		  cmd_99},

};

void track_cust_module_power_cmd(cmd_return_struct *return_sms, kal_uint8 *data, int len)
{
    memcpy(&g_return_sms, return_sms, sizeof(cmd_return_struct));
    g_return_sms_valid = 1;
    track_spi_write(data, len);
}

void track_cust_module_extchip_time_synchronization(kal_uint32 arg)
{
    LOGD(L_UART, L_V5, "%d", arg);
    if(!g_02_ok)
    {
        applib_time_struct time = {0};
        g_02_ok = KAL_TRUE;
        track_fun_get_time(&time, KAL_FALSE, &G_parameter.zone.zone);
        LOGD(L_UART, L_V5, "time: %0.2d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d",
             time.nYear,
             time.nMonth,
             time.nDay,
             time.nHour,
             time.nMin,
             time.nSec);
        if(time.nYear >= 2016)
        {
            U8 buf[100] = {0}, valid = 0;
            //U32 timep;
            sprintf(buf, "AT^GT_CM=02,%2d,%2d#", time.nHour, time.nMin);
            track_spi_write(buf, strlen(buf));
            if(G_realtime_data.tracking_time < 1422748800 && G_realtime_data.tracking_time + arg >= 1422748800)
            {
                G_realtime_data.tracking_time += arg;
                Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
            }
            track_cust_module_gt300s_auto_sw_mode();
        }
    }
}

void track_cust_module_Handshake(void)
{
    char buf[100] = {0};
    sprintf(buf, "AT^GT_CM=08#");
    track_spi_write(buf, strlen(buf));
    LOGD(L_UART, L_V5, "");
    tr_start_timer(TRACK_CUST_EXTCHIP_BATTERY_VOLTAGE_TIMER_ID, 5000, track_cust_module_Handshake);
}



void track_cust_spi_read_data(kal_uint8 *data, kal_uint16 len)
{
    char* p;
    kal_uint8 count;
    int size, i, ret;
    LOGD(L_DRV, L_V4, "��Ƭ����Ϣ:%s", data);
    if(G_realtime_data.netLogControl & 1)
    {
        cust_packet_log_data(1, data, len);
    }

    p = strstr(data, "GT_CM=");
    if(p != NULL)
    {
        count = track_fun_str_analyse(data + 9, cmd_parameters, PARAMETER_MAX, NULL, "#", ',');
        size = sizeof(CMD_Data) / sizeof(cust_recv_struct);
        LOGD(L_UART, L_V1, "[%d %d] ", count, size);

        LOGH(L_UART, 6, cmd_parameters, 15);
        for(i = 0; i < size; i++)
        {
            ret = strcmp(cmd_parameters[0], CMD_Data[i].cmd_string);
            if(!ret)
            {
                LOGD(L_UART, L_V5, "[%d], %s", i, CMD_Data[i].cmd_string);
                if(CMD_Data[i].func_ptr != NULL)
                {
                    CMD_Data[i].func_ptr();
                    LOGD(L_UART, L_V5, "run [%d], %s", i, CMD_Data[i].cmd_string);
                }
                break;
            }
        }
    }
}

#elif defined (__GT370__)|| defined (__GT380__)
/*ΪGT370���µ�Ƭ������Э���޸�       --    chengjun  2017-03-22*/

/*****************************************************************************
*  Local variable                       �ֲ�����
*****************************************************************************/
static char *cmd_parameters[PARAMETER_MAX] = {0};
static int cmd_parameters_count = 0;
static U32 g_tick_new = 0;
static cmd_return_struct g_return_sms = {0};
static kal_bool g_02_ok = KAL_FALSE;//�Ƿ����Ƭ��Уʱ��
static kal_uint8 g_mode = 0,  g_return_sms_valid = 0;
static kal_bool ensleep = FALSE;
static kal_bool b_track_mode = KAL_FALSE;//׷��ģʽ
static kal_uint8 st_file_name[20] = {"ST.txt"};

/****************************************************************************
*  Global Variable                      ȫ�ֱ���
*****************************************************************************/


/****************************************************************************
*  Global Variable - Extern             ����ȫ�ֱ���
*****************************************************************************/


/*****************************************************************************
*  Global Functions - Extern            �����ⲿ����
******************************************************************************/
extern void track_cust_teardown_alarm();
extern void track_spi_write(kal_uint8 *data, kal_uint8 len);
extern void track_drv_sys_power_off_req(void);
extern void teardown_alarm_overtime(void);
extern void track_cust_open_shell_alarm();
extern void track_open_shell_alarm_overtime(void);
/*****************************************************************************
*  Local Functions Define               ���غ�������
******************************************************************************/


/*****************************************************************************
*  Local Functions                      ���غ���
******************************************************************************/
void track_cust_save_logfile(kal_uint8 *data, kal_uint16 len)
{
    kal_int8 i = 1, j = 1;
    U32 filelen = 1024 * 2, readsize = 0;
    FS_HANDLE ret = NULL;
    kal_uint16 ret2 = 0;
    WCHAR tmpfile[40] = {0}, tmpfile2[40] = {0};
    kal_uint8 tmpname[20] = {0};
    int close_ret = 0, close_ret2 = 0, delefault = 99, fs_result = 0;
    int wSize = 0;
    kal_uint8 *g_rec_data2 = NULL;
    kal_wsprintf(tmpfile, st_file_name);
    ret = FS_Open(tmpfile, FS_READ_WRITE | FS_CREATE);
    if(g_rec_data2 == NULL)
    {
        g_rec_data2 = (U8*)Ram_Alloc(6, 2 * 1024);
        if(g_rec_data2 == NULL)
        {
            LOGD(L_UART, L_V1, "ERROR: Ram_Alloc");
            track_cust_restart(64, 0);
            return;
        }
    }
    if(ret < 0)
    {
        FS_Close(ret);
        ret = 0;
        LOGD(L_APP, L_V5, "Can not create this file! ret:%d", ret);
        return;
    }
    FS_Read(ret, g_rec_data2, filelen, &readsize);
    LOGD(L_UART, L_V3, "==file %d %d %d", filelen, readsize, strlen(g_rec_data2));
    if(readsize >= 5 * 1024)
    {
        LOGD(L_UART, L_V5, "���ݴ���5K,ɾ��!");
        FS_Close(ret);
        ret2 = FS_Delete(tmpfile);
        LOGD(L_APP, L_V5, "FS_Delete=%d", ret2);
        return;
    }
    fs_result = FS_Seek(ret, 0, FS_FILE_END);
    if(fs_result < FS_NO_ERROR)
    {
        FS_Close(ret);
        LOGD(L_APP, L_V5, "FS_Seek %d", fs_result);
        return;
    }
    ret2 = FS_Write(ret, (void *)data, len , &wSize);
    if(ret2 < FS_NO_ERROR)
    {
        FS_Close(ret2);
        LOGD(L_APP, L_V5, "ret2:%d", ret2);
        ret2 = FS_Delete(tmpfile);
        LOGD(L_APP, L_V5, "FS_Delete=%d", ret2);
    }
    FS_Commit(ret2);
    close_ret = FS_Close(ret2);
    close_ret2 = FS_Close(ret);

    LOGD(L_UART, L_V6, "==[DVT] %d %d %d %d %d==delefault=%d", ret, close_ret, ret2, wSize, delefault, close_ret2);

}

void track_cust_pre_remove_alarm()
{
    kal_uint8 send_packet16_counts = 3;
    static kal_uint8 i = 1;
    track_gps_data_struct *gpsLastPoint = NULL;
    LOGD(L_UART, L_V2, "index:%d,timer is:%d", i, track_is_timer_run(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID));
    if(!track_is_timer_run(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID))
    {
        gpsLastPoint = track_cust_gpsdata_alarm();

        if(gpsLastPoint->gprmc.status==2)//����3�α��������յ�3�ζ��ţ����˺�������
        {
            track_cust_paket_19(track_drv_get_lbs_data(), TR_CUST_ALARM_PRE_REMOVE, KAL_TRUE, 0);
        }
        else
        {
            track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_PRE_REMOVE, KAL_TRUE, 0);
        }

        i++;
        if(i <= send_packet16_counts) //��������,��ʱ����������
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID, 60 * 1000, track_cust_pre_remove_alarm);
        }
        else
        {
            track_stop_timer(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID);
            i = 1;
        }

    }
    else
    {
        //��ֹ��ͻ֮ǰ�ı���
        LOGD(L_UART, L_V2, "��ͻ,�ӳ�2��!");
        if(!track_is_timer_run(TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID))
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_SEND_REMOVE_ALARM_TIMER_ID, 2000, track_cust_pre_remove_alarm);
        }
    }
}


void track_cust_pre_open_shell_alarm()
{
    kal_uint8 send_packet16_counts = 3;
    static kal_uint8 i = 1;
    track_gps_data_struct *gpsLastPoint = NULL;
    LOGD(L_UART, L_V2, "index:%d,timer is:%d", i, track_is_timer_run(TRACK_CUST_EXTCHIP_SEND_OPEN_SHELL_ALARM_TIMER_ID));
    if(!track_is_timer_run(TRACK_CUST_EXTCHIP_SEND_OPEN_SHELL_ALARM_TIMER_ID))
    {
        gpsLastPoint = track_cust_gpsdata_alarm();
        if(gpsLastPoint->gprmc.status==2)
        {
            track_cust_paket_19(track_drv_get_lbs_data(), TR_CUST_ALARM_OPEN_SHELL, KAL_TRUE, 0);
        }
        else
        {
            track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_OPEN_SHELL, KAL_TRUE, 0);
        }
        
        i++;
        if(i <= send_packet16_counts) //��������,��ʱ����������
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_SEND_OPEN_SHELL_ALARM_TIMER_ID, 60 * 1000, track_cust_pre_open_shell_alarm);
        }
        else
        {
            track_stop_timer(TRACK_CUST_EXTCHIP_SEND_OPEN_SHELL_ALARM_TIMER_ID);
            i = 1;
        }
    }
    else
    {
        //��ֹ��ͻ֮ǰ�ı���
        LOGD(L_UART, L_V2, "��ͻ,�ӳ�2��!");
        if(!track_is_timer_run(TRACK_CUST_EXTCHIP_SEND_OPEN_SHELL_ALARM_TIMER_ID))
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_SEND_OPEN_SHELL_ALARM_TIMER_ID, 2000, track_cust_pre_open_shell_alarm);
        }
    }
}


U8 track_cust_module_tracking_mode(U8 op)
{
    if(op == 99)
    {
        return G_realtime_data.tracking_mode;
    }
    if(op != G_realtime_data.tracking_mode)
    {
        G_realtime_data.tracking_mode = op;
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
    }
    LOGD(L_UART, L_V5, "%d", op);
    return 0;
}

//�տ����ӳ�30�뷢����
static void track_cust_delay_send_msg()
{
    track_os_intoTaskMsgQueue(track_cust_teardown_alarm);
}


static void confirm(char *str)
{
    if((track_is_testmode() == KAL_TRUE))
    {
        LOGS("%s", str);
        return;
    }
    if(g_return_sms_valid)
    {
        track_cust_cmd_reply(&g_return_sms, str, strlen(str));
        g_return_sms_valid = 0;
    }
    else
    {
        LOGD(L_UART, L_V5, "%s", str);
    }
}

kal_bool track_cust_get_mode_status(kal_bool status)
{
    static kal_bool tmp = 0;
    LOGD(L_UART, L_V5, "%d,%d", status, tmp);
    if(status == 0XFF)
    {
        return tmp;
    }
    tmp = status;
}

static void track_cust_module_sw_mode_to_mcu(void)
{
    char buf[100] = {0};
    LOGD(L_UART, L_V5, "mode:%d,m1:%d,dt:%d", G_parameter.work_mode.mode, G_parameter.work_mode.mode1_timer, G_parameter.work_mode.datetime);
    if(G_parameter.work_mode.mode == 3)
    {
        sprintf(buf, "AT^GT_CM=04,2,%d,%d#", G_parameter.work_mode.mode3_timer, G_parameter.work_mode.datetime);
        track_spi_write(buf, strlen(buf));
    }
    else if(G_parameter.work_mode.mode == 2 || G_parameter.work_mode.mode == 1)
    {
        sprintf(buf, "AT^GT_CM=04,1,%d,%d#", G_parameter.work_mode.mode1_timer, G_parameter.work_mode.datetime);
        track_spi_write(buf, strlen(buf));
    }
    tr_start_timer(TRACK_CUST_GT300S_MODE_AUTO_SW_TIMER_ID, 6000, track_cust_module_sw_mode_to_mcu);
}

void track_cust_module_extchip_shutdown(void)
{
    char buf[100] = {0};
    LOGD(L_APP, L_V5, "mode:%d", G_parameter.work_mode.mode);
    if(G_parameter.work_mode.mode != 3)
    {
        return;
    }

    if(track_cust_message_status())
    {
        LOGD(L_APP, L_V4, "�ж��Ŵ������ػ�");
    }
    
    track_soc_bakup_data((void*)3);
    if(track_cust_get_mode_status(0xff))
    {
        sprintf(buf, "AT^GT_CM=03,3#");
        track_spi_write(buf, strlen(buf));
        LOGD(L_UART, L_V5, "׼���ػ�");
    }
    else
    {
        LOGD(L_APP, L_V1, "ģʽδ�ĳɹ���ģ�鲻�����ػ�");
    }

}

void track_mode2_to_mode1(void)
{
    LOGD(L_UART, L_V5, "mode:%d,t_status_mode:%d", G_parameter.work_mode.mode, G_realtime_data.tracking_status_mode);
    if(G_realtime_data.tracking_status_mode < 4 && G_realtime_data.tracking_status_mode > 0)
    {
        G_parameter.work_mode.mode = G_realtime_data.tracking_status_mode;

        G_parameter.gps_work.Fti.interval_acc_on = G_parameter.extchip.mode1_par1;
        G_parameter.gps_work.Fti.interval_acc_off = G_parameter.extchip.mode1_par2;
    }
    G_realtime_data.tracking_mode = 0;
    track_cust_module_sw_mode_to_mcu();
    if(G_parameter.work_mode.mode == 3)
    {
        tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, 10000, track_cust_module_extchip_shutdown);
    }
    else if(G_parameter.work_mode.mode == 1)
    {
        track_stop_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID);
        track_cust_wake_mode();
    }
    Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);
    Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
}

#define START_UP_UTC_TIME_MS  1422748800    /* 1420070400 = 2015-01-01 00:00:00*/ /* 1422748800 = 2015-02-01 00:00:00*/
void track_cust_module_gt300s_auto_sw_mode(void)
{
    if(G_parameter.work_mode.mode == 2 && G_realtime_data.tracking_mode == 1)
    {
        kal_uint32 timep, deadtime;
        static applib_time_struct tmp = {0};
        static kal_uint8 cnt = 0;
        OTA_applib_dt_get_rtc_time(&tmp);
        timep = OTA_applib_dt_mytime_2_utc_sec(&tmp, 0);
        LOGD(L_UART, L_V5, "tracking_time:%d, mode:%d.t_status_mode=%d,ms:%d", G_realtime_data.tracking_time, \
             G_parameter.work_mode.mode, G_realtime_data.tracking_status_mode, timep);
        if(G_realtime_data.tracking_time >= START_UP_UTC_TIME_MS && timep >= START_UP_UTC_TIME_MS)
        {
            deadtime = G_realtime_data.tracking_time + G_parameter.work_mode.teardown_time * 60;
            if(deadtime >= timep)
            {
                timep = deadtime - timep;
                LOGD(L_UART, L_V4, "ģʽһ����%d����%d��֮���Զ��л���ģʽ��", timep / 60, timep % 60);
                tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, timep * 1000, track_mode2_to_mode1);
            }
            else
            {
                LOGD(L_UART, L_V4, "WARN: ģʽһ�Ѿ���ʱ����%d����%d�룬�����Զ��л���ģʽ��", timep / 60, timep % 60);
                tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, 1000, track_mode2_to_mode1);
            }
        }
        else if(timep < START_UP_UTC_TIME_MS)
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, 30000, track_cust_module_gt300s_auto_sw_mode);
            cnt++;
            if(cnt > 30)
            {
                cnt = 0;
                track_mode2_to_mode1();
                LOGD(L_UART, L_V5, "15���Ӷ���ȡ����ʱ�䰡timep:%d,G_realtime_data.tracking_time=%d ", timep, G_realtime_data.tracking_time);
            }
            return;
        }
        else
        {
            tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, G_parameter.work_mode.teardown_time * 60 * 1000, track_mode2_to_mode1);
            LOGD(L_UART, L_V5, "ʱ�����Ϊʲô��������timep:%d,G_realtime_data.tracking_time=%d ", timep, G_realtime_data.tracking_time);
        }
    }
}



/******************************************************************************
 *  Function    -  track_set_cust_status_charger
 *
 *  Purpose     -  ���³��״̬���
 *
 *  Description -   GT370A ��Ƭ������֪ͨ
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-03-23,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_charger_status_updata_from_mcu(kal_uint8 chr_in)
{
    static kal_uint8 pre_chr_in = 0xFF;

    ilm_struct *ilm_ptr;
    charbat_status_struct *myMsgPtr = NULL;
    module_type src_mod_id = kal_get_active_module_id();

    if(chr_in == pre_chr_in)
    {
        return;
    }

    pre_chr_in = chr_in;

    LOGD(L_UART, L_V5, "chr_in=%d", chr_in);

    heartbeat_packets((void *)110);
    myMsgPtr = (charbat_status_struct*) construct_local_para(sizeof(charbat_status_struct), TD_CTRL);
    if(myMsgPtr != NULL)
    {
        if(chr_in != 0)
        {
            myMsgPtr->status = PMIC_CHARGER_IN;
        }
        else
        {
            myMsgPtr->status = PMIC_CHARGER_OUT;
        }

        myMsgPtr->level = BATTERY_LEVEL_3 ;
        myMsgPtr->volt = 4000000;

        ilm_ptr = (ilm_struct *)allocate_ilm(src_mod_id);
        if(ilm_ptr != NULL)
        {
            ilm_ptr->src_mod_id = src_mod_id;
            ilm_ptr->dest_mod_id = MOD_MMI;
            ilm_ptr->sap_id = INVALID_SAP;
            ilm_ptr->msg_id = (msg_type) MSG_ID_CHARBAT_BMT_STATUS;
            ilm_ptr->local_para_ptr = (local_para_struct*) myMsgPtr;
            ilm_ptr->peer_buff_ptr = NULL;
            msg_send_ext_queue(ilm_ptr);
        }
    }
}


void track_cust_module_sw_init(void)
{
    char buf[100] = {0};
    //��ʼ������
    LOGD(L_UART, L_V5, "");

    sprintf(buf, "AT^GT_CM=01#");
    track_spi_write(buf, strlen(buf));
    tr_start_timer(TRACK_CUST_GT300S_MODE_INIT_SW_TIMER_ID, 10000, track_cust_module_sw_init);
}

void track_cust_module_delay_close(kal_uint8 op)
{
    LOGD(L_UART, L_V5, "�ػ�ʱ�䵽!:%d", op);
    LOGS("\r\n\r\n!! track_cust_module_delay_close GPS positioning has been completed,Location uploaded!\r\n!! Shutdown...\r\n");
    track_cust_module_extchip_shutdown();
    tr_start_timer(TRACK_POWER_RESET_TIMER_ID, 1000, track_drv_sys_power_off_req);
}

void track_cust_work_off(void)
{
    LOGD(L_UART, L_V5, "����׼���ػ�");
    if(track_cust_get_work_mode() == WORK_MODE_3)
    {
        LOGD(L_UART, L_V5, "����׼���ػ�");
        track_soc_bakup_data((void*)3);
        track_cust_module_delay_close(10);
    }
}


//GSM����AT^GT_CM=14,A,B# ��X=1,��ʾ���ƣ�X=0����ʾ�����
//A���������ͣ�01��ʾ�������02��ʾ��������03��ʾ���Ǽ���04��ʾ�������⣨������Ч����05��ʾSOS������
//B��00��ʾʹ�ܣ�01��ʾʧ��
static void track_cust_key_enable_sync_to_mcu(kal_uint8 key_type, kal_uint8 disable)
{
    char buf[100] = {0};
    sprintf(buf, "AT^GT_CM=14,%02d,%02d#", key_type, disable);
    track_spi_write(buf, strlen(buf));
}

/******************************************************************************
 *  Function    -  track_cust_key_mode_check
 *
 *  Purpose     -  ����ʹ��ͬ��     //B��00��ʾʹ�ܣ�01��ʾʧ��
 *
 *  Description -  GSMÿ��ֻ�ܷ�һ��ͬ����
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-03-27,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_key_mode_check(kal_uint8 pwrlimit_sw, kal_uint8 open_shell_alarm, kal_uint8 teardown_str)
{
    kal_uint8 disable = 1;

    LOGD(L_UART, L_V5, "pwr=%d,%d,open=%d,%d,teardown=%d,%d", G_parameter.pwrlimit_sw, pwrlimit_sw, G_parameter.open_shell_alarm.sw, open_shell_alarm, G_parameter.teardown_str.sw, teardown_str);

    if(G_parameter.pwrlimit_sw != pwrlimit_sw)  //���ƹػ�01��ʹ��Ϊ01
    {
        if(G_parameter.pwrlimit_sw == 1)
        {
            disable = 1;
        }
        else
        {
            disable = 0;
        }
        track_cust_key_enable_sync_to_mcu(2, disable);
    }
    else if(G_parameter.open_shell_alarm.sw == open_shell_alarm)    //���Ǳ�������01��00��ʾʹ��
    {
        if(G_parameter.open_shell_alarm.sw == 1)
        {
            disable = 0;
        }
        else
        {
            disable = 1;
        }

        track_cust_key_enable_sync_to_mcu(3, disable);
    }
    else if(G_parameter.teardown_str.sw == teardown_str)    //���𱨾�����01��00��ʾʹ��
    {
        if(G_parameter.teardown_str.sw == 1)
        {
            disable = 0;
        }
        else
        {
            disable = 1;
        }
        track_cust_key_enable_sync_to_mcu(1, disable);
    }
    else
    {
        LOGD(L_UART, L_V5, "��ֵʹ��ͬ��");
        return;
    }

    if(!track_is_timer_run(TRACK_CUST_GT300S_MODE_INIT_SW_TIMER_ID))
    {
        tr_start_timer(TRACK_CUST_GT300S_MODE_INIT_SW_TIMER_ID, 5000, track_cust_module_sw_init);
    }
}

kal_uint8  track_is_updata(kal_uint8 type)
{
    static kal_uint8 status = 0;
    if(type == 0xff)
    {
        return status;
    }
    else
    {
        status = type;
        return 0xff;
    }
}

/******************************************************************************
 *  Function    -  track_cust_alarm_teardown
 *
 *  Purpose     -  �������
 *
 *  Description -  ��Ƭ����⵽�����¼��󴮿�֪ͨ�豸����
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-03-22,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_alarm_teardown(void)
{
    applib_time_struct tmp = {0};
    nvram_work_mode_struct chip = {0};
    kal_uint32 tick = 0;
    static kal_uint32 cur_tick = 0;
    U8 buf[100] = {0};

    LOGD(L_DRV, L_V5, "teardown_sw:%d,tm:%d,tracking:%d,mode:%d,teardown:%d", \
         G_parameter.teardown_str.sw, G_realtime_data.tracking_status_mode, G_realtime_data.tracking_mode, \
         G_parameter.work_mode.mode,	G_parameter.work_mode.teardown_time);

    if((track_is_testmode() == KAL_TRUE))
    {
        return;
    }
    if(G_parameter.teardown_str.sw == 1)
    {
        if(cur_tick == 0)
        {
            cur_tick = kal_get_systicks();
        }
        else
        {
            tick = kal_get_systicks();
            if(tick - cur_tick > KAL_TICKS_1_MIN * 3)
            {
                cur_tick = tick;
            }
            else
            {
                LOGD(L_UART, L_V5, "���𱨾��������δ����3����!");
                return;
            }
        }
        track_cust_gps_work_req((void *)3);
        //tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_SEND_ALARM_TIMER_ID, 30*1000, track_cust_delay_alarm);
        track_os_intoTaskMsgQueue(track_cust_pre_remove_alarm);
        if(G_parameter.teardown_str.alarm_type > 0 && G_parameter.teardown_str.alarm_type < 3) //1��2
        {
            //����
            tr_start_timer(TRACK_CUST_EXTCHIP_DELAY_SEND_ALARM_MSG_TIMER_ID, 60 * 1000, teardown_alarm_overtime);
        }
        if(G_parameter.teardown_str.alarm_type > 1) //2��3
        {
            //call
            track_start_timer(TRACK_CUST_ALARM_TEAR_DOWN_CALL_TIMER, 10 * 1000, track_cust_make_call, (void*)1);
        }

        if(G_parameter.work_mode.mode == 1 || G_parameter.work_mode.mode == 2 && G_realtime_data.tracking_mode == 0)
        {
            LOGD(L_UART, L_V5, "ģʽ%d,�Ƿ���׷��%d ����", G_parameter.work_mode.mode, G_realtime_data.tracking_mode);
            return;
        }

        if(G_realtime_data.tracking_mode == 0)
        {
            G_parameter.extchip.mode1_par1 = G_parameter.gps_work.Fti.interval_acc_on;
            G_parameter.extchip.mode1_par2 = G_parameter.gps_work.Fti.interval_acc_off;
            G_parameter.gps_work.Fti.interval_acc_on = 20;
            G_parameter.gps_work.Fti.interval_acc_off = 20;
            G_realtime_data.tracking_status_mode = G_parameter.work_mode.mode;
        }

        track_cust_module_tracking_mode(1);

        memcpy(&chip, &G_parameter.work_mode, sizeof(nvram_work_mode_struct));
        chip.mode = G_parameter.work_mode.teardown_mode;

        OTA_applib_dt_get_rtc_time(&tmp);
        G_realtime_data.tracking_time = OTA_applib_dt_mytime_2_utc_sec(&tmp, 0);
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);

        LOGD(L_UART, L_V5, "%d time: %0.2d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d    sec:%d", G_parameter.work_mode.teardown_time,
             tmp.nYear, tmp.nMonth, tmp.nDay, tmp.nHour, tmp.nMin, tmp.nSec, G_realtime_data.tracking_time);

        memcpy(&chip, &G_parameter.work_mode, sizeof(nvram_work_mode_struct));
        chip.mode = G_parameter.work_mode.teardown_mode;

        memcpy(&G_parameter.work_mode, &chip, sizeof(nvram_work_mode_struct));
        //Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
        if(Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE))
        {
            //track_cust_upload_param();//����״̬�ı��ϴ�ֻ��01��ѯʱ��һ���ϴ�
            //track_cust_module_gt300s_auto_sw_mode();
            tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, \
                           G_parameter.work_mode.teardown_time * 60 * 1000 + 5000, track_mode2_to_mode1);
        }
        else
        {
            LOGD(L_UART, L_V5, "error write nvram 05!");
        }

        track_cust_module_sw_mode_to_mcu();
        track_cust_wake_mode();
    }
    /*
    if(track_cust_gps_status() >= GPS_STAUS_2D)
    {
        track_gps_data_struct *p_gpsdata = track_cust_backup_gps_data(0, NULL);
        if(p_gpsdata->gprmc.status != 0 && (p_gpsdata->gprmc.Latitude != 0 || p_gpsdata->gprmc.Longitude != 0))
        {
            track_cust_paket_position(p_gpsdata, track_drv_get_lbs_data(), TR_CUST_POSITION_KEY, 0);
        }
    }
    else
    {
        track_cust_sendlbs_limit();
    }
    */
}

/******************************************************************************
 *  Function    -  track_cust_alarm_openshell
 *
 *  Purpose     -  ����Ǳ���
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-03-23,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_alarm_openshell()
{
    applib_time_struct tmp = {0};
    nvram_work_mode_struct chip = {0};
    kal_uint32 tick = 0;
    static kal_uint32 cur_tick = 0;
    U8 buf[100] = {0};

    LOGD(L_DRV, L_V5, "open_shell_alarm:%d,tm:%d,tracking:%d,mode:%d,teardown:%d", \
         G_parameter.open_shell_alarm.sw, G_realtime_data.tracking_status_mode, G_realtime_data.tracking_mode, \
         G_parameter.work_mode.mode,	G_parameter.work_mode.teardown_time);

    if((track_is_testmode() == KAL_TRUE))
    {
        return;
    }

    if(G_parameter.open_shell_alarm.sw == 1)
    {
        if(cur_tick == 0)
        {
            cur_tick = kal_get_systicks();
        }
        else
        {
            tick = kal_get_systicks();
            if(tick - cur_tick > KAL_TICKS_1_MIN * 3)
            {
                cur_tick = tick;
            }
            else
            {
                LOGD(L_UART, L_V5, "���Ǳ����������δ����3����!");
                return;
            }
        }
        track_cust_gps_work_req((void *)3);

        track_os_intoTaskMsgQueue(track_cust_pre_open_shell_alarm);
        if(G_parameter.open_shell_alarm.alarm_type > 0 && G_parameter.open_shell_alarm.alarm_type < 3) //1��2
        {
            //����
            tr_start_timer(TRACK_CUST_SEND_OPEN_SHELL_SMS_TIMEOVER_TIMER_ID, 60 * 1000, track_open_shell_alarm_overtime);
        }
        if(G_parameter.open_shell_alarm.alarm_type > 1) //2��3
        {
            //call
            track_start_timer(TRACK_CUST_ALARM_TEAR_DOWN_CALL_TIMER, 10 * 1000, track_cust_make_call, (void*)1);
        }

        if(G_parameter.work_mode.mode == 1 || G_parameter.work_mode.mode == 2 && G_realtime_data.tracking_mode == 0)
        {
            LOGD(L_UART, L_V5, "ģʽ%d,�Ƿ���׷��%d ����", G_parameter.work_mode.mode, G_realtime_data.tracking_mode);
            return;
        }

        if(G_realtime_data.tracking_mode == 0)
        {
            G_parameter.extchip.mode1_par1 = G_parameter.gps_work.Fti.interval_acc_on;
            G_parameter.extchip.mode1_par2 = G_parameter.gps_work.Fti.interval_acc_off;
            G_parameter.gps_work.Fti.interval_acc_on = 20;
            G_parameter.gps_work.Fti.interval_acc_off = 20;
            G_realtime_data.tracking_status_mode = G_parameter.work_mode.mode;
        }

        track_cust_module_tracking_mode(1);

        memcpy(&chip, &G_parameter.work_mode, sizeof(nvram_work_mode_struct));
        chip.mode = G_parameter.work_mode.teardown_mode;

        OTA_applib_dt_get_rtc_time(&tmp);
        G_realtime_data.tracking_time = OTA_applib_dt_mytime_2_utc_sec(&tmp, 0);
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);

        LOGD(L_UART, L_V5, "%d time: %0.2d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d    sec:%d", G_parameter.work_mode.teardown_time,
             tmp.nYear, tmp.nMonth, tmp.nDay, tmp.nHour, tmp.nMin, tmp.nSec, G_realtime_data.tracking_time);

        memcpy(&chip, &G_parameter.work_mode, sizeof(nvram_work_mode_struct));
        chip.mode = G_parameter.work_mode.teardown_mode;

        memcpy(&G_parameter.work_mode, &chip, sizeof(nvram_work_mode_struct));
        //Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
        if(Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE))
        {
            //track_cust_upload_param();//����״̬�ı��ϴ�ֻ��01��ѯʱ��һ���ϴ�
            //track_cust_module_gt300s_auto_sw_mode();
            tr_start_timer(TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID, \
                           G_parameter.work_mode.teardown_time * 60 * 1000 + 5000, track_mode2_to_mode1);
        }
        else
        {
            LOGD(L_UART, L_V5, "error write nvram 05!");
        }

        track_cust_module_sw_mode_to_mcu();
        track_cust_wake_mode();
    }

}

//AT^GT_CM=01,GT300S_2.0_V1.0_160101,345,17:03:21:13:05,2,50,300,10,0,0,0,0,0,0,0,checksum#
void cmd_01(void)
{
    int mode, i, sum = 0;
    char *p, *q = NULL, tmp[150] = {0};
    kal_uint16 datetime;
    kal_uint16 update_time, timer;
    kal_uint8 pwrlimit_sw, open_shell_alarm, teardown_str, chr_in;
    static   kal_uint8 flg = 0;
    static U8 singlebuf[100] = {0};

    tr_stop_timer(TRACK_CUST_GT300S_MODE_INIT_SW_TIMER_ID);

    if(cmd_parameters_count != 16)
    {
        LOGD(L_UART, L_V1, "��Ƭ������汾����");
        return;
    }

    for(i = 0; i < cmd_parameters_count; i++)
    {
        if(i >= 2 && i < cmd_parameters_count - 1)
        {
            sum += atoi(cmd_parameters[i]);

            p = strchr(cmd_parameters[i], ':');
            while(p != NULL)
            {
                if(q != NULL)
                {
                    sum += atoi(q);
                    LOGD(L_UART, L_V6, "sum=%d",  sum);
                }

                q = p + 1;
                p = strchr(q, ':');
                if(p == NULL)
                {
                    sum += atoi(q);
                    LOGD(L_UART, L_V6, "sum=%d",  sum);
                }
            }
        }
        LOGD(L_UART, L_V6, "[%d]%s -> %d", i, cmd_parameters[i], sum);
    }

    if(sum != atoi(cmd_parameters[cmd_parameters_count - 1]))
    {
        LOGD(L_UART, L_V1, "У��ʹ��� %d,%d", sum, atoi(cmd_parameters[cmd_parameters_count - 1]));
        return;
    }
#if defined (__MCU_SW_UPDATE__)
    memset(mcu_sw_version_string, 0x00, sizeof(mcu_sw_version_string));
    snprintf(mcu_sw_version_string, sizeof(mcu_sw_version_string) - 1, "%s", cmd_parameters[1]);
#endif /* __MCU_SW_UPDATE__ */

    snprintf(tmp, 150, "version:%s,vBat:%s,time:%s\r\nmode:%s,%s; start:%s;upgrade:%s\r\nkey:%s,%s,%s;status:%s,%s,%s,%s",
             cmd_parameters[1], cmd_parameters[2], cmd_parameters[3], cmd_parameters[4], cmd_parameters[5], cmd_parameters[6], cmd_parameters[7],
             cmd_parameters[8], cmd_parameters[9], cmd_parameters[10], cmd_parameters[11], cmd_parameters[12], cmd_parameters[13], cmd_parameters[14]);

    confirm(tmp);
    mode = atoi(cmd_parameters[4]);
    timer = atoi(cmd_parameters[5]);
    datetime = atoi(cmd_parameters[6]);
    update_time = atoi(cmd_parameters[7]);
    pwrlimit_sw = atoi(cmd_parameters[8]);
    open_shell_alarm = atoi(cmd_parameters[9]);
    teardown_str = atoi(cmd_parameters[10]);

    chr_in = atoi(cmd_parameters[12]);
    track_cust_charger_status_updata_from_mcu(chr_in);

    if(flg == 0)
    {
        flg = 1;
        snprintf(singlebuf, 99, "ver:%s,bat:%s,timer:%s,mode:%s,type:%d\n\r", cmd_parameters[1], cmd_parameters[2], cmd_parameters[3], cmd_parameters[4], mmi_Get_PowerOn_Type());
        track_cust_save_logfile(singlebuf, strlen(singlebuf));
    }
    if(mode == 0) mode++;

    LOGD(L_APP, L_V5, "mode:%d,work mode:%d,mode3 timer:%d,work datetime:[%d,%d]", mode, G_parameter.work_mode.mode, G_parameter.work_mode.mode3_timer, \
         G_parameter.work_mode.datetime, datetime);

    if((mode == 1) && (G_parameter.work_mode.mode != 1 && G_parameter.work_mode.mode != 2))
    {
        LOGD(L_UART, L_V1, "ģʽ1 ��һ��,%d,%d,%d", G_parameter.work_mode.mode1_timer, timer, G_parameter.work_mode.mode);
        track_cust_module_sw_mode_to_mcu();
    }
    else if(mode == 2)
    {
        if(G_parameter.work_mode.mode3_timer != 0 && G_parameter.work_mode.mode3_timer != timer || G_parameter.work_mode.mode != 3)
        {
            LOGD(L_UART, L_V1, "ģʽ2 ��һ��,%d,%d,%d", G_parameter.work_mode.mode3_timer, timer, G_parameter.work_mode.mode);
            track_cust_module_sw_mode_to_mcu();
            track_cust_get_mode_status(0);

        }
        else
        {
            track_cust_get_mode_status(1);
            LOGD(L_UART, L_V1, "ģʽ����һ��");
        }

    }
    if(datetime != G_parameter.work_mode.datetime && mode == 2)
    {
        LOGD(L_UART, L_V1, "��ʼʱ�䲻һ%d,%d", datetime, G_parameter.work_mode.datetime);
        track_cust_module_sw_mode_to_mcu();
        track_cust_get_mode_status(0);
    }
    else
    {
        LOGD(L_UART, L_V1, "��ʼʱ��һ��");
    }

    track_cust_key_mode_check(pwrlimit_sw, open_shell_alarm, teardown_str);

    track_cust_module_gt300s_auto_sw_mode();//׷��ģʽ
    if(G_realtime_data.switch_down != atoi(cmd_parameters[10]))
    {
        G_realtime_data.switch_down = atoi(cmd_parameters[10]);
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
        //track_cust_upload_param();
    }
}

static void cmd_02(void)
{
    U8 buf[100] = {0};
    LOGD(L_UART, L_V5, "");
    confirm("02 OK!");
    if(!g_02_ok) return;
    track_cust_module_sw_mode_to_mcu();
}

static void cmd_03(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("03 OK!");
    track_drv_sys_power_off_req();
}
void cmd_04(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("04 OK!");
    track_stop_timer(TRACK_CUST_GT300S_MODE_AUTO_SW_TIMER_ID);
    track_cust_get_mode_status(1);
}


static void cmd_05(void)
{
    U8 buf[100] = {0};

    if(track_is_testmode())
    {
        if(strcmp(cmd_parameters[2], "03") == 0)
        {
            snprintf(buf, 100, "KEY=05,%s,02#", cmd_parameters[1]);//��Ƭ���ӵ����ģ�ǿ��Ϊ���Ը�
        }
        else
        {
            snprintf(buf, 100, "KEY=05,%s,%s", cmd_parameters[1], cmd_parameters[2]);
        }
    }
    else
    {
        snprintf(buf, 100, "KEY=05,%s,%s", cmd_parameters[1], cmd_parameters[2]);
    }
    confirm(buf);

    sprintf(buf, "AT^GT_CM=05,OK#\r\n");
    track_spi_write(buf, strlen(buf));

    if((track_is_testmode() == KAL_TRUE))
    {
        return;
    }

    if(strcmp(cmd_parameters[2], "01") == 0)   //����
    {
        if(strcmp(cmd_parameters[1], "02") == 0) //������
        {
            LOGD(L_UART, L_V5, "LED 1����");
            track_cust_led_sleep(3);
            tr_start_timer(TRACK_CUST_LED_START_TIMER, 60000, track_cust_led_first_start);
        }
        else if(strcmp(cmd_parameters[1], "04") == 0) //��������
        {
            track_cust_charger_status_updata_from_mcu(1);
        }
    }
    else if(strcmp(cmd_parameters[2], "02") == 0)   //����
    {
        if(strcmp(cmd_parameters[1], "04") == 0) //�γ������
        {
            track_cust_charger_status_updata_from_mcu(0);
        }
    }
    else if(strcmp(cmd_parameters[2], "03") == 0)    //����
    {
        if(strcmp(cmd_parameters[1], "01") == 0) //���𱨾�
        {
            track_cust_alarm_teardown();
        }
        else if(strcmp(cmd_parameters[1], "03") == 0) //���Ǳ���
        {
            track_cust_alarm_openshell();
        }
    }
}

static void cmd_06(void)
{
    U8 buf[32] = {0};
    snprintf(buf, 32, "KEY=06,%s", cmd_parameters[1]);
    confirm(buf);

    sprintf(buf, "AT^GT_CM=06,OK#\r\n");
    track_spi_write(buf, strlen(buf));

    if(strcmp(cmd_parameters[1], "01") == 0)
    {
        if(track_is_timer_run(TRACK_CUST_PWROFFALM_TIMER_ID))
        {
            tr_stop_timer(TRACK_CUST_PWROFFALM_TIMER_ID);
        }
    }
    else if(strcmp(cmd_parameters[1], "02") == 0)
    {
        LOGD(L_UART, L_V3, "׼���ػ�");
        track_cust_receive_power_off_msg();
    }
}

/******************************************************************************
 *  Function    -  track_send_07
 *
 *  Purpose     -  GSM����������,��֪MCU
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-03-22,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_send_07(void)
{
    U8 buf[100] = {0};
    static kal_uint8 cnt = 0;
    LOGD(L_UART, L_V5, "");
    sprintf(buf, "AT^GT_CM=07,OK#");
    track_spi_write(buf, strlen(buf));
    if(cnt < 3)
    {
        cnt++;
        tr_start_timer(TRACK_CUST_07_TIMEOUT_TIMER, 1000, track_send_07);
    }
}

static void cmd_07(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("07 OK!");
    tr_stop_timer(TRACK_CUST_07_TIMEOUT_TIMER);
}

static void cmd_08(void)
{
    U8 buf[100] = {0};
    LOGD(L_UART, L_V5, "");
    confirm("08 OK!");

    sprintf(buf, "AT^GT_CM=08,OK#");
    track_spi_write(buf, strlen(buf));
}

static void cmd_10(void)
{
    U8 buf[100] = {0};
    LOGD(L_UART, L_V5, "");
    confirm("10 OK!");

    sprintf(buf, "AT^GT_CM=10,OK#");
    track_spi_write(buf, strlen(buf));
}

static void cmd_14(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("14 OK!");

}

static void cmd_15(void)
{
    LOGD(L_UART, L_V5, "");
    confirm("15 OK!");
}

static void cmd_99(void)
{
#if defined (__MCU_SW_UPDATE__)
    track_drv_mcu_sw_update_rsp();
#endif /* __MCU_SW_UPDATE__ */
    LOGD(L_UART, L_V5, "");
    confirm("99 OK!");
}

static cust_recv_struct CMD_Data[] =
{
    {"01",      cmd_01},
    {"02",      cmd_02},
    {"03",      cmd_03},
    {"04",      cmd_04},
    {"05",      cmd_05},
    {"06",      cmd_06},
    {"07",      cmd_07},
    {"08",      cmd_08},
    {"10",      cmd_10},
    {"14",      cmd_14},
    {"15",      cmd_15},
    {"99",      cmd_99},

};

void track_cust_module_power_cmd(cmd_return_struct *return_sms, kal_uint8 *data, int len)
{
    memcpy(&g_return_sms, return_sms, sizeof(cmd_return_struct));
    g_return_sms_valid = 1;
    track_spi_write(data, len);
}

void track_cust_module_extchip_time_synchronization(kal_uint32 arg)
{
    LOGD(L_UART, L_V5, "%d", arg);
    if(!g_02_ok)
    {
        applib_time_struct time = {0};
        g_02_ok = KAL_TRUE;
        track_fun_get_time(&time, KAL_FALSE, &G_parameter.zone.zone);
        LOGD(L_UART, L_V5, "time: %02d-%02d-%02d %02d:%02d:%02d",
             time.nYear,
             time.nMonth,
             time.nDay,
             time.nHour,
             time.nMin,
             time.nSec);
        if(time.nYear >= 2016)
        {
            U8 buf[100] = {0}, valid = 0;

            sprintf(buf, "AT^GT_CM=02,%02d,%02d,%02d,%02d,%02d#", time.nYear - 2000, time.nMonth, time.nDay, time.nHour, time.nMin);
            track_spi_write(buf, strlen(buf));

            if(G_realtime_data.tracking_time < 1422748800 && G_realtime_data.tracking_time + arg >= 1422748800)
            {
                G_realtime_data.tracking_time += arg;
                Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
            }
            track_cust_module_gt300s_auto_sw_mode();
        }
    }
}

void track_cust_module_Handshake(void)
{
    char buf[100] = {0};
    sprintf(buf, "AT^GT_CM=08#");
    track_spi_write(buf, strlen(buf));
    LOGD(L_UART, L_V5, "");
    tr_start_timer(TRACK_CUST_EXTCHIP_BATTERY_VOLTAGE_TIMER_ID, 5000, track_cust_module_Handshake);
}



void track_cust_spi_read_data(kal_uint8 *data, kal_uint16 len)
{
    char* p;
    int size, i, ret;
    LOGD(L_DRV, L_V4, "��Ƭ����Ϣ:%s", data);
    if(G_realtime_data.netLogControl & 1)
    {
        cust_packet_log_data(1, data, len);
    }

    p = strstr(data, "GT_CM=");
    if(p != NULL)
    {
        cmd_parameters_count = track_fun_str_analyse(data + 9, cmd_parameters, PARAMETER_MAX, NULL, "#", ',');
        size = sizeof(CMD_Data) / sizeof(cust_recv_struct);
        LOGD(L_UART, L_V4, "part=%d", cmd_parameters_count);

        for(i = 0; i < size; i++)
        {
            ret = strcmp(cmd_parameters[0], CMD_Data[i].cmd_string);
            if(!ret)
            {
                LOGD(L_UART, L_V5, "Tab[%d], cmd=%s", i, CMD_Data[i].cmd_string);
                if(CMD_Data[i].func_ptr != NULL)
                {
                    CMD_Data[i].func_ptr();
                }
                break;
            }
        }
    }
}


void track_cust_module_power_data(kal_uint8 *data, int len)
{
    track_cust_spi_read_data(data, len);
}

void track_spi_write(kal_uint8 *data, kal_uint8 len)
{
    char at_content[128] = {0};
    memcpy(&at_content[0], data, len);
    at_content[len] = 0x0d;
    at_content[len + 1] = 0x0a;
    track_drv_write_exmode(at_content, len + 2);
    //track_drv_write_exmode("\r\n", 2);
}

void track_cust_poweroff_ready()
{
    char buf[100] = {0};

    track_cust_restart(80, 10); //������û�����������ǹػ�ǰ���ݴ�����Ƭ����ػ���
    sprintf(buf, "AT^GT_CM=03,2#");
    track_spi_write(buf, strlen(buf));
}

#endif /*__GT740__*/


