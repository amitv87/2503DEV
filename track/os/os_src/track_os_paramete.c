/******************************************************************************
 * track_os_paramete.c -
 *
 * Copyright .
 *
 * DESCRIPTION: -
 *        ͨ��ͷ�ļ�
 *
 * modification history
 * --------------------
 * v1.0   2012-07-14,  jwj create this file
 *
 ******************************************************************************/



/****************************************************************************
* Include Files             ����ͷ�ļ�
*****************************************************************************/
#include "track_os_data_types.h"
#include "track_version.h"
#include "track_os_paramete.h"
#include "track_os_log.h"
#include "track_cust.h"

/*****************************************************************************
 *  Define			    �궨��
*****************************************************************************/
//#define ON_OFF(value) (value==0 ? s_OFF : (value==1 ? s_ON : s_Error))

/*****************************************************************************
* Typedef  Enum         ö��
*****************************************************************************/


/*****************************************************************************
 *  Struct			    ���ݽṹ����
*****************************************************************************/


/*****************************************************************************
* Local variable            �ֲ�����
*****************************************************************************/


/****************************************************************************
* Global Variable           ȫ�ֱ���
*****************************************************************************/

#pragma arm section rodata = "TRACK_NVRAM_RODATA"

const nvram_importance_parameter_struct  NVRAM_EF_IMPORTANCE_PARAMETER_DEFAULT =
{
    {__DATE__},
    {__TIME__},
#if defined(__SAME_GT02__) && defined(__SYS_VERSION__)
    {"TQ"},
#else
    {0},
#endif /* __SAME_GT02__ */
    {_TRACK_VER_PARAMETER_},
    {KAL_FALSE, "CMNET", "", ""},
    {
#if defined(__NULL_SERVER__)
        /*Ĭ��Ϊ��*/
        {0},
        1, {0, 0, 0, 0}, 0, 0, 0, 0, 0
#elif defined(__SAME_GT02__) && defined(__SERVER_TUQIANG__)
        /*gpsdev.tracksolid.com*/
        {0xE7, 0xF0, 0xF3, 0xE4, 0xE5, 0xF6, 0xAE, 0xEA, 0xE9, 0xED, 0xE9, 0xE3, 0xEC, 0xEF, 0xF5, 0xE4, 0xAE, 0xE3, 0xEF, 0xED},
        1, {202, 70, 2, 53}, 21101, 0, 0, 0, 0
#elif defined(__NT33__)
#if defined (__LANG_ENGLISH__)
        /*gpsdev.tracksolid.com*/
        {0xE7, 0xF0, 0xF3, 0xE4, 0xE5, 0xF6, 0xAE, 0xF4, 0xF2, 0xE1, 0xE3, 0xEB, 0xF3, 0xEF, 0xEC, 0xE9, 0xE4, 0xAE, 0xE3, 0xEF, 0xED, 0x80},
        1, {49, 213, 12, 232}, 21100, 0, 0, 0
#else
        /*gpsdev.jimicloud.com*/
        {0xE7, 0xF0, 0xF3, 0xE4, 0xE5, 0xF6, 0xAE, 0xEA, 0xE9, 0xED, 0xE9, 0xE3, 0xEC, 0xEF, 0xF5, 0xE4, 0xAE, 0xE3, 0xEF, 0xED, 0x80},
        1, {112, 74, 143, 157}, 21100, 0, 0, 0
#endif /* __LANG_ENGLISH__ */

#elif defined(__NT31__)&& defined(__LANG_ENGLISH__)
        /*gpsdev.tracksolid.com*/
        {0xE7, 0xF0, 0xF3, 0xE4, 0xE5, 0xF6, 0xAE, 0xF4, 0xF2, 0xE1, 0xE3, 0xEB, 0xF3, 0xEF, 0xEC, 0xE9, 0xE4, 0xAE, 0xE3, 0xEF, 0xED, 0x80},
        1, {49, 213, 12, 232}, 21100, 0, 0, 0

#elif defined (__SERVER_COOACCESS__)&&defined (__SAME_GT02__)
        /*www.cooaccess.net*/
        {0xF7, 0xF7, 0xF7, 0xAE, 0xE3, 0xEF, 0xEF, 0xE1, 0xE3, 0xE3, 0xE5, 0xF3, 0xF3, 0xAE, 0xEE, 0xE5, 0xF4},
        1, {183, 60, 142, 137}, 8821, 0, 0, 0, 0
#elif defined(__ET310__) && defined(__LANG_ENGLISH__)
        /*gpsdev.tracksolid.com:21100*/
        {0xE7, 0xF0, 0xF3, 0xE4, 0xE5, 0xF6, 0xAE, 0xF4, 0xF2, 0xE1, 0xE3, 0xEB, 0xF3, 0xEF, 0xEC, 0xE9, 0xE4, 0xAE, 0xE3, 0xEF, 0xED},
        1, {49, 213, 12, 232}, 21100, 0, 0, 0, 0
#elif defined(__ET310__)
        /*et310.tuqianggps.com:9555*/
        {0xE5, 0xF4, 0xB3, 0xB1, 0xB0, 0xAE, 0xF4, 0xF5, 0xF1, 0xE9, 0xE1, 0xEE, 0xE7, 0xE7, 0xF0, 0xF3, 0xAE, 0xE3, 0xEF, 0xED},
        1, {120, 76, 103, 131}, 9555, 0, 0, 0, 0
#elif defined(__EL100__)
        /*gpsdev.jimicloud.com:21100*/
        {0xE7, 0xF0, 0xF3, 0xE4, 0xE5, 0xF6, 0xAE, 0xEA, 0xE9, 0xED, 0xE9, 0xE3, 0xEC, 0xEF, 0xF5, 0xE4, 0xAE, 0xE3, 0xEF, 0xED, 0x80},
        1, {112, 74, 143, 157}, 21104, 0, 0, 0
#elif defined(__NT13V__)
        /*gpsdev.jimicloud.com:21100*/
        {0xE7, 0xF0, 0xF3, 0xE4, 0xE5, 0xF6, 0xAE, 0xEA, 0xE9, 0xED, 0xE9, 0xE3, 0xEC, 0xEF, 0xF5, 0xE4, 0xAE, 0xE3, 0xEF, 0xED, 0x80},
        1, {112, 74, 143, 157}, 21100, 0, 0, 0        
#elif defined(__NT17__)
        /*gpsdev.jimicloud.com:21100*/
        {0xE7, 0xF0, 0xF3, 0xE4, 0xE5, 0xF6, 0xAE, 0xEA, 0xE9, 0xED, 0xE9, 0xE3, 0xEC, 0xEF, 0xF5, 0xE4, 0xAE, 0xE3, 0xEF, 0xED, 0x80},
        1, {112, 74, 143, 157}, 21100, 0, 0, 0
#elif defined(__NT55__)
        /*gpsdev.jimicloud.com:21100*/
        {0xE7, 0xF0, 0xF3, 0xE4, 0xE5, 0xF6, 0xAE, 0xEA, 0xE9, 0xED, 0xE9, 0xE3, 0xEC, 0xEF, 0xF5, 0xE4, 0xAE, 0xE3, 0xEF, 0xED, 0x80},
        1, {112, 74, 143, 157}, 21100, 0, 0, 0
#elif defined(__QB200__)
        /*gpsdev.jimicloud.com:21100*/
        {0xE7, 0xF0, 0xF3, 0xE4, 0xE5, 0xF6, 0xAE, 0xEA, 0xE9, 0xED, 0xE9, 0xE3, 0xEC, 0xEF, 0xF5, 0xE4, 0xAE, 0xE3, 0xEF, 0xED, 0x80},
        1, {112, 74, 143, 157}, 21100, 0, 0, 0
#elif defined(__NF2319__)
        /*gpsdev.jimicloud.com:21100*/
        {0xE7, 0xF0, 0xF3, 0xE4, 0xE5, 0xF6, 0xAE, 0xEA, 0xE9, 0xED, 0xE9, 0xE3, 0xEC, 0xEF, 0xF5, 0xE4, 0xAE, 0xE3, 0xEF, 0xED, 0x80},
        1, {112, 74, 143, 157}, 21100, 0, 0, 0    
#elif defined (__NF2318__)
#if defined(__EL100_XYT__)
        /*msg.51xytu.com:7071*/
        {0xED, 0xF3, 0xE7, 0xAE, 0xB5, 0xB1, 0xF8, 0xF9, 0xF4, 0xF5, 0xAE, 0xE3, 0xEF, 0xED},
        1, {121, 42, 247, 56}, 7071, 0, 0, 0    
#else
        /*radar.songguo7.com:1024*/
        {0xF2, 0xE1, 0xE4, 0xE1, 0xF2, 0xAE, 0xF3, 0xEF, 0xEE, 0xE7, 0xE7, 0xF5, 0xEF, 0xB7, 0xAE, 0xE3, 0xEF, 0xED, 0x80},
        1, {123, 59, 54, 138}, 1024, 0, 0, 0    
#endif
#elif defined(__JM10__)
        /*gpsdev.jimicloud.com:21100*/
        {0xE7, 0xF0, 0xF3, 0xE4, 0xE5, 0xF6, 0xAE, 0xEA, 0xE9, 0xED, 0xE9, 0xE3, 0xEC, 0xEF, 0xF5, 0xE4, 0xAE, 0xE3, 0xEF, 0xED, 0x80},
        1, {112, 74, 143, 157}, 21100, 0, 0, 0  
#else
#error "please define project_null"
#endif
    }, {
#if defined(__LANG_ENGLISH__)&& !defined(__NT36__)&& !defined(__MT200__)&&!defined(__GT740__)
        /*agps.topstargps.com:12283*/
        {0xE4 , 0xE5 , 0xF6 , 0xAE , 0xEA , 0xE9 , 0xED , 0xE9 , 0xE3 , 0xEC , 0xEF , 0xF5 , 0xE4 , 0xAE , 0xE3 , 0xEF , 0xED},
        1, {116, 204, 8, 57}, 12283, 0, 0, 0
#else
        /*agps.topstargps.com:12283*/
        {0xE4 , 0xE5 , 0xF6 , 0xAE , 0xEA , 0xE9 , 0xED , 0xE9 , 0xE3 , 0xEC , 0xEF , 0xF5 , 0xE4 , 0xAE , 0xE3 , 0xEF , 0xED},
        1, {116, 204, 8, 57}, 12283, 0, 0, 0
#endif /* __LANG_ENGLISH__ */
    },
    {
        {

            {0XF7 , 0xF7 , 0xF7 , 0xAE , 0xEA , 0xE9 , 0xED , 0xE9 , 0xE1 , 0xE7 , 0xF0 , 0xF3 , 0xAE , 0xE3 , 0xEF , 0xED},
            1, {116, 204, 8, 57}, 12283, 0, 0, 0
        }
        , {
            {0xF7 , 0xF7 , 0xF7 , 0xAE , 0xEA , 0xE9 , 0xED , 0xE9 , 0xE1 , 0xED , 0xF3 , 0xAE , 0xE3 , 0xEF , 0xED},
            1, {116, 204, 8, 57}, 12283, 0, 0, 0
        }
        , {
            {0xE4 , 0xE5 , 0xF6 , 0xAE , 0xEA , 0xE9 , 0xED , 0xE9 , 0xE3 , 0xEC , 0xEF , 0xF5 , 0xE4 , 0xAE , 0xE3 , 0xEF , 0xED},
            1, {116, 204, 8, 57}, 12283, 0, 0, 0
        }
        , {
            {0xE1, 0xE7, 0xF0, 0xF3, 0xAE, 0xF4, 0xEF, 0xF0, 0xF3, 0xF4, 0xE1, 0xF2, 0xE7, 0xF0, 0xF3, 0xAE, 0xE3, 0xEF, 0xED},
            0, {120, 77, 2, 16}, 12283, 0, 0, 0
        }
    }
#if defined(__SAME_GT02__) && (defined(__GUMI__) || defined(__GUMI_GENERAL__))
    , {
        //gt02.goomegps.net
        {0xE7 , 0xF4, 0xB0 , 0xB2, 0xAE , 0xE7 , 0xEF, 0xEF, 0xED , 0xE5 , 0xE7, 0xF0, 0xF3, 0xAE, 0xEE, 0xE5 , 0xF4},
        1,
        {202, 104, 149, 80},
        9888,
        0,
        1,
        0,
        0,
    }
#else
    , {
        {0xE7 , 0xF4, 0xB0 , 0xB2, 0xAE , 0xE7 , 0xEF, 0xEF, 0xED , 0xE5 , 0xE7, 0xF0, 0xF3, 0xAE, 0xEE, 0xE5 , 0xF4},
        1,
        {202, 104, 149, 80},
        9888,
        0,
        0,
        0,
        0
    }
#endif
#if defined(__BACKUP_SERVER__)
    , {0}
#endif/*__BACKUP_SERVER__*/
#ifdef __SAME_GT02__///����ΪĬ�ϰ汾
#if defined( __GUMI__)
    , {1, 1, 0, 1} //version
#elif defined( __GUMI_GENERAL__)
    , {2, 1, 0, 0} //����ͨ�ð汾����֧��SENSOR����֧�ֹ���湦��
#elif defined(__SERVER_TRACKSOLID__) || defined( __SERVER_COOACCESS__)
    , {3, 1, 1, 0} //��ó�汾��SENSOR��֧�ֹ���湦��
#elif defined( __SERVER_TUQIANG__)
    , {4, 1, 0, 0} //ͼǿ�汾��֧��SENSOR����֧�ֹ���湦��
#endif
#else
    , {0, 0, 0, 0} //version
#endif /* __SAME_GT02__ */

#if defined (__LUYING__)
    , { /*tcpupload.jimicloud.com:20010*/
        {0xF4, 0xE3, 0xF0, 0xF5, 0xF0, 0xEC, 0xEF, 0xE1, 0xE4, 0xAE, 0xEA, 0xE9, 0xED, 0xE9, 0xE3, 0xEC, 0xEF, 0xF5, 0xE4, 0xAE, 0xE3, 0xEF, 0xED, 0x80},
        1, {202, 70, 2, 53}, 20010, 0, 0, 0, 0
    }
#endif /* __LUYING__ */
#if defined(__NF2318__)
	,99
	,{"700000001F"}
	,{"G3A650000000371XH"}			////�豸ID��
#if defined(__EL100_XYT__)
    ,0x0B                           // ���̴���
    ,{0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30}  // ʶ����
#endif
#endif
};

