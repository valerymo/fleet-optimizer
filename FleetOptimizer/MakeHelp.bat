@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by FleetOptimizer.HPJ >"..\hlp\FleetOptimizer.hm"
echo. >>"..\hlp\FleetOptimizer.hm"
echo // Commands (ID_* and IDM_*) >>"..\hlp\FleetOptimizer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"..\hlp\FleetOptimizer.hm"
echo. >>"..\hlp\FleetOptimizer.hm"
echo // Prompts (IDP_*) >>"..\hlp\FleetOptimizer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"..\hlp\FleetOptimizer.hm"
echo. >>"..\hlp\FleetOptimizer.hm"
echo // Resources (IDR_*) >>"..\hlp\FleetOptimizer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"..\hlp\FleetOptimizer.hm"
echo. >>"..\hlp\FleetOptimizer.hm"
echo // Dialogs (IDD_*) >>"..\hlp\FleetOptimizer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"..\hlp\FleetOptimizer.hm"
echo. >>"..\hlp\FleetOptimizer.hm"
echo // Frame Controls (IDW_*) >>"..\hlp\FleetOptimizer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"..\hlp\FleetOptimizer.hm"
REM -- Make help for Project FleetOptimizer


REM echo Building Win32 Help files
REM start /wait hcw /C /E /M "..\hlp\FleetOptimizer.hpj"
REM if errorlevel 1 goto :Error
REM if not exist "..\hlp\FleetOptimizer.hlp" goto :Error
REM if not exist "..\hlp\FleetOptimizer.cnt" goto :Error
REM echo.
REM if exist Debug\nul copy "..\hlp\FleetOptimizer.hlp" Debug
REM if exist Debug\nul copy "..\hlp\FleetOptimizer.cnt" Debug
REM if exist Release\nul copy "..\hlp\FleetOptimizer.hlp" Release
REM if exist Release\nul copy "..\hlp\FleetOptimizer.cnt" Release
REM echo.
REM goto :done

REM :Error
REM echo hlp\FleetOptimizer.hpj(1) : error: Problem encountered creating help file

:done
echo.
