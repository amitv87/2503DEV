/*******************************************************************************************
 * Filename:
 * Author :
 * Date :
 * Comment:
 ******************************************************************************************/

/****************************************************************************
*  Include Files                        ����ͷ�ļ�
*****************************************************************************/
#include "math.h"

#include "track_cust.h"
#include "l4c_common_enum.h"
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
static kal_bool ReStartValid = KAL_FALSE;
/****************************************************************************
*  Global Variable                      ȫ�ֱ���
*****************************************************************************/

/****************************************************************************
*  Global Variable - Extern             ����ȫ�ֱ���
*****************************************************************************/

/*****************************************************************************
*  Global Functions - Extern            �����ⲿ����
******************************************************************************/

/*****************************************************************************
*  Local Functions Define               ���غ�������
******************************************************************************/

#if 0
#endif /* 0 */
/*****************************************************************************
*  Local Functions                      ���غ���
******************************************************************************/
void track_cust_gprs_conn_fail(void *arg);

/*****************************************************************************
*  Global Functions                     ȫ�ֺ���
******************************************************************************/

/******************************************************************************
 *  Function    -  track_cust_periodic_restart
 *
 *  Purpose     -  �����Զ�ʱ��������
 *
 *  Description -  NULL
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 08-12-2012,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_periodic_restart(void *arg)
{
    kal_uint32 par = (kal_uint32)arg;
    LOGD(L_APP, L_V6, "%d", par);
#if defined(__GT300S__) || defined(__GT370__)|| defined (__GT380__)
    if(WORK_MODE_3 == track_cust_get_work_mode()) 
    {
        track_start_timer(TRACK_CUST_PERIODIC_RESTART_TIMER_ID, track_drv_get_one_day_reset_time(), track_cust_periodic_restart, (void*)2);
        CTimer_Start(CTIMER_24HOUR_RESET, track_drv_get_one_day_reset_time(), track_cust_periodic_restart, (void*)2);
        return;
    }
#endif
#if defined(__NF2318__)
	if(G_realtime_data.sleep == 16  || G_parameter.ste_status.lease == 1)
	{
	    track_start_timer(TRACK_CUST_PERIODIC_RESTART_TIMER_ID, track_drv_get_one_day_reset_time(), track_cust_periodic_restart, (void*)2);
        CTimer_Start(CTIMER_24HOUR_RESET, track_drv_get_one_day_reset_time(), track_cust_periodic_restart, (void*)2);
        return;
	}
	else if(track_getIsDownLoadFile() > 0)
	{
		LOGD(L_APP, L_V4, "�ļ������У��ӳ�10��������");
		track_start_timer(TRACK_CUST_PERIODIC_RESTART_TIMER_ID, 600000, track_cust_periodic_restart, (void*)2);
        CTimer_Start(CTIMER_24HOUR_RESET, 600000, track_cust_periodic_restart, (void*)2);
		return;
	}
#endif
#if defined(__FLY_MODE__)
    if(track_cust_flymode_set(192))//����ģʽ�У�����20������������
        {
            track_start_timer(TRACK_CUST_PERIODIC_RESTART_TIMER_ID, track_drv_get_one_day_reset_time(), track_cust_periodic_restart, (void*)2);
            CTimer_Start(CTIMER_24HOUR_RESET, track_drv_get_one_day_reset_time(), track_cust_periodic_restart, (void*)2);
            return;
        }
#endif /* __FLY_MODE__*/
    if(par == 1)
    {
        if(ReStartValid)
        {
            LOGD(L_APP, L_V4, "��⵽�𶯣����������������Ӻ�ʮ����");
            track_start_timer(TRACK_CUST_PERIODIC_RESTART_DELAY_TIMER_ID, 600000, track_cust_periodic_restart, (void*)3);
            CTimer_Start(CTIMER_24HOUR_RESET, 600000, track_cust_periodic_restart, (void*)3);
        }
    }
    else if(par == 2)
    {
        LOGD(L_APP, L_V4, "24Сʱ��������ʮ���Ӿ�ֹ�ж�");
        ReStartValid = KAL_TRUE;
        track_start_timer(TRACK_CUST_PERIODIC_RESTART_DELAY_TIMER_ID, 600000, track_cust_periodic_restart, (void*)4);
        CTimer_Start(CTIMER_24HOUR_RESET, 600000, track_cust_periodic_restart, (void*)4);
    }
    else if(par == 3)
    {
        LOGD(L_APP, L_V6, "%d", par);
        if((G_realtime_data.netLogControl & 128) == 0)
        {
            track_cust_restart(4, 0);
        }
    }
    else if(par == 4)
    {
        LOGD(L_APP, L_V6, "%d", par);
        if((G_realtime_data.netLogControl & 128) == 0)
        {
            track_cust_restart(27, 0);
        }
    }
}