const nvram_parameter_struct NVRAM_EF_PARAMETER_DEFAULT =
{
    {0},
    {NULL},
#ifdef __LANG_ENGLISH__
    {"http://maps.google.com/maps?q="},                 // URL����
#else
    {"http://ditu.google.cn/maps?q="},                 // URL����
#endif /* __LANG_ENGLISH__ */

    10,                                                 // ������ʱ����Χ��5-18S��Ĭ������ 10S

#if defined(__LANG_ENGLISH__)
    0,                                                  // 1 ��ʾΪ���ģ�0 ��ʾΪӢ�ģ����ڵ�ַ��ѯ
#else
    1,                                                  // 1 ��ʾΪ���ģ�0 ��ʾΪӢ�ģ����ڵ�ַ��ѯ
#endif /* __LANG_ENGLISH__ */
    1,                                                  // 1 ����GPRS   0 �ر�GPRS
#if defined(__GT300S__)||defined(__SAME_GT02__) || defined(__GT740__)|| defined(__GT370__)|| defined (__GT380__)||defined (__NT33__)
    180,                                                  // ���������ü��(����ָ��Ϊ���ӣ�ʵ�ʴ洢Ϊ��)  ACC ON �ϴ����
    180,                                                  // ���������ü��(����ָ��Ϊ���ӣ�ʵ�ʴ洢Ϊ��)  ACC OFF �ϴ����
#elif defined(__NF2318__)
	120,												  // ���������ü��
	120,
#else
    180,                                                  // ���������ü��
    300,
#endif

    0,                                                  // �������巽ʽ

    3,                                                  // �绰�ز�����

    10,                                                 // ������ʱ��0��5��18�룻����Զ�̼�����ʱ��Ĭ��ֵΪ��10��

#if defined(__SPANISH__)
    1,                                                  // ��������Զ����ص�ַ��1 ��Ч�� 0 ���ã�
#else
    0,                                                  // ��������Զ����ص�ַ��1 ��Ч�� 0 ���ã�
#endif

#if defined(__SAME_GT02__)
#if defined(__NO_SENSOR__)
    0,                                                  // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
#else
    5,                                                  // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
#endif /* __NO_SENSOR__ */
    2,                                                  // ����GPS�ر� LED���ߣ�1Ϊ���ã�0Ϊ��ֹ
    600,                                                // ��ʱ�������ʱ�䣨�룩
    1,                                                  // ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
    {1, 100, 60, 1, 5, {10, 1, 1}, {1, 20, 2}, {0, 300}, {1, 10, 10, 2, 30}}, // GPS����ģʽ
    {0, 1, 10, 5, 1, 1800, 180, 2},                     // �𶯱���
    {0, 1, 10, 300, 0, 0},                                // �ϵ籨������
    {0, 100, 1, 20},                                    // ���ٱ�������
    {"666666", "999999", 1, 0, 0, 1, 0},                // Ȩ������
    {1, 2},                                             // SOS��������
    {0, 60, 20, 1},                                     // GPSä������
    {1, 10, 6, 3},                                      // ��ֹ�ϱ����λ�ÿ���ָ��
    {0, 1, 300},                                        // λ�Ʊ���
    {1, 1, 1, 0},                                          // AGPS

#elif defined (__ET310__)
    3,                                                  // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
    1,                                                  // ����GPS�ر� LED���ߣ�1Ϊ���ã�0Ϊ��ֹ
    60,                                                 // ��ʱ�������ʱ�䣨�룩
    1,                                                  // ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
    {1, 100, 60, 1, 5, {10, 3, 1}, {1, 30, 3}, {0, 300}, {1, 20, 20, 2, 30}}, // GPS����ģʽ
    {1, 0, 10, 5, 1, 60, 30, 2},                        // �𶯱���
    {1, 1, 10, 1, 0},                                   // �ϵ籨������
    {0, 50, 0, 20},                                     // ���ٱ�������
    {"666666", "SUPER", 0, 0, 0, 1, 2},                 // Ȩ������
    {1, 2},                                             // SOS��������
    {0, 60, 20, 0},                                     // GPSä������
    {1, 20, 6, 3},                                      // ��ֹ�ϱ����λ�ÿ���ָ��
    {0, 0, 300},                                        // λ�Ʊ���
    {1, 1, 1, 0},                                       // AGPS
#elif defined (__EL100__)
    1,                                                  // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
    1,                                                  // ����GPS�ر� LED���ߣ�1Ϊ���ã�0Ϊ��ֹ
    600,                                                 // ��ʱ�������ʱ�䣨�룩
    1,                                                  // ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
    {1, 100, 60, 1, 5, {10, 3, 1}, {1, 30, 3}, {0, 300}, {1, 20, 20, 2, 30}}, // GPS����ģʽ
    {1, 0, 10, 5, 1, 60, 30, 2},                        // �𶯱���
    {1, 0, 5, 300, 0},                                   // �ϵ籨������
    {0, 50, 0, 20},                                     // ���ٱ�������
    {"666666", "SUPER", 0, 0, 0, 1, 2},                 // Ȩ������
    {1, 2},                                             // SOS��������
    {0, 60, 20, 0},                                     // GPSä������
    {1, 20, 6, 3},                                      // ��ֹ�ϱ����λ�ÿ���ָ��
    {0, 0, 300},                                        // λ�Ʊ���
    {1, 1, 1, 0},                                       // AGPS
#elif defined (__NT13V__)
#if defined (__LANG_ENGLISH__)
    3,                                                  // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
    1,                                                  // ����GPS�ر� LED���ߣ�1Ϊ���ã�0Ϊ��ֹ
    60,                                                 // ��ʱ�������ʱ�䣨�룩
    1,                                                  // ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
    {1, 100, 60, 1, 5, {10, 2, 1}, {1, 30, 3}, {0, 300}, {1, 20, 20, 2, 30}}, // GPS����ģʽ
    {1, 0, 10, 2, 1, 60, 30, 2},                        // �𶯱���
    {1, 2, 10, 1, 0},                                   // �ϵ籨������
    {0, 50, 0, 20},                                     // ���ٱ�������
    {"666666", "SUPER", 0, 0, 0, 1, 3},                 // Ȩ������
    {1, 2},                                             // SOS��������
    {0, 60, 20, 0},                                     // GPSä������
    {1, 20, 6, 3},                                      // ��ֹ�ϱ����λ�ÿ���ָ��
    {0, 0, 300},                                        // λ�Ʊ���
    {1, 1, 1, 0},                                       // AGPS
#else
    3,                                                  // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
    1,                                                  // ����GPS�ر� LED���ߣ�1Ϊ���ã�0Ϊ��ֹ
    60,                                                 // ��ʱ�������ʱ�䣨�룩
    1,                                                  // ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
    {1, 100, 60, 1, 5, {10, 2, 1}, {1, 30, 3}, {0, 300}, {1, 20, 20, 2, 30}}, // GPS����ģʽ
    {1, 0, 10, 2, 1, 60, 30, 2},                        // �𶯱���
    {1, 1, 10, 1, 0},                                   // �ϵ籨������
    {0, 50, 0, 20},                                     // ���ٱ�������
    {"666666", "SUPER", 0, 0, 0, 1, 3},                 // Ȩ������
    {1, 2},                                             // SOS��������
    {0, 60, 20, 0},                                     // GPSä������
    {1, 20, 6, 3},                                      // ��ֹ�ϱ����λ�ÿ���ָ��
    {0, 0, 300},                                        // λ�Ʊ���
    {1, 1, 1, 0},                                       // AGPS
#endif /* __LANG_ENGLISH__ */
#elif defined (__NT17__)
#if defined (__LANG_ENGLISH__)
    3,                                                  // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
    1,                                                  // ����GPS�ر� LED���ߣ�1Ϊ���ã�0Ϊ��ֹ
    60,                                                 // ��ʱ�������ʱ�䣨�룩
    1,                                                  // ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
    {1, 100, 60, 1, 5, {10, 2, 1}, {1, 30, 3}, {0, 300}, {1, 20, 20, 2, 30}}, // GPS����ģʽ
    {1, 0, 10, 2, 1, 60, 30, 2},                        // �𶯱���
    {1, 2, 10, 1, 0},                                   // �ϵ籨������
    {0, 50, 0, 20},                                     // ���ٱ�������
    {"666666", "SUPER", 0, 0, 0, 1, 3},                 // Ȩ������
    {1, 2},                                             // SOS��������
    {0, 60, 20, 0},                                     // GPSä������
    {1, 20, 6, 3},                                      // ��ֹ�ϱ����λ�ÿ���ָ��
    {0, 0, 300},                                        // λ�Ʊ���
    {1, 1, 1, 0},                                       // AGPS
#else
    3,                                                  // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
    1,                                                  // ����GPS�ر� LED���ߣ�1Ϊ���ã�0Ϊ��ֹ
    60,                                                 // ��ʱ�������ʱ�䣨�룩
    1,                                                  // ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
    {1, 100, 60, 1, 5, {10, 2, 1}, {1, 30, 3}, {0, 300}, {1, 20, 20, 2, 30}}, // GPS����ģʽ
    {1, 0, 10, 2, 1, 60, 30, 2},                        // �𶯱���
    {1, 1, 10, 1, 0},                                   // �ϵ籨������
    {0, 50, 0, 20},                                     // ���ٱ�������
    {"666666", "SUPER", 0, 0, 0, 1, 3},                 // Ȩ������
    {1, 2},                                             // SOS��������
    {0, 60, 20, 0},                                     // GPSä������
    {1, 20, 6, 3},                                      // ��ֹ�ϱ����λ�ÿ���ָ��
    {0, 0, 300},                                        // λ�Ʊ���
    {1, 1, 1, 0},                                       // AGPS
#endif /* __LANG_ENGLISH__ */

#elif defined (__NT36__)
    5,                                                  // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
    1,                                                  // ����GPS�ر� LED���ߣ�1Ϊ���ã�0Ϊ��ֹ
    600,                                                 // ��ʱ�������ʱ�䣨�룩
    1,                                                  // ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
    {1, 100, 60, 1, 5, {10, 1, 1}, {1, 30, 3}, {0, 300}, {1, 10, 10, 2, 30}}, // GPS����ģʽ
    {0, 2, 10, 5, 1, 1800, 180, 2},                        // �𶯱���
    {1, 2, 5, 300, 0},                                   // �ϵ籨������
    {0, 100, 1, 20},                                     // ���ٱ�������
    {"000000", "SUPER", 0, 0, 0, 1, 2},                 // Ȩ������
    {1, 2},                                             // SOS��������
    {0, 10, 10, 1},                                     // GPSä������
    {1, 10, 6, 3},                                      // ��ֹ�ϱ����λ�ÿ���ָ��
    {0, 1, 300},                                        // λ�Ʊ���
    {1, 1, 1, 0},                                       // AGPS
#elif defined (__NT55__)
    5,                                                  // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
    1,                                                  // ����GPS�ر� LED���ߣ�1Ϊ���ã�0Ϊ��ֹ
    180,                                                 // ��ʱ�������ʱ�䣨�룩
    0,                                                  // ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
    {1, 100, 60, 0, 5, {10, 3, 1}, {1, 20, 2}, {0, 300}, {1, 10, 180, 180, 30}}, // GPS����ģʽ
    {0, 0, 10, 5, 1, 1800, 180, 2},                        // �𶯱���
    {1, 1, 20, 300, 0},                                   // �ϵ籨������
    {0, 100, 1, 20},                                     // ���ٱ�������
    {"666666", "SUPER", 0, 0, 0, 1, 2},                 // Ȩ������
    {1, 2},                                             // SOS��������
    {0, 10, 10, 1},                                     // GPSä������
    {1, 10, 6, 3},                                      // ��ֹ�ϱ����λ�ÿ���ָ��
    {0, 1, 300},                                        // λ�Ʊ���
#if defined(__LANG_ENGLISH__)
    {0, 1, 1, 0},                                       // AGPS
#else
    {1, 1, 1, 0},                                       // AGPS
#endif /* __LANG_ENGLISH__ */

#elif defined (__QB200__)
    5,                                                  // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
    1,                                                  // ����GPS�ر� LED���ߣ�1Ϊ���ã�0Ϊ��ֹ
    180,                                                 // ��ʱ�������ʱ�䣨�룩
    0,                                                  // ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
    {1, 100, 60, 1, 5, {10, 1, 1}, {1, 20, 2}, {0, 300}, {1, 10, 10, 180, 30}}, // GPS����ģʽ
    {0, 0, 10, 5, 1, 1800, 1, 2},                        // �𶯱���
    {1, 1, 20, 300, 0},                                   // �ϵ籨������
    {0, 100, 1, 20},                                     // ���ٱ�������
    {"666666", "SUPER", 0, 0, 0, 1, 2},                 // Ȩ������
    {1, 2},                                             // SOS��������
    {0, 10, 10, 1},                                     // GPSä������
    {1, 10, 6, 3},                                      // ��ֹ�ϱ����λ�ÿ���ָ��
    {0, 1, 300},                                        // λ�Ʊ���
#if defined(__LANG_ENGLISH__)
    {0, 1, 1, 0},                                       // AGPS
#else
    {1, 1, 1, 0},                                       // AGPS
#endif /* __LANG_ENGLISH__ */

#elif defined (__NF2319__)
    5,                                                  // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
    1,                                                  // ����GPS�ر� LED���ߣ�1Ϊ���ã�0Ϊ��ֹ
    180,                                                 // ��ʱ�������ʱ�䣨�룩
    0,                                                  // ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
    {1, 100, 60, 1, 5, {10, 1, 1}, {1, 20, 2}, {0, 300}, {1, 10, 10, 180, 30}}, // GPS����ģʽ
    {0, 0, 10, 5, 1, 1800, 1, 2},                        // �𶯱���
    {1, 1, 20, 300, 0},                                   // �ϵ籨������
    {0, 100, 1, 20},                                     // ���ٱ�������
    {"666666", "SUPER", 0, 0, 0, 1, 2},                 // Ȩ������
    {1, 2},                                             // SOS��������
    {0, 10, 10, 1},                                     // GPSä������
    {1, 10, 6, 3},                                      // ��ֹ�ϱ����λ�ÿ���ָ��
    {0, 1, 300},                                        // λ�Ʊ���
#if defined(__LANG_ENGLISH__)
    {0, 1, 1, 0},                                       // AGPS
#else
    {1, 1, 1, 0},                                       // AGPS
#endif /* __LANG_ENGLISH__ */
#elif defined (__NF2318__)
	3,													// 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
	1,													// ����GPS�ر� LED���ߣ�1Ϊ���ã�0Ϊ��ֹ
	180,												 // ��ʱ�������ʱ�䣨�룩
	0,													// ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
	{1, 10, 60, 1, 5, {10, 3, 1}, {1, 20, 2}, {0, 300}, {1, 15, 15, 180, 30}}, // GPS����ģʽ
	{1, 0, 10, 3, 1, 120, 1, 2},						 // �𶯱���
	{1, 1, 3, 3, 0}, 								  // �ϵ籨������
	{0, 100, 1, 5},									 // ���ٱ�������
	{"666666", "SUPER", 0, 0, 0, 1, 2}, 				// Ȩ������
	{1, 2}, 											// SOS��������
	{0, 10, 10, 1}, 									// GPSä������
	{1, 10, 6, 3},										// ��ֹ�ϱ����λ�ÿ���ָ��
	{0, 1, 300},										// λ�Ʊ���
#if defined(__LANG_ENGLISH__)
	{0, 1, 1, 0},										// AGPS
#else
	{1, 1, 1, 0},										// AGPS
#endif /* __LANG_ENGLISH__ */
#elif defined (__JM10__)
    5,                                                  // 0��300���ӣ���⵽һ���𶯣����� GPS ������ʱ��
    1,                                                  // ����GPS�ر� LED���ߣ�1Ϊ���ã�0Ϊ��ֹ
    180,                                                 // ��ʱ�������ʱ�䣨�룩
    0,                                                  // ���͵繦�ܣ�1 ��Ч�� 0 ���ã�
    {1, 100, 60, 1, 5, {10, 1, 1}, {1, 20, 2}, {0, 300}, {1, 10, 10, 180, 30}}, // GPS����ģʽ
    {0, 0, 10, 5, 1, 1800, 1, 2},                        // �𶯱���
    {1, 1, 20, 300, 0},                                   // �ϵ籨������
    {0, 100, 1, 20},                                     // ���ٱ�������
    {"666666", "SUPER", 0, 0, 0, 1, 2},                 // Ȩ������
    {1, 2},                                             // SOS��������
    {0, 10, 10, 1},                                     // GPSä������
    {1, 10, 6, 3},                                      // ��ֹ�ϱ����λ�ÿ���ָ��
    {0, 1, 300},                                        // λ�Ʊ���
#if defined(__LANG_ENGLISH__)
    {0, 1, 1, 0},                                       // AGPS
#else
    {1, 1, 1, 0},                                       // AGPS
#endif /* __LANG_ENGLISH__ */

#else
#error "please define project_null"
#endif

#if defined(__GT370__)|| defined (__GT380__)
    {1, 0},                                             // ������������
#else
    {0, 1},                                             // ������������
#endif

#ifdef __GT300S__
    {0, 30, 30},                                        // �������
#elif defined(__GT370__)|| defined (__GT380__)
    {0, 30, 30},										// �������
#else
    {0, 10, 10},                                        // �������
#endif /* __GT300S__ */
    {0, 0, 1000},                                      // ���ͳ��
    {0, 1, 60},                                         // ͣ����ʱ����

    {'E', 8, 0, 1},                                     // ʱ������

// �͵籨������  batalm
#if defined(__SAME_GT02__)
    {0, 1},                                             // �͵籨������
#elif defined (__NT13V__) || defined (__NT17__)
    #if defined(__LANG_ENGLISH__)
        {1, 1},
    #else
        {1, 0},
    #endif
#elif defined(__ET310__)||defined(__MT200__)||defined(__SPANISH__)||defined(__EL100__)
    {1, 0},
#else
    {1, 1},                                             // �͵籨������
#endif /* __SAME_GT02__ */

    {1, 0, 240, 20},                                    // ƣ�ͼ�ʻ����
    {
        {0, 2, 1, 0, 3, 0, 0, 0, 0}                     // ����Χ������
    },
    {0, 0, 3000, 40, 10},                               // ���������

#if defined(__GT300S__)
    {1, 2, 60 * 24, 0 , 30, 2, 20},                                      // �ն˹���ģʽ
#elif defined(__GT370__)|| defined (__GT380__)
    {1, 1, 60 * 24, 0 , 30, 2, 20},						 // �ն˹���ģʽ
#else
    {0, 2, 60 * 24, 0 , 30, 2, 20},                                      // �ն˹���ģʽ
#endif

#if (defined(__ET310__)) && defined(__LANG_ENGLISH__)
    {0, 1, 128, 138, 10},                               // �ⲿ��Դ�͵籨��
    {1, 0, 125, 130, 10},                               // �ⲿ�͵��жϱ���
#elif defined(__ET310__)
    {0, 0, 128, 138, 10},                               // �ⲿ��Դ�͵籨��
    {1, 0, 125, 130, 10},                               // �ⲿ�͵��жϱ���
#elif defined(__NT31__)
    {0, 0, 115, 125, 10},                               // �ⲿ��Դ�͵籨��
    {0, 0, 125, 130, 10},                               // �ⲿ�͵��жϱ���
#elif defined(__NT13V__) || defined (__NT17__)
    {0, 0, 128, 138, 10},                               // �ⲿ��Դ�͵籨��
    {1, 0, 115, 120, 10},                               // �ⲿ�͵��жϱ���
#else
    {0, 1, 128, 138, 10},                               // �ⲿ��Դ�͵籨��
    {0, 1, 125, 130, 10},                               // �ⲿ�͵��жϱ���
#endif
#if defined (__NT33__)
    {0, 6},
#else
    {0, 3},                                             // GPRS ��������
#endif
    {0, 1, 0},                                          // �ű���
#if defined(__NT55__)
	{0, 1, 7, 0, 0, {0}, 60, 0, 0, 0},					// OBD
#else
    {0, 1, 7, 0, 0, {0}, 60, 0, 0, 0},                  // OBD
#endif
    {126, 128}                                          // ACC�ⲿ����ѹ

#if defined(__ACC_ALARM__)
    , 1
#else
    , 0                                             // 0 ��ʾΪACC�л��޶���֪ͨ��1 ��ʾΪACC�л��ж���֪ͨ
#endif
#if (defined(__ET310__) || defined(__MT200__))

#if  ((defined (__ET310__)) && defined(__LANG_ENGLISH__))
    , 0
#else
    , 80
#endif /* __ET310__ */

#else
    , 0                                             // �綯�����ٷ�ֵ  ���ض�λ������Ϊ0
#endif

#if defined (__GT370__)|| defined (__GT380__)
    , {1, 0}                                        // ������������
    , {0, 0, "0000000000000000"}                    // ��������
#else
    , {0, 0}                                        // ������������
    , {0, 0, "0000000000000000"}                    // ��������
#endif /* __GW100__ */

    , 0                                             // ���ƹػ�����,ֻ�����п��ػ�������Ŀ

    , 20

    , 0                                             // ADC4����ֵ
    , 0                                             // mode3_work_interval;//����ģʽ�¶�ʱ�ϱ����
    , 1                                             // mode3_up_counts;//����ģʽ�µ��ι����ϱ�����
    , {2, 10, 5, 0, 1440, 0, 1, 0, {0}, 0, 2}                   // extchip
    , {"OBDMIFI", "123456"}                         // ap �û���+����
#if defined(__GT300S__) || defined(__GT370__)|| defined (__GT380__)
    , {1, 0, "0000000000000000"}                    // smstc ͸��
#else
    , {0, 0, "0000000000000000"}                    // smstc ͸��
#endif /* __GT300S__ */
//����Ҫ������û��OBDģ�飬��Ϊ�Բ�û��OBD��������ʱ����
    , 0                     // �Զ����ģʽ X=0  �Զ����ģʽ��X=1 �ֶ����ģʽ

    , {0, 600}                                       //(���׹���)��ӵ�Դ��ѹ��Ϣ�ϴ�����  ADTָ��

#if defined (__GT370__)|| defined (__GT380__)
    , {1, 0}                                        //teardown_str //���𱨾�
#else
    , {1, 1}                                        //teardown_str //���𱨾�
#endif /* __GT370__ */

#ifdef __WIFI__
#if defined(__NT13V__)||defined(__GT740__)||defined(__GT370__)|| defined (__GT380__)         //WIFI����
    , {1, 10, 0}
#else
    , {0, 10, 10 * 60}
#endif
#else
    , {0, 10, 10 * 60}
#endif /* __WIFI__ */

#if defined(__NT31__) || defined(__ET310__) || defined(__NT36__)|| defined(__NT33__)
    , {1, 60, 60}           //lbson
#else
    , {0, 60, 60}
#endif /* __NT31__ */

#if defined (__NT33__)
    , {0, 0, 30}           //α��վ����
#else
    , {0, 0, 30}           //α��վ����
#endif

#if (defined(__ET310__) && defined(__LANG_ENGLISH__))
    , 0                     //����ģʽ����Ĭ�Ͽ�
#else
    , 1                     //����ģʽ����Ĭ�Ͽ�
#endif /* __LANG_ENGLISH__ */
    , {0}                  //�����������ò����ļ���MD5
#if defined(__SECOND_SET__)
    , 1    // EPO���ؿ���
#else
    , 0
#endif
    , 2                                             // 5V��Դ���״̬
#if defined(__ACC_ALARM__)
    , {0, 1, 10, 0}            //ACC���������ʱ��
#endif
#if defined(__GT740__)
    , {0}                 //
    , 0                  //�����¶�λ���ϴ����� Ĭ�Ϲ�
    , 1                 //�����������㿪�أ�Ĭ�Ͽ�
    , {0, 20}          //��̬����
#endif

#if defined (__NETWORK_LICENSE__)
, 0
#elif defined (__NT33__)||defined (__NT31__) || defined(__GT740__)
    , 1                 //fake_cell_enable
#else
    , 0                 //fake_cell_enable ��α��վȫ���رգ����������
#endif
    , 1                 //α��վ��Ϣ�Ƿ��ϱ�AMS
#if defined (__GT370__)|| defined (__GT380__)
    , 1                 //gpsDup gpsλ�������ϱ�����
#else
    , 0                 //gpsDup gpsλ�������ϱ�����
#endif /* __GT370__ */
#if defined (__CUST_BT__) && defined(__NT33__)
    , {0, 2, 2, 0}      //bt
#endif /* __CUST_BT__ */
    , 1  //����GPS Ĭ��0

#if defined (__GT370__)|| defined (__GT380__)
    , {1, 1}    //���Ǳ���
#endif /* __GT370__ */
    , 0xFF         //��ʱ�Զ��ط�α����
#if defined(__SPANISH__)
    , 0
#endif
    , {1, 1, 1}
    , {0, 10, 5}
    , 0
#if defined(__DROP_ALM__)//��������
    , {1, 1}
#endif
    , 0  //ͨ��ģʽ
    , 0
#if defined(__EL100__)
    , {"xinsiwei&concox"}   //�ܳ�
    , 15                                //һ��ԤԼʱ��
    , {0, 3, 15, 5}                 //����Ѱ��ָ��
    , {1, 0, 10}
#endif
#if defined(__SMT_BMS__)
       ,{1,300,1200}// ﮵������Ϣ�ϱ�
       ,{1,0,5}//
#endif
#if defined(__NT55__)
    ,{1,3}	//acc ��ⷽʽ��1 adc�����磬2 obd���,3 sensor��� ��Ĭ��3
#endif

#if defined (__NT13V__)
    ,{0}        //device_sleep_times
#endif /* __NT13V__ */
#if defined (__NF2319__)
    ,{0,75}
#endif /* __NF2319__ */
#if defined(__NF2318__)
	,{3,6,0,0,0,0}//����������洢
	,{0,
	//{"700000001F"},
	99,
	{
	0xc6,0x69,0xa3,0xc2,0x79,0x06,0x18,0xae,0x82,0xdf,0xef,0x07,0x6e,0xa6,0x9f,0x9d,0xa2,0x10,
	0xe5,0x0d,0xe6,0xf0,0x11,0x3d,0xbf,0x7d,0x95,0x3b,0xb1,0x2a,0xab,0x4b,0xa7,0xc4,0xab,0xac,
	0xdd,0x1a,0x23,0xa2,0x01,0x13,0x7f,0x52,0x68,0x54,0xb4,0x28,0xee,0x52,0x41,0x66,0xe1,0x0c,
	0xd3,0xfb,0x2c,0xc9,0x79,0xf1,0x7c,0x6d,0x27,0x1d,0x2a,0x17,0xdc,0x8d,0xe4,0x77,0xd0,0xe3,
	0x1c,0xb3,0x37,0xcb,0x31,0x7b,0x5b,0xea,0xf9,0x1e,0x06,0xb8,0x4e,0x25,0x0f,0xda,0xdd,0xb1,
	0xd8,0xd7,0xac,0xfa,0x1e,0x7c,0xdd,0x51,0x80,0x30,0xa4,0x04,0x15,0x9b,0x10,0x3e,0xba,0x4a,
	0xa6,0x08,0xc7,0x49,0x37,0xcf,0xf1,0x60,0x53,0x72,0x13,0xc7,0xb5,0x36,0xfa,0x7d,0x27,0xdb,
	0x0f,0x55
	},
	{0},
	10,
	30,
	10,
	7,
	60,
	1,
	16,
	25,//�Ÿֶ���
	1,//�����������÷�Χ
	6,//�������
	3,//��������ʱ������
	300,//�����������ã� mg
	3,//����������������
	10,//�ֶ����ʱ�� s
	3,//�ֶ��ж�������� 3
	1800,//�����߼��gps����ʱ��
	"sg-ebike",//��������
#if defined(__EL100_XYT__)
    0,//��Ȩ�볤��
    {0,},//��Ȩ��
#endif
	}
	//,{0,0,0,1,0,2,0,1,0,1}//�ն�״̬
	,{0,0,1,0,0,0,0,1}//�ն�״̬
#endif
	,{1,1}//����¶Ȳɼ�
	,{1,15,3,0,0,{0x00,0x00,0x0E},{0x00,0x00,0x0F}}//���ٱ���
	,{0x30,0x00,0x03,0xE8}//�����ļ��汾
	,18,18,23,25
	,0 //����bms��ʼ���Ŀ��أ�0-��  1-��
	,0//������������ 0�� 1��
	,0//�����ļ��ܴ�С
	,{0x65, 0x83, 0x04, 0x14, 0x4f, 0x5a, 0xd2, 0x76, 0xb4, 0xae, 0x15, 0x7b, 0x25, 0x31, 0xf2, 0x2e}//������Կ//20190717
	,0
	,{"101131004020079090210118180174021123037049242046"}//��������
	,{0}//����������
};


