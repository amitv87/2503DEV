#include "MMI_features.h" 
#include "custresdef.h"

#if defined(__MMI_FILE_MANAGER__)

<?xml version="1.0" encoding="UTF-8"?>

<APP id="APP_FILEMANAGER" 
#if defined(__MMI_APP_MANAGER_SUPPORT__)
	package_name = "native.mtk.filemgr"
	name = "STR_FMGR_TITLE"
	#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__) //MM_ICON
	img = "IMG_ID_FMGR_MAIN_MENUICON"
	#endif
	hidden_in_mainmenu = "false"
	launch = "mmi_fmgr_app_mgr_launch"
#else/*__MMI_APP_MANAGER_SUPPORT__*/
  name = "STR_FMGR_TITLE"
#endif
>
    <INCLUDE file="GlobalResDef.h"/>
    <INCLUDE file="RightsMgrResDef.h"/>
    <INCLUDE file="FileMgrServiceResDef.h"/>
    #ifdef __MMI_RMGR__
    <INCLUDE file="mmi_rp_app_rmgr_def.h"/>
    #endif
    <INCLUDE file="gdi_features.h"/>

	#if defined(__MMI_FMGR_MULTI_SELECT_SUPPORT__) && defined(__MMI_FMGR_MULTI_SELECT_SUPPORT_WITH_ASM__)
	<MEMORY heap = "$(MMI_FMGR_APP_MEM_SIZE)+$(MMI_FMGR_MARK_SEVERAL_MEM_SIZE)" inc = "FileMgrGProt.h"/>
	#else
	<MEMORY heap = "$(MMI_FMGR_APP_MEM_SIZE)" inc = "FileMgrGProt.h"/>
	#endif
	
<!-- 
**************************************** Image section ****************************************
-->
#ifdef __MMI_APP_MANAGER_SUPPORT__
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__) //MM_ICON
 	<IMAGE id="IMG_ID_FMGR_MAIN_MENUICON" >CUST_IMG_PATH"\\\\MainLCD\\\\VUI\\\\Launcher\\\\MainMenu\\\\Icons\\\\FMGR.png"</IMAGE>
#endif
#endif /*__MMI_APP_MANAGER_SUPPORT__*/

#ifndef __MMI_SLIM_FILE_MANAGER__
#ifdef __MMI_FTE_SUPPORT__
	<IMAGE id="IMG_ID_FMGR_DRIVE_SPACE_0">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIPNG0.png"</IMAGE>
	<IMAGE id="IMG_ID_FMGR_DRIVE_SPACE_100">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIPNG100.png"</IMAGE>
#else
    <IMAGE id="IMG_ID_FMGR_DRIVE_SPACE_0">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIPNG0.gif"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_DRIVE_SPACE_10">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIPNG10.gif"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_DRIVE_SPACE_20">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIPNG20.gif"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_DRIVE_SPACE_30">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIPNG30.gif"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_DRIVE_SPACE_40">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIPNG40.gif"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_DRIVE_SPACE_50">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIPNG50.gif"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_DRIVE_SPACE_60">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIPNG60.gif"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_DRIVE_SPACE_70">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIPNG70.gif"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_DRIVE_SPACE_80">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIPNG80.gif"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_DRIVE_SPACE_90">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIPNG90.gif"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_DRIVE_SPACE_100">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIPNG100.gif"</IMAGE>
#endif
#endif

#ifdef __FS_CARD_SUPPORT__
#ifdef  __FTE_NON_SLIM_RES__
    <IMAGE id="IMG_ID_FMGR_DRIVE_MEM_CARD">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_card.png"</IMAGE>
#else  
    <IMAGE id="IMG_ID_FMGR_DRIVE_MEM_CARD">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_card.bmp"</IMAGE>
#endif
#endif

    <IMAGE id="IMG_ID_FMGR_DRIVE_PHONE">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_phone.bmp"</IMAGE>
    
#ifdef  __FTE_NON_SLIM_RES__    
    <IMAGE id="IMG_ID_FMGR_ICON_FILE_TYPE_FOLDER">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_SIFOLDER.png"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_ICON_FILE_TYPE_UNKNOWN">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_SIUNKNOWN.png"</IMAGE>
