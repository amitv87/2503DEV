
/*******
�汾�����ñ����
Ӧ�ð汾�ŵĸ��ģ��޸��ļ���\track\cust\cust_inc\track_version.h
TRACK_VER_APP_  ��ʾӦ�ð汾
TRACK_VER_PARAMETER_  ��ʾ�����汾��һ��������㣬ֻ����Ҫ�����������������ʱ��Ҫ�޸ģ�
TRACK_VER_BASE_ ��ʾ�����汾��������δ�������������޸ģ����޸���ζ����ͬ�Ļ����汾֧�ֽ�������
ԭ\make\Verno_GXQ60D_MOD_11B.bld �ļ�������Ҫ�޸ġ�
******/

/*��Ŀǰ�ĸ�ʽ��չ��ÿ����Ŀ��������3��#define�ؼ��֣�����ͱ���       --    chengjun  2016-07-15*/

#if defined (__NT17__)
#define _TRACK_VER_PARAMETER_   "R0"
#define _TRACK_VER_BASE_        "NT17_10_A1DE_D23_F1"
#define _TRACK_VER_APP_         "NT17_10_A1DE_D23_V01"

#elif defined (__NT55__)
#define _TRACK_VER_PARAMETER_   "R0"
#define _TRACK_VER_BASE_        "NT55_10_A1DE_D23_F1"
#define _TRACK_VER_APP_         "NT55_10_A1DE_D23_V01"

#elif defined(__EL100__)
#define _TRACK_VER_PARAMETER_   "R0"
#define _TRACK_VER_BASE_        "EL100_20_A1DE_D23_F1"
#define _TRACK_VER_APP_         "EL100_20_A1DE_D23_V01"
#elif defined(__NT13V__)
#define _TRACK_VER_PARAMETER_   "R0"
#define _TRACK_VER_BASE_        "NT13V_10_A1AE_D23_F1"
#define _TRACK_VER_APP_         "NT13V_10_A1AE_D23_V01"

#elif defined(__QB200__)
#define _TRACK_VER_PARAMETER_   "R0"
#define _TRACK_VER_BASE_        "QB200_10_A1DE_D23_F1"
#define _TRACK_VER_APP_         "QB200_10_A1DE_D23_V01_TEST"

#elif defined(__NF2319__)
#define _TRACK_VER_PARAMETER_   "R0"
#define _TRACK_VER_BASE_        "NF2319_10_A1AE_D23_F1"
#define _TRACK_VER_APP_         "NF2319_10_A1AE_D23_V01_TEST"
#elif defined (__NF2318__)
#if defined(__EL100_XYT__)
#define _TRACK_VER_PARAMETER_   "R0"
#define _TRACK_VER_BASE_        "NF2318_XYT_20_A1AE_D23_F1"
#define _TRACK_VER_APP_         "NF2318_XYT_20_A1AE_D23_V17"
#else
#define _TRACK_VER_PARAMETER_   "R0"
#define _TRACK_VER_BASE_        "NF2318_20_A1AE_D23_F1"
#define _TRACK_VER_APP_         "NF2318_20_A1AE_D23_V17"
#endif
#elif defined(__JM10__)
#define _TRACK_VER_PARAMETER_   "R0"
#define _TRACK_VER_BASE_        "JM10_10_A1AE_D23_F1"
#define _TRACK_VER_APP_         "JM10_10_A1AE_D23_V01_TEST"

#else
#error "project_null"
#endif

