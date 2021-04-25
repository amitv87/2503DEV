::����ָ�� chengj 20170413
::mk [project] or mk

@echo off
cls

:START
if not "1%~1%"=="1" goto check1
::����Ŀ������
:select_project
ECHO.
ECHO *******************************************************
ECHO e. Log    (error message)
ECHO t. Track  (remake track + fota)
ECHO ------------------------------------------------------
ECHO 1. NT55
ECHO 2. ET310
ECHO 3. EL100
ECHO 4. NT13V
ECHO 5. QB200
ECHO 6. NT17
ECHO 7. NF2319
ECHO 8. JM10
ECHO 9. NF2318
ECHO *******************************************************
set /p userInput=ѡ����Ŀ���(Q�˳�):
if /i "%userInput%"=="e" goto select_project_error_log
if /i "%userInput%"=="t" goto select_project_track
if /i "%userInput%"=="1" goto select_project_NT55
if /i "%userInput%"=="2" goto select_project_ET310
if /i "%userInput%"=="3" goto select_project_EL100
if /i "%userInput%"=="4" goto select_project_NT13V
if /i "%userInput%"=="5" goto select_project_QB200
if /i "%userInput%"=="6" goto select_project_NT17
if /i "%userInput%"=="7" goto select_project_NF2319
if /i "%userInput%"=="8" goto select_project_JM10
if /i "%userInput%"=="9" goto select_project_NF2318
if /i "%userInput%"=="Q" goto END
echo �㾿��Ҫ�����ĸ���Ŀ?
goto :select_project

:check1
if "1%~2%"=="1" goto check2
echo ֻ������һ����Ŀ������
goto :select_project

:check2
::echo %~dp0
::�ж��������Ŀ��
if /i "%1%"=="LOG" goto select_project_error_log
if /i "%1%"=="TRACK" goto select_project_track
if /i "%1%"=="NT55" goto select_project_NT55
if /i "%1%"=="ET310" goto select_project_ET310
if /i "%1%"=="EL100" goto select_project_EL100
if /i "%1%"=="NT13V" goto select_project_NT13V
if /i "%1%"=="QB200" goto select_project_QB200
if /i "%1%"=="NT17" goto select_project_NT17
if /i "%1%"=="NF2319" goto select_project_NF2319
if /i "%1%"=="JM10" goto select_project_JM10
if /i "%1%"=="NF2318" goto select_project_NF2318
echo ��Ŀ�������ڣ�������ѡ��
goto select_project

:select_project_error_log
	set mtk_project=GXQ03D_M2M_11C
	IF EXIST .\build\%mtk_project%\err.log DEL .\build\%mtk_project%\err.log /q
	findstr /S Error: .\build\%mtk_project%\bootloader.log >>.\build\%mtk_project%\err.log
	findstr /S Error: .\build\%mtk_project%\bootloader_ext.log >>.\build\%mtk_project%\err.log
	findstr /S Error: .\build\%mtk_project%\log\*.log >>.\build\%mtk_project%\err.log
	findstr /S Error: .\build\%mtk_project%\MT6261.log >>.\build\%mtk_project%\err.log
	ECHO.  >> .\build\%mtk_project%\err.log
	.\build\%mtk_project%\err.log
	goto end

::������Ŀ���ò���
:select_project_NT55
	set project=NT55
	set chip=MT2503D
	goto CompileCopy

:select_project_ET310
	set project=ET310
	set chip=MT2503D
	goto CompileCopy

:select_project_EL100
	set project=EL100
	set chip=MT2503D
	goto CompileCopy

:select_project_NT13V
	set project=NT13V
	set chip=MT2503A
	goto CompileCopy

:select_project_QB200
	set project=QB200
	set chip=MT2503D
	goto CompileCopy

:select_project_NT17
	set project=NT17
	set chip=MT2503D
	goto CompileCopy

:select_project_NF2319
	set project=NF2319
	set chip=MT2503A
	goto CompileCopy

:select_project_JM10
	set project=JM10
	set chip=MT2503A
	goto CompileCopy

:select_project_NF2318
	set project=NF2318
	set chip=MT2503A_128M
	goto CompileCopy

:CompileCopy