const nvram_phone_number_struct NVRAM_EF_PHONE_NUMBER_DEFAULT =
{
    {0},
    {0},

    {1, 1, 1}
	//,{0}	//�洢��Ҫ���ص������б�
	//,0 //��Ҫ���ص�ԭ��������
};

const nvram_ef_logd_struct NVRAM_EF_LOGD_DEFAULT =
{
    /*save paramter for log wangqi*/
    {0}, //kal_uint8          logd_mdoule[L_MAX];
    //{L_CON,L_DRV,L_SOC,L_CMD,L_OS,L_APP,L_SYS,}//��ע�����������L_MAX��Ӧ
    0,        //kal_uint16          log_sw;
    LOG_AUTO,    //log_target          logd_target;
    ""
};

const nvram_realtime_struct NVRAM_EF_REALTIME_DEFAULT =
{
    {0},                    // ����Χ�� ��ǰ�ǳ���Χ����(1)����Χ����(2)
    0,                      // �͵籨�����ͱ��
    0,                      // �ⲿ��Դ�͵籨�����ͱ��
    0,                      // �ⲿ��Դ�Ͽ����ͱ��
    0,                      // λ�Ʊ��� ���
    0,                      // λ�Ʊ���
    0,                      // λ�Ʊ���
    0,                      // ���������ϱ���־����
    0,                      // 20����������������
    0,                      // ���״̬  1 ������2 Ԥ�����3 ���

    0,                      //ledsw  ����״̬ 0 ��    1 ��

    0,                      // �͵�״̬
    0,                      // ���͵�ָ����Դ
    2,                      // ���ڼ�¼ACC״̬����ֹ�������״̬���ϴιػ�ǰһ��
    0,                      // �͵�ػ��������ͱ��
    0,                      // ʡ��ģʽ(0:����ģʽ;1:ʡ��ģʽ)
    1,                      // �Ƿ���������������(0�����ͣ�1����)
    0,                      // L4C_RAC_LIMITED_SERVICE ��������
    2,                      // ä������״̬0��Ϊ����1Ϊ����2Ϊ��ʼֵ��

    0,                      // �Զ����ģʽ X=0  �Զ����ģʽ��X=1 �ֶ����ģʽ��Ĭ��X=1

    1,                       // ��λ�����ϴ�����  0 �ر�  1 ��
#if !defined(__NF2318__)
    {0},
#endif
    1,                       // �ⲿ��ѹ6v֮�±���������Ϊ1, 6v֮��Ϊ0
    0,                       //����������־λ
    500,
#if defined (__LUYING__)
	0,      //��Ƶ����
    60,     //ʱ��
    {{0}, {0}, {0}, {0}, {0}}, //��Ƶ�ļ�
#endif
    0,                       // ����ģʽ������������ʱ
    {0},
    0,
    0,
#if defined(__MECH_SENSOR__)
    1,
    700,
#else
    1,
    1000,
#endif
#if defined(__GT740__)
    0,
    20,//׷��ģʽʱ��
    0,
#endif
    0,
    1,//�Զ��ϴ�����
    1, //ams ����LOG����
    0,    //710�ϴ�LOG����
    0,     //��λ��Э�����
    0    // �����������
#if defined (__BAT_TEMP_TO_AMS__)
    , {0}
    , 0
#endif /* __BAT_TEMP_TO_AMS__ */
    , 0//�Ƿ�ͣ�ϴ��ϵ籨����־λ
#if defined(__CUST_FAKECELL__)
    ,{0}//α��վ��Ϣ��¼
#endif
    , 0 //����վID
    , 0 // ������־
    , 0
    , 0
    , 0
    , 0                                //��״̬

#if defined (__LUYING__)
    , 0     //record_file
    , 0
    , 0     //recard_time
    , {0}   //¼���ϴ�����ˮ��
    , 0
#endif /* __LUYING__ */
#if defined(__NF2318__)
	,0
	,0
	,0
	,0//������쳣�澯
	,{0}//http���ز�����ʱ�洢
	,0//�Ѿ��Ա���汾��Ϣ �й̼����� 00000001 ���������� 00000010  
	,0//�̼�������ʶ
	//,0//���״̬ 0  ���� 1 ��
	,0//������״̬0 ���� 1��
	,2//�����״̬ 0 ���� 1 ��
	,0,0,0,0
	,0 //��ǰ����״̬ 0 ������ 15������  16 ȫ����
	,0//http���Ӽ���
	,{0}//http����
	,{0}	//�洢��Ҫ���ص������б�
	,0 //��Ҫ���ص�ԭ��������
	,{0}//��ʱ�洢�����汾��
	,0//��ʱ��ű��������������ܴ�С
	,{0}//bms ״̬����#endif	
	,0//ACC�쳣����
	,{0}//����������
	//--------------20191010
	,0
	,0
#endif
};

