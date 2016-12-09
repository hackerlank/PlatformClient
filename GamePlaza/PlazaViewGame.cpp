#include "StdAfx.h"
#include "PlazaViewGame.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"

//////////////////////////////////////////////////////////////////////////////////

//按钮控件
const TCHAR* const szButtonGameBaccaratControlName	= TEXT("ButtonGameBaccarat");
const TCHAR* const szButtonGameFiveCardControlName	= TEXT("ButtonGameFiveCard");
const TCHAR* const szButtonGameLandControlName		= TEXT("ButtonGameLand");
const TCHAR* const szButtonGameOxBattleControlName	= TEXT("ButtonGameOxBattle");
const TCHAR* const szButtonGameOxNewControlName		= TEXT("ButtonGameOxNew");
const TCHAR* const szButtonGameZaJinHuaControlName	= TEXT("ButtonGameZaJinHua");
const TCHAR* const szButtonGameDeZhouControlName	= TEXT("ButtonGameDeZhou");
const TCHAR* const szButtonGameSparrowXZControlName	= TEXT("ButtonGameSparrowXZ");

//////////////////////////////////////////////////////////////////////////////////

//控件图片
const TCHAR* const szButtonGameBaccaratImageName	= TEXT("file='BT_GAME_BACCARAT' restype='PNG'");
const TCHAR* const szButtonGameFiveCardImageName	= TEXT("file='BT_GAME_FIVECARD' restype='PNG'");
const TCHAR* const szButtonGameLandImageName		= TEXT("file='BT_GAME_LAND' restype='PNG'");
const TCHAR* const szButtonGameOxBattleImageName	= TEXT("file='BT_GAME_OXBATTLE' restype='PNG'");
const TCHAR* const szButtonGameOxNewImageName		= TEXT("file='BT_GAME_OXNEW' restype='PNG'");
const TCHAR* const szButtonGameZaJinHuaImageName	= TEXT("file='BT_GAME_ZAJINHUA' restype='PNG'");
const TCHAR* const szButtonGameDeZhouImageName	    = TEXT("file='BT_GAME_DEZHOU' restype='PNG'");
const TCHAR* const szButtonGameSparrowXZImageName   = TEXT("file='BT_GAME_SPARROWXZ' restype='PNG'");

//////////////////////////////////////////////////////////////////////////////////
CGameButtonImage::CGameButtonImage(WORD wID, const TCHAR * szName, const TCHAR* szImage)
{
	wGameID			=	wID;
	wButtonName		=	szName;
	wButtonImage	=	szImage;
}

CGameButtonImage::~CGameButtonImage()
{
	wGameID = 0;
	wButtonName = NULL;
	wButtonImage = NULL;
}

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewGame, CFGuiWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()

	//自定义消息
	ON_MESSAGE(WM_CLICKED_LEFT, OnClickedLeftPageView)
	ON_MESSAGE(WM_CLICKED_RIGHT, OnClickedRightPageView)

END_MESSAGE_MAP()

//构造函数
CPlazaViewGame::CPlazaViewGame()
{
	m_ButtonImageMap.InitHashTable(PRIME_KIND);
	m_GameServerArray.AllocMemory(PRIME_SERVER);
	
	m_ButtonImageMap[GID_BACCARAT]	= new CGameButtonImage( GID_BACCARAT,  szButtonGameBaccaratControlName,  szButtonGameBaccaratImageName );
	m_ButtonImageMap[GID_FIVECARD]	= new CGameButtonImage( GID_FIVECARD,  szButtonGameFiveCardControlName,  szButtonGameFiveCardImageName );
	m_ButtonImageMap[GID_LAND]		= new CGameButtonImage( GID_LAND,	   szButtonGameLandControlName,	     szButtonGameLandImageName	   );
	m_ButtonImageMap[GID_OXBATTLE]	= new CGameButtonImage( GID_OXBATTLE,  szButtonGameOxBattleControlName,  szButtonGameOxBattleImageName );
	m_ButtonImageMap[GID_OXNEW]		= new CGameButtonImage( GID_OXNEW,	   szButtonGameOxNewControlName,	 szButtonGameOxNewImageName	   );
	m_ButtonImageMap[GID_ZAJINHUA]	= new CGameButtonImage( GID_ZAJINHUA,  szButtonGameZaJinHuaControlName,  szButtonGameZaJinHuaImageName );
	m_ButtonImageMap[GID_DEZHOU]	= new CGameButtonImage( GID_DEZHOU,	   szButtonGameDeZhouControlName,	 szButtonGameDeZhouImageName   );
	m_ButtonImageMap[GID_SPARROWXZ]	= new CGameButtonImage( GID_SPARROWXZ, szButtonGameSparrowXZControlName, szButtonGameSparrowXZImageName);
	
	return;
}

