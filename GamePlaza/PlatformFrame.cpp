#include "Stdafx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "PlatformFrame.h"
#include "DlgEnquire.h"

//////////////////////////////////////////////////////////////////////////////////

#define LAYERED_SIZE				5									//分层大小

//////////////////////////////////////////////////////////////////////////////////

//屏幕限制
#define LESS_LIST_CX				285									//列表宽度
#define SPLITTER_WIDTH				8									//拆分宽度

//屏幕限制
#define LESS_SCREEN_CY				740									//最小高度
#define LESS_SCREEN_CX				1024								//最小宽度

//////////////////////////////////////////////////////////////////////////////////

//控件标识
#define IDC_SERVER_LIST				300									//游戏列表
#define IDC_WEB_PUBLICIZE			301									//浏览控件
#define IDC_SKIN_SPLITTER			302									//拆分控件
#define IDC_SYSTEM_TRAY_ICON		303									//任务图标
#define IDC_PLAZA_VIEW_GAME_TYPE	304									//类型控件
#define IDC_PLAZA_VIEW_GAME			305									//游戏视图
#define IDC_PLAZA_GAME_CONTAINER	306									//游戏容器
#define IDC_PLAZA_SERVER_CONTAINER	307									//房间容器

//控件标识
#define IDC_PLAZA_VIEW_AFFICHE		310									//公告面板
#define IDC_PLAZA_VIEW_USER			311									//用户信息
#define IDC_PLAZA_VIEW_SERVER		312									//房间控制
#define IDC_PLAZA_VIEW_ENTRY		313									//登入房间
#define IDC_PLAZA_VIEW_NEWS			314									//新闻信息
#define IDC_PLAZA_VIEW_NOTICE		315									//公告信息

//////////////////////////////////////////////////////////////////////////////////

//房间数目
#define MAX_SERVER_COUNT			5									//房间数目

//时间标识
#define IDI_FLASH_TRAY_ICON			10									//闪动图标

//时间标识
#define IDI_UPDATA_ONLINE			16									//更新人数
#define TIME_UPDATA_ONLINE			10000								//更新人数

//////////////////////////////////////////////////////////////////////////////////

//消息命令
#define WM_TASK_BAR					0x0313								//任务栏右键
#define WM_INSTALL_CANCEL_EVENT		8888								//取消安装

//////////////////////////////////////////////////////////////////////////////////

//按钮控件
const TCHAR* const szButtonMinControlName		  = TEXT("ButtonMin");
const TCHAR* const szButtonCloseControlName		  = TEXT("ButtonClose");
const TCHAR* const szButtonNavigation1ControlName = TEXT("ButtonNavigation1");
const TCHAR* const szButtonNavigation2ControlName = TEXT("ButtonNavigation2");
const TCHAR* const szButtonNavigation3ControlName = TEXT("ButtonNavigation3");
const TCHAR* const szButtonNavigation4ControlName = TEXT("ButtonNavigation4");
const TCHAR* const szButtonNavigation5ControlName = TEXT("ButtonNavigation5");
const TCHAR* const szButtonNavigation6ControlName = TEXT("ButtonNavigation6");
const TCHAR* const szButtonNavigation7ControlName = TEXT("ButtonNavigation7");
const TCHAR* const szButtonFrameBannerControlName = TEXT("ButtonFrameBanner");

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CPlatformFrame * CPlatformFrame::m_pPlatformFrame=NULL;					//框架指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlatformFrame, CFGuiFrameWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()

	//自定消息
	ON_MESSAGE(WM_HOTKEY, OnMessageHotKey)
	ON_MESSAGE(WM_TASK_BAR, OnMessageTaskBar)	
	ON_MESSAGE(WM_TRAY_ICON, OnMessageTrayIcon)
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)
	ON_MESSAGE(WM_INSTALL_CANCEL_EVENT, OnMessageInstallCancelEvent)

	//自定消息
	ON_MESSAGE(WM_INSUREPLAZA_EVENT, OnMessageInsurePlazaEvent)
	
	//自定消息
	ON_MESSAGE(WM_ENTER_GAME, OnMessageEnterGame)
	ON_MESSAGE(WM_QUIT_GAME, OnMessageQuitGame)
	ON_MESSAGE(WM_ENTER_SERVER, OnMessageEnterServer)
	ON_MESSAGE(WM_LOGON_SERVER, OnMessageLogonServer)
	ON_MESSAGE(WM_LOGON_SUCCESS, OnMessageLogonSuccess)
	ON_MESSAGE(WM_QUIT_SERVER, OnMessageQuitServer)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlatformFrame::CPlatformFrame()
{
	//任务组件
	m_MissionManager.InsertMissionItem(&m_MissionLogon);
	m_MissionManager.InsertMissionItem(&m_MissionUpdate);
	m_MissionManager.InsertMissionItem(&m_MissionList);

	//平台变量
	ASSERT(m_pPlatformFrame==NULL);
	if (m_pPlatformFrame==NULL) m_pPlatformFrame=this;

	m_bServerStatu = false;
	return;
}