#else
    <IMAGE id="IMG_ID_FMGR_ICON_FILE_TYPE_FOLDER">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_SIFOLDER.bmp"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_ICON_FILE_TYPE_UNKNOWN">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_SIUNKNOWN.bmp"</IMAGE>
#endif
    
    
#ifndef __MMI_ULTRA_SLIM_FILE_MANAGER__
#ifndef GDI_USING_GIF
    <IMAGE id="IMG_ID_FMGR_FILE_TYPE_UNKNOWN">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIUNKNOWN.bmp"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_FILE_TYPE_FOLDER">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIFOLDER.bmp"</IMAGE>
#else
    <IMAGE id="IMG_ID_FMGR_FILE_TYPE_UNKNOWN">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIUNKNOWN.gif"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_FILE_TYPE_FOLDER">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIFOLDER.gif"</IMAGE>
#endif
#endif	/* __MMI_ULTRA_SLIM_FILE_MANAGER__ */
#ifdef __DRM_SUPPORT__
    <IMAGE id="IMG_ID_FMGR_DRM_LOCKED">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_DRM_LOCKED.bmp"</IMAGE>
#endif
#if defined(__DRM_V02__)
    <IMAGE id="IMG_ID_FMGR_ICON_FILE_TYPE_ODF">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_SIDRM.bmp"</IMAGE>
    <IMAGE id="IMG_ID_FMGR_FILE_TYPE_ODF_MULTI_PART">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_LIDRM.gif"</IMAGE>
#endif
#if defined(__FMGR_CSK_FORWARD__)
    <IMAGE id="IMG_ID_FMGR_ICON_FAST_FORWARD_CSK">CUST_IMG_PATH"\\\\MainLCD\\\\UIElement\\\\Softkey\\\\forward.bmp"</IMAGE>
#endif
#if !defined(__MMI_SLIM_FILE_MANAGER__)
#ifdef __FS_CARD_SUPPORT__
    <IMAGE id="IMG_ID_FMGR_TAB_CARD">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_tab_card.bmp"</IMAGE>
#endif
    <IMAGE id="IMG_ID_FMGR_TAB_PHONE">CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_tab_phone.bmp"</IMAGE>
#endif

<!-- 
**************************************** String section ***************************************
-->

    <STRING id="STR_ID_FMGR_NO_FORWARD_OPTION"/>
    //<STRING id="STR_ID_FMGR_NOT_AVAILABLE"/>

    <STRING id="STR_FMGR_TITLE"/>
    <STRING id="STR_FMGR_FOLDER_CREATE"/>
#ifndef __MMI_FMGR_HIDE_FORMAT_DRIVE__
    <STRING id="STR_FMGR_FORMAT"/>
#endif
    <STRING id="STR_FMGR_FORMATING"/>
#if (!defined(APP_STORAGE_IN_SYS_DRV)) && defined(__MMI_EMAIL__)
    <STRING id="STR_FMGR_FORMAT_CONFIRM"/>
#endif
#if defined (__MMI_VECTOR_FONT_MEMORY_CARD_SUPPORT__) || \
                    defined (__MMI_FE_VECTOR_FONT_ON_FILE_SYSTEM) || \
                    defined (__MMI_IME_EMMORY_CARD_SUPPORT__) || \
                    defined (__MMI_HANDWRITING_MEMORY_CARD_SUPPORT__)
    <STRING id="STR_ID_FMGR_RESTART_CONTINUE"/>
#endif
    <STRING id="STR_FMGR_UNMOUNTING"/>
#if defined (__FS_CARD_SUPPORT__)
    <STRING id="STR_FMGR_CARD_NAME"/>
    <STRING id="STR_ID_FMGR_TOTAL_SIZE"/>
    <STRING id="STR_ID_FMGR_FREE_SIZE"/>
#endif
    <STRING id="STR_FMGR_USE"/>
#ifndef __MMI_SLIM_FILE_MANAGER__
    <STRING id="STR_FMGR_DELETE_ALL"/>
#endif
    <STRING id="STR_FMGR_COPY_TO"/>
    <STRING id="STR_FMGR_MOVE_TO"/>