//析构函数
CPlazaViewGame::~CPlazaViewGame()
{
	WORD wKey=0;
	
	//删除按钮集
	CGameButtonImage * pGameTypeItem=NULL;
	POSITION Position= m_ButtonImageMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_ButtonImageMap.GetNextAssoc(Position, wKey, pGameTypeItem);
		SafeDelete(pGameTypeItem);
	}
	m_ButtonImageMap.RemoveAll();

	return;
}


//类型点击
void CPlazaViewGame::OnViewPageKindItemClicked(tagGameKindInfo * pGameKindInfo)
{
	return;
}

//房间点击
void CPlazaViewGame::OnViewPageServerItemClicked(tagGameServerInfo * pGameServerInfo)
{
	return;
}

//比赛点击
void CPlazaViewGame::OnViewPageMatchItemClicked(tagGameMatchInfo * pGameMatchInfo,WORD wCommandID)
{
	
}


//下载完成
VOID CPlazaViewGame::OnEventDownLoadTaskFinish()
{
	//更新资源

	/*
	UpdateServerViewItem();

	//获取标识
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	WORD wServerID = CGlobalUnits::GetInstance()->GetTrackServerID();
	if(wServerID != 0)
	{
		//变量定义
		tagGameServerInfo GameServerInfo;
		ZeroMemory(&GameServerInfo,sizeof(GameServerInfo));

		//构造结构
		GameServerInfo.wServerID=wServerID;
		OnViewPageServerItemClicked(&GameServerInfo);
	}
	*/
}

//检测完成
VOID CPlazaViewGame::OnEventUpdateCheckFinish(bool bNeedUpdate)
{	
	/*
	if(bNeedUpdate == false)
	{
		//获取标识
		ASSERT(CGlobalUnits::GetInstance()!=NULL);
		WORD wServerID = CGlobalUnits::GetInstance()->GetTrackServerID();
		if(wServerID != 0)
		{
			//变量定义
			tagGameServerInfo GameServerInfo;
			ZeroMemory(&GameServerInfo,sizeof(GameServerInfo));

			//构造结构
			GameServerInfo.wServerID=wServerID;
			OnViewPageServerItemClicked(&GameServerInfo);
		}
	}
	*/
}


//消息提醒
void CPlazaViewGame::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	if (pPlatformFrame==NULL) return;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		WORD wParam = 0;
		if(lstrcmp(pControlUI->GetName(), szButtonGameBaccaratControlName)==0) {
			wParam = GID_BACCARAT;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonGameFiveCardControlName)==0) {
			wParam = GID_FIVECARD;
		}
		else  if(lstrcmp(pControlUI->GetName(), szButtonGameLandControlName)==0) {
			wParam = GID_LAND;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonGameOxBattleControlName)==0) {
			wParam = GID_OXBATTLE;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonGameOxNewControlName)==0) {
			wParam = GID_OXNEW;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonGameZaJinHuaControlName)==0) {
			wParam = GID_ZAJINHUA;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonGameDeZhouControlName)==0) {
			wParam = GID_DEZHOU;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonGameSparrowXZControlName)==0) {
			wParam = GID_SPARROWXZ;
		}

		//查找对象
		CGameButtonImage * pGameButtonImage=NULL;
		if ( m_ButtonImageMap.Lookup(wParam, pGameButtonImage)==FALSE ) {
			return;
		}	
	
		//游戏进程
		if ( pGameButtonImage->dwVersion == 0L ) {
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox( TEXT("此游戏未下载或更新失败，请选择其他游戏！") );
			return;
		}

		pPlatformFrame->PostMessage( WM_ENTER_GAME, 0, wParam);
	}
	return;
}



