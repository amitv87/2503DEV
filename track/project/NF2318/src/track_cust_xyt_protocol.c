/*******************************************************************************************
 * Filename:    track_cust_xyt_protocol.c
 * Author :     lgs
 * Date :       2019-12-16
 * Comment:     ������ƽ̨Э��ӿ�
 ******************************************************************************************/

#if defined(__NF2318__)


/****************************************************************************
*  Include Files                        ����ͷ�ļ�
*****************************************************************************/
#include "track_cust.h"
#include "track_cust_xyt_protocol.h"

/*****************************************************************************
*  Define                               �궨��
******************************************************************************/



/*****************************************************************************
* Local variable				�ֲ�����
*****************************************************************************/



/*****************************************************************************
* Global variable				ȫ������
*****************************************************************************/
UINT16  g_rabbit_terminal_serial_number = 0;
UINT8   g_rabbit_identifier_string[XYT_IDENTIFIER_CODE_LEN] = {0};
UINT32  g_rabbit_device_code = 0;   /*�豸�����޷������ֽ�����*/
UINT8   g_rabbit_supplier_code = 0; /*��Ӧ�̴���  �û����Ҵ�����,1�ֽ�*/
UINT8   g_rabbit_recive_buff[2048];

/*****************************************************************************
*  Typedef  Enum                        ö�ٶ���
*****************************************************************************/


/*****************************************************************************
*  Struct                               ���ݽṹ����
******************************************************************************/


