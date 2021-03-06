/******************************************************************************
 * track_os_timer.h -
 *
 * Copyright .
 *
 * DESCRIPTION: -
 *        自定义定时器
 *
 * modification history
 * --------------------
 * v1.0   2012-07-15,  chengjun create this file
 *
 ******************************************************************************/
#ifndef _TRACK_TIMER_H
#define _TRACK_TIMER_H

/****************************************************************************
* Include Files             包含头文件
*****************************************************************************/
#include "track_os_data_types.h"
#include "kal_public_defs.h"

/*****************************************************************************
 *  Define			    宏定义
*****************************************************************************/


/*****************************************************************************
* Typedef  Enum         枚举
*****************************************************************************/
typedef enum
{
    /*DRV start*/
    SYSTEM_BOOTUP_TIMER_ID,
    WATCH_DOG_TIMER_ID,
    WATCH_DOG_EXTCHANGE_TIMER_ID,
    WATCH_DOG_ENABLE_TIMER_ID,
    SYSTEM_SHUTDOWN_TIMER_ID,
    CHECK_NETWORKINFO_TIMER_ID,
    //CHECK_BMT_TIMER_ID,
    GPS_RESET_TIMER_ID,
    GPS_DATA_UPDATA_TIMER_ID,

    GPS_UART2_RESET_INIT_TIME_ID,//delete
    GPS_UART1_RESET_INIT_TIME_ID,//delete

    TRACK_DALY_SHUTDOWN_TIMER_ID,
#if defined(__TRACK_SENSOR__)
    MOTION_SENSOR_DATA_TIMER_ID,
    MOTION_SENSOR_RESET_TIMER_ID,
#endif /* __TRACK_SENSOR__ */
    DISPLAY_TEST_TIMER_ID,
    PCBA_TEST_OUTPUT_TIMER_ID,
    PCBA_TEST_OUTPUT2_TIMER_ID,
#if defined (__NETWORK_LICENSE__)
    NETWORK_LICENSE_GPRS_ATTACH_TIMER_ID,
    NETWORK_LICENSE_AUTO_MAKE_CALL_TIMER_ID,
#endif /* __NETWORK_LICENSE__ */
#if defined (__OLED__)
    LED_DISPALY_TIMER_ID,
#endif /* __OLED__ */

    DEALY_INIT_OBD_TIMER_ID,//delete

    DEALY_INIT_POWERKEY_TIMER_ID,
    DEALY_INIT_RESET_TIMER_ID,

    TRACK_DRV_GPS_CHECK_TIMER_ID,
    TRACK_DRV_GLP_DELAY_TIMER_ID,
    TRACK_DRV_ACTIVE_REFTIME_TIMER_ID,
#if defined (__CUST_BT__)
    TRACK_DRV_BT_WAKEUP_GSM_TIMER_ID,
#endif
    TRACK_DRV_MCU_WAKEUP_GSM_TIMER_ID,

#if defined (__CUST_RS485__)
    TRACK_DRV_RS485_RX_WAKEUP_TIMER_ID,
#endif

    TRACK_DRV_BUZZER_SOUND_TIMER_ID,

#if defined (__MCU_SW_UPDATE__)
    TRACK_MCU_SW_UPDATE_UART_RSP_TIMEOUT,
    TRACK_MCU_SW_UPDATE_DOWNLOAD_TMER_ID,
#endif /* __MCU_SW_UPDATE__ */

    TRACK_DRV_MCU_NOACK_RESEND_TIMER_ID,
    TRACK_DRV_MCU_SEND_DELAY_TIMER_ID,
    /*DRV end*/

    /*SYS begin*/
    TRACK_POWER_RESET_TIMER_ID,
    TRACK_QUERY_SIM_TYPE_TIMER_ID,
    /*SYS end*/

    /*AT*/
#if defined(__AT_CA__)
    TRACK_SEND_VSAT_TIMER_ID,
    //TRACK_WAIT_UART_DATA_TIMER_ID,
    TRACK_AT_HAND_FREE_TIMER_ID,
    TRACK_SMS_AUTO_IN_TIMER_ID,
    TRACK_SMS_SEND_RSP_TIMER_ID,
    TRACK_SMS_SEND_CTX_TIMER_ID,
    TRACK_SMS_SEND_CHECK_TIMER_ID,
    TRACK_SMS_SEND_CHECK_LOCK_TIMER_ID,
#endif /* __AT_CA__ */

    /*CMD begin*/
    TRACK_CALL_WAIT_TEIMER_ID,
    TRACK_CALL_OVERTIME_TEIMER_ID,
    TRACK_DELAY_MAKECALL_TEIMER_ID,//wangqi
    TRACK_AKEY_CHECK_TIMEID,
    TRACK_SMS_WAIT_LONG_SMS_TIME_ID,
    TRACK_TM_RSTORE_LED_TIMER_ID,
    TRACK_SIM_WRITE_SB_TIMER_ID,
    TRACK_SIM_READ_SB_TIMER_ID,
    TRACK_SIM_W_CALLBACK_TIMER_ID,
    TRACK_SIM_DECODE_SMS_TIMER_ID,
    TRACK_OUTBOX_CALLBACK_TIMER_ID,
    TRACK_OUTBOX_READY_TIMER_ID,

    TRACK_FACTORY_ACC_CHECK_TIME_ID,
    TRACK_FACTORY_RSTORE_LED_TIME_ID,
    /*CMD end*/
    /*CMD end*/

    /*NET begin*/
    TRACK_SOCKET_SEND_TIMER_ID,
    TRACK_SOCKET_SEND_TIMER_ID2 = TRACK_SOCKET_SEND_TIMER_ID + 6,
    TRACK_SOC_TIMEOUT_TIMER_ID,
    TRACK_SOC_TIMEOUT_TIMER_ID2 = TRACK_SOC_TIMEOUT_TIMER_ID + 6,
    TRACK_SOC_SEND_TIMER_ID,
    TRACK_SOC_SEND_OVER_TIMER_ID,
    /*NET end*/

    /*APP begin*/
    TRACK_CUST_BACKUP_GPS_TIMER_ID,
    TRACK_CUST_PERIODIC_RESTART_TIMER_ID,
    TRACK_CUST_PERIODIC_RESTART_DELAY_TIMER_ID,
    TRACK_CUST_PERIODIC_RESTART_GPS_TIMER_ID,
    TRACK_CUST_PERIODIC_RESTART_GPRS_TIMER_ID,
    TRACK_CUST_NET_SEND_TIMER_ID,
    TRACK_CUST_NET2_RESEND_TIMER_ID,
    TRACK_CUST_NET_SERVER2_TIMER_ID,
    TRACK_CUST_NET_CONN_TIMER_ID,
    TRACK_CUST_NET_CONN_TIMER_ID2,
    TRACK_CUST_NET_CONN_TIMER_ID3,
    TRACK_CUST_NET_CONN_TIMEOUT_ID,
    TRACK_CUST_SEND_SUPL_TIMER_ID,
    TRACK_CUST_NET_CONN_TIMEOUT_TIMER_ID,
    TRACK_CUST_NET_CONN_BAKDATA_TIMER_ID,
    TRACK_CUST_NET_HEARTBEAT_TIMER_ID,
    TRACK_CUST_NET_HEARTBEAT_OVER_TIMER_ID,
    TRACK_CUST_NET_LOGIN_RESEND_TIMER_ID,
    TRACK_CUST_NET_LOGIN_TIMEOUT_TIMER_ID,
    TRACK_CUST_GPS_LOCATION_TIMEOUT_TIMER_ID,
    TRACK_CUST_GPS_LOCATION_WORKTIME_TIMER_ID,
    TRACK_CUST_GPS_UPDATE_EPHEMERIS_OVERTIME_TIMER_ID,
    TRACK_CUST_GPS_EPHEMERIS_UPDATE_TIMER_ID,
    TRACK_CUST_GPS_LOCATION_CURVED_CONDUIT_TIMER_ID,
    TRACK_CUST_GPS_SEND_TIMER_ID,
    TRACK_CUST_GPS_SECSET_TIMER_ID,
    TRACK_CUST_GPS_SECSET_AIDING_TIMER_ID,
    TRACK_CUST_SENSOR_DELAY_VALID_TIMER_ID,
    TRACK_CUST_SENSOR_LOCATION_STATICREP_TIMER_ID,
    TRACK_CUST_GET_ADDRESS_OVER_TIMER_ID,
    TRACK_CUST_GET_ADDRESS_OVER2_TIMER_ID,
    TRACK_CUST_GET_ADDRESS_OVER3_TIMER_ID,
    TRACK_CUST_SCROLL_MAKECALL_TIMER,
    TRACK_CUST_SCROLL_MAKECALL_OVERTIME_TIMER,
    TRACK_CUST_SOS_MAKECALL_TIMER,
    TRACK_CUST_SOS_SMS_OVERTIME_TIMER,
    TRACK_CUST_FENCE_OVERTIME_TIMER,
    TRACK_CUST_FENCE_TRIGGER_TIMER,
    TRACK_CUST_SOS_CALL_QUERY_ADDRESS_OVER_TIMER,
    TRACK_CUST_SOS_KEY_TIMER_ID,
    TRACK_CUST_STATUS_DEFENCES_TIMER_ID,
    TRACK_CUST_MODULE_ALARM_VIBRATES_TIMER_ID,
    TRACK_CUST_MODULE_ALARM_VIBRATES_CHECK_TIMER_ID,
    TRACK_CUST_MODULE_ALARM_VIBRATES_DELAY_TIMER_ID,
    TRACK_CUST_MODULE_ALARM_VIBRATES_SHORTEST_DELAY_TIMER_ID,
    TRACK_CUST_MODULE_ALARM_POWER_FAILS_TIMER_ID,
    TRACK_CUST_ALARM_MAKECALL_TIMER,
    TRACK_CUST_ALARM_LowBattery_OVERTIME_TIMER,
    TRACK_CUST_ALARM_LowBattery_Call_TIMER,
    TRACK_CUST_ALARM_LowBattery_SHAKE_TIMER,
    TRACK_CUST_ALARM_speed_limit_OVERTIME_TIMER,
    TRACK_CUST_ALARM_power_fails_OVERTIME_TIMER,
    TRACK_CUST_ALARM_power_fails_Call_TIMER,
    TRACK_CUST_ALARM_alarm_vibrates_OVERTIME_TIMER,
    TRACK_CUST_ALARM_alarm_vibrates_Call_TIMER,
    TRACK_CUST_ALARM_TEAR_DOWN_CALL_TIMER,

    TRACK_CUST_ALARM_displacement_OVERTIME_TIMER,
    TRACK_CUST_ALARM_displacement_Call_TIMER,
    TRACK_CUST_ALARM_displacement_set_OVERTIME_TIMER,
    TRACK_CUST_ALARM_displacement_CONFIRM_TIMER,

    TRACK_CUST_FETION_AUTO_AUTHORIZE_TIMER,
    TRACK_CUST_MODULE_SMS_RELAY_TIMER_ID,
    TRACK_CUST_MODULE_MOTOR_LOCK_TIMER_ID,
    TRACK_CUST_MODULE_MOTOR_LOCK_CINTINUE_TIMER_ID,
    TRACK_CUST_MODULE_MOTOR_LOCK_STATUS_OVERTIME_TIMER_ID,
    TRACK_CUST_SIM_BACKUP_PARAM_TIMER,
    TRACK_CUST_SIM_RESTART_TIMER,
    TRACK_CUST_SERVER2_RESEND_TIMER,
    TRACK_CUST_SERVER2_OVERTIME_TIMER,
    TRACK_CUST_LED_START_TIMER,
    TRACK_CUST_EPO_INT_TIMER_ID,
    TRACK_CUST_LED_ALARM_TIMER,
    TRACK_GPRS_ATTACHED_OVERTIME_TIMER,
    TRACK_VOICE_RSP_TIMER_ID,
    TRACK_CUST_SOS_IN_CALL_TIMER,
    TRACK_CUST_GPS_BLINDALM_OUT_TIMER_ID,
    TRACK_CUST_GPS_BLINDALM_IN_TIMER_ID,
    TRACK_CUST_bootalm_sms_TIMER,
    TRACK_CUST_SEND_LBS_TIMER_ID,
    TRACK_CUST_SEND_LBS_LIMIT_TIMER_ID,
    TRACK_CUST_LBS_UPDATE_L_TIMER_ID,
    TRACK_CUST_ALARM_GPS_WORK_TIMER,
    TRACK_CUST_ALARM_ACC_TIMER,
    TRACK_CUST_POWERON_CLOSE_GPS_TIMER,
    TRACK_CUST_SOS_KEY_DOUBLE_TIMER,
    TRACK_CUST_SOS_KEY_LED_ON_TIMER,
    TRACK_CUST_URL_OVER_TIMER_ID,
    TRACK_CUST_WHERE_OVER_TIMER_ID,
    TRACK_CUST_DW_GPS_WORK_TIMER,
    TRACK_CUST_WHERE_GPS_WORK_TIMER_ID,
    TRACK_CUST_KEY_UPDATA_OVER_TIMER_ID,
    TRACK_CUST_RING_VIB_TIMER_ID,
    TRACK_CUST_CALLMODE_STOP_TIMER_ID,
    TRACK_CUST_WORK_MODE_GPSUPDATE_TIMER_ID,//复用给7号电单车，用作半休眠定时开关GPS
    TRACK_CUST_WORK_MODE_GPSUPDATE_OVERTIME_TIMER_ID,
    TRACK_CUST_OVER_TIMER_GPSOFF_TIMER_ID,
    TRACK_CUST_GPS_COLD_START_TIMER_ID,
    TRACK_CUST_CLEAN_SMS_TIMER,
    TRACK_CUST_POWERON_LED_TIME_ID,
    TRACK_CUST_ACC_INIT_OVERTIME_ID,
    TRACK_CUST_GET_ADDRESS_OVER4_TIMER_ID,
    TRACK_SERVER_OVERTIME_TIMER,
    TRACK_CUST_DAIL_MAKECALL_TIMER,
    TRACK_CUST_DAIL_MAKECALL_OVERTIME_TIMER,
    TRACK_DEFENCE_RELAY_TIMER_ID,
    TRACK_GPSOFF_RELAY_TIMER_ID,
    TERMINAL_SLEEP_TO_WAKEUP_TIMER_ID,
    GPRS_SOCKET_INIT_TIMER_ID,
    TRACK_CUST_MODULE_ALARM_POWERFAIL_DELAY_TIMER_ID,
    TRACK_CUST_MODULE_ALARM_POWERFAIL_TIMER_ID,
    TRACK_CUST_ACCL_CHARGEOUT_CHECK_TIMER_ID,
    TRACK_CUST_CHANGE_OTAFILE_FLAG_TIMER_ID,
    TRACK_CUST_DIAL_NUM_TIMER_ID,
    TRACK_CUST_LED_SWITCH_TIMER,
    TRACK_CUST_WORK_MODE_GPSON_UPDATE_TIMER_ID,
    TRACK_CUST_PWROFFALM_TIMER_ID,
    TRACK_CUST_SEND_SMS_PWROFFALM_TIMER_ID,
    TRACK_CUST_RECODE_OFF_TIMER_ID,
    TRACK_CUST_SOS_GPS_DELAY_TIMER_ID,
    TRACK_CUST_WORK_MODE3_TIMER_ID,
    TRACK_CUST_WORK_MODE3__GPSON_TIMER_ID,
    TRACK_CUST_WORK_MODE1_GPSON_TIMER_ID,
    TRACK_CUST_GET_IMEI_TIMER_ID,
    TRACK_CUST_GET_LBS_TIMER_ID,
    TRACK_CUST_MOVING_ALARM_ONCE_TIMER,
#if !(defined(__GT740__)||defined(__EL100__)||defined(__NT13V__))
    TRACK_CUST_OBD_SEND_TIMER_ID,
    TRACK_CUST_OBD_SEND_OVERTIME_TIMER_ID,
    TRACK_CUST_OBD_RESTART_TIMER_ID,
    TRACK_CUST_OBD_OPENDATA_TIMER_ID,
    TRACK_CUST_OBD_CXALL_TIMER_ID,
    TRACK_CUST_OBD_SLEEP_TIMER_ID,
    TRACK_CUST_OBD_CMD_TIMER_ID,
    TRACK_CUST_OBD_START_TIMER_ID,
    TRACK_CUST_433_CONTROL_INI_TIMER_ID,//delete
    TRACK_CUST_433_CONTROL_DELAY_TIMER_ID,//delete
    TRACK_CUST_433_CONTROL_TIMER_ID,//delete
    TRACK_CUST_GT300S_MODE_AUTO_SW_TIMER_ID,
    TRACK_CUST_GT300S_MODE_INIT_SW_TIMER_ID,
#endif
    TRACK_CUST_EXTCHIP_SLEEP_TIMER_ID,
#if !(defined(__EL100__)||defined(__NT13V__))
    TRACK_CUST_EXTCHIP_MODE_AUTO_SW_TIMER_ID,
    TRACK_CUST_EXTCHIP_CAL_ELETICITY_TIMER_ID,
    TRACK_CUST_EXTCHIP_BATTERY_VOLTAGE_TIMER_ID,
    TRACK_CUST_EXTCHIP_WAIT_TIMER_ID,
    TRACK_CUST_EXTCHIP_WAIT02_TIMER_ID,
    TRACK_CUST_EXTCHIP_WAIT03_TIMER_ID,
    TRACK_CUST_EXTCHIP_WAIT05_TIMER_ID,
    TRACK_CUST_EXTCHIP_WAIT13_TIMER_ID,
    TRACK_CUST_EXTCHIP_WAIT99_TIMER_ID,
    TRACK_CUST_EXTCHIP_DELAY_SHUTDOWN_TIMER_ID,
    TRACK_CUST_EXTCHIP_TRACK_MODE_TIMEROUT_TIMER_ID,
    TRACK_CUST_EXTCHIP_SEND_REMOVE_ALARM_TIMER_ID,
    TRACK_CUST_EXTCHIP_SEND_OPEN_SHELL_ALARM_TIMER_ID,
    TRACK_CUST_EXTCHIP_DELAY_SEND_ALARM_TIMER_ID,
    TRACK_CUST_EXTCHIP_DELAY_SEND_ALARM_MSG_TIMER_ID,
    TRACK_CUST_EXTCHIP_DELAY_CAL_MODE2_ELE_TIMER_ID,
    TRACK_CUST_EXTCHIP_CALCULATE_BOOT_TIMER_ID,
    TRACK_CUST_EXTCHIP_DELAY_SEND_REMOVE_ALARM_TIMER_ID,
    TRACK_CUST_EXTCHIP_DELAY_SEND_OPEN_SHELL_ALARM_TIMER_ID,
    TRACK_CUST_EXTCHIP_CLOSE_SERIAL_TIMER_ID,
    TRACK_CUST_EXTCHIP_LBS_UPDATE_TIMER_ID,
    TRACK_CUST_EXTCHIP_DELAY_CLOSE_TIMER_ID,
    TRACK_CUST_EXTCHIP_DELAY_CLOSE2_TIMER_ID,
    TRACK_CUST_EXTCHIP_DELAY_FOR_LBS_TIMER_ID,
    TRACK_CUST_EXTCHIP_BATTERY_VOLTAGE2_TIMER_ID,
    TRACK_CUST_EXTCHIP_WAIT10_TIMER_ID,
    TRACK_CUST_CALCULATE_ELE_TIMER_ID,
    TRACK_CUST_EXTCHIP_NW_RESET_TIMER_ID,
    TRACK_CUST_EXTCHIP_DELAY_SHOWELE_TIMER_ID,
#endif
    TRACK_CUST_SERSOR_TO_REST_TIMER_ID,
    TRACK_CUST_FIND_CAR_TIMER_ID,
    TRACK_CUST_ALARM_DOOR_Call_TIMER,
    TRACK_CUST_ALARM_FCARM_TIMER,
    TRACK_CUST_NET_INFO_UP_TIMER_ID,
    TRACK_CUST_NET2_CHECK_TIMER_ID,
    TRACK_CUST_PACKET_94O4_DELAY_ID,
    TRACK_DRV_MECH_SENSOR_RELEASE_TIMER_ID,
    TRACK_CUST_WIFI_OFF_TIMER_ID,
    TRACK_CUST_WIFI_SCAN_TIMER_ID,
    TRACK_CUST_WORK_OFF_MODE3_TIMER_ID,
    TRACK_CUST_WIFI_WORK_TIMER_ID,
    TRACK_CUST_DELAY_OPEN_GPS_TIMER_ID,
    TRACK_CUST_DWCHANGE_INSTANT_CONTRACT_TIMER_ID,
    TRACK_CUST_ACCCHANGE_INSTANT_CONTRACT_TIMER_ID,
    TRACK_CUST_NET_CONN_TIMEOUT_TIMER_ID3,
    TRACK_CUST_NET3_LOGIN_TIMER_ID,
    TRACK_CUST_NET_HEARTBEAT_TIMER_ID3,
    TRACK_CUST_NET_HEARTBEAT_OVER_TIMER_ID3,
    TRACK_CUST_NET3_RESEND_TIMER_ID,
    TRACK_CUST_CALL_GPS_POSITION_TIMER_ID,
    TRACK_CUST_SHOCKS_UPLBS_TIMER_ID,
    TRACK_CUST_SHOCKS_UPLBS_CHECK_TIMER_ID,
    TRACK_CUST_FT_WORK_TIMER_ID,
    TRACK_CUST_ALARM_LowBattery_Fly_TIMER,
    TRACK_CUST_ALARM_RETURN_FlyMode_TIMER,
    TRACK_CUST_LED_OFF_TIMER,
    TRACK_CUST_NET3_LOGIN_TIMEOUT_TIMER_ID,
    TRACK_CUST_NET3_LOGIN_RESEND_TIMER_ID,
    TRACK_CUST_07_TIMEOUT_TIMER,
    TRACK_CUST_WRITE_BACKUP_TIMER,
    TRACK_CUST_READ_BACKUP_TIMER,
    TRACK_DRV_OIL_CUT_PULSE_TIMER_ID,
    TRACK_CUST_OIL_CUT_PWM_DELAY_TIMER_ID,
    TRACK_CUST_EXT_POWER_GPIO_LOW_TIMER_ID,
#if defined (__BAT_TEMP_TO_AMS__)
    TRACK_CUST_TEPERATURE_DATA_TIMER_ID,
#endif /* __BAT_TEMP_TO_AMS__ */
    TRACK_CUST_DETECT_NO_VIB_TIMER_ID,
    TRACK_CUST_NO_VIB_GPS_UPDATE_TIMER_ID,
    TRACK_CUST_POWER_FAILS_TIMER_ID,
    TRACK_CUST_LJDW_OVERTIME_1MIN_TIMER_ID,
    TRACK_CUST_LJDW_OVERTIME_2MIN_TIMER_ID,
    TRACK_CUST_GPS_WORK_DJDW_TIMER_ID,
    TRACK_CUST_GPSUP_SEND_TIMER_ID,
#if defined(__GERMANY__)
    TRACK_CUST_PREHET_SYSTEM_TIMER_ID,
#endif

#if defined (__CUST_RS485__)
    TRACK_CUST_READ_VEHICLE_STATUS_TIMER_ID,
    TRACK_RS485_HW_TEST_TIMER_ID,
#endif /* __CUST_RS485__ */

    TRACK_CUST_FAKECELL_FLAG_TIMER_ID,
    TRACK_CUST_FAKECELL_DISCONNECT_GPRS_TIMER_ID,
    TRACK_CUST_FAKECELL_LOW_SPEED_DISABLE_TIMER_ID,
    TRACK_CUST_FAKECELL_DISABLE_AUTO_RESUME_TIMER_ID,

    TRACK_CUST_SEND_OPEN_SHELL_SMS_TIMEOVER_TIMER_ID,

    TRACK_CUST_DEVICE_SLEEP_TIME_COUNT_DOWN_TIMER_ID,
    
    TRACK_CUST_PRE_REMOVEL_SMS_FILTER_TIMER_ID,
    TRACK_CUST_OPEN_SHELL_SMS_FILTER_TIMER_ID,
    TRACK_CUST_LED_FIRST_PWN_TIMER_ID,
    TRACK_CUST_KEY_SHORTPRESSFUN_TIMER_ID,
    TRACK_CUST_PWROFF_TIMER_ID,
    TRACK_CUST_INTO_DEEP_SLEEP_MODE_TIMER_ID,
    TRACK_CUST_FALL_TIMER_ID,
    TRACK_CUST_SDFIND_TIMER_ID,
    TRACK_CUST_LED_RESERVE_TIMER_ID,
    TRACK_CUST_RESET_HALLSIG_TIMER_ID,
    TRACK_CUST_INFINITE_RING_TIMER_ID,
    TRACK_CUST_GATTS_READ_LE_MAC_TIMER_ID,
    TRACK_CUST_GSENSOR_ACC_STATUS_TIMER_ID,

    TRACK_CUST_NO_FIX_SEND_WIFI_TIMER_ID,
    
#if defined (__LUYING__)
    TRACK_CUST_NET_CONN_TIMEOUT_TIMER_ID4,
    TRACK_CUST_NET_HEARTBEAT_TIMER_ID4,
    TRACK_CUST_NET_HEARTBEAT_OVER_TIMER_ID4,
    TRACK_CUST_NET4_RESEND_TIMER_ID,
    TRACK_CUST_NET4_LOGIN_TIMEOUT_TIMER_ID,
    TRACK_CUST_NET4_LOGIN_RESEND_TIMER_ID,
    TRACK_CUST_NET_CONN_TIMER_ID4,
    TRACK_CUST_NET_STOP_CONN_TIMER_ID4,
    TRACK_LY_TIMEOUT_TEIMER_ID,
    TRACK_LY_TIMEOUT_10min_TEIMER_ID,
    TRACK_SERVER4_TIMEOUT_TIMER_ID,
#endif
#if !(defined(__GT710__))
		TRACK_CUST_OBD_UART_WRITE_ID,
		TRACK_CUST_OBD_CMD_VIN_TIMER_ID,
		TRACK_CUST_OBD_CMD_ODOMETER_TIMER_ID,
		TRACK_CUST_OBD_CMD_CLEAR_TIMER_ID,
		TRACK_CUST_OBD_CMD_VERSION_TIMER_ID,
#endif
    TRACK_CUST_BMS_WRITE_CHECK_TIMER_ID,
    TRACK_CUST_BMSCOMM_DEFAULT_CHECK_TIMER_ID,
#if defined(__BMS_PHYLION__)
    TRACK_CUST_BMS_CMD_TIMER_ID,
#endif

#if defined(__WHEEL_CHECK__)
    TRACK_CUST_CARTWHEEL_CHECK_TIMER_ID,
#endif
#if 1//defined (__NF2319_TEST__)
    TRACK_RROJECT_CONTROL_UART_WRITE_ID,
    TRACK_SADDLE_LOCK_TIME_ID,
    TRACK_FLASHLIGHT_TIME_ID,
#endif /* __NF2319_TEST__ */
#if defined(__NF2318__)
	TRACK_CUST_ELE_SW_TIMER_ID,
	TRACK_CUST_WHEEL_LOCK_TIMER_ID,
#endif
#if defined(__BMS_PHYLION__)
	TRACK_CUST_BMS_INIT_TIMER_ID,
#endif
#if defined(__NF2318__)
	TRACK_CUST_TEST_RING_TIMER_ID,
	TRACK_CUST_WHEEL_ALARM_LOCK_ELE_TIMER_ID,
	TRACK_CUST_CONTROLLER_DATA_TIMER_ID,
	TRACK_CUST_LEASE_MODE_TIMER_ID,
	TRACK_CUST_RING_OVERTIME_TIMER_ID,
	TRACK_CUST_VEHICLE_MOVING_OVERTIME_TIMER_ID,
	TRACK_CUST_SENALM_LOCK_ELE_TIMER_ID,
	TRACK_CUST_SLEEP_MODE_TIMER_ID,
	TRACK_CUST_PAGING_OVERTIME_TIMER_ID,
	TRACK_CUST_BLE_RESET_TIMER_ID,
	TRACK_CUST_HALFSLEEP_POSITION_TIMER_ID,
	
	TRACK_CUST_SENALM_LOCK_ELE_OVERTIME_TIMER_ID,
	TRACK_CUST_SENALM_VOICE_TIMER_ID,
	TRACK_CUST_SENALM_VOICE_OVERTIME_TIMER_ID,
	TRACK_CUST_AUDIO_AMPLIFIER_DELAY_TIMER_ID,
	TRACK_CUST_ONLINE_COMMAND_RESULT_DELAY_TIMER_ID,
	TRACK_CUST_BLUETOOTH_COMMAND_RESULT_DELAY_TIMER_ID,
	TRACK_CUST_ELE_LOCK_DELAY_TIMER_ID,
	TRACK_CUST_CONTROLLER_FAULT_ALARM_DELAY_TIMER_ID,
	TRACK_CUST_ONLINE_REARLOCK_DELAY_TIMER_ID,
	TRACK_CUST_ONLINE_ELELOCK_DELAY_TIMER_ID,
	TRACK_CUST_BLUETOOTH_ELELOCK_DELAY_TIMER_ID,
	TRACK_CUST_BLUETOOTH_REARLOCK_DELAY_TIMER_ID,
	TRACK_CUST_SENSOR_READ_TIMER_ID,
	TRACK_CUST_ONLINE_CMD_FIX_TIMER_ID,
	TRACK_CUST_MODE_CHANGE_POSITION_TIMER_ID,
	TRACK_CUST_HALFSLEEP_POSITION_OVERTIME_TIMER_ID,
	TRACK_CUST_WIFI_WORKMODE_INTERVAL_TIMER_ID,
	TRACK_CUST_WIFI_WHEELMOVING_OVERTIME_TIMER_ID,
	TRACK_CUST_BMS_DATA_SEND_OVERTIME_TIMER_ID,
	
	TRACK_CUST_NET_HTTP_CONN_TIMEOUT_TIMER_ID,
	TRACK_CUST_NET_HTTP_HEARTBEAT_TIMER_ID,
	TRACK_CUST_NET_HTTP_HEARTBEAT_OVERTIME_TIMER_ID,
	TRACK_CUST_NET_HTTP_RESEND_TIMER_ID,
	TRACK_CUST_NET_HTTP_LOGIN_TIMEOUT_TIMER_ID,
	TRACK_CUST_NET_HTTP_LOGIN_RESEND_TIMER_ID,
	TRACK_CUST_NET_HTTP_CONN_TIMER_ID,
	TRACK_CUST_NET_HTTP_STOP_CONN_TIMER_ID,
	TRACK_HTTP_TIMEOUT_TEIMER_ID,
	TRACK_HTTP_TIMEOUT_10min_TEIMER_ID,
	TRACK_SERVERHTTP_TIMEOUT_TIMER_ID,
	TRACK_HTTP_DOWN_LOAD_DELAY_TIMER_ID,
	TRACK_BT_RESET_OVERTIME_TIMER_ID,
	TRACK_LED_SLEEP_OVERTIME_TIMER_ID,
	TRACK_HTTP_AUDIO_FILE_REQUEST_OVERTIME_TIMER_ID,
	TRACK_LOGIN_DATA_SEND_DELAY_TIMER_ID,
	TRACK_CUST_VEHICLE_LEASE_OFF_NO_SHOCK_TIMER_ID,
	TRACK_CUST_LEASE_OFF_NO_ALARM_TIMER_ID,
	TRACK_CUST_HTTP_IS_DOWNLOAD_FILE_TIMER_ID,
	TRACK_CUST_CHARGEIN_POWERON_OVERTIME_TIME_ID,
	TRACK_CUST_SPEED_LIMIT_SILENCE_TIME_ID,
	TRACK_CUST_SPEED_LIMIT_MAXTIME_TIME_ID,
	TRACK_CUST_UNREAD_SMS_CHECK_ONE_TIMER_ID,
	TRACK_CUST_UNREAD_SMS_CHECK_TWO_TIMER_ID,
	TRACK_CUST_UNREAD_SMS_CHECK_THREE_TIMER_ID,
	TRACK_YANCHI_THREE_MINE,
	TRACK_YANCHI_THREE_MINE_CHONGQI,
	TRACK_YANCHI_THREE_MINE_CLOSESUO,
	TRACK_CUST_CONTROLLER_DATA_RECOVER_TIMER_ID,
	TRACK_CUST_CHANGE_BATTERIES_GET_BMS,//201904
	TRACK_CUST_LEASE_ON_PRECONDITION_TIMER_ID,
	TRACK_CUST_VOICE_VERSION_UPDATE_RECONNECT_TIMER_ID,
	TRACK_CUST_SET_PARAMS_TIMER_ID,
	TRACK_CUST_VOICE_GET_LIST_TIMER_ID,
	TRACK_CUST_VOICE_8F_TIMER_ID,
	TRACK_CUST_NVRAM_WRITE_DELEY_TIMER_ID,
#endif
	TRACK_CUST_ACC_ALRM_ID,//acc故障定时器
    /* Imp: Please do not modify this */
    //---
    TRACK_CUST_XC_CON_TIM, 
#if defined(__BLE_N07__)    
    TRACK_CUST_BT_DEVICE_ID_TIMER_ID,
#endif    
    MAX_NUM_OF_TRACK_TIMER,
} track_timer_enum;



