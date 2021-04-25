
/*******************************************************************************************
 * Filename:    track_cust_xyt_procotol.h
 * Author :     lgs
 * Date :       2019-12-16
 * Comment:     ������ƽ̨Э��ӿ�ͷ�ļ�
 ******************************************************************************************/

#if defined(__NF2318__)

/****************************************************************************
* Include Files                         ����ͷ�ļ�
*****************************************************************************/
#include "track_cust.h"


/*****************************************************************************
 *  Define					�궨��
 *****************************************************************************/
#define XYT_PACKET_HEAD_LEN     22
#define XYT_IDENTIFIER_CODE_LEN 12
#define XYT_DEVICE_CODE         4
#define XYT_PACKET_MIN_LEN      25  /* (��Ϣ����2) + (��Ϣͷ22) + (��Ϣ��n) + (У����1) */

#define XYT_STE_CODE            0x04000000  /* ע��ID */

/*****************************************************************************
 *  Struct                              ���ݽṹ����
 *****************************************************************************/
typedef enum
{
    XYT_UL_REGISTER             = 0x8F,         // ע������
    XYT_UL_AUTH                 = 0x81,         // �ն˼�Ȩ
    XYT_UL_GPS                  = 0x82,         // GPS
    XYT_UL_HB                   = 0x83,         // �ն˻�����Ϣ�ϱ�/����
    XYT_UL_REQUEST              = 0x8A,         // ������������
    XYT_UL_DEFAULT              = 0x8B,         // �ָ�������������
    XYT_UL_RESET                = 0x8C,         // ����������
    XYT_UL_DEFENCE              = 0x90,         // ���/��������
    XYT_UL_ENGINE               = 0x91,         // ����/�ر�����
    XYT_UL_SEEK                 = 0x97,         // Ѱ���ź�����
    XYT_UL_TURNOFF              = 0x98,         // �ػ�����
    XYT_UL_UPG                  = 0xA0,         // ��������׼������
    XYT_UL_BATT_LOCK            = 0xA8,         // ����غ�������
    XYT_UL_GPS_INTERVAL         = 0xA9,         // GPS�����ϴ�ʱ����������
    XYT_UL_LOCK_PARAM           = 0xAB,         // ���õ��������
    XYT_UL_SIM                  = 0xAD,         // SIM����Ϣ�ϱ�
    XYT_UL_STORE_ALM            = 0xC0,         // �洢�ı�����Ϣ�ϱ�
    XYT_UL_STORE_BATT           = 0xC1,         // �洢�ĵ��״̬��Ϣ�ϱ�
    XYT_UL_DEVICE_ID            = 0x9D,         // �޸��豸ID
    XYT_UL_PASSTHROUGH          = 0xAE,         // �ı�����͸������
    XYT_UL_VOICE_PLAY_RSP       = 0xD1,         // Զ������������Ӧ
    XYT_UL_VOICE_SYNC           = 0xD2,         // �������ݱ��ͬ��������
    XYT_UL_GET_HTTP_LINK        = 0xD3,         // �ļ����ӻ�ȡ
    XYT_UL_VOICE_ADD            = 0xD4,         // ������������
    XYT_UL_VOICE_ADD_RST        = 0x64,         // �����������ݽ���ϱ�
    XYT_UL_VOICE_DEL            = 0xD5,         // ɾ����������
    XYT_UL_VOICE_DEL_RST        = 0x65,         // ɾ���������ݽ���ϱ�
    XYT_UL_UNKNOW
} xyt_uplink_cmd_type;