#pragma arm section rodata

nvram_drv_importance_backup_struct   importance_backup = {0};

nvram_parameter_struct* OTA_NVRAM_EF_PARAMETER_DEFAULT = (nvram_parameter_struct*)&NVRAM_EF_PARAMETER_DEFAULT;
nvram_importance_parameter_struct* OTA_NVRAM_EF_IMPORTANCE_PARAMETER_DEFAULT  = (nvram_importance_parameter_struct*)&NVRAM_EF_IMPORTANCE_PARAMETER_DEFAULT;
nvram_phone_number_struct* OTA_NVRAM_EF_PHONE_NUMBER_DEFAULT = (nvram_phone_number_struct*)&NVRAM_EF_PHONE_NUMBER_DEFAULT ;
nvram_ef_logd_struct* OTA_NVRAM_EF_LOGD_DEFAULT = (nvram_ef_logd_struct*)&NVRAM_EF_LOGD_DEFAULT;
nvram_realtime_struct* OTA_NVRAM_EF_REALTIME_DEFAULT = (nvram_realtime_struct*)&NVRAM_EF_REALTIME_DEFAULT;

extern void track_cust_paket_9404_handle(void);

void track_os_importance_backup_write(void)
{
    static char *s_ON = {"ON"}, *s_OFF = {"OFF"}, *s_Error = {"ERROR"};
    kal_uint32 len = 2000, l = 0;
	
    kal_uint32 value1 = 0 ,value2 = 0;
    //LOGD(L_APP, L_V5, "");
#ifdef __SAME_GT02__
    kal_uint8 chip_rid[16];
    kal_uint16 CRC, CRC_IN = 0;
    memcpy(chip_rid, track_drv_get_chip_id(), 16);
    CRC = GetCrc16(chip_rid, 16);
#endif /* __SAME_GT02__ */

    memset(&importance_backup, 0x00, sizeof(importance_backup));
	//�����������˿ں��жϣ����ͻ�䣬��ȥд��
	if(G_importance_parameter_data.server.conecttype == 255 || G_importance_parameter_data.server.server_port == 65535 ||  G_importance_parameter_data.server.soc_type == 255)
	{
    	LOGD(L_APP, L_V5, "error,%d,%d,%d", G_importance_parameter_data.server.conecttype,G_importance_parameter_data.server.server_port,G_importance_parameter_data.server.soc_type);
		return;
	}
	if(G_parameter.ste_param.rcv == 0 || G_parameter.ste_param.rcv == 255 || G_parameter.ste_param.rcv != 99)
	{
	    LOGD(L_APP, L_V5, "error1,%d", G_parameter.ste_param.rcv);
		return;
	}
	if(G_importance_parameter_data.rcv == 0 || G_importance_parameter_data.rcv == 255 || G_importance_parameter_data.rcv != 99)
	{
	    LOGD(L_APP, L_V5, "error2,%d", G_importance_parameter_data.rcv);
		return;
	}
    l += OTA_snprintf(importance_backup.data + l, len - l, "ASETAPN,%s\r\n", (G_importance_parameter_data.apn.custom_apn_valid ? s_OFF : s_ON));
    if(G_importance_parameter_data.apn.custom_apn_valid)
    {
        extern nvram_GPRS_APN_struct *track_socket_get_use_apn(void);
        nvram_GPRS_APN_struct *use_apn = track_socket_get_use_apn();
        l += OTA_snprintf(importance_backup.data + l, len - l, "APN,%s,%s,%s\r\n", use_apn->apn, use_apn->username, use_apn->password);
    }
#ifdef __SAME_GT02__
    l += OTA_snprintf(importance_backup.data + l, len - l, "SYS_VERSION,%d,%d\r\n", G_importance_parameter_data.app_version.version, CRC);
#endif /* __SAME_GT02__ */

#if defined(__BACKUP_SERVER__)
    {
        if(G_importance_parameter_data.bserver.conecttype == 0)
        {
            l += OTA_snprintf(importance_backup.data + l, len - l, "BSERVER,0,%d.%d.%d.%d,%d,%d\r\n",
                              G_importance_parameter_data.bserver.server_ip[0], G_importance_parameter_data.bserver.server_ip[1],
                              G_importance_parameter_data.bserver.server_ip[2], G_importance_parameter_data.bserver.server_ip[3],
                              G_importance_parameter_data.bserver.server_port, G_importance_parameter_data.bserver.soc_type);
        }
        else
        {
            l += OTA_snprintf(importance_backup.data + l, len - l, "BSERVER,%d,%s,%d,%d\r\n",
                              G_importance_parameter_data.bserver.conecttype,
                              track_domain_decode(G_importance_parameter_data.bserver.url),
                              G_importance_parameter_data.bserver.server_port,
                              G_importance_parameter_data.bserver.soc_type);
        }
    }
#endif
/*
    if(0)//(G_importance_parameter_data.dserver.server_type)
    {
        if(G_importance_parameter_data.dserver.conecttype == 0)
        {
            l += OTA_snprintf(importance_backup.data + l, len - l, "DSERVER,0,%d.%d.%d.%d,%d,%d\r\n",
                              G_importance_parameter_data.dserver.server_ip[0], G_importance_parameter_data.dserver.server_ip[1],
                              G_importance_parameter_data.dserver.server_ip[2], G_importance_parameter_data.dserver.server_ip[3],
                              G_importance_parameter_data.dserver.server_port, G_importance_parameter_data.dserver.soc_type);
        }
        else
        {
            l += OTA_snprintf(importance_backup.data + l, len - l, "DSERVER,%d,%s,%d,%d\r\n",
                              G_importance_parameter_data.dserver.conecttype,
                              track_domain_decode(G_importance_parameter_data.dserver.url),
                              G_importance_parameter_data.dserver.server_port,
                              G_importance_parameter_data.dserver.soc_type);
        }
    }
    else
    {
    */
        if(G_importance_parameter_data.server.conecttype == 0)
        {
            l += OTA_snprintf(importance_backup.data + l, len - l, "SERVER,0,%d.%d.%d.%d,%d,%d\r\n",
                              G_importance_parameter_data.server.server_ip[0], G_importance_parameter_data.server.server_ip[1],
                              G_importance_parameter_data.server.server_ip[2], G_importance_parameter_data.server.server_ip[3],
                              G_importance_parameter_data.server.server_port, G_importance_parameter_data.server.soc_type);
        }
        else
        {
            l += OTA_snprintf(importance_backup.data + l, len - l, "SERVER,%d,%s,%d,%d\r\n",
                              G_importance_parameter_data.server.conecttype,
                              track_domain_decode(G_importance_parameter_data.server.url),
                              G_importance_parameter_data.server.server_port,
                              G_importance_parameter_data.server.soc_type);
        }
    //}
    if(G_importance_parameter_data.server2.conecttype == 0)
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "SERVER2,0,%d.%d.%d.%d,%d,%d\r\n",
                          G_importance_parameter_data.server2.server_ip[0], G_importance_parameter_data.server2.server_ip[1],
                          G_importance_parameter_data.server2.server_ip[2], G_importance_parameter_data.server2.server_ip[3],
                          G_importance_parameter_data.server2.server_port, G_importance_parameter_data.server2.soc_type);
    }
    else
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "SERVER2,%d,%s,%d,%d\r\n",
                          G_importance_parameter_data.server2.conecttype,
                          track_domain_decode(G_importance_parameter_data.server2.url),
                          G_importance_parameter_data.server2.server_port,
                          G_importance_parameter_data.server2.soc_type);
    }
    /*
    l += OTA_snprintf(importance_backup.data + l, len - l, "SERVER,0,%d.%d.%d.%d,%d,%d\r\n",
            G_importance_parameter_data.server.server_ip[0], G_importance_parameter_data.server.server_ip[1],
            G_importance_parameter_data.server.server_ip[2], G_importance_parameter_data.server.server_ip[3],
            G_importance_parameter_data.server.server_port, G_importance_parameter_data.server.soc_type);

    l += OTA_snprintf(importance_backup.data + l, len - l, "SERVER2,0,%d.%d.%d.%d,%d,%d\r\n",
            G_importance_parameter_data.server2.server_ip[0], G_importance_parameter_data.server2.server_ip[1],
            G_importance_parameter_data.server2.server_ip[2], G_importance_parameter_data.server2.server_ip[3],
            G_importance_parameter_data.server2.server_port, G_importance_parameter_data.server2.soc_type);
    */