#ifdef __MMI_FMGR_MULTI_SELECT_SUPPORT__
    <STRING id="STR_FMGR_COPIED"/>
    <STRING id="STR_FMGR_MOVED"/>
    <STRING id="STR_FMGR_MARK_SEVERAL"/>
    <STRING id="STR_FMGR_MARK_DELETE"/>
    <STRING id="STR_FMGR_MARK_COPY"/>
    <STRING id="STR_FMGR_MARK_MOVE"/>
    <STRING id="STR_FMGR_MARK_PRINT"/>
    <STRING id="STR_ID_FMGR_MARK_TO_BT"/>
    <STRING id="STR_FMGR_MARK_MAX_FILE"/>
    <STRING id="STR_FMGR_MARK_SEVERAL_PRESS_STAR"/>
    <STRING id="STR_FMGR_MARK_SEVERAL_TOUCH_BOX"/>
    <STRING id="STR_ID_FMGR_MARK_ALL_PROTECTED"/>
    <STRING id="STR_ID_FMGR_MARK_PARTIAL_PROTECTED_CONFIRM"/>
    <STRING id="STR_ID_FMGR_MARK_PARTIAL_FOLDERS_CONFIRM"/>
    <STRING id="STR_ID_FMGR_MARK_ALL_CAN_NOT_SEND"/>
    <STRING id="STR_ID_FMGR_MARK_ALL_CAN_NOT_SEND_CONFIRM"/>    
#endif
#ifdef __FS_SORT_SUPPORT__
    <STRING id="STR_FMGR_SORT_BY_NAME"/>
    <STRING id="STR_FMGR_SORT_BY_TYPE"/>
    <STRING id="STR_FMGR_SORT_BY_TIME"/>
    <STRING id="STR_FMGR_SORT_BY_SIZE"/>
#endif
#ifndef __MMI_SLIM_FILE_MANAGER__
    <STRING id="STR_ID_FMGR_DRV_FREE_SPACE"/>
    <STRING id="STR_ID_FMGR_DRV_TOTAL_SPACE"/>
#endif
    <STRING id="STR_ID_FMGR_FILE_DATE"/>
    <STRING id="STR_ID_FMGR_FILE_SIZE"/>
    <STRING id="STR_ID_FMGR_SYSTEM_BUSY_TO_DECODE"/>
#if (!defined __MMI_FMGR_HIDE_FORMAT_DRIVE__) || (!defined __MMI_SLIM_FILE_MANAGER__)
    <STRING id="STR_ID_FMGR_QUESTION_MARK"/>
#endif
#ifndef __FS_CARD_SUPPORT__
    <STRING id="STR_ID_FMGR_NO_AVAILABLE_STORAGE"/>
#endif
    <STRING id="STR_ID_FMGR_NEW_FOLDER_LSK"/>
    <STRING id="STR_ID_FMGR_FOLDER_NAME"/>
#if defined (__FS_CARD_SUPPORT__)
#ifndef __MMI_SLIM_FILE_MANAGER__
    <STRING id="STR_ID_FMGR_CARD_NAME"/>
#endif
#endif
    <STRING id="STR_ID_FMGR_NO_FILES"/>
    <STRING id="STR_ID_FMGR_NO_FOLDERS"/>