typedef enum
{
    XYT_DL_REGISTER             = 0x0F,         // ע������
    XYT_DL_AUTH                 = 0x01,         // �ն˼�Ȩ
    XYT_DL_GPS                  = 0x02,         // GPS
    XYT_DL_HB                   = 0x03,         // �ն˻�����Ϣ�ϱ�/����
    XYT_DL_REQUEST              = 0x0A,         // ������������
    XYT_DL_DEFAULT              = 0x0B,         // �ָ�������������
    XYT_DL_RESET                = 0x0C,         // ����������
    XYT_DL_DEFENCE              = 0x10,         // ���/��������
    XYT_DL_ENGINE               = 0x11,         // ����/�ر�����
    XYT_DL_SEEK                 = 0x17,         // Ѱ���ź�����
    XYT_DL_TURNOFF              = 0x18,         // �ػ�����
    XYT_DL_UPG                  = 0x20,         // ��������׼������
    XYT_DL_BATT_LOCK            = 0x28,         // ����غ�������
    XYT_DL_GPS_INTERVAL         = 0x29,         // GPS�����ϴ�ʱ����������
    XYT_DL_LOCK_PARAM           = 0x2B,         // ���õ��������
    XYT_DL_SIM                  = 0x2D,         // SIM����Ϣ�ϱ�
    XYT_DL_STORE_ALM            = 0x40,         // �洢�ı�����Ϣ�ϱ�
    XYT_DL_STORE_BATT           = 0x41,         // �洢�ĵ��״̬��Ϣ�ϱ�
    XYT_DL_DEVICE_ID            = 0x1D,         // �޸��豸ID
    XYT_DL_PASSTHROUGH          = 0x2E,         // �ı�����͸������
    XYT_DL_VOICE_PLAY           = 0x51,         // Զ����������
    XYT_DL_VOICE_SYNC           = 0x52,         // �������ݱ��ͬ����Ӧ
    XYT_DL_HTTP_LINK_RSP        = 0x53,         // �ļ�������Ӧ
    XYT_DL_VOICE_ADD            = 0x54,         // ������������
    XYT_DL_VOICE_DEL            = 0x55,         // ɾ����������
    XYT_DL_UNKNOW
} xyt_downlink_cmd_type;


typedef struct 
{
    uint8           msg_id;                     // ��ϢID
    uint16          body_len;                   // ��Ϣ�峤��
    uint16          serial_id;                  // ��Ϣ��ˮ��
    uint8           identify_code[XYT_IDENTIFIER_CODE_LEN]; // ʶ����
    uint32          equipment_code;             // �豸����
    uint8           supplier_code;              // ��Ӧ�̴���
} xyt_pack_head;

typedef struct
{
    xyt_pack_head   hd;
    uint8         * body;
} xyt_pack_type;


/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/
void    track_cust_packet_rabbit_init(void);
uint8   track_cust_packet_rabbit_calc_crc(uint8 * data, uint16 data_len);

void    track_cust_packet_rabbit_8F(void);
void    track_cust_packet_rabbit_81(void);
void    track_cust_packet_rabbit_82(void);
void    track_cust_packet_rabbit_83(void);
void    track_cust_packet_rabbit_8A(void);
void    track_cust_packet_rabbit_8B(void);
void    track_cust_packet_rabbit_8C(void);
void    track_cust_packet_rabbit_90(void);
void    track_cust_packet_rabbit_91(void);
void    track_cust_packet_rabbit_97(void);
void    track_cust_packet_rabbit_98(void);
void    track_cust_packet_rabbit_A0(void);
void    track_cust_packet_rabbit_A8(void);
void    track_cust_packet_rabbit_A9(void);
void    track_cust_packet_rabbit_AB(void);
void    track_cust_packet_rabbit_AD(void);
void    track_cust_packet_rabbit_C0(void);
void    track_cust_packet_rabbit_C1(void);
void    track_cust_packet_rabbit_9D(void);
void    track_cust_packet_rabbit_AE(void);
void    track_cust_packet_rabbit_D1(void);
void    track_cust_packet_rabbit_D2(void);
void    track_cust_packet_rabbit_D3(void);
void    track_cust_packet_rabbit_D4(void);
void    track_cust_packet_rabbit_D5(void);
void    track_cust_packet_rabbit_64(void);
void    track_cust_packet_rabbit_65(void);

#endif /* __NF2318__ */

