#if defined(__BMS_PHYLION__)
/****************************************************************************
***file author: Liujw
***creat date:2017-11-10
***introduction:
****************�Ǻ���BMSͨѶЭ��
****************************************************************************/
#include "track_cust.h"
#include "c_vector.h"
/****************************************************************************
***�궨��
****************************************************************************/
#define BMS_CMD_NUM 17//15
//#define __BMS_TEST__
#define __BMS_LOG__
/****************************************************************************
***ö��
****************************************************************************/

typedef enum
{
    bms_cmd1 = 0x08,//��ȡ����¶�
    bms_cmd2 = 0x09,//����ܵ�ѹ
    bms_cmd3 = 0x0A,//ʵʱ����
    bms_cmd4 = 0x0D,//��������ٷֱ�
    bms_cmd5 = 0x0E,//���������ٷֱ�
    bms_cmd6 = 0x0F,//ʣ������
    bms_cmd7 = 0x10,//�������
    bms_cmd8 = 0x17,//ѭ������
    bms_cmd9 = 0x24,//1-7��ص�ѹ
    bms_cmd10 = 0x25,//8-14��ص�ѹ
    bms_cmd11 = 0x47,//��ǰ�����ʱ��
    bms_cmd12 = 0x48,//�������ʱ��
    bms_cmd13 = 0x7E,//��д��Ʒ������
    bms_cmd14 = 0x7F,//�汾��
    bms_cmd15 = 0x20,//��������쳧����
    bms_cmd16 = 0x23,//��ȡ���״̬
    bms_cmd17 = 0x1D,//���������
    bms_max			//���ָ���
} bms_cmd_enum; //bmsָ�

/****************************************************************************
***��̬����
****************************************************************************/
kal_uint8 bmscmd[BMS_CMD_NUM] = {0x08, 0x09, 0x0A, 0x0D, 0x0E, 0x0F, 0x10, 0x17, 0x24, 0x25, 0x47, 0x48, 0x7E, 0x7F, 0x20,0x23, 0x1D};
static int bmsinfoup = 0;
static int readall = 0;
static controller_data_struct bmsDataTemp = {0};
static U8 isreading = 0;// 1 һ��ѭ���Ѿ���ʼ 0 δ��ʼ���߽���

//201904
static int num_baojing = 0; //�ϱ�BMS����
static int num_bmsb_send = 0; //���ʹ���
static int num_bmsb_read = 0; //���մ���
static int baojingbiaozhi = 0; //������־Ϊ
static int ii = 99; //2019
//201904

//��ǵ��SN��16λ����20λ
static kal_uint8 battery_sn = 0;

/****************************************************************************
***���ñ���
****************************************************************************/
extern controller_data_struct cData;

/****************************************************************************
***���ú���
****************************************************************************/
extern void track_cust_ele_off_gpio(void* arg);
extern void track_cust_restart(kal_uint8 mode, kal_uint32 sec);
/****************************************************************************
***���غ���
****************************************************************************/
void track_cust_bms_codebar_message(kal_uint8 *msg);
extern kal_uint8 track_bms_data_send(void* arg);


//201904
void track_msg_get_bms(void);
void track_msg_get_bms_make(void);
void if_same_msg(void);
void track_cust_bms_command(void);
//201904
void track_cust_bms_status_alarm(kal_uint8 s_type);
//��ȡ������кŵ�16����20λ�ı�־ 0 16  1 20λ
kal_uint8 get_bett_sn(kal_uint8 value);//���ص��SN��16����20λ�ı�־


U8 track_cust_bms_reading()
{
    return isreading;
}

