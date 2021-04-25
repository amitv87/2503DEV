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

/*****************************************************************************
*  Typedef  Enum                        ö�ٶ���
*****************************************************************************/

/*****************************************************************************
*  Struct                               ���ݽṹ����
******************************************************************************/

/*****************************************************************************
*  Local variable                       �ֲ�����
*****************************************************************************/
#if defined(__GERMANY__)
static char *str_battery_low_en = "Achtung - interne Batterie ist niedrig, bitte Laden !";
#elif defined(__SPANISH__)
static char *str_battery_low_en = "Alerta! Nivel de bateria bajo. Por favor cargarla.";
#else
#if defined(__MT200__)
static char *str_battery_low_en = "Attention!The built-in battery is low!";
#else
static char *str_battery_low_en = "Attention! The built-in battery is low, please charge!";
#endif/*__MT200__*/
#endif
#if defined(__MT200__)
static char str_battery_low_cn[23] =   // ע�⣡�ն˵�ص�����!
{
    0x6C, 0xE8, 0x61, 0x0F, 0xFF, 0x01, 0x7E, 0xC8, 0x7A, 0xEF, 0x75, 0x35, 0x6C, 0x60, 0x75, 0x35,
    0x91, 0xCF, 0x4F, 0x4E, 0xFF, 0x01
};
#else
static char str_battery_low_cn[34] =   // ע�⣡�ն˵�ص����ͣ��뼰ʱ��硣
{
    0x6C, 0xE8, 0x61, 0x0F, 0xFF, 0x01, 0x7E, 0xC8, 0x7A, 0xEF, 0x75, 0x35, 0x6C, 0x60, 0x75, 0x35,
    0x91, 0xCF, 0x4F, 0x4E, 0xFF, 0x0C, 0x8B, 0xF7, 0x53, 0xCA, 0x65, 0xF6, 0x51, 0x45, 0x75, 0x35,
    0x30, 0x02
};
#endif
#if !defined(__GT740__)
static char *str_battery_low_will_turn_off_en = "Attention! The battery is low, the device shutdown!";
static char str_battery_low_will_turn_off_cn[34] =   // ע�⣡�ն˵�ص������ͣ��Զ��ػ���
{
    0x6C, 0xE8, 0x61, 0x0F, 0xFF, 0x01, 0x7E, 0xC8, 0x7A, 0xEF, 0x75, 0x35, 0x6C, 0x60, 0x75, 0x35,
    0x91, 0xCF, 0x8F, 0xC7, 0x4F, 0x4E, 0xFF, 0x0C, 0x81, 0xEA, 0x52, 0xA8, 0x51, 0x73, 0x67, 0x3A,
    0xFF, 0x01
};

static char *str_external_power_low_en = "Attention! Vehicle's battery power is low. Please charge!";
static char str_external_power_low_cn[32] =   // ע�⣡������ƿ�͵磬�뼰ʱ��磡
{
    0x6C, 0xE8, 0x61, 0x0F, 0xFF, 0x01, 0x8F, 0x66, 0x8F, 0x86, 0x75, 0x35, 0x74, 0xF6, 0x4F, 0x4E,
    0x75, 0x35, 0xFF, 0x0C, 0x8B, 0xF7, 0x53, 0xCA, 0x65, 0xF6, 0x51, 0x45, 0x75, 0x35, 0xFF, 0x01
};

static char *str_disconnect_the_external_power_en = "Vehicle's battery power is low, the low battery protection has been activated, please charge!";
static char str_disconnect_the_external_power_cn[42] =   // ע�⣡������ƿ�͵籣���Ѷ������뼰ʱ��磡
{
    0x6C, 0xE8, 0x61, 0x0F, 0xFF, 0x01, 0x8F, 0x66, 0x8F, 0x86, 0x75, 0x35, 0x74, 0xF6, 0x4F, 0x4E,
    0x75, 0x35, 0x4F, 0xDD, 0x62, 0xA4, 0x5D, 0xF2, 0x52, 0xA8, 0x4F, 0x5C, 0xFF, 0x0C, 0x8B, 0xF7,
    0x53, 0xCA, 0x65, 0xF6, 0x51, 0x45, 0x75, 0x35, 0xFF, 0x01
};
#endif
U32 tick_for_disconnect_ext_power = 0;
#if defined(__MT200__) && defined(__FLY_MODE__)
static kal_uint8 GPIO1_control = 0; //��¼GPIO�ı仯
#endif
/****************************************************************************
*  Global Variable                      ȫ�ֱ���
*****************************************************************************/
track_enum_sms_staus sms_staus = SMS_NORMAL_MODE; //��¼SMS��״̬������ģʽor ����ģʽ

/****************************************************************************
*  Global Variable - Extern             ����ȫ�ֱ���
*****************************************************************************/