/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_0F
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  ע������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_0F(void * data, uint16 len)
{
    /*
        1   ע��״̬        BYTE    01 ��һ��ע��
                                    02 ��ע�ᣨ�ڶ��Σ������Σ�
                                    03 �Ƿ�ע��
        1   ע����Ϣ����    BYTE    ע����Ϣ����
        N   ע����Ϣ        BYTE[n] �ն��ṩ��ע���루��Ȩ������
        4   ע��ID          DWORD   �ն�ע��ID��
    */
    int i = 0;
    uint8 register_status;
    uint8 auth_len;
    uint32 ste_code;
    uint8 * ptr = (uint8 *)data;

    LOGH2(L_APP, L_V5, "", data, len);

    if(data == NULL || len == 0)
    {
        return;
    }

    register_status = ptr[i++];
    auth_len = ptr[i++]; //��Ȩ��������
    if(auth_len > 32)
    {
        LOGD(L_APP, L_V5, "��Ȩ�����!");
        return;
    }
    memset(G_parameter.ste_param.auth_code, 0, sizeof(G_parameter.ste_param.auth_code));
    memcpy(G_parameter.ste_param.auth_code, &ptr[i], auth_len);
    G_parameter.ste_param.auth_len = auth_len;
    i += auth_len;
    // ƽ̨�·���ע��ID�� Ӧ�����豸���е���һ�µ�,�˴������������ж�
    ste_code = (uint32)(ptr[  i] << 24)
             | (uint32)(ptr[i+1] << 16)
             | (uint32)(ptr[i+2] <<  8)
             | (uint32)(ptr[i+3] <<  0);

    Track_nvram_write(NVRAM_EF_PARAMETER_LID, 1, (void *)&G_parameter, NVRAM_EF_PARAMETER_SIZE);

    if(register_status != 0x03)
    {
        track_cust_packet_rabbit_81();
    }
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_01
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  �ն˼�ȨӦ��
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_01(void * data, uint16 len)
{
    /*
    
    1   ��¼״̬    BYTE    1����ʾ�����¼����
                            2�����豸δע�ᣬ��Ҫ����ע��
                            3��ϵͳ�޴��豸��Ϣ���ܾ���¼����
                            4������ԭ�򣬽�ֹ��¼���� 
    6   ��������ʱ�� BYTE[N] �ն˿ɸ��ݷ�������ʱ�䣬����ʱ��
                            BYTE[0] ��  2016 =2000+ BYTE[0]
                            BYTE[1] ��
                            BYTE[2] ��
                            BYTE[3] ʱ
                            BYTE[4] ��
                            BYTE[5] ��
    */

    int i = 0;
    uint8 auth_status;
    uint8 * ptr = (uint8 *)data;
    applib_time_struct sys_time;
    
    LOGH2(L_APP, L_V5, "", data, len);

    if(data == NULL || len < 7)
    {
        return;
    }
    auth_status = ptr[i++];

    memset(&sys_time, 0, sizeof(applib_time_struct));

    sys_time.nYear  = ptr[i++] + 2000;
    sys_time.nMonth = ptr[i++];
    sys_time.nDay   = ptr[i++];
    sys_time.nHour  = ptr[i++];
    sys_time.nMin   = ptr[i++];
    sys_time.nSec   = ptr[i++];

    if(auth_status == 1)
    {
        // ��Ȩ�ɹ�,��¼�ɹ�
        tr_stop_timer(TRACK_CUST_NET_LOGIN_TIMEOUT_TIMER_ID);
        tr_stop_timer(TRACK_CUST_NET_LOGIN_RESEND_TIMER_ID);

        track_set_login_status(KAL_TRUE);
    }
    else if(auth_status == 2)
    {
        // ��Ҫ����ע��
        track_cust_packet_rabbit_8F();
    }
    else
    {
        LOGD(L_APP, L_V5, "ƽ̨�ܾ���¼,���߽�ֹ��¼ err_code[%d]", auth_status);
        return;
    }
    // ͬ��ʱ��
    track_fun_update_time_to_system(2, &sys_time);
    LOGD(L_APP, L_V5, "����������:%d-%02d-%02d,ʱ��:%02d:%02d:%02d", sys_time.nYear, sys_time.nMonth, sys_time.nDay, sys_time.nHour, sys_time.nMin, sys_time.nSec);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_02
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  GPSӦ��
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_02(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_03
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  �ն˻�����Ϣ�ϱ�/����
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_03(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}


/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_0A
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  ƽ̨������������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_0A(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_0B
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  �ָ�������������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_0B(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}


/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_0C
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  ����������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_0C(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}


/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_10
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  ���/��������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_10(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}


/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_11
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  ����/�ر�����
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_11(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_17
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  Ѱ���ź�����
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_17(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_18
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  �ػ�����
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_18(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_20
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  ��������׼������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_20(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_28
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  ����غ�������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_28(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_29
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  GPS�����ϴ�ʱ����������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_29(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_2B
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  ���õ��������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_2B(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_2D
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  SIM����Ϣ�ϱ�Ӧ��
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_2D(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_40
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  �������յ��ն�����͸����,�����ݽ��лظ�
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_40(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_dl_parse
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  �洢�ĵ��״̬��Ϣ�ϱ���Ӧ��
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_41(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_1D
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  �޸��豸ID
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_1D(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_2E
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  �ı�����͸�������Ӧ��
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_2E(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_51
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  Զ����������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_51(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_52
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  �������ݱ��ͬ����Ӧ
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_52(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_53
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  �ļ�������Ӧ
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_53(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_54
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  ������������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_54(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_55
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -  ɾ����������
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_rabbit_55(void * data, uint16 len)
{
    LOGD(L_APP, L_V5, "");
    LOGH2(L_APP, L_V5, "", data, len);
}


/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_dl_parse
 *
 *  Purpose     -  �������������ݽ����ӿ�
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_dl_parse(void * data_ptr, uint16 data_len)
{
    /*
    *   ��Ϣ����	��Ϣͷ	 ��Ϣ��	    У����
    *   2 Bytes     22 Bytes  n Bytes   1 Byte
    */
    uint16  msg_len = 0;
    uint16 len = data_len;
    xyt_pack_type msg;
    uint8 temp[1024] = {0};
    uint8 cs;

    LOGH2(L_APP, L_V5, "", data_ptr, data_len);

    if(data_ptr == NULL || data_len < XYT_PACKET_MIN_LEN)
    {
        LOGD(L_APP, L_V5, "�����������");
        return;
    }

    memset(&msg, 0, sizeof(xyt_pack_type));

    memcpy(g_rabbit_recive_buff, data_ptr, len);

    while(len > 0)
    {
        msg_len = ((uint16)g_rabbit_recive_buff[0] << 0) & 0x00FF
                | ((uint16)g_rabbit_recive_buff[1] << 8) & 0xFF00;

        if(msg_len > len)
        {
            LOGD(L_APP, L_V5, "%d, ���ȴ���!�������ĳ�����ʵ�ʳ��Ȳ�һ��!", __LINE__);
            break;
        }
        // ע��:�˴�temp���������Ϊ "��Ϣͷ+��Ϣ��+У����"
        memcpy(temp, &g_rabbit_recive_buff[2], msg_len);

        cs = track_cust_packet_rabbit_calc_crc(temp, msg_len - 1);
        if(cs == temp[msg_len - 1])
        {
            // ��Ϣͷ
            msg.hd.msg_id = temp[0];
            msg.hd.body_len = ((uint16)temp[1] << 0) & 0x00FF | ((uint16)temp[2] << 8) & 0xFF00;
            msg.hd.serial_id = ((uint16)temp[3] << 0) & 0x00FF | ((uint16)temp[4] << 8) & 0xFF00;
            memcpy(msg.hd.identify_code, &temp[5], XYT_IDENTIFIER_CODE_LEN);
            msg.hd.equipment_code = ((uint32)temp[17] <<  0) & 0x000000FF
                                  | ((uint32)temp[18] <<  8) & 0x0000FF00
                                  | ((uint32)temp[19] << 16) & 0x00FF0000
                                  | ((uint32)temp[20] << 24) & 0xFF000000;
            msg.hd.supplier_code = temp[21];

            if(msg.hd.body_len > 0)
            {
                // ��Ϣ��
                msg.body = (uint8 *)Ram_Alloc(5, msg.hd.body_len);
                if(msg.body != NULL)
                {
                    memcpy(msg.body, &temp[22], msg.hd.body_len);
                }
            }

            switch(msg.hd.msg_id)
            {
                case XYT_DL_REGISTER:           // ע������
                    track_cust_packet_rabbit_0F(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_AUTH:               // �ն˼�Ȩ
                    track_cust_packet_rabbit_01(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_GPS:                // GPS
                    track_cust_packet_rabbit_02(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_HB:                 // �ն˻�����Ϣ�ϱ�/����
                    track_cust_packet_rabbit_03(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_REQUEST:            // ������������
                    track_cust_packet_rabbit_0A(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_DEFAULT:            // �ָ�������������
                    track_cust_packet_rabbit_0B(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_RESET:              // ����������
                    track_cust_packet_rabbit_0C(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_DEFENCE:            // ���/��������
                    track_cust_packet_rabbit_10(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_ENGINE:             // ����/�ر�����
                    track_cust_packet_rabbit_11(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_SEEK:               // Ѱ���ź�����
                    track_cust_packet_rabbit_17(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_TURNOFF:            // �ػ�����
                    track_cust_packet_rabbit_18(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_UPG:                // ��������׼������
                    track_cust_packet_rabbit_20(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_BATT_LOCK:          // ����غ�������
                    track_cust_packet_rabbit_28(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_GPS_INTERVAL:       // GPS�����ϴ�ʱ����������
                    track_cust_packet_rabbit_29(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_LOCK_PARAM:         // ���õ��������
                    track_cust_packet_rabbit_2B(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_SIM:                // SIM����Ϣ�ϱ�
                    track_cust_packet_rabbit_2D(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_STORE_ALM:          // �洢�ı�����Ϣ�ϱ�
                    track_cust_packet_rabbit_40(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_STORE_BATT:         // �洢�ĵ��״̬��Ϣ�ϱ�
                    track_cust_packet_rabbit_41(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_DEVICE_ID:          // �޸��豸ID
                    track_cust_packet_rabbit_1D(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_PASSTHROUGH:        // �ı�����͸������
                    track_cust_packet_rabbit_2E(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_VOICE_PLAY:         // Զ����������
                    track_cust_packet_rabbit_51(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_VOICE_SYNC:         // �������ݱ��ͬ����Ӧ
                    track_cust_packet_rabbit_52(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_HTTP_LINK_RSP:      // �ļ�������Ӧ
                    track_cust_packet_rabbit_53(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_VOICE_ADD:          // ������������
                    track_cust_packet_rabbit_54(msg.body, msg.hd.body_len);
                    break;

                case XYT_DL_VOICE_DEL:          // ɾ����������
                    track_cust_packet_rabbit_55(msg.body, msg.hd.body_len);
                    break;

                default:
                    LOGD(L_APP, L_V5, "���֧��");
                    break;
            }

            if(msg.body != NULL)
            {
                Ram_Free(msg.body);
            }
        }

        len = len - (msg_len + 2);
        memcpy(g_rabbit_recive_buff, &g_rabbit_recive_buff[msg_len + 2], len);
    }
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_calc_crc
 * 
 *  Purpose     -  BCC�����У�飩��
 * 
 *  Description -   
 * 
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-13,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
uint8 track_cust_packet_rabbit_calc_crc(uint8 * data, uint16 data_len)
{
    uint8 crc;
    uint16 i = 0;

    if(data == NULL || data_len == 0)
    {
        LOGD(L_APP, L_V3, "��������!");
        return 0xFF;
    }

    crc = data[i];
    while(++i < data_len)
    {
        crc ^= data[i];
    }

    return crc;
}


/******************************************************************************
 *  Function    -  track_cust_packet_ul_pack_head
 *
 *  Purpose     -  ��������Ϣͷ�����Ϣ
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-13,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
static void track_cust_packet_ul_pack_head(xyt_uplink_cmd_type cmd, uint8 * out_info, uint16 body_len)
{
    uint8 temp[XYT_PACKET_HEAD_LEN] = {0};
    int pos = 0;

    if(out_info == NULL)
    {
        LOGD(L_APP, L_V5, "�����������");
        return;
    }

    // ����
    temp[pos++] = cmd;

    // ��Ϣ�峤��
    temp[pos++] = (uint8)((body_len & 0x00FF) >> 0);
    temp[pos++] = (uint8)((body_len & 0xFF00) >> 8);

    // ��ˮ��
    temp[pos++] = (uint8)((g_rabbit_terminal_serial_number & 0x00FF) >> 0);
    temp[pos++] = (uint8)((g_rabbit_terminal_serial_number & 0xFF00) >> 8);

    // ʶ����
    memcpy(&temp[pos], G_importance_parameter_data.identify, XYT_IDENTIFIER_CODE_LEN);
    pos += XYT_IDENTIFIER_CODE_LEN;

    // ע��ID
    temp[pos++] = (uint8)((XYT_STE_CODE & 0xFF000000) >> 24);
    temp[pos++] = (uint8)((XYT_STE_CODE & 0x00FF0000) >> 16);
    temp[pos++] = (uint8)((XYT_STE_CODE & 0x0000FF00) >>  8);
    temp[pos++] = (uint8)((XYT_STE_CODE & 0x000000FF) >>  0);

    // ��Ӧ�̴���
    temp[pos++] = G_importance_parameter_data.cs_code;

    memcpy(out_info, temp, pos);

    g_rabbit_terminal_serial_number++;
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_8F
 *
 *  Purpose     -  ע������
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_8F(void)
{
    /*
    1   ע����Ϣ����    BYTE    ע����Ϣ����
    N   ע����Ϣ        BYTE[n] �ն��ṩ��ע����
    4   ע��ID          DWORD   �ն�ע��ID��(�豸����)
    */
    int main_pos = 2; // Ԥ����λ��ų���
    int body_pos = 0;
    uint8 * code = NULL;
    uint8 main_buff[512] = {0};
    uint8 body_buff[256] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    code = track_drv_get_imei(0);

    // ע����Ϣ����
    body_buff[body_pos++] = strlen(code);

    // �ն��ṩ��ע���� ��IMEI�ַ����
    memcpy(&body_buff[body_pos], code, strlen(code));
    body_pos += strlen((char *)code);

    // �ն�ע��ID��(�豸����)
    body_buff[body_pos++] = (uint8)((XYT_STE_CODE & 0xFF000000) >> 24);
    body_buff[body_pos++] = (uint8)((XYT_STE_CODE & 0x00FF0000) >> 16);
    body_buff[body_pos++] = (uint8)((XYT_STE_CODE & 0x0000FF00) >>  8);
    body_buff[body_pos++] = (uint8)((XYT_STE_CODE & 0x000000FF) >>  0);

    track_cust_packet_ul_pack_head(XYT_UL_REGISTER, head_buff, body_pos);

    memcpy(&main_buff[main_pos], head_buff, XYT_PACKET_HEAD_LEN);
    main_pos += XYT_PACKET_HEAD_LEN;
    memcpy(&main_buff[main_pos], body_buff, body_pos);
    main_pos += body_pos;

    // calculate У����
    main_buff[main_pos] = track_cust_packet_rabbit_calc_crc(&main_buff[2], main_pos - 2);
    main_pos += 1;

    // ע��!!! ��Ϣ�����峤�ȣ����������������ֽ�
    main_buff[0] = (uint8)(((main_pos - 2) & 0x00FF) >> 0);
    main_buff[1] = (uint8)(((main_pos - 2) & 0xFF00) >> 8);

    LOGH2(L_APP, L_V5, "", main_buff, main_pos);
    track_soc_write_req(main_buff, main_pos, XYT_UL_REGISTER, 0);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_81
 *
 *  Purpose     -  �ն˼�Ȩ
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_81(void)
{
    /*
        2   ��¼��ˮ��      WORD    ����������1��65535(�糬��65535���ٴδ�1��ʼ)
        1   ��Ȩ��������    BYTE    ��Ȩ�����ĳ���
        N   ��Ȩ����        BYTE[N] ��Ȩ������ͨ��ע�������ȡ
        1   ����ID����      BYTE    ����ID�ĳ���
        N   ����ID          BYTE[N] ����ID
    */
    uint16 body_pos = 0;
    uint16 main_pos = 2;
    uint8 main_buff[512] = {0};
    uint8 body_buff[256] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};


    // ��¼��ˮ������Ϣ��ˮ����ͬ, �˴� +1Ҳ��Ϊ�˱���ͬһ�����е���Ϣ��ˮ�����¼��ˮ��һ��
    body_buff[body_pos++] = (uint8)((g_rabbit_terminal_serial_number+1 & 0x00FF) >> 0);
    body_buff[body_pos++] = (uint8)((g_rabbit_terminal_serial_number+1 & 0xFF00) >> 8);

    body_buff[body_pos++] = G_parameter.ste_param.auth_len;

    memcpy(&body_buff[body_pos], G_parameter.ste_param.auth_code, G_parameter.ste_param.auth_len);
    body_pos += G_parameter.ste_param.auth_len;

    body_buff[body_pos++] = 17;
    memcpy(&body_buff[body_pos], G_importance_parameter_data.device_id, 17);
    body_pos += 17;
    
    track_cust_packet_ul_pack_head(XYT_UL_AUTH, head_buff, body_pos);

    memcpy(&main_buff[main_pos], head_buff, XYT_PACKET_HEAD_LEN);
    main_pos += XYT_PACKET_HEAD_LEN;
    memcpy(&main_buff[main_pos], body_buff, body_pos);
    main_pos += body_pos;

    // calculate У����
    main_buff[main_pos] = track_cust_packet_rabbit_calc_crc(&main_buff[2], main_pos - 2);
    main_pos += 1;

    // ע��!!! ��Ϣ�����峤�ȣ����������������ֽ�
    main_buff[0] = (uint8)(((main_pos - 2) & 0x00FF) >> 0);
    main_buff[1] = (uint8)(((main_pos - 2) & 0xFF00) >> 8);

    LOGH2(L_APP, L_V5, "", main_buff, main_pos);

    track_soc_write_req(main_buff, main_pos, XYT_UL_AUTH, 0);
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_82
 *
 *  Purpose     -  GPS��λ��Ϣ�ϴ�
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_82(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_GPS, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_83
 *
 *  Purpose     -  ����
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_83(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_HB, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_8A
 *
 *  Purpose     -  ������������
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_8A(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_REQUEST, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_8B
 *
 *  Purpose     -  �ָ�������������ִ�н��Ӧ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_8B(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_DEFAULT, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_8C
 *
 *  Purpose     -  ����������ִ�н��Ӧ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_8C(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_RESET, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_90
 *
 *  Purpose     -  ���/��������ִ�н��Ӧ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_90(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_DEFENCE, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_91
 *
 *  Purpose     -  ����/�ر�����ִ�н��Ӧ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_91(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_ENGINE, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_97
 *
 *  Purpose     -  Ѱ���ź�����ִ�н��Ӧ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_97(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_SEEK, head_buff, body_pos);
    // todo...
    // ����
}


/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_98
 *
 *  Purpose     -  �ػ�����ִ�н��Ӧ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_98(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_TURNOFF, head_buff, body_pos);
    // todo...
    // ����
}


/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_A0
 *
 *  Purpose     -  �յ� ��������׼������ִ�н��Ӧ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_A0(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_UPG, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_A8
 *
 *  Purpose     -  ����غ�������ִ�н��Ӧ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_A8(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_BATT_LOCK, head_buff, body_pos);
    // todo...
    // ����
}


/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_A9
 *
 *  Purpose     -  GPS�����ϴ�ʱ����������Ӧ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_A9(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_GPS_INTERVAL, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_AB
 *
 *  Purpose     -  ���õ��������Ӧ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_AB(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_LOCK_PARAM, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_AD
 *
 *  Purpose     -  SIM����Ϣ�ϱ�
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_AD(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_SIM, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_C0
 *
 *  Purpose     -  �洢�ı�����Ϣ�ϱ�
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_C0(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_STORE_ALM, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_C1
 *
 *  Purpose     -  �洢�ĵ��״̬��Ϣ�ϱ�
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_C1(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_STORE_BATT, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_9D
 *
 *  Purpose     -  �޸��豸ID
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_9D(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_DEVICE_ID, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_AE
 *
 *  Purpose     -  �ı�����͸������
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_AE(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_PASSTHROUGH, head_buff, body_pos);
    // todo...
    // ����
}


/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_D1
 *
 *  Purpose     -  Զ������������Ӧ
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_D1(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_VOICE_PLAY_RSP, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_D2
 *
 *  Purpose     -  �������ݱ��ͬ��������
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_D2(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_VOICE_SYNC, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_D3
 *
 *  Purpose     -  �ļ����ӻ�ȡ
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_D3(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_GET_HTTP_LINK, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_D4
 *
 *  Purpose     -  ������������
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_D4(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_VOICE_ADD, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_64
 *
 *  Purpose     -  �����������ݽ���ϱ�
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_64(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_VOICE_ADD_RST, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_D5
 *
 *  Purpose     -  ɾ����������
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_D5(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_VOICE_DEL, head_buff, body_pos);
    // todo...
    // ����
}

/******************************************************************************
 *  Function    -  track_cust_packet_rabbit_65
 *
 *  Purpose     -  ɾ���������ݽ���ϱ�
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 2019-12-19,  lgs  written
 * ----------------------------------------
 ******************************************************************************/
void track_cust_packet_rabbit_65(void)
{
    uint16 body_pos = 0;
    uint8 main_buff[512] = {0};
    uint8 head_buff[XYT_PACKET_HEAD_LEN] = {0};

    track_cust_packet_ul_pack_head(XYT_UL_VOICE_DEL_RST, head_buff, body_pos);

    // todo...
    // ����
}

#endif /* __NF2318__ */

