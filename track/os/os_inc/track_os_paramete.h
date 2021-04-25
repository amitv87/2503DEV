/******************************************************************************
 * track_os_paramete.h -
 *
 * Copyright .
 *
 * DESCRIPTION: -
 *        ͨ��ͷ�ļ�
 *
 * modification history
 * --------------------
 * v1.0   2012-07-14,  jwj create this file
 *
 ******************************************************************************/

#ifndef _TRACK_PARAMETE_H
#define _TRACK_PARAMETE_H

/****************************************************************************
* Include Files             ����ͷ�ļ�
*****************************************************************************/
#include "track_os_data_types.h"
#include "track_version.h"
#include "track_os_log.h"
#include "track_project_common_define.h"

/*****************************************************************************
 *  Define			    �궨��
*****************************************************************************/
#ifdef __TRACK_DISK_NVRAM__
#define MAX_APN_LEN             100
#define MAX_PDP_ADDR_LEN        16
#define TCM_MAX_PEER_ACL_ENTRIES	 	10

#define MAX_GPRS_USER_NAME_LEN 32
#define MAX_GPRS_PASSWORD_LEN  32
#define MAX_GPRS_APN_LEN       100
#define MAX_GPRS_IP_ADDR       4 /* IP address length */
#define TRACK_MAX_URL_SIZE                 99       // URL ���������ַ���

#if defined(__NF2318__)
#define BACKUP_DATA_MAX                  500       // NVRAM�������������
#else
#define BACKUP_DATA_MAX                  1000       // NVRAM�������������
#endif
#if defined(__GT740__)
#define BACKUP_DATA_PACKET_MAX             150
#elif defined(__NF2318__)
#define BACKUP_DATA_PACKET_MAX             160
#else
#define BACKUP_DATA_PACKET_MAX             45
#endif

#define TRACK_DEFINE_SOS_COUNT              3       // ֧�� SOS ����ĸ���

#define TRACK_DEFINE_FN_COUNT               2       // ֧�� ���� ����ĸ���
#define TRACK_DEFINE_WN_COUNT               15
#define TRACK_DEFINE_PHONE_NUMBER_LEN_MAX  21       // �绰�������������
#if defined(__GT300S__)||defined(__GT370__)|| defined (__GT380__)
#define TRACK_DEFINE_FENCE_SUM              5       // ����Χ������
#else
#define TRACK_DEFINE_FENCE_SUM              1       // ����Χ������
#endif


#define NVRAM_EF_GPS_BACKUP_DATA_SIZE sizeof(BACKUP_GPS_DATA)
#define NVRAM_EF_GPS_BACKUP_DATA_TOTAL BACKUP_DATA_MAX
#define NVRAM_EF_LOGD_TOTAL     1
#define NVRAM_EF_LOGD_SIZE      sizeof(nvram_ef_logd_struct)
#define NVRAM_EF_REALTIME_SIZE sizeof(nvram_realtime_struct)
#define NVRAM_EF_DISPLACEMENT_REALTIME_TOTAL 1

#define NVRAM_EF_PARAMETER_SIZE sizeof(nvram_parameter_struct)

#define NVRAM_EF_IMPORTANCE_PARAMETER_SIZE sizeof(nvram_importance_parameter_struct)
#define NVRAM_EF_PHONE_NUMBER_SIZE sizeof(nvram_phone_number_struct)

//#define NVRAM_EF_DRV__IMP_BACKUP_SIZE   sizeof(nvram_drv_importance_backup_struct)

/*****************************************************************************
* Typedef  Enum         ö��
*****************************************************************************/
typedef enum
{
    LOG_AUTO,
    LOG_SERIAL,
    LOG_CATCHER,
    LOG_2FILE,
    LOG_MAX
} log_target_enum;

/*****************************************************************************
 *  Struct			    ���ݽṹ����
*****************************************************************************/
typedef struct
{
    kal_uint8                filename[20];
    kal_uint8                index;
    kal_uint8                not_read;
} audio_file_struct;

typedef struct
{
    kal_uint8   zone;
    kal_uint8   time;
    kal_uint8   time_min;
    kal_uint8   timezone_auto;
} nvram_gps_time_zone_struct;

typedef struct
{
    kal_uint8 sw;
    kal_uint16 up_interval_time;
} nvram_gpsup_struct;

typedef struct
{
    kal_uint8                                   sw;                   // ON/OFF��
    kal_uint8                                   car;                  // Car Type NO (1 ��OBD�� 2 ��� 4 ��� 5 ���ڣ� 6 ͨ��)
    kal_uint8                                   upload_type;          // �ϴ�����
    kal_uint8                                   hard_acceleration;    // ����������ֵ
    kal_uint8                                   rapid_deceleration;   // ����������ֵ
    char                                        ver[20];              // OBD �汾
    kal_uint16                                  upload_time;          // ��ʱ�ϱ����ݵļ��ʱ��
    kal_uint16                                  displacement;         // ��������
    kal_uint16                                  fuel_consumption;     // �����ٹ���ƽ���ͺ�
    kal_uint32                                  total_mileage;        // �궨Ĭ�������
} nvram_obd_parameter_struct;

typedef struct
{
    kal_uint16  version;                                         //1Ϊ���ף�2Ϊͨ�ã�3Ϊ��ó��4Ϊͼǿ
    kal_uint8   support_sensor;                                 //�Ƿ�֧��sensor,0Ϊ��֧�֣�1Ϊ֧��
    kal_uint8   support_WM;                                 //�Ƿ�Ϊ��ó�棬��Ҫ����apn�޸ģ�0Ϊ���ǣ�1Ϊ��
    kal_uint8   server_lock;                                  //�Ƿ�����������0Ϊ��������1Ϊ����
} app_version_struct;


/*  begin  NVRAM���ݶ�λ����   */
typedef struct
{
    kal_uint16  len;
    kal_uint8   buf[BACKUP_DATA_PACKET_MAX];
} BACKUP_GPS_DATA;

typedef struct
{
    kal_uint8           logd_module[L_MAX];
    kal_uint8           log_sw;
    log_target_enum     logd_target;
    char                log_file_name[32];
} nvram_ef_logd_struct;

typedef struct
{
    kal_uint8           sw;             // 0=��Ч  1=����
    kal_uint8           in_out;         // 0��IN,��Χ��������1��OUT��Χ��������2��Ϊ��,��/��Χ��������Ĭ��Ϊ����Χ��������
    kal_uint8           alarm_type;     // �������ͣ�0 ��GPRS��1 SMS+GPRS��
    kal_uint8           square;         // 0=Բ��  1=����
    kal_uint32          radius;         // �뾶(��)
    float               lon;
    float               lat;
    float               lon2;
    float               lat2;
} nvram_electronic_fence_struct;

typedef struct
{
    kal_uint8           sw;             // �������ٱ�����Ĭ��Ϊ�رգ�
    kal_uint8           threshold;      // �������޷�Χ��km/h����1-255 ��Ĭ�� 100km/h
    kal_uint8           alarm_type;     // �����ϱ���ʽ��0 ��GPRS��1 SMS+GPRS��
    kal_uint16          delay_time;     // ʱ�䷶Χ���룩��5-600S  ��Ĭ�� 20S��
} nvram_speed_limit_alarm_struct;