/*****************************************************************************
*  Global Functions - Extern            �����ⲿ����
******************************************************************************/
extern void track_instructions_get_paging(void);
/*****************************************************************************
*  Local Functions Define               ���غ�������
******************************************************************************/
#if defined(__MT200__) && defined(__FLY_MODE__)
extern void track_cust_control_gpio_status(kal_uint8 arg);
kal_uint8 track_cust_extbatcut_gpio_status(kal_uint8 arg);

#endif
#if 0
#endif /* 0 */
/*****************************************************************************
*  Local Functions                      ���غ���
******************************************************************************/

/******************************************************************************
 *  Function    -  lowbattery_sendsms
 *
 *  Purpose     -  ���͵͵籨������
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
static void lowbattery_sendsms(void)
{
    if(G_parameter.low_power_alarm.alarm_type >= 1 && G_parameter.low_power_alarm.alarm_type != 3)
    {
        struct_msg_send g_msg = {0};
        g_msg.addUrl = KAL_FALSE;
        g_msg.cm_type = CM_SMS;
        snprintf(g_msg.msg_en, CM_PARAM_LONG_LEN_MAX, str_battery_low_en);
#if defined(__MT200__)
        g_msg.msg_cn_len = 23;
#else
        g_msg.msg_cn_len = 34;
#endif
        memcpy(g_msg.msg_cn, str_battery_low_cn, g_msg.msg_cn_len);
        track_cust_module_alarm_msg_send(&g_msg, NULL);
    }
    if(G_parameter.low_power_alarm.alarm_type == 2 || G_parameter.low_power_alarm.alarm_type == 3)
    {
        track_start_timer(TRACK_CUST_ALARM_LowBattery_Call_TIMER, 60000, track_cust_make_call, (void*)1);
    }
    track_alarm_vec_queue_remove(2, 0, TR_CUST_ALARM_LowBattery);
}


static void poweroff_sendsms(void)
{
#if !defined(__GT740__)
    LOGD(L_APP, L_V5, "======");
    if(G_parameter.low_power_alarm.alarm_type >= 1)
    {
        struct_msg_send g_msg = {0};
        g_msg.addUrl = KAL_FALSE;
        g_msg.cm_type = CM_SMS;
        snprintf(g_msg.msg_en, CM_PARAM_LONG_LEN_MAX, str_battery_low_will_turn_off_en);
        g_msg.msg_cn_len = 34;
        memcpy(g_msg.msg_cn, str_battery_low_will_turn_off_cn, g_msg.msg_cn_len);
        track_cust_module_alarm_msg_send(&g_msg, NULL);
    }
#endif
}
/******************************************************************************
 *  Function    -  lowbattery
 *
 *  Purpose     -  ��ʱ����
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
static void lowbattery(void *arg)
{
    kal_uint32 par = (kal_uint32)arg;
    LOGD(L_APP, L_V5, "%d", par);
    if(par == 1)
    {
        // ��ѹ�ָ� >= 3.8V
        G_realtime_data.lowBattery = 0;
        G_realtime_data.poweroff = 0;
		track_cust_N07_paket_04(ALARM_EXBAT_LOW, 0x00);
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
    }
    else if(par == 2)
    {
#if defined(__NF2318__)
		track_cust_N07_paket_04(ALARM_EXBAT_LOW, 0x01);
#else
        track_gps_data_struct *gpsLastPoint = NULL;
        gpsLastPoint = track_cust_gpsdata_alarm();
#if defined(__GT740__)||defined (__GT370__)|| defined (__GT380__)
        if(gpsLastPoint->gprmc.status == 2)
        {
            track_cust_paket_19(track_drv_get_lbs_data(), TR_CUST_ALARM_LowBattery, KAL_TRUE, 0);
        }
        else
        {
            track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_LowBattery, KAL_TRUE, 0);
        }
#else
        track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_LowBattery, KAL_TRUE, 0);
#endif

        track_os_intoTaskMsgQueue(lowbattery_sendsms);
#endif
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);

    }
	track_cust_paging_command();//�ϴ���λ��
	//track_instructions_get_paging();
}

/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/

/******************************************************************************
 *  Function    -  track_cust_module_low_power
 *
 *  Purpose     -  �͵籨���жϣ���ʱ���ѹ������Ч��
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_module_low_power(kal_bool batteryStatus)
{
    if(G_parameter.low_power_alarm.sw == 0)
    {
        return;
    }
    if(batteryStatus == 0 && G_realtime_data.lowBattery != 0)
    {
        // ��ص�ѹ >= 3.8V
        LOGD(L_APP, L_V5, "flag=%d, batteryStatus=%d, sw=%d, alarm_type=%d, battery_volt=%d",
             G_realtime_data.lowBattery,
             batteryStatus,
             G_parameter.low_power_alarm.sw,
             G_parameter.low_power_alarm.alarm_type,
             track_cust_get_battery_volt());
        G_realtime_data.lowBattery = 0;
#if defined(__NF2318__)
        track_start_timer(TRACK_CUST_ALARM_LowBattery_SHAKE_TIMER, 10000, lowbattery, (void *)1);
#else
        track_start_timer(TRACK_CUST_ALARM_LowBattery_SHAKE_TIMER, 30000, lowbattery, (void *)1);
#endif
    }
    else if(batteryStatus && G_realtime_data.lowBattery == 0)
    {
        // ��ص�ѹ
        LOGD(L_APP, L_V5, "flag=%d, batteryStatus=%d, sw=%d, alarm_type=%d, battery_volt=%d",
             G_realtime_data.lowBattery,
             batteryStatus,
             G_parameter.low_power_alarm.sw,
             G_parameter.low_power_alarm.alarm_type,
             track_cust_get_battery_volt());
#if defined(__FLY_MODE__)
        track_cust_flymode_set(30);
#endif
        G_realtime_data.lowBattery = 1;
#if defined(__GT740__) || defined(__NF2318__)
        track_start_timer(TRACK_CUST_ALARM_LowBattery_SHAKE_TIMER, 10000, lowbattery, (void *)2);
#else
        track_start_timer(TRACK_CUST_ALARM_LowBattery_SHAKE_TIMER, 30000, lowbattery, (void *)2);
#endif
    }
    else
    {
        LOGD(L_APP, L_V6, "flag=%d, batteryStatus=%d, sw=%d, alarm_type=%d, battery_volt=%d",
             G_realtime_data.lowBattery,
             batteryStatus,
             G_parameter.low_power_alarm.sw,
             G_parameter.low_power_alarm.alarm_type,
             track_cust_get_battery_volt());
    }
}

//1=������籣��  �ض����
//0=�����籣��  �ָ�����
/*GSM�������ʹ��       --    chengjun  2017-10-13*/
void track_cust_module_extbat_control(U8 sw)
{
    //LOGD(L_APP, L_V6, "%d, %d", sw, G_parameter.flycut);
    if(sw == 1)
    {
        // ������籣��
        track_gps_data_struct *gpsLastPoint = NULL;
        gpsLastPoint = track_cust_gpsdata_alarm();
#if defined(__FLY_MODE__)
        if(G_parameter.flycut)
        {
            track_cust_flymode_set(10);  // ��ʱ�������ģʽ
        }
#endif

#if !(defined(__GT740__))
        if(G_realtime_data.disExtBattery != 1)
        {
#if defined(__FLY_MODE__)
            if(G_parameter.flycut)
                track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_Ext_LowBattery_Fly, KAL_TRUE, 1);
            else
#endif /* __FLY_MODE__ */
                track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_Ext_PowerFailure, KAL_TRUE, 1);
            if(G_parameter.power_extbatcut.alarm_type >= 1)
            {
                struct_msg_send g_msg = {0};
                g_msg.addUrl = KAL_FALSE;
                g_msg.cm_type = CM_SMS;
                snprintf(g_msg.msg_en, CM_PARAM_LONG_LEN_MAX, str_disconnect_the_external_power_en);
                g_msg.msg_cn_len = 42;
                memcpy(g_msg.msg_cn, str_disconnect_the_external_power_cn, g_msg.msg_cn_len);
                track_cust_module_alarm_msg_send(&g_msg, NULL);
            }
        }
