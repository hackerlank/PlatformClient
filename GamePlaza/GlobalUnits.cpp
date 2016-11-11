#include "Stdafx.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"


//////////////////////////////////////////////////////////////////////////////////
//静态变量

CGlobalUnits * CGlobalUnits::m_pGlobalUnits=NULL;						//对象指针

//////////////////////////////////////////////////////////////////////////////////



//构造函数
CGlobalUnits::CGlobalUnits()
{
	//版本信息
	m_dwPlazaVersion=VERSION_PLAZA;
	m_dwFrameVersion=VERSION_FRAME;

	//追踪变量
	m_dwTrackUserID=0;
	m_wTrackServerID=0;

	//设置对象
	ASSERT(m_pGlobalUnits==NULL);
	if (m_pGlobalUnits==NULL) m_pGlobalUnits=this;

	return;
}

//析构函数
CGlobalUnits::~CGlobalUnits()
{
	//释放对象
	ASSERT(m_pGlobalUnits==this);
	if (m_pGlobalUnits==this) m_pGlobalUnits=NULL;

	return;
}


//播放声音
bool CGlobalUnits::PlayGameSound(LPCTSTR pszSoundName)
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//播放判断
	if (pParameterGlobal->m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//播放声音
	PlaySound(pszSoundName,AfxGetInstanceHandle(),SND_ASYNC|SND_NODEFAULT);

	return true;
}

//播放声音
bool CGlobalUnits::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//播放判断
	if (pParameterGlobal->m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//加载资源
	HRSRC hResource=FindResource(hInstance,pszSoundName,TEXT("WAVE"));
	if (hResource==NULL) return false;

	//锁定资源
	HGLOBAL hGlobal=LoadResource(hInstance,hResource);
	if (hGlobal==NULL) return false;

	//播放声音
	LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
	PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY);

	//清理资源
	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return true;
}


//获取标识
DWORD CGlobalUnits::GetTrackUserID()
{
	//变量定义
	DWORD dwTrackUserID=m_dwTrackUserID;

	//设置变量
	m_dwTrackUserID=0;

	return dwTrackUserID;
}

//获取房间
WORD CGlobalUnits::GetTrackServerID()
{
	//变量定义
	WORD wTrackServerID=m_wTrackServerID;

	//设置变量
	m_wTrackServerID=0;

	return wTrackServerID;
}


//下载游戏
bool CGlobalUnits::DownLoadClient(LPCTSTR pszClientName, WORD wKindID, WORD wServerID)
{
	//创建对象
	
	/*
	if (m_pDlgDownLoad==NULL)
	{
		m_pDlgDownLoad=new CDlgDownLoad;
	}

	//创建窗口
	if (m_pDlgDownLoad->m_hWnd==NULL)
	{
		m_pDlgDownLoad->Create(IDD_DLG_POPUP,CPlatformFrame::GetInstance());
	}

	//下载游戏
	m_pDlgDownLoad->DownLoadClient(pszClientName,wKindID,wServerID);
	*/

	return true;
}