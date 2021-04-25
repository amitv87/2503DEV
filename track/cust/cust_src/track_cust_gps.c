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
#include "track_os_ell.h"
/*****************************************************************************
*  Define                               �궨��
******************************************************************************/

#if defined(__TRACK_HAND_DEVICE__)
#define  WHERE_GPS_WORK_TIME   120 * 1000
#define  DW_CMD_GPS_WORK_TIME  130 * 1000
#elif defined (__TRACK_CAR_DEVICE__)
#define  WHERE_GPS_WORK_TIME   180 * 1000
#define  DW_CMD_GPS_WORK_TIME  310 * 1000
#else
#define  WHERE_GPS_WORK_TIME   300 * 1000
#define  DW_CMD_GPS_WORK_TIME  310 * 1000
#endif

#define  UP_TIME   10

#define  EXTCHANGE_MODE_GPS_WORK_TIME   3 * 60 * 1000

/*****************************************************************************
*  Typedef  Enum                        ö�ٶ���
*****************************************************************************/

/*****************************************************************************
*  Struct                               ���ݽṹ����
******************************************************************************/

/*****************************************************************************
*  Local variable                       �ֲ�����
*****************************************************************************/
static track_enum_gps_staus gps_status = 0;   // 0:GPS�� 1:GPS�� 2:���� 3:2D��λ 4:3D��λ
static track_enum_gps_staus gps_before_off_status = 1;
static kal_uint8 net_upload_position = 0; //��GPS������״̬�£���һ�������Ƿ���Ҫ�ϱ���λ�㵽������
static kal_uint8 g_restart_gps_count = 0;
static kal_bool positioned = KAL_FALSE;     // �Ƿ� ����֮��������λ��
static kal_uint8 GPS_NOT_OPEN = 0;// ��������GPS

/****************************************************************************
*  Global Variable                      ȫ�ֱ���
*****************************************************************************/

/****************************************************************************
*  Global Variable - Extern             ����ȫ�ֱ���
*****************************************************************************/

/*****************************************************************************
*  Global Functions - Extern            �����ⲿ����
******************************************************************************/
extern void track_cust_gps_nodata_restart(void);
extern  void track_cust_sleep_mode(void);
extern void track_cust_module_get_addr(CMD_DATA_STRUCT *_cmd);
extern kal_uint32 track_cust_gpson_timer(kal_uint32 time);
extern void acc_status_change(void *arg);
extern kal_uint8 track_cust_status_acc(void);
#if defined(__NF2318__)
extern kal_uint8 track_cust_halfsleep_position(U8 param,track_gps_data_struct *gps_data);
extern void track_cust_paging_overtime(void *arg);
#endif
/*****************************************************************************
*  Local Functions Define               ���غ�������
******************************************************************************/
static void gps_location_timeout_ind(void * arg);
void track_cust_gps_work_req(void *arg);
//void track_cust_mode3_update_LBS(void *f);
void track_cust_intermittent_send_LBS(void *arg);
void track_cust_instant_contract_position_data(kal_uint8 flag);
#if 0
#endif /* 0 */
/*****************************************************************************
*  Local Functions                      ���غ���
******************************************************************************/


/******************************************************************************
 *  Function    -  gps_location_timeout_ind
 *
 *  Purpose     -  ���Ƕ�λ��ʱ�Ĵ�����
 *
 *  Description -  NULL
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 04-09-2012,  Cml  written
 * v1.1  , 07-11-2013,  ZengPing && JWJ  written
 * ----------------------------------------
*******************************************************************************/
static void gps_location_timeout_ind(void * arg)
{
    kal_uint8 work_mode = track_cust_get_work_mode();
    U8 par = (U8)arg;
	vehicle_status_struct v_status = {0};
	track_cust_get_controller_data(&v_status);
    LOGD(L_GPS, L_V5, "gps_mode:%d,%d", work_mode, par);
#if defined(__GT300S__) || defined(__GT370__)|| defined (__GT380__)
    if(track_cust_get_work_mode() == WORK_MODE_3)
    {
        return;
    }
#endif

    if((!track_is_timer_run(TRACK_CUST_ALARM_GPS_WORK_TIMER)) && (!track_is_timer_run(TRACK_CUST_GPS_LOCATION_TIMEOUT_TIMER_ID)) \
            && (!track_is_timer_run(TRACK_CUST_POWERON_CLOSE_GPS_TIMER)) && (!track_is_timer_run(TRACK_CUST_DW_GPS_WORK_TIMER)) \
            && (!track_is_timer_run(TRACK_CUST_WHERE_GPS_WORK_TIMER_ID)) && (!track_is_timer_run(TRACK_CUST_KEY_UPDATA_OVER_TIMER_ID))\
            && (!track_is_timer_run(TRACK_CUST_GPS_LOCATION_WORKTIME_TIMER_ID)) && (!track_is_timer_run(TRACK_CUST_WORK_MODE_GPSUPDATE_OVERTIME_TIMER_ID))\
            && (!track_is_timer_run(TRACK_CUST_GPS_UPDATE_EPHEMERIS_OVERTIME_TIMER_ID)) && (!track_is_timer_run(TRACK_CUST_WORK_MODE_GPSON_UPDATE_TIMER_ID))
            && (!track_is_timer_run(TRACK_CUST_WORK_MODE3__GPSON_TIMER_ID))  && (!track_is_timer_run(TRACK_CUST_FT_WORK_TIMER_ID)))
    {
#if defined(__SAME_GT02__) || defined(__TRACK_CAR_DEVICE__)
#if defined(__NF2318__)
		LOGD(L_GPS, L_V5, "wheel:%d,ele:%d,sleep:%d,acc:%d,alarmacc:%d,speed:%d,worktime:%d,save_mode:%d",track_cust_wheel_state(),v_status.ele_sta,G_realtime_data.sleep, track_cust_status_acc(),track_acc_status((void*)3), track_drv_get_gps_RMC()->Speed, G_parameter.sensor_gps_work,G_realtime_data.power_saving_mode);
		if(track_cust_wheel_state() == KAL_FALSE && v_status.ele_sta == 0 && G_realtime_data.sleep != 0 && (track_cust_status_acc() == 0 || track_acc_status((void*)3) == 1) && track_drv_get_gps_RMC()->Speed < 20 && (G_parameter.sensor_gps_work != 0 || G_realtime_data.power_saving_mode == 1))//(track_cust_status_acc() == 0 || track_acc_status((void*)3) == 1)
#else
        if(track_cust_status_acc() == 0 && track_drv_get_gps_RMC()->Speed < 20 && (G_parameter.sensor_gps_work != 0 || G_realtime_data.power_saving_mode == 1))
#endif			
        {

            track_cust_gps_off_req((void*)3);
        }
        else
        {
            //LOGD(L_GPS, L_V5, "track_cust_status_acc() =%d,seep=%d,G_parameter.sensor_gps_work=%d ", track_cust_status_acc(), track_drv_get_gps_RMC()->Speed, G_parameter.sensor_gps_work);
            LOGD(L_GPS, L_V5, "rst work T=%d m", G_parameter.gps_work.location_timeout);

            track_start_timer(TRACK_CUST_GPS_LOCATION_TIMEOUT_TIMER_ID, G_parameter.gps_work.location_timeout * 60000, gps_location_timeout_ind, (void*)1);
        }
#else
        track_cust_gps_off_req((void*)3);
#endif
    }
    else
    {
        if(track_is_timer_run(TRACK_CUST_ALARM_GPS_WORK_TIMER))
        {
            LOGD(L_GPS, L_V6, "1");
        }
        else if(track_is_timer_run(TRACK_CUST_GPS_LOCATION_TIMEOUT_TIMER_ID))
        {
            LOGD(L_GPS, L_V6, "2");
        }
        else if(track_is_timer_run(TRACK_CUST_POWERON_CLOSE_GPS_TIMER))
        {
            LOGD(L_GPS, L_V6, "3");
        }
        else if(track_is_timer_run(TRACK_CUST_DW_GPS_WORK_TIMER))
        {
            LOGD(L_GPS, L_V6, "4");
        }
        else if(track_is_timer_run(TRACK_CUST_WHERE_GPS_WORK_TIMER_ID))
        {
            LOGD(L_GPS, L_V6, "5");
        }
        else if(track_is_timer_run(TRACK_CUST_KEY_UPDATA_OVER_TIMER_ID))
        {
            LOGD(L_GPS, L_V6, "6");
        }
        else if(track_is_timer_run(TRACK_CUST_GPS_LOCATION_WORKTIME_TIMER_ID))
        {
            LOGD(L_GPS, L_V6, "7");
        }
        else if(track_is_timer_run(TRACK_CUST_WORK_MODE_GPSUPDATE_OVERTIME_TIMER_ID))
        {
            LOGD(L_GPS, L_V6, "8");
        }
        else if(track_is_timer_run(TRACK_CUST_GPS_UPDATE_EPHEMERIS_OVERTIME_TIMER_ID))
        {
            LOGD(L_GPS, L_V6, "9");
        }
        else if(track_is_timer_run(TRACK_CUST_WORK_MODE_GPSON_UPDATE_TIMER_ID))
        {
            LOGD(L_GPS, L_V6, "10");
        }
        else
        {
            LOGD(L_GPS, L_V6, "11");
        }
    }
}

