/****************************************************************************
*  Include Files                        ����ͷ�ļ�
*****************************************************************************/
#include "track_cust.h"
#include "track_os_ell.h"

/*****************************************************************************
*  Define                               �궨��
******************************************************************************/
static char *blank = {""}, *s_ON = {"ON"}, *s_OFF = {"OFF"}, *s_Error = {"ERROR"};
#define ON_OFF(value) (value==0?s_OFF:(value==1?s_ON:s_Error))

/*****************************************************************************
*  Typedef  Enum                        ö�ٶ���
*****************************************************************************/

/*****************************************************************************
*  Struct                               ���ݽṹ����
******************************************************************************/

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

/*****************************************************************************
*  Local Functions Define               ���غ�������
******************************************************************************/

/*****************************************************************************
*  Local Functions                      ���غ���
******************************************************************************/

/******************************************************************************
 *  Function    -  
 *
 *  Purpose     -  
 *
 *  Description -  NULL
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 01-11-2017,  Cjj  written
 * ----------------------------------------
*******************************************************************************/

/******************************************************************************
 *  Function    -  track_fun_ASCII_to_BCD8
 *
 *  Purpose     -  ��BCD��ʽ����(8λ)
 *                      data(ascii)=123456789012345
 *                      BCD(hex)   =01 23 45 67 89 01 23 45
 *
 *  Description -  NULL
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 01-11-2017,  Cjj  written
 * ----------------------------------------
*******************************************************************************/
void track_fun_ASCII_to_BCD8(kal_uint8 *dec,kal_uint8 *src)
{
    char data[17] = {0};
    char BCD[8];
    int len,i;
    len = strlen(src);
    if(len > 16)
    {
        LOGD(L_APP, L_V5,"Error: Number length exceeds 16 characters!");
        return;
    }
    sprintf(data,"%016s",src);
    len = strlen(data);
    if(len%2 > 0)len += 1;
    for(i=0;i<len;i+=2)
    {
        BCD[i/2]=(data[i]<<4)|(data[i+1]&0x0F);
    }
    LOGD(L_APP, L_V5,"BCD=%02x%02x%02x%02x%02x%02x%02x%02x\n",BCD[0],BCD[1],BCD[2],BCD[3],BCD[4]
        ,BCD[5],BCD[6],BCD[7]);
    LOGH(L_APP, L_V5, BCD, 8);
    memcpy(dec, BCD, 8);
}
/******************************************************************************
 *  Function    -  track_fun_ASCII_to_BCD2
 *
 *  Purpose     -  ��BCD��ʽ����(2λ)
 *                      data(ascii)=123
 *                      BCD(hex)   =01 23 
 *
 *  Description -  NULL
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 01-11-2017,  Cjj  written
 * ----------------------------------------
*******************************************************************************/
void track_fun_ASCII_to_BCD2(kal_uint8 *dec,kal_uint8 *src)
{
    char data[5] = {0};
    char BCD[2];
    int len,i;
    len = strlen(src);
    if(len > 4)
    {
        LOGD(L_APP, L_V5,"Error: Number length exceeds 4 characters!");
        return;
    }
    sprintf(data,"%04s",src);
    len = strlen(data);
    if(len%2 > 0)len += 1;
    for(i=0;i<len;i+=2)
    {
        BCD[i/2]=(data[i]<<4)|(data[i+1]&0x0F);
    }
    LOGD(L_APP, L_V5,"BCD=%02x%02x\n",BCD[0],BCD[1]);
    LOGH(L_APP, L_V5, BCD, 2);
    memcpy(dec, BCD, 2);
}

