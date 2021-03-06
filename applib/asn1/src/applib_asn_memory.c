/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2007
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 * applib_asn_memory.c
 *
 * Project:
 * --------
 *   Maui
 *
 * Description:
 * ------------
 *   This file is intends for memory alloc and free function for AGPS project.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

/****************************************************************************
 *
 * COMPONENT:   ASN
 * MODULE:      ASN_MEMORY
 * DESCRIPTION: Auto generated by MTK ASN.1 Compiler
 *
 ****************************************************************************/
//#include "kal_release.h"
//#include "app_buff_alloc.h"
//#include "applib_asn_memory.h"
#include "applib_asn_common.h"

///add for RHR
#include "kal_public_api.h"
#include "kal_general_types.h"
#include "applib_mtkasn_global.h"
#include "string.h"
///

extern  void  applib_asnMemAlloc(void *pVoid, void ** ppBuffer, unsigned long size)
{
	applib_AsnContext *pContext = (applib_AsnContext*) pVoid;
    if(size == 0)
    {
      *ppBuffer = (void *)0;
      return;
    }
	if (pContext->pMemAllocFunc != NULL)
	{
		pContext->pMemAllocFunc(ppBuffer, size, __FILE__, __LINE__);
	}
	else
	{
#ifdef  MCD_DLL_EXPORT
		*ppBuffer = (void *)malloc(size);
		memset(*ppBuffer ,0,size);
#else
		*ppBuffer = (void*) get_ctrl_buffer(size);
#endif 
		if(*ppBuffer == NULL)
		{
			ASSERT(0);
		}
	}
}

extern  void  applib_asnMemFree(void ** ppBuffer)
{
  if(NULL == *ppBuffer)
        return;
#ifdef  MCD_DLL_EXPORT
	free(*ppBuffer);
#else       
	free_ctrl_buffer(*ppBuffer);
#endif
	*ppBuffer = NULL;
}

extern void applib_asnMemAlloc2 (void *pVoid , void **ppBuffer, unsigned long uLen, const char *file, unsigned long line)
{
	applib_AsnContext *pContext = (applib_AsnContext*) pVoid;
    if(uLen == 0)
    {
      *ppBuffer = (void *)0;
      return;
    }
	if (pContext->pMemAllocFunc != NULL)
	{
		pContext->pMemAllocFunc(ppBuffer, uLen, file, line);
	}
	else
	{
#ifdef  MCD_DLL_EXPORT
		*ppBuffer = (void *)malloc(uLen);
		memset(*ppBuffer ,0,uLen);
#else
		*ppBuffer = (void*)get_ctrl_buffer(uLen) ;
#endif
		if(*ppBuffer == NULL)
		{
			ASSERT(0);
		}
	}
}

extern  void  applib_asnMemMove(void * pDest, const void * pSource, U32 uLen)
{
	memmove(pDest, pSource, uLen);
}

extern  void  applib_asnMemCopy(void * pDest, const void * pSource, U32 uLen)
{
	memcpy(pDest, pSource, uLen);
}

extern S32 applib_asnMemCmp(void *p1, void *p2, U32 uLen)
{
	return memcmp(p1, p2, uLen);
}

extern void applib_asnMemSet(void *p1, U32 value, U32 uLen)
{
	memset(p1, value, uLen);
}