/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/




/******************************************************************************
 *  Function    -  gps_on
 *
 *  Purpose     -  ����GPS����
 *
 *  Description -  return Null
 *
 * modification history  //����GPS������track_cust_gps_work_req �����������������ֱ�ӵ���
 �˺�������������û�����ǳ�ʱ����
 * ----------------------------------------
 * v1.0  , 27-07-2012,  Liujw  written
 * v1.1  , 08-10-2013,  WangQi && JiangWeiJun
 * ----------------------------------------
 ******************************************************************************/
void track_cust_gps_control(kal_uint8 on)
{
#if defined(__AMS_HEX_LOG__)
            {
                kal_uint8 data[8] = {0};
                data[0] = on;
                track_os_file_log(1, 0x05, data);
            }
#endif
    LOGD(L_GPS, L_V5, "control:%d,%d,%d", on, track_cust_status_charger(), gps_status);
#if defined(__TEST_DEVICE__)
    if(track_rid_given()) return;
#endif /* __TEST_DEVICE__ */
    if(on == 1)
    {
#if defined(__FLY_MODE__) && !defined(__MT200__)
        if(G_parameter.flycut != 0 && track_cust_isbatlow() && !(track_is_testmode()))
        {
            LOGD(L_GPS, L_V4, "��⵽�𶯣�����ص͵����");
            return;
        }
#endif /* __FLY_MODE__ */
        if(gps_status == 0)
        {
            gps_status = GPS_STAUS_ON;
            track_cust_gps_switch(1);
#if defined(__GT740__)
            track_stop_timer(TRACK_CUST_GPSUP_SEND_TIMER_ID);
#endif


#if defined(__GT300S__)

#elif defined(__ET310__) || defined(__MT200__)
            if(G_parameter.led_sleep != 3)
            {
                track_cust_led_sleep(1);
            }
#else
            if(G_parameter.led_sleep != 3 && G_realtime_data.key_status == 1)
            {
                track_cust_led_sleep(1);
            }
#endif
            g_restart_gps_count = 0;

            tr_start_timer(TRACK_CUST_PERIODIC_RESTART_GPS_TIMER_ID, 20 * 1000, track_cust_gps_nodata_restart);

            if(G_realtime_data.netLogControl & 8)
            {
                cust_packet_log(8, 1, 0, 0, 0, 0, 0);
            }
            tr_stop_timer(TRACK_CUST_GPS_EPHEMERIS_UPDATE_TIMER_ID);
        }
    }
    else if(on == 0)
    {
        if(gps_status > 0)
        {
            track_cust_instant_contract_position_data(2);

            track_cust_gps_check_nofix(0);
            track_cust_gps_switch(0);

            track_stop_timer(TRACK_CUST_GPS_LOCATION_WORKTIME_TIMER_ID);

            track_stop_timer(TRACK_CUST_GPS_LOCATION_TIMEOUT_TIMER_ID);
            track_stop_timer(TRACK_CUST_GPS_UPDATE_EPHEMERIS_OVERTIME_TIMER_ID);
            track_stop_timer(TRACK_CUST_PERIODIC_RESTART_GPS_TIMER_ID);
#if defined(__GT300S__)
            if(G_parameter.led_sleep != 2 && !track_is_timer_run(TRACK_CUST_SOS_KEY_LED_ON_TIMER) && !track_is_timer_run(TRACK_CUST_POWERON_LED_TIME_ID))
            {
                track_cust_led_sleep(2);
            }
#elif defined(__ET310__) || defined(__MT200__)
            if(G_parameter.led_sleep != 2)
            {
                track_cust_led_sleep(2);
            }
#elif defined (__NT33__)
            if(G_realtime_data.key_status == 1)
            {
                if(G_parameter.led_sleep != 2)
                {
                    track_cust_led_sleep(2);
                }
            }
#else
            if(G_parameter.led_sleep != 2)
            {
                track_cust_led_sleep(2);
            }
#endif

#if defined(__GT740__)
            if(G_parameter.gpsup_struct.sw == 1)
            {
                if(G_parameter.extchip.mode == 1)
                {
                    track_cust_gpsoff_gpsup((void*)gps_status);
                }
            }
            gps_before_off_status = gps_status;
#endif

            gps_status = GPS_STAUS_OFF;

#if defined(__GT740__)
            if(G_parameter.extchip.mode == 1)
            {
                track_cust_sensor_to_rest_supp_wifihot();
            }
#endif
            net_upload_position = 0;

            g_restart_gps_count = 0;
            /*��AGPS����ȥ��EPH����*/
#if 0
            if(G_parameter.gps_work.ephemeris_interval > 0)
            {
                track_start_timer(TRACK_CUST_GPS_EPHEMERIS_UPDATE_TIMER_ID, G_parameter.gps_work.ephemeris_interval * 60000, track_cust_gps_work_req, (void *)1);
            }
#endif
            if(G_realtime_data.netLogControl & 8)
            {
                cust_packet_log(8, 0, 0, 0, 0, 0, 0);
            }
            //track_cust_module_alarm_blind();

#if defined(__GPS_INTERMITTENT_START__)
            //track_cust_stop_update_LBS();dddd
            track_cust_gprson_status(0);

            if(track_is_timer_run(TRACK_CUST_WORK_MODE_GPSUPDATE_TIMER_ID) == KAL_FALSE && track_cust_get_work_mode() == WORK_MODE_1)
            {
#if defined(__GT300S__)
                track_mode1_gt300s_reset_work();
#elif defined(__GT370__)|| defined (__GT380__)
                track_cust_mode_change(2);;
#else
                tr_stop_timer(TRACK_CUST_GPS_EPHEMERIS_UPDATE_TIMER_ID);
                track_start_timer(TRACK_CUST_WORK_MODE_GPSUPDATE_TIMER_ID, G_parameter.gps_work.Fti.mode1_gps_time  * 60 * 1000, track_cust_gps_work_req, (void *)13);
#endif /*  __GT300S__ */
            }
#if defined(__GT300S__) || defined(__GT370__)|| defined (__GT380__)
#else
            if(track_cust_gprson_status(99) == 0 && track_cust_get_work_mode() == WORK_MODE_3)
            {
                track_os_intoTaskMsgQueue2(track_cust_sleep_mode);
            }
#endif /* __GT300S__ */

#endif
        }
    }
}
/******************************************************************************
 *  Function    -  track_cust_gps_location_timeout
 *
 *  Purpose     -  ����GPS��ʱ�رպ���
 *
 *  Description -  time ����Ϊ��λ
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 03-09-2013,  ZengPing && Liujw  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_gps_location_timeout(kal_uint32 time)
{
    LOGD(L_GPS, L_V6, " time =%d", time);
    track_start_timer(TRACK_CUST_WHERE_GPS_WORK_TIMER_ID, time * 1000, gps_location_timeout_ind, (void*)10);
}
/******************************************************************************
 *  Function    -  track_cust_gps_off_req
 *
 *  Purpose     -  ����ر�GPS
 *
 *  Description -  arg
 *                  1   �������³�ʱ
 *                  2   AGPS��EPO������ɣ�����GPS����EPO
 *                  3   ���Ƕ�λ��ʱ
 *                  4   �������
 *                  5   Ӧ��ģʽ�л�����ر�GPS
 *                  6   ���ο�������GPS 710
 *  modification history
 * ----------------------------------------
 * v1.0  , 21-11-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_gps_off_req(void *arg)
{
    kal_uint32 value = (kal_uint32)arg;
    kal_uint8 work_mode = track_cust_get_work_mode();
    LOGD(L_GPS, L_V5, "%d,%d,mode:%d", value, G_parameter.sensor_gps_work, work_mode);
    if(track_is_testmode())
    {
        return;
    }
#if defined(__GT300S__)
    /* ͬʱ��������������GPS���ܹرգ�
    1. SENDS ����Ϊ 0��
    2. �ر�ģʽ����AGPS����
    3. ����ģʽ���£�������GT300/GT500�Ȱ���ģʽ�л�����Ŀ���� */
    LOGD(L_GPS, L_V5, "[1] %d,%d", value, G_parameter.sensor_gps_work);
    if(value != 2 && G_parameter.sensor_gps_work == 0 && (work_mode == WORK_MODE_2 || work_mode == WORK_MODE_4))
    {
        return;
    }
