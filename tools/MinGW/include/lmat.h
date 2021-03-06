#ifndef _LMAT_H
#define _LMAT_H
#if __GNUC__ >=3
#pragma GCC system_header
#endif

#ifdef __cplusplus
extern "C" {
#endif
#define JOB_RUN_PERIODICALLY	1
#define JOB_EXEC_ERROR	2
#define JOB_RUNS_TODAY	4
#define JOB_ADD_CURRENT_DATE	8
#define JOB_NONINTERACTIVE	16
#define JOB_INPUT_FLAGS	(JOB_RUN_PERIODICALLY|JOB_ADD_CURRENT_DATE|JOB_NONINTERACTIVE)
#define JOB_OUTPUT_FLAGS (JOB_RUN_PERIODICALLY|JOB_EXEC_ERROR|JOB_RUNS_TODAY|JOB_NONINTERACTIVE)
typedef struct _AT_ENUM {
	DWORD JobId;
	DWORD JobTime;
	DWORD DaysOfMonth;
	UCHAR DaysOfWeek;
	UCHAR Flags;
	LPWSTR Command;
} AT_ENUM,*PAT_ENUM,*LPAT_ENUM;
typedef struct _AT_INFO {
	DWORD JobTime;
	DWORD DaysOfMonth;
	UCHAR DaysOfWeek;
	UCHAR Flags;
	LPWSTR Command;
} AT_INFO,*PAT_INFO,*LPAT_INFO;
NET_API_STATUS WINAPI NetScheduleJobAdd(LPWSTR,PBYTE,LPDWORD);
NET_API_STATUS WINAPI NetScheduleJobDel(LPWSTR,DWORD,DWORD);
NET_API_STATUS WINAPI NetScheduleJobEnum(LPWSTR,PBYTE*,DWORD,PDWORD,PDWORD,PDWORD);
NET_API_STATUS WINAPI NetScheduleJobGetInfo(LPWSTR,DWORD,PBYTE*);
#ifdef __cplusplus
}
#endif
#endif