#if defined(__GT740__)
    if(G_parameter.extchip.mode == 3)
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "MODE,3,%d,%0.2d:%0.2d\r\n", G_parameter.extchip.mode3_par1, G_parameter.extchip.mode3_par2 / 60, G_parameter.extchip.mode3_par2 % 60);
    }
    else if(G_parameter.extchip.mode == 4)
    {
        char m_buf[100] = {0};
        U8 vle = G_parameter.extchip.mode4_par1, i = 0;
        for(; i < vle; i++)
        {
            if(0 == i)
            {
                snprintf(m_buf + strlen(m_buf), 99, "%0.2d:%0.2d", G_parameter.extchip.mode4_par2[i] / 60, G_parameter.extchip.mode4_par2[i] % 60);
            }
            else
            {
                snprintf(m_buf + strlen(m_buf), 99, ",%0.2d:%0.2d", G_parameter.extchip.mode4_par2[i] / 60, G_parameter.extchip.mode4_par2[i] % 60);
            }
        }
        l += OTA_snprintf(importance_backup.data + l, len - l, "MODE,4,%d,%s\r\n", vle, m_buf);
    }
    else
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "MODE,2,%d:%d,%d\r\n", G_parameter.extchip.mode2_par1 / 60, G_parameter.extchip.mode2_par1 % 60, G_parameter.extchip.mode2_par2 / 60);
    }
    l += OTA_snprintf(importance_backup.data + l, len - l, "TRACKTIME,%d\r\n", G_realtime_data.tracking_time);
    l += OTA_snprintf(importance_backup.data + l, len - l, "GPSWAKE,%s\r\n",   ON_OFF(G_parameter.gpswake));