#elif defined(__GT370__)|| defined (__GT380__)
    LOGD(L_GPS, L_V5, "[3] %d,%d", value, G_parameter.sensor_gps_work);
    if(G_parameter.sensor_gps_work == 0 && work_mode == WORK_MODE_2)
    {
        return;
    }
#else
    if(G_parameter.sensor_gps_work == 0) return;
#endif
    track_cust_gps_control(0);
}

/******************************************************************************
 *  Function    -  track_cust_gps_work_req
 *
 *  Purpose     -  Ӧ������GPS����
 *
 *  Description -  arg
 *                  1    ��������
 *                  2    ͨ�翪���״�����
 *                  3    �𶯻���
 *                  4    ACC�仯��GPS��Ӱ�죨����GPS��ʱ�رյĶ�ʱ����
 *                  5    DW��ָ���ѯλ������
 *                  6    λ�Ʊ�������
 *                  7    ATָ������GPS����
 *                  8    ǿ��GPS����20����
 *                  9    AGPS����
 *                  10   ����ǿ�ƿ���GPS ����5 Min
 *                  11   WHER��urlָ��gps��ʱ5����
 *                  12   �����ϱ�λ��gps��ʱ5����
 *                  13   ��ʱ����GPS���ϴ�����
 *                  14   GPSON����ʱ��
 *                  15   03Dģʽ3GPS����  gt310
 *                  16   ����2����GPS
 *                  17   ljdw����gsp
 *                  18   ���������modeָ��
 *                  19   ��������WIFI����
 *  modification history
 * ----------------------------------------
 * v1.0  , 01-11-2012,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_gps_work_req(void *arg)
{
    kal_uint32 value = (kal_uint32)arg;
    kal_uint32 timer = 0;
    kal_int32 tick = 0;
    static U8 allow_open = 0;
    LOGD(L_GPS, L_V5, "value:%d", value);
	//return;//nf2318 ���ԣ�����GPS
#if defined(__GT740__)
    if(!track_is_testmode() && G_parameter.extchip.mode != 1)
    {
        if(1 == GPS_NOT_OPEN || (G_parameter.gpswake == 0 && (value != 19)) || G_realtime_data.nw_reset_sign > 0)
        {
            LOGD(L_APP, L_V6, "���ο���������GPS��λ!!! gps_not_open %d,gpswake %d ", GPS_NOT_OPEN, G_parameter.gpswake);
            return;
        }
    }
#endif

    if(value == 1)
    {
        LOGD(L_GPS, L_V4, "�������£�����GPS�����ϴ���λ��");
		track_cust_halfsleep_position(2,NULL);
    }
    else if(value == 13)
    {
        LOGD(L_GPS, L_V4, "��ʱ����GPS�����ϴ���λ��");
#if defined(__GT300S__)
        //     tr_start_timer(TRACK_CUST_SEND_LBS_TIMER_ID, 2 * 60 * 1000, track_cust_send_lbs);
#endif /* __GT300S__ */
    }
    else
    {
        net_upload_position = 1;
    }
    if(value == 1)
    {
        track_start_timer(TRACK_CUST_GPS_UPDATE_EPHEMERIS_OVERTIME_TIMER_ID, WHERE_GPS_WORK_TIME, gps_location_timeout_ind, (void*)11);
    }
    else if(value == 2)//NF2318��
    {
        track_start_timer(TRACK_CUST_POWERON_CLOSE_GPS_TIMER, WHERE_GPS_WORK_TIME, gps_location_timeout_ind, (void*)12);
    }
    else if(value == 3)
    {
        track_start_timer(TRACK_CUST_GPS_LOCATION_WORKTIME_TIMER_ID, G_parameter.sensor_gps_work * 60000, gps_location_timeout_ind, (void *)7);
    }
    else if(value == 5)
    {
        track_start_timer(TRACK_CUST_DW_GPS_WORK_TIMER, DW_CMD_GPS_WORK_TIME * 1000 , gps_location_timeout_ind, (void*)15);
    }
    else if(value == 10)
    {
        track_start_timer(TRACK_CUST_ALARM_GPS_WORK_TIMER, 5 * 60 * 1000, gps_location_timeout_ind, (void*)20);
    }
    else if(value == 11)
    {
        track_start_timer(TRACK_CUST_WHERE_GPS_WORK_TIMER_ID, WHERE_GPS_WORK_TIME, gps_location_timeout_ind, (void*)21);
    }
    else if(value == 13)//NF2318��
    {
        track_start_timer(TRACK_CUST_WORK_MODE_GPSUPDATE_OVERTIME_TIMER_ID, 3 * 60 * 1000, track_cust_gps_send_data, (void *)0);
    }
    else if(value == 14)
    {
        timer = track_cust_gpson_timer(999);
        track_start_timer(TRACK_CUST_WORK_MODE_GPSON_UPDATE_TIMER_ID, timer * 60 * 1000, gps_location_timeout_ind, (void *)0);
    }
    else if(value == 15)
    {
        track_start_timer(TRACK_CUST_WORK_MODE3__GPSON_TIMER_ID, 2 * 1000 * 60, gps_location_timeout_ind, (void *)25);
    }
    else if(value == 16)
    {
        track_start_timer(TRACK_CUST_FT_WORK_TIMER_ID, track_get_ft_work_timer() * 1000, gps_location_timeout_ind, (void *)0);
    }
    else if(value == 17)
    {
        track_start_timer(TRACK_CUST_GPS_WORK_DJDW_TIMER_ID, 2 * 60 * 1000, gps_location_timeout_ind, (void *)27);
    }
    else if(value == 18 || (value == 19))
    {
        track_start_timer(TRACK_CUST_POWERON_CLOSE_GPS_TIMER, EXTCHANGE_MODE_GPS_WORK_TIME, gps_location_timeout_ind, (void *)29);
    }
    else
    {
        track_start_timer(TRACK_CUST_GPS_LOCATION_TIMEOUT_TIMER_ID, G_parameter.gps_work.location_timeout * 60000, gps_location_timeout_ind, (void*)3);
    }

    if(gps_status == 0)
    {
        LOGD(L_GPS, L_V4, "%d����GPS", value);
        track_cust_gps_control(1);
#if 0//defined(__GT300S__)
        if(((track_cust_get_work_mode() == WORK_MODE_2) || (track_cust_get_work_mode() == WORK_MODE_4)) && net_upload_position == 1)
        {
            tr_start_timer(TRACK_CUST_LBS_UPDATE_L_TIMER_ID, 5 * 1000 * 60, track_cust_update_LBS);
        }
//#elif defined(__GT370__)|| defined (__GT380__)
        if(track_cust_get_work_mode() == WORK_MODE_2)
        {
            tr_start_timer(TRACK_CUST_LBS_UPDATE_L_TIMER_ID, 2 * 1000 * 60, track_cust_update_LBS);
        }
//#elif defined(__GT740__)
        if(net_upload_position == 1)
        {
            tr_start_timer(TRACK_CUST_LBS_UPDATE_L_TIMER_ID,  G_parameter.gps_work.Fti.lbs_interval * 1000, track_cust_update_LBS);
        }
#endif
    }

    else if(gps_status == 1)
    {
        LOGD(L_GPS, L_V4, "%d�����ӳ�GPS���ǳ�ʱʱ��", value);

#if defined(__GT370__)|| defined (__GT380__)
        if(value == 13)return;
#else
        track_stop_timer(TRACK_CUST_GPS_LOCATION_WORKTIME_TIMER_ID);
        track_start_timer(TRACK_CUST_GPS_LOCATION_TIMEOUT_TIMER_ID, G_parameter.gps_work.location_timeout * 60000, gps_location_timeout_ind, (void*)3);
#endif
    }
    else if(gps_status >= 2)
    {
        LOGD(L_GPS, L_V4, "%d����GPS��ʱ����%d����", value, G_parameter.sensor_gps_work);

        tr_stop_timer(TRACK_CUST_GPS_LOCATION_TIMEOUT_TIMER_ID);
#if defined(__GT370__)|| defined (__GT380__)
        if(track_is_timer_run(TRACK_CUST_GPS_LOCATION_WORKTIME_TIMER_ID))
        {
            track_start_timer(TRACK_CUST_GPS_LOCATION_WORKTIME_TIMER_ID, G_parameter.sensor_gps_work * 60000, gps_location_timeout_ind, (void*)4);
        }
#else
        track_start_timer(TRACK_CUST_GPS_LOCATION_WORKTIME_TIMER_ID, G_parameter.sensor_gps_work * 60000, gps_location_timeout_ind, (void*)4);
#endif
    }
}