//析构函数
CPlatformFrame::~CPlatformFrame()
{
	//平台变量
	ASSERT( m_pPlatformFrame==this );

	if ( m_pPlatformFrame==this ) {
		m_pPlatformFrame=NULL;
	}

	return;
}

//接口查询
VOID * CPlatformFrame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICustomFaceEvent,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICustomFaceEvent,Guid,dwQueryVer);
	return NULL;
}

//初始控件
void CPlatformFrame::InitControlUI()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	m_PaintManager.SetCaptionRect( CRect(0,0,0,38) );
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	CControlUI * pParent = static_cast<CControlUI *>( m_PaintManager.GetRoot() );
	if(pParent==NULL) return;

	//LOGO对象
	CLabelUI * pFrameLogo = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelLogo") );
	if( pFrameLogo != NULL )  {
		pFrameLogo->SetFloat(true);
		pFrameLogo->SetPos(28,13);
		pFrameLogo->SetFixedWidth(187);
		pFrameLogo->SetFixedHeight(67);
		pFrameLogo->SetBkImage( TEXT("file='PLATFORM_LOGO' restype='PNG'") );
	}

	//新闻背景
	CLabelUI * pFrameNewsBK = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("PlatformFrameNewsBK") );
	if( pFrameNewsBK != NULL )  {
		pFrameNewsBK->SetFloat(true);
		pFrameNewsBK->SetPos(10,395);
		pFrameNewsBK->SetFixedWidth(229);
		pFrameNewsBK->SetFixedHeight(230);
		pFrameNewsBK->SetBkImage( TEXT("file='PF_NEWS_BK' restype='PNG'") );
	}

	//新闻标题
	CLabelUI * pFrameNewsHOM = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("PlatformFrameNewsHOM") );
	if( pFrameNewsHOM != NULL )  {
		pFrameNewsHOM->SetFloat(true);
		pFrameNewsHOM->SetPos(270,125);
		pFrameNewsHOM->SetFixedWidth(50);
		pFrameNewsHOM->SetFixedHeight(17);
		pFrameNewsHOM->SetBkImage( TEXT("file='BT_NEWS_HOM' restype='PNG'") );
	}

	//Navigation1
	CLabelUI * pLabNavigation1 = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelNavigation1") );
	if( pLabNavigation1 != NULL )  {
		pLabNavigation1->SetFloat(true);
		pLabNavigation1->SetPos(267,83);		
		pLabNavigation1->SetFixedWidth(71);
		pLabNavigation1->SetFixedHeight(20);
		pLabNavigation1->SetBkImage( TEXT("file='LB_NAVIGATION_1' restype='PNG'") );
	}

	//Navigation2
	CLabelUI * pLabNavigation2 = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelNavigation2") );
	if( pLabNavigation2 != NULL )  {
		pLabNavigation2->SetFloat(true);
		pLabNavigation2->SetPos(367,83);
		pLabNavigation2->SetFixedWidth(71);
		pLabNavigation2->SetFixedHeight(20);
		pLabNavigation2->SetBkImage( TEXT("file='LB_NAVIGATION_2' restype='PNG'") );
	}

	//Navigation3
	CLabelUI * pLabNavigation3 = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelNavigation3") );
	if( pLabNavigation3 != NULL )  {
		pLabNavigation3->SetFloat(true);
		pLabNavigation3->SetPos(467,83);
		pLabNavigation3->SetFixedWidth(71);
		pLabNavigation3->SetFixedHeight(20);
		pLabNavigation3->SetBkImage( TEXT("file='LB_NAVIGATION_3' restype='PNG'") );
	}

	//Navigation4
	CLabelUI * pLabNavigation4 = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelNavigation4") );
	if( pLabNavigation4 != NULL )  {
		pLabNavigation4->SetFloat(true);
		pLabNavigation4->SetPos(567,83);
		pLabNavigation4->SetFixedWidth(71);
		pLabNavigation4->SetFixedHeight(20);
		pLabNavigation4->SetBkImage( TEXT("file='LB_NAVIGATION_4' restype='PNG'") );
	}

	//Navigation5
	CLabelUI * pLabNavigation5 = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelNavigation5") );
	if( pLabNavigation5 != NULL )  {
		pLabNavigation5->SetFloat(true);
		pLabNavigation5->SetPos(667,83);
		pLabNavigation5->SetFixedWidth(71);
		pLabNavigation5->SetFixedHeight(20);
		pLabNavigation5->SetBkImage( TEXT("file='LB_NAVIGATION_5' restype='PNG'") );
	}

	//Navigation6
	CLabelUI * pLabNavigation6 = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelNavigation6") );
	if( pLabNavigation6 != NULL )  {
		pLabNavigation6->SetFloat(true);
		pLabNavigation6->SetPos(767,83);
		pLabNavigation6->SetFixedWidth(71);
		pLabNavigation6->SetFixedHeight(20);
		pLabNavigation6->SetBkImage( TEXT("file='LB_NAVIGATION_6' restype='PNG'") );
	}

	//Navigation7
	CLabelUI * pLabNavigation7 = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelNavigation1") );
	if( pLabNavigation7 != NULL )  {
		pLabNavigation7->SetFloat(true);
		pLabNavigation7->SetPos(867,83);
		pLabNavigation7->SetFixedWidth(71);
		pLabNavigation7->SetFixedHeight(20);
		pLabNavigation7->SetBkImage( TEXT("file='LB_NAVIGATION_7' restype='PNG'") );
	}

	//最小化按钮
	CButtonUI * pButtonMin = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonMin") );
	if( pButtonMin != NULL )  {
		pButtonMin->SetFloat(true);
		pButtonMin->SetPos(940,8);
		pButtonMin->SetFixedWidth(30);
		pButtonMin->SetFixedHeight(35);
		pButtonMin->SetStatusImage( TEXT("file='BT_LOGON_MIN' restype='PNG'") );
	}

	//关闭按钮
	CButtonUI * pButtonClose = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonClose") );
	if( pButtonClose != NULL )  {
		pButtonClose->SetFloat(true);
		pButtonClose->SetPos(978,8);
		pButtonClose->SetFixedWidth(30);
		pButtonClose->SetFixedHeight(35);
		pButtonClose->SetStatusImage( TEXT("file='BT_LOGON_CLOSE' restype='PNG'") );
	}

	
	//Navigation1
	CButtonUI * pBtnNavigation1 = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonNavigation1") );
	if( pBtnNavigation1 != NULL )  {
		pBtnNavigation1->SetTag(1);
		pBtnNavigation1->SetFloat(true);
		pBtnNavigation1->SetPos(274,33);		
		pBtnNavigation1->SetFixedWidth(310);
		pBtnNavigation1->SetFixedHeight(50);
		pBtnNavigation1->SetStatusImage( TEXT("file='BT_NAVIGATION_1' restype='PNG'") );
	}

	//Navigation2
	CButtonUI * pBtnNavigation2 = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonNavigation2") );
	if( pBtnNavigation2 != NULL )  {
		pBtnNavigation2->SetTag(2);
		pBtnNavigation2->SetFloat(true);
		pBtnNavigation2->SetPos(374,33);
		pBtnNavigation2->SetFixedWidth(310);
		pBtnNavigation2->SetFixedHeight(50);
		pBtnNavigation2->SetStatusImage( TEXT("file='BT_NAVIGATION_2' restype='PNG'") );
	}

	//Navigation3
	CButtonUI * pBtnNavigation3 = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonNavigation3") );
	if( pBtnNavigation3 != NULL )  {
		pBtnNavigation3->SetTag(3);
		pBtnNavigation3->SetFloat(true);
		pBtnNavigation3->SetPos(474,33);
		pBtnNavigation3->SetFixedWidth(310);
		pBtnNavigation3->SetFixedHeight(50);
		pBtnNavigation3->SetStatusImage( TEXT("file='BT_NAVIGATION_3' restype='PNG'") );
	}

	//Navigation4
	CButtonUI * pBtnNavigation4 = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonNavigation4") );
	if( pBtnNavigation4 != NULL )  {
		pBtnNavigation4->SetTag(4);
		pBtnNavigation4->SetFloat(true);
		pBtnNavigation4->SetPos(574,33);
		pBtnNavigation4->SetFixedWidth(310);
		pBtnNavigation4->SetFixedHeight(50);
		pBtnNavigation4->SetStatusImage( TEXT("file='BT_NAVIGATION_4' restype='PNG'") );
	}

	//Navigation5
	CButtonUI * pBtnNavigation5 = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonNavigation5") );
	if( pBtnNavigation5 != NULL )  {
		pBtnNavigation5->SetTag(5);
		pBtnNavigation5->SetFloat(true);
		pBtnNavigation5->SetPos(674,33);
		pBtnNavigation5->SetFixedWidth(310);
		pBtnNavigation5->SetFixedHeight(50);
		pBtnNavigation5->SetStatusImage( TEXT("file='BT_NAVIGATION_5' restype='PNG'") );
	}

	//Navigation6
	CButtonUI * pBtnNavigation6 = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonNavigation6") );
	if( pBtnNavigation6 != NULL )  {
		pBtnNavigation6->SetTag(6);
		pBtnNavigation6->SetFloat(true);
		pBtnNavigation6->SetPos(774,33);
		pBtnNavigation6->SetFixedWidth(310);
		pBtnNavigation6->SetFixedHeight(50);
		pBtnNavigation6->SetStatusImage( TEXT("file='BT_NAVIGATION_6' restype='PNG'") );
	}

	//Navigation7
	CButtonUI * pBtnNavigation7 = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonNavigation7") );
	if( pBtnNavigation7 != NULL )  {
		pBtnNavigation7->SetTag(7);
		pBtnNavigation7->SetFloat(true);
		pBtnNavigation7->SetPos(874,33);
		pBtnNavigation7->SetFixedWidth(310);
		pBtnNavigation7->SetFixedHeight(50);
		pBtnNavigation7->SetStatusImage( TEXT("file='BT_NAVIGATION_7' restype='PNG'") );
	}

	//平衡按钮
	CLabelUI * pLblFrameBanner = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("ButtonFrameBanner") );
	if( pLblFrameBanner != NULL )  {
		pLblFrameBanner->SetTag(7);
		pLblFrameBanner->SetFloat(true);
		pLblFrameBanner->SetPos(10,632);
		pLblFrameBanner->SetFixedWidth(225);
		pLblFrameBanner->SetFixedHeight(98);
		pLblFrameBanner->SetBkImage( TEXT("file='BT_BANNER' restype='PNG'") );
	}

	//加载资源
	m_ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("PLATFORM_BK"));
}