#endif
#if defined(__MT200__) && defined(__FLY_MODE__)//MT200������һ�������ͱ���
        if(GPIO1_control != 1)
        {
            track_drv_gpio_disconnect_ext_power(1);
            GPIO1_control = 1;

        }
#else
        track_drv_gpio_disconnect_ext_power(1);
#endif
        if(G_realtime_data.disExtBattery != 1)
        {
            G_realtime_data.disExtBattery = 1;
            Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
        }
        tick_for_disconnect_ext_power = OTA_kal_get_systicks();
    }
    else
    {
        // �����籣��
#if defined(__FLY_MODE__)
        track_cust_flymode_set(20);
#endif /* __FLY_MODE__ */
        if(G_realtime_data.disExtBattery != 0)
        {
            LOGD(L_APP, L_V7, "���͵籨����־�ָ�:%d", G_realtime_data.lowExtBattery);
            G_realtime_data.disExtBattery = 0;
            Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
        }
        track_drv_gpio_disconnect_ext_power(0);
#if defined(__MT200__) && defined(__FLY_MODE__)
        if(GPIO1_control != 0)
        {
            GPIO1_control = 0;
        }
#endif
    }
}

U32 get_ms_from_last_disconnect_ext_power()
{
    U32 ms = OTA_kal_ticks_to_milli_secs(OTA_kal_get_systicks() - tick_for_disconnect_ext_power);
    LOGD(L_APP, L_V7, "%d", ms);
    return ms;
}