/******************************************************************************
 *  Function    -  track_cust_module_gps_vibrates_wake
 *
 *  Purpose     -  �񶯻���GPS
 *
 *  Description -
 *      ����: ʮ���⵽�����񶯣�����GPS
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_module_gps_vibrates_wake()
{
    static kal_uint32 tick_loop[TRACK_MAX_LOOP] = {0};
    static kal_uint8 start = 0, end = 0;
    U32 tick = OTA_kal_get_systicks();
    U32 detection_time = G_parameter.gps_work.vibrates.detection_time * KAL_TICKS_1_SEC;
    int i;
    kal_bool flag = KAL_TRUE;

#if defined(__GT370__)|| defined (__GT380__) || defined (__NF2318__) 

#else
    /*  GPS�����ڼ�������⵽һ���𶯣�GPS����ʱ��˳�� 3����*/
    if(track_cust_gps_status() >= GPS_STAUS_SCAN)
    {
        flag = KAL_FALSE;
        track_cust_gps_work_req((void *)3);
    }
#endif
#if defined(__FLY_MODE__)
    track_cust_flymode_set(31);
#endif/*__FLY_MODE__*/
    /* �ж����µ�һ�μ�¼�Ƿ�������̼�� */
    if(start != end && G_parameter.gps_work.vibrates.sampling_interval != 0)
    {
        i = end - 1;
        if(i < 0)
        {
            i = TRACK_MAX_LOOP - 1;
        }
        if(tick < tick_loop[i] + G_parameter.gps_work.vibrates.sampling_interval * KAL_TICKS_1_SEC - KAL_TICKS_100_MSEC * 2)
        {
            LOGD(L_APP, L_V7, "�������𶯼�����̼��");
            return;
        }
    }

    /* �������µ�һ�μ�¼ */
    tick_loop[end] = tick;
    end++;
    if(end >= TRACK_MAX_LOOP)
    {
        end = 0;
        if(start == 0) start++;
    }

    /* �Ƴ��Ѿ���ʱ�ļ�¼ */
    for(i = 0; i < TRACK_MAX_LOOP; i++)
    {
        if(start == end || tick_loop[start] == 0 || tick <= detection_time || tick_loop[start] >= tick - detection_time)
        {
            LOGD(L_APP, L_V9, "%d, %d, %d[%d]", tick, detection_time, start, tick_loop[start]);
            break;
        }
        else
        {
            LOGD(L_APP, L_V9, "del: %d, %d, %d[%d], end:%d", tick, detection_time, start, tick_loop[start], end);
        }
        start++;
        if(start >= TRACK_MAX_LOOP)
        {
            start = 0;
        }
    }

    /* ͳ������������¼���� */
    if(start == end)
    {
        i = 1;
    }
    else if(start < end)
    {
        i = end - start;
    }
    else
    {
        i = end + TRACK_MAX_LOOP - start;
    }
    LOGD(L_APP, L_V7, "%d/%d, detection_time:%d,%d, %d / %d", start, end, G_parameter.gps_work.vibrates.detection_time, G_parameter.gps_work.vibrates.sampling_interval, i, G_parameter.gps_work.vibrates.detection_count);
    if(i >= G_parameter.gps_work.vibrates.detection_count)
    {
        if(flag)
        {
            {
#if defined(__FLY_MODE__) && defined(__MT200__)
                //  MT200����ģʽ�У������β�����GPS
                if(!track_cust_flymode_set(189))
                {
                    track_cust_gps_work_req((void *)3);
                }
                else if(i == G_parameter.vibrates_alarm.detection_count && G_parameter.vibrates_alarm.sw && G_parameter.vibrates_alarm.alarm_delay_time > 0 && !track_is_timer_run(TRACK_CUST_MODULE_ALARM_VIBRATES_DELAY_TIMER_ID))//����ģʽ��
                {
                    LOGD(L_APP, L_V5, "��ǰ���ڷ����У��𶯱�������GPS");
                    track_cust_gps_work_req((void *)3);
                }
                else//����ģʽ��
                {
                    LOGD(L_APP, L_V5, "��ǰ���ڷ�����");
                }

#else
                track_cust_gps_work_req((void *)3);
#endif
            }
        }
		
#if defined(__NF2318__)
		track_cust_vehicle_moving(1);
#endif
#if defined(__NT55__)
		if(G_parameter.acct.sw && G_parameter.acct.type == 3)
		{
			if(track_cust_status_acc() != 1)
			{
				acc_status_change((void*)1);
			}
			track_start_timer(TRACK_CUST_GSENSOR_ACC_STATUS_TIMER_ID, 300000-20000, acc_status_change, (void*)0);//������GPS����5������ͬ�������GPS���ܼ�ʱ�رյ�����
		}
#endif
#if 0
        if(G_parameter.motor_lock.mode == 0 && track_cust_status_defences() == 2)
        {
            track_motor_lock_change_status(MOTOR_REQ);
            /*track_start_timer(
                TRACK_CUST_MODULE_ALARM_VIBRATES_SHORTEST_DELAY_TIMER_ID,
                1000,
                track_cust_module_alarm_vibrates, (void*)2);*/
        }
#endif
#if defined(__FLY_MODE__) && !defined(__MT200__)
        track_cust_flymode_set(32);
#endif/*__FLY_MODE__*/
    }
}
#if 0
/******************************************************************************
 *  Function    -  track_cust_stop_update_LBS
 *
 *  Purpose     -  �ر�lbs��Ϣ����
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 08-08-2013,  jwj  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_stop_update_LBS(void)
{
    LOGD(L_GPS, L_V5, "");

    if(track_is_timer_run(TRACK_CUST_LBS_UPDATE_L_TIMER_ID))
    {
        tr_stop_timer(TRACK_CUST_LBS_UPDATE_L_TIMER_ID);
    }
}


void track_cust_mode3_update_LBS(void * f)
{
    static kal_uint8 up_counts = 0;
    kal_uint32 flag = (kal_uint32)f;

    LOGD(L_GPS, L_V5, "flag:%d,up_counts:%d -----------", flag, up_counts);

    if(WORK_MODE_3 == track_cust_get_work_mode() && G_parameter.mode3_work_interval)
    {
        track_stop_timer(TRACK_CUST_LBS_UPDATE_L_TIMER_ID);

        if(0 == flag)
        {
            up_counts = 1;
            track_cust_mode3_uping(0);
        }
        else if(99 == flag)
        {
            up_counts = 1;
            return;
        }
        else if(up_counts++ > G_parameter.mode3_up_counts)
        {
            up_counts = 0;
            track_cust_mode3_uping(0);
            track_os_intoTaskMsgQueue2(track_soc_disconnect);
            track_cust_gprson_status(0);
            LOGD(L_GPS, L_V5, "MODE3�ϴ�����");
            return ;
        }

        track_cust_gprson_status(1);
        if(up_counts == 1)
        {
            track_soc_gprs_reconnect((void*)302);
        }
        if(track_cust_gps_status() > GPS_STAUS_2D)
        {
            track_gps_data_struct *p_gpsdata = track_cust_backup_gps_data(0, NULL);
            LOGD(L_GPS, L_V6, "��ʱ����GPS���ϴ���λ��");
            track_cust_backup_gps_data(3, p_gpsdata);
            track_cust_paket_position(p_gpsdata, track_drv_get_lbs_data(), TR_CUST_POSITION_MODE_GPSUPDATE, 0);
        }
        else
        {
            track_cust_paket_18(track_drv_get_lbs_data(), 0);
        }
        if(2 != flag)
        {
            track_start_timer(TRACK_CUST_LBS_UPDATE_L_TIMER_ID, 2000, track_cust_mode3_update_LBS, (void *)1);
        }
    }
}
#endif

/************************************************
���Ʒ���LBS��,��������ЧLBS���ݺ�ʱ��
************************************************/
void track_cust_sendlbs_limit(void)
{
    LBS_Multi_Cell_Data_Struct *lbs_strcut = track_drv_get_lbs_data();
    applib_time_struct time = {0};
    applib_time_struct *currTime = NULL;
    currTime = &time;
    if(track_cust_is_upload_UTC_time())
    {
        track_fun_get_time(currTime, KAL_TRUE, NULL);
    }
    else
    {
        track_fun_get_time(currTime, KAL_FALSE, &G_parameter.zone.zone);
    }
    LOGD(L_GPS, L_V5, "time: %d-%d-%d ", currTime->nYear, currTime->nMonth, currTime->nDay);
    if(lbs_strcut->MainCell_Info.cell_id == 0)
    {
        LOGD(L_GPS, L_V5, "cell_id is Zero, cannot upload delay 5s! ");
        tr_start_timer(TRACK_CUST_SEND_LBS_LIMIT_TIMER_ID, 5000, track_cust_sendlbs_limit);
        return;
    }
    if(currTime->nYear == (*OTA_DEFAULT_HARDWARE_YEAR + 2000) && currTime->nMonth == 1 && currTime->nDay == 1)
    {
        LOGD(L_GPS, L_V5, "current time is not Synchronous! ");
        tr_start_timer(TRACK_CUST_SEND_LBS_LIMIT_TIMER_ID, 5000, track_cust_sendlbs_limit);
        return;
    }
    track_cust_paket_18(track_drv_get_lbs_data(), 0);
}
#if 0
void track_cust_LBS_upload(void)
{
    static U32 last_t2 = 0, last_t1 = 0;
    U32 tick = OTA_kal_get_systicks(), t1, t2;
    LOGD(L_GPS, L_V5, "");
    if(G_parameter.lbson.sw == 0 || G_parameter.lbson.t1 == 0) return;

#if defined(__FLY_MODE__)
    if(track_cust_flymode_set(199))//����ģʽ��
    {
        LOGD(L_GPS, L_V4, "��ǰ���ڷ�����");
        return;
    }
#endif /* __FLY_MODE__*/

    if(track_cust_gps_status() >= GPS_STAUS_2D || track_cust_gps_status() == GPS_STAUS_OFF)
    {
        last_t2 = 0;
        track_stop_timer(TRACK_CUST_SEND_LBS_LIMIT_TIMER_ID);
        LOGD(L_GPS, L_V5, "1");
        return; /* LBS �� GPS ����λ״̬�²Ŵ� */
    }
    if(last_t2 == 0)
    {
        last_t2 = tick;
        last_t1 = 0;
        LOGD(L_GPS, L_V5, "start the timer");
        tr_start_timer(TRACK_CUST_SEND_LBS_LIMIT_TIMER_ID, G_parameter.lbson.t1 * 1000, track_cust_LBS_upload);
        return;
    }
    t2 = kal_ticks_to_secs(tick - last_t2);
    if(G_parameter.lbson.t2 > t2)
    {
        LOGD(L_GPS, L_V5, "t2:%d", t2);
        tr_start_timer(TRACK_CUST_SEND_LBS_LIMIT_TIMER_ID, (G_parameter.lbson.t2 - t2) * 1000, track_cust_LBS_upload);
        return; /* GPS����λʱ��δ����t2 */
    }
    t1 = kal_ticks_to_secs(tick - last_t1);
    if(G_parameter.lbson.t1 > t1)
    {
        LOGD(L_GPS, L_V5, "t1:%d", t1);
        tr_start_timer(TRACK_CUST_SEND_LBS_LIMIT_TIMER_ID, (G_parameter.lbson.t1 - t1) * 1000, track_cust_LBS_upload);
        return; /* GPS����λʱ��δ����t1 */
    }
    {
        static kal_uint16 last_cell_id;
        LBS_Multi_Cell_Data_Struct *lbs = track_drv_get_lbs_data();
        applib_time_struct time = {0};
        applib_time_struct *currTime = NULL;
        currTime = &time;
        if(track_cust_is_upload_UTC_time())
        {
            track_fun_get_time(currTime, KAL_TRUE, NULL);
        }
        else
        {
            track_fun_get_time(currTime, KAL_FALSE, &G_parameter.zone.zone);
        }
        LOGD(L_GPS, L_V5, "time: %d-%d-%d ", currTime->nYear, currTime->nMonth, currTime->nDay);
        if(lbs->MainCell_Info.cell_id == 0)
        {
            LOGD(L_GPS, L_V5, "Warning: cell_id is Zero, cannot upload delay 5s! ");
            tr_start_timer(TRACK_CUST_SEND_LBS_LIMIT_TIMER_ID, 5000, track_cust_LBS_upload);
            return;
        }
        if(currTime->nYear == 2015 && currTime->nMonth == 1 && currTime->nDay == 1)
        {
            LOGD(L_GPS, L_V5, "Warning: current time is not Synchronous! ");
            tr_start_timer(TRACK_CUST_SEND_LBS_LIMIT_TIMER_ID, 5000, track_cust_LBS_upload);
            return;
        }
        if(last_cell_id == lbs->MainCell_Info.cell_id || lbs->MainCell_Info.cell_id == 0)
        {
            LOGD(L_GPS, L_V5, "Warning: cell id the same with the previous! ");
            tr_start_timer(TRACK_CUST_SEND_LBS_LIMIT_TIMER_ID, 5000, track_cust_LBS_upload);
            return;
        }
        last_cell_id = lbs->MainCell_Info.cell_id;
        track_cust_paket_18(lbs, 0);
        last_t1 = tick;
        tr_start_timer(TRACK_CUST_SEND_LBS_LIMIT_TIMER_ID, G_parameter.lbson.t1 * 1000, track_cust_LBS_upload);
    }
}
/******************************************************************************
 *  Function    -  track_cust_update_LBS
 *
 *  Purpose     -  ����lbs��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 08-08-2013,  jwj  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_update_LBS(void)
{
#if defined(__SAME_GT02__)
    return;
#endif

    if(G_parameter.gps_work.Fti.lbs_interval == 0)
    {
        return;
    }
    LOGD(L_GPS, L_V5, "gps status:%d", track_cust_gps_status());

    //��ľ�������
    if(!track_is_timer_run(TRACK_CUST_LBS_UPDATE_L_TIMER_ID))
    {
        tr_start_timer(TRACK_CUST_LBS_UPDATE_L_TIMER_ID, G_parameter.gps_work.Fti.lbs_interval * 1000, track_cust_update_LBS);
    }

    {
    LBS_Multi_Cell_Data_Struct *lbs = track_drv_get_lbs_data();
    {
        static kal_uint16 last_cell_id;

        if(last_cell_id == lbs->MainCell_Info.cell_id || lbs->MainCell_Info.cell_id == 0)
        {
            return;
        }
        last_cell_id = lbs->MainCell_Info.cell_id;
    }

    track_cust_paket_18(lbs, 0);

    }
}
#endif

/******************************************************************************
 *  Function    -  track_gps_locate_data_receive
 *
 *  Purpose     -  ����GPS  ��������
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 03-08-2012,  chengjun  written
 *
 *  �ṩȫ�����ݰ��������ɴ�ʵ��ʹ��
 * ----------------------------------------
 ******************************************************************************/