//201904
//fghjkltghjklghjk���»�ȡBMS��Ϣ
void track_msg_get_bms(void)
{
    ii = 0;
    LOGD(L_APP, L_V1, "");
    track_cust_bms_upload(0);
    //isreading = 0;
    if(track_is_timer_run(TRACK_CUST_BMS_CMD_TIMER_ID))
    {
        tr_stop_timer(TRACK_CUST_BMS_CMD_TIMER_ID);
    }
    track_cust_bms_command();


}
//kkkkkkk�����֮��һ�д��¿�ʼ��ȡBMS��Ϣ�Լ�����
void track_msg_get_bms_make(void)
{
    LOGD(L_APP, L_V1, "");

    num_baojing = 0; //�ϱ�BMS����
    num_bmsb_send = 0; //���ʹ���
    num_bmsb_read = 0; //���մ���
    baojingbiaozhi = 0; //������־Ϊ
    track_msg_get_bms();
}
//kkkkkk�������ζ�û��������BMS��Ϣ�������жϣ������ν��б���

void if_same_msg(void)
{
    LOGD(L_APP, L_V1, "BMS Message,read:%d,send:%d,num_baojing:%d,isreading:%d", num_bmsb_read, num_bmsb_send, num_baojing, isreading);

    if(num_bmsb_read >= num_bmsb_send)
    {
        num_bmsb_send = 0;
        num_bmsb_read = 0;
        num_baojing = 0;
        if(baojingbiaozhi == 1)
        {
            baojingbiaozhi = 0;

            track_cust_N07_paket_04(ALARM_BMS_WARNING, 0x00);
        }
        return;
    }
    if(num_bmsb_read < num_bmsb_send)//num_bmsb_send ������15��
    {
        num_bmsb_send = 0;
        num_bmsb_read = 0;
        num_baojing++;
        if(num_baojing < 2)
        {
            track_msg_get_bms();
            return;
        }
    }
    LOGD(L_APP, L_V1, "BMS Message,baojingbiaozhi:%d", baojingbiaozhi);
    if(num_baojing == 2 && baojingbiaozhi == 0) //����δ�յ�������BMS������BMS����
    {
        num_baojing = 0;
        baojingbiaozhi = 1; //ֻ����һ��
        num_bmsb_read = 0;//����֮��Ѷ�ȡ��������
        track_cust_N07_paket_04(ALARM_BMS_WARNING, 0x01);
    }

}


//201904


int track_cust_bms_upload(int param)
{
    if(param != 99)
    {
        bmsinfoup = param;
    }
    return bmsinfoup;
}
int track_cust_bms_read_all(U8 arg)
{
    if(arg != 99)
    {
        readall = arg;
    }
    return readall;
}
static kal_uint16 checksum(kal_uint8 *srcdata, kal_uint8 len)
{
    int i = 0;
    kal_uint16 crc = 0;
    for(i = 0; i < len; i++)
    {
        crc += srcdata[i];
        //#if defined(__BMS_LOG__)
        //LOGD(L_BMS, L_V8, "crc:%X,src", crc, srcdata[i]);
        //#endif
    }
    return crc;
}
/**
**Author:Liujw Date:2017-11-13
**Function name :track_cust_bms_data_req
**Description :����BMS����
**/
void track_cust_bms_data_req(kal_uint8 cmd)
{
    kal_uint8 sendbuf[30];
    kal_uint8 sumL = 0, sumH = 0;
    kal_uint16 sum = 0;
    int i = 0;
    sendbuf[0] = 0x3A;//��ʼ�ֽ�
    sendbuf[1] = 0x16;//��ַ�ֽ�
    sendbuf[2] = cmd;//����
    sendbuf[3] = 0x01;//���ݳ���
    sendbuf[4] = 0x00;
    //�ۼ�У��
    sum = sendbuf[1] + sendbuf[2] + sendbuf[3] + sendbuf[4];
    sumL = sum & 0x00FF;
    sumH = (sum >> 8);
    sendbuf[5] = sumL;
    sendbuf[6] = sumH;
    sendbuf[7] = 0x0D;
    sendbuf[8] = 0x0A;//������
    LOGH2(L_BMS, L_V6, "", sendbuf, 9);
    U_PutUARTBytes(uart_port1, sendbuf, 9);//uart_port3

}
/**
**Author:Liujw Date:2017-11-13
**Function name :track_cust_bms_data_analyze
**Description :���ԭʼ���ݣ���ֵ�ṹ�� cData
**/
static void track_cust_bms_data_analyze(kal_uint8 *data, kal_uint8 cmd, kal_uint16 len)
{
#if defined(__BMS_TEST__)
    kal_uint8 bmslog[15];
#endif
    kal_uint8 batTeamState[8] = 0;//�����״̬��Ϣ
    static kal_uint8 read_state = 0;
    kal_uint8 tmp[2];
    int i = 0;
    //��Ϸ�벻��20�Ĳ�4λ0
    kal_uint8 add_sn[4]={0};
    LOGH2(L_BMS, L_V8, "", data, len);
    //LOGD(L_BMS, L_V8, "cmd:%0.2X", cmd);
    switch(cmd)
    {
        case 0x08://������ڲ��¶�
            bmsDataTemp.temperature[1] = *(data + i);
            i += 1;
            bmsDataTemp.temperature[0] = *(data + i);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "����ڲ��¶�0x%02X,0x%02X", bmsDataTemp.temperature[0], bmsDataTemp.temperature[1]);
#endif
            if(read_state == 0) read_state = 1;
            break;
        case 0x09://������ܵ�ѹ
            bmsDataTemp.totalValtage[1] = *(data + i);
            i += 1;
            bmsDataTemp.totalValtage[0] = *(data + i);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "������ܵ�ѹ0x%02X,0x%02X", bmsDataTemp.totalValtage[0], bmsDataTemp.totalValtage[1]);
