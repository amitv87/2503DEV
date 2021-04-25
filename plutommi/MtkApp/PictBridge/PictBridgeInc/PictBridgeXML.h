/*****************************************************************************
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
 *    PictBridgeXML.h
 *
 * Project:
 * --------
 *    MAUI
 *
 * Description:
 * ------------
 *    This file is intended for PictBridge appliction.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
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
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!! 
 *============================================================================== 
 *******************************************************************************/

#ifndef __PICT_BRIDGE_XML_H__
#define __PICT_BRIDGE_XML_H__
#include "MMI_features.h"
#ifdef __MMI_PICT_BRIDGE_SUPPORT__
#define PICT_XML_ATTR_PAPER_SIZE     "paperSize"


enum PICT_XML_TAG
{
    PICT_XML_TAG_DPS,
    PICT_XML_TAG_INPUT,
    PICT_XML_TAG_OUTPUT,
    PICT_XML_TAG_RESULT,
   
    PICT_XML_TAG_PRINT_CONFIGURE,
    PICT_XML_TAG_DPS_VERSION,
    PICT_XML_TAG_VENDER_NAME,
    PICT_XML_TAG_VENDER_VERSION,
    PICT_XML_TAG_PRODUCT_NAME,
    PICT_XML_TAG_SERIAL_NO,
    PICT_XML_TAG_PRINT_AVAILABLE,
   
    PICT_XML_TAG_GET_CAPABILITY,
    PICT_XML_TAG_CAPABILITY,
    PICT_XML_TAG_QUALITIES,
    PICT_XML_TAG_PAPER_SIZES,
    PICT_XML_TAG_PAPER_TYPES,
    PICT_XML_TAG_FILE_TYPES,
    PICT_XML_TAG_DATE_PRINTS,
    PICT_XML_TAG_FILENAME_PRINTS,
    PICT_XML_TAG_IMAGE_OPTIMIZES,
    PICT_XML_TAG_LAYOUTS,
    PICT_XML_TAG_FIXED_SIZES,
    PICT_XML_TAG_CROPPINGS,

    PICT_XML_TAG_START_JOB,
    PICT_XML_TAG_ABORT_JOB,
    PICT_XML_TAG_CONTINUE_JOB,

    PICT_XML_TAG_NOTIFY_JOB,
    PICT_XML_TAG_PROGRESS,
    PICT_XML_TAG_IMAGE_PRINTED,

    PICT_XML_TAG_NOTIFY_DEVICE,
    PICT_XML_TAG_DPS_STATUS,
    PICT_XML_TAG_END_REASON,
    PICT_XML_TAG_ERROR_STATUS,
    PICT_XML_TAG_ERROR_REASON,
    PICT_XML_TAG_DISCONNECT_ENABLE,
    PICT_XML_TAG_CAPABILITY_CHANGED,
    PICT_XML_TAG_NEW_JOB,

    PICT_XML_TAG_MAX
};


#define PICT_XML_VAL_OK              10000000L
#define PICT_XML_VAL_NOT_EXECUTED    10010000L
#define PICT_XML_VAL_NOT_SUPPORTED   10020000L // with minor code
#define PICT_XML_VAL_NOT_RECOGNIZED  10030000L

#define PICT_XML_VAL_NOT_AVAILABLE   30000000L
#define PICT_XML_VAL_AVAILABLE       30010000L

#define PICT_XML_VAL_DEFAULT_QUALITY 50000000L
#define PICT_XML_VAL_NORMAL_QUALITY  50010000L
#define PICT_XML_VAL_DRAFT_QUALITY   50020000L
#define PICT_XML_VAL_FINE_QUALITY    50030000L

#define PICT_XML_VAL_PAPER_DEFAULT   51000000L
#define PICT_XML_VAL_PAPER_POSTCARD  51030000L
#define PICT_XML_VAL_PAPER_4X6       51060000L
#define PICT_XML_VAL_PAPER_LETTER    51080000L
#define PICT_XML_VAL_PAPER_A4        51140000L
#define PICT_XML_VAL_PAPER_L         51010000L
#define PICT_XML_VAL_PAPER_2L        51020000L

#define PICT_XML_VAL_PAPER_TYPES     52000000L

#define PICT_XML_VAL_TYPE_DEFAULT    53000000L
#define PICT_XML_VAL_TYPE_EXIF_JPG   53010000L
#define PICT_XML_VAL_TYPE_EXIF       53020000L
#define PICT_XML_VAL_TYPE_JPG        53030000L
#define PICT_XML_VAL_TYPE_BMP        53060000L
#define PICT_XML_VAL_TYPE_DPOF       53180000L // reserved

#define PICT_XML_VAL_DATE_DEFAULT    54000000L
#define PICT_XML_VAL_DATE_OFF        54010000L
#define PICT_XML_VAL_DATE_ON         54020000L

#define PICT_XML_VAL_FILE_DEFAULT    55000000L
#define PICT_XML_VAL_FILE_OFF        55010000L
#define PICT_XML_VAL_FILE_ON         55020000L

#define PICT_XML_VAL_OPT_DEFAULT     56000000L
#define PICT_XML_VAL_OPT_OFF         56010000L
#define PICT_XML_VAL_OPT_ON          56020000L

#define PICT_XML_VAL_LAYOUT_DEFAULT  57000000L
#define PICT_XML_VAL_LAYOUT_1UP      57010000L
#define PICT_XML_VAL_LAYOUT_2UP      57020000L
#define PICT_XML_VAL_LAYOUT_4UP      57040000L
#define PICT_XML_VAL_LAYOUT_INDEX    "57FE0000"
#define PICT_XML_VAL_LAYOUT_FULL     "57FF0000"

#define PICT_XML_VAL_CROP_DEFAULT    59000000L
#define PICT_XML_VAL_CROP_OFF        59010000L
#define PICT_XML_VAL_CROP_ON         59020000L

#define PICT_XML_VAL_DPS_PRINTING    70000000L
#define PICT_XML_VAL_DPS_IDLE        70010000L
#define PICT_XML_VAL_DPS_PAUSED      70020000L

#define PICT_XML_VAL_ERROR_NONE      72000000L
#define PICT_XML_VAL_ERROR_WARNING   72010000L
#define PICT_XML_VAL_ERROR_FATAL     72020000L

#define PICT_XML_VAL_ERROR_UNKNOWN   73000000L
#define PICT_XML_VAL_ERROR_PAPER     73010000L
#define PICT_XML_VAL_ERROR_INK       73020000L
#define PICT_XML_VAL_ERROR_HARDWARE  73030000L
#define PICT_XML_VAL_ERROR_FILE      73040000L
#define PICT_XML_VAL_ERROR_NO_PAPER  73010100L
#define PICT_XML_VAL_ERROR_NO_INK    73020100L
#define PICT_XML_VAL_ERROR_NO_DECODE 73040200L

#endif /* __MMI_PICT_BRIDGE_SUPPORT__ */

#endif /* __PICT_BRIDGE_XML_H__ */