void track_gps_locate_data_receive(track_gps_data_struct * gps_decode_data)
{
	static U8 FIX_COUNTS = 0;
    static kal_uint8 flag = 0;
    if(gps_decode_data == NULL || gps_status == 0)
    {
        LOGD(L_GPS, L_V1, "gps_status=%d, error!", gps_status);
        return;
    }
    g_restart_gps_count = 0;

    tr_start_timer(TRACK_CUST_PERIODIC_RESTART_GPS_TIMER_ID, 20000, track_cust_gps_nodata_restart);

    /*2D-3D��λ*/
    if(gps_decode_data->status == 2 || gps_decode_data->status == 3)
    {
        static kal_uint32 gps_just_location_tick = 0; // ��¼�ɲ���λת��λ��ʱ��ֵ
        if(!track_cust_is_upload_UTC_time())
        {
            applib_time_struct time = {0};
            /* GPS�����е�ʱ�����ʱ��У�� */
            track_drv_utc_timezone_to_rtc(&time, (applib_time_struct*)&gps_decode_data->gprmc.Date_Time, G_parameter.zone);
            memcpy(&gps_decode_data->gprmc.Date_Time, &time, sizeof(applib_time_struct));
        }
        LOGD(L_GPS, L_V7, "time(%d%c): %0.2d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d",
             G_parameter.zone.time, G_parameter.zone.zone,
             gps_decode_data->gprmc.Date_Time.nYear,
             gps_decode_data->gprmc.Date_Time.nMonth,
             gps_decode_data->gprmc.Date_Time.nDay,
             gps_decode_data->gprmc.Date_Time.nHour,
             gps_decode_data->gprmc.Date_Time.nMin,
             gps_decode_data->gprmc.Date_Time.nSec);

        /* ���ݳɹ���λ������һ������ */
        track_cust_backup_gps_data(1, gps_decode_data);
        positioned = KAL_TRUE;

        /* ��ַ��ѯ���� */
        if(track_is_timer_run(TRACK_CUST_GET_ADDRESS_OVER2_TIMER_ID))
        {
            track_stop_timer(TRACK_CUST_GET_ADDRESS_OVER2_TIMER_ID);
            track_os_intoTaskMsgQueueExt(track_cust_module_get_addr, NULL);
        }
#if 0//defined(__NF2318__)//�����ﲻ���ʣ��õ�����֮��
		if(track_is_timer_run(TRACK_CUST_PAGING_OVERTIME_TIMER_ID))
		{
			tr_stop_timer(TRACK_CUST_PAGING_OVERTIME_TIMER_ID);
			track_cust_paging_overtime((void*)1);
		}
#endif
#if defined(__TRACK_CAR_DEVICE__) && !defined(__NF2318__)
        /* ���͵� */
        track_cust_module_oil_cut_gps(gps_decode_data);
#endif
        //�״ν���2D/3D��λ

        if ((gps_status != GPS_STAUS_2D && gps_decode_data->status == 2)\
            || (gps_status != GPS_STAUS_3D && gps_decode_data->status == 3))
        {
            gps_status = gps_decode_data->status+1;

            if(!flag)
            {
                flag = 1;
            }

            track_project_gps_work_and_fix_nitify(gps_decode_data);

            LED_GPS_StatusSet(GPS_LOCATION_STATE);

            //track_cust_module_alarm_blind();
            if(track_is_timer_run(TRACK_CUST_DWCHANGE_INSTANT_CONTRACT_TIMER_ID))
            {
                track_stop_timer(TRACK_CUST_DWCHANGE_INSTANT_CONTRACT_TIMER_ID);
            }
            LOGD(L_GPS, L_V4, "GPS��λ!");

            //track_cust_LBS_upload();
        }
#if defined(__NF2318__)
		if(G_realtime_data.sleep == 15 && track_is_timer_run(TRACK_CUST_GPS_UPDATE_EPHEMERIS_OVERTIME_TIMER_ID))
		{
			if(track_cust_halfsleep_position(99,gps_decode_data) < 20)
			{
				track_cust_halfsleep_position(1,gps_decode_data);
			}
			return;
		}
#else
        if(track_is_timer_run(TRACK_CUST_WORK_MODE_GPSUPDATE_OVERTIME_TIMER_ID) == KAL_TRUE)
        {
            track_cust_gps_send_data(2);
            if(!net_upload_position)
            {
                return;
            }
        }
#endif

        if(!track_is_timer_run(TRACK_CUST_PAGING_OVERTIME_TIMER_ID)&&(!net_upload_position) && (track_is_timer_run(TRACK_CUST_WORK_MODE_GPSUPDATE_OVERTIME_TIMER_ID) == KAL_FALSE))
        {
            LOGD(L_GPS, L_V6, "����Χ�������٣����ϴ���λ�㡣");
            if(track_is_timer_run(TRACK_CUST_GPS_UPDATE_EPHEMERIS_OVERTIME_TIMER_ID) == KAL_TRUE)
            {
                track_stop_timer(TRACK_CUST_GPS_UPDATE_EPHEMERIS_OVERTIME_TIMER_ID);
                tr_stop_timer(TRACK_CUST_GPS_LOCATION_TIMEOUT_TIMER_ID);
                tr_stop_timer(TRACK_CUST_GPS_LOCATION_WORKTIME_TIMER_ID);
                track_cust_gps_location_timeout(10);
            }
            return; // ���������������´�����Ư��Ӱ��
        }



        if(flag == 1)
        {
            flag = 2;
            gps_just_location_tick = kal_get_systicks();
            if(G_realtime_data.netLogControl & 8)
            {
                cust_packet_log(8, 2, 0, 0, 0, 0, 0);
            }
        }

        /* ��λ��ľ�γ���ϴ� */
        track_cust_net_upload_position(gps_decode_data, gps_just_location_tick);
#if defined(__NF2318__)
		//LOGD(L_GPS, L_V4,"--------------------->>>>%d  timer:%d",FIX_COUNTS,track_is_timer_run(TRACK_CUST_PAGING_OVERTIME_TIMER_ID));
		if(track_is_timer_run(TRACK_CUST_PAGING_OVERTIME_TIMER_ID))
		{
			LOGD(L_GPS, L_V4, "%d,%d,%d,%d,%d",FIX_COUNTS,gps_decode_data->filter,gps_decode_data->gprmc.Speed,gps_decode_data->gpgga.sat,gps_decode_data->status);
			if(FIX_COUNTS >= 20 && gps_decode_data->status > 2)
			{
				//track_cust_paging_overtime((void*)1);
				FIX_COUNTS = 0;//�ڰ�����ģʽ���п����ڲ���Χ���Ƕ��ͷ����ˣ������ֻ��һ�Σ�û��20�ι��ˣ�Ū���ٵģ���װ����20��
				ALARM_get_position(gps_decode_data);
			}
			else
			{
				if(FIX_COUNTS >= 254) FIX_COUNTS = 0;
				FIX_COUNTS ++;
			}
		}
		else
		{
			FIX_COUNTS = 0;
		}
#endif

        if(track_is_timer_run(TRACK_CUST_GPS_COLD_START_TIMER_ID))
        {
            track_cust_gps_check_nofix(0);
        }
    }
    else
    {
        flag = 0;
        if(gps_status != GPS_STAUS_SCAN)
        {
            gps_status = GPS_STAUS_SCAN;
            track_cust_set_anglerep_upload(0);// ��λ������λ���ϴ��ս�

            LED_GPS_StatusSet(GPS_SCAN_STATE);

            //track_cust_module_alarm_blind();

            track_project_gps_work_no_fix_nitify();

            LOGD(L_GPS, L_V4, "GPS δ��λ!");
            if(G_realtime_data.netLogControl & 8)
            {
                cust_packet_log(8, 3, 0, 0, 0, 0, 0);
            }
            track_start_timer(TRACK_CUST_DWCHANGE_INSTANT_CONTRACT_TIMER_ID, 30 * 1000, track_cust_instant_contract_position_data, (void *) 1);
            track_cust_gps_check_nofix(1);//����gps���������
            //track_cust_LBS_upload();
#if defined(__AMS_HEX_LOG__)
            {
                kal_uint8 data[8] = {0};
                data[0] = GPS_STAUS_SCAN;
                track_os_file_log(1, 0x05, data);
            }
#endif

        }
#if defined(__NF2318__)
		if(G_realtime_data.sleep == 15)
		{
			track_cust_halfsleep_position(2,NULL);
		}
#endif
    }
}