//消息提醒
void CPlatformFrame::Notify(TNotifyUI &  msg)
{	
	//获取对象
	/////////////////////////////////////////////////////////////////////////////
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0) 
		{
			PostMessage(WM_CLOSE,0,0);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMinControlName)==0)
		{
			ShowWindow(SW_MINIMIZE);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonFrameBannerControlName)==0)
		{
			//获取对象
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
			
			//构造地址
			TCHAR szBillUrl[256]=TEXT("");
			ZeroMemory(szBillUrl,CountArray(szBillUrl));
			_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/PlatformBanner.aspx"),pGlobalWebLink->GetPlatformLink());
			
			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szBillUrl,NULL,NULL,SW_NORMAL);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonNavigation1ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonNavigation2ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonNavigation3ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonNavigation4ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonNavigation5ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonNavigation6ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonNavigation7ControlName)==0)
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//获取对象
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//构造地址
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sNavigation%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(),pControlUI->GetTag());

			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);

			return;
		}
	}
}


//结束绘画
void CPlatformFrame::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画背景
	m_ImageBack.DrawImage(pDC,0,0);
}


//下载失败
VOID CPlatformFrame::OnEventSystemFace(DWORD dwUserID, WORD wFaceID)
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//自我数据
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//设置变量
		pGlobalUserData->wFaceID=wFaceID;

		//清理数据
		pGlobalUserData->dwCustomID=0L;
		ZeroMemory(&pGlobalUserData->CustomFaceInfo,sizeof(pGlobalUserData->CustomFaceInfo));

		//发送事件
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	return;
}

