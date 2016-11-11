// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

//////////////////////////////////////////////////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#include "targetver.h"

#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//////////////////////////////////////////////////////////////////////////////////

//MFC 文件
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

//MFC 文件
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

//////////////////////////////////////////////////////////////////////////////////
//链接代码

//多媒体库
#pragma comment (lib,"Winmm.lib")

//链接代码
#ifndef _DEBUG
	#ifndef _UNICODE
		#pragma comment (lib,"../../链接库/Ansi/WHImage.lib")
		#pragma comment (lib,"../../链接库/Ansi/FGuilib.lib")
		#pragma comment (lib,"../../链接库/Ansi/GameEngine.lib")
		#pragma comment (lib,"../../链接库/Ansi/ServiceCore.lib")
		#pragma comment (lib,"../../链接库/Ansi/SkinControl.lib")
		#pragma comment (lib,"../../链接库/Ansi/ShareControl.lib")
		#pragma comment (lib,"../../链接库/Ansi/PlatformData.lib")
		#pragma comment (lib,"../../链接库/Ansi/GameProperty.lib")
		#pragma comment (lib,"../../链接库/Ansi/AvatarControl.lib")
		#pragma comment (lib,"../../链接库/Ansi/VideoService.lib")
		#pragma comment (lib,"../../链接库/Ansi/FGuimfcFactory.lib")
	#else
		#pragma comment (lib,"../../链接库/Unicode/WHImage.lib")
		#pragma comment (lib,"../../链接库/Unicode/FGuilib.lib")
		#pragma comment (lib,"../../链接库/Unicode/GameEngine.lib")
		#pragma comment (lib,"../../链接库/Unicode/ServiceCore.lib")
		#pragma comment (lib,"../../链接库/Unicode/SkinControl.lib")
		#pragma comment (lib,"../../链接库/Unicode/ShareControl.lib")
		#pragma comment (lib,"../../链接库/Unicode/PlatformData.lib")
		#pragma comment (lib,"../../链接库/Unicode/GameProperty.lib")
		#pragma comment (lib,"../../链接库/Unicode/AvatarControl.lib")
		#pragma comment (lib,"../../链接库/Unicode/VideoService.lib")
		#pragma comment (lib,"../../链接库/Unicode/FGuimfcFactory.lib")
	#endif
#else
	#ifndef _UNICODE
		#pragma comment (lib,"../../链接库/Ansi/WHImageD.lib")
		#pragma comment (lib,"../../链接库/Ansi/FGuilibD.lib")
		#pragma comment (lib,"../../链接库/Ansi/GameEngineD.lib")
		#pragma comment (lib,"../../链接库/Ansi/ServiceCoreD.lib")
		#pragma comment (lib,"../../链接库/Ansi/SkinControlD.lib")
		#pragma comment (lib,"../../链接库/Ansi/ShareControlD.lib")
		#pragma comment (lib,"../../链接库/Ansi/PlatformDataD.lib")
		#pragma comment (lib,"../../链接库/Ansi/GamePropertyD.lib")
		#pragma comment (lib,"../../链接库/Ansi/AvatarControlD.lib")
		#pragma comment (lib,"../../链接库/Ansi/VideoServiceD.lib")
		#pragma comment (lib,"../../链接库/Ansi/FGuimfcFactoryD.lib")
	#else
		#pragma comment (lib,"../../链接库/Unicode/WHImageD.lib")
		#pragma comment (lib,"../../链接库/Unicode/FGuilibD.lib")
		#pragma comment (lib,"../../链接库/Unicode/GameEngineD.lib")
		#pragma comment (lib,"../../链接库/Unicode/ServiceCoreD.lib")
		#pragma comment (lib,"../../链接库/Unicode/SkinControlD.lib")
		#pragma comment (lib,"../../链接库/Unicode/ShareControlD.lib")
		#pragma comment (lib,"../../链接库/Unicode/PlatformDataD.lib")
		#pragma comment (lib,"../../链接库/Unicode/GamePropertyD.lib")
		#pragma comment (lib,"../../链接库/Unicode/AvatarControlD.lib")
		#pragma comment (lib,"../../链接库/Unicode/VideoServiceD.lib")
		#pragma comment (lib,"../../链接库/Unicode/FGuimfcFactoryD.lib")
	#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#define GAME_FRAME_DLL