track_enum_gps_staus track_cust_gps_before_off_status(void)
{
    return gps_before_off_status;
}


/******************************************************************************
 *  Function    -  track_cust_gps_status
 *
 *  Purpose     -  GPS״̬
 *
 *  return      -  0: GPS�ر�
 *                 1: GPS����
 *                 2: ��  ��
 *                 3: 2D��λ
 *                 4: 3D��λ
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 30-10-2012,  Cml  written
 * ----------------------------------------
*******************************************************************************/
track_enum_gps_staus track_cust_gps_status(void)
{
    return gps_status;
}

/******************************************************************************
 *  Function    -  track_cust_gps_positioned_ever_after_booting
 *
 *  Purpose     -  ���ؿ���֮���Ƿ�������λ��
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 17-01-2014,  Cml  written
 * ----------------------------------------
*******************************************************************************/
kal_bool track_cust_gps_positioned_ever_after_booting(void)
{
    return positioned;
}

/******************************************************************************
 *  Function    -  track_cust_gps_cmd_change_set
 *
 *  Purpose     -  ��������ָ�����ʱ����ʱ����޸�
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_gps_cmd_change_set(void)
{
    if(G_parameter.sensor_gps_work == 0 && gps_status == 0)
    {
        track_cust_gps_work_req((void*)7);
    }
    if(G_parameter.sensor_gps_work > 0)
    {
        if(!track_is_timer_run(TRACK_CUST_GPS_LOCATION_WORKTIME_TIMER_ID))
        {
            track_start_timer(TRACK_CUST_GPS_LOCATION_WORKTIME_TIMER_ID, G_parameter.sensor_gps_work * 60000, gps_location_timeout_ind, (void*)6);
        }
    }
}

/******************************************************************************
 *  Function    -  track_cust_poweron_close_gps
 *
 *  Purpose     -  ������һ���ϴ�GPS��λ���ر�GPS
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 07-08-2013,  ZengPing && Liujw  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_poweron_close_gps(void)
{
    kal_uint16 sn;
    track_gps_data_struct *p_gpsdata = track_cust_gpsdata_alarm();
    track_cust_backup_gps_data(3, p_gpsdata);
#if defined (__GT300S__) || defined(__GT370__)|| defined (__GT380__)
    if(track_cust_get_work_mode() == WORK_MODE_3)
    {
        return;
    }
#endif

#if !defined(__ET310__)
    {
        sn = track_cust_paket_position(p_gpsdata, track_drv_get_lbs_data(), TR_CUST_POSITION_POWER_ON, 0);
    }
#endif /* __ET310__ */
    LOGD(L_GPS, L_V5, "������ر�GPS %d", track_cust_get_work_mode());
    if(track_is_timer_run(TRACK_CUST_POWERON_CLOSE_GPS_TIMER))
    {
        track_stop_timer(TRACK_CUST_POWERON_CLOSE_GPS_TIMER);
    }
    if(track_is_timer_run(TRACK_CUST_GPS_LOCATION_TIMEOUT_TIMER_ID))
    {
        track_stop_timer(TRACK_CUST_GPS_LOCATION_TIMEOUT_TIMER_ID);
    }
    gps_location_timeout_ind((void*)16);

    if(track_cust_get_work_mode() == WORK_MODE_1)
    {
        //track_start_timer(TRACK_CUST_WORK_MODE_GPSUPDATE_TIMER_ID, G_parameter.gps_work.Fti.mode1_gps_time * 60 * 1000, track_cust_gps_work_req, (void *)13);
    }
}