<!-- 
**************************************** Menu section *****************************************
-->

    <MENUSET id="MENU_ID_FMGR_GENERAL_OPTION">
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_FORWARD_SEND</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_FORWARD_USE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_FOLDER_CREATE</MENUITEM_ID>
    #ifdef __MMI_RMGR__
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MORE_RITS</MENUITEM_ID>
    #endif
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_RENAME</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_DELETE</MENUITEM_ID>
    #ifdef __FS_SORT_SUPPORT__
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_SORT</MENUITEM_ID>
    #endif
    #ifdef __MMI_FMGR_MULTI_SELECT_SUPPORT__
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_SEVERAL</MENUITEM_ID>
    #endif
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_COPY</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MOVE</MENUITEM_ID>
    #ifndef __MMI_SLIM_FILE_MANAGER__
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_DELETE_ALL</MENUITEM_ID>
    #endif
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_DETAIL</MENUITEM_ID>
    </MENUSET>
    <MENUSET id="MENU_ID_FMGR_GEN_SEND_OPTION_SET">
        #ifdef __MMI_MMS__
        <MENUITEM_ID>@OID:MENU_ID_FMGR_FWD_MMS</MENUITEM_ID>
        #endif
        #ifdef __MMI_EMAIL__    
        <MENUITEM_ID>@OID:MENU_ID_FMGR_FWD_EMAIL</MENUITEM_ID>
        #endif
        #ifdef __MMI_OPP_SUPPORT__
        <MENUITEM_ID>@OID:MENU_ID_FMGR_FWD_BT</MENUITEM_ID>
        #endif
    </MENUSET>
    <MENU id="MENU_ID_FMGR_DRIVE_OPTIONS" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUITEM_ID>MENU_ID_FMGR_DRIVE_OPEN</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_FOLDER_CREATE</MENUITEM_ID>
    #ifndef __MMI_FMGR_HIDE_FORMAT_DRIVE__
        <MENUITEM_ID>MENU_ID_FMGR_DRIVE_FORMAT</MENUITEM_ID>
    #endif
    #ifndef __MMI_SLIM_FILE_MANAGER__
        <MENUITEM_ID>MENU_ID_FMGR_DRIVE_UNMOUNT</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_DRIVE_RENAME</MENUITEM_ID>
    #endif
        <MENUITEM_ID>MENU_ID_FMGR_CARD_DETAIL</MENUITEM_ID>
        </MENU>
    <MENU id="MENU_ID_FMGR_APP_DRIVE_OPTIONS" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUITEM_ID>MENU_ID_FMGR_DRIVE_OPEN</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_DRIVE_SELECT_DONE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_FOLDER_CREATE</MENUITEM_ID>
    </MENU>
    <MENUITEM id="MENU_ID_FMGR_DRIVE_OPEN" str="STR_GLOBAL_OPEN"/>
#ifndef __MMI_FMGR_HIDE_FORMAT_DRIVE__
    <MENUITEM id="MENU_ID_FMGR_DRIVE_FORMAT" str="STR_FMGR_FORMAT"/>
#endif
#ifndef __MMI_SLIM_FILE_MANAGER__
    <MENUITEM id="MENU_ID_FMGR_DRIVE_UNMOUNT" str="STR_GLOBAL_REMOVE"/>
#endif
    <MENUITEM id="MENU_ID_FMGR_DRIVE_SELECT_DONE" str="STR_GLOBAL_SELECT"/>
#if defined (__FS_CARD_SUPPORT__)
#ifndef __MMI_SLIM_FILE_MANAGER__
    <MENUITEM id="MENU_ID_FMGR_DRIVE_RENAME" str="STR_GLOBAL_RENAME"/>
#endif
    <MENUITEM id="MENU_ID_FMGR_CARD_DETAIL" str="STR_GLOBAL_DETAILS"/>
#endif
    <MENU id="MENU_ID_FMGR_FOLDER_OPTIONS" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUITEM_ID>MENU_ID_FMGR_FOLDER_OPEN</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_FOLDER_CREATE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_RENAME</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_DELETE</MENUITEM_ID>
    #ifdef __FS_SORT_SUPPORT__
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_SORT</MENUITEM_ID>
    #endif
    #ifdef __MMI_FMGR_MULTI_SELECT_SUPPORT__
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_SEVERAL</MENUITEM_ID>       
    #endif
	#ifdef __MMI_FMGR_FOLDER_COPY_SUPPORT__
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_COPY</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MOVE</MENUITEM_ID>
    #endif
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_DETAIL</MENUITEM_ID>
    </MENU>
    <MENU id="MENU_ID_FMGR_APP_FOLDER_OPTIONS" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUITEM_ID>MENU_ID_FMGR_FOLDER_OPEN</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_FOLDER_SELECT_DONE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_FOLDER_CREATE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_RENAME</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_DELETE</MENUITEM_ID>
    </MENU>
    <MENUITEM id="MENU_ID_FMGR_FOLDER_OPEN" str="STR_GLOBAL_OPEN"/>
    <MENUITEM id="MENU_ID_FMGR_FOLDER_CREATE" str="STR_FMGR_FOLDER_CREATE"/>
    <MENUITEM id="MENU_ID_FMGR_FOLDER_SELECT_DONE" str="STR_GLOBAL_SELECT"/>
    <MENU id="MENU_ID_FMGR_HYPERLINK_OPTIONS" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUITEM_ID>MENU_ID_FMGR_HYPERLINK_APP_FUNC</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_FOLDER_CREATE</MENUITEM_ID>
    </MENU>
    <MENUITEM id="MENU_ID_FMGR_HYPERLINK_APP_FUNC" str="STR_GLOBAL_ERROR"/>
    <MENU id="MENU_ID_FMGR_TREE_OPTIONS" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUITEM_ID>MENU_ID_FMGR_TREE_SELECT</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_TREE_OPEN</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_FOLDER_CREATE</MENUITEM_ID>
    </MENU>
    <MENUITEM id="MENU_ID_FMGR_TREE_SELECT" str="STR_GLOBAL_DONE"/>
    <MENUITEM id="MENU_ID_FMGR_TREE_OPEN" str="STR_GLOBAL_OPEN"/>
