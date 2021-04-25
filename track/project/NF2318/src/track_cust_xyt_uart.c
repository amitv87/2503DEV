/*******************************************************************************************
 * Filename:    track_cust_xyt_uart.c
 * Author :     lgs
 * Date :       2019-12-16
 * Comment:     �����ÿ�����,BMS �ӿ�
 ******************************************************************************************/

#if defined(__NF2318__)


/****************************************************************************
*  Include Files                        ����ͷ�ļ�
*****************************************************************************/
#include "track_cust.h"
#include "track_cust_xyt_uart.h"

/*****************************************************************************
*  Define                               �궨��
******************************************************************************/

/*****************************************************************************
*  Local Functions Define               ���غ�������
******************************************************************************/
static void xyt_ctrl_handler_err(const uint8 * data, uint16 len);
static void xyt_ctrl_handler_info(const uint8 * data, uint16 len);
static void xyt_ctrl_handler_speed(const uint8 * data, uint16 len);
static void xyt_ctrl_handler_volt(const uint8 * data, uint16 len);
static void xyt_ctrl_handler_curr(const uint8 * data, uint16 len);
static void xyt_ctrl_handler_lowstart(const uint8 * data, uint16 len);
static void xyt_ctrl_handler_light(const uint8 * data, uint16 len);
static void xyt_ctrl_handler_acc(const uint8 * data, uint16 len);
static void xyt_ctrl_handler_sw(const uint8 * data, uint16 len);
static void xyt_ctrl_handler_fd(const uint8 * data, uint16 len);
static void xyt_ctrl_handler_upg_start(const uint8 * data, uint16 len);
static void xyt_ctrl_handler_upg_data(const uint8 * data, uint16 len);
static void xyt_ctrl_handler_end(const uint8 * data, uint16 len);

/*****************************************************************************
* Local variable				        �ֲ�����
*****************************************************************************/
static xyt_ctrl_handler_map_type cmd_handler_map[] = 
{
    { XYT_CMD_ERR,            (ctrl_handler_type)xyt_ctrl_handler_err       }, 
    { XYT_CMD_INFO,           (ctrl_handler_type)xyt_ctrl_handler_info      }, 
    { XYT_CMD_SPEED,          (ctrl_handler_type)xyt_ctrl_handler_speed     },
    { XYT_CMD_VOLT,           (ctrl_handler_type)xyt_ctrl_handler_volt      },
    { XYT_CMD_CURR,           (ctrl_handler_type)xyt_ctrl_handler_curr      },
    { XYT_CMD_LOWSTART,       (ctrl_handler_type)xyt_ctrl_handler_lowstart  },
    { XYT_CMD_LIGHT,          (ctrl_handler_type)xyt_ctrl_handler_light     },
    { XYT_CMD_ACC,            (ctrl_handler_type)xyt_ctrl_handler_acc       },
    { XYT_CMD_SW,             (ctrl_handler_type)xyt_ctrl_handler_sw        },
    { XYT_CMD_FD,             (ctrl_handler_type)xyt_ctrl_handler_fd        },
    { XYT_CMD_UPG_START,      (ctrl_handler_type)xyt_ctrl_handler_upg_start },
    { XYT_CMD_UPG_DATA,       (ctrl_handler_type)xyt_ctrl_handler_upg_data  },
    { XYT_CMD_UPG_END,        (ctrl_handler_type)xyt_ctrl_handler_end       }
};


/*****************************************************************************
* Global variable				        ȫ������
*****************************************************************************/


/*****************************************************************************
*  Typedef  Enum                        ö�ٶ���
*****************************************************************************/


/*****************************************************************************
*  Struct                               ���ݽṹ����
******************************************************************************/


/*****************************************************************************
*  Local Functions                      ���غ���
******************************************************************************/