#endif


    //l += OTA_snprintf(importance_backup.data + l, len - l, "EURL,%s\r\n", G_parameter.url);

    l += OTA_snprintf(importance_backup.data + l, len - l, "LANG,%d\r\n", G_parameter.lang);

    /*��Ҫ�ص����*/
//   l += OTA_snprintf(importance_backup.data + l, len - l, "IMEI,1,%s\r\n", (char *)track_drv_get_imei(0));

    l += OTA_snprintf(importance_backup.data + l, len - l, "AGPS,%s\r\n", ON_OFF(G_parameter.agps.sw));

    if(G_importance_parameter_data.server.lock)
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "LOCKIP\r\n");
    }
#if defined(__NF2318__)
	l += OTA_snprintf(importance_backup.data + l, len - l, "DEVICESN,%s\r\n",G_importance_parameter_data.SequenceNumber);
	l += OTA_snprintf(importance_backup.data + l, len - l, "LEASE,%d\r\n",G_parameter.ste_status.lease);
	l += OTA_snprintf(importance_backup.data + l, len - l, "MELIAGE,%d,%d\r\n",G_parameter.ele_data.oncemileage,G_parameter.ele_data.totalmileage);
	l += OTA_snprintf(importance_backup.data + l, len - l, "ENCODETYPE,%d\r\n",G_parameter.ste_status.encode_type);