typedef struct
{
    kal_uint8           sw;                         //
    kal_uint8           alarm_type;                 // �ϱ���ʽ��Ĭ��Ϊ 1��(0=only GPRS,1=GPRS+SMS,2=GPRS+SMS+Call,3=GPRS+CALL)
    kal_uint16          delay_time_off;             // ��Χ��2-60 S,Ĭ�� 5S
    kal_uint16          delay_time_on;              // ��Χ��1-3600 S,Ĭ�� 300S
    kal_uint16          delay_time_acc_on_to_off;   // ACC ON to OFF �����ֹ����ʱ�䣬Ĭ�� 180 ��
#ifdef __NT31__
    kal_uint16          send_alarm_time;   //  �Ͽ�������ͣ���ͱ������ļ��ʱ�䣬Ĭ��Ϊ0
#endif /* __NT32__ */

} nvram_power_fails_alarm_struct;

typedef struct
{
    kal_uint8           sw;                     // �𶯱��� ��/��
    kal_uint8           alarm_type;             // (0=only GPRS,1=GPRS+SMS,2=GPRS+SMS+CALL,3=GPRS+CALL)
    kal_uint16          detection_time;         // ���ʱ�䣨�룩��Ĭ��Ϊ��10
    kal_uint16          detection_count;        // �ڼ��ʱ���ڣ��������Ĵ�����Ĭ��Ϊ��5
    kal_uint8           sampling_interval;      // ������������̼����Ĭ��Ϊ��1
    kal_uint32          alarm_interval;         // ��̱���������룩
    kal_uint32          alarm_delay_time;       // ������ʱ���룩��Ĭ��Ϊ��180
    kal_uint8           sensitivity_level;      // �����ȵȼ���Ĭ��ֵΪ��16
} nvram_vibrates_alarm_struct;
typedef struct
{
    kal_uint8           sw;                     // �𶯱��� ��/��
    kal_uint8           alarm_type;             // (0=only GPRS,1=GPRS+SMS,2=GPRS+SMS+CALL,3=GPRS+CALL)
    kal_uint8                               door_status;
} nvram_door_alarm_struct;
typedef struct
{
    kal_uint8           sw;
    kal_uint8           alarm_type;                      // (0=only GPRS,1=GPRS+SMS)
} nvram_low_power_alarm_struct;

typedef struct
{
    kal_uint8           sw;                              // SOS�����Ƿ����ã�Ĭ��Ϊ������
    kal_uint8           alarm_type;                      // SOS�����ϱ���ʽ��Ĭ��Ϊ 1��(0=only GPRS,1=GPRS+SMS,2=GPRS+SMS+Call,3=GPRS+CALL)
#if defined(__SPANISH__)
    kal_uint8           key_type;                        // 0  �̰�����һ�ξʹ�����; 1  ������ʱ��3�룩(Ĭ��)
    char                alarm_sms_head[100];             // �����Զ��屨�����Ѷ�������
#endif
} nvram_sos_alarm_struct;

typedef struct
{
    kal_uint8           sw;                              // λ�Ʊ������أ�����1���أ�0��
    kal_uint8           alarm_type;                      // 0��2��0 ��GPRS��1 SMS+GPRS��2 GPRS+SMS+�绰,3=GPRS+CALL��Ĭ��ֵ��1
    kal_uint16          radius;                          // λ�ư뾶����λ���ף�100��1000��Ĭ��ֵ��300
} nvram_displacement_alarm_struct;

typedef struct
{
    kal_uint8           sw;                              // ͣ����ʱ���أ�����1���أ�0��
    kal_uint8           alarm_type;                      // �������ͣ�0 ��GPRS��1 SMS+GPRS��
    kal_uint16          time;                            // ͣ��ʱ����5��3600���ӣ�ͣ��ʱ����Ĭ��ֵ��60
} nvram_carpark_alarm_struct;

typedef struct
{
    kal_uint8           sw;                              // ƣ�ͼ�ʻ����������1���أ�0��Ĭ�Ͽ�
    kal_uint8           alarm_type;                      // �����ϱ���ʽ��0 ��GPRS��1 SMS+GPRS��Ĭ��0
    kal_uint16          delay_time;                      // 1��3600���ӣ�ƣ�ͼ�ʻ����ʱ�䣬Ĭ��240����
    kal_uint16          delay_reset;                     // 1��3600���ӣ���ʻ�����ʱʱ�䣬Ĭ��20����
} nvram_fatigue_driving_alarm_struct;

typedef struct
{
    kal_uint8           sw;                              // GPSä������������1���أ�0��
    kal_uint8           alarm_type;                      // �����ϱ���ʽ��0 ��GPRS��1 SMS+GPRS��
    kal_uint8           in_blind_area;                   // 1��10���ӣ�����ä��������ʱ�ϱ�ʱ�䣻Ĭ��ֵ��3����
    kal_uint8           out_blind_area;                  // 1��10���ӣ��뿪ä����ʱ�ϱ�ʱ�䣻Ĭ��ֵ��1����
} nvram_blind_area_alarm_struct;

typedef struct
{
    kal_uint8           sw;                              // ON/OFF��Ĭ��ֵ��ON
    kal_uint8           alarm_type;                      // �����ϱ���ʽ��0 �� GPRS��1 SMS+GPRS��Ĭ��ֵ��1��
    kal_uint16          threshold;                       // 10��360��Ĭ��ֵ��125
    kal_uint16          recover_threshold;               // 10��360��Ĭ��ֵ��130
    kal_uint16          detection_time;                  // 1��300�룻Ĭ��ֵ��10 ��
} nvram_power_extbatalm_struct;

typedef struct
{
    kal_uint8           sw;                              // ON/OFF��Ĭ��ֵ��ON
    kal_uint8           alarm_type;                      // �����ϱ���ʽ��0 �� GPRS��1 SMS+GPRS��Ĭ��ֵ��1��
    kal_uint16          threshold;                       // 10��360��10���ķ��أ�Ĭ��115Ϊ11.5V
    kal_uint16          recover_threshold;               // 10��360��10���ķ��أ�Ĭ��120Ϊ12.0V
    kal_uint16          detection_time;                  // 1��300�룻Ĭ��ֵ��10 ��
} nvram_power_extbatcut_struct;

typedef struct
{
    kal_uint8           sw;                              // ON/OFF��Ĭ��ֵ��ON
    kal_uint8           alarm_type;                      // �����ϱ���ʽ��0 �� GPRS��1 SMS+GPRS, 2 SMS+GPRS+CALL,3 GPRS+CALL��Ĭ��ֵ��1��
} nvram_teardown_struct;
typedef struct
{
    kal_uint8           sw;                              // ON/OFF��Ĭ��ֵ��ON
    kal_uint8           sacn_timer;                      // ����ʱ��
    kal_uint16          work_timer;                      // �������ʱ��
} nvram_wifi_struct;
typedef struct
{
    kal_uint16          acc_l_threshold_vbat;                       // 10��360��Ĭ��ֵ��132
    kal_uint16          acc_h_thresholdvbat;               // 10��360��Ĭ��ֵ��135
} nvram_ext_vbat_check_acc_struct;

typedef struct
{
    kal_uint8                      mode;                 // 0/1,0 ��ʱ�ϴ���1 �����ϴ�
    kal_uint16                     interval_acc_on;      // 0��5��18000�룬ACC ON ״̬���ϴ������Ĭ��ֵ��10
    kal_uint16                     interval_acc_off;     // 0��5��18000�룬ACC OFF ״̬���ϴ������Ĭ��ֵ��0
    kal_uint16                     distance;             // 0��50��10000�ף�������룻Ĭ��ֵ��300
} nvram_gps_report_mode_struct;

typedef struct
{
    kal_uint16                     detection_time;       // ���ʱ�䣨�룩��Ĭ��Ϊ��10
    kal_uint16                     detection_count;      // �𶯼���GPS�����������ޣ��ڼ��ʱ���ڣ��������Ĵ�����Ĭ��Ϊ��3
    kal_uint8                      sampling_interval;    // ������������̼����Ĭ��Ϊ��1
} nvram_vibrates_start_gps_struct;