/*CTIMER_IDS  核心定时器是直接调用，对时间要求高，
     只用于LED控制       --    chengjun  2012-07-25*/
typedef enum
{
    LED_Num1 = 0,
    LED_Num2,
    LED_Num3,
    LED_Num4,
    LED_Num_Max,
    LED_NULL,               //未定义的LED
} LED_Type;

typedef enum
{
    CTIMER_START = 0,
    CTIMER_ID_RED_FLASH,
    CTIMER_ID_FLASH_NUM,
    CTIMER_ID_FLASH_NUM_MAX = CTIMER_ID_FLASH_NUM + LED_Num_Max,

    CTIMER_20MIN_RESET,
    CTIMER_24HOUR_RESET,
    CTIMER_WATCHDOG_RESET,

    /* Imp: Please do not modify this */
    MAX_CTIMERS
} CTIMER_IDS;

/*****************************************************************************
 *  Struct			    数据结构定义
*****************************************************************************/
typedef struct
{
    event_scheduler *event_scheduler_ptr;
} track_context_struct;

typedef struct
{
    eventid event_id;
    kal_uint32 tick;
    kal_timer_func_ptr callback_func;
    void *arg;
} track_timer_table_struct;

typedef struct
{
    kal_uint32 delay;            //延迟时间
    kal_uint32 time;             //现在的计时
    kal_timer_func_ptr funcPtr;  //回调函数
    kal_bool param;              //带参数
    void* funcParamPtr;          //回调函数的参数指针
} ctimer_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_hisr_func_ptr app;
    kal_timer_func_ptr appext;
    void *arg;
} TaskMsgQueue_struct;