//初始控件
void CPlazaViewGame::InitControlUI()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	m_PaintManager.AddFontAt(0,TEXT("宋体"), 14, false, false, false);
	m_PaintManager.AddFontAt(1,TEXT("黑体"), 16, false, false, false);

	//////////////////////////////////////////////////////////////////////////////////////////////
	CControlUI * pParent = static_cast<CControlUI *>( m_PaintManager.GetRoot() );
	if(pParent==NULL) return;

	//百家乐
	CButtonUI * pButtonBaccarat = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonGameBaccarat") );
	if( pButtonBaccarat != NULL )  {
		pButtonBaccarat->SetTag(1);
		pButtonBaccarat->SetFloat(true);
		pButtonBaccarat->SetPos(20,13);
		pButtonBaccarat->SetFixedWidth(175);
		pButtonBaccarat->SetFixedHeight(35);
		pButtonBaccarat->SetAttribute( TEXT("align"), TEXT("center") );
		pButtonBaccarat->SetStatusImage( TEXT("file='BT_GAME_BACCARAT_0' restype='PNG'") );
	}

	//五张牌
	CButtonUI * pButtonFiveCard = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonGameFiveCard") );
	if( pButtonFiveCard != NULL )  {
		pButtonFiveCard->SetTag(2);
		pButtonFiveCard->SetFloat(true);
		pButtonFiveCard->SetPos(200,13);
		pButtonFiveCard->SetFixedWidth(175);
		pButtonFiveCard->SetFixedHeight(35);
		pButtonFiveCard->SetAttribute( TEXT("align"), TEXT("center") );
		pButtonFiveCard->SetStatusImage( TEXT("file='BT_GAME_FIVECARD_0' restype='PNG'") );
	}

	//斗地主
	CButtonUI * pButtonLand = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonGameLand") );
	if( pButtonLand != NULL )  {
		pButtonLand->SetTag(1);
		pButtonLand->SetFloat(true);
		pButtonLand->SetPos(383,13);
		pButtonLand->SetFixedWidth(175);
		pButtonLand->SetFixedHeight(35);
		pButtonLand->SetAttribute( TEXT("align"), TEXT("center") );
		pButtonLand->SetStatusImage( TEXT("file='BT_GAME_LAND_0' restype='PNG'") );
	}

	//百人牛牛
	CButtonUI * pButtonOxBattle = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonGameOxBattle") );
	if( pButtonOxBattle != NULL )  {
		pButtonOxBattle->SetTag(1);
		pButtonOxBattle->SetFloat(true);
		pButtonOxBattle->SetPos(566,13);
		pButtonOxBattle->SetFixedWidth(175);
		pButtonOxBattle->SetFixedHeight(174);
		pButtonOxBattle->SetAttribute( TEXT("align"), TEXT("center") );
		pButtonOxBattle->SetStatusImage( TEXT("file='BT_GAME_OXBATTLE_0' restype='PNG'") );
	}

	//四人牛牛
	CButtonUI * pButtonOxNew = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonGameOxNew") );
	if( pButtonOxNew != NULL )  {
		pButtonOxNew->SetTag(1);
		pButtonOxNew->SetFloat(true);
		pButtonOxNew->SetPos(20,192);
		pButtonOxBattle->SetFixedWidth(175);
		pButtonOxBattle->SetFixedHeight(174);
		pButtonOxNew->SetAttribute( TEXT("align"), TEXT("center") );
		pButtonOxNew->SetStatusImage( TEXT("file='BT_GAME_OXNEW_0' restype='PNG'") );
	}

	//诈金花
	CButtonUI * pButtonZaJinHua = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonGameZaJinHua") );
	if( pButtonZaJinHua != NULL )  {
		pButtonZaJinHua->SetTag(1);
		pButtonZaJinHua->SetFloat(true);
		pButtonZaJinHua->SetPos(200,192);
		pButtonOxBattle->SetFixedWidth(175);
		pButtonOxBattle->SetFixedHeight(174);
		pButtonZaJinHua->SetAttribute( TEXT("align"), TEXT("center") );
		pButtonZaJinHua->SetStatusImage( TEXT("file='BT_GAME_ZAJINHUA_0' restype='PNG'") );
	}

	//德州扑克 
	CButtonUI * pButtonDeZhou = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonGameDeZhou") );
	if( pButtonDeZhou != NULL )  {
		pButtonDeZhou->SetTag(1);
		pButtonDeZhou->SetFloat(true);
		pButtonDeZhou->SetPos(383,192);
		pButtonOxBattle->SetFixedWidth(175);
		pButtonOxBattle->SetFixedHeight(174);
		pButtonDeZhou->SetAttribute( TEXT("align"), TEXT("center") );
		pButtonDeZhou->SetStatusImage( TEXT("file='BT_GAME_DEZHOU_0' restype='PNG'") );
	}

	//血战麻将
	CButtonUI * pButtonSparrowXZ = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonGameSparrowXZ") );
	if( pButtonSparrowXZ != NULL )  {
		pButtonSparrowXZ->SetTag(1);
		pButtonSparrowXZ->SetFloat(true);
		pButtonSparrowXZ->SetPos(566,192);
		pButtonOxBattle->SetFixedWidth(175);
		pButtonOxBattle->SetFixedHeight(174);
		pButtonSparrowXZ->SetAttribute( TEXT("align"), TEXT("center") );
		pButtonSparrowXZ->SetStatusImage( TEXT("file='BT_GAME_SPARROWXZ_0' restype='PNG'") );
	}
}