#if defined(__DRM_V02__)
    <MENU id="MENU_ID_FMGR_DRM_ODF_TYPE_OPTIONS" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUITEM_ID>MENU_ID_FMGR_DRM_OPTION_MUL_ODF_OPEN</MENUITEM_ID>
        <MENUSET_ID>MENU_ID_FMGR_GENERAL_OPTION</MENUSET_ID>
    </MENU>
    <MENUITEM id="MENU_ID_FMGR_DRM_OPTION_MUL_ODF_OPEN" str="STR_GLOBAL_OPEN"/>
#endif
    <MENU id="MENU_ID_FMGR_GEN_TYPE_OPTIONS" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUSET_ID>MENU_ID_FMGR_GENERAL_OPTION</MENUSET_ID>
    </MENU>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_FORWARD_USE" str="STR_FMGR_USE"/>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_FORWARD_SEND" str="STR_GLOBAL_SEND"/>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_DETAIL" str="STR_GLOBAL_DETAILS"/>
#ifdef __MMI_RMGR__
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_MORE_RITS" str="STR_ID_RMGR_MORE_RITS"/>
#endif /* __MMI_RMGR__ */
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_RENAME" str="STR_GLOBAL_RENAME"/>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_COPY" str="STR_GLOBAL_COPY"/>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_MOVE" str="STR_GLOBAL_MOVE"/>

#ifdef __MMI_FMGR_MULTI_SELECT_SUPPORT__
    <MENU id="MENU_ID_FMGR_GEN_OPTION_MARK_SEVERAL" str="STR_FMGR_MARK_SEVERAL">
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_DELETE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_COPY</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_MOVE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_APP_COPY</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_APP_MOVE</MENUITEM_ID>
    #if defined(__MMI_PICT_BRIDGE_SUPPORT__) && !defined(__MMI_PICT_BRIDGE_MULTI_FILE_PRINT__)
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_PRINT</MENUITEM_ID>
    #endif
#ifdef __MMI_OPP_SUPPORT__        
        <MENUITEM_ID>MENU_ID_FMGR_MARK_OPTION_TO_BT</MENUITEM_ID>
#endif        
    </MENU>
    <MENU id="MENU_ID_FMGR_GEN_OPTION_MARK_OPTION" type="OPTION" str="STR_GLOBAL_OPTIONS">
    	<MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_DONE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_MARK</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_UNMARK</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_MARK_ALL</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_UNMARK_ALL</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_EXIT</MENUITEM_ID>
        </MENU>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_MARK_DELETE" str="STR_FMGR_MARK_DELETE"/>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_MARK_COPY" str="STR_FMGR_MARK_COPY"/>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_MARK_MOVE" str="STR_FMGR_MARK_MOVE"/>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_MARK_APP_COPY" str="STR_FMGR_MARK_COPY"/>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_MARK_APP_MOVE" str="STR_FMGR_MARK_MOVE"/>
#if defined(__MMI_PICT_BRIDGE_SUPPORT__) && !defined(__MMI_PICT_BRIDGE_MULTI_FILE_PRINT__)
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_MARK_PRINT" str="STR_FMGR_MARK_PRINT"/>
#endif
#ifdef __MMI_OPP_SUPPORT__
    <MENUITEM id="MENU_ID_FMGR_MARK_OPTION_TO_BT" str="STR_ID_FMGR_MARK_TO_BT"/>
