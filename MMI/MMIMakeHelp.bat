@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by MMI.HPJ >"..\hlp\MMI.hm"
echo. >>"..\hlp\MMI.hm"
echo // Commands (ID_* and IDM_*) >>"..\hlp\MMI.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"..\hlp\MMI.hm"
echo. >>"..\hlp\MMI.hm"
echo // Prompts (IDP_*) >>"..\hlp\MMI.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"..\hlp\MMI.hm"
echo. >>"..\hlp\MMI.hm"
echo // Resources (IDR_*) >>"..\hlp\MMI.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"..\hlp\MMI.hm"
echo. >>"..\hlp\MMI.hm"
echo // Dialogs (IDD_*) >>"..\hlp\MMI.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"..\hlp\MMI.hm"
echo. >>"..\hlp\MMI.hm"
echo // Frame Controls (IDW_*) >>"..\hlp\MMI.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"..\hlp\MMI.hm"
REM -- Make help for Project MMI


REM echo Building Win32 Help files
REM start /wait hcw /C /E /M "..\hlp\MMI.hpj"
REM if errorlevel 1 goto :Error
REM if not exist "..\hlp\MMI.hlp" goto :Error
REM if not exist "..\hlp\MMI.cnt" goto :Error
REM echo.
REM if exist Debug\nul copy "..\hlp\MMI.hlp" Debug
REM if exist Debug\nul copy "..\hlp\MMI.cnt" Debug
REM if exist Release\nul copy "..\hlp\MMI.hlp" Release
REM if exist Release\nul copy "..\hlp\MMI.cnt" Release
REM echo.
REM goto :done

REM :Error
REM echo hlp\MMI.hpj(1) : error: Problem encountered creating help file

:done
echo.
