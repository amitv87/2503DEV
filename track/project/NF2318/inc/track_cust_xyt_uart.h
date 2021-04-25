/*******************************************************************************************
 * Filename:    track_cust_xyt_uart.h
 * Author :     lgs
 * Date :       2019-12-16
 * Comment:     �����ÿ�����,BMS �ӿ�ͷ�ļ�
 ******************************************************************************************/

#if defined(__NF2318__)

/****************************************************************************
* Include Files                         ����ͷ�ļ�
*****************************************************************************/
#include "track_cust.h"


/*****************************************************************************
 *  Define					�궨��
 *****************************************************************************/
#define C_SIGN                          0x10    // ���������ݱ�ʶλ
#define B_SIGN                          0x20    // BMS ���ݱ�ʶλ

#define XYT_CONTROL_VER                 0x03    // Э��汾��(��ֵ�̶�Ϊ3)

// ���̴���
#define MANUFACTURER_JH                 1       // ����
#define MANUFACTURER_GB                 2       // �߱�
#define MANUFACTURER_SZ                 3       // ����
#define MANUFACTURER_HY                 4       // ����
#define MANUFACTURER_OURMETER           5       // ourmeter
#define MANUFACTURER_JF                 6       // ���
#define MANUFACTURER_YZ                 7       // ��׿

#define XYT_CONTROLLER_PACK_MIN         6       // ������������С����

/*****************************************************************************
 *  Struct                              ���ݽṹ����
 *****************************************************************************/

typedef void (* ctrl_handler_type) (void * data_ptr, uint16 data_len);

typedef struct
{
    uint8               cmd;
    ctrl_handler_type   handler;
} xyt_ctrl_handler_map_type;

typedef struct 
{
    uint8               add;                    // ��ַ
    uint8               cmd;                    // ����
    uint8               len;                    // ����
    uint8               data[255];              // ����
} xyt_controller_type;

typedef enum
{
    XYT_CMD_ERR             = 0x00,             // ��ȡ���һ�δ������
    XYT_CMD_INFO            = 0x11,             // ��ȡ��������Ϣ
    XYT_CMD_SPEED           = 0x12,             // �޸����ʱ��
    XYT_CMD_VOLT            = 0x13,             // �޸�Ƿѹ����ֵ
    XYT_CMD_CURR            = 0x14,             // �޸�����ֵ
    XYT_CMD_LOWSTART        = 0x15,             // �޸��Ƿ�����
    XYT_CMD_LIGHT           = 0x16,             // ���ƴ�ƿ���
    XYT_CMD_ACC             = 0x17,             // ��ACC
    XYT_CMD_SW              = 0x18,             // ���ÿ������Ĺ��ܿ���
    XYT_CMD_FD              = 0x19,             // ��������
    XYT_CMD_UPG_START       = 0x1A,             // ��ʼ����
    XYT_CMD_UPG_DATA        = 0x1B,             // ������������
    XYT_CMD_UPG_END         = 0x1C,             // ��������
    XYT_CMD_UNKNOW          = 0xFF
} xyt_controller_cmd_type;

/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/
void track_cust_xyt_uart_read(uint8 * data, uint16 data_len);


#endif /* __NF2318__ */