/******************************************************************************
 *  Function    -  track_cust_gps_nodata_restart
 *
 *  Purpose     -  GPS���������������������
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_gps_nodata_restart1(void)
{
    LOGD(L_APP, L_V4, "GPS���ڿ���״̬������20�����������������");
#if defined(__TEST_DEVICE__)
    if(track_rid_given()) return;
#endif /* __TEST_DEVICE__ */
    if(track_is_testmode() || (G_parameter.gprson == 0))
    {
        LOGD(L_APP, L_V7, "����ģʽȡ������");
        return;
    }
    if(track_os_log5_status())
    {
        LOGD(L_APP, L_V7, "ȡ������");
        return;
    }
    if((G_realtime_data.netLogControl & 128) == 0)
    {
		if(track_getIsDownLoadFile() > 0)
		{
        	LOGD(L_APP, L_V7, "�ļ�������");
		}
		else
		{
        	track_cust_restart(5, 0);
		}
		
    }
}

void track_cust_gps_nodata_restart(void)
{
    if(track_is_testmode()) 
    {
        return;
    }
    
    LOGD(L_APP, L_V4, "GPS���ڿ���״̬������20���������������������GPS");
    track_cust_agps_restart();
 //   tr_start_timer(TRACK_CUST_PERIODIC_RESTART_GPS_TIMER_ID, 10000, track_cust_gps_nodata_restart1);
}

void ret_3min_tim(void)
{
	tr_stop_timer(TRACK_YANCHI_THREE_MINE_CHONGQI);
	LOGD(L_APP, L_V4, "��������ѽ�������ӵ���");
	track_cust_restart(28, 0);
	return;
}


void speed_hull_o(void)
{
	static kal_uint32 num=0;
	static kal_uint32 tim=180000;//3//3�������й�����������������֮���ӳ�10����������֤��������
	if(get_speed()==0)
	{
		LOGD(L_APP, L_V4, "�������ж�%d",num);
		if(G_parameter.ste_status.lease == 0)
		{
			//LOGD(L_APP, L_V4, "������������====%d",tim);
			tim=190000;
			tim-=(num*1000);
			if(!(track_is_timer_run(TRACK_YANCHI_THREE_MINE_CLOSESUO)))
			{
				tr_stop_timer(TRACK_YANCHI_THREE_MINE);
				tr_start_timer(TRACK_YANCHI_THREE_MINE_CLOSESUO, tim,ret_3min_tim);
			}
		}
		else
		{
			if(!(track_is_timer_run(TRACK_YANCHI_THREE_MINE)))
			{
				tr_start_timer(TRACK_YANCHI_THREE_MINE, tim,ret_3min_tim);
			}
			num++;
		}
				
	}
	else
	{
		num=0;
		tr_stop_timer(TRACK_YANCHI_THREE_MINE);
		tr_stop_timer(TRACK_YANCHI_THREE_MINE_CHONGQI);
		LOGD(L_APP, L_V4, "20����GPRS����ά�ֻ������ã����������˶�");
		track_cust_gprs_conn_fail((void*)1);
		return;
	}
	//num++;
	tr_start_timer(TRACK_YANCHI_THREE_MINE_CHONGQI, 1000, speed_hull_o);
	return;
}