typedef struct
{
    kal_uint8                      sw;                   // �յ㲹������/�ر�
    kal_uint8                      angleOfDeflection;    // 30��180�ȣ�Ĭ��ֵ��60�ȣ�ƫת�Ƕ�
    kal_uint8                      detection_time;       // 2��10�룬Ĭ��ֵ��5�룬���ʱ��
} nvram_gps_anglerep_struct;

typedef struct
{
    kal_uint8                      sw;                   // �����ϴ� ����/�ر�
    kal_uint16                     distance;             // 0��50��10000�ף�������룻Ĭ��ֵ��300
} nvram_gps_fixed_distance_struct;

typedef struct
{
    kal_uint8                      sw;                   // ��ʱ�ϴ� ����/�ر�
    kal_uint16                     interval_acc_on;      // 0��5��18000�룬ACC ON ״̬���ϴ������Ĭ��ֵ��10
    kal_uint16                     interval_acc_off;     // 0��5��18000�룬ACC OFF ״̬���ϴ������Ĭ��ֵ��0
    kal_uint16                     lbs_interval;         // ���浥λ ��
    kal_uint16                     mode1_gps_time;       // gt300ģʽһGPSʱ����
} nvram_gps_fixed_time_interval_struct;

typedef struct
{
    kal_uint8                            static_filter_sw;              // ��̬���ݹ��˿���
    kal_uint16                           static_filter_distance;        // ��̬������С����(��)Ĭ��ֵ:100��
    kal_uint8                            ephemeris_interval;            // GPS ��������ʱ�����ã�60��240���ӣ�Ĭ��ֵ��60����
    kal_uint8                            accrep;                        // ACC״̬�仯�����ϱ����ܿ���/�ر�
    kal_uint8                            location_timeout;              // ���ǳ�ʱ(��)
    nvram_vibrates_start_gps_struct      vibrates;                      // �𶯻���GPS������
    nvram_gps_anglerep_struct            anglerep;                      // �յ㲹��
    nvram_gps_fixed_distance_struct      Fd;                            // �����ϴ�
    nvram_gps_fixed_time_interval_struct Fti;                           // ��ʱ�ϴ�
} nvram_gps_work_struct;

typedef struct
{
    kal_uint8                      sw;                   // ���ͳ�� ����/�ر�
    kal_uint32                     mileage;              // ��� km/h
    kal_uint32                     K;              //  ϵ��k��Ĭ��1000

} nvram_mileage_statistics_struct;

typedef struct
{
    char                           password[20];         // �û�����
    char                           superCmd[20];         // ����ָ��ͷ
    kal_uint8                      password_sw;          // �������ָ�/��
    kal_uint8                      Permit;               // SOS�����ĺ���Ȩ�޿��ƣ�Ĭ��Ϊ0
    kal_uint8                      SOSPermit;            // �����Ĳ�ѯ����Ȩ�ޣ�Ĭ��Ϊ0
    kal_uint8                      AlarmNumCount;        // �����ķ��ͺ��������Ĭ��Ϊ1
    kal_uint8                      MonPermit;            // ����Ȩ�޿��ƣ�Ĭ��Ϊ2
} nvram_Permission_settings_struct;

typedef struct
{
    kal_uint8                      sw;                   // GPRS��������
    kal_uint8                      link;                 // GPRS��������
} nvram_GPRS_Obstruction_struct;

typedef struct
{
    kal_uint8                      sw;                   // ON/OFF��SENSOR ��⾲ֹ�ϱ����λ�ÿ��أ�Ĭ��ֵ��OFF
    kal_uint16                     detection_time;       // 10��300 �룻ʱ�䷶Χ��Ĭ��ֵ��180 ��
    kal_uint16                     speed;                // �˶��жϵ���С�ٶȣ�Ĭ��ֵ��6km/h
    kal_uint16                     count;                // 0��100 �������ĸ���
} nvram_staticrep_struct;

typedef struct
{
    kal_uint8                      sw;                   // ON/OFF��ä����������
    kal_uint16                     blind_in_time;        // gps�˳�ä��֮��gps��λ����ä����ñ���
    kal_uint16                     blind_out_time;       // gps�ڽ���ä��֮��gps��λ�Ƴ�ä����ñ���
    kal_uint8                      alarm_type;           // M=0 ��GPRS;M=1 SMS+GPRS��Ĭ���ϱ���ʽΪ1//
} nvram_blind_struct;

typedef struct
{
    kal_uint8                      sw;                   // ON/OFF��������������
    kal_uint8                      alarm_type;           // M=0 ��GPRS;M=1 SMS+GPRS��Ĭ���ϱ���ʽΪ1//
} nvram_boot_struct;

typedef struct
{
    kal_uint8                      sw;                   // ON/OFF��α��վ����
    kal_uint8                      alarm_type;           // M �ϱ���ʽ
    kal_uint16                     alarm_filter;         //α��վ��־��Чʱ�䣬��λ ��
} nvram_FCFALM_struct;

typedef struct
{
    kal_uint8                      sw;                   // ON/OFF��
    kal_uint8                      mode;                 // ���ģʽ��
    kal_uint32                     lock_on_time;         // ÿ������ʱ�䣻Ĭ��ֵ��3000 ����
    kal_uint32                     lock_off_time;        // ÿ�����ͷŵļ��ʱ�䣬Ĭ��ֵ��40����
    kal_uint32                     lock_continue_time;   // ���ֶ�����֮ǰѭ����������ʱ�䣬Ĭ��ֵ��20��
} nvram_motor_lock_struct;

typedef struct
{
    kal_bool                       custom_apn_valid;
    char                           apn[MAX_GPRS_APN_LEN];
    char                           username[MAX_GPRS_USER_NAME_LEN];
    char                           password[MAX_GPRS_PASSWORD_LEN];
} nvram_GPRS_APN_struct;

typedef struct
{
    kal_uint8   url[TRACK_MAX_URL_SIZE];
    kal_uint8   conecttype;       // 1=domain ;0=IP
    kal_uint8   server_ip[4];     // ʮ����
    kal_uint16  server_port;      // ʮ����
    kal_uint8   soc_type;         // 0=TCP;1=UDP
    kal_uint8   server_type;      // 1=��̬����
    kal_uint8   lock;             // �����޸�
    kal_uint8   ip_update;        // ip����
} nvram_server_addr_struct;
typedef struct
{
	kal_uint8 url[TRACK_MAX_URL_SIZE];//�ӷ�������ȡ֮���http����
	kal_uint8 isdownload;//�Ƿ��Ѿ�������� 0-��� 1-δ���
	kal_uint8 filestyle;//�̼���������mp3���� 1- mp3  2 �̼�
	kal_uint32 file_length;//�ļ��ܴ�С
	kal_uint32 load_length;//�����ش�С
	kal_uint8 etag;//�Ƿ����
	kal_uint8 voice_Id[3];//����ID
}nvram_http_server_url_struct;
#if 1
typedef struct
{
	kal_uint8 isdownload;//�Ƿ��Ѿ�������� 0-��� 1-δ���
	kal_uint8 filestyle;//�̼���������mp3���� 1- mp3  2 �̼�
	kal_uint32 file_length;//�ļ��ܴ�С
	kal_uint32 load_length;//�����ش�С
	kal_uint8 etag;//�Ƿ����
	kal_uint8 voice_Id[3];//����ID
}nvram_http_server_struct;
#endif
typedef struct
{
    kal_uint8                      sw;                   // ON/OFF��
    kal_uint8                      at_least_pack;        // GPS��λ���ݴ������������Ҫ�����������2��25��Ĭ��ֵΪ��10
    kal_uint8                      batch_paket;          // һ�����ݷ������������1��25��Ĭ��ֵΪ��10
} nvram_group_package_struct;