void track_cust_module_ext_vbat(kal_uint32 volt)
{
    static kal_uint32 last_tick = 0, last_tick2 = 0, last_tick3 = 0, last_tick4 = 0;
    kal_uint32 tick;
    char buf[320] = {0};
    U8 acc = track_cust_status_acc();

    tick = kal_get_systicks();
    LOGD(L_APP, L_V7, "��� acc:%d, volt:%d", acc, volt);
#if defined(__MT200__) && defined(__FLY_MODE__)
    if(track_drv_external_cut_status())//ADC�����綼�ε����Ŵ����ϵ籨��
    {
        track_cust_control_gpio_status(1);
    }
    else
    {
        track_cust_control_gpio_status(0);
    }
#endif
    if(G_parameter.power_extbatalm.sw)
    {
        LOGD(L_APP, L_V7, "���͵籨��:%d, tick:%d(%d) sec:%d(%d), volt:%d(%d)",
             G_realtime_data.lowExtBattery,
             tick, last_tick, kal_ticks_to_secs(tick - last_tick), G_parameter.power_extbatalm.detection_time,
             volt, G_parameter.power_extbatalm.threshold * 100000);
        // ���͵籨��
        if(volt < G_parameter.power_extbatalm.threshold * 100000)
        {
            if(last_tick != 0 && volt > 5000000)
            {
                if(G_realtime_data.lowExtBattery == 0 && kal_ticks_to_secs(tick - last_tick) > G_parameter.power_extbatalm.detection_time)
                {
#if !(defined(__GT740__))
                    track_gps_data_struct *gpsLastPoint = NULL;
                    gpsLastPoint = track_cust_gpsdata_alarm();
                    track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_Ext_LowBattery, KAL_TRUE, 1);
                    if(G_parameter.power_extbatalm.alarm_type >= 1)
                    {
                        struct_msg_send g_msg = {0};
                        g_msg.addUrl = KAL_FALSE;
                        g_msg.cm_type = CM_SMS;
                        snprintf(g_msg.msg_en, CM_PARAM_LONG_LEN_MAX, str_external_power_low_en);
                        g_msg.msg_cn_len = 32;
                        memcpy(g_msg.msg_cn, str_external_power_low_cn, g_msg.msg_cn_len);
                        track_cust_module_alarm_msg_send(&g_msg, NULL);
                    }
#endif
                    //track_alarm_vec_queue_remove(2, 0, TR_CUST_ALARM_LowBattery);
                    G_realtime_data.lowExtBattery = 1;
                    Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
                }
            }
            else
            {
                last_tick = tick;
            }
        }
        else
        {
            last_tick = 0;
        }
    }
    // ����ѹ�ָ�
    if(volt > G_parameter.power_extbatalm.recover_threshold * 100000)
    {
        if(last_tick2 != 0)
        {
            if(G_realtime_data.lowExtBattery == 1 && kal_ticks_to_secs(tick - last_tick2) > G_parameter.power_extbatalm.detection_time)
            {
                G_realtime_data.lowExtBattery = 0;
                Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
            }
        }
        else
        {
            last_tick2 = tick;
        }
    }
    else
    {
        last_tick2 = 0;
    }


    /*��籣������       --    chengjun  2017-10-13*/

#if defined (__MCU_CTL_EXBAT__)
    return;
#endif /* __MCU_CTL_EXBAT__ */

    if(acc == 1)
    {
        track_cust_module_extbat_control(0);
#if defined(__MT200__) && defined(__FLY_MODE__)
        if(GPIO1_control != 0)
        {
            GPIO1_control = 0;
        }
#endif
    }
    else
    {
        // ���ָ���ͨ
        if(volt > G_parameter.power_extbatcut.recover_threshold * 100000)
        {
#if defined(__MT200__)
            last_tick3 = 0;
#endif
            if(last_tick4 == 0)
            {
                last_tick4 = tick;
            }
            else if(kal_ticks_to_secs(tick - last_tick4) > G_parameter.power_extbatcut.detection_time)
            {
#if !defined(__MT200__)
                track_cust_module_extbat_control(0);
#endif
            }
        }
        else
        {
            last_tick4 = 0;
            if(!G_parameter.power_extbatcut.sw) return;
            LOGD(L_APP, L_V7, "���͵�Ͽ����:%d, tick:%d(%d) sec:%d(%d), volt:%d(%d)",
                 G_realtime_data.disExtBattery,
                 tick, last_tick3, kal_ticks_to_secs(tick - last_tick3), G_parameter.power_extbatcut.detection_time,
                 volt, G_parameter.power_extbatcut.threshold * 100000);
            // ���͵�Ͽ�
            if(volt < G_parameter.power_extbatcut.threshold * 100000)
            {
#if defined(__MT200__)
                if(volt < 5000000)
                {
                    //�޸������Ͽ��ͽ�������м��ʳ��֣�tick - last_tick3 ����detection_time��
                    last_tick3 = 0;
                }
#endif
                if(last_tick3 == 0)
                {
                    last_tick3 = tick;
                }
                else if(volt > 5000000 && kal_ticks_to_secs(tick - last_tick3) > G_parameter.power_extbatcut.detection_time)
                {
                    track_cust_module_extbat_control(1);
                }
                LOGD(L_APP, L_V7, "tick:%d,last_tick3:%d; sec:%d(%d)", tick, last_tick3, kal_ticks_to_secs(tick - last_tick3), G_parameter.power_extbatcut.detection_time);
            }
            else
            {
                last_tick3 = 0;
            }
        }
    }
}