/*****************************************************************************
* Local variable            局部变量
*****************************************************************************/


/****************************************************************************
* Global Variable           全局变量
*****************************************************************************/


/*****************************************************************************
 *  Local Functions	            本地函数
*****************************************************************************/
static void track_timer_expiry_callback(void *arg);


/*****************************************************************************
 *  Global Functions            全局函数
*****************************************************************************/
void track_timer_init(void);
void track_timer_expiry(ilm_struct *ilm_ptr);

void track_start_timer(kal_uint16 timer_id, kal_uint32 period, kal_timer_func_ptr timer_expiry, void *arg);
void track_stop_timer(kal_uint16 timer_id);

void tr_start_timer(kal_uint16 timer_id, kal_uint32 delay, FuncPtr funcPtr);
void tr_stop_timer(kal_uint16 timer_id);

BOOL track_is_timer_run(kal_uint16 timer_id);

BOOL CTimer_Start(U16 timerid, kal_uint32 delay, kal_timer_func_ptr funcPtr, void* funcParamPtr);
BOOL CTimer_Stop(U16 timerid);
BOOL CTimer_IsRun(U16 timerid);

void track_os_intoTaskMsgQueue(kal_hisr_func_ptr app);
void track_os_intoTaskMsgQueueExt(kal_timer_func_ptr app, void *arg);
void track_os_task_expiry(void *local_para_ptr);
#endif  /* _TRACK_TIMER_H */