/******************************************************************************
 *  Function    -  xyt_ctrl_handler_err
 *
 *  Purpose     -  �����ÿ������ӿ�
 *
 *  Description -  ��ȡ���һ�δ������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void xyt_ctrl_handler_err(const uint8 * data, uint16 len)
{
}

/******************************************************************************
 *  Function    -  xyt_ctrl_handler_info
 *
 *  Purpose     -  �����ÿ������ӿ�
 *
 *  Description -  ��ȡ��������Ϣ
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void xyt_ctrl_handler_info(const uint8 * data, uint16 len)
{
}

/******************************************************************************
 *  Function    -  xyt_ctrl_handler_speed
 *
 *  Purpose     -  �����ÿ������ӿ�
 *
 *  Description -  �޸����ʱ��
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void xyt_ctrl_handler_speed(const uint8 * data, uint16 len)
{
}

/******************************************************************************
 *  Function    -  xyt_ctrl_handler_volt
 *
 *  Purpose     -  �����ÿ������ӿ�
 *
 *  Description -  �޸�Ƿѹ����ֵ
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void xyt_ctrl_handler_volt(const uint8 * data, uint16 len)
{
}

/******************************************************************************
 *  Function    -  xyt_ctrl_handler_curr
 *
 *  Purpose     -  �����ÿ������ӿ�
 *
 *  Description -  �޸�����ֵ
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void xyt_ctrl_handler_curr(const uint8 * data, uint16 len)
{
}

/******************************************************************************
 *  Function    -  xyt_ctrl_handler_lowstart
 *
 *  Purpose     -  �����ÿ������ӿ�
 *
 *  Description -  �޸��Ƿ�����
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void xyt_ctrl_handler_lowstart(const uint8 * data, uint16 len)
{
}

/******************************************************************************
 *  Function    -  xyt_ctrl_handler_light
 *
 *  Purpose     -  �����ÿ������ӿ�
 *
 *  Description -  ���ƴ�ƿ���
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void xyt_ctrl_handler_light(const uint8 * data, uint16 len)
{
}

/******************************************************************************
 *  Function    -  xyt_ctrl_handler_acc
 *
 *  Purpose     -  �����ÿ������ӿ�
 *
 *  Description -  ��ACC
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void xyt_ctrl_handler_acc(const uint8 * data, uint16 len)
{
}

/******************************************************************************
 *  Function    -  xyt_ctrl_handler_sw
 *
 *  Purpose     -  �����ÿ������ӿ�
 *
 *  Description -  ���ÿ������Ĺ��ܿ���
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void xyt_ctrl_handler_sw(const uint8 * data, uint16 len)
{
}

/******************************************************************************
 *  Function    -  xyt_ctrl_handler_fd
 *
 *  Purpose     -  �����ÿ������ӿ�
 *
 *  Description -  ��������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void xyt_ctrl_handler_fd(const uint8 * data, uint16 len)
{
}

/******************************************************************************
 *  Function    -  xyt_ctrl_handler_upg_start
 *
 *  Purpose     -  �����ÿ������ӿ�
 *
 *  Description -  ��ʼ����
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void xyt_ctrl_handler_upg_start(const uint8 * data, uint16 len)
{
}

/******************************************************************************
 *  Function    -  xyt_ctrl_handler_upg_data
 *
 *  Purpose     -  �����ÿ������ӿ�
 *
 *  Description -  ������������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void xyt_ctrl_handler_upg_data(const uint8 * data, uint16 len)
{
}

/******************************************************************************
 *  Function    -  xyt_ctrl_handler_end
 *
 *  Purpose     -  �����ÿ������ӿ�
 *
 *  Description -  ��������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void xyt_ctrl_handler_end(const uint8 * data, uint16 len)
{
}

/******************************************************************************
 *  Function    -  xyt_ctrl_calc_cs
 *
 *  Purpose     -  �����ü���У���
 *
 *  Description -  
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static uint8 xyt_ctrl_calc_cs(const uint8 * data, uint16 len)
{
    int i = 0;
    uint8 cs = 0;

    if(data == NULL || len == 0)
    {
        LOGD(L_UART, L_V5, "line %d, ��������!");
    }

    while(i < len)
    {
        cs += data[i++];
    }

    return cs;
}

/******************************************************************************
 *  Function    -  track_cust_xyt_controller_parse
 *
 *  Purpose     -  �����ô��ڽ��մ���ӿ�
 *
 *  Description -  ����������������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
#if 1
void track_cust_xyt_controller_parse(uint8 * raw_data, uint16 raw_len)
{
    int i;
    uint8 cs; // ���ݳ���
    xyt_controller_type ctrl;

    LOGH(L_UART, L_V5, raw_data, raw_len);

    if(raw_data == NULL || raw_len < XYT_CONTROLLER_PACK_MIN)
    {
        LOGD(L_UART, L_V5, "line=%d,���ݴ���!", __LINE__);
        return;
    }

    memset(&ctrl, 0, sizeof(xyt_controller_type));

    if(raw_data[0] == 0xAA && raw_data[1] == 0x55)
    {
        cs = xyt_ctrl_calc_cs(&raw_data[2], raw_len - 3);
        if(cs != raw_data[raw_len - 1])
        {
            LOGD(L_UART, L_V5, "line=%d,���ݴ���!", __LINE__);
            LOGH(L_UART, L_V5, raw_data, raw_len);
            return;
        }

        ctrl.add = raw_data[2];
        ctrl.cmd = raw_data[3];
        ctrl.len = raw_data[4];
        if(ctrl.len > 0)
        {
            memcpy(ctrl.data, &raw_data[5], ctrl.len);
        }

        for(i = 0; i < sizeof(cmd_handler_map) / sizeof(cmd_handler_map[0]); i++)
        {
            if(ctrl.cmd == cmd_handler_map[i].cmd)
            {
                cmd_handler_map[i].handler(ctrl.data, ctrl.len);
            }
        }
        if(i == sizeof(cmd_handler_map) / sizeof(cmd_handler_map[0]))
        {
            LOGD(L_UART, L_V5, "ָ�����[%02x]", ctrl.cmd);
        }
    }
    else
    {
        LOGD(L_UART, L_V5, "line=%d,���������Ĵ���!", __LINE__);
    }
}
#else
void track_cust_xyt_controller_parse(uint8 * raw_data, uint16 raw_len)
{
    /*
        ��ʼ��  ��ַ    ������  ���ݳ���    ����    У���
        2bytes  1byte   1byte   1byte       N bytes 1byte
    */
    int i, pos = 0;
    int remain_len; // ʣ�೤��
    int data_len; // ���ݳ���
    static uint8 buffer[1024] = {0};
    static int buffer_len = 0;
    static bool is_waiting = FALSE; // �ȴ��´����ݱ�־, �������ݶ�����һ��������,�����
    uint8 temp[1024] = {0};
    uint8 pack[378] = {0};
    int pack_len;
    uint8 cs; // check sum, У���
    uint8 calc_cs; // ����ó���У���
    xyt_controller_type ctrl;

    if(raw_data == NULL || raw_len == 0)
    {
        return;
    }

    // ����������,��ղ�����������
    if(buffer_len + raw_len > sizeof(buffer))
    {
        memset(buffer, 0, sizeof(buffer));
        buffer_len = 0;
    }

    memcpy(&buffer[buffer_len], raw_data, raw_len);
    buffer_len += raw_len;

    // С�ڿ��������ĵ���С����,��ʾδ��ȫ����,�������ݱ����,����������.
    if(buffer_len < XYT_CONTROLLER_PACK_MIN)
    {
        LOGD(L_UART, L_V5, "line=%d, �����ÿ��������ݲ��������߱����,�ȴ���������!", __LINE__);
        LOGH(L_UART, L_V5, buffer, buffer_len);
        return;
    }

    while(pos < buffer_len)
    {
        // �ҵ�����ͷ
        if(buffer[pos] == 0xAA && buffer[pos+1] == 0x55)
        {
            remain_len = buffer_len - pos;
            memcpy(temp, &buffer[pos], remain_len);

            // �����ÿ����������ҵ�����ͷ,�����Ȳ���,�ȴ�������������! ���ݻ�������
            if(remain_len < XYT_CONTROLLER_PACK_MIN)
            {
                LOGD(L_UART, L_V5, "line=%d, �����ÿ����������ҵ�����ͷ,�����Ȳ���,�ȴ�������������,�˳�����!", __LINE__);
                LOGH(L_UART, L_V5, temp, remain_len);
                memcpy(buffer, temp, remain_len);
                buffer_len = remain_len;
                break; // ����ѭ��
            }
            else
            {
                data_len = temp[4]; // ע��temp[4]��Ϊ������'���ݳ���'
                // ʣ�����ݲ������ݳ���
                if(data_len > remain_len - XYT_CONTROLLER_PACK_MIN)
                {
                    if(!is_waiting)
                    {
                        LOGD(L_UART, L_V5, "line=%d, �����ÿ����������ҵ�����ͷ,�����Ȳ���,�ȴ�������������,�˳�����!", __LINE__);
                        LOGH(L_UART, L_V5, temp, remain_len);
                        memcpy(buffer, temp, remain_len);
                        buffer_len = remain_len;
                        is_waiting = TRUE; // �ȴ��´�����
                        break; // ����ѭ��
                    }
                    else
                    {
                        LOGD(L_UART, L_V5, "line=%d, �����ÿ����������ҵ�����ͷ,�����ν��յ������ݶ�����һ����,�����ñ���ͷ����ɨ��!", __LINE__);
                        memcpy(buffer, &temp[2], remain_len - 2);
                        buffer_len = remain_len - 2;
                        pos = 0;
                        is_waiting = FALSE;
                        continue;
                    }
                }
                memcpy(pack, temp, data_len + XYT_CONTROLLER_PACK_MIN);
                pack_len = data_len + XYT_CONTROLLER_PACK_MIN;
                cs = pack[pack_len - 1];
                // ����У���Ϊ��ַ,������,���ݳ��Ⱥ����ݵ��ۼӺ�,�����ǽ�λ
                calc_cs = xyt_ctrl_calc_cs(&pack[2], pack_len - 3); // -3 ����ȥ0xAA 0x55 �� ����У��

                // У��ɹ�
                if(cs == calc_cs)
                {
                    memset(&ctrl, 0, sizeof(xyt_controller_type));
                    ctrl.add = pack[2];
                    ctrl.cmd = pack[3];
                    ctrl.len = pack[4];
                    if(ctrl.len > 0)
                    {
                        memcpy(ctrl.data, &pack[5], ctrl.len); 
                    }

                    for(i = 0; i < sizeof(cmd_handler_map) / sizeof(cmd_handler_map[0]); i++)
                    {
                        if(ctrl.cmd == cmd_handler_map[i].cmd)
                        {
                            cmd_handler_map[i].handler(ctrl.data, ctrl.len);
                        }
                    }
                    if(i == sizeof(cmd_handler_map) / sizeof(cmd_handler_map[0]))
                    {
                        LOGD(L_UART, L_V5, "ָ�����[%02x]", ctrl.cmd);
                    }
                    buffer_len = remain_len - pack_len;
                    memcpy(buffer, &temp[pack_len], buffer_len);
                    pos = 0;// ��ͷɨ��
                }
                else
                { // �����ñ���ͷ����ɨ��!
                    memcpy(buffer, &temp[2], remain_len - 2);
                    buffer_len = remain_len - 2;
                    pos = 0;
                    is_waiting = FALSE;
                    continue;
                }
            }
        }

        pos++;
    }
}
#endif

/******************************************************************************
 *  Function    -  track_cust_xyt_uart_read
 *
 *  Purpose     -  �����ô��ڽ��մ���ӿ�
 *
 *  Description -  UART���ݽ���
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-16,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_xyt_uart_read(uint8 * data, uint16 data_len)
{
    if(data == NULL || data_len == 0)
    {
        return;
    }

    LOGH(L_UART, L_V5, data, data_len);

    track_cust_xyt_controller_parse(data, data_len);
}

#endif /* __NF2318__ */

