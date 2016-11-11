// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#ifndef WINVER			
#define WINVER 0x0400	
#endif

#ifndef _WIN32_WINNT	
#define _WIN32_WINNT 0x0400	
#endif						

#ifndef _WIN32_WINDOWS		
#define _WIN32_WINDOWS 0x0410 
#endif

#ifndef _WIN32_IE			
#define _WIN32_IE 0x0400	
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	

#include <afxwin.h>         
#include <afxext.h>         

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         
#include <afxodlgs.h>       
#include <afxdisp.h>        
#endif 

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			
#endif 

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			
#endif 

#include <afxdtctl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			
#endif 


//////////////////////////////////////////////////////////////////////////
//链接代码
#pragma comment (lib,"Winmm.lib")

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCore.lib")
#pragma comment (lib,"../../链接库/Ansi/SkinControl.lib")
#pragma comment (lib,"../../链接库/Ansi/WHImage.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCore.lib")
#pragma comment (lib,"../../链接库/Unicode/SkinControl.lib")
#pragma comment (lib,"../../链接库/Unicode/WHImage.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCoreD.lib")
#pragma comment (lib,"../../链接库/Ansi/SkinControlD.lib")
#pragma comment (lib,"../../链接库/Ansi/WHImageD.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCoreD.lib")
#pragma comment (lib,"../../链接库/Unicode/SkinControlD.lib")
#pragma comment (lib,"../../链接库/Unicode/WHImageD.lib")
#endif
#endif


#include "./VideoSDK/include/BRAnyChatCoreSDK.h"
#include "./VideoSDK/include/GVMessageDefine.h"
#pragma comment(lib,"./VideoSDK/lib/BRAnyChatCore.lib")


//////////////////////////////////////////////////////////////////////////
#define VIDEO_SERVICE_DLL