/******************************************************************************
 *  Function    -  track_cust_gprs_conn_fail
 *
 *  Purpose     -  GPRS����ʧ�ܣ���ʱ����
 *
 *  Description -
 *                 GPRS���ӿ�����20�����޷��ɹ��������ж�GPS��λ״̬
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 22-12-2012,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_gprs_conn_fail(void *arg)
{
    kal_uint32 status = (kal_uint32)arg;

    if(status == 1)
    {
        kal_uint32 period;
        //period = pow(2, G_realtime_data.reStartSystemCount);
        LOGD(L_APP, L_V6, "���20������δ�յ���һ�������ظ��������������ָ�!");
        track_start_timer(TRACK_CUST_PERIODIC_RESTART_GPRS_TIMER_ID, 1200000, track_cust_gprs_conn_fail, (void*)3);
        //track_start_timer(TRACK_CUST_PERIODIC_RESTART_GPRS_TIMER_ID, 180000, track_cust_gprs_conn_fail, (void*)3);
        CTimer_Start(CTIMER_20MIN_RESET, 1500000, track_cust_gprs_conn_fail, (void*)3);
#if defined(__NOFIX_GPSDUP__) || defined(__NF2318__)
       if(!track_is_timer_run( TRACK_CUST_DETECT_NO_VIB_TIMER_ID))
       {
        track_gps_no_location_dup_updata();
       }
#endif
    }
    else if(status == 3)
    {
        LOGD(L_APP, L_V5, "status:%d", status);
        if(G_parameter.gprson == 0 || track_cust_get_work_mode() == WORK_MODE_3||track_is_notquit())
        {
            LOGD(L_APP, L_V4, "20����GPRS����ά�ֻ���ֹͣ����ǰ�ǲ��԰׿����û������ر���GPRS����");
            return;
        }

        if(track_drv_get_sim_type()==2)
        {
            LOGD(L_APP, L_V4, "�׿��ݲ�����");
            return;
        }
        
        if(track_is_in_call() || track_cust_make_call((void *)99)==1)//ͨ������������20������������
        {
            LOGD(L_APP, L_V4, "20����GPRS����ά�ֻ������ã���ǰ����ͨ��");
            track_cust_gprs_conn_fail((void*)1);
            return;
        }
#if defined(__FLY_MODE__)
        if(track_cust_flymode_set(191))//����ģʽ�У�����20������������
        {
            LOGD(L_APP, L_V4, "20����GPRS����ά�ֻ������ã���ǰ���ڷ�����");
            track_cust_gprs_conn_fail((void*)1);
            return;
        }
#endif /* __FLY_MODE__*/
#if defined(__NF2318__)
		/*if(G_realtime_data.sleep == 16 || G_parameter.ste_status.lease == 1)
		{
			LOGD(L_APP, L_V4, "20����GPRS����ά�ֻ������ã���ǰ��ȫ����ģʽ");
			track_cust_gprs_conn_fail((void*)1);
            return;
		}*/
		
		if(G_realtime_data.sleep == 16)
		{
			LOGD(L_APP, L_V4, "20����GPRS����ά�ֻ������ã���ǰ��ȫ����ģʽ");
			track_cust_gprs_conn_fail((void*)1);
            return;
		}
		if(track_getIsDownLoadFile() > 0)
		{
			LOGD(L_APP, L_V4, "20����GPRS����ά�ֻ������ã��ļ�������");
			track_cust_gprs_conn_fail((void*)1);
            return;
		}
		//LOGD(L_APP, L_V4, "=========��������============");
		if(G_parameter.ste_status.lease == 1)
		{
			LOGD(L_APP, L_V4, "=========�޷����ӷ�������״̬�ж�============");
			
			if(get_speed())
			{
				LOGD(L_APP, L_V4, "20����GPRS����ά�ֻ������ã���ǰ������״̬");
				track_cust_gprs_conn_fail((void*)1);
	            return;
			}
			else
			{
				LOGD(L_APP, L_V4, "��ǰ��ֹ��ʼ");
				speed_hull_o();
				return;
			}
		}

#endif

#if defined(__SAME_GT02__)
        if(G_importance_parameter_data.dserver.conecttype== 1&&G_importance_parameter_data.dserver.ip_update&&G_importance_parameter_data.dserver.server_type==1)
        {
            G_importance_parameter_data.dserver.conecttype=0;
            Track_nvram_write(NVRAM_EF_IMPORTANCE_PARAMETER_LID, 1, (void *)&G_importance_parameter_data, NVRAM_EF_IMPORTANCE_PARAMETER_SIZE);
        }
        else if(G_importance_parameter_data.server.conecttype== 1&&G_importance_parameter_data.dserver.ip_update&&G_importance_parameter_data.dserver.server_type==0)
        {
            G_importance_parameter_data.server.conecttype=0;
             Track_nvram_write(NVRAM_EF_IMPORTANCE_PARAMETER_LID, 1, (void *)&G_importance_parameter_data, NVRAM_EF_IMPORTANCE_PARAMETER_SIZE);
        }
        else if(G_importance_parameter_data.server.conecttype== 0&&G_importance_parameter_data.dserver.ip_update&&G_importance_parameter_data.dserver.server_type==0)
        {
             G_importance_parameter_data.server.conecttype=1;
             Track_nvram_write(NVRAM_EF_IMPORTANCE_PARAMETER_LID, 1, (void *)&G_importance_parameter_data, NVRAM_EF_IMPORTANCE_PARAMETER_SIZE);
        }
       else if(G_importance_parameter_data.dserver.conecttype== 0&&G_importance_parameter_data.dserver.ip_update&&G_importance_parameter_data.dserver.server_type==1)
        {
            G_importance_parameter_data.dserver.conecttype=1;
           Track_nvram_write(NVRAM_EF_IMPORTANCE_PARAMETER_LID, 1, (void *)&G_importance_parameter_data, NVRAM_EF_IMPORTANCE_PARAMETER_SIZE);
        }
       