#endif
            break;
        case 0x0A://ʵʱ����
            bmsDataTemp.currentL = *(data + i);//tmp[0] = *(data + i);
            i += 1;
            bmsDataTemp.currentH = *(data + i);//tmp[1] = *(data + i);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "ʵʱ����0x%02X,0x%02X", bmsDataTemp.currentH, bmsDataTemp.currentL);
#endif
            break;
        case 0x0D://��������ٷֱ�
            bmsDataTemp.relativeCapacityPer = *(data + i);
            LOGD(L_BMS, L_V5, "��������ٷֱ�0x%02X", bmsDataTemp.relativeCapacityPer);
            break;
        case 0x0E://���������ٷֱ�
            bmsDataTemp.absoluteCapacityPer = *(data + i);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "���������ٷֱ�0x%02X", bmsDataTemp.absoluteCapacityPer);
#endif
            break;
        case 0x0F://ʣ������
            bmsDataTemp.leftCapatiryL = *(data + i);
            i += 1;
            bmsDataTemp.leftCapatiryH = *(data + i);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "ʣ������0x%02X,0x%02X", bmsDataTemp.leftCapatiryH, bmsDataTemp.leftCapatiryL);
#endif
            break;
        case 0x10://��������
            bmsDataTemp.absoluteFullCapacity[1] = *(data + i);
            i += 1;
            bmsDataTemp.absoluteFullCapacity[0] = *(data + i);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "��������0x%02X,0x%02X", bmsDataTemp.absoluteFullCapacity[0], bmsDataTemp.absoluteFullCapacity[1]);
#endif
            break;
        case 0x17://�����ѭ��ʹ�ô���
            bmsDataTemp.circulationOfBat[1] = *(data + i);
            i += 1;
            bmsDataTemp.circulationOfBat[0] = *(data + i);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "�����ѭ��ʹ�ô���0x%02X,0x%02X", bmsDataTemp.circulationOfBat[0], bmsDataTemp.circulationOfBat[1]);