//服务判断
bool CPlatformFrame::IsEnteredServerItem()
{
	return m_bServerStatu;
}

//用户更新
VOID CPlatformFrame::UpdateUserInfo()
{
	m_PlazaViewUser.PostMessage(WM_UPDATE_USERINFO, 0, 0);
}

//头像数据
VOID CPlatformFrame::OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//自我数据
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//设置变量
		pGlobalUserData->dwCustomID=dwCustomID;
		CopyMemory(&pGlobalUserData->CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

		//发送事件
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	//当前房间

	/* if (m_PlazaViewServer.m_hWnd!=NULL)
	{
		m_PlazaViewServer.UpdateUserCustomFace(dwUserID,dwCustomID,CustomFaceInfo);
	} */

	return;
}

void CPlatformFrame::SetVideoOption(WORD wAVServerPort, DWORD dwAVServerAddr)
{
	m_wAVServerPort=wAVServerPort;
	m_dwAVServerAddr=dwAVServerAddr;
}


//获取通知
VOID CPlatformFrame::OnGameItemFinish()
{
	//显示列表
	// m_PlazaViewGame.ShowKindItemView(m_PlazaViewGameType.GetCurrActiveIndex(),FALSE);
	return;
}

//完成通知
VOID CPlatformFrame::OnGameMatchFinish()
{
	//显示比赛
	/* if(m_PlazaViewGame.GetShowItemMode()==VIEW_MODE_MATCH) {
		m_PlazaViewGame.ShowMatchItemView(FALSE);
	} */

	return;
}

//获取通知
VOID CPlatformFrame::OnGameKindFinish(WORD wKindID)
{
	//显示房间
	if(wKindID!=INVALID_WORD) {
		// m_PlazaViewGame.ShowServerItemView(wKindID,TRUE);
	}

	return;
}

