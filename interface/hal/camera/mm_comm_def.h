/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2010
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
*******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *	.h
 *
 * Project:
 * --------
 *   MT6238
 *
 * Description:
 * ------------
 *   This file is intends for ISP HW.
 *
 * Author:
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
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
#ifndef __MM_COMM_DEF_H__
#define __MM_COMM_DEF_H__

#include "jpeg_enum.h"

typedef enum
{
    MM_ERROR_NONE=0,
    MM_ERROR_BEGIN=1,
    MM_ERROR_INVALID_PARAMETER,
    MM_ERROR_OUT_OF_MEMORY,
    MM_ERROR_MODIS_FAIL,
    MM_ERROR_ISP_BEGIN=1000,
    MM_ERROR_ISP_NO_VSYNC,
    MM_ERROR_MDP_BEGIN=2000,
    MM_ERROR_MDP_BUFFER_NO_SYNC,
    MM_ERROR_LCD_BEGIN=3000,
    MM_ERROR_JPEG_BEGIN=4000,
    MM_ERROR_CAL_BEGIN=5000,
    MM_ERROR_CAL_INVALID_CTRL_TYPE,
    MM_ERROR_CAL_INVALID_STATE,
    MM_ERROR_CAL_INVALID_CAMERA_SOURCE_SELECT,
    MM_ERROR_CAL_STILL_CAPTURE_TIMEOUT,
    MM_ERROR_CAL_STILL_CATPURE_OUT_OF_MEMORY,
    MM_ERROR_SENSOR_BEGIN=6000,
    MM_ERROR_SENSOR_FAIL,
    MM_ERROR_SENSOR_READ_ID_FAIL=0xFFFF,
    MM_ERROR_SENSOR_FEATURE_NOT_SUPPORT=MM_ERROR_SENSOR_FAIL+1,
    MM_ERROR_SENSOR_FRAME_RATE_LUT_NOT_SUPPORT,
    MM_ERROR_SENSOR_FRAME_RATE_LUT_NOT_MATCH,
    MM_ERROR_LENS_BEGIN=7000,
    MM_ERROR_FLASHLIGHT_BEGIN=8000,
    MM_ERROR_PPI_BEGIN=9000,
    MM_ERROR_PPI_INVALID_PARAMETER,  //New add
    MM_ERROR_PPI_INVALID_STATE, //New add
    MM_ERROR_PPI_BUFFER_NO_SYNC, //New add
    MM_ERROR_MAX
} MM_ERROR_CODE_ENUM;

typedef enum
{
    MM_IMAGE_FORMAT_RGB565=1,
    MM_IMAGE_FORMAT_BGR565,
    MM_IMAGE_FORMAT_RGB888,
    MM_IMAGE_FORMAT_BGR888,
    MM_IMAGE_FORMAT_ARGB888,
    MM_IMAGE_FORMAT_ABGR888,
    MM_IMAGE_FORMAT_BGRA8888,                                
    MM_IMAGE_FORMAT_RGBA8888,                              
    MM_IMAGE_FORMAT_YUV444,
    MM_IMAGE_FORMAT_YUV422,
    MM_IMAGE_FORMAT_YUV420,
    MM_IMAGE_FORMAT_YUV411,
    MM_IMAGE_FORMAT_YUV400,
    MM_IMAGE_FORMAT_PACKET_UYVY422,
    MM_IMAGE_FORMAT_PACKET_YUY2,
    MM_IMAGE_FORMAT_PACKET_YVYU,

    MM_IMAGE_FORMAT_RAW8=100,
    MM_IMAGE_FORMAT_RAW10,
    MM_IMAGE_FORMAT_EXT_RAW8,
    MM_IMAGE_FORMAT_EXT_RAW10,
    MM_IMAGE_FORMAT_JPEG=200
} MM_IMAGE_FORMAT_ENUM;

typedef enum
{
    MM_IMAGE_COLOR_FORMAT_RGB_GROUP_BEGIN = MM_IMAGE_FORMAT_RGB565,
    MM_IMAGE_COLOR_FORMAT_RGB565 = MM_IMAGE_COLOR_FORMAT_RGB_GROUP_BEGIN,
    MM_IMAGE_COLOR_FORMAT_BGR565,
    MM_IMAGE_COLOR_FORMAT_RGB888,
    MM_IMAGE_COLOR_FORMAT_BGR888,
    MM_IMAGE_COLOR_FORMAT_ARGB8888,
    MM_IMAGE_COLOR_FORMAT_ABGR8888,
    MM_IMAGE_COLOR_FORMAT_BGRA8888,                                
    MM_IMAGE_COLOR_FORMAT_RGBA8888,                              
    MM_IMAGE_COLOR_FORMAT_RGB_GROUP_END   = MM_IMAGE_COLOR_FORMAT_RGBA8888,
    MM_IMAGE_COLOR_FORMAT_YUV_GROUP_BEGIN = MM_IMAGE_FORMAT_YUV444,
    MM_IMAGE_COLOR_FORMAT_YUV444 = MM_IMAGE_COLOR_FORMAT_YUV_GROUP_BEGIN,
    MM_IMAGE_COLOR_FORMAT_YUV422,
    MM_IMAGE_COLOR_FORMAT_YUV420,
    MM_IMAGE_COLOR_FORMAT_YUV411,
    MM_IMAGE_COLOR_FORMAT_YUV400,
    MM_IMAGE_COLOR_FORMAT_PACKET_UYVY422,
    MM_IMAGE_COLOR_FORMAT_PACKET_YUY2,
    MM_IMAGE_COLOR_FORMAT_PACKET_YVYU,
    MM_IMAGE_COLOR_FORMAT_YUV_GROUP_END = MM_IMAGE_COLOR_FORMAT_PACKET_YVYU
} MM_IMAGE_COLOR_FORMAT_ENUM;

typedef enum
{
    MM_IMAGE_ROTATE_0 = 0,      /**< clock-wise   0 degree. */
    MM_IMAGE_ROTATE_90,         /**< clock-wise  90 degree. */
    MM_IMAGE_ROTATE_180,        /**< clock-wise 180 degree. */
    MM_IMAGE_ROTATE_270,        /**< clock-wise 270 degree. */
    MM_IMAGE_H_MIRROR,
    MM_IMAGE_H_MIRROR_ROTATE_90,  /**< clock-wise */
    MM_IMAGE_H_MIRROR_ROTATE_180, /**< clock-wise */
    MM_IMAGE_H_MIRROR_ROTATE_270, /**< clock-wise */
    MM_IMAGE_ROTATE_0_WITH_LCD_CONFIG, /**< lock-wise   0 degree include LCD config */
    MM_NO_OF_IMAGE_ROTATE = MM_IMAGE_ROTATE_0_WITH_LCD_CONFIG
} MM_IMAGE_ROTATE_ENUM;

typedef enum
{
    JPG_QTY_LOW = 0,
    JPG_QTY_NORMAL,
    JPG_QTY_HIGH,
    JPG_QTY_FINE,
    NO_OF_JPG_QTY
} JPG_QUALITY_ENUM;

#endif /* __MM_COMM_DEF_H__ */