void track_cust_module_set_power_saving_mode(kal_int8 status)
{
    if(G_realtime_data.power_saving_mode != status)
    {
        if(status == 0)
        {
            LOGD(L_DRV, L_V5, "@@1 status =%d", status);
            G_realtime_data.power_saving_mode = 0;
        }
        else
        {
            LOGD(L_DRV, L_V5, "@@2 status =%d", status);
            G_realtime_data.power_saving_mode = 1;
        }
        LOGD(L_DRV, L_V5, "status:%d, power_saving_mode:%d ", status, G_realtime_data.power_saving_mode);
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
        if(G_realtime_data.power_saving_mode == 1)
        {
            track_cust_led_sleep(2);
            track_cust_gps_off_req((void*)5);
        }
        else
        {
            track_cust_led_sleep(1);
            if(G_parameter.sensor_gps_work == 0)
            {
                track_cust_gps_work_req((void*)3);
            }
            else if(track_cust_status_acc() == 0)
            {
                track_cust_gps_off_req((void*)5);
            }
        }

    }
    else
    {
        LOGD(L_DRV, L_V5, "�Ѿ����ڸ�״̬status:%d, power_saving_mode:%d;led_sleep=%d ", status, G_realtime_data.power_saving_mode, track_drv_led_sleep_status());
        if(G_realtime_data.power_saving_mode == 1 && track_drv_led_sleep_status() == 1)
        {
            track_cust_led_sleep(2);
        }
    }

}
void track_cust_module_power_off(void)
{
    U8 buf[100] = {0};
    extern void track_drv_sys_power_off_req(void);

    LOGD(L_APP, L_V5, "\r\n======");

    if(track_cust_status_charger() == 0)
    {
        track_drv_watch_dog_close(1);
#if defined(__GT300S__)||defined(__GT370__)|| defined (__GT380__)
        track_soc_bakup_data((void*)3);
        sprintf(buf, "AT^GT_CM=03#");
        track_spi_write(buf, strlen(buf));
        tr_start_timer(TRACK_POWER_RESET_TIMER_ID, 1000, track_drv_sys_power_off_req);
#endif /* __GT300S__ */
    }
    LOGD(L_APP, L_V5, "======\r\n\r\n");

}