//插入通知
VOID CPlatformFrame::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//效验参数
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//更新时间
	pGameListItem->m_dwUpdateTime=(DWORD)time(NULL);

	//插入处理
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//游戏种类
		{
			break;
		}
	case ItemGenre_Kind:	//游戏类型
		{
			//获取对象
			CGameKindItem * pGameKindItem = (CGameKindItem *)pGameListItem;
			m_PlazaViewGame.InsertGameItem( pGameKindItem->m_GameKind.wKindID, pGameKindItem->m_dwProcessVersion);
			break;
		}
	case ItemGenre_Server:	//游戏房间
		{
			CGameServerItem * pGameServerItem = (CGameServerItem *)pGameListItem;
			m_PlazaViewGame.InsertServerItem( pGameServerItem );
			break;
		}
	}

	return;
}

//更新通知
VOID CPlatformFrame::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	//更新时间
	pGameListItem->m_dwUpdateTime=(DWORD)time(NULL);

	switch(pGameListItem->GetItemGenre())
	{		
	case ItemGenre_Type:	//类型子项
		{
			break;
		}
	case ItemGenre_Kind:	//游戏子项
		{
			//获取对象
			// CGameKindItem * pGameKindItem = (CGameKindItem *)pGameListItem;
			// m_PlazaViewGame.UpdateGameItem( pGameKindItem->m_GameKind.wKindID, pGameKindItem->m_dwProcessVersion);
			break;
		}
	case ItemGenre_Server:	//房间子项
		{
			break;
		}
	}

	return;
}

//删除通知
VOID CPlatformFrame::OnGameItemDelete(CGameListItem * pGameListItem)
{

	return;
}


//消息过虑
BOOL CPlatformFrame::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if (pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_RETURN) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CPlatformFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//菜单命令
	switch (nCommandID)
	{
	case IDM_USER_LOGON:		//用户登录
		{
			m_MissionLogon.ShowLogon();

			return TRUE;
		}
	case IDM_DELETE_SERVER_ITEM:	//关闭服务
		{
			m_PlazaViewEntry.PostMessage(WM_DELETESERVERITEM,0,0);

			return TRUE;
		}
	case IDM_SWITCH_ACCOUNTS:	//切换帐号
		{
			//切换询问
			if (m_PlazaViewServer.m_hWnd!=NULL)
			{
				CInformation Information(this);
				if (Information.ShowMessageBox(TEXT("游戏房间即将关闭，确定要“切换帐号”吗？ "),MB_YESNO)!=IDYES) 
					return TRUE;
			}

			//全局设置
			//CGlobalUnits::GetInstance()->DeleteUserCookie();
			CGlobalUserInfo::GetInstance()->ResetUserInfoData();

			//获取对象
			CServerListData * pServerListData = CServerListData::GetInstance();

			//清除报名
			POSITION Position=NULL;
			do
			{
				//获取房间
				CGameServerItem * pGameServerItem=pServerListData->EmunGameServerItem(Position);
				if(pGameServerItem!=NULL)
				{
					pGameServerItem->m_bSignuped=false;
					ZeroMemory(&pGameServerItem->m_GameMatch,sizeof(pGameServerItem->m_GameMatch));
				}

			} while (Position!=NULL);

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) 
				pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGOUT,0L);

			return TRUE;
		}
	case IDM_CLOSE:		//关闭
		{
			//投递消息
			PostMessage(WM_CLOSE,0,0);

			return TRUE;
		}
	case IDM_RESTORE:   //还原
		{
			if(IsIconic())
			{
				m_bServerStatu = false;
				ShowWindow(SW_RESTORE);
			}
			return TRUE;
		}
	case IDM_MINIMIZE:  //最小化
		{
			if(IsWindowVisible())
			{
				m_bServerStatu = true;
				ShowWindow(SW_MINIMIZE);
			}
			return TRUE;
		}
	case IDM_MAXIMIZE:  //最大化
		{
			return TRUE;
		}
	case IDM_SHOW_LOGON: //显示登录
		{
			m_MissionLogon.ShowLogon(SW_RESTORE);

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//关闭消息
VOID CPlatformFrame::OnClose()
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//关闭提示
	if ((pGlobalUserData->dwUserID!=0L)&&(m_MissionLogon.GetActiveStatus()==false))
	{
		//显示窗口
		CDlgEnquire DlgEnquire;
		INT_PTR nResult=DlgEnquire.DoModal();

		//命令处理
		switch (nResult)
		{
		case IDCANCEL:				//取消动作
			{
				return;
			}
		case IDC_CLOSE_SERVER:		//退出房间
			{
				//关闭房间
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
				return;
			}
		case IDC_SWITCH_ACCOUNTS:	//切换帐号
			{
				//投递消息
				PostMessage(WM_COMMAND,IDM_SWITCH_ACCOUNTS,0);
				return;
			}
		}
	}

	//注销热键
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS);
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_WHISPER);

	__super::OnClose();
}

