// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>            // MFC �׽�����չ



//////////////////////////////////////////////////////////////////////////////////
//���Ӵ���

#define SHARE_CONTROL_DLL

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../���ӿ�/Ansi/WHImage.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/FGuilib.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/DownLoad.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCore.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/SkinControl.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/PlatformData.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/FGuimfcFactory.lib")
#else
	#pragma comment (lib,"../../���ӿ�/Unicode/WHImage.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/FGuilib.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/DownLoad.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCore.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/SkinControl.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/PlatformData.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/FGuimfcFactory.lib")	
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../���ӿ�/Ansi/WHImageD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/FGuilibD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/DownLoadD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCoreD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/SkinControlD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/PlatformDataD.lib")
	#pragma comment (lib,"../../���ӿ�/Ansi/FGuimfcFactoryD.lib")	
#else
	#pragma comment (lib,"../../���ӿ�/Unicode/WHImageD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/FGuilibD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/DownLoadD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCoreD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/SkinControlD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/PlatformDataD.lib")
	#pragma comment (lib,"../../���ӿ�/Unicode/FGuimfcFactoryD.lib")	
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////