#endif
	<MENUITEM id = "MENU_ID_FMGR_GEN_OPTION_MARK_DONE" str = "STR_GLOBAL_DONE"/>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_MARK_MARK" str="STR_GLOBAL_MARK"/>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_MARK_UNMARK" str="STR_GLOBAL_UNMARK"/>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_MARK_MARK_ALL" str="STR_GLOBAL_MARK_ALL"/>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_MARK_UNMARK_ALL" str="STR_GLOBAL_UNMARK_ALL"/>
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_MARK_EXIT" str="STR_GLOBAL_EXIT"/>
#endif

    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_DELETE" str="STR_GLOBAL_DELETE"/>
#ifndef __MMI_SLIM_FILE_MANAGER__
    <MENUITEM id="MENU_ID_FMGR_GEN_OPTION_DELETE_ALL" str="STR_FMGR_DELETE_ALL"/>
#endif
#ifdef __FS_SORT_SUPPORT__
    <MENU id="MENU_ID_FMGR_GEN_OPTION_SORT" type="OPTION" str="STR_GLOBAL_SORT_BY" hint="mmi_fmgri_main_hint_sort">
        <MENUITEM_ID>MENU_ID_FMGR_SORT_BY_NAME</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_SORT_BY_TYPE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_SORT_BY_TIME</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_SORT_BY_SIZE</MENUITEM_ID>
        </MENU>
    <MENUITEM id="MENU_ID_FMGR_SORT_BY_NAME" str="STR_FMGR_SORT_BY_NAME"/>
    <MENUITEM id="MENU_ID_FMGR_SORT_BY_TYPE" str="STR_FMGR_SORT_BY_TYPE"/>
    <MENUITEM id="MENU_ID_FMGR_SORT_BY_TIME" str="STR_FMGR_SORT_BY_TIME"/>
    <MENUITEM id="MENU_ID_FMGR_SORT_BY_SIZE" str="STR_FMGR_SORT_BY_SIZE"/>
#endif
    <MENU id="MENU_ID_FMGR_EDITOR_OPTION" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUITEM_ID>MENU_ID_FMGR_EDITOR_DONE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_EDITOR_INPUT_METHOD</MENUITEM_ID>
        </MENU>
    <MENUITEM id="MENU_ID_FMGR_EDITOR_DONE" str="STR_GLOBAL_DONE"/>
    <MENUITEM id="MENU_ID_FMGR_EDITOR_INPUT_METHOD" str="STR_GLOBAL_INPUT_METHOD"/>

    <RECEIVER id="EVT_ID_IDLE_GROUP_ENTER" proc="mmi_fmgr_nor_sweep_device"/>
	<RECEIVER id="EVT_ID_SRV_FMGR_NOTIFICATION_DEV_PLUG_IN" proc="mmi_fmgr_notify_hdlr"/>
	<RECEIVER id="EVT_ID_SRV_FMGR_NOTIFICATION_DEV_PLUG_OUT" proc="mmi_fmgr_notify_hdlr"/>
	<RECEIVER id="EVT_ID_SRV_FMGR_NOTIFICATION_DEV_SOFT_MOUNT" proc="mmi_fmgr_notify_hdlr"/>
	<RECEIVER id="EVT_ID_SRV_FMGR_NOTIFICATION_ADV_ACTION" proc="mmi_fmgr_notify_hdlr"/>
	#ifdef __DM_LAWMO_SUPPORT__
	<RECEIVER id = "EVT_ID_SRV_DM_LAWMO_WIPE_EX_IND" proc = "fmgr_dm_lawmo_hdlr"/>
	#endif/*__DM_LAWMO_SUPPORT__*/
	
<!-- 
**************************************** Timer section ***************************************
-->
    <TIMER id="FMGR_AUDIO_TIMER"/>
    <TIMER id="FMGR_SCR_TIMER"/>
</APP>