//时间消息
VOID CPlatformFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_FLASH_TRAY_ICON:	//任务图标
		{
			return;
		}
	}
	__super::OnTimer(nIDEvent);
}

//位置消息
VOID CPlatformFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	// RectifyControl(cx,cy);

	return;
}

//位置信息
VOID CPlatformFrame::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	//窗口位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//设置位置
	lpMMI->ptMinTrackSize.x=__min(LESS_SCREEN_CX,rcArce.Width());
	lpMMI->ptMinTrackSize.y=__min(LESS_SCREEN_CY,rcArce.Height());

	return;
}

//建立消息
INT CPlatformFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置窗口
	ModifyStyle(WS_CAPTION|WS_BORDER, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
	ModifyStyleEx(WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);	

	//变量定义
	CRect rcViewGame( PARENT_VIEWGAME_X, PARENT_VIEWGAME_Y, 0, 0);
	CRect rcUserInfo( PARENT_USERINFO_X, PARENT_USERINFO_Y, 0, 0);
	CRect rcViewServer( PARENT_VIEWSERVER_X, PARENT_VIEWSERVER_Y, 0, 0);
	CRect rcViewEntry( PARENT_VIEWENTRY_X, PARENT_VIEWENTRY_Y, 0, 0);
	CRect rcViewNews( PARENT_VIEWNEWS_X, PARENT_VIEWNEWS_Y, 0, 0);
	CRect rcViewNotice( PARENT_VIEWNOTICE_X, PARENT_VIEWNOTICE_Y, 0, 0);

	//视图控件
	m_PlazaViewUser.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcUserInfo,this,IDC_PLAZA_VIEW_USER);
	m_PlazaViewGame.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcViewGame,this,IDC_PLAZA_VIEW_GAME);
	m_PlazaViewServer.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcViewServer,this,IDC_PLAZA_VIEW_SERVER);
	m_PlazaViewEntry.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcViewEntry,this,IDC_PLAZA_VIEW_ENTRY);
	m_PlazaViewNews.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcViewNews,this,IDC_PLAZA_VIEW_NEWS);
	m_PlazaViewNotice.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcViewNotice,this,IDC_PLAZA_VIEW_NEWS);

	//视图隐藏
	m_PlazaViewServer.ShowWindow(SW_HIDE);
	m_PlazaViewEntry.ShowWindow(SW_HIDE);

	//游戏列表
	ASSERT(CServerListData::GetInstance()!=NULL);
	CServerListData::GetInstance()->SetServerListDataSink(this);
	
	//注册事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	//设置位置
	CSize SizeRestrict;
	SizeRestrict.SetSize(LESS_SCREEN_CX,LESS_SCREEN_CY);

	//窗口位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//读取位置
	CWHRegKey InfoKeyItem;
	if (InfoKeyItem.OpenRegKey(REG_INTERFACE_INFO,false)==true)
	{
		SizeRestrict.cx=InfoKeyItem.GetValue(TEXT("ScreenWidth"),rcArce.Width()/2);
		SizeRestrict.cy=InfoKeyItem.GetValue(TEXT("ScreenHeight"),rcArce.Height()/2);
	}

	//位置调整
	SizeRestrict.cx=__max(LESS_SCREEN_CX,SizeRestrict.cx);
	SizeRestrict.cy=__max(LESS_SCREEN_CY,SizeRestrict.cy);
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//移动窗口
	m_rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
	m_rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
	m_rcNormalSize.right=m_rcNormalSize.left+SizeRestrict.cx;
	m_rcNormalSize.bottom=m_rcNormalSize.top+SizeRestrict.cy;

	//移动窗口
	SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);

	//显示窗口
	SetWindowRgn(NULL,TRUE);

	//登录系统
	PostMessage(WM_COMMAND,IDM_USER_LOGON,0);

	//注册热键
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	CWHService::RegisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS,pParameterGlobal->m_wBossHotKey);
	CWHService::RegisterHotKey(m_hWnd,IDI_HOT_KEY_WHISPER,pParameterGlobal->m_wWhisperHotKey);

	//设置托盘
	// m_SystemTrayIcon.InitTrayIcon(m_hWnd,IDC_SYSTEM_TRAY_ICON);
	// m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)),szProduct);

	//更新人数
	SetTimer(IDI_UPDATA_ONLINE, TIME_UPDATA_ONLINE, NULL);

	//设置字体
	CSkinDialog::SetWndFont(this,NULL);

	return 0;
}

//鼠标消息
VOID CPlatformFrame::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//状态判断
	if (Point.y>GetCaptionRect().Height()) return;

	//窗口控制
	/*　if (m_bMaxShow==true)
	{
		RestoreWindow();
	}
	else
	{
		MaxSizeWindow();
	}　*/
	
	return;
}

//设置改变
VOID CPlatformFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	//窗口控制
	/*　if (m_bMaxShow==true)
	{
		m_bMaxShow=false;
		MaxSizeWindow(false);
	}
	else
	{
		m_bMaxShow=true;
		RestoreWindow();
	}　*/

	return;
}

//热键消息
LRESULT CPlatformFrame::OnMessageHotKey(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDI_HOT_KEY_BOSS:		//老板热键
		{
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//登录判断
			if (pGlobalUserData->dwUserID!=0L)
			{
				//变量定义
				bool bBossCome=(IsWindowVisible()==FALSE)?false:true;

				//隐藏窗口
				if (bBossCome==false)
				{
					//还原窗口
					ShowWindow(SW_RESTORE);
					ShowWindow(SW_SHOW);

					//置顶窗口
					SetActiveWindow();
					BringWindowToTop();
					SetForegroundWindow();

					//显示图标
					// m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
				}
				else
				{
					//隐藏窗口
					ShowWindow(SW_MINIMIZE);
					ShowWindow(SW_HIDE);

					//隐藏图标
					// m_SystemTrayIcon.HideTrayIcon();
				}

				////发送消息
				//for (INT_PTR i=0;i<m_PlazaViewServerArray.GetCount();i++)
				//{
				//	ASSERT(m_PlazaViewServerArray[i]!=NULL);
				//	m_PlazaViewServerArray[i]->NotifyBossCome(bBossCome);
				//}
			}

			return 0;
		}
	case IDI_HOT_KEY_WHISPER:	//私聊热键
		{
			//提取消息
			/*　if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			}　*/

			return 0;
		}
	}

	return 0;
}

//任务消息
LRESULT CPlatformFrame::OnMessageTaskBar(WPARAM wParam, LPARAM lParam)
{
	//创建菜单
	CSkinMenu Menu;
	Menu.CreateMenu();

	//构造菜单
	Menu.AppendMenu(IDM_RESTORE,TEXT("还原"), (IsIconic()||IsWindowVisible()==FALSE)?MF_ENABLED:MF_GRAYED);
	Menu.AppendMenu(IDM_MINIMIZE,TEXT("最小化"),(IsIconic()==FALSE||IsWindowVisible())?MF_ENABLED:MF_GRAYED);
	Menu.AppendMenu(IDM_MAXIMIZE,TEXT("最大化"));
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_CLOSE,TEXT("退出广场"));

	//显示菜单
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);

	return 0L;
}

//图标消息
LRESULT CPlatformFrame::OnMessageTrayIcon(WPARAM wParam, LPARAM lParam)
{
	//事件处理
	switch (lParam)
	{
	case WM_LBUTTONDOWN:		//鼠标单击
		{
			if(IsIconic())
			{
				ShowWindow(SW_RESTORE);
			}

			//激活窗口
			SetActiveWindow();
			BringWindowToTop();
			SetForegroundWindow();

			return 1;
		}
	case WM_LBUTTONDBLCLK:		//鼠标双击
		{
			//提取消息
			
			/* if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			} */

			return 1;
		}
	case WM_RBUTTONDOWN:		//鼠标单击
	case WM_RBUTTONDBLCLK:		//鼠标双击
		{
			//创建菜单
			CSkinMenu Menu;
			Menu.CreateMenu();

			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//用户未登陆
			if(pGlobalUserData->dwUserID == 0)
			{
				Menu.AppendMenu(IDM_SHOW_LOGON, TEXT("显示主面板"), MF_ENABLED);
				Menu.AppendMenu(IDM_CLOSE, TEXT("退出广场"), MF_ENABLED);
			}
			else
			{
				//构造菜单
				Menu.AppendMenu(IDM_RESTORE,TEXT("还原"), (IsIconic()||IsWindowVisible()==FALSE)?MF_ENABLED:MF_GRAYED);
				Menu.AppendMenu(IDM_MINIMIZE,TEXT("最小化"),(IsIconic()==FALSE||IsWindowVisible())?MF_ENABLED:MF_GRAYED);
				Menu.AppendMenu(IDM_MAXIMIZE,TEXT("最大化"));
				Menu.AppendSeparator();
				Menu.AppendMenu(IDM_CLOSE,TEXT("退出广场"));
			}			
			
			//Menu.AppendMenu(IDM_NULL_COMMAND,TEXT("关于..."));

			//显示菜单
			CPoint MousePoint;
			GetCursorPos(&MousePoint);
			Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);

			return 1;
		}
	}

	return 0;
}

