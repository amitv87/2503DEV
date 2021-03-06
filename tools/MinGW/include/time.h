/* 
 * time.h
 *
 * Date and time functions and types.
 *
 * This file is part of the Mingw32 package.
 *
 * Contributors:
 *  Created by Colin Peters <colin@bird.fu.is.saga-u.ac.jp>
 *
 *  THIS SOFTWARE IS NOT COPYRIGHTED
 *
 *  This source code is offered for use in the public domain. You may
 *  use, modify or distribute it freely.
 *
 *  This code is distributed in the hope that it will be useful but
 *  WITHOUT ANY WARRANTY. ALL WARRANTIES, EXPRESS OR IMPLIED ARE HEREBY
 *  DISCLAIMED. This includes but is not limited to warranties of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Revision: 1.7 $
 * $Author: earnie $
 * $Date: 2003/05/03 13:48:46 $
 *
 */

#ifndef	_TIME_H_
#define	_TIME_H_

/* All the headers include this file. */
#include <_mingw.h>

#define __need_wchar_t
#define __need_size_t
#define __need_NULL
#ifndef RC_INVOKED
#include <stddef.h>
#endif	/* Not RC_INVOKED */

/*
 * Need a definition of time_t.
 */
#include <sys/types.h>

/*
 * Number of clock ticks per second. A clock tick is the unit by which
 * processor time is measured and is returned by 'clock'.
 */
#define	CLOCKS_PER_SEC	((clock_t)1000)
#define	CLK_TCK		CLOCKS_PER_SEC


#ifndef RC_INVOKED

/*
 * A type for storing the current time and date. This is the number of
 * seconds since midnight Jan 1, 1970.
 * NOTE: Normally this is defined by the above include of sys/types.h
 */
#ifndef _TIME_T_DEFINED
typedef	long	time_t;
#define _TIME_T_DEFINED
#endif

/*
 * A type for measuring processor time (in clock ticks).
 */
#ifndef _CLOCK_T_DEFINED
typedef	long	clock_t;
#define _CLOCK_T_DEFINED
#endif


/*
 * A structure for storing all kinds of useful information about the
 * current (or another) time.
 */
struct tm
{
	int	tm_sec;		/* Seconds: 0-59 (K&R says 0-61?) */
	int	tm_min;		/* Minutes: 0-59 */
	int	tm_hour;	/* Hours since midnight: 0-23 */
	int	tm_mday;	/* Day of the month: 1-31 */
	int	tm_mon;		/* Months *since* january: 0-11 */
	int	tm_year;	/* Years since 1900 */
	int	tm_wday;	/* Days since Sunday (0-6) */
	int	tm_yday;	/* Days since Jan. 1: 0-365 */
	int	tm_isdst;	/* +1 Daylight Savings Time, 0 No DST,
				 * -1 don't know */
};

#ifdef	__cplusplus
extern "C" {
#endif

_CRTIMP clock_t __cdecl	clock (void);
_CRTIMP time_t __cdecl	time (time_t*);
_CRTIMP double __cdecl	difftime (time_t, time_t);
_CRTIMP time_t __cdecl	mktime (struct tm*);

/*
 * These functions write to and return pointers to static buffers that may
 * be overwritten by other function calls. Yikes!
 *
 * NOTE: localtime, and perhaps the others of the four functions grouped
 * below may return NULL if their argument is not 'acceptable'. Also note
 * that calling asctime with a NULL pointer will produce an Invalid Page
 * Fault and crap out your program. Guess how I know. Hint: stat called on
 * a directory gives 'invalid' times in st_atime etc...
 */
_CRTIMP char* __cdecl		asctime (const struct tm*);
_CRTIMP char* __cdecl		ctime (const time_t*);
_CRTIMP struct tm*  __cdecl	gmtime (const time_t*);
_CRTIMP struct tm*  __cdecl	localtime (const time_t*);


_CRTIMP size_t __cdecl	strftime (char*, size_t, const char*, const struct tm*);

_CRTIMP size_t __cdecl	wcsftime (wchar_t*, size_t, const wchar_t*, const struct tm*);

#ifndef __STRICT_ANSI__
extern _CRTIMP void __cdecl	_tzset (void);

#ifndef _NO_OLDNAMES
extern _CRTIMP void __cdecl	tzset (void);
#endif

_CRTIMP char* __cdecl	_strdate(char*);
_CRTIMP char* __cdecl	_strtime(char*);

#endif	/* Not __STRICT_ANSI__ */

/*
 * _daylight: non zero if daylight savings time is used.
 * _timezone: difference in seconds between GMT and local time.
 * _tzname: standard/daylight savings time zone names (an array with two
 *          elements).
 */
#ifdef __MSVCRT__

/* These are for compatibility with pre-VC 5.0 suppied MSVCRT. */
extern _CRTIMP int* __cdecl	__p__daylight (void);
extern _CRTIMP long* __cdecl	__p__timezone (void);
extern _CRTIMP char** __cdecl	__p__tzname (void);

__MINGW_IMPORT int	_daylight;
__MINGW_IMPORT long	_timezone;
__MINGW_IMPORT char 	*_tzname[2];

#else /* not __MSVCRT (ie. crtdll) */

#ifndef __DECLSPEC_SUPPORTED

extern int*	_imp___daylight_dll;
extern long*	_imp___timezone_dll;
extern char**	_imp___tzname;

#define _daylight	(*_imp___daylight_dll)
#define _timezone	(*_imp___timezone_dll)
#define _tzname		(*_imp___tzname)

#else /* __DECLSPEC_SUPPORTED */

__MINGW_IMPORT int	_daylight_dll;
__MINGW_IMPORT long	_timezone_dll;
__MINGW_IMPORT char*	_tzname[2];

#define _daylight	_daylight_dll
#define _timezone	_timezone_dll

#endif /* __DECLSPEC_SUPPORTED */

#endif /* not __MSVCRT__ */

#ifndef _NO_OLDNAMES

#ifdef __MSVCRT__

/* These go in the oldnames import library for MSVCRT. */
__MINGW_IMPORT int	daylight;
__MINGW_IMPORT long	timezone;
__MINGW_IMPORT char 	*tzname[2];

#ifndef _WTIME_DEFINED

/* wide function prototypes, also declared in wchar.h */

_CRTIMP wchar_t* __cdecl	_wasctime(const struct tm*);
_CRTIMP wchar_t* __cdecl	_wctime(const time_t*);
_CRTIMP wchar_t* __cdecl	_wstrdate(wchar_t*);
_CRTIMP wchar_t* __cdecl	_wstrtime(wchar_t*);

#define _WTIME_DEFINED
#endif /* _WTIME_DEFINED */ 


#else /* not __MSVCRT__ */

/* CRTDLL is royally messed up when it comes to these macros.
   TODO: import and alias these via oldnames import library instead 
   of macros.  */

#define daylight        _daylight
/* NOTE: timezone not defined because it would conflict with sys/timeb.h.
   Also, tzname used to a be macro, but now it's in moldname. */
__MINGW_IMPORT char 	*tzname[2];

#endif /* not __MSVCRT__ */

#endif	/* Not _NO_OLDNAMES */

#ifdef	__cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* Not _TIME_H_ */

