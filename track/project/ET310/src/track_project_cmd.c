/******************************************************************************
 * track_project_cmd.c -
 *
 * Copyright .
 *
 * DESCRIPTION: -
 *        ����Ŀ�����ָ����������ȼ���ߣ��ͱ�׼��ͬ���ģ�ֻ�������ļ���ָ��
 *
 * modification history
 * --------------------
 * v1.0   2017-06-30,  chengjun create this file
 *
 ******************************************************************************/

/****************************************************************************
* Include Files                         ����ͷ�ļ�
*****************************************************************************/
#include "track_os_data_types.h"
#include "kal_public_defs.h"
#include "track_os_log.h"
#include "track_cmd.h"

/*****************************************************************************
 *  Define					�궨��
 *****************************************************************************/


/*****************************************************************************
* Typedef  Enum         ö��
*****************************************************************************/


/*****************************************************************************
 *  Struct                              ���ݽṹ����
 *****************************************************************************/
typedef void(*Func_CustRecvPtr)(CMD_DATA_STRUCT*);

typedef struct
{
    const char           *cmd_string;
    Func_CustRecvPtr     func_ptr;
    BOOL                valid;
} track_project_cmd_table_struct;

/*****************************************************************************
 *  Global Functions			ȫ�ֱ���
 *****************************************************************************/


/*****************************************************************************
 *  Local Functions			���غ���
 *****************************************************************************/


/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/

//TRUE  ��ʾʹ����Ŀ�Զ���Ĺ��ܣ����Ǳ�׼��
//FALSE ��ʾ��֧�ָ�ָ�ͬʱ��׼���Ҳ����ִ��
static track_project_cmd_table_struct track_project_cmd_table[] =
{
    {"TIMER", NULL,   FALSE},
};


kal_bool track_project_command(CMD_DATA_STRUCT * cmd)
{
    int i, size, ret;
    kal_bool find = KAL_FALSE, pass_status = KAL_FALSE;
    size = sizeof(track_project_cmd_table) / sizeof(track_project_cmd_table_struct);

    LOGD(L_APP, L_V5, "num_type:%d, len:%d, data:%s   [%d/%d]%s|%s|%s|%s|%s",
         cmd->return_sms.num_type, cmd->rcv_length, cmd->rcv_msg_source, cmd->part, size, cmd->pars[0], cmd->pars[1], cmd->pars[2], cmd->pars[3], cmd->pars[4]);

    if(cmd->rcv_length >= 1024)
    {
        return KAL_TRUE;
    }

    for(i = 0; i < size; i++)
    {
        ret = strcmp(cmd->pars[0], track_project_cmd_table[i].cmd_string);
        LOGD(L_APP, L_V9, "ret=%d|%s,%s|%p", ret, cmd->pars[0], track_project_cmd_table[i].cmd_string, track_project_cmd_table[i].cmd_string);
        if(!ret)
        {
            find = KAL_TRUE;
            cmd->rsp_length = 0;
            memset(cmd->rsp_msg, 0, sizeof(cmd->rsp_msg));
            track_is_sms_cmd(1);

            if(!track_project_cmd_table[i].valid)
            {
                LOGD(L_APP, L_V5, "this project disable %s",cmd->pars[0]);
                return KAL_TRUE;
            }

            pass_status = track_cmd_check_authentication(cmd);
            if(cmd->supercmd == 1 || cmd->return_sms.cm_type != CM_SMS || pass_status)
            {
                if(track_project_cmd_table[i].func_ptr != NULL)
                {
                    track_is_recv_cmd_after_boot(1);
                    track_project_cmd_table[i].func_ptr(cmd);
                    break;
                }
            }
            else
            {
                LOGD(L_APP, L_V4, "Password error!");
                sprintf(cmd->rsp_msg, "Password error!");      //�������
            }
            break;
        }
    }
    if(find)
    {
        /*  ָ�����ִ����ֻ�����Ϣ��ȫ���ڴ˷���*/
        track_cust_cmd_reply(&cmd->return_sms, cmd->rsp_msg, cmd->rsp_length);
        return KAL_TRUE;        // ִ�в�����ɾ������
    }
    else
    {
        LOGD(L_APP, L_V5, "invalid project command!");
        return KAL_FALSE; 
    }    
}