//设置属性
void CPlazaViewGame::SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue)
{
	//变量定义
	return;
}


//结束绘画
void CPlazaViewGame::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画背景
	m_ImageBack.DrawImage(pDC,0,0);
}

//上页点击
LRESULT CPlazaViewGame::OnClickedLeftPageView( WPARAM wParam, LPARAM lParam )
{
	//状态校验
	return 0;
}

//下页点击
LRESULT CPlazaViewGame::OnClickedRightPageView( WPARAM wParam, LPARAM lParam )
{
	//状态校验
	return 0;
}


VOID CPlazaViewGame::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
	return;
}

//位置消息
VOID CPlazaViewGame::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	return;
}

//创建消息
INT CPlazaViewGame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	//加载资源
	//CPngImage ImageBack;
	m_ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("CONTENT_BK"));

	//设置大小
	CSize SizeWindow(m_ImageBack.GetWidth(),m_ImageBack.GetHeight());
	SetWindowPos(NULL, 0, 0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	return 0;
}

// 更新游戏
VOID CPlazaViewGame::InsertGameItem(WORD wGameID, DWORD dwProcessVersion)
{
	//查找对象
	CGameButtonImage * pGameButtonImage=NULL;
	if ( m_ButtonImageMap.Lookup(wGameID, pGameButtonImage)==FALSE ) {
		return;
	}	
	
	//游戏进程
	pGameButtonImage->dwVersion = 0L;
	if ( dwProcessVersion==0L ) {
		return;
	}

	//进程版本
	pGameButtonImage->dwVersion = dwProcessVersion;

	//点亮按钮
	if ( wGameID==pGameButtonImage->wGameID ) {
		CButtonUI * pButton= (CButtonUI * )GetControlByName(pGameButtonImage->wButtonName);
		if(pButton!=NULL) {
				pButton->SetStatusImage( pGameButtonImage->wButtonImage );
		}
	}
}

// 房间列表
VOID CPlazaViewGame::InsertServerItem( CGameServerItem * pGameServerItem )
{
	if ( pGameServerItem==0 )	return;
	m_GameServerArray.Add( pGameServerItem );
}

// 更新房间
VOID CPlazaViewGame::UpdateServerItem( WORD wKindID )
{
	//框架指针
	CPlatformFrame * pPlatformFrame = NULL;
	pPlatformFrame = CPlatformFrame::GetInstance();
	if (pPlatformFrame==NULL) return;
	
	//框架房间
	CPlazaViewServer * pPlazaViewServer = NULL;
	pPlazaViewServer = static_cast<CPlazaViewServer *> (&pPlatformFrame->m_PlazaViewServer);
	if (pPlazaViewServer==NULL) return;

	//服务列表
	WORD wCount = m_GameServerArray.GetCount();
	if (wCount==0) return;

	//清理列表
	pPlazaViewServer->RemoveServerItem();

	//更新列表
	for(int i=0; i<wCount; i++) 
	{
		CGameServerItem * pGameServerItem = NULL;
		pGameServerItem = m_GameServerArray[i];
		if (pGameServerItem!=NULL) 
		{
			if (pGameServerItem->m_GameServer.wKindID==wKindID) 
			{
				pPlazaViewServer->InsertServerItem( pGameServerItem );
			}
		}
	}

}