# Microsoft Developer Studio Project File - Name="MMI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MMI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MMI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MMI.mak" CFG="MMI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MMI - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MMI - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MMI - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /incremental:yes /debug /machine:I386 /def:".\MMI.def" /out:"../FleetOptimizer/Release/MMI.dll" /implib:"../FleetOptimizer/Release_Libraries/MMI.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MMI - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\MMI.def" /out:"../FleetOptimizer/Debug/MMI.dll" /implib:"../FleetOptimizer/Debug_Libraries/MMI.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "MMI - Win32 Release"
# Name "MMI - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\EditGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\EditWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\InvestDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LogsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainDetailsPPage3.cpp
# End Source File
# Begin Source File

SOURCE=.\MMI.cpp
# End Source File
# Begin Source File

SOURCE=.\MMI.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\MMI.rc
# End Source File
# Begin Source File

SOURCE=.\msflexgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\OptimizSettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\PortDetailsPSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\PortDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PortDialog1.cpp
# End Source File
# Begin Source File

SOURCE=.\PortDistMatrixPPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PortLandStructInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PortLandStructPPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PortMainDetailsPPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PortMainInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PortWizardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PrototypeWizardACVDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PrototypeWizardShipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\rowcursor.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenarioWizardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShipMainInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ShipTypeMainPPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ShipTypeOptimizParamPPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ShipTypePropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ShipTypeWizardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\EditGrid.h
# End Source File
# Begin Source File

SOURCE=.\EditWnd.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\InvestDlg.h
# End Source File
# Begin Source File

SOURCE=.\LogsDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainDetailsPPage3.h
# End Source File
# Begin Source File

SOURCE=.\msflexgrid.h
# End Source File
# Begin Source File

SOURCE=.\OptimizSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\PortDetailsPSheet.h
# End Source File
# Begin Source File

SOURCE=.\PortDialog.h
# End Source File
# Begin Source File

SOURCE=.\PortDialog1.h
# End Source File
# Begin Source File

SOURCE=.\PortDistMatrixPPage.h
# End Source File
# Begin Source File

SOURCE=.\PortLandStructInfo.h
# End Source File
# Begin Source File

SOURCE=.\PortLandStructPPage.h
# End Source File
# Begin Source File

SOURCE=.\PortMainDetailsPPage.h
# End Source File
# Begin Source File

SOURCE=.\PortMainInfo.h
# End Source File
# Begin Source File

SOURCE=.\PortWizardDlg.h
# End Source File
# Begin Source File

SOURCE=.\PrototypeWizardACVDlg.h
# End Source File
# Begin Source File

SOURCE=.\PrototypeWizardShipDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\rowcursor.h
# End Source File
# Begin Source File

SOURCE=.\ScenarioWizardDlg.h
# End Source File
# Begin Source File

SOURCE=.\ShipMainInfo.h
# End Source File
# Begin Source File

SOURCE=.\ShipTypeMainPPage.h
# End Source File
# Begin Source File

SOURCE=.\ShipTypeOptimizParamPPage.h
# End Source File
# Begin Source File

SOURCE=.\ShipTypePropertySheet.h
# End Source File
# Begin Source File

SOURCE=.\ShipTypeWizardDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\MMI.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section MMI : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section MMI : {9F6AA700-D188-11CD-AD48-00AA003C9CB6}
# 	2:5:Class:CRowCursor
# 	2:10:HeaderFile:rowcursor.h
# 	2:8:ImplFile:rowcursor.cpp
# End Section
# Section MMI : {6262D3A0-531B-11CF-91F6-C2863C385E30}
# 	2:21:DefaultSinkHeaderFile:msflexgrid.h
# 	2:16:DefaultSinkClass:CMSFlexGrid
# End Section
# Section MMI : {5F4DF280-531B-11CF-91F6-C2863C385E30}
# 	2:5:Class:CMSFlexGrid
# 	2:10:HeaderFile:msflexgrid.h
# 	2:8:ImplFile:msflexgrid.cpp
# End Section
# Section MMI : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
