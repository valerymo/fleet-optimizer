# Microsoft Developer Studio Project File - Name="FleetOptimizer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FleetOptimizer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FleetOptimizer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FleetOptimizer.mak" CFG="FleetOptimizer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FleetOptimizer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FleetOptimizer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FleetOptimizer - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Release_Libraries\MMI.lib Release_Libraries\ShipDesignModule.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "FleetOptimizer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FleetOptimizer - Win32 Release"
# Name "FleetOptimizer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BasePlan.cpp
# End Source File
# Begin Source File

SOURCE=.\BasePlanSt1LP.cpp
# End Source File
# Begin Source File

SOURCE=.\BasePlanSt2LpDp.cpp
# End Source File
# Begin Source File

SOURCE=.\BasePlanSt3Dp.cpp
# End Source File
# Begin Source File

SOURCE=.\Cargo.cpp
# End Source File
# Begin Source File

SOURCE=.\EasyStartDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FleetOptimizer.cpp
# End Source File
# Begin Source File

SOURCE=.\FleetOptimizer.rc
# End Source File
# Begin Source File

SOURCE=.\FleetOptimizerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\FleetOptimizerView.cpp
# End Source File
# Begin Source File

SOURCE=.\FleetTechEconomicEng.cpp
# End Source File
# Begin Source File

SOURCE=.\InitialBasePlan.cpp
# End Source File
# Begin Source File

SOURCE=.\LandStructList.cpp
# End Source File
# Begin Source File

SOURCE=.\LandStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\Line.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MarineTechEconomicEng.cpp
# End Source File
# Begin Source File

SOURCE=.\OptimizationSt1LP.cpp
# End Source File
# Begin Source File

SOURCE=.\OptimizationSt2LpDp.cpp
# End Source File
# Begin Source File

SOURCE=.\OptimizationSt3Dp.cpp
# End Source File
# Begin Source File

SOURCE=.\Optimizer.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputView.cpp
# End Source File
# Begin Source File

SOURCE=.\Port.cpp
# End Source File
# Begin Source File

SOURCE=.\PortFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\PortList.cpp
# End Source File
# Begin Source File

SOURCE=.\PortPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RegisterRules.cpp
# End Source File
# Begin Source File

SOURCE=.\Router.cpp
# End Source File
# Begin Source File

SOURCE=.\RouterL.cpp
# End Source File
# Begin Source File

SOURCE=.\RouterP.cpp
# End Source File
# Begin Source File

SOURCE=.\RouterS.cpp
# End Source File
# Begin Source File

SOURCE=.\Scheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\Ship.cpp
# End Source File
# Begin Source File

SOURCE=.\ShipCargoPlan.cpp
# End Source File
# Begin Source File

SOURCE=.\ShipCargoPlanNode.cpp
# End Source File
# Begin Source File

SOURCE=.\ShipFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Vehicle.cpp
# End Source File
# Begin Source File

SOURCE=.\VehiclesList.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BasePlan.h
# End Source File
# Begin Source File

SOURCE=.\BasePlanSt1LP.h
# End Source File
# Begin Source File

SOURCE=.\BasePlanSt2LpDp.h
# End Source File
# Begin Source File

SOURCE=.\BasePlanSt3Dp.h
# End Source File
# Begin Source File

SOURCE=.\Cargo.h
# End Source File
# Begin Source File

SOURCE=.\EasyStartDlg.h
# End Source File
# Begin Source File

SOURCE=.\FleetOptimizer.h
# End Source File
# Begin Source File

SOURCE=.\FleetOptimizerDoc.h
# End Source File
# Begin Source File

SOURCE=.\FleetOptimizerView.h
# End Source File
# Begin Source File

SOURCE=.\FleetTechEconomicEng.h
# End Source File
# Begin Source File

SOURCE=.\InitialBasePlan.h
# End Source File
# Begin Source File

SOURCE=.\LandStructList.h
# End Source File
# Begin Source File

SOURCE=.\LandStructure.h
# End Source File
# Begin Source File

SOURCE=.\Line.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MarineTechEconomicEng.h
# End Source File
# Begin Source File

SOURCE=.\OptimizationSt1LP.h
# End Source File
# Begin Source File

SOURCE=.\OptimizationSt2LpDp.h
# End Source File
# Begin Source File

SOURCE=.\OptimizationSt3Dp.h
# End Source File
# Begin Source File

SOURCE=.\Optimizer.h
# End Source File
# Begin Source File

SOURCE=.\OutputView.h
# End Source File
# Begin Source File

SOURCE=.\Port.h
# End Source File
# Begin Source File

SOURCE=.\PortFactory.h
# End Source File
# Begin Source File

SOURCE=.\PortList.h
# End Source File
# Begin Source File

SOURCE=.\PortPoint.h
# End Source File
# Begin Source File

SOURCE=.\ProcessDlg.h
# End Source File
# Begin Source File

SOURCE=.\RegisterRules.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Router.h
# End Source File
# Begin Source File

SOURCE=.\RouterL.h
# End Source File
# Begin Source File

SOURCE=.\RouterP.h
# End Source File
# Begin Source File

SOURCE=.\RouterS.h
# End Source File
# Begin Source File

SOURCE=.\Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Ship.h
# End Source File
# Begin Source File

SOURCE=.\ShipCargoPlan.h
# End Source File
# Begin Source File

SOURCE=.\ShipCargoPlanNode.h
# End Source File
# Begin Source File

SOURCE=.\ShipFactory.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Vehicle.h
# End Source File
# Begin Source File

SOURCE=.\VehiclesList.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\FleetOptimizer.ico
# End Source File
# Begin Source File

SOURCE=.\res\FleetOptimizer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\FleetOptimizerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section FleetOptimizer : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section FleetOptimizer : {9F6AA700-D188-11CD-AD48-00AA003C9CB6}
# 	2:5:Class:CRowCursor
# 	2:10:HeaderFile:rowcursor.h
# 	2:8:ImplFile:rowcursor.cpp
# End Section
# Section FleetOptimizer : {5F4DF280-531B-11CF-91F6-C2863C385E30}
# 	2:5:Class:CMSFlexGrid
# 	2:10:HeaderFile:msflexgrid.h
# 	2:8:ImplFile:msflexgrid.cpp
# End Section
# Section FleetOptimizer : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section FleetOptimizer : {6262D3A0-531B-11CF-91F6-C2863C385E30}
# 	2:21:DefaultSinkHeaderFile:msflexgrid1.h
# 	2:16:DefaultSinkClass:CMSFlexGrid
# End Section