#endif
            break;
        case 0x24://1~7�ڵ�ص�ѹ
            memcpy(&bmsDataTemp.valtage[0], &data[i], 14);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "1-7�ڵ�ص�ѹ0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X", bmsDataTemp.valtage[0], bmsDataTemp.valtage[1], \
                 bmsDataTemp.valtage[2], bmsDataTemp.valtage[3], bmsDataTemp.valtage[4], bmsDataTemp.valtage[5], bmsDataTemp.valtage[6], bmsDataTemp.valtage[7], bmsDataTemp.valtage[8], bmsDataTemp.valtage[9], \
                 bmsDataTemp.valtage[10], bmsDataTemp.valtage[11], bmsDataTemp.valtage[12], bmsDataTemp.valtage[13]);
#endif
            break;
        case 0x25://8~14�ڵ�ص�ѹ
            memcpy(&bmsDataTemp.valtage[14], &data[i], 14);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "8-14�ڵ�ص�ѹ0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X", bmsDataTemp.valtage[14], bmsDataTemp.valtage[15], \
                 bmsDataTemp.valtage[16], bmsDataTemp.valtage[17], bmsDataTemp.valtage[18], bmsDataTemp.valtage[19], bmsDataTemp.valtage[20], bmsDataTemp.valtage[21], bmsDataTemp.valtage[22], bmsDataTemp.valtage[23], \
                 bmsDataTemp.valtage[24], bmsDataTemp.valtage[25], bmsDataTemp.valtage[26], bmsDataTemp.valtage[27]);
#endif
            break;
        case 0x47://��ǰ�����ʱ��
            bmsDataTemp.chargeInterval[1] = *(data + i);
            i += 1;
            bmsDataTemp.chargeInterval[0] = *(data + i);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "��ǰ�����ʱ��0x%02X,0x%02X", bmsDataTemp.chargeInterval[0], bmsDataTemp.chargeInterval[1]);
#endif
            break;
        case 0x48://�������ʱ��
            bmsDataTemp.largeChargeInterval[1] = *(data + i);
            i += 1;
            bmsDataTemp.largeChargeInterval[0] = *(data + i);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "�������ʱ��0x%02X,0x%02X", bmsDataTemp.largeChargeInterval[0], bmsDataTemp.largeChargeInterval[1]);
#endif
            break;
        case 0x7E://��д��Ʒ������
        	if(len == 20)
        	{	if(get_bett_sn(99) != 1)
        		{
        			get_bett_sn(1);
        		}
				memcpy(&bmsDataTemp.barCode[0], &data[i], 20);
        	}
        	else if(len == 16)
        	{	if(get_bett_sn(99) != 0)
        		{
        			get_bett_sn(0);
        		}
            	memcpy(&bmsDataTemp.barCode[0], &data[i], 16);
            	memcpy(&bmsDataTemp.barCode[16], add_sn, 4);
            }
            track_cust_bms_codebar_message(&bmsDataTemp.barCode[0]);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "��д��Ʒ������0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X", bmsDataTemp.barCode[0]\
                 , bmsDataTemp.barCode[1], bmsDataTemp.barCode[2], bmsDataTemp.barCode[3], bmsDataTemp.barCode[4], bmsDataTemp.barCode[5], bmsDataTemp.barCode[6], bmsDataTemp.barCode[7], bmsDataTemp.barCode[8]\
                 , bmsDataTemp.barCode[9], bmsDataTemp.barCode[10], bmsDataTemp.barCode[11], bmsDataTemp.barCode[12], bmsDataTemp.barCode[13], bmsDataTemp.barCode[14], bmsDataTemp.barCode[15]);
#endif
            break;
        case 0x7F://���汾��
            bmsDataTemp.versionNum[0] = *(data + i);
            i += 1;
            bmsDataTemp.versionNum[1] = *(data + i);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "���汾��0x%02X,0x%02X", bmsDataTemp.versionNum[0], bmsDataTemp.versionNum[1]);
#endif
            break;
        case 0x20://����������쳧����
            memset(&bmsDataTemp.valtageManufacturer[0], 0, 16);
            if(len > 16) len = 16;
            memcpy(&bmsDataTemp.valtageManufacturer[0], &data[i], len);