typedef struct
{
    kal_uint8                      sw;                   // ON/OFF��
    kal_uint8                      last_position_valid;  // �Ƿ��Զ�ע����һ�ζ�λ��
    kal_uint8                      server_mode;          // 0 ��Ӧ����ͬ��������͸�����ݷ�ʽ���䣻 1 ������������������ַ����
    kal_uint32                     next_update_time;     // �����������´θ���ʱ��
} nvram_agps_struct;


typedef enum
{
    WORK_MODE_1 = 1,                //��ʱ��λ����ģʽ
    WORK_MODE_2,                    //���ܶ�λģʽ->Ĭ��ģʽ
    WORK_MODE_3,                    //�������ģʽ
    WORK_MODE_4,                    //Tracker����ģʽ
    WORK_MODE_MAX          // 4
} track_enum_work_mode;

typedef struct
{
    kal_uint8                   sw;                  // ON/OFF��
    track_enum_work_mode        mode;                // �ն˹���ģʽ
    //ע  GT370 -- chengjun  2017-04-11
    //1=Ϊ�������ģʽ1��T>120s
    //2=Ϊ�������ģʽ1��T<120s
    //3=Ϊ�������ģʽ2����ʱ��������������
    kal_uint16          mode3_timer;                 // ���ʱ�䣬����Ϊ��λ
    kal_uint16          datetime;                    //��ʼʱ��
    kal_uint16          mode1_timer;                 // ģʽ1�ļ��ʱ�䣬����Ϊ��λ
    kal_uint8           teardown_mode;               //����ģʽ/����ģʽ���ܽ���ǽ����쳣��
    kal_uint8           teardown_time;               //����ʱ�䣬�쳣ģʽ����ʱ��
} nvram_work_mode_struct;

typedef struct
{
    kal_uint8                      sw;                   // ON/OFF��������������
    kal_uint8                      simalm_type;           // M=0 ��GPRS;M=1 SMS+GPRS��Ĭ���ϱ���ʽΪ0
    kal_uint8                      sim_imsi[16];
} nvram_sim_alm_struct;

typedef struct
{
    kal_uint8                      sw;                   // ON/OFF��������������
    kal_uint8                      type;           // 0����ָ�1����ָ��
    kal_uint8                      Number[TRACK_DEFINE_PHONE_NUMBER_LEN_MAX];
} nvram_smstc_struct;

typedef struct
{
    kal_uint8                      sw;                  // ON/OFF
    kal_uint16                     t1;                  // �ϴ������10~3600�룬Ĭ��60�룻
    kal_uint16                     t2;                  // GPS����λ����LBS��������10-3600�룬Ĭ��60�룻
} nvram_lbson_struct;

typedef struct
{
    char                           wifiname[20];
    char                           wifipasw[20];
} nvram_wifi_param_struct;

typedef struct
{
    kal_uint8                      mode;                // ��ҵ�Ƭ������ģʽ
    kal_uint16                     mode1_par1;          // ����ģʽ1�µĲ���
    kal_uint16                     mode1_par2;          // ����ģʽ1�µĲ���
    kal_uint16                     mode2_par1;          // ����ģʽ2�µĲ���
    kal_uint16                     mode2_par2;          // ����ģʽ2�µĲ���
    U32                            mode3_par1;
    kal_uint16                     mode3_par2;          // ����ģʽ3�µĲ���
    U8                             mode4_par1;
    kal_uint16                     mode4_par2[12];          // ����ģʽ4�µĲ���
    kal_uint32                     mode1_time;          // ģʽһ��������ʱ��
    kal_uint8                      premode;                // ��¼����ǰ��ģʽ
} nvram_ext_chip_struct;

typedef struct
{
    kal_uint8 gsm_volt_low_calibration_count;// ��Ƭ������С��2.85V��ѹ����
    kal_uint8 gsm_calibration_status;// У׼��־
    kal_uint8 bat_low_time;//����2.75�Ĵ���
    float comsultion_ele;// ���ĵĵ���
    U8 first_calibration_ele;//��ѹУ׼�ĵ����ٷֱ�
    kal_uint32 bat_lowtimep;// ����2.75ʱ��
    kal_uint32 bat_syntimep;// ����2.75ʱ��
    kal_uint32 gsm_calibration_time[5];// У׼��ѹ��ʱ��
} track_work_time;

typedef struct
{
    kal_bool gps_fix_sw;//gps��λ����
    kal_bool wf_lbs_fix_sw;//WF��λ����
    kal_uint8 fix_level;//��λ���ȼ���Ĭ��1 GPS>WF
} track_fixmode_struct;

typedef struct
{
    kal_uint8 sw;
    kal_uint16 mode1_startTime;
    kal_uint16 mode1_endTime;
    kal_uint16 mode2_sensorOutTime;//�𶯳�ʱʱ��������
} track_fly_mode_struct;

typedef struct
{
    U8 sw;
    U8 alarm_type;    // �����ϱ���ʽ��0 �� GPRS��1 SMS+GPRS, 2 SMS+GPRS+CALL,3 GPRS+CALL��Ĭ��ֵ��1��
} nvram_drop_alarm;

typedef struct
{
    kal_uint8                    sw;  // ON/OFF����Ϣ���俪�أ�Ĭ�Ͽ�
    kal_uint16                    time;          //�ϴ������Ĭ��60S
} nvram_info_up_struct;

