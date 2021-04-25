/*******************************************************************************************
 * Filename:track_cust_platform_N07.h
 * Author :Liujw
 * Date :2017-07-31
 * Comment:7�ŵ絥����ƽ̨ͨѶЭ��
 ******************************************************************************************/
#if !defined(__NUM_SEVEN__)
#define __NUM_SEVEN__
/**����ͷ�ļ�**/
#include "track_cust.h"
#include "track_os_ell.h"
/*
*�ֲ���������
*/
/*
*�ṹ�嶨��
*/
typedef enum
{
    SYSTEM_RESET = 0x00,//ϵͳ������ĵ�½
    RECONNECT = 0x01, //ͨ����·���ϻָ���ĵ�½
    SERVERID_CHANGE = 0x02,//�����л���ĵ�½
    WAKEUP = 0x03,//ȫ���߻ָ���ĵ�½
    FOTA_UPDATE = 0x04,//�̼����º��½
    LOGINMAX = 99//���
} enum_login_reason; //��½ԭ��

typedef enum
{
    RESET_NO_RESION,//δ֪ԭ������
    RESET_GPS_10MIN,//gps10���������������
    RESET_NO_LOGIN, //gprs30����δ��½�ɹ�
    RESET_NO_HEARTBEAT,//δ�յ�������
    RESET_HEARTBEAT_NOTARRIVE,//��������ʱû���յ�
    RESET_REVISE_USER,//�û����޸�
    RESET_REVISE_SIM,//�ն�sim�����޸�
    RESET_RECEIVE_ACTIVATE_SMS,//�յ��������
    RESET_RECEIVE_RELOGIN_SMS,//�յ��ص�½ָ��
    RESET_RESETTING_SMS_ONLINE,//�յ��û����͵ĸ�λָ��
    RESET_SHUTDOWM_SMS_ONLINE,//�յ��û����͵Ĺػ�ָ��
    RESET_NOACTIVATE_SMS_ONLINE,//�û��·��⼤��ָ��
    RESET_APN_CHANGED,//APN���޸�ʱ
    RESET_SERVER_CHANGED,//�������޸�
    RESET_ONLINE_MODE_CHANGED,//����ģʽ�ı�
    RESET_COMM_MODE_CHANGED,//TCP��UDP�л���
    RESET_TID_CHANGED,//�ն˵�TID���޸�
    RESET_GSENSOR_ERROR,//��gsensor����һ���Ĵ�����û���޸���
    RESET_SIM_IN,//��sim������ʱ
    RESET_SUPERNUM_CHANGED,//���ն˵ĳ����������ʱ
    RESET_PDP_ACTIVATED_MORE,//���豸��PDP�����������ϵͳ�涨��ʱ��ʱ������һ��ϵͳ
    RESET_LOADERROR_FROM_FLASH,//�豸��FLASH�м�������ԭ��������
    RESET_REMOTE_UPDATE,//�豸Զ������֮����Զ���������
    RESET_FACTORY,//�û��·���ʽ��ָ��
    RESET_SELF_CHECKING,//ϵͳ���޸���������
} enum_reset_type;

typedef enum
{
    CMD_KEEPBYTE = 0x00,//����
    CMD_FORTIFICATION   = 0x01,//Զ�����
    CMD_DISCARM = 0x02,//Զ�̳���
    CMD_REMOTE_RESET = 0x03,//Զ������
    CMD_REMOTE_SHUTDOWN = 0x04,//Զ�̹ػ�
    CMD_FACTORY = 0x05,//�ָ���������
    //0x06 0x07����
    CMD_GPS_FIX = 0x08,//������λ
    CMD_FIND_CAR = 0x09,//Ѱ��
    CMD_RESET_BT = 0x0A,//��������
    CMD_UNLOCKING = 0x0B,//Զ�̿���
    CMD_LOCKING = 0x0C,//Զ�̼���
    CMD_BAT_STATUS = 0x0D,//��ѯ�ն˵����Ϣ
    CMD_CONTROLLER = 0x0E,//������ѯ��������Ϣ
    CMD_OPEN_BAT = 0x0F,//Զ�̴򿪵����
    CMD_CLOSE_BAT = 0x10,//Զ�̹رյ����
    CMD_CLOSE_FINDCAR = 0x11,//Զ�̹ر�Ѱ��
    CMD_RESET_ALL = 0x12,//Զ����������ϵͳ
    CMD_FACTORY_CONTRALLER = 0x13,//��ʽ����������
    CMD_EXUSB_ENABLE = 0x14,//�ⲿusbʹ��
    CMD_EXUSB_DISABLE = 0x15,//�ⲿusbʧ��
    CMD_OPEN_BOSS_OF_WHEEL = 0x16,//Զ�̴������
    CMD_CLOSE_BOSS_OF_WHEEL = 0x17,//Զ�̹ر������
} enum_control_command; //����ָ��

typedef enum
{
    MOD_BT7 = 1,//��������
    MOD_WIFI = 2,//wifi����
    MOD_EXBAT_LOCK = 4,//���������
    MOD_LOUDSPEAKER = 8,//�������ȹ���
    MOD_SMS_CONTROL = 10,//���ſ��ƹ���
    MOD_MESSAGE_COLLECTION = 20,//һ���ƿ�����Ϣ�ɼ�
} enum_fun_module; //�ն˹���

typedef struct
{
    kal_uint8 borrow;//�����軹״̬ 1-����״̬ 0-�賵״̬
    kal_uint8 moving;//�����˶�״̬ 1-�˶�״̬ 0-��ֹ״̬
    kal_uint8 ele_locking;//�������״̬ 1-���Ϊ����״̬ 0-���Ϊ����״̬
    kal_uint8 acc_status;//����acc״̬ 1-����װ�� 0 �ϵ�״̬
    kal_uint8 sleeping;//��������״̬ 0- ����δ���� 1- ���������� 2-����ȫ����
    kal_uint8 bt_connect;//��������״̬ 1-����Ϊ����״̬ 0-����Ϊδ����״̬
    kal_uint8 bat_lock;//�����״̬ 1-���������״̬ 0-�������״̬
    kal_uint8 riding;//�ն�����״̬ 1-����Ϊ����װ�� 0-����Ϊ������״̬
    kal_uint8 exbat_in;//��ӵ���Ƿ���λ 1��ӵ�Դ��λ 0-��ӵ�Դ����λ
    kal_uint8 handle_status;//ת�ѿ�ʹ��״̬ 1-ת�ѿ�ʹ�ÿ�š������ 0-ת��ʧ�ܣ���ʹ������
    kal_uint8 behind_wheel;//����������״̬ 1-���ֽ���״̬ 0-���ּ���״̬
} struct_terminal_status; //�ն˵�һЩʵʱ״̬


extern void track_cust_no7_paket_86(U8 exeresult, U8 result, kal_uint8* id);
extern void track_cust_no7_paket_8C(U8 *voiceid, U8 exe);
extern kal_uint8 get_speed(void);
extern U8 AUDIO_file_play(kal_uint8 *audioID);
#endif