::1��������Ŀmak�ļ�
set fileSource=%~dp0make\GXQ03D_M2M_11C_GPRS_%project%.mak
echo ����%fileSource%
set fileTarget=%~dp0make\GXQ03D_M2M_11C_GPRS.mak
if exist %fileSource% (
if exist %fileTarget% del %fileTarget%
copy %fileSource%  %fileTarget%
echo ����%project%.mak�ļ�
) else (
echo %fileSource% ������
goto paramError
)

::2������ell_fota_memory_define.h
set fileSource=%~dp0track\mtk_sal\%chip%\ell_fota_memory_define.h
set fileTarget=%~dp0ELL\ell-common\ell_fota_memory_define.h
if exist %fileSource% (
if exist %fileTarget% del %fileTarget%
copy %fileSource%  %fileTarget%
echo ����%chip%\ell_fota_memory_define.h�ļ�
) else (
echo %fileSource% ������
goto paramError
)

::3������AutoGenFotaScat.pl
set fileSource=%~dp0track\mtk_sal\%chip%\AutoGenFotaScat.pl
set fileTarget=%~dp0tools\AutoGenFotaScat.pl
if exist %fileSource% (
if exist %fileTarget% del %fileTarget%
copy %fileSource%  %fileTarget%
echo ����%chip%\AutoGenFotaScat.pl�ļ�
) else (
echo %fileSource% ������
goto paramError
)

::4������custom_MemoryDevice.h
set fileSource=%~dp0track\mtk_sal\%chip%\custom_MemoryDevice.h
set fileTarget=%~dp0custom\system\GXQ03D_M2M_11C_BB\custom_MemoryDevice.h
if exist %fileSource% (
if exist %fileTarget% del %fileTarget%
copy %fileSource%  %fileTarget%
echo ����%chip%\custom_MemoryDevice.h�ļ�
) else (
echo %fileSource% ������
goto paramError
)

::5������FOTA_Tracker.pl
set fileSource=%~dp0track\mtk_sal\%chip%\FOTA_Tracker.pl
set fileTarget=%~dp0tools\FOTA_Tracker.pl
if exist %fileSource% (
if exist %fileTarget% del %fileTarget%
copy %fileSource%  %fileTarget%
echo ����%chip%\FOTA_Tracker.pl�ļ�
) else (
echo %fileSource% ������
goto paramError
)

::6������MergeSections.pl
set fileSource=%~dp0track\mtk_sal\%chip%\MergeSections.pl
set fileTarget=%~dp0tools\MergeSections.pl
if exist %fileSource% (
if exist %fileTarget% del %fileTarget%
copy %fileSource%  %fileTarget%
echo ����%chip%\MergeSections.pl�ļ�
) else (
echo %fileSource% ������
goto paramError
)

::7������scatGenLib.pl
set fileSource=%~dp0track\mtk_sal\%chip%\scatGenLib.pl
set fileTarget=%~dp0tools\scatGenLib.pl
if exist %fileSource% (
if exist %fileTarget% del %fileTarget%
copy %fileSource%  %fileTarget%
echo ����%chip%\scatGenLib.pl�ļ�
) else (
echo %fileSource% ������
goto paramError
)

::8������emigenMD.pl
set fileSource=%~dp0track\mtk_sal\%chip%\emigenMD.pl
set fileTarget=%~dp0tools\emigenMD.pl
if exist %fileSource% (
if exist %fileTarget% del %fileTarget%
copy %fileSource%  %fileTarget%
echo ����%chip%\emigenMD.pl�ļ�
) else (
echo %fileSource% ������
goto paramError
)


::����new+fota
ping -n 2 127.0.0.1>nul
::������Ŀmak�ļ���new����fota���������
CALL new.bat
ping -n 2 127.0.0.1>nul
CALL fota.bat newall
ping -n 1 127.0.0.1>nul
echo "mk >> %project% (%chip%) ִ�н���"
goto END

::remak track + fota newall
:select_project_track
CALL track.bat
CALL fota.bat newall
ping -n 1 127.0.0.1>nul
echo "track+fota ִ�н���"
goto END

:paramError
echo ������������޷�ִ��
goto END

:END
ping -n 1 127.0.0.1>nul
echo.