#if defined(__BMS_PHYLION__)
typedef struct
{
	kal_uint8 otherData1;//��λ bit7 Ԥ�� bit6 ת�ѹ��� bit5 ����������� bit4 Ƿѹ���� bit3 ���ɳ����� bit2 ��ת���� bit1 ���״̬��1 ��������״̬�� bit0 ɲ�� ��1 ����ɲ��״̬�� 
	kal_uint8 otherData2;//��λ bit2 Ԥ�� bit1 ������źŻ��� bit0ECU����ʹ�ܻ��� ��1 �����������������綯 ������ �綯������� bit7 1 0 1 bit6 0 1 1 bit5~bit3 ������ת������״̬���� 90% 85% 80% 75% 70% bit5 1 1 0 0 0 bit4 0 0 1 1 0 bit3 1 0 1 0 1 
	kal_uint8 voltageH;//�����仯���ֽ�
	kal_uint8 voltageL;//�����仯���ֽ�
	kal_uint8 relativeCapacity;//�������
	kal_uint8 currentH;//ʵʱ�������ֽ�
	kal_uint8 currentL;//ʵʱ�������ֽ�
	kal_uint8 leftCapatiryH;//ʣ���������ֽ�
	kal_uint8 leftCapatiryL;//ʣ���������ֽ�
	kal_uint8 absoluteCapacity;//���������ٷֱ�
	kal_uint8 absoluteFullCapacity[2];//�����������
	kal_uint8 circulationOfBat[2];//�����ѭ������
	kal_uint8 valtage[28];//���1-14�ŵ�ص�ѹ��ѹ
	kal_uint8 totalValtage[2];//������ܵ�ѹ
	kal_uint8 temperature[2];//������ڲ��¶�
	kal_uint8 chargeInterval[2];//��ǰ�����ʱ��
	kal_uint8 largeChargeInterval[2];//�������ʱ��
	kal_uint8 barCode[20];//��Ʒ������//1.2.0��20��16λ�ļ������
	kal_uint8 versionNum[2];//�汾��
	kal_uint8 valtageManufacturer[16];//������쳧��
	kal_uint8 relativeCapacityPer;//��������ٷֱ�
	kal_uint8 absoluteCapacityPer;//���������ٷֱ�
}controller_data_struct;
typedef struct
{
	kal_uint8 brake;//ɲ��
    kal_uint8 ele_sta;//���״̬
    kal_uint8 locked_rotor;//��ת����
    kal_uint8 flying_car;//���ɳ�����
    kal_uint8 uder_voltage;//��Ƿѹ����
    kal_uint8 holzer_fault;//�����������
    kal_uint8 malfunction;//ת�ѹ���
    kal_uint8 locked_ele;//������ź�
    kal_uint8 turn_limit;//ת������״̬
    kal_uint8 con_mode;//������ģʽ(�綯�����綯)
    kal_uint8 ecu_enable;//ecu����ʹ��״̬
    kal_uint8 holzer_stah;//0.5 �������������仯�ĸ������ֽ�
    kal_uint8 holzer_stal;//0.5 �������������仯�ĸ������ֽ�
}vehicle_status_struct;
#endif
#if defined(__NF2318__)
typedef struct
{
	kal_uint16 deviceID;//�豸ID
    //kal_uint8  SequenceNumber[11];                                 //�ն����к�
    kal_uint8  rcv;//����Ƿ�ʧ���� Ĭ��ֵ99
	kal_uint8 publickey[128];//�豸��Կ
	kal_uint8 TEA_KEY[16];//XxTEA������Կ
	kal_uint8 dfttimeout;//������Ӧ��ʱʱ������ 1-60
	kal_uint8 dftlockvoiceplayt;//����������ʱ��������
	kal_uint8 sleept;//��������ʱ������
	kal_uint8 volume;//����������С����
	kal_uint16 xctime;//Ѱ����������ʱ��
	kal_uint8 btsw;//��������
	kal_uint8 diameter;//����ֱ������
	kal_uint8 alnicounum;//���õ���Ÿֶ���
	kal_uint8 lockdomain;//������������
	kal_uint8 alarmreq;//�������
	kal_uint8 dftoutpoffiltert;//��������ʱ������
	kal_uint16 viblmg;//�������� mg 0-2000
	kal_uint8 senspkset;//����������������
	kal_uint8 wheelmovtime;//�ֶ��������
	kal_uint8 wheelmov;//�ֶ��ж�����  �������
	kal_uint32 dormantfreq;//�����߼��ʱ�� 
	char       bt_name[20];//��������
#if defined(__EL100_XYT__)
    kal_uint8 auth_len;//��Ȩ�볤��
    kal_uint8 auth_code[32];//��Ȩ��
#endif
}ste_parameter_struct;
typedef struct
{
	kal_uint8 ele;//���״̬ 0  ���� 1 ��
	//kal_uint8 wheel;//������״̬0 ���� 1�� �������
	kal_uint8 lease;//����״̬// 1���� 0����
	kal_uint8 findcar;//Զ��Ѱ������ 0 �ر� 1 ��
	kal_uint8 vechile_sw;//�������أ�ģ���һ��
	//kal_uint8 exbatlock;//�����״̬ 0 ���� 1 �� �������
	kal_uint8 sleep;//���� 0 ������ 15������  16 ȫ����
	kal_uint8 encode_type;//�Ƿ����  1 �����ܣ�0 ����
	kal_uint8 pwrsave;//��άģʽ 1 ���� 0 �˳�
	kal_uint8 btadvtype;//���Դ����ã�1 ��̬ʹ��sn�Ź㲥  0 д��700000001�㲥 Ĭ�� 1
}ste_status_struct;//�ն�״̬
typedef struct
{
	kal_uint8 con_mode;//�綯��ģʽ
	kal_uint8 turn_limit;//ת������
	kal_uint8 locked_ele;//������ź�
	kal_uint8 soc;//��������ٷֱ�
	kal_uint32 totalmileage;//�����ͳ��
	kal_uint32 oncemileage;//�������
}ele_data_struct;
typedef struct
{
	kal_uint8 isload;//�Ƿ��Ѿ�����
	kal_uint8 voice_id[3];//����ID
}voice_id_struct;//�����ļ�ID

typedef struct
{
	voice_id_struct void_id[100];//��д��20�������ļ� ���ӵ�100�� Liujw 2019-7-19
}voice_list_struct;//Ҫ���ص�����ID�б�
typedef struct
{
	kal_uint8 osdswitch;//���� Ĭ�� 1
	kal_uint8 osdspeed;//ʱ�� 0-60 Ĭ��15KM/H
	kal_uint8 osdtimeinterval;//�������Ѽ�� 0- 60 Ĭ�� 3fenzhong
	kal_uint8 osdmaxstatus;//�Ƿ񳬹���ʱ��
	kal_uint16 osdtimemax;//������ʱ��0-1800 Ĭ�� 0
	kal_uint8 osdvoiceid[3];//��������ID Ĭ�� 00000E
	kal_uint8 osdringid[3];//��������ID   Ĭ�� 00000F
}osd_struct;//���ٱ���
#endif
typedef struct
{
    char                                build_date[15];
    char                                build_time[15];
    char                                version_string[20];
    char                                version_string2[20];
    nvram_GPRS_APN_struct               apn;
    nvram_server_addr_struct            server;
    nvram_server_addr_struct            server2;
    nvram_server_addr_struct            mserver2[4];
    nvram_server_addr_struct            dserver;
#if defined(__BACKUP_SERVER__)
    nvram_server_addr_struct             bserver;
#endif
    app_version_struct                           app_version;

#if defined (__LUYING__)
    nvram_server_addr_struct            server4;    //¼���ϱ�����˵�ַ
#endif /* __LUYING__ */
#if defined(__NF2318__)
	kal_uint8 rcv;//����һ����ʶλ Ĭ��99
    kal_uint8  SequenceNumber[11];                                 //�ն����к�
    char								device_id[17];				//�豸ID��
#if defined(__EL100_XYT__)
    kal_uint8                           cs_code;                    // ���̴���
    kal_uint8                           identify[12];               // ʶ����
#endif
#endif
} nvram_importance_parameter_struct;

#if defined(__ACC_ALARM__)
typedef struct
{
    kal_bool                               accalm_sw;                            //ACC����������
    kal_uint8                              accalm_type;                          //ACC��������
    kal_uint8                              accalm_time;                          //ACC���������ʱ��
    kal_uint8                              accalm_cause;                          //ACC��������������0��ACC�仯������1��ACC��OFF������2��acc��ON���� Ĭ��0
} nvram_accalm_struct;
#endif

#if defined (__CUST_BT__)
typedef struct
{
    kal_bool                               sw;                              //BTģ�鿪��
    kal_uint8                              broadcast_time;                  //�㲥ʱ��
    kal_uint8                              listen_time;                     //����ʱ��
    kal_uint16                             sleep_time;                      //����ʱ��
} nvram_bt_struct;
#endif /* __CUST_BT__ */

typedef struct
{
    kal_uint8                      sw;                   // ON/OFF ���Ǳ���
    kal_uint8                      alarm_type;           // M=0 ��GPRS;M=1 SMS+GPRS��Ĭ���ϱ���ʽΪ0
} nvram_open_shell_alarm_struct;

typedef struct
{
    kal_uint8                                   sw;                   // ����Ѱ��ָ��ON/OFF��
    kal_uint16                                 ring_time;         //����ʱ��
    kal_uint16                                 delay_time;      //���ʱ��
    kal_uint16                                 t_time;              //����
} nvram_sdfind_struct;


typedef struct
{
    kal_uint8                                   sw;                   // ﮵����Ϣ�ϴ�ON/OFF��
    kal_uint16                                 accon_up;         //
    kal_uint16                                 accoff_up;      //
} nvram_litmer_struct;