#if defined(__BMS_LOG__)
            LOGD(L_BMS, L_V5, "����������쳧����0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X", bmsDataTemp.valtageManufacturer[0]\
                 , bmsDataTemp.valtageManufacturer[1], bmsDataTemp.valtageManufacturer[2], bmsDataTemp.valtageManufacturer[3], bmsDataTemp.valtageManufacturer[4], bmsDataTemp.valtageManufacturer[5], bmsDataTemp.valtageManufacturer[6], bmsDataTemp.valtageManufacturer[7], bmsDataTemp.valtageManufacturer[8], bmsDataTemp.valtageManufacturer[9]\
                 , bmsDataTemp.valtageManufacturer[10], bmsDataTemp.valtageManufacturer[11], bmsDataTemp.valtageManufacturer[12], bmsDataTemp.valtageManufacturer[13], bmsDataTemp.valtageManufacturer[14], bmsDataTemp.valtageManufacturer[15]);
#endif
            if(read_state == 1) read_state = 2;

            break;
        case 0x23:
            LOGD(L_BMS, L_V5, "��ȡ���״̬");
            memcpy(batTeamState, &data[i], 8);
			track_cust_bms_status_alarm(batTeamState[1]);
            break;
        case 0x1D:
            LOGD(L_BMS, L_V5, "���������");
            break;
        default:
            break;

    }
    if(read_state == 2)//(cmd == 0x20)
    {
        read_state = 0;
        memcpy(&cData, &bmsDataTemp, sizeof(controller_data_struct));
        readall = 1;
        if(track_soc_login_status() && !bmsinfoup)
        {
            tr_stop_timer(TRACK_CUST_BMS_DATA_SEND_OVERTIME_TIMER_ID);
            track_start_timer(TRACK_CUST_BMS_DATA_SEND_OVERTIME_TIMER_ID, 1000, track_bms_data_send, (void*)0);
            //track_cust_N07_paket_03();
            bmsinfoup = 1;
        }
    }
}
/**
**function author:Liujw
**function name:track_cust_bms_data_receive ;Date :2017-11-10
**Description:����BMS�������ݣ�ƴ������
HEX:cmd_bmscmd()
    3A 16 20 01 00 37 00 0D 0A


[16:34:28.943]�ա���[8]track_project_uart_receice_data_notify() port:2,len:18
HEX:track_cust_bms_data_receive()
    3A 16 20 0A 50 48 59 4C 49 4F 4E 20 47 58 22 03 0D 0A
**/
void track_cust_bms_data_receive(kal_uint8 *data, kal_uint16 len)
{
    kal_uint8 cmd = 0, addid = 0, cmdlen = 0;
    kal_uint8 i = 0, j = 0, k = 0, l = 0, m = 0;
    static kal_bool find = KAL_FALSE;
    static kal_uint32 t_len = 0;
    //static kal_uint8 buf[500];
    //kal_uint8 h_buf[100];
    kal_uint16 crc = 0;
    kal_uint8 c_l = 0, c_h = 0;
#if defined(__BMS_LOG__)
    LOGH2(L_OBD, L_V8, "", data, len);
#endif
    if(len <= 0) return;
#if 0
    if(t_len + len > 500)
    {
        memset(buf, 0, 500);
        t_len = 0;
        return;//���������������Ϊ���оƬ����
    }
    memcpy(&buf[t_len], data, len);
    t_len += len;
    for(i = 0; i < t_len; i++)
    {
        if(buf[i] == 0x3A)
        {
            k = i;
            find = KAL_TRUE;
        }
        if(buf[i-1] == 0x0D && buf[i] == 0x0A)
        {
            l = i;
            if(find)
            {
                m = l - k + 1;
                memcpy(h_buf, &buf[k], m);
                LOGH2(L_BMS, L_V6, "", h_buf, m);
                crc = checksum(&h_buf[1], m - 5);
                c_l = crc & 0xFF;
                c_h = crc >> 8;
                LOGD(L_BMS, L_V8, "CRC L:%2X,CRC H:%2X,%2X,%2X", c_l, c_h, h_buf[m-4], h_buf[m-3]);
                if(h_buf[m-3] == c_h && h_buf[m-4] == c_l)//У��ͨ��
                {
                    cmd = h_buf[2];
                    cmdlen = h_buf[3];
                    track_cust_bms_data_analyze(&h_buf[4], cmd, cmdlen);
                }
            }
            find = KAL_FALSE;
        }

    }

    if(l + 1 < t_len)
    {
        kal_uint8 *tempbuf = NULL;
        int last = t_len - (l + 1);
        tempbuf = &buf[l+1];
        memset(buf, 0, 500);
        memcpy(buf, tempbuf, last);
        t_len = last;
        Ram_Free(tempbuf);
    }
    else
    {
        memcpy(buf, 0, 500);
        t_len = 0;
    }
#endif
    /*һ��������һ��һ�����մ��룬�����޹�*/
    //���Դ���
    if(data[0] != 0x3A)
    {
        return;
    }
    if(data[i] == 0x3A)
    {
        addid = data[i+1];
        cmd = data[i+2];
        cmdlen = data[i+3];
    }

    LOGD(L_BMS, L_V5, "cmd:%02X,addid:%x,cmdlen:%x", cmd, addid, cmdlen);
    j = cmdlen + 3;
    j += 2;
    if(data[j+1] == 0x0D && data[j+2] == 0x0A)
    {
    //���յ�0xF0��һ����������
    	if(cmd == 0x55)
    	{	

    		track_cust_bms_data_req(0x55);
    		track_cust_restart(77, 3);
    		return;
    	}
        num_bmsb_read++;//201904

        i += 4;
        track_cust_bms_data_analyze(data + i, cmd, cmdlen);
    }

}
void track_cust_bms_command(void)
{
    static kal_uint8 notsleep = 0;
    static int i = 0;
    kal_uint16 interval = 0;
    if(G_realtime_data.sleep == 16)
    {
        LOGD(L_BMS, L_V5, "sleeping now!!!");
        return;
    }
    if(get_BmsUart() == 0)
    {
        LOGD(L_BMS, L_V5, "Message:BMS Uart init not ready!,%d", get_BmsUart());
        return;
    }

    //201904
    if(ii == 0)
    {
        i = ii;
        notsleep = 0;
    }

    if(i == 0)//ѭ����ʼ������
    {
        ii = 99;//2019
        //isreading = 1;
        //Ӧ�ý���ȡ�ļ�������������
        num_bmsb_read = 0;
        if(notsleep == 0)
        {
            track_drv_sleep_enable(SLEEP_OBD_MOD, FALSE);
            notsleep = 1;
            tr_start_timer(TRACK_CUST_BMS_CMD_TIMER_ID, 500, track_cust_bms_command);
            return;
        }
        else if(notsleep == 1)
        {
            notsleep = 2;
            memset(&bmsDataTemp, 0, sizeof(controller_data_struct));
            track_cust_bms_data_req(bmscmd[0]);
            tr_start_timer(TRACK_CUST_BMS_CMD_TIMER_ID, 500, track_cust_bms_command);
            return;
        }
    }
    LOGD(L_BMS, L_V5, "BMS GET i=%d,isreading:%d", i, isreading);
    if(i < BMS_CMD_NUM)
    {
        //if(i == 0)
        //{

        //isreading = 1;
        //}
        track_cust_bms_data_req(bmscmd[i]);
        i++;
        interval = 1;
    }
    else
    {
        num_bmsb_send = i;
        i = 0;
        interval = 120 * 3;
        memcpy(&cData, &bmsDataTemp, sizeof(controller_data_struct));
        track_drv_sleep_enable(SLEEP_OBD_MOD, TRUE);
        notsleep = 0;
        //isreading = 0;
    }
    tr_start_timer(TRACK_CUST_BMS_CMD_TIMER_ID, interval * 330, track_cust_bms_command);
    if(num_bmsb_send >= BMS_CMD_NUM)// && isreading == 0)
    {
        if_same_msg();
    }
}

