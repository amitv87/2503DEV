#ifndef _CUSTCNTL_H
#define _CUSTCNTL_H
#if __GNUC__ >=3
#pragma GCC system_header
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CCF_NOTEXT 1
#define CCHCCCLASS 32
#define CCHCCDESC 32
#define CCHCCTEXT 256

typedef struct tagCCSTYLEA {
	DWORD flStyle;
	DWORD flExtStyle;
	CHAR szText[CCHCCTEXT];
	LANGID lgid;
	WORD wReserved1;
} CCSTYLEA,*LPCCSTYLEA;
typedef struct tagCCSTYLEW {
	DWORD flStyle;
	DWORD flExtStyle;
	WCHAR szText[CCHCCTEXT];
	LANGID lgid;
	WORD wReserved1;
} CCSTYLEW,*LPCCSTYLEW;
typedef struct tagCCSTYLEFLAGA {
	DWORD flStyle;
	DWORD flStyleMask;
	LPSTR pszStyle;
} CCSTYLEFLAGA,*LPCCSTYLEFLAGA;
typedef struct tagCCSTYLEFLAGW {
	DWORD flStyle;
	DWORD flStyleMask;
	LPWSTR pszStyle;
} CCSTYLEFLAGW,*LPCCSTYLEFLAGW;
typedef BOOL(CALLBACK* LPFNCCSTYLEA)(HWND,LPCCSTYLEA);
typedef INT(CALLBACK* LPFNCCSIZETOTEXTA)(DWORD,DWORD,HFONT,LPSTR);
typedef struct tagCCINFOA {
	CHAR szClass[CCHCCCLASS];
	DWORD flOptions;
	CHAR szDesc[CCHCCDESC];
	UINT cxDefault;
	UINT cyDefault;
	DWORD flStyleDefault;
	DWORD flExtStyleDefault;
	DWORD flCtrlTypeMask;
	CHAR szTextDefault[CCHCCTEXT];
	INT cStyleFlags;
	LPCCSTYLEFLAGA aStyleFlags;
	LPFNCCSTYLEA lpfnStyle;
	LPFNCCSIZETOTEXTA lpfnSizeToText;
	DWORD dwReserved1;
	DWORD dwReserved2;
} CCINFOA,*LPCCINFOA;
typedef UINT(CALLBACK* LPFNCCINFOA)(LPCCINFOA);
typedef BOOL(CALLBACK* LPFNCCSTYLEW)(HWND,LPCCSTYLEW);
typedef INT (CALLBACK* LPFNCCSIZETOTEXTW)(DWORD,DWORD,HFONT,LPWSTR);
typedef struct tagCCINFOW {
	WCHAR szClass[CCHCCCLASS];
	DWORD flOptions;
	WCHAR szDesc[CCHCCDESC];
	UINT cxDefault;
	UINT cyDefault;
	DWORD flStyleDefault;
	DWORD flExtStyleDefault;
	DWORD flCtrlTypeMask;
	WCHAR szTextDefault[CCHCCTEXT];
	INT cStyleFlags;
	LPCCSTYLEFLAGW aStyleFlags;
	LPFNCCSTYLEW lpfnStyle;
	LPFNCCSIZETOTEXTW lpfnSizeToText;
	DWORD dwReserved1;
	DWORD dwReserved2;
} CCINFOW,*LPCCINFOW;
typedef UINT(CALLBACK* LPFNCCINFOW)(LPCCINFOW);

UINT CALLBACK CustomControlInfoA(LPCCINFOA acci);
UINT CALLBACK CustomControlInfoW(LPCCINFOW acci);

#ifdef UNICODE
typedef CCSTYLEW CCSTYLE,*LPCCSTYLE;
typedef CCSTYLEFLAGW CCSTYLEFLAG,*LPCCSTYLEFLAG;
typedef CCINFOW CCINFO,*LPCCINFO;
#define LPFNCCSTYLE LPFNCCSTYLEW
#define LPFNCCSIZETOTEXT LPFNCCSIZETOTEXTW
#define LPFNCCINFO LPFNCCINFOW
#else
typedef CCSTYLEA CCSTYLE,*LPCCSTYLE;
typedef CCSTYLEFLAGA CCSTYLEFLAG,*LPCCSTYLEFLAG;
typedef CCINFOA CCINFO,*LPCCINFO;
#define LPFNCCSTYLE LPFNCCSTYLEA
#define LPFNCCSIZETOTEXT LPFNCCSIZETOTEXTA
#define LPFNCCINFO LPFNCCINFOA
#endif
#ifdef __cplusplus
}
#endif
#endif