typedef struct
{
    kal_uint8                      sw;                   // bmsͨѶģ���쳣����
    kal_uint8                      alarm_type;           // M=0 ��GPRS;M=1 SMS+GPRS��Ĭ���ϱ���ʽΪ0
    kal_uint8                      readtimer;//��ȡ���
} nvram_bmsfault_struct;

typedef struct
{
    kal_uint8                      sw;                   // ON/OFF �Ƿ����б���
    kal_uint8                      alarm_type;           // M=0 ����Ĭ���ϱ���ʽΪ0
    U16 t;                   //�����ź����ʱ��(10��)
} nvram_fride_struct;

typedef struct
{
    kal_uint8                      sw;                  // ON/OFF
    kal_uint16                     t1;                  // �ϴ������5~18000�룬Ĭ��10�룻
    kal_uint16                     t2;                  // ����ʱ��(��λ:����)��0��ʾ�����ƣ�Ĭ��5����
} nvram_sv_struct;

typedef struct
{
	kal_uint8					   sw;					//ON/OFF
	kal_uint8					   type;				//acc ��ⷽʽ��1 adc�����磬2 obd���,3 sensor��� ��Ĭ��3
}nvram_acct_struct;

#if defined (__NT13V__)
typedef struct
{
    kal_uint16 count_down_time;
}nvram_device_sleep_time_struct;
#endif

#if defined (__NF2319__)
typedef struct
{
	kal_uint8					   state;					//�������
	kal_uint8					   speed;				//�ٶȱ������ϵ�Ĭ��75%
}nvram_ctrl_struct;

#endif /* __NF2319__ */
typedef struct
{
	kal_uint8 sw;//����
	kal_uint8 interval;//�ɼ����  H Ĭ��1
}bat_temp_struct;//����¶Ȳɼ�

typedef struct
{
    char                                   sos_num[4/*TRACK_DEFINE_SOS_COUNT*/][TRACK_DEFINE_PHONE_NUMBER_LEN_MAX];
    char                                   centerNumber[TRACK_DEFINE_PHONE_NUMBER_LEN_MAX];
    char                                   url[100];                       // URL����
    kal_uint8                              monitor_delay;                  // ������ʱ����Χ��5-18S��Ĭ������ 10S
    kal_uint8                              lang;                           // 1 ��ʾΪ���ģ�0 ��ʾΪӢ�ģ����ڵ�ַ��ѯ
    kal_uint8                              gprson;                         // 1 ����GPRS   0 �ر�GPRS
    kal_uint32                             hbt_acc_on;                     // ���������ü��(����)  ACC ON �ϴ����
    kal_uint32                             hbt_acc_off;                    // ���������ü��(����)  ACC OFF �ϴ����
    kal_uint8                              callmode;                       // �������巽ʽ 0 ���� 1�� 2����+��  3����
    kal_uint8                              redial_count;                   // �绰�ز�����
    kal_uint8                              monitoring_delay;               // ������ʱ��0��5��18�룻����Զ�̼�����ʱ��Ĭ��ֵΪ��10��
    kal_uint8                              call_get_addr_sw;               // ��������Զ����ص�ַ��1 ��Ч�� 0 ���ã�
    kal_uint16                             sensor_gps_work;                // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
    kal_uint8                              led_sleep;                      // LED���ߣ�1 (������Ч����GPS���ذ󶨹���ͬ��ͬ��)��2 (��������GPS�����޹�), 3 (����)
    kal_uint16                             defense_time;                   // ��ʱ�������ʱ�䣬Ĭ��ֵΪ��10����(600��)
    kal_uint8                              oil_sw;                         // ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
    nvram_gps_work_struct                  gps_work;                       // GPS����ģʽ
    nvram_vibrates_alarm_struct            vibrates_alarm;                 // �𶯱���
    nvram_power_fails_alarm_struct         power_fails_alarm;              // �ϵ籨������
    nvram_speed_limit_alarm_struct         speed_limit_alarm;              // ���ٱ�������
    nvram_Permission_settings_struct       Permission;                     // Ȩ������
    nvram_sos_alarm_struct                 sos_alarm;                      // SOS��������
    nvram_blind_struct                     gps_blind;                      // GPSä������
    nvram_staticrep_struct                 staticrep;                      // ��ֹ�ϱ����λ�ÿ���ָ��
    nvram_displacement_alarm_struct        displacement_alarm;             // λ�Ʊ���
    nvram_agps_struct                      agps;                           // AGPS
    nvram_boot_struct                      boot;                           // ������������
    nvram_group_package_struct             group_package;                  // �������
    nvram_mileage_statistics_struct        mileage_statistics;             // ���ͳ��
    nvram_carpark_alarm_struct             carpark_alarm;                  // ͣ����ʱ����
    nvram_gps_time_zone_struct             zone;                           // ʱ������
    nvram_low_power_alarm_struct           low_power_alarm;                // �͵籨������
    nvram_fatigue_driving_alarm_struct     fatigue_driving_alarm;          // ƣ�ͼ�ʻ����
    nvram_electronic_fence_struct          fence[TRACK_DEFINE_FENCE_SUM];  // ����Χ������
    nvram_motor_lock_struct                motor_lock;                     // �綯�������
    nvram_work_mode_struct                 work_mode;                      // �ն˹���ģʽ
    nvram_power_extbatalm_struct           power_extbatalm;                // �ⲿ��Դ�͵籨��
    nvram_power_extbatcut_struct           power_extbatcut;                // �ⲿ�͵��жϱ���
    nvram_GPRS_Obstruction_struct          gprs_obstruction_alarm;         // GPRS ��������
    nvram_door_alarm_struct                door_alarm;                     // �ű���
    nvram_obd_parameter_struct             obd;                            // OBD
    nvram_ext_vbat_check_acc_struct        vbat_check_acc;                 // GT500��ѹ���acc����
    kal_uint8                              accalm_sw;                       // 0 ��ʾΪACC�л��޶���֪ͨ��1 ��ʾΪACC�л��ж���֪ͨ
    kal_uint8                              speed_limit;                     // �綯�����ٷ�ֵ
    nvram_boot_struct                      pwroff;                          // �ػ���������
    nvram_sim_alm_struct                   sim_alarm;                       // ��������
    kal_uint8                              pwrlimit_sw;                     // ���ƹػ�����,ֻ�����п��ػ�������Ŀ
    kal_uint8                              lcd_on_time;                     // GW100LCD�Զ�����ʱ��
    kal_int32                              adc4_offset;                     // ADC4����ֵ
    kal_uint16                             mode3_work_interval;             // ����ģʽ�¶�ʱ�ϱ��������
    kal_uint8                              mode3_up_counts;                 // ����ģʽ�µ��ι����ϱ�����
    nvram_ext_chip_struct                  extchip;                         // extchip
    nvram_wifi_param_struct                wifi_param;                      // ap �û���+����
    nvram_smstc_struct                     smstc;                           // 1 ����smstc   0 �ر�smstc
    kal_uint8                              defense_mode;                    // �Զ����ģʽ X=0  �Զ����ģʽ��X=1 �ֶ����ģʽ��2 Ϊң�ر�����Ĭ��X=1
    nvram_info_up_struct                   info_up;                         // (���׹���)��ӵ�Դ��ѹ��Ϣ�ϴ�����
    nvram_teardown_struct                  teardown_str;                    // ���𱨾�,Ĭ�Ͽ���,������ʽ1,SMS+GPRS
    nvram_wifi_struct                      wifi;
    nvram_lbson_struct                     lbson;                           // LBS��������
    nvram_FCFALM_struct                    fcfalm;                          // α��վ����
    kal_uint8                              flycut;                          // ����ģʽ����
    kal_uint8                              paramFileCrc[2];                 // �����������ò����ļ���MD5
    kal_uint8                              dwd_epo_sw;                      // EPO���ؿ���
    kal_uint8                              PW5;                             // 5V��Դ���״̬
#if defined(__ACC_ALARM__)
    nvram_accalm_struct                    accalm;                          // 06F���ư棬ACC����
#endif
#if defined(__GT740__)
    track_work_time         cal_wk_time_struct;
    kal_uint8    new_position_sw;   //�����¶�λ���ϴ����� Ĭ�Ϲ�
    kal_uint8    cal_time_sw;    //�����������㿪�أ�Ĭ�Ͽ�
    nvram_gpsup_struct             gpsup_struct;    //��̬����
#endif

    kal_uint8 fake_cell_enable;     //��α��վʹ��
    kal_uint8 fake_cell_to_ams;     //α��վ��Ϣ�Ƿ��ϱ�AMS

    kal_uint8 gpsDup;               //gpsλ�������ϱ�����

#if defined (__CUST_BT__) && defined(__NT33__)
    nvram_bt_struct bt;
#endif /* __CUST_BT__ */
    kal_uint8    gpswake;  //����GPS Ĭ��0
#if defined (__GT370__)|| defined (__GT380__)
    nvram_open_shell_alarm_struct open_shell_alarm;
#endif /* __GT370__ */
    kal_uint8 fake_cell_auto_temp;  //��ʱ�Զ����ط�α���� 0=��ʱ�رգ�����ָ���0xFF=�ָ��󲻴���
#if defined(__SPANISH__)
    kal_uint8 itp_sw;                //94�����俪��
#endif
    track_fixmode_struct fixmode;//��λ���ȼ�
    nvram_sv_struct                         sv;                             //������Ϣ�ϴ�����
    kal_uint8                               sw_nbver;                       //�޵�ذ汾����ָ��
#if defined(__DROP_ALM__)
    nvram_drop_alarm drop_alm;  //��������Ĭ�Ͽ�
#endif
    kal_uint8  jc_mode;//0 ˫��ͨ���� 1 ����  Ĭ��ֵ��0
    kal_uint8  answer;// �������� 0���Զ����� 1
#if defined (__EL100__)
    char                                    ecrckey[50];                          //�����ܳ�
    kal_uint16                          reserve;                            //һ��ԤԼʱ��
    nvram_sdfind_struct           sdfind;                               //����Ѱ��ָ��
    nvram_fride_struct        sfride;
#endif /* __GB100__ */
#if defined(__SMT_BMS__)
    nvram_litmer_struct    Litimer_struct;//Ĭ��on 
    nvram_bmsfault_struct   Bmsfault_struct;//Ĭ��on 
#endif
#if defined(__NT55__)
	nvram_acct_struct 		acct;	////acc ��ⷽʽ��1 adc�����磬2 obd���,3 sensor��� ��Ĭ��3
#endif

#if defined (__NT13V__)
    nvram_device_sleep_time_struct  device_sleep_time;
#endif
#if defined (__NF2319__)
    nvram_ctrl_struct        ctrl;      //��������������
#endif /* __NF2319__ */
#if defined(__NF2318__)
	ele_data_struct		ele_data;//����������洢
	ste_parameter_struct 			    ste_param;//7������
	ste_status_struct				ste_status;//�ն�״̬
#endif
	bat_temp_struct		bat_temp;//����¶Ȳɼ�
	osd_struct			osd;//���ٱ���
	kal_uint8			voice_ver[4];//�����汾
	kal_uint8			bwtopen;//���������� ��λ��*10
	kal_uint8			bwtclose;//����������
	kal_uint8			eletopen;//���������
	kal_uint8			eletclose;//���������
	kal_uint8			BMSOC;//����bms��ʼ���Ŀ��أ�0-��  1-��
	kal_uint8			viblvoicesw;//�����������أ�1-��0-��
	kal_uint32          voicefilesize;//�Ѿ����ص������ļ���С
	kal_uint8             encode_token[16];//20190717
	kal_uint8        bt_token_mark;
	kal_uint8       encode_token_s[50];//���������ַ�������
	kal_uint8       new_encode_token_s[50];//���������ַ�������
} nvram_parameter_struct;