void track_cust_bms_codebar_message(kal_uint8 *msg)
{
    int i = 0;
    static kal_bool isfirst = KAL_TRUE;
    kal_bool ischange = KAL_FALSE;
    static kal_uint8 codebar[20];
    kal_uint8 barCodetemp[20];
//	if(isfirst)
//	{
//		isfirst = KAL_FALSE;
//		return;
//	}
    //LOGS("%s",cData.barCode);
    memcpy(barCodetemp, &cData.barCode[0], 20);
    memcpy(codebar, msg, 20);

    LOGH(L_APP, L_V5, barCodetemp, 20);
    LOGH(L_APP, L_V5, codebar, 20);
    if(track_cust_bms_read_all(99) == 0)
    {
        return;
    }
    for(i = 0 ; i < 20; i++)
    {
        if(codebar[i] != barCodetemp[i])
        {
            ischange = KAL_TRUE;
            break;
        }
    }

    LOGD(L_BMS, L_V5, "%d", ischange);
    if(ischange)
    {
        track_cust_bms_upload(0);
        if(G_realtime_data.exbatlock != 1)
        {
            track_start_timer(TRACK_CUST_ELE_LOCK_DELAY_TIMER_ID, 60 * 1000, track_cust_ele_off_gpio, (void *)1);
        }
    }
}
void track_bms_struct_copy(controller_data_struct *c_data_struct)
{
    memcpy(c_data_struct, &cData, sizeof(controller_data_struct));
}
kal_uint8 track_bms_data_send(void* arg)
{
    static kal_uint8 up = 0;
    static U8 up_times = 0;
    kal_uint8 par = (kal_uint8)arg;
    if(par == 0) up = 0;
    LOGD(L_BMS, L_V5, "get bms data delay,send overtime");
    if(up == 0 && par == 0)
    {
        up = 1;//���ڷ���03��
        if(track_soc_login_status())
        {
            track_cust_N07_paket_03();
        }
        if(up_times < 2)
        {
            track_start_timer(TRACK_CUST_BMS_DATA_SEND_OVERTIME_TIMER_ID, 20000, track_bms_data_send, (void*)0);
            up_times ++;
        }
        else
        {
            up_times = 0;
        }
    }
    else if(par == 1 && up == 1)
    {
        up = 2;
        up_times = 0;
        tr_stop_timer(TRACK_CUST_BMS_DATA_SEND_OVERTIME_TIMER_ID);
        track_cust_bms_upload(1);
    }
    return up;
}

void track_cust_bms_status_alarm(kal_uint8 s_type)
{
    kal_uint8 s_alarm = 2;
    //static kal_uint8 alarm_type[7] = {0};//0x11 ��ѹ 0x12 ��ѹ 0x13 ����� 0x14 ��ص��� 0x15 һ������ 0x16 �������� 0x17 ��ظ����¶�
    //G_realtime_data.bms_alarm����alarm_type
    //��ͬʱ�����ı�������if�����жϺ�һ��
    LOGD(L_BMS, L_V5, "------------>BMS ALARM  s_type:%d",s_type);
    if((s_type & 1) || G_realtime_data.bms_alarm[0] == 1) //��ѹ��������
    {
        if(G_realtime_data.bms_alarm[0] == 0) //δ��������
        {
            s_alarm = 1;
            G_realtime_data.bms_alarm[0] = 1;
        }
        else if((s_type & 1) == 0 /*&& alarm_type[0] == 1*/)
        {
            s_alarm = 0;
            G_realtime_data.bms_alarm[0] = 0;
        }
        if(s_alarm != 2)
            track_cust_N07_paket_04(ALARM_BMS_COV, s_alarm);
        s_alarm = 2;
    }
    if((s_type & 2) || G_realtime_data.bms_alarm[1] == 1)//��ѹ����
    {
        if(G_realtime_data.bms_alarm[1] == 0) //δ��������
        {
            s_alarm = 1;
            G_realtime_data.bms_alarm[1] = 1;
        }
        else if((s_type & 2) == 0 /*&& alarm_type[1] == 1*/)
        {
            s_alarm = 0;
            G_realtime_data.bms_alarm[1] = 0;
        }
        if(s_alarm != 2)
            track_cust_N07_paket_04(ALARM_BMS_CUV, s_alarm);
        s_alarm = 2;
    }
    if((s_type & 4) || G_realtime_data.bms_alarm[2] == 1)//����������
    {
        if(G_realtime_data.bms_alarm[2] == 0) //δ��������
        {
            s_alarm = 1;
            G_realtime_data.bms_alarm[2] = 1;
        }
        else if((s_type & 4) == 0/*&& alarm_type[2] == 1*/)
        {
            s_alarm = 0;
            G_realtime_data.bms_alarm[2] = 0;
        }
        if(s_alarm != 2)
            track_cust_N07_paket_04(ALARM_BMS_OC, s_alarm);
        s_alarm = 2;
    }
    if((s_type & 8) || G_realtime_data.bms_alarm[3] == 1)//���±���
    {
        if(G_realtime_data.bms_alarm[3] == 0) //δ��������
        {
            s_alarm = 1;
            G_realtime_data.bms_alarm[3] = 1;
        }
        else if((s_type & 8) == 0 /*&& alarm_type[3] == 1*/)
        {
            s_alarm = 0;
            G_realtime_data.bms_alarm[3] = 0;
        }
        if(s_alarm != 2)
            track_cust_N07_paket_04(ALARM_BMS_TOVER2, s_alarm);
        s_alarm = 2;
    }
    if((s_type & 16) || G_realtime_data.bms_alarm[4] == 1)//�ŵ��������
    {
        if(G_realtime_data.bms_alarm[4] == 0) //δ��������
        {
            s_alarm = 1;
            G_realtime_data.bms_alarm[4] = 1;
        }
        else if((s_type & 16) == 0 /*&& alarm_type[4] == 1*/)
        {
            s_alarm = 0;
            G_realtime_data.bms_alarm[4] = 0;
        }
        if(s_alarm != 2)
            track_cust_N07_paket_04(ALARM_BMS_LEVEL1_OLOAD, s_alarm);
        s_alarm = 2;
    }
#if 0
    if(s_type & 16)//�ŵ��������
    {
        if(alarm_type[5] == 0) //δ��������
        {
            s_alarm = 1;
        }
        else if(s_type & 16 == 0 && alarm_type[5] == 1)
        {
            s_alarm = 0;
        }
        if(s_alarm != 2)
            track_cust_N07_paket_04(ALARM_BMS_LEVEL2_OLOAD, s_alarm);
    }
#endif
    if((s_type & 32) || G_realtime_data.bms_alarm[6] == 1)//�ŵ��������
    {
        if(G_realtime_data.bms_alarm[6] == 0) //δ��������
        {
            s_alarm = 1;
            G_realtime_data.bms_alarm[6] = 1;
        }
        else if((s_type & 32) == 0 /*&& alarm_type[6] == 1*/)
        {
            s_alarm = 0;
            G_realtime_data.bms_alarm[6] = 0;
        }
        if(s_alarm != 2)
            track_cust_N07_paket_04(ALARM_BMS_TOVER, s_alarm);
        s_alarm = 2;
    }
   // if(iswrite)
    {
        Track_nvram_write(NVRAM_EF_REALTIME_LID, 1, (void *)&G_realtime_data, NVRAM_EF_REALTIME_SIZE);
    }
}
#endif/*__BMS_PHYLION__*/

kal_uint8 get_bett_sn(kal_uint8 value)//���ص��SN��16����20λ�ı�־
{
	if(value != 99)
	{
		battery_sn = value;
	}
	return battery_sn;
}