/******************************************************************************
 *  Function    -  track_cust_module_power_off_ready
 *
 *  Purpose     -
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 04-09-2013,  ZengPing && Liujw  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_module_power_off_ready(void)
{
    static kal_uint8 flag = 0;
#if defined(__GT300S__)|| defined(__GT370__)|| defined (__GT380__)

    LOGD(L_APP, L_V5, "======sw =%d  poweroff =%d", G_parameter.low_power_alarm.sw, G_realtime_data.poweroff);
    if(flag == 0)
    {
        flag = 1;
        if(G_realtime_data.lowBattery == 1)
        {
            if(G_parameter.low_power_alarm.sw == 1)
            {
                if(G_realtime_data.poweroff == 0)
                {
                    //send sms
                    poweroff_sendsms();
                    G_realtime_data.poweroff = 1;
                    Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
                }
            }
        }
        //power off
        if(FALSE == track_is_timer_run(TRACK_DALY_SHUTDOWN_TIMER_ID))
        {
            tr_start_timer(TRACK_DALY_SHUTDOWN_TIMER_ID, 90000, track_cust_module_power_off);
        }
        //track_drv_watch_dog_close();
    }

#endif
}


#if defined (__MCU_CTL_EXBAT__)
static kal_uint8 mcu_control_extbat_connect = 0;//1;
kal_uint8 track_is_mcu_control_extbat_connect(void)
{
    return mcu_control_extbat_connect;
}

/******************************************************************************
 *  Function    -  track_cust_module_mcu_control_extbat
 *
 *  Purpose     -  ��Ƭ��������絼ͨ���ֹ���ϱ�֪ͨ
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-10-13,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_module_mcu_control_extbat(U8 connect)
{
    LOGD(L_APP, L_V5, "%d, %d", connect, G_parameter.flycut);

    mcu_control_extbat_connect = connect;

    if(connect == 0)
    {
        // ������籣��
        track_gps_data_struct *gpsLastPoint = NULL;
        gpsLastPoint = track_cust_gpsdata_alarm();
#if defined(__FLY_MODE__)
        if(G_parameter.flycut)
        {
            track_cust_flymode_set(10);  // ��ʱ�������ģʽ
        }
#endif

        if(G_realtime_data.disExtBattery != 1)
        {
#if defined(__FLY_MODE__)
            if(G_parameter.flycut)
                track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_Ext_LowBattery_Fly, KAL_TRUE, 1);
            else
#endif /* __FLY_MODE__ */
                track_cust_paket_16(gpsLastPoint, track_drv_get_lbs_data(), TR_CUST_ALARM_Ext_PowerFailure, KAL_TRUE, 1);
            if(G_parameter.power_extbatcut.alarm_type >= 1)
            {
                struct_msg_send g_msg = {0};
                g_msg.addUrl = KAL_FALSE;
                g_msg.cm_type = CM_SMS;
                snprintf(g_msg.msg_en, CM_PARAM_LONG_LEN_MAX, str_disconnect_the_external_power_en);
                g_msg.msg_cn_len = 42;
                memcpy(g_msg.msg_cn, str_disconnect_the_external_power_cn, g_msg.msg_cn_len);
                track_cust_module_alarm_msg_send(&g_msg, NULL);
            }
        }

        if(G_realtime_data.disExtBattery != 1)
        {
            G_realtime_data.disExtBattery = 1;
            Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
        }
        tick_for_disconnect_ext_power = OTA_kal_get_systicks();
    }
    else
    {
        // �����籣��
#if defined(__FLY_MODE__)
        track_cust_flymode_set(20);
#endif /* __FLY_MODE__ */
        if(G_realtime_data.disExtBattery != 0)
        {
            LOGD(L_APP, L_V7, "���͵籨����־�ָ�:%d", G_realtime_data.lowExtBattery);
            G_realtime_data.disExtBattery = 0;
            Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
        }
    }
}
#endif /* __MCU_CTL_EXBAT__ */


#if defined(__FLY_MODE__)
/******************************************************************************
 *  Function    -  track_cust_module_low_power_fly_change
 *
 *  Purpose     -  ����ģʽ�л�
 *
 *  Description -   <50������

 * shutdown :
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 18-6-2015,  chenjiajun  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_module_low_power_fly_change(U32 port)
{
    static kal_uint8 time_reconn  = 0;

#if defined (__NT13V__)
    //��������
#else
    if(G_parameter.flycut == 0) 
    {
        return;
    }
#endif /* __NT13V__ */
 
    LOGD(L_APP, L_V6, "port:%d, sms_staus:%d", port, sms_staus);
    if(port >= 0 && port < 50 && sms_staus != SMS_AIRPLANE_MODE && !(track_is_testmode()))
    {
        time_reconn = 0;
        sms_staus = SMS_AIRPLANE_MODE;
        track_stop_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER);
        //track_cust_gps_off_req((void*)5);
        track_cust_gps_control(0);
        //track_drv_sensor_off();
        track_cust_led_sleep(2);
        track_drv_sys_cfun(4);
        track_soc_disconnect();
        track_cust_server2_disconnect();
        //track_os_intoTaskMsgQueue(track_drv_ext_vbat_measure_modify_parameters);
        track_cust_make_call((void*)23);
        track_cust_call_hangup();
        track_sms_reset();
        LOGD(L_APP, L_V4, "!!! �������ģʽ!!!");
    }
    else if(port >= 50 && port < 90 && sms_staus != SMS_NORMAL_MODE)
    {
        track_stop_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER);
        time_reconn = 0;
        sms_staus = SMS_NORMAL_MODE;
        //track_cust_gps_work_req((void *)2);    // gps on
        //if(G_parameter.sensor_gps_work == 0) track_cust_gps_control(1);
        //track_cust_sensor_init();
        track_drv_sys_cfun(1);
        track_stop_timer(TRACK_CUST_NET_CONN_TIMEOUT_TIMER_ID);
        tr_stop_timer(TRACK_CUST_ALARM_RETURN_FlyMode_TIMER);
        //track_cust_net_conn();
        //tr_start_timer(TRACK_CUST_NET_CONN_TIMER_ID, 1000, track_cust_net_conn);
        if(G_realtime_data.key_status != 0) /* LEDSW �и������ȼ� */
            track_cust_led_sleep(1);
        //track_os_intoTaskMsgQueue(track_drv_ext_vbat_measure_modify_parameters);
        track_soc_exit_fly_only();
        track_cust_call_hangup();
        track_sms_reset();
        LOGD(L_APP, L_V4, "!!! �˳�����ģʽ!!!");
    }
}