/******************************************************************************
 *  Function    -  DataPacketEncode67
 *
 *  Purpose     -  67Э����
 *
 *  Description -  NULL
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 01-11-2017,  Cjj  written
 * ----------------------------------------
*******************************************************************************/
static kal_uint16 DataPacketEncode67(U8 cmd, U8 *sendData, U8 len)
{
    U16 tmp, tmp1;

    //��ʼλ
    sendData[0] = 0x67;
    sendData[1] = 0x67;

    //Э���
    sendData[2] = cmd;
    
    //����
    track_fun_reverse_16(&sendData[3],(len - 7));

    //��Ϣ���к�
    tmp = getSerialNumber();
    track_fun_reverse_16(&sendData[5],tmp);
    LOGD(L_APP, L_V5, "SerialNumber:%.2X%.2X", sendData[5], sendData[6]);

    //CRCУ��
    tmp1 = GetCrc16(sendData, len - 2);
    sendData[len - 2] = (tmp1 & 0xFF00) >> 8;
    sendData[len - 1] = (tmp1 & 0x00FF);

    return tmp;
}

/******************************************************************************
 *  Function    -  DataPackingLocation_Mask
 *
 *  Purpose     -  ��λ����
 *
 *  Description -  ���������ݶ��ܻ�ȡʱ:GPS��λ(GPS����)����һ��LBS��WIFI��
 *              GPS����λʱ(GPS������)��������LBS��WIFI��    
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 01-11-2017,  Cjj  written
 * ----------------------------------------
*******************************************************************************/
static void DataPackingLocation_Mask(U8 *sendbuf, track_gps_data_struct *gd,LBS_Multi_Cell_Data_Struct *lbs)
{
    if(gd->status >=2 )//��λ
    {
        sendbuf[0] |= 0x01;//��λ���ݷ���
        if(lbs->MainCell_Info.cell_id != 0)
        {
            sendbuf[0] |= 0x02;//LBS���ݷ���
        }
        if(wifi_status.wifi_sum !=0)
        {
            sendbuf[0] |= 0x10;//WIFI���ݷ���
        }
    }
    else//δ��λ
    {
        if(lbs->MainCell_Info.cell_id != 0)
        {
            sendbuf[0] |= 0x02;//LBS1���ݷ���
        }
        if(lbs->NbrCell_Info[0].cell_id != 0)
        {
            sendbuf[0] |= 0x04;//LBS2���ݷ���
        }
        if(lbs->NbrCell_Info[1].cell_id != 0)
        {
            sendbuf[0] |= 0x08;//LBS3���ݷ���
        }
        if(wifi_status.wifi_sum ==1)
        {
            sendbuf[0] |= 0x10;//WIFI1���ݷ���
        } 
        if(wifi_status.wifi_sum ==2)
        {
            sendbuf[0] |= 0x20;//WIFI2���ݷ���
        } 
        if(wifi_status.wifi_sum >=3)
        {
            sendbuf[0] |= 0x40;//WIFI3���ݷ���
        }        
    }
}

