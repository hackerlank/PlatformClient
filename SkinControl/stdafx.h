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


//�����ļ�
#include "Other\SkinSB.h"
#include "Other\Detours.h"

//ͼƬ�ؼ�
#import "ImageOle.dll" named_guids

//////////////////////////////////////////////////////////////////////////////////
//���Ӵ���

#define  SKIN_CONTROL_DLL

#pragma comment (lib,"Other/SkinSB.lib")
#pragma comment (lib,"Other/Detours.lib")

#ifndef _DEBUG
	#ifndef _UNICODE
		#pragma comment (lib,"../../���ӿ�/Ansi/WHImage.lib")
		#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCore.lib")
		#pragma comment (lib,"../../���ӿ�/Ansi/FGuilib.lib")
		#pragma comment (lib,"../../���ӿ�/Ansi/FGuimfcFactory.lib")
	#else
		#pragma comment (lib,"../../���ӿ�/Unicode/WHImage.lib")
		#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCore.lib")
		#pragma comment (lib,"../../���ӿ�/Unicode/FGuilib.lib")
		#pragma comment (lib,"../../���ӿ�/Unicode/FGuimfcFactory.lib")
	#endif
	#else
	#ifndef _UNICODE
		#pragma comment (lib,"../../���ӿ�/Ansi/WHImageD.lib")
		#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCoreD.lib")
		#pragma comment (lib,"../../���ӿ�/Ansi/FGuilibD.lib")
		#pragma comment (lib,"../../���ӿ�/Ansi/FGuimfcFactoryD.lib")
	#else
		#pragma comment (lib,"../../���ӿ�/Unicode/WHImageD.lib")
		#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCoreD.lib")
		#pragma comment (lib,"../../���ӿ�/Unicode/FGuilibD.lib")
		#pragma comment (lib,"../../���ӿ�/Unicode/FGuimfcFactoryD.lib")
	#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