#endif

    l += OTA_snprintf(importance_backup.data + l, len - l, "ASETGMT,%s\r\n", ON_OFF(G_parameter.zone.timezone_auto));
    if(G_parameter.zone.timezone_auto == 0 && (G_parameter.zone.zone == 'E' || G_parameter.zone.zone=='W'))
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "GMT,%c,%d,%d\r\n", G_parameter.zone.zone, G_parameter.zone.time, G_parameter.zone.time_min);
    }

    //l += OTA_snprintf(importance_backup.data + l, len - l, "CENTER,A,%s\r\n", G_parameter.centerNumber);

    //l += OTA_snprintf(importance_backup.data + l, len - l, "SOS,A,%s,%s,%s\r\n", G_parameter.sos_num[0], G_parameter.sos_num[1], G_parameter.sos_num[2]);
    //l += OTA_snprintf(importance_backup.data + l, len - l, "SOSALM,%s,%d\r\n", ON_OFF(G_parameter.sos_alarm.sw), G_parameter.sos_alarm.alarm_type);
    //l += OTA_snprintf(importance_backup.data + l, len - l, "ALMREP,%d,%d,%d\r\n", G_phone_number.almrep[0], G_phone_number.almrep[1], G_phone_number.almrep[2]);

    l += OTA_snprintf(importance_backup.data + l, len - l, "PERMIT,%d\r\n", G_parameter.Permission.Permit);

    /*��Ҫ�ص����*/
#if !defined(__NF2318__)
    l += OTA_snprintf(importance_backup.data + l, len - l, "PWDSW\r\n", ON_OFF(G_parameter.Permission.password_sw));
    if(G_parameter.Permission.password_sw)
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "PASSWORD,%s\r\n", G_parameter.Permission.password);
    }
    if(G_parameter.speed_limit_alarm.sw)
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "SPEED,%s,%d,%d,%d\r\n",
                          ON_OFF(G_parameter.speed_limit_alarm.sw), G_parameter.speed_limit_alarm.delay_time,
                          G_parameter.speed_limit_alarm.threshold, G_parameter.speed_limit_alarm.alarm_type);
    }
    else
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "SPEED,%s\r\n",
                          ON_OFF(G_parameter.speed_limit_alarm.sw));
    }
#endif
    if(G_parameter.power_fails_alarm.sw)
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "POWERALM,%s,%d,%d,%d,%d\r\n",
                          ON_OFF(G_parameter.power_fails_alarm.sw),
                          G_parameter.power_fails_alarm.alarm_type,
                          G_parameter.power_fails_alarm.delay_time_off,
                          G_parameter.power_fails_alarm.delay_time_on,
                          G_parameter.power_fails_alarm.delay_time_acc_on_to_off);
    }
    else
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "POWERALM,%s\r\n",
                          ON_OFF(G_parameter.power_fails_alarm.sw));
    }
    if(G_parameter.low_power_alarm.sw)
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "BATALM,%s,%d\r\n", ON_OFF(G_parameter.low_power_alarm.sw), G_parameter.low_power_alarm.alarm_type);
    }
    else
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "BATALM,%s\r\n", ON_OFF(G_parameter.low_power_alarm.sw));
    }
    if(G_parameter.vibrates_alarm.sw)
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "SENALM,%s,%d\r\n", ON_OFF(G_parameter.vibrates_alarm.sw), G_parameter.vibrates_alarm.alarm_type);
    }
    else
    {
        l += OTA_snprintf(importance_backup.data + l, len - l, "SENALM,%s\r\n", ON_OFF(G_parameter.vibrates_alarm.sw));
    }
#if defined(__NT36__)||defined(__MT200__)
    l += OTA_snprintf(importance_backup.data + l, len - l, "SENSOR,%d,%d,%d\r\n",
                      G_parameter.vibrates_alarm.detection_time, G_parameter.vibrates_alarm.alarm_delay_time,
                      G_parameter.vibrates_alarm.alarm_interval / 60);
#else
    l += OTA_snprintf(importance_backup.data + l, len - l, "SENSOR2,%d,%d,%d\r\n",
                      G_parameter.vibrates_alarm.detection_time, G_parameter.vibrates_alarm.alarm_delay_time,
                      G_parameter.vibrates_alarm.alarm_interval);
#endif

    l += OTA_snprintf(importance_backup.data + l, len - l, "GPRSON,%d\r\n", G_parameter.gprson);

    l += OTA_snprintf(importance_backup.data + l, len - l, "LINK,%d\r\n", G_parameter.gprs_obstruction_alarm.link);

    /*��Ҫ�ص����*/
    l += OTA_snprintf(importance_backup.data + l, len - l, "HBT,S%d,S%d\r\n", G_parameter.hbt_acc_on, G_parameter.hbt_acc_off);
    l += OTA_snprintf(importance_backup.data + l, len - l, "ADC_BC,B,%d\r\n", G_parameter.adc4_offset);
    l += OTA_snprintf(importance_backup.data + l, len - l, "DEFMODE,%d\r\n", G_realtime_data.defense_mode);
#if defined(__OBD__)
	l += OTA_snprintf(importance_backup.data + l, len - l, "OBDCMD,%s\r\n", ON_OFF(G_parameter.obd.sw));