typedef struct
{
    char                                   fn_num[TRACK_DEFINE_FN_COUNT][TRACK_DEFINE_PHONE_NUMBER_LEN_MAX];
    char                                   white_num[TRACK_DEFINE_WN_COUNT][TRACK_DEFINE_PHONE_NUMBER_LEN_MAX];
    kal_uint8                              almrep[TRACK_DEFINE_SOS_COUNT];
	//voice_list_struct voice_list;//�洢��Ҫ���ص������б�
	//kal_uint8           voice_max;//��Ҫ���ص�ԭ��������
} nvram_phone_number_struct;

typedef struct
{
    // nvram_importance_parameter_struct  importance_parameter;
    // char                                   sos_num[4/*TRACK_DEFINE_SOS_COUNT*/][TRACK_DEFINE_PHONE_NUMBER_LEN_MAX];
    // nvram_ext_vbat_check_acc_struct        vbat_check_acc;                 // GT500��ѹ���acc����
//   kal_int32                              adc4_offset;        // ADC4����ֵ
    //  kal_uint16                             sensor_gps_work;                // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
    //   kal_uint8       defense_mode;                               // �Զ����ģʽ X=0  �Զ����ģʽ��X=1 �ֶ����ģʽ��2 Ϊң�ر�����Ĭ��X=1
    //  nvram_phone_number_struct   PHONE_NUMBER;
    kal_uint8 data[2000];
} nvram_drv_importance_backup_struct;


typedef struct
{
    kal_uint8 fake_cell_flg;
    //0=����,�ޱ���
    //1=��⵽α��վ��Ϣ����Ҫ�����ж�GPRS
    //2=���ѵ�α��վ����Чʱ����GPRS����10�����ϣ����Ա����� ���������ٻָ�0
    //chengjun  2017-02-15

    //�ѵ�α��վ��ʱ�䡢�ص㻺�浽AMS,����4��
    kal_uint8 fake_cell_count;
    kal_uint32 alm_sec[4];
    float alm_longitude[4];
    float alm_latitude[4];

    //���һ���ѵ�α��վ�ĵص�
    float last_longitude;
    float last_latitude;

} fake_cell_info_notes_struct;


