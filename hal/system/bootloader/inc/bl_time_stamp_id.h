/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2009
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
 *   bl_time_stamp_id.h
 *
 * Project:
 * --------
 *   MAUI
 *
 * Description:
 * ------------
 *   Time stamp measurement id (Bootloader Only)
 *
 * Author:
 * -------
 * -------
 *
 ****************************************************************************/
#ifndef __BL_TIME_STAMP_ID_H__
#define __BL_TIME_STAMP_ID_H__

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// TimeStamp Measurement ID
//==============================================================================

typedef enum {
     TSID_LAST_SINGLE_SIG_HASH   = 7
    ,TSID_LAST_SINGLE_SIG_CIPHER = 8

//Level 1, All
    ,TSID_BL_TOTAL
    
//Level 2, Continuous part    
    ,TSID_BL_BOOTLOADER
    ,TSID_BL_EXT_BOOTLOADER
    
//Level 3, Main stages
    ,TSID_BL_SET_PLLEMI
    ,TSID_BL_INIT
    ,TSID_BL_INIT2
    ,TSID_BL_LOAD_EXT_BOOTLOADER
    ,TSID_BL_VERIFY_EXT_BOOTLOADER
    ,TSID_BL_PRINT_WELCOME
    ,TSID_BL_EXT_INIT
    ,TSID_BL_LOAD_ILB_CBR
    ,TSID_BL_LOAD_MAUI_FIRST_PART
    ,TSID_BL_GET_FATURE_COMINATION
    ,TSID_BL_FACTORY_PROCEDURE
    ,TSID_BL_CARD_DOWNLOAD_PROCEDURE
    ,TSID_BL_PREPARE_BOOTUP
    ,TSID_BL_PREPARE_LATCH_POWER
    ,TSID_BL_LATCH_POWER
    ,TSID_BL_CACHE_INIT
    ,TSID_BL_DISPLAY_LOGO    
    ,TSID_BL_BOOT_AND_HANDOVER
    
//Level 4, Separted components
    ,TSID_BL_SET_EMI
    ,TSID_BL_SET_PLL    
    ,TSID_BL_SET_ADVEMI
    ,TSID_BL_INIT_MINI_FLASH_DRV
    ,TSID_BL_LOAD_EXT_BOOTLOADER_BODY
    ,TSID_BL_INIT_EXT_FLASH_DRV
    ,TSID_BL_INIT_CBR
    ,TSID_BL_READ_FLASH_LAYOUT_INFO
    ,TSID_BL_LOAD_MAUI_BODY
    ,TSID_BL_VERIFY_NFB_IMAGE_CONTENT
    ,TSID_BL_MULTIPLE_PAGE_READ
    ,TSID_BL_VERIFY_MAUI
    ,TSID_BL_LOAD_FOTA_IMAGE

//Level 5, Cipher elements
    ,TSID_CALC_EXT_BOOTLOADER_CRC
    ,TSID_SINGLE_SIG_HASH_IN_BL
    ,TSID_SINGLE_SIG_CIPHER_IN_BL
    ,TSID_SINGLE_SIG_HASH_IN_EBL
    ,TSID_SINGLE_SIG_CIPHER_IN_EBL

    ,TSID_NUM


    ,TSID_END = (U8)255    // max 255 records     
    ,TSID_NONE = TSID_END

} TSID;

#ifdef __cplusplus
}
#endif

#endif