/******************************************************************************
 *  Function    -  track_cust_flymode_set
 *
 *  Purpose     -  �͵籣�����Ƿ������ģʽ
 *
 *  Description -  0=�˳�����ģʽ
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2017-10-28,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
kal_bool track_cust_flymode_set(U32 mode)
{
    static kal_bool is_in_flight_mode = 0, alarm = 0;
    static U32 tick_alarm = 0;
    U32 tick = OTA_kal_get_systicks();
    U32 volt = track_cust_get_battery_volt();
    U32 volt_ext = track_cust_get_ext_volt();
    kal_bool islowbat = track_cust_isbatlow();
    kal_bool accStatus = track_cust_status_acc();


#if defined (__NT13V__)
    if (mode==51) //SLEEP ON ǿ�ƽ�����ģʽ
    {
        /* ָ�������ģʽ*/
        is_in_flight_mode = 1;
        track_cust_module_low_power_fly_change(10);
        return;
    }
    else if (mode == 50)
    {
        /* �˳�����ģʽ */
        if(track_is_mcu_control_extbat_connect()==0)
        {
            LOGD(L_APP, L_V4, "��籣�������У������˳�");
        }
        else
        {
            is_in_flight_mode = 0;
            alarm = 0;
            track_stop_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER);
            if(sms_staus == SMS_AIRPLANE_MODE)
            {
                track_cust_module_low_power_fly_change(53);
            }
        }
        return;
    }
#endif /* __NT13V__ */

    //����������ģʽ
    if(G_parameter.flycut != 0)
    {
        if(mode == 0)
        {
            /* ָ��ر�ǿ���˳�����ģʽ�����Ե�ѹ */
            is_in_flight_mode = 0;
            alarm = 0;
            track_stop_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER);
            track_cust_module_low_power_fly_change(51);
        }
#if defined(__MT200__) || defined (__NT17__)
        //������ģʽ�����ǵ�ص�ѹ
#else
        else if(islowbat)
        {
            track_stop_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER);
            if(sms_staus == SMS_NORMAL_MODE)
            {
                LOGD(L_APP, L_V4, "�ڲ���ص͵磬����������̶�����ģʽ");
                is_in_flight_mode = 1;
                track_cust_module_low_power_fly_change(1);
            }
        }
#endif
        else if(mode == 1 || accStatus)
        {
            /* ACC �ߣ��˳�����ģʽ */
            is_in_flight_mode = 0;
            alarm = 0;
            track_stop_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER);
            if(sms_staus == SMS_AIRPLANE_MODE)
            {
                track_cust_module_low_power_fly_change(52);
            }
        }
        else if(mode == 10 && is_in_flight_mode == 0)
        {
            /* �������ģʽ */
            if(sms_staus == SMS_NORMAL_MODE)
            {
                if(track_is_timer_run(TRACK_CUST_MODULE_ALARM_VIBRATES_DELAY_TIMER_ID))
                {
                    LOGD(L_APP, L_V5, "�𶯱�����ʱ�У���ʱ�������ģʽ");
                    track_start_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER, 10000, track_cust_flymode_set, (void*)10);
                }
                else
                {
                    is_in_flight_mode = 1;
                    track_start_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER, 60000, track_cust_module_low_power_fly_change, (void*)2);
                }
            }
            else
            {
                track_stop_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER);
            }
        }
        else if(mode == 20)
        {
            /* �˳�����ģʽ */
            is_in_flight_mode = 0;
            alarm = 0;
            track_stop_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER);
            if(sms_staus == SMS_AIRPLANE_MODE)
            {
                track_cust_module_low_power_fly_change(53);
            }
        }
        else if(mode == 30 && is_in_flight_mode == 1)
        {
            /* ��ʱ�˳�����ģʽ����ʱ�ȴ������¼������������½������ģʽ */
            alarm = 1;
            tick_alarm = OTA_kal_get_systicks();
            track_cust_module_low_power_fly_change(54);
            track_start_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER, G_parameter.sensor_gps_work * 60000, track_cust_flymode_set, (void*)40);
        }
        else if(mode == 31 && is_in_flight_mode == 1 && alarm == 1)
        {
            /* �����Ѿ���������ʱδ�����¼�⵽�𶯣����¼�����ʱ */
            tick_alarm = OTA_kal_get_systicks();
            track_start_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER,G_parameter.sensor_gps_work * 60000,track_cust_flymode_set, (void*)41);
        }
        else if(mode == 32 && is_in_flight_mode == 1)
        {
            /* ��ʱ�˳�����ģʽ����ʱ�ȴ��𶯻���GPS��λ�¼������������½������ģʽ */
            alarm = 1;
            track_cust_module_low_power_fly_change(54);
            track_start_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER, G_parameter.sensor_gps_work * 60000, track_cust_flymode_set, (void*)42);
        }
        else if(mode >= 40 && mode <= 49 && is_in_flight_mode == 1)
        {
            if(mode <= 41 && track_cust_module_alarm_lock(98, 2) != 0
                    && kal_ticks_to_secs(OTA_kal_get_systicks() - tick_alarm) < G_parameter.sensor_gps_work * 120)/*��ֹ��ʱ�˳�����ģʽ���б��������ڱ����޷�˳���ͳ����²����ٽ�����е�����*/
            {
                track_start_timer(TRACK_CUST_ALARM_LowBattery_Fly_TIMER, 30000, track_cust_flymode_set, (void*)40);
            }
            else
            {
                alarm = 0;
                track_cust_module_low_power_fly_change(3);
            }
        }
    }
    LOGD(L_APP, L_V7, "%d,%d, volt:%d, volt_ext:%d, flyMode:%d, is_in_flight_mode:%d, isBatLow:%d, sensor_gps_work:%d",
         G_parameter.flycut, mode, volt, volt_ext, sms_staus, is_in_flight_mode, islowbat, G_parameter.sensor_gps_work);
    return (sms_staus == SMS_AIRPLANE_MODE ? 1 : 0);
}