<FILE_TYPES>
<!-- 
**************************************** Default Type *****************************************
-->
#if !defined(__COSMOS_MMI__)
    <FILE_GROUP_TYPE type_id="FMGR_GROUP_COMMON">
    #ifndef __MMI_ULTRA_SLIM_FILE_MANAGER__
        <FILE_TYPE type_id="FMGR_TYPE_UNKNOWN" small_icon="IMG_ID_FMGR_ICON_FILE_TYPE_UNKNOWN" large_icon="IMG_ID_FMGR_FILE_TYPE_UNKNOWN" option="MENU_ID_FMGR_GEN_TYPE_OPTIONS" send_option="MENU_ID_FMGR_GEN_SEND_OPTION"/>
        <FILE_TYPE type_id="FMGR_TYPE_FOLDER" small_icon="IMG_ID_FMGR_ICON_FILE_TYPE_FOLDER" large_icon="IMG_ID_FMGR_FILE_TYPE_FOLDER" option="MENU_ID_FMGR_FOLDER_OPTIONS"/>
    #else
        <FILE_TYPE type_id="FMGR_TYPE_UNKNOWN" small_icon="IMG_ID_FMGR_ICON_FILE_TYPE_UNKNOWN" option="MENU_ID_FMGR_GEN_TYPE_OPTIONS" send_option="MENU_ID_FMGR_GEN_SEND_OPTION"/>
        <FILE_TYPE type_id="FMGR_TYPE_FOLDER" small_icon="IMG_ID_FMGR_ICON_FILE_TYPE_FOLDER" option="MENU_ID_FMGR_FOLDER_OPTIONS"/>
    #endif
        #if defined(__DRM_V02__)
        <FILE_TYPE type_id="FMGR_TYPE_ODF" extension="ODF" small_icon="IMG_ID_FMGR_ICON_FILE_TYPE_ODF" large_icon="IMG_ID_FMGR_FILE_TYPE_ODF_MULTI_PART" option="MENU_ID_FMGR_DRM_ODF_TYPE_OPTIONS"/>
        #endif
        <FILE_TYPE type_id="FMGR_TYPE_HYPER_LINK"/>
    </FILE_GROUP_TYPE>

    <FILE_OPTION_HDLR option="MENU_ID_FMGR_FOLDER_OPTIONS" enabler="mmi_fmgr_general_option_enabler"/>
#endif    
    <FILE_OPTION_HDLR option="MENU_ID_FMGR_GEN_OPTION_FORWARD_SEND" enabler="mmi_fmgr_general_option_enabler"/>
    <FILE_OPTION_HDLR option="MENU_ID_FMGR_GEN_OPTION_FORWARD_USE" enabler="mmi_fmgr_general_option_enabler"/>

#ifdef __MMI_FMGR_MULTI_SELECT_SUPPORT__
    <FILE_OPTION_HDLR option = "MENU_ID_FMGR_GEN_OPTION_MARK_SEVERAL" enabler = "mmi_fmgr_general_option_mul_sel_enabler"/>

    <FILE_OPTION_HDLR option = "MENU_ID_FMGR_GEN_OPTION_MARK_COPY" handler = "mmi_fmgr_general_option_mul_sel_handler"/>
    <FILE_OPTION_HDLR option = "MENU_ID_FMGR_GEN_OPTION_MARK_MOVE" handler = "mmi_fmgr_general_option_mul_sel_handler"/>
    <FILE_OPTION_HDLR option = "MENU_ID_FMGR_GEN_OPTION_MARK_DELETE" handler = "mmi_fmgr_general_option_mul_sel_handler"/>
#if defined(__MMI_PICT_BRIDGE_SUPPORT__) && !defined(__MMI_PICT_BRIDGE_MULTI_FILE_PRINT__)
    <FILE_OPTION_HDLR option = "MENU_ID_FMGR_GEN_OPTION_MARK_PRINT" handler = "mmi_fmgr_general_option_mul_sel_handler"/>
#endif
#ifdef __MMI_OPP_SUPPORT__
	<FILE_OPTION_HDLR option = "MENU_ID_FMGR_MARK_OPTION_TO_BT" handler = "mmi_fmgr_general_option_mul_sel_handler"/>
#endif
#endif

#ifdef __MMI_RMGR__
    <FILE_OPTION_HDLR option="MENU_ID_FMGR_GEN_OPTION_MORE_RITS" enabler="mmi_fmgr_general_option_enabler"/>
#endif
    <FILE_OPTION_HDLR option="MENU_ID_FMGR_AUD_TYPE_OPTIONS" enabler="mmi_fmgr_option_audio_enabler_ex"/>
    <FILE_OPTION_HDLR option="MENU_ID_FMGR_IMG_TYPE_OPTIONS" enabler="mmi_fmgr_option_image_enabler_ex"/>    
</FILE_TYPES>

#endif