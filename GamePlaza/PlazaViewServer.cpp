#include "Stdafx.h"
#include "PlazaViewServer.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"

//////////////////////////////////////////////////////////////////////////////////

//房间等级
#define SERVER_LEVEL_1			1									//初级
#define SERVER_LEVEL_2			2									//中级
#define SERVER_LEVEL_3			3									//高级
#define SERVER_LEVEL_VIP		4									//VIP

//////////////////////////////////////////////////////////////////////////////////

//控件名称
const TCHAR* const szButtonGoBackControlName	= TEXT("ButtonGoBack");
const TCHAR* const szButtonServer1ControlName	= TEXT("ButtonServer1");
const TCHAR* const szButtonServer2ControlName	= TEXT("ButtonServer2");
const TCHAR* const szButtonServer3ControlName	= TEXT("ButtonServer3");
const TCHAR* const szButtonServer4ControlName	= TEXT("ButtonServer4");
const TCHAR* const szButtonServer5ControlName	= TEXT("ButtonServer5");
const TCHAR* const szButtonServer6ControlName	= TEXT("ButtonServer6");
const TCHAR* const szLabelServer1ControlName	= TEXT("LabelServer1");
const TCHAR* const szLabelServer2ControlName	= TEXT("LabelServer2");
const TCHAR* const szLabelServer3ControlName	= TEXT("LabelServer3");
const TCHAR* const szLabelServer4ControlName	= TEXT("LabelServer4");
const TCHAR* const szLabelServer5ControlName	= TEXT("LabelServer5");
const TCHAR* const szLabelServer6ControlName	= TEXT("LabelServer6");

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewServer, CFGuiWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

CPlazaViewServer::CPlazaViewServer()
{
	m_wUserClickIndex = 0;
}

//析构函数
CPlazaViewServer::~CPlazaViewServer()
{

}

//创建控件
CControlUI* CPlazaViewServer::CreateControl(LPCTSTR pstrClass)
{  
	if (_tcsicmp(pstrClass, kComplexInterface) == 0)  
		return  new CComplexUI();  
    return NULL;
}


//消息提醒
void CPlazaViewServer::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	if (pPlatformFrame==NULL) return;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonGoBackControlName)==0) {
			pPlatformFrame->PostMessage( WM_QUIT_GAME, WM_QUIT_GAME, 0);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonServer1ControlName)==0) {
			m_wUserClickIndex = 1;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonServer2ControlName)==0) {
			m_wUserClickIndex = 2;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonServer3ControlName)==0) {
			m_wUserClickIndex = 3;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonServer4ControlName)==0) {
			m_wUserClickIndex = 4;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonServer5ControlName)==0) {
			m_wUserClickIndex = 5;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonServer6ControlName)==0) {
			m_wUserClickIndex = 6;
		}
		pPlatformFrame->PostMessage( WM_ENTER_SERVER, 0, m_wUserClickIndex );
	}

	return;
}

CGameServerItem * CPlazaViewServer::GetUserChickServerItem()
{
	if (m_wUserClickIndex==0 ) return NULL;

	//服务列表
	WORD wCount = m_GameServerArray.GetCount();
	if (wCount==0) return NULL;

	if (wCount<m_wUserClickIndex) return NULL;
	return m_GameServerArray[(m_wUserClickIndex-1)];
}


void CPlazaViewServer::InitControlUI()
{
	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());
	
	//获取对象
	m_pButtonArray[0] = (CComplexUI * ) GetControlByName( szButtonServer1ControlName );
	m_pButtonArray[1] = (CComplexUI * ) GetControlByName( szButtonServer2ControlName );
	m_pButtonArray[2] = (CComplexUI * ) GetControlByName( szButtonServer3ControlName );
	m_pButtonArray[3] = (CComplexUI * ) GetControlByName( szButtonServer4ControlName );
	m_pButtonArray[4] = (CComplexUI * ) GetControlByName( szButtonServer5ControlName );
	m_pButtonArray[5] = (CComplexUI * ) GetControlByName( szButtonServer6ControlName );

	//投影背景
	m_pLabelArray[0] = (CLabelUI * ) GetControlByName( szLabelServer1ControlName );
	m_pLabelArray[1] = (CLabelUI * ) GetControlByName( szLabelServer2ControlName );
	m_pLabelArray[2] = (CLabelUI * ) GetControlByName( szLabelServer3ControlName );
	m_pLabelArray[3] = (CLabelUI * ) GetControlByName( szLabelServer4ControlName );
	m_pLabelArray[4] = (CLabelUI * ) GetControlByName( szLabelServer5ControlName );
	m_pLabelArray[5] = (CLabelUI * ) GetControlByName( szLabelServer6ControlName );
	
}

VOID CPlazaViewServer::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
	return;
}

//位置消息
VOID CPlazaViewServer::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);
	return;
}

//创建消息
INT CPlazaViewServer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("CONTENT_BK"));

	//设置大小
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL, 0, 0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	return 0;
}

// 更新房间
VOID CPlazaViewServer::RemoveServerItem()
{
	//首页设置
	m_GameServerPage = 0;

	//移除列表
	m_GameServerArray.RemoveAll();
}

// 房间目录
VOID CPlazaViewServer::InsertServerItem( CGameServerItem * pGameServerItem )
{
	//安全返回
	if (pGameServerItem==NULL) return;
	
	//房间设置
	m_GameServerArray.Add( pGameServerItem );

}

	// 房间目录
