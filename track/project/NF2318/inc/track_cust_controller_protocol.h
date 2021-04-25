#if defined(__NF2318__)
/*******************************************************************************************
 * Filename:track_cust_controller_protocol.h
 * Author :Liujw
 * Date :2017-08-02
 * Comment:7�ŵ絥������������ͨѶЭ��
 *ͨѶ��ʽ������ͨѶ��TTL0V-5V���� �����ʣ�9600bps��8bit����λ��1λֹͣλ����У��λ�� 
 ******************************************************************************************/
 /**����ͷ�ļ�**/
#include "track_cust.h"
#include "track_os_ell.h"
/*
*�ֲ���������
*/
/*
*
*
*/
void track_cust_status_recover(void);

extern void track_cust_module_speed_limit(track_gps_data_struct *gps_data,kal_uint8 tmpspeed);
#if 0
typedef struct
{
	kal_uint8 otherData1;//��λ bit7 Ԥ�� bit6 ת�ѹ��� bit5 ����������� bit4 Ƿѹ���� bit3 ���ɳ����� bit2 ��ת���� bit1 ���״̬��1 ��������״̬�� bit0 ɲ�� ��1 ����ɲ��״̬�� 
	kal_uint8 otherData2;//��λ bit2 Ԥ�� bit1 ������źŻ��� bit0ECU����ʹ�ܻ��� ��1 �����������������綯 ������ �綯������� bit7 1 0 1 bit6 0 1 1 bit5~bit3 ������ת������״̬���� 90% 85% 80% 75% 70% bit5 1 1 0 0 0 bit4 0 0 1 1 0 bit3 1 0 1 0 1 
	kal_uint8 voltageH;//�����仯���ֽ�
	kal_uint8 voltageL;//�����仯���ֽ�
	kal_uint8 relativeCapacity;//�������
	kal_uint8 currentH;//ʵʱ�������ֽ�
	kal_uint8 currentL;//ʵʱ�������ֽ�
	kal_uint8 leftCapatiryH;//ʣ���������ֽ�
	kal_uint8 leftCapatiryL;//ʣ���������ֽ�
	kal_uint8 absoluteCapacity;//���������ٷֱ�
	kal_uint8 absoluteFullCapacity[2];//�����������
	kal_uint8 circulationOfBat[2];//�����ѭ������
	kal_uint8 valtage[28];//���1-14�ŵ�ص�ѹ��ѹ
	kal_uint8 totalValtage[2];//������ܵ�ѹ
	kal_uint8 temperature[2];//������ڲ��¶�
	kal_uint8 chargeInterval[2];//��ǰ�����ʱ��
	kal_uint8 largeChargeInterval[2];//�������ʱ��
	kal_uint8 barCode[16];//��Ʒ������
	kal_uint8 versionNum[2];//�汾��
	kal_uint8 valtageManufacturer[16];//������쳧��
	kal_uint8 relativeCapacityPer;//��������ٷֱ�
	kal_uint8 absoluteCapacityPer;//���������ٷֱ�
}struct_controller_data;
#endif
#endif