typedef struct
{
    kal_uint8       in_or_out[TRACK_DEFINE_FENCE_SUM];          // ����Χ�� ��ǰ�ǳ���Χ����(1)����Χ����(2)
    kal_uint8       lowBattery;                                 // �͵籨�����ͱ��
    kal_uint8       lowExtBattery;                              // �ⲿ��Դ�͵籨�����ͱ��
    kal_uint8       disExtBattery;                              // �ⲿ��Դ�Ͽ����ͱ��
    kal_uint8       flag;                                       // λ�Ʊ��� ���
    float           longitude;                                  // λ�Ʊ���
    float           latitude;                                   // λ�Ʊ���
    kal_uint32      netLogControl;                              // ���������ϱ���־����
    kal_uint8       reStartSystemCount;                         // 20����������������
    kal_uint8       defences_status;                            // ���״̬  1 ������2 Ԥ�����3 ���
    kal_uint8       key_status;                                 // GT02D����״̬  0 �ر�LED��ʾ    1 ����LED��ʾ
    kal_uint8       oil_status;                                 // �͵�״̬��0 �͵�������1 �͵�Ͽ���2 ����Ͽ���GPSδ��λ��3 ����Ͽ���δ�������ӻ�ִ�еĶ��ţ�4 ����Ͽ����ѷ������ӻ�ִ�еĶ��ţ�
    kal_uint8       oil_cmd_source;                             // ���͵�ָ����Դ
    kal_uint8       acc_last_status;                            // ���ڼ�¼ACC״̬����ֹ�������״̬���ϴιػ�ǰһ��
    kal_uint8       poweroff;                                   // �͵�ػ��������ͱ��  24byte
    kal_uint8       power_saving_mode;                          // ʡ��ģʽ(0:����ģʽ;1:ʡ��ģʽ)
    kal_uint8       reconnect_send_flg;                         // �Ƿ���������������(0�����ͣ�1����)
    kal_uint8       restart_system_limited_service;             // L4C_RAC_LIMITED_SERVICE ��������
    kal_uint8       blind_status;                               // GPSä��״̬��0Ϊ����ä����1Ϊä����
    kal_uint8       defense_mode;                               // ���ģʽ�� 0 �Զ����ģʽ��   1 �ֶ����ģʽ��   2 �Զ�����ֶ�������Ĭ��1
    kal_uint8       location_switch;                            // ��λ�����ϴ�����  0 �ر�  1 ��
#if !defined(__NF2318__)
    kal_uint8       timeslot[3][8][5];
#endif
    kal_uint8       disExtBattery2;                             // �ⲿ��ѹ6v֮�±���������Ϊ1, 6v֮��Ϊ0
    kal_uint8       sim_alm;                                    // ����������־λ
    kal_uint16      flashtime;
#if defined (__LUYING__)
	kal_uint8       audio_count;                                // ��Ƶ�ļ��� Ĭ��0
    kal_uint8       record_time;                                // ʱ����Ĭ��10
    audio_file_struct audio_file[5];                            // ��Ƶ�ļ�
#endif
    kal_uint16      work3_countdowm;                            // ����ģʽ������������ʱ
    kal_uint8       server_ip[4];                               // ���������õ���IP��ַ����
    kal_uint8       oil_lock_status;                            //���͵�����״̬,0:������;1:���� V20 433ģ������
    kal_uint8       switch_down;//���𿪹�,1����,Ĭ��0
    kal_uint8       mech_sensor_count;
    kal_uint16       mech_sensor_invel_time;
#if defined(__GT740__)
    kal_uint8        tracking_mode;//׷��ģʽ
    kal_uint32        tracking_time;//׷��ģʽʱ��
    kal_uint8        tracking_status_mode;//ԭ��״̬ģʽ
#endif
    kal_uint8        glp;
    kal_uint8        up_param;
    kal_uint8       ams_log;                          //710����LOG����
    kal_uint8       ams_push;                         //710�ϴ�LOG����
    kal_uint8       TPRO_COUNT;                       //��λ��Э�����
    kal_uint8       nw_reset_sign;                    // �����������
#if defined (__BAT_TEMP_TO_AMS__)
    kal_int8        temperature_data[70][5];
    kal_uint8       temp_count;
#endif /* __BAT_TEMP_TO_AMS__ */

    kal_uint8       power_fails_flg;
#if defined(__CUST_FAKECELL__)
    fake_cell_info_notes_struct fake_cell_info_notes;   //��α��վ��Ϣ��¼
#endif
    U16  MainCellId;
    kal_uint8       nw_sign;                    // ������־
    kal_uint32      logBlocksEnd;               // log end
    kal_uint32      logBlocksBegin;             // log begin
    kal_uint8       AMS_HEX_sw_log;             // sw_log
    kal_uint16      lock_status;                 //��״̬

#if defined (__LUYING__)
    kal_uint8         record_file;//�����ļ���¼���Ŀ�ʼ����
    kal_uint8         record_up;//�ϴ���ƽ̨��¼���Ŀ�ʼ����
    kal_uint8         record_cnt;//¼���Ŀ�ʼ�ܸ���
    kal_uint8         record_stamp[6];
    kal_uint8         record_last_time;
#endif /* __LUYING__ */
#if defined(__NF2318__)//����43
	kal_uint8 		acc_alarm;//acc�澯״̬1 �澯  0  ����澯
	kal_uint8		controller_alarm;//������ͨѶ���� 1 �澯  0 ���
	kal_uint8       rearwheelalarm;//�������쳣�澯
	kal_uint8       elelockalarm;//������쳣�澯
	nvram_http_server_struct  http_server;//http�ļ����ز����洢
	kal_uint8           file_update_style;//�Ѿ��Ա���汾��Ϣ �й̼����� 00000001 ���������� 00000010  
	kal_uint8			fota_update;//�̼�������ʶ
	//kal_uint8 ele;//���״̬ 0  ���� 1 ��
	kal_uint8 wheel;//������״̬0 ���� 1��
	kal_uint8 exbatlock;//�����״̬ 0 ���� 1 ��
	kal_uint16      lock_counts;//��������
	kal_uint16      unlock_counts;//��������
	kal_uint16		exbat_lockcounts;//�������������
	kal_uint16		exbat_unlockcounts;//�������������
	kal_uint8		sleep;//��ǰ����״̬ 0 ������ 15������  16 ȫ����
	kal_uint8		httpdowncount;//httpyһ����δ���سɹ�����
	kal_uint8		httpdowndate[3];//����
	voice_list_struct voice_list;//�洢��Ҫ���ص������б�
	kal_uint8           voice_max;//��Ҫ���ص���Դ������
	kal_uint8 voice_version_tmp[4];//��ʱ�洢�����汾��
	kal_uint32      temp_voicesize;//��ʱ��ű����������ص��ܴ�С
	kal_uint8  bms_alarm[7];//bms״̬����#endif	
	kal_uint8       acc_fault_alarm;//ACC�쳣������־��1�澯//20190715} nvram_realtime_struct;
	kal_uint8   controller_alarm_type[5];
	//-------------20191010
	kal_uint8 lesea_off_to_sta;//0û�й������� 1 �����߹������� 2 �����������굫�Ǻ�����ʧ��
	kal_uint8 lesea_off_to_volume_sta;//���������Ƿ񲥷����� 0 ��һ�β���  1�Ѿ����Ź���  ������
#endif
} nvram_realtime_struct;


/*****************************************************************************
* Local variable            �ֲ�����
*****************************************************************************/


/****************************************************************************
* Global Variable           ȫ�ֱ���
*****************************************************************************/


/****************************************************************************
*  Global Variable - Extern             ����ȫ�ֱ���
*****************************************************************************/
extern nvram_parameter_struct G_parameter;
//extern nvram_realtime_struct G_realtime_data;
extern nvram_importance_parameter_struct G_importance_parameter_data;
extern nvram_phone_number_struct G_phone_number;
extern nvram_parameter_struct* OTA_NVRAM_EF_PARAMETER_DEFAULT ;
extern nvram_importance_parameter_struct* OTA_NVRAM_EF_IMPORTANCE_PARAMETER_DEFAULT  ;
extern  nvram_phone_number_struct* OTA_NVRAM_EF_PHONE_NUMBER_DEFAULT;
extern   nvram_ef_logd_struct* OTA_NVRAM_EF_LOGD_DEFAULT;
extern   nvram_realtime_struct* OTA_NVRAM_EF_REALTIME_DEFAULT ;
/*****************************************************************************
 *  Local Functions	            ���غ���
*****************************************************************************/
static void track_system_bootup_delay(void);

/*****************************************************************************
 *  Global Functions            ȫ�ֺ���
*****************************************************************************/

#endif /* __TRACK_DISK_NVRAM__ */


#endif  /* _TRACK_PARAMETE_H */