#endif

    /*
    l += OTA_snprintf(importance_backup.data + l, len - l, "REPORT\r\n", );
    l += OTA_snprintf(importance_backup.data + l, len - l, "\r\n", );
    l += OTA_snprintf(importance_backup.data + l, len - l, "\r\n", );
    l += OTA_snprintf(importance_backup.data + l, len - l, "\r\n", );
    l += OTA_snprintf(importance_backup.data + l, len - l, "\r\n", );
    l += OTA_snprintf(importance_backup.data + l, len - l, "\r\n", );
    l += OTA_snprintf(importance_backup.data + l, len - l, "\r\n", );
    l += OTA_snprintf(importance_backup.data + l, len - l, "\r\n", );
    l += OTA_snprintf(importance_backup.data + l, len - l, "\r\n", );
    l += OTA_snprintf(importance_backup.data + l, len - l, "\r\n", );
    l += OTA_snprintf(importance_backup.data + l, len - l, "\r\n", );*/



#if defined(__GT300S__)
    if(G_parameter.work_mode.mode == 2)
    {
        sprintf(importance_backup.data + l, "MODE,%d,%d",  G_parameter.work_mode.mode, G_parameter.gps_work.Fti.interval_acc_on);
    }
    else  if(G_parameter.work_mode.mode == 1)
    {
        sprintf(importance_backup.data + l, "MODE,%d,%d", G_parameter.work_mode.mode, G_parameter.work_mode.mode1_timer);

    }
    else if(G_parameter.work_mode.mode == 3)
    {
        sprintf(importance_backup.data + l, "MODE,%d,%0.2d:%0.2d,%d",  G_parameter.work_mode.mode,
                G_parameter.work_mode.datetime / 60,  G_parameter.work_mode.datetime % 60,  G_parameter.work_mode.mode3_timer / 60);
    }
#elif defined(__GT370__)|| defined (__GT380__)
    if(G_parameter.work_mode.mode == 2)
    {
        sprintf(importance_backup.data + l, "MODE,%d,%d,%d",  G_parameter.work_mode.mode, G_parameter.gps_work.Fti.interval_acc_on, G_parameter.gps_work.Fti.interval_acc_off);
    }
    else  if(G_parameter.work_mode.mode == 1)
    {
        sprintf(importance_backup.data + l, "MODE,%d,%d,%d", G_parameter.work_mode.mode, G_parameter.gps_work.Fti.interval_acc_on, G_parameter.gps_work.Fti.interval_acc_off);

    }
    else if(G_parameter.work_mode.mode == 3)
    {
        sprintf(importance_backup.data + l, "MODE,%d,%0.2d:%0.2d,%d",  G_parameter.work_mode.mode,
                G_parameter.work_mode.datetime / 60,  G_parameter.work_mode.datetime % 60,	G_parameter.work_mode.mode3_timer / 60);
    }

#endif /* __GT300S__ */
#if defined(__NF2318__)
	//l += OTA_snprintf(importance_backup.data + l, len - l, "DEVICESN,%s\r\n",G_importance_parameter_data.SequenceNumber);
	//l += OTA_snprintf(importance_backup.data + l, len - l, "LEASE,%d\r\n",G_parameter.ste_status.lease);
	//l += OTA_snprintf(importance_backup.data + l, len - l, "MELIAGE,%d,%d\r\n",G_parameter.ele_data.oncemileage,G_parameter.ele_data.totalmileage);
	//l += OTA_snprintf(importance_backup.data + l, len - l, "ENCODETYPE,%d\r\n",G_parameter.ste_status.encode_type);
	
	l += OTA_snprintf(importance_backup.data + l, len - l, "ADVTYPE,%d\r\n",G_parameter.ste_status.btadvtype);
	l += OTA_snprintf(importance_backup.data + l, len - l, "DISARM,%d\r\n",G_realtime_data.defences_status);
	l += OTA_snprintf(importance_backup.data + l, len - l, "AVB,%d,%d,%d\r\n",G_parameter.voice_ver[1],G_parameter.voice_ver[2],G_parameter.voice_ver[3]);
	l += OTA_snprintf(importance_backup.data + l, len - l, "DIAMETER,%d\r\n",G_parameter.ste_param.diameter);
	l += OTA_snprintf(importance_backup.data + l, len - l, "ALNICONUM,%d\r\n",G_parameter.ste_param.alnicounum);
	l += OTA_snprintf(importance_backup.data + l, len - l, "VIBL,%d\r\n",G_parameter.ste_param.viblmg);
	l += OTA_snprintf(importance_backup.data + l, len - l, "BLEKG,%d\r\n",G_parameter.ste_param.btsw);
	l += OTA_snprintf(importance_backup.data + l, len - l, "FREQ,%d\r\n",G_parameter.gps_work.Fti.interval_acc_on);
	//�µı���Ĳ����ɣ�2019.02.23
	l += OTA_snprintf(importance_backup.data + l, len - l, "SLEEPT,%d\r\n",G_parameter.ste_param.sleept);
	l += OTA_snprintf(importance_backup.data + l, len - l, "DFTLOCKVOICEPLAYT,%d\r\n",G_parameter.ste_param.dftlockvoiceplayt);
	l += OTA_snprintf(importance_backup.data + l, len - l, "MAXECUSPEED,%d\r\n",G_parameter.ele_data.turn_limit);
	l += OTA_snprintf(importance_backup.data + l, len - l, "ALARMFREQ,%d\r\n",G_parameter.ste_param.alarmreq);
	l += OTA_snprintf(importance_backup.data + l, len - l, "DFTTIMEOUT,%d\r\n",G_parameter.ste_param.dfttimeout);
	l += OTA_snprintf(importance_backup.data + l, len - l, "OSDTIMEINTERVAL,%d\r\n",G_parameter.osd.osdtimeinterval);
	l += OTA_snprintf(importance_backup.data + l, len - l, "OSDTIMEMAX,%d\r\n",G_parameter.osd.osdtimemax);

	
	l += OTA_snprintf(importance_backup.data + l, len - l, "OSDSPEED,%d\r\n",G_parameter.osd.osdspeed);
	l += OTA_snprintf(importance_backup.data + l, len - l, "BWOT,%d\r\n",(G_parameter.bwtopen*100));
	l += OTA_snprintf(importance_backup.data + l, len - l, "BWCT,%d\r\n",(G_parameter.bwtclose*100));
	l += OTA_snprintf(importance_backup.data + l, len - l, "ELEOT,%d\r\n",(G_parameter.eletopen*100));
	l += OTA_snprintf(importance_backup.data + l, len - l, "ELECT,%d\r\n",(G_parameter.eletclose*100));
	
	value1 = ((G_parameter.osd.osdvoiceid[0]&0x0000FF)<<16) |((G_parameter.osd.osdvoiceid[1]&0x0000FF)<<8) | G_parameter.osd.osdvoiceid[2];
	value2 = ((G_parameter.osd.osdringid[0]&0x0000FF)<<16) |((G_parameter.osd.osdringid[1]&0x0000FF)<<8) | G_parameter.osd.osdringid[2];
	l += OTA_snprintf(importance_backup.data + l, len - l, "OSDRING,%d:%d\r\n",value1,value2);
	
	l += OTA_snprintf(importance_backup.data + l, len - l, "OSDSWITCH,%d\r\n",G_parameter.osd.osdswitch);
	l += OTA_snprintf(importance_backup.data + l, len - l, "VIBLVOICESW,%s\r\n",ON_OFF(G_parameter.viblvoicesw));
	
	//l += OTA_snprintf(importance_backup.data + l, len - l, "VOICESIZE,%d\r\n",G_parameter.voicefilesize);
	
	l += OTA_snprintf(importance_backup.data + l, len - l, "BTNAME,%s\r\n",G_parameter.ste_param.bt_name);
		//________��������
	l += OTA_snprintf(importance_backup.data + l, len - l, "BTPASSWD,%s\r\n",G_parameter.encode_token_s);
	l += OTA_snprintf(importance_backup.data + l, len - l, "NEWBTPSWD,%s\r\n",G_parameter.new_encode_token_s);
	l += OTA_snprintf(importance_backup.data + l, len - l, "NEWBTPSTA,%d\r\n",G_parameter.bt_token_mark);
	//l += OTA_snprintf(importance_backup.data + l, len - l, "RREASON,%d\r\n",G_realtime_data.fota_update);
	//----------------------20191010
	l += OTA_snprintf(importance_backup.data + l, len - l,"LESOFF,%d,%d\r\n",G_realtime_data.lesea_off_to_sta,G_realtime_data.lesea_off_to_volume_sta);
	
#endif
    //LOGD(L_DRV, L_V5, "result=%d,strlen(data)=%d ,%s", len, strlen(importance_backup.data), importance_backup.data);
    l = strlen(importance_backup.data);
    LOGD(L_DRV, L_V5, "result=%d,strlen(data)=%d, stringCRC:%0.4X,\r\n%s", len, l, GetCrc16(importance_backup.data, l), importance_backup.data);
    Track_nvram_write(DISK1_BACKUP_LID, 1, (void *)&importance_backup, NVRAM_EF_DRV__IMP_BACKUP_SIZE);
}

void track_os_importance_backup_read(void)
{
    int l;
    LOGD(L_APP, L_V5, "");

    memset(&importance_backup, 0x00, sizeof(importance_backup));
    Track_nvram_read(DISK1_BACKUP_LID, 1, (void *)&importance_backup, NVRAM_EF_DRV__IMP_BACKUP_SIZE);
    l = strlen(importance_backup.data);
    LOGD(L_DRV, L_V7, "strlen(data)=%d, stringCRC:%0.4X,\r\n%s", l, GetCrc16(importance_backup.data, l), importance_backup.data);
    track_cmd_params_recv(importance_backup.data, CM_PARAM_CMD);
}