//��ʱ��λ����ģʽ���ϴ���

/******************************************************************************
 *  Function    -  track_cust_gps_send_data
 *
 *  Purpose     -  ��ʱ��λ����ģʽ���ϴ���
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 17-10-2013,  ZengPing && Liujw  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_gps_send_data(kal_uint32 status)
{
    static kal_uint32 flag = 0;
    kal_uint16 sn;

    if(flag == 2 && status == 2)
    {
        return ;//�����ظ�����10��ر�GPS��ʱ��
    }
    else
    {
        flag = status;
    }
    LOGD(L_GPS, L_V6, " flag =%d", flag);
    if(flag == 0)
    {
        LOGD(L_GPS, L_V6, "��ʱ����GPS�����ǳ�ʱ������LBS��");
        //track_cust_update_LBS();
        gps_location_timeout_ind((void*)17);
    }
    else if(flag == 1)
    {

        track_gps_data_struct *p_gpsdata = track_cust_backup_gps_data(0, NULL);
        LOGD(L_GPS, L_V6, "��ʱ����GPS��%d����ʱ�����ϴ���λ��", UP_TIME);
        track_cust_backup_gps_data(3, p_gpsdata);
        gps_location_timeout_ind((void*)18);
    }
    else if(flag == 2)
    {
        track_start_timer(TRACK_CUST_WORK_MODE_GPSUPDATE_OVERTIME_TIMER_ID, UP_TIME * 1000, track_cust_gps_send_data, (void *) 1);
        LOGD(L_GPS, L_V6, "��ʱ����GPS���Ѿ���λ��%d��󴫵�", UP_TIME);
    }
    else
    {
        LOGD(L_GPS, L_V6, " Error  flag =%d", flag);
    }

    if(flag == 0 || flag == 1)
    {

        //track_start_timer(TRACK_CUST_WORK_MODE_GPSUPDATE_TIMER_ID, G_parameter.gps_work.Fti.mode1_gps_time * 60 * 1000, track_cust_gps_work_req, (void *)13);
    }
}

/******************************************************************************
 *  Function    -  track_cust_gps_cold_start
 *
 *  Purpose     -  GPS������
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 31-10-2013,  ZengPing && Liujw  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_gps_cold_start(void)
{
    LOGD(L_GPS, L_V6, "ִ��������");
#if defined(__GPS_UART__)||defined(__GPS_MT3333__)
    track_cust_ubxcold();
    track_gps_data_clean();
#endif
    track_cust_gps_check_nofix(2);
}

void track_cust_gps_check_nofix(kal_uint8 flag)
{
    static kal_uint8 state = 0;

    LOGD(L_GPS, L_V6, "flag:%d,state:%d", flag, state);
    if(flag == 1 && state == 0)
    {
        state = 1;
        tr_start_timer(TRACK_CUST_GPS_COLD_START_TIMER_ID, 10 * 60 * 1000, track_cust_gps_cold_start);
    }
    else if(flag == 0 && state == 1)
    {
        state = 0;
        if(track_is_timer_run(TRACK_CUST_GPS_COLD_START_TIMER_ID))
        {
            tr_stop_timer(TRACK_CUST_GPS_COLD_START_TIMER_ID);
        }
    }
    else if(flag == 2)
    {
        state = 2;
    }
}

kal_uint32 track_cust_gpson_timer(kal_uint32 time)
{
    static kal_uint32 gpson_timer = 5;
    if(time == 0)
    {
        gpson_timer = 5;
    }
    else if(time == 999)
    {
        LOGD(L_GPS, L_V6, "gpson_timer = %d", gpson_timer);
        return gpson_timer;
    }
    else
    {
        gpson_timer = time;
    }
    LOGD(L_GPS, L_V6, "time = %d;gpson_timer = %d", time, gpson_timer);
}
#if !defined(__NF2318__)
kal_bool track_cust_time_slot(void)
{
    int i;
    kal_uint8 DayIndex;
    applib_time_struct currTime = {0};
    if(track_cust_is_upload_UTC_time())
    {
        track_fun_get_time(&currTime, KAL_TRUE, NULL);
    }
    else
    {
        track_fun_get_time(&currTime, KAL_FALSE, &G_parameter.zone.zone);
    }
    DayIndex = currTime.DayIndex;
    if(DayIndex == 0) DayIndex = 2;
    else if(DayIndex == 6) DayIndex = 1;
    else DayIndex = 0;

    for(i = 0; i < 8; i++)
    {
        if(G_realtime_data.timeslot[DayIndex][i][0] == 1
                && currTime.nHour >= G_realtime_data.timeslot[DayIndex][i][1]
                && currTime.nMin >= G_realtime_data.timeslot[DayIndex][i][2]
                && currTime.nHour <= G_realtime_data.timeslot[DayIndex][i][3]
                && currTime.nMin <= G_realtime_data.timeslot[DayIndex][i][4])
        {
            return KAL_TRUE;
        }
    }
    return KAL_FALSE;
}
#endif

void track_cust_intermittent_send_LBS(void * arg)
{
    static U32 shock_count = 0;
    static kal_bool gps = KAL_FALSE;
    U32 par = (U32)arg;
    LOGD(L_GPS, L_V7, "");
    if(G_parameter.gps_work.Fti.lbs_interval == 0)
        return;
    if(par == 1) // ��⵽��
    {
        shock_count++;
        return;
    }
    else if(par == 2) // GPS ��λ
    {
        gps = KAL_TRUE;
        return;
    }
    track_start_timer(
        TRACK_CUST_LBS_UPDATE_L_TIMER_ID,
        G_parameter.gps_work.Fti.lbs_interval * 60000,
        track_cust_intermittent_send_LBS,
        (void*)0);

    if(par == 3) return;

    if(shock_count > G_parameter.gps_work.Fti.lbs_interval * 10
            && !gps && gps_status < 3)
    {
        LBS_Multi_Cell_Data_Struct *lbs = track_drv_get_lbs_data();
        /*static kal_uint16 last_cell_id;
        if(last_cell_id == lbs->MainCell_Info.cell_id || lbs->MainCell_Info.cell_id == 0)
        {
            LOGD(L_GPS, L_V5, "LBS ����һ����ͬ");
            return;
        }
        last_cell_id = lbs->MainCell_Info.cell_id;*/
        track_cust_paket_18(lbs, 0);
    }
    else
    {
        LOGD(L_GPS, L_V5, "LBS�̶�����ϴ�: ��=%d/%d, GPS��������λ:%d, GPS��ǰ��λ״̬:%d",
             shock_count, G_parameter.gps_work.Fti.lbs_interval * 10,
             gps, gps_status);
        gps = KAL_FALSE;
        shock_count = 0;
    }
}

