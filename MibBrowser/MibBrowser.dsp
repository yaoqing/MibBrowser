# Microsoft Developer Studio Project File - Name="MibBrowser" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MibBrowser - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MibBrowser.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MibBrowser.mak" CFG="MibBrowser - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MibBrowser - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MibBrowser - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "MibBrowser"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MibBrowser - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_UNICODE" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "MibBrowser - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_UNICODE" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MibBrowser - Win32 Release"
# Name "MibBrowser - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AddIpConfWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\base64.cpp
# End Source File
# Begin Source File

SOURCE=.\BGChartWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonEx.cpp
# End Source File
# Begin Source File

SOURCE=.\CaptionBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView.cpp
# End Source File
# Begin Source File

SOURCE=.\CSGuard.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawCube.cpp
# End Source File
# Begin Source File

SOURCE=.\EditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\LangLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\MacXFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\md5.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuEx.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MibBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\MibBrowser.rc
# End Source File
# Begin Source File

SOURCE=.\MibInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MibLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\MibTreeWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\NewGuard.cpp
# End Source File
# Begin Source File

SOURCE=.\PageDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\PageIcmpStat.cpp
# End Source File
# Begin Source File

SOURCE=.\PageInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\PageIpAddr.cpp
# End Source File
# Begin Source File

SOURCE=.\PageIpMedia.cpp
# End Source File
# Begin Source File

SOURCE=.\PageIpRoute.cpp
# End Source File
# Begin Source File

SOURCE=.\PageIpStat.cpp
# End Source File
# Begin Source File

SOURCE=.\PagePerformance.cpp
# End Source File
# Begin Source File

SOURCE=.\PageProc.cpp
# End Source File
# Begin Source File

SOURCE=.\PageService.cpp
# End Source File
# Begin Source File

SOURCE=.\PageSnmpStat.cpp
# End Source File
# Begin Source File

SOURCE=.\PageSoft.cpp
# End Source File
# Begin Source File

SOURCE=.\PageStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\PageSystemDesc.cpp
# End Source File
# Begin Source File

SOURCE=.\PageTcpStat.cpp
# End Source File
# Begin Source File

SOURCE=.\PageTcpTable.cpp
# End Source File
# Begin Source File

SOURCE=.\PageUdpStat.cpp
# End Source File
# Begin Source File

SOURCE=.\PageUdpTable.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PercentCharWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PromptWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\RegisterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\RegKeyEx.cpp
# End Source File
# Begin Source File

SOURCE=.\snmp.cpp
# End Source File
# Begin Source File

SOURCE=.\snmpdef.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitterBar.cpp
# End Source File
# Begin Source File

SOURCE=.\SpyedWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StringEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StringToken.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutWnd.h
# End Source File
# Begin Source File

SOURCE=.\AddIpConfWnd.h
# End Source File
# Begin Source File

SOURCE=.\base64.h
# End Source File
# Begin Source File

SOURCE=.\BGChartWnd.h
# End Source File
# Begin Source File

SOURCE=.\ButtonEx.h
# End Source File
# Begin Source File

SOURCE=.\CaptionBar.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrame.h
# End Source File
# Begin Source File

SOURCE=.\ChildView.h
# End Source File
# Begin Source File

SOURCE=.\CSGuard.h
# End Source File
# Begin Source File

SOURCE=.\DrawCube.h
# End Source File
# Begin Source File

SOURCE=.\EditEx.h
# End Source File
# Begin Source File

SOURCE=.\LangLoader.h
# End Source File
# Begin Source File

SOURCE=.\MacXFrame.h
# End Source File
# Begin Source File

SOURCE=.\md5.h
# End Source File
# Begin Source File

SOURCE=.\MemoWnd.h
# End Source File
# Begin Source File

SOURCE=.\MenuBar.h
# End Source File
# Begin Source File

SOURCE=.\MenuEx.h
# End Source File
# Begin Source File

SOURCE=.\MessageWnd.h
# End Source File
# Begin Source File

SOURCE=.\MibBrowser.h
# End Source File
# Begin Source File

SOURCE=.\MibInfoWnd.h
# End Source File
# Begin Source File

SOURCE=.\MibLoader.h
# End Source File
# Begin Source File

SOURCE=.\MibTreeWnd.h
# End Source File
# Begin Source File

SOURCE=.\NewGuard.h
# End Source File
# Begin Source File

SOURCE=.\PageDevice.h
# End Source File
# Begin Source File

SOURCE=.\PageIcmpStat.h
# End Source File
# Begin Source File

SOURCE=.\PageInterface.h
# End Source File
# Begin Source File

SOURCE=.\PageIpAddr.h
# End Source File
# Begin Source File

SOURCE=.\PageIpMedia.h
# End Source File
# Begin Source File

SOURCE=.\PageIpRoute.h
# End Source File
# Begin Source File

SOURCE=.\PageIpStat.h
# End Source File
# Begin Source File

SOURCE=.\PagePerformance.h
# End Source File
# Begin Source File

SOURCE=.\PageProc.h
# End Source File
# Begin Source File

SOURCE=.\PageService.h
# End Source File
# Begin Source File

SOURCE=.\PageSnmpStat.h
# End Source File
# Begin Source File

SOURCE=.\PageSoft.h
# End Source File
# Begin Source File

SOURCE=.\PageStorage.h
# End Source File
# Begin Source File

SOURCE=.\PageSystemDesc.h
# End Source File
# Begin Source File

SOURCE=.\PageTcpStat.h
# End Source File
# Begin Source File

SOURCE=.\PageTcpTable.h
# End Source File
# Begin Source File

SOURCE=.\PageUdpStat.h
# End Source File
# Begin Source File

SOURCE=.\PageUdpTable.h
# End Source File
# Begin Source File

SOURCE=.\PanelWnd.h
# End Source File
# Begin Source File

SOURCE=.\PercentCharWnd.h
# End Source File
# Begin Source File

SOURCE=.\PromptWnd.h
# End Source File
# Begin Source File

SOURCE=.\PropertyWnd.h
# End Source File
# Begin Source File

SOURCE=.\RegisterWnd.h
# End Source File
# Begin Source File

SOURCE=.\RegKeyEx.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\snmp.h
# End Source File
# Begin Source File

SOURCE=.\snmpdef.h
# End Source File
# Begin Source File

SOURCE=.\SplitterBar.h
# End Source File
# Begin Source File

SOURCE=.\SpyedWnd.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StringEx.h
# End Source File
# Begin Source File

SOURCE=.\StringToken.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Arrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\computer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Gripe.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GripeHot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Leaf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LeafAdd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LeafSub.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MenuBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MenuHot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MibBrowser.exe.Manifest
# End Source File
# Begin Source File

SOURCE=.\res\MibBrowser.ico
# End Source File
# Begin Source File

SOURCE=.\res\MibBrowser.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Pattern1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Pattern2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TitleBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TitleBox.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TitleIco.bmp
# End Source File
# End Group
# End Target
# End Project
