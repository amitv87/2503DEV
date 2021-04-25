/******************************************************************************
 * track_drv_network_license.c - 
 * 
 * Copyright .
 * 
 * DESCRIPTION: - 
 *
 *      ���������֤   	__NETWORK_LICENSE__
 *
 *          ����������Ʒ CTA/3C/CE/FCC ��֤���
 *----------------------------------------------------------------------------------
 * 
 *  1���������ӷ��������������ƣ�����Ӱ����������Ƶ���ԣ�
 *
 *  2������30���Զ��������磬���㽨��GPRS�������ӣ�
 *
 *  3����������112��û�а����򿪻�20s�Զ�����112��������Ƶ�������ӣ����������Զ�������
 *
 *-----------------------------------------------------------------------------------
 * modification history
 * --------------------
 * v1.0   2013-04-02,  chengjun create this file
 * 
 ******************************************************************************/

#if defined (__NETWORK_LICENSE__)

/****************************************************************************
*  Include Files                        ����ͷ�ļ�
*****************************************************************************/
#include "stack_ltlcom.h"
#include "stack_msgs.h"
#include "stdlib.h"
#include "kal_public_defs.h"
#include "track_os_data_types.h"
#include "track_os_timer.h"
#include "l4c_common_enum.h"
#include "GlobalConstants.h"

/*****************************************************************************
*  Define                               �궨��
******************************************************************************/

/*****************************************************************************
*  Typedef  Enum                        ö�ٶ���
*****************************************************************************/

/*****************************************************************************
*  Struct                               ���ݽṹ����
******************************************************************************/
typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8	type;   /* l4c_gprs_connect_type_enum */
}nw_set_gprs_connect_type_req_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_bool	result;
}nw_set_gprs_connect_type_rsp_struct;
    
/*****************************************************************************
*  Local variable                       �ֲ�����
*****************************************************************************/

/****************************************************************************
*  Global Variable                      ȫ�ֱ���
*****************************************************************************/

/****************************************************************************
*  Global Variable - Extern             ����ȫ�ֱ���
*****************************************************************************/

/*****************************************************************************
*  Global Functions - Extern            �����ⲿ����
******************************************************************************/
extern void track_press_key_dial_112(void);
/*****************************************************************************
*  Local Functions                      ���غ���
******************************************************************************/
/******************************************************************************
 *  Function    -  track_drv_make_call_112
 * 
 *  Purpose     -  ����112
 * 
 *  Description -   
 * 
 * modification history
 * ----------------------------------------
 * v1.0  , 02-04-2013,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
static void track_drv_make_call_112(void)
{
    track_press_key_dial_112();
}

#if 1
/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/
#endif


/******************************************************************************
 *  Function    -  track_drv_network_license_key_make_call
 * 
 *  Purpose     -  ������̵�ע�����112
 * 
 *  Description -   
 * 
 * modification history
 * ----------------------------------------
 * v1.0  , 02-04-2013,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_drv_network_license_key_make_call(void)
{
#if defined(__GT300S__)||defined(__GT300S__)
//�������--ע�ᰴ����Ӧ
    track_drv_set_key_handler(track_drv_make_call_112, KEY_SOS, KEY_EVENT_DOWN);

#else
//û�а���--����20���������У���ǰע��

#endif
}

/******************************************************************************
 *  Function    -  track_drv_network_license_eint_make_call
 * 
 *  Purpose     -  �ж��Ͱ�����Ӧ����112
 * 
 *  Description -   
 * 
 * modification history
 * ----------------------------------------
 * v1.0  , 02-04-2013,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_drv_network_license_eint_make_call(void)
{
#if defined (__GT300S__)
//�������--ע�ᰴ����Ӧ

#elif defined(__NT36__)
//�ж��Ͱ���--�жϴ����ص�
    track_drv_make_call_112();

#else
//û�а���--����20���������У���ǰע��

#endif

}

/******************************************************************************
 *  Function    -  track_drv_network_license_auto_make_call
 * 
 *  Purpose     -  û�а����Ŀ���ע��20������112
 * 
 *  Description -   
 * 
 * modification history
 * ----------------------------------------
 * v1.0  , 02-04-2013,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_drv_network_license_auto_make_call(int ms)
{
    //û�а���--����20���������У���ǰע��
    tr_start_timer(NETWORK_LICENSE_AUTO_MAKE_CALL_TIMER_ID,ms,track_drv_make_call_112);
}

/******************************************************************************
 *  Function    -  track_drv_network_license_auto_answer_action
 * 
 *  Purpose     -  ���������Զ�����
 * 
 *  Description -   
 * 
 * modification history
 * ----------------------------------------
 * v1.0  , 02-04-2013,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_drv_network_license_auto_answer_action(void)
{
    track_send_at_accept_call();
}
#if 0//�Ѿ����������ӵ�ʱ�򴴽�//modify by zengping & liujunwu

/******************************************************************************
 *  Function    -  track_drv_network_license_gprs_attach_req
 * 
 *  Purpose     -  GPRS��������
 * 
 *  Description -   
 * 
 * modification history
 * ----------------------------------------
 * v1.0  , 02-04-2013,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_drv_network_license_gprs_attach_req(void)
{
    ilm_struct *ilm_ptr;
    nw_set_gprs_connect_type_req_struct *myMsgPtr = NULL;

    myMsgPtr = (nw_set_gprs_connect_type_req_struct*) construct_local_para(sizeof(nw_set_gprs_connect_type_req_struct),TD_CTRL);
    myMsgPtr->type = L4C_WHEN_NEEDED;

    ilm_ptr = allocate_ilm(MOD_MMI);
    ilm_ptr->src_mod_id = MOD_MMI;
    ilm_ptr->dest_mod_id = MOD_L4C;
    ilm_ptr->sap_id = INVALID_SAP;
    ilm_ptr->msg_id = (msg_type) MSG_ID_MMI_NW_SET_GPRS_CONNECT_TYPE_REQ;
    ilm_ptr->local_para_ptr = (local_para_struct*) myMsgPtr;
    ilm_ptr->peer_buff_ptr = NULL;
    msg_send_ext_queue(ilm_ptr);  
}


/******************************************************************************
 *  Function    -  track_drv_network_license_gprs_attach_rsp
 * 
 *  Purpose     -  GPRS������Ӧ 
 * 
 *  Description -  MOD���̲ü���L4C�յ�����������󣬵������ؽ������˸ú������ᱻִ��! 
 * 
 * modification history
 * ----------------------------------------
 * v1.0  , 02-04-2013,  chengjun  written
 * ----------------------------------------
 ******************************************************************************/
void track_drv_network_license_gprs_attach_rsp(void *msg)
{
    nw_set_gprs_connect_type_rsp_struct *gprs_type=(nw_set_gprs_connect_type_rsp_struct *)msg;

    LOGS("gprs_attach result=%d",gprs_type->result);    
}
#endif /* 0 */


#endif /* __NETWORK_LICENSE__ */