/******************************************************************************
 *  Function    -  track_cust_instant_contract_position_data
 *
 *  Purpose     -  ��ʱ�������Ѵ���Ķ�λ����
 *
 *  Description -flag=1,��λ������λ��������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 02-12-2015,  Cjj written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_instant_contract_position_data(kal_uint8 flag)
{
    if(!G_parameter.group_package.sw)
    {
        return;
    }
    LOGD(L_GPS, L_V4, "flag:%d", flag);

    if(flag == 1) //GPS��������ʱ����������֤Ч��
    {
        //�ɶ�λ������λ�������ϴ��������ݰ�
        LOGD(L_GPS, L_V4, " ��λת����λ����");
        track_soc_instant_upload_position_status();
        track_soc_send((void*)9);
        return;
    }
    else if(flag == 2)
    {
        //  GPS��������ǰ�������ϴ�������еĶ�λ���ݣ�
        LOGD(L_GPS, L_V5, " !!!!!!!!!!���߷���!!!!!!!!!!!!!!!");
        track_soc_instant_upload_position_status();
        track_soc_send((void*)9);
    }
    else if(flag == 3)
    {
        //���ⲻ������������Ϩ���һֱ��������λ���ݵ����⡣
        LOGD(L_GPS, L_V5, "!!!!!!!!!Ϩ��ǿ�Ʒ���λ��!!!!!!!");
        track_soc_instant_upload_position_status();
        track_soc_send((void*)9);
    }
    else if(flag == 99)
    {
        //��ʱ����������ٵ�ACC�仯���������ݰ��ϴ���
        track_start_timer(TRACK_CUST_ACCCHANGE_INSTANT_CONTRACT_TIMER_ID, 10 * 1000, track_cust_instant_contract_position_data, (void *) 3);
    }

}
#if defined(__WIFI__)
/******************************************************************************
 *  Function    -  track_cust_sensor_to_rest_supp_wifihot
 *
 *  Purpose     -  ��ֹʱ����λ�ϴ�һ��WIFI��Ϣ��
 *
 *  Description -  GPS�ر�ʱҲ��Ϊ�Ǿ�ֹ
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 21-02-2016,  Xzq  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_sensor_to_rest_supp_wifihot()
{
    LOGD(L_APP, L_V5, "gps_status %d, gps_before_off_status %d", gps_status, gps_before_off_status);
    if(G_parameter.wifi.sw)
    {
        if(gps_status == GPS_STAUS_OFF && gps_before_off_status < GPS_STAUS_2D)
        {
            track_stop_timer(TRACK_CUST_SERSOR_TO_REST_TIMER_ID);
            track_cust_sendWIFI_paket();
        }
        else if(gps_status != GPS_STAUS_OFF)
        {
            tr_start_timer(TRACK_CUST_SERSOR_TO_REST_TIMER_ID, 30 * 1000, track_cust_sensor_to_rest_supp_wifihot);
        }
    }
}
#endif

#if defined (__NOFIX_GPSDUP__)
void track_gps_no_location_dup_updata(void)
{
    kal_uint16 interval_timer;
    track_gps_data_struct last_gps_data = {0};
    static track_gps_data_struct last_tmp_gps_data = {0};

    track_gps_data_struct *gpsLastPoint = NULL;
    applib_time_struct currTime = {0};

    if(G_parameter.gpsDup == 0)
    {
        return;
    }

#if defined (__NT33__)
    interval_timer = G_parameter.gps_work.Fti.lbs_interval;
#else
    if(track_cust_status_acc())
    {
        interval_timer = G_parameter.gps_work.Fti.interval_acc_on;
    }
    else
    {
        interval_timer = G_parameter.gps_work.Fti.interval_acc_off;
    }
#endif
    if(interval_timer == 0)
    {
        return;
    }
    if(!track_soc_login_status() || track_cust_gps_status() >= GPS_STAUS_2D)
    {
        tr_start_timer(TRACK_CUST_DETECT_NO_VIB_TIMER_ID, interval_timer * 1000, track_gps_no_location_dup_updata);
        return;
    }

    gpsLastPoint = track_cust_backup_gps_data(2, NULL);
    memcpy(&last_gps_data, gpsLastPoint, sizeof(track_gps_data_struct));

    last_gps_data.gprmc.Speed = 0;
    last_gps_data.gprmc.status = 0;

    track_fun_get_time(&currTime, KAL_TRUE, NULL);
    LOGD(L_APP, L_V5, "%0.4d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d", currTime.nYear, currTime.nMonth, currTime.nDay, \
         currTime.nHour, currTime.nMin, currTime.nSec);

    last_gps_data.gprmc.Date_Time.nYear = currTime.nYear;
    last_gps_data.gprmc.Date_Time.nMonth = currTime.nMonth;
    last_gps_data.gprmc.Date_Time.nDay = currTime.nDay;
    last_gps_data.gprmc.Date_Time.nHour = currTime.nHour;
    last_gps_data.gprmc.Date_Time.nMin = currTime.nMin;
    last_gps_data.gprmc.Date_Time.nSec = currTime.nSec;
    tr_start_timer(TRACK_CUST_DETECT_NO_VIB_TIMER_ID, interval_timer * 1000, track_gps_no_location_dup_updata);
    track_cust_paket_position(&last_gps_data, track_drv_get_lbs_data(), TR_CUST_POSITION_GPSDUP, 0);
}


/******************************************************************************
 *  Function    -  track_gps_location_dup_updata
 *
 *  Purpose     -  GPS��λ����������Ӧ�����˵ģ�����
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-02-07,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_gps_location_dup_updata(track_gps_data_struct * gpsdata)
{
    track_gps_data_struct last_gps_data = {0};
    static  track_gps_data_struct last_tmp_gps_data = {0};
    track_gps_data_struct *gpsLastPoint = NULL;
    applib_time_struct currTime = {0};

    if(G_parameter.gpsDup == 0)
    {
        return;
    }

    if(G_parameter.gps_work.static_filter_sw)
    {
        gpsLastPoint = track_cust_backup_gps_data(2, NULL);
        memcpy(&last_gps_data, gpsLastPoint, sizeof(track_gps_data_struct));
        track_fun_get_time(&currTime, KAL_TRUE, NULL);
        LOGD(L_APP, L_V5, "%0.4d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d", currTime.nYear, currTime.nMonth, currTime.nDay, \
             currTime.nHour, currTime.nMin, currTime.nSec);
        last_gps_data.gprmc.Date_Time.nYear = currTime.nYear;
        last_gps_data.gprmc.Date_Time.nMonth = currTime.nMonth;
        last_gps_data.gprmc.Date_Time.nDay = currTime.nDay;
        last_gps_data.gprmc.Date_Time.nHour = currTime.nHour;
        last_gps_data.gprmc.Date_Time.nMin = currTime.nMin;
        last_gps_data.gprmc.Date_Time.nSec = currTime.nSec;
        if(memcmp(&last_tmp_gps_data, &last_gps_data, sizeof(track_gps_data_struct)) != 0)
        {
            memcpy(&last_tmp_gps_data, &last_gps_data, sizeof(track_gps_data_struct));
        }
        else
        {
            last_gps_data.gprmc.Speed = 0;
        }
        track_cust_paket_position(&last_gps_data, track_drv_get_lbs_data(), TR_CUST_POSITION_GPSDUP, 0);
    }
    else
    {
        track_cust_paket_position(gpsdata, track_drv_get_lbs_data(), TR_CUST_POSITION_GPSDUP, 0);
    }
}

void track_cust_gpsdup_switch(kal_uint8 gpsdup)
{
    if(gpsdup == 0)
    {
        if(track_is_timer_run(TRACK_CUST_DETECT_NO_VIB_TIMER_ID))
        {
            tr_stop_timer(TRACK_CUST_DETECT_NO_VIB_TIMER_ID);
        }
    }
    else
    {
        if(!track_is_timer_run(TRACK_CUST_DETECT_NO_VIB_TIMER_ID))
        {
            track_gps_no_location_dup_updata();
        }
    }
}

void track_cust_gpsdup_interval_update(void)
{
    if(G_parameter.gpsDup == 0)
    {
        return;
    }
    if(track_is_timer_run(TRACK_CUST_DETECT_NO_VIB_TIMER_ID))
    {
        tr_stop_timer(TRACK_CUST_DETECT_NO_VIB_TIMER_ID);
    }
    track_gps_no_location_dup_updata();
}
#endif