VOID CPlazaViewServer::UpdateServerItem( WORD wKindID )
{
	// 更新标提
	CLabelUI * pLabelTitle = (CLabelUI * ) GetControlByName( TEXT("LableGameName") );
	if (pLabelTitle==NULL) return;
	
	//标题背景
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("BK_TOP_GAME_LOGO"));
	WORD hBack  = ImageBack.GetHeight();
	WORD wBack  = ImageBack.GetWidth();

	//更新图片
	CPngImage ImageTitle;
	switch(wKindID)
	{
	case GID_BACCARAT:
		ImageTitle.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("GAME_TITLE_BACCARAT"));
		pLabelTitle->SetBkImage(TEXT("file='GAME_TITLE_BACCARAT' restype='PNG'"));
		break;
	case GID_FIVECARD:
		ImageTitle.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("GAME_TITLE_FIVECARD"));
		pLabelTitle->SetBkImage(TEXT("file='GAME_TITLE_FIVECARD' restype='PNG'"));
		break;
	case GID_LAND:
		ImageTitle.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("GAME_TITLE_LAND"));
		pLabelTitle->SetBkImage(TEXT("file='GAME_TITLE_LAND' restype='PNG'"));
		break;
	case GID_OXBATTLE:
		ImageTitle.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("GAME_TITLE_OXBATTLE"));
		pLabelTitle->SetBkImage(TEXT("file='GAME_TITLE_OXBATTLE' restype='PNG'"));
		break;
	case GID_OXNEW:
		ImageTitle.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("GAME_TITLE_OXNEW"));
		pLabelTitle->SetBkImage(TEXT("file='GAME_TITLE_OXNEW' restype='PNG'"));
		break;
	case GID_ZAJINHUA:
		ImageTitle.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("GAME_TITLE_ZAJINHUA"));
		pLabelTitle->SetBkImage(TEXT("file='GAME_TITLE_ZAJINHUA' restype='PNG'"));
		break;
	}

	//更新位置
	WORD hTitle =  ImageTitle.GetHeight();
	WORD wTitle  = ImageTitle.GetWidth();
	WORD x = (wBack - wTitle)/2;
	WORD y = (hBack - hTitle)/2;
	pLabelTitle->SetPos(x,y);
	pLabelTitle->SetFixedHeight(hTitle);
	pLabelTitle->SetFixedWidth(wTitle);

	//服务列表
	WORD wCount = m_GameServerArray.GetCount();
	if (wCount==0) return;

	//等级排序
	CGameServerItem * pTemp = NULL;
	for(int i=0; i<wCount; i++)
	{
		for(int j=i+1; j<wCount; j++)
		{
			if (m_GameServerArray[i] && m_GameServerArray[j])
			{
				if( m_GameServerArray[i]->m_GameServer.wServerLevel > m_GameServerArray[j]->m_GameServer.wServerLevel )
				{
					pTemp = m_GameServerArray[i];
					m_GameServerArray[i] = m_GameServerArray[j];
					m_GameServerArray[j] = pTemp;
				}
			}
		}
	}

	//隐藏按钮
	for(int i=0; i<PAGE_SERVER; i++) 
	{
		if(m_pButtonArray[i]!=NULL) m_pButtonArray[i]->SetVisible(false);
		if(m_pLabelArray[i]!=NULL) m_pLabelArray[i]->SetVisible(false);
	}

	//更新列表
	WORD wBeginPos = m_GameServerPage * PAGE_SERVER;
	if (wBeginPos<wCount) 
	{
		WORD wEndPos = 0;

		//更新按钮
		for(int i=wBeginPos; i<wCount; i++) 
		{
			wEndPos++;
			CGameServerItem * pGameServerItem = NULL;
			pGameServerItem = m_GameServerArray[i];
			if (pGameServerItem!=NULL) 
			{
				//参数加载
				m_pButtonArray[i]->SetMaxOnline(pGameServerItem->m_GameServer.dwFullCount);
				m_pButtonArray[i]->SetOnline(pGameServerItem->m_GameServer.dwAndroidCount);
				m_pButtonArray[i]->SetEnterScore(pGameServerItem->m_GameServer.lEnterScore);
				m_pButtonArray[i]->SetCellScore(pGameServerItem->m_GameServer.lCellScore);
				m_pButtonArray[i]->UpdateInfo();

				//显示加载
				WORD wLevel = pGameServerItem->m_GameServer.wServerLevel;
				switch (wLevel)
				{
				case SERVER_LEVEL_1:
					m_pButtonArray[i]->SetStatusImage(L"file='BT_ROOM_1' restype='PNG'");
					break;
				case SERVER_LEVEL_2:
					m_pButtonArray[i]->SetStatusImage(L"file='BT_ROOM_2' restype='PNG'");
					break;
				case SERVER_LEVEL_3:
					m_pButtonArray[i]->SetStatusImage(L"file='BT_ROOM_3' restype='PNG'");
					break;
				case SERVER_LEVEL_VIP:
					m_pButtonArray[i]->SetStatusImage(L"file='BT_ROOM_VIP' restype='PNG'");
					break;
				}


				//打开按钮
				if(m_pButtonArray[i]!=NULL) m_pButtonArray[i]->SetVisible(true);
				if(m_pLabelArray[i]!=NULL) m_pLabelArray[i]->SetVisible(true);
			}
		}
	}
}