/******************************************************************************
 *  Function    -  DataPackingGps_15
 *
 *  Purpose     -  ���GPS�����ֶ�����
 *
 *  Description -  NULL
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 01-11-2017,  Cjj  written
 * ----------------------------------------
*******************************************************************************/
static void DataPackingGps_15(U8 *sendbuf, kal_uint8 l_mask, track_gps_data_struct *gd, U8 *size)
{
    kal_uint16 Course,speed;
    kal_uint8 totalsat = 0;
    int i, x, j;
    kal_uint8 snr[2] = {0};    
    if(!(l_mask&1))
    {
       LOGD(L_SOC, L_V6, "GPS is not located, do not need to send GPS data");
       return;
    }
    *size +=15;
    
    LOGD(L_APP, L_V6, "status:%d,%d, sat:%d,%d, Latitude:%f%c, Longitude:%f%c, Speed:%f, Course:%f",
         gd->gprmc.status, gd->gpgga.posi_fix, gd->gpgga.sat, gd->bdgga.sat, gd->gprmc.Latitude, gd->gprmc.NS, gd->gprmc.Longitude, gd->gprmc.EW, gd->gprmc.Speed, gd->gprmc.Course);
    //γ�ȡ�����8
    track_fun_LongitudeLatitudeToHex(&sendbuf[0], gd->gprmc.Latitude);
    track_fun_LongitudeLatitudeToHex(&sendbuf[4], gd->gprmc.Longitude);

    //�������SNRֵ2
    for(i = 0, j = 0; i < gd->gpgga.sat; i++)
    {
        for(x = 0; x < 16; x++)
        {
            if(gd->gpgsa.satellite_used[i] == gd->gpgsv.Satellite_ID[x])
            {
                gd->gpgsa.satellite_SNR_used[i] = gd->gpgsv.SNR[x];
                if(i < 2)
                {
                    snr[i] = gd->gpgsa.satellite_SNR_used[i];
                }
                else
                {
                    for(j = 0; j < 2; j++)
                    {
                        if(snr[j] < gd->gpgsa.satellite_SNR_used[i])
                        {
                            snr[j] = gd->gpgsa.satellite_SNR_used[i];
                            break;
                        }
                    }
                }
                break;
            }
        }
        LOGD(L_SOC, L_V4, "satellite:[%d]%d", gd->gpgsa.satellite_used[i], gd->gpgsa.satellite_SNR_used[i]);
    }  
    memcpy(&sendbuf[8], snr, 2);
    
    //�ٶ�2
    speed = gd->gprmc.Speed*100;
    track_fun_reverse_16(&sendbuf[10], speed);
    //����2
    Course = gd->gprmc.Course;
    track_fun_reverse_16(&sendbuf[12], Course);
    if(gd->gprmc.NS == 'N')
    {
        sendbuf[10] |= 1 << 2;
    }
    if(gd->gprmc.EW == 'W')
    {
        sendbuf[10] |= 1 << 3;
    }
    //��Ч��λ������1
    totalsat = gd->gpgga.sat + gd->bdgga.sat;
    if(totalsat > 15)
        totalsat = 15;
    sendbuf[14] = totalsat;
}

/******************************************************************************
 *  Function    -  DataPackingLbs_25
 *
 *  Purpose     -  ���վ���ݴ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 01-11-2017,  Cjj  written
 * ----------------------------------------
 ******************************************************************************/
static void DataPackingLbs_25(U8 *sendbuf, kal_uint8 l_mask, LBS_Multi_Cell_Data_Struct *lbs, U8 *size)
{   
     U8 i = 0, num,j;
    if(lbs->MainCell_Info.cell_id ==0)
    {
       LOGD(L_SOC, L_V6, "Is not LBS, do not need to send LBS data");
       return;       
    }
    //LOGD(L_APP, L_V6, "mcc:%d, mnc:%d, lac:%d, cell id:%d",
    //     lbs->MainCell_Info.mcc, lbs->MainCell_Info.mnc, lbs->MainCell_Info.lac, lbs->MainCell_Info.cell_id);
    track_fun_reverse_16(&sendbuf[0], lbs->MainCell_Info.mcc);
    track_fun_reverse_16(&sendbuf[2], lbs->MainCell_Info.mnc);
    track_fun_reverse_16(&sendbuf[4], lbs->MainCell_Info.lac);
    track_fun_reverse_32(&sendbuf[6], lbs->MainCell_Info.cell_id);
    sendbuf[10] = (U8)lbs->MainCell_Info.rxlev;
    num = 11;
    *size += 11;
    if(!(l_mask&1))
    {
        LOGD(L_SOC, L_V6, "GPS is not located");
        if((l_mask&4) && (l_mask&10))
        {
            j=2;
            *size += 14;
        }
        else if(l_mask&4)
        {
            j=1;
            *size += 7;
        }
        else if(l_mask&10)
        {
            j=1;
            num +=7;
            *size += 7;
        }
        else
        {
            j=0;
        }
        for(i = 0; i < j; i++)
        {
            track_fun_reverse_16(&sendbuf[num + i * 7], lbs->NbrCell_Info[i].lac);
            track_fun_reverse_32(&sendbuf[num + i * 7 + 4], lbs->NbrCell_Info[i].cell_id);
            sendbuf[num + i * 6 + 6] = (U8)lbs->NbrCell_Info[i].rxlev;
        }
    }
}

/******************************************************************************
 *  Function    -  DataPackingWifi_21
 *
 *  Purpose     -  WIFI���ݴ��
 *
 *  Description -
 *
 * modification history
 * ----------------------------------------
 * v1.0  , 01-11-2017,  Cjj  written
 * ----------------------------------------
 ******************************************************************************/
static void DataPackingWifi_21(U8 *sendbuf, kal_uint8 l_mask, U8 *size)
{   U8 num = 1;
    kal_uint8 i;
    int index = 0;
    if(wifi_status.wifi_sum == 0)
    {
        LOGD(L_APP, L_V1, "Is not WIFI, do not need to send WIFI data");
        return;
    }
    
    if(!(l_mask&1))
    {
        LOGD(L_SOC, L_V6, "GPS is not located");
        if((l_mask&40) && (l_mask&100))
        {
            num = 3;
        }
        else if(l_mask&40)
        {
            num = 2;            
        }
        else if(l_mask&100)
        {
            //�ܲ�������ģ�WIFI��RSII��ֵ������õġ�
            num = 2;
        }
        else
        {
            num = 1;
        }
    }
    for(i = 0; i < num; i++)
    {
        if(strstr(wifi_status.wifi_mac[i], "LAST"))
        {
            LOGD(L_APP, L_V7, "1");
            break;
        }
        else if(wifi_status.wifi_rssi[i] == 0)
        {
            LOGD(L_APP, L_V7, "rssi����״̬�²���Ϊ0?");
            continue;
        }
        else
        {
            LOGD(L_APP, L_V7, "3");
            if(strlen(wifi_status.wifi_mac[i]) > 0 && wifi_status.wifi_rssi[i] != 0)
            {
                memcpy(&sendbuf[index], track_fun_stringtohex(wifi_status.wifi_mac[i], 6), 6);
                index = index + 6;
                sendbuf[index] = wifi_status.wifi_rssi[i];
                index++;
                *size += 7;
            }
        }
    }
}   

/******************************************************************************
 *  Function    -  DataPacking_Indefinable
 *
 *  Purpose     -  GPS��WIFI��LBS��������Ϣ(�����Ϣ����̡��ٶ�)���ݴ��
 *
 *  Description -  NULL
 *
 *  modification history
 * ----------------------------------------
 * v1.0  , 01-11-2017,  Cjj  written
 * ----------------------------------------
*******************************************************************************/
static void DataPacking_Indefinable(U8 *sendbuf, track_gps_data_struct *gd,LBS_Multi_Cell_Data_Struct *lbs)
{
    char imei_data[17];
    applib_time_struct currTime = {0};
    kal_uint8 i = 0;
    kal_uint8 location_mask = 0;
    kal_uint32 sec;
    charge_measure_struct charge_measure = {0};
    kal_int16 temporary = 0;
    //boxcode(ͬ�ն�ID)
    sprintf(imei_data, "%s", (char *)track_drv_get_imei(0));
    track_fun_ASCII_to_BCD8(&sendbuf[i],imei_data);
    i+=8;
    
    //ʱ��
    applib_dt_get_rtc_time(&currTime);
    sec = applib_dt_mytime_2_utc_sec(&currTime, 0);   
    track_fun_reverse_32(&sendbuf[i],sec);
    i+=4;
    
    //��λ����
    DataPackingLocation_Mask(&sendbuf[i],gd,lbs);
    location_mask = sendbuf[i];
    i+=1;

    //GPS����
    DataPackingGps_15(&sendbuf[i],location_mask,gd,&i);

    //LBS����
    DataPackingLbs_25(&sendbuf[i],location_mask,lbs,&i);

    //WIFI����
    DataPackingWifi_21(&sendbuf[i],location_mask,&i);

    //���ʣ�����
    i+=2;
    //��ص�ѹ
    i+=2;
    //��������
    i+=2;
    //�������
    i+=4;
    //����    
    i+=2;
    
    //����¶�
    track_bmt_get_ADC_result(&charge_measure);
    temporary = (kal_int16)(charge_measure.bmt_vTemp / 1000.00); 
    track_fun_reverse_16(&sendbuf[i], temporary);
    i+=2;
}

