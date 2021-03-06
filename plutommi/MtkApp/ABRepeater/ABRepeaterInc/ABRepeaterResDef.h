/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
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

/*******************************************************************************
 * Filename:
 * ---------
 *  ABRepeaterResDef.h
 *
 * Project:
 * --------
 *   MAUI
 *
 * Description:
 * ------------
 *  Resource definitions.
 *
 *  Author:
 * -------
 *  
 *
 *==============================================================================
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
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!! 
 *==============================================================================
 *******************************************************************************/

#ifndef AB_REPEATER_RES_DEF_H
#define AB_REPEATER_RES_DEF_H


#include "MMI_features.h"   /* add if you use any compile options defined in MMI_features.h */

#ifdef __MMI_AB_REPEATER__

#include "MMIDataType.h"    /* for resource base */

/***************************************************************************** 
 * Typedef for Resource Enum
 *****************************************************************************/
/* Screen IDs */
typedef enum
{
    SCR_ID_ABREPEAT_BASE = AB_REPEATER_BASE + 1,
    SCR_ID_AB_LIST,
    SCR_ID_AB_BUILD_CACHE,
    SCR_ID_AB_MAIN,
    SCR_ID_ABREPEAT_OPTION,
    SCR_ID_ABREPEAT_STORAGE_SETTINGS,

    SCR_ID_ABREPEAT_FMGR_LIST,
    SCR_ID_ABREPEAT_SELECT_STORAGE,
    NO_OF_SCR_ABREPEAT
} abrepeat_scr_id_enum;


/* String IDs */
typedef enum
{
    STR_ID_ABREPEAT_BASE = AB_REPEATER_BASE + 1,
    STR_ID_ABREPEAT_TITLE,
    //STR_ID_ABENTER_TITLE,
    STR_ID_ABREPEAT_BUILDING_CACHE,
    //STR_ID_ABREPEAT_STORAGE,//
    STR_ID_ABREPEAT_SWITCH,
    STR_ID_ABREPEAT_CANNOT_PAUSE,
    STR_ID_ABREPEAT_COMMAND_ERROR,
    STR_ID_ABREPEAT_DEVICE_BUSY,
    STR_ID_ABREPEAT_DISC_FULL,
    STR_ID_ABREPEAT_OPEN_FILE_ERROR,
    STR_ID_ABREPEAT_BAD_FORMAT,
    STR_ID_ABREPEAT_INVALID_FORMAT,
    STR_ID_ABREPEAT_UNSUPPORTED_CHANNEL,
    STR_ID_ABREPEAT_UNSUPPORTED_FREQ,
    STR_ID_ABREPEAT_UNSUPPORTED_TYPE,
    STR_ID_ABREPEAT_ERROR,
    STR_ID_ABREPEAT_NO_SPACE,
   // STR_ID_ABREPEAT_STORAGE_PHONE,//
   // STR_ID_ABREPEAT_STORAGE_CARD,//
   // STR_ID_ABREPEAT_NO_CARD,//
    STR_ID_ABREPEAT_BUILD_CACHE_FAILED,
    STR_ID_ABREPEAT_NOTIFY_STORAGE_NOT_READY,
    NO_OF_STR_ID_ABREPEAT
} abrepeat_str_id_enum;


/* Image IDs */
typedef enum
{
    IMG_ID_ABREPEAT_BASE = AB_REPEATER_BASE + 1,
    IMG_ID_ABREPEAT_MAIN_SKIN1_BG,
    IMG_ID_ABREPEAT_PROGRESS_BAR,
    IMG_ID_ABREPEAT_INDICATOR,
    IMG_ID_ABREPEAT_BUTTON0,
    IMG_ID_ABREPEAT_BUTTON1,
    IMG_ID_ABREPEAT_BUTTON2,
    IMG_ID_ABREPEAT_BUTTON3,
    IMG_ID_ABREPEAT_BUTTON4,
    IMG_ID_ABREPEAT_BUTTON5,
    IMG_ID_ABREPEAT_BUTTON6,
    IMG_ID_ABREPEAT_BUTTON7,
    IMG_ID_ABREPEAT_BUTTON8,
    IMG_ID_ABREPEAT_BUTTON9,
    IMG_ID_ABREPEAT_ABSECTION_PLAYING,
    IMG_ID_ABREPEAT_ABSECTION_RECORDING,
    IMG_ID_ABREPEAT_ABSECTION_RECORDING_DOWN,
    IMG_ID_ABREPEAT_AB_SETSECTION,
    IMG_ID_ABREPEAT_AB_SETSECTION_DOWN,
    IMG_ID_ABREPEAT_AB_NONSETSECTION,
    IMG_ID_ABREPEAT_AB_NONSETSECTION_DOWN,
    IMG_ID_ABREPEAT_VOLUME,

    IMG_ID_ABREPEAT_VOLUME_DEC_UP,
    IMG_ID_ABREPEAT_VOLUME_DEC_DOWN,
    IMG_ID_ABREPEAT_VOLUME_DEC_DISABLE,

    IMG_ID_ABREPEAT_VOLUME_INC_UP,
    IMG_ID_ABREPEAT_VOLUME_INC_DOWN,
    IMG_ID_ABREPEAT_VOLUME_INC_DISABLE,

    IMG_ID_ABREPEAT_STOP_DOWN,
    IMG_ID_ABREPEAT_STOP_UP,
    IMG_ID_ABREPEAT_PROG_BAR_EMPTY,
    IMG_ID_ABREPEAT_PROG_BAR_FILL,
    IMG_ID_ABREPEAT_FWD_DOWN,
    IMG_ID_ABREPEAT_FWD_UP,
    IMG_ID_ABREPEAT_REW_DOWN,
    IMG_ID_ABREPEAT_REW_UP,
    IMG_ID_ABREPEAT_PAUSE_DOWN,
    IMG_ID_ABREPEAT_PAUSE_UP,
    IMG_ID_ABREPEAT_PLAY_DOWN,
    IMG_ID_ABREPEAT_PLAY_UP,
    IMG_ID_ABREPEAT_REPEAT_DOWN,
    IMG_ID_ABREPEAT_REPEAT_UP,
    IMG_ID_ABREPEAT_REPEAT_OFF_DOWN,
    IMG_ID_ABREPEAT_REPEAT_OFF_UP,
    IMG_ID_ABREPEAT_REPEAT_DISABLE,
    
    IMG_ID_ABREPEAT_SETTING_MODE_DOWN,
    IMG_ID_ABREPEAT_SETTING_MODE_UP,
    IMG_ID_ABREPEAT_PLAY_MODE_DOWN,
    IMG_ID_ABREPEAT_PLAY_MODE_UP,

    IMG_ID_ABREPEAT_SUBMENU_ICON,
    IMG_ID_ABREPEAT_STORAGE,

    NO_OF_AB_REPEATER_ICON
} abrepeat_img_id_enum;


#endif  /* __MMI_AB_REPEATER__ */

#endif /* AB_REPEATER_RES_DEF_H */ 