#if defined(__MT200__)
/******************************************************************************
 *  Function    -  track_cust_control_gpio_status
 *
 *  Purpose     -  ����MT200�ڷ���ģʽ�аε����ʱ�������ϵ籨��
 *          ����ģʽ�����½������ʱ��Ҫ��GPIO�����¹���
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 31-10-2016,  Cjj  written
 * v2.0  , 22-03-2017,  Cjj  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_control_gpio_status(kal_uint8 arg)
{
    static kal_bool control_poweralm_fly = KAL_FALSE;//��ʶ���ã������ظ�����
    if(arg == 1)
    {
        if(track_cust_extbatcut_gpio_status(99) == 1)
        {
            //����Ӳ������GPIO�����⣬�ε����
            //ԭ�������ģʽ���رյ�GPIO�ڣ����Զ�Ĭ�ϴ򿪣���������ͬ��״̬���
            //�������ķ����߼��ж�
            GPIO1_control = 0;
            track_cust_extbatcut_gpio_status(0);
        }
        //LOGD(L_APP, L_V6, "1,%d  %d   %d",track_cust_flymode_set(188),GPIO1_control,control_poweralm_fly);
        if(track_cust_flymode_set(188) && GPIO1_control == 0 && control_poweralm_fly == KAL_FALSE)
        {
            track_cust_module_power_fails((void *)1);
            control_poweralm_fly = KAL_TRUE;
        }
    }
    else if(arg == 0)
    {
        if(track_cust_extbatcut_gpio_status(99) != 1)
        {
            track_cust_extbatcut_gpio_status(1);
        }
        //�������ϵ籨�����������ڷ���ģʽ�½ӻ���磬����Ӳ������GPIO�����⣬�ε����
        //�ٽӻ�ʱ��ԭ�������ģʽ���رյ�GPIO�ڣ����Զ�Ĭ�ϴ򿪣�������Ҫ���¹ر�
        //�ָ���磬ֻ��ACC�л���ONʱ���������˳�����ģʽ����GPIO�ڡ�
        //LOGD(L_APP, L_V6, "0,%d  %d   %d",track_cust_flymode_set(188),GPIO1_control,control_poweralm_fly);
        if(track_cust_flymode_set(188) && GPIO1_control == 0 && control_poweralm_fly)
        {
            GPIO1_control = 1;
            track_drv_gpio_disconnect_ext_power(1);
            track_cust_led_sleep(2);
            control_poweralm_fly = KAL_FALSE;
        }
    }
}
#if 1
/******************************************************************************
 *  Function    -  track_cust_extbatcut_gpio_status
 *
 *  Purpose     -  MT200����ģʽ�£���Ҫ�ر�GPIO�ڵģ����ڰε�����ٽ��룬
 *         Ӳ�����Ƶ�GPIO�����´򿪣��˴����ڼ�¼������̣�����������߼�
 *                 gpio_control=1,˵��֮ǰ�жϹ���磬�������½����ˣ������ڷ���ģʽ�£�GPIO���Զ��ر�
 *                 gpio_control=0,˵���Ͽ�����ˣ�����Ҫ��GPIO�ڱ�ʶ��Ӳ���Ŀ���ͬ����һ���
 *                 gpio_control=2,��ʼֵ
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 03-21-2017,  Cjj  written
 * ----------------------------------------
*******************************************************************************/
kal_uint8 track_cust_extbatcut_gpio_status(kal_uint8 arg)
{
    static kal_bool gpio_control = 2;
    if(arg == 1)
        gpio_control = 1;
    else if(arg == 0)
        gpio_control = 0;
    else if(arg == 2)
        gpio_control = 2;
    else
        return gpio_control;

}
#endif

#endif
#endif /* __FLY_MODE__ */