#endif

        //G_realtime_data.reStartSystemCount++;
        //Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);


        if((G_realtime_data.netLogControl & 128) == 0)
        {
            track_cust_restart(28, 0);
        }
    }
}


/******************************************************************************
 *  Function    -  track_cust_fetion_auto_authorize_valid
 *
 *  Purpose     -  Ϊ���Է��㣬�ر�Ϊ���ſ��������Ƶ��Զ���Ȩ
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 10-01-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
kal_uint8 track_cust_fetion_auto_authorize_valid(void *arg)
{
    static kal_uint8 valid = 0;
    switch((kal_uint32)arg)
    {
        case 1:
            LOGD(L_APP, L_V4, "ʮ���ӷ����Զ���Ȩ����");
            valid = 1;
            track_start_timer(TRACK_CUST_FETION_AUTO_AUTHORIZE_TIMER, 600000, track_cust_fetion_auto_authorize_valid, (void *)2);
            break;

        case 2:
            LOGD(L_APP, L_V4, "�����Զ���Ȩ����");
            valid = 0;
            break;
    }
    return valid;
}

/******************************************************************************
 *  Function    -  track_cust_fetion_auto_authorize
 *
 *  Purpose     -  �����Զ���Ȩ����
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 29-07-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_fetion_auto_authorize(track_sms_param *sms_rev)
{
    LOGD(L_APP, L_V5, "");
    if(track_cust_fetion_auto_authorize_valid(NULL) == 1)
    {
        kal_uint8 *start, *end;
        kal_uint8 data[18] =
        {
            0x5E, 0x0C, 0x67, 0x1B, 0x52, 0xA0, 0x60, 0xA8, 0x4E, 0x3A, 0x98, 0xDE, 0x4F, 0xE1, 0x59, 0x7D,
            0x53, 0xCB
        };
        start = track_fun_strWchr(data, sms_rev->TP_UD, sms_rev->TP_UDL);
        if(start == NULL)
        {
            LOGD(L_APP, L_V5, "error 1");
            return;
        }
        if(memcmp(start, data, 18))
        {
            LOGD(L_APP, L_V5, "error 2");
            return;
        }
        track_cust_sms_send(sms_rev->P_TYPE, sms_rev->TPA, 1, "Y", 1);
    }
}


/******************************************************************************
 *  Function    -  track_cust_restart_disable
 *
 *  Purpose     -  ��ֹӦ���߼���������
 *
 *  Description -
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 30-01-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_restart_disable(void)
{
    G_realtime_data.netLogControl |= 128;
}

/******************************************************************************
 *  Function    -  track_cust_sim_restart
 *
 *  Purpose     -  ��SIM����Ӵ��������ܵ����ţ����³�ʼ��SIM�ظ�GSM���總��
 *
 *  Description -  
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 20-12-2013,  Cml  written
 * ----------------------------------------
*******************************************************************************/
void track_cust_sim_restart(void *arg)
{
    static kal_uint8 status = 0;
    kal_uint32 par = (kal_uint32)arg;
    LOGD(L_APP, L_V5, "%d", par);
    if(par == 0)
    {
        if(!track_is_timer_run(TRACK_CUST_SIM_RESTART_TIMER))
        {
            status = 0;
            track_start_timer(TRACK_CUST_SIM_RESTART_TIMER, 60000, track_cust_sim_restart, (void *)1);
        }
    }
    else if(par == 1)
    {
        status = 1;
        l4c_nw_cfun_state_req(RMMI_SRC, 0);  //�������ģʽ
        track_start_timer(TRACK_CUST_SIM_RESTART_TIMER, 3000, track_cust_sim_restart, (void *)10);
    }
    else if(par == 10)
    {
        l4c_nw_cfun_state_req(RMMI_SRC, 1);  //�˳�����ģʽ
    }
    else if(par == 99)
    {
        if(status == 0 && track_is_timer_run(TRACK_CUST_SIM_RESTART_TIMER))
            track_stop_timer(TRACK_CUST_SIM_RESTART_TIMER);
    }
}