//事件消息
LRESULT CPlatformFrame::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	//事件处理
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//登录完成
		{
			//加载信息
			m_MissionUpdate.PerformUpdateMission(&m_PlazaViewNews, &m_PlazaViewNotice);
			
			//显示窗口
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
			return 0L;
		}
	case EVENT_USER_LOGOUT:			//注销成功
		{
			//隐藏窗口
			ShowWindow(SW_HIDE);

			//关闭房间

			//显示登录
			m_MissionLogon.ShowLogon();

			return 0L;
		}
	case EVENT_DOWN_LOAD_FINISH:	//下载完成
		{
			//获取参数
			WORD wKindID=LOWORD(lParam);
			WORD wServerID=HIWORD(lParam);

			//更新列表
			CServerListData * pServerListData=CServerListData::GetInstance();
			if (pServerListData!=NULL) pServerListData->OnEventDownLoadFinish(wKindID);

			return 0L;
		}
	}

	return 0L;
}

//事件消息
LRESULT CPlatformFrame::OnMessageInstallCancelEvent(WPARAM wParam, LPARAM lParam)
{
	//显示登录
	m_MissionLogon.ShowLogon();

	return 0L;
}

//事件消息
LRESULT CPlatformFrame::OnMessageInsurePlazaEvent(WPARAM wParam, LPARAM lParam)
{
	tagInsurePlazaEvent * pInsureEvent = (tagInsurePlazaEvent*)wParam;
	if(pInsureEvent==NULL) return 0L;

	//更新命令
	if(pInsureEvent->wCommandID==INSURE_COMMAND_UPDATE)
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//更新
		pGlobalUserData->lUserScore = pInsureEvent->lUserScore;
		pGlobalUserData->lUserInsure = pInsureEvent->lUserInsure;

		//更新信息
		UpdateUserInfo();
	}
	
	//兑换命令
	if(pInsureEvent->wCommandID==INSURE_COMMAND_EXCHANGE)
	{
		//显示兑换
		ASSERT(CGlobalUnits::GetInstance());
		// CGlobalUnits::GetInstance()->PerformShowExchange();		
	}

	//释放对象
	if(pInsureEvent->bHeapData==true) SafeDelete(pInsureEvent);

	return 0;
}


LRESULT CPlatformFrame::OnMessageEnterGame(WPARAM wParam, LPARAM lParam)
{
	WORD wKind = lParam;
	if (wKind==0L) return 0;

	//更新房间
	m_PlazaViewGame.UpdateServerItem(wKind);
	m_PlazaViewServer.UpdateServerItem(wKind);

	//更新房间
	m_PlazaViewGame.ShowWindow(SW_HIDE);
	m_PlazaViewServer.ShowWindow(SW_SHOW);
	return 0;
}

//退出游戏
LRESULT CPlatformFrame::OnMessageQuitGame(WPARAM wParam, LPARAM lParam)
{
	//移除房间
	m_PlazaViewServer.RemoveServerItem();
	
	//更新房间
	m_PlazaViewServer.ShowWindow(SW_HIDE);
	m_PlazaViewGame.ShowWindow(SW_SHOW);
	return 0;
}

//进入房间
LRESULT CPlatformFrame::OnMessageEnterServer(WPARAM wParam, LPARAM lParam)
{
	//登入游戏
	try
	{
		//清理房间
		m_PlazaViewEntry.Release();

		//创建视图
		m_PlazaViewEntry.CreateViewFrame();

		//初始游戏
		CGameServerItem * pGameServerItem = m_PlazaViewServer.GetUserChickServerItem();
		if (pGameServerItem==NULL) return 0;
		m_PlazaViewEntry.InitGameServer(pGameServerItem);
		
		//登入游戏厅
		m_PlazaViewEntry.PerformSitDownAction(INVALID_TABLE, INVALID_TABLE, false);
	}
	catch(...)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox( TEXT("登录游戏错误!") );

		return -1;
	}
	
	return 0;
}

//登录成功
LRESULT CPlatformFrame::OnMessageLogonSuccess(WPARAM wParam, LPARAM lParam)
{	
	//更新房间
	m_PlazaViewServer.ShowWindow(SW_HIDE);
	m_PlazaViewEntry.ShowWindow(SW_SHOW);
	return 0;
}

//分组登入
LRESULT CPlatformFrame::OnMessageLogonServer(WPARAM wParam, LPARAM lParam)
{
	try
	{
		// 参加分组
		m_PlazaViewEntry.PerformJoinGame();
	}
	catch(...)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox( TEXT("执行分组任务失败!") );
		return -1;
	}

	m_PlazaViewEntry.ResetViewFrame(true);

	return 0;
}

//退出分组
LRESULT CPlatformFrame::OnMessageQuitServer(WPARAM wParam, LPARAM lParam)
{
	m_PlazaViewEntry.Release();

	//更新房间
	m_PlazaViewEntry.ShowWindow(SW_HIDE);
	m_PlazaViewServer.ShowWindow(SW_SHOW);
	m_PlazaViewEntry.ResetViewFrame(false);
	return 0;
}

//删除房间
bool CPlatformFrame::DeleteServerItem()
{
	return true;
}