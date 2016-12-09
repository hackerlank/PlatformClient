#include "Stdafx.h"
#include "PlazaViewEntry.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "TableViewFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//控件标识
#define IDC_SERVER_TABLE			101									//桌子控件

//////////////////////////////////////////////////////////////////////////////////

//控件名称
const TCHAR* const szButtonGoBackControlName	= TEXT("ButtonEnter");
const TCHAR* const szButtonQuitControlName		= TEXT("ButtonQuit");
const TCHAR* const szLabelLoadingControlName	= TEXT("LabelLoading");

//////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CPlazaViewEntry, CFGuiWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////
CPlazaViewEntry::CPlazaViewEntry()
{
	//辅助变量
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;
	m_wResultTableID=INVALID_TABLE;
	m_wInvitedTableID=INVALID_TABLE;
	m_bUserInRoom=false;

	//用户属性
	m_dwUserRight=0L;
	m_dwMasterRight=0L;
	m_pIMySelfUserItem=NULL;

	//游戏桌子
	m_pITableViewFrame=NULL;

	//房间属性
	m_wServerType=0L;
	m_dwServerRule=0L;
	m_ServiceStatus=ServiceStatus_Unknow;
	m_wAVServerPort=0;
	m_dwAVServerAddr=0;
	m_szServerPassword[0]=0;

	//配置参数
	m_pParameterGame=NULL;
	m_pParameterServer=NULL;

	//设置组件
	m_DlgStatus.SetStatusViewSink(this);

	//配置信息
	m_cbColumnCount=0;
	m_cbPropertyCount=0;
	ZeroMemory(m_ColumnItem,sizeof(m_ColumnItem));
	ZeroMemory(m_PropertyInfo,sizeof(m_PropertyInfo));

	//房间属性
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));
}

//析构函数
CPlazaViewEntry::~CPlazaViewEntry()
{

}

VOID CPlazaViewEntry::Release()
{
	//视图重置
	if (m_pITableViewFrame) 
	{
		m_pITableViewFrame->Release();
		m_pITableViewFrame = NULL;
	}

	//组件重置
	m_GameLevelParserModule.CloseInstance();

	//用户管理
	m_PlazaUserManagerModule.CloseInstance();
	m_pIMySelfUserItem = NULL;

	//关闭连接器
	m_TCPSocketModule.CloseInstance();
}

//接口查询
VOID * CPlazaViewEntry::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(IUserManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE(IProcessManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerViewItem,Guid,dwQueryVer);
	return NULL;
}


void CPlazaViewEntry::InitControlUI()
{
	//获取对象
	//////////////////////////////////////////////////////////////////////////////////////////////
	m_PaintManager.AddFontAt(0,TEXT("宋体"), 14, false, false, false);
	m_PaintManager.AddFontAt(1,TEXT("黑体"), 16, false, false, false);

	//////////////////////////////////////////////////////////////////////////////////////////////
	CControlUI * pParent = static_cast<CControlUI *>( m_PaintManager.GetRoot() );
	if(pParent==NULL) return;

	// pLabelLoading->SetVisible(false);

	//登入游戏
	CButtonUI * pButtonEnter = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonEnter") );
	if( pButtonEnter != NULL )  {
		pButtonEnter->SetFloat(true);
		pButtonEnter->SetPos(175,360);		
		pButtonEnter->SetFixedWidth(355);
		pButtonEnter->SetFixedHeight(315);
		pButtonEnter->SetStatusImage( TEXT("file='BT_JOIN' restype='PNG'") );
	}

	//退出按钮
	CButtonUI * pButtonQuit = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonQuit") );
	if( pButtonQuit != NULL )  {
		pButtonQuit->SetFloat(true);
		pButtonQuit->SetPos(405,360);
		pButtonQuit->SetFixedWidth(585);
		pButtonQuit->SetFixedHeight(315);
		pButtonQuit->SetStatusImage( TEXT("file='BT_QUIT' restype='PNG'") );
	}

	//登录加载
	CButtonUI * pLabelLoading = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("LabelLoading") );
	if( pLabelLoading != NULL )  {
		pLabelLoading->SetFloat(true);
		pLabelLoading->SetPos(175,360);
		pLabelLoading->SetFixedWidth(555);
		pLabelLoading->SetFixedHeight(285);
		pLabelLoading->SetStatusImage( TEXT("file='LB_LOADING' restype='PNG'") );
	}

}

//消息提醒
void CPlazaViewEntry::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	if (pPlatformFrame==NULL) return;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonGoBackControlName )==0) {
			pPlatformFrame->PostMessage( WM_LOGON_SERVER, 0, 0);
			return;
		}
		if(lstrcmp(pControlUI->GetName(), szButtonQuitControlName )==0) {
			pPlatformFrame->PostMessage( WM_QUIT_SERVER, 0, 0);
			return;
		}
	}

	return;
}


//结束绘画
void CPlazaViewEntry::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画背景
	m_ImageBack.DrawImage(pDC,0,0);
}


//创建视图
bool CPlazaViewEntry::CreateViewFrame()
{
	//状态提示
	m_DlgStatus.SetStatusViewSink(this);

	//创建提示
	m_DlgStatus.ShowStatusWindow(TEXT("创建游戏房间，请稍候..."));

	try
	{
		//比赛类型
		if(m_GameServer.wServerType&GAME_GENRE_MATCH)
		{
			CMatchViewFrame * pMatchViewFrame=new CMatchViewFrame;
			if(pMatchViewFrame!=NULL)
			{
				m_pITableViewFrame = QUERY_OBJECT_PTR_INTERFACE(pMatchViewFrame,ITableViewFrame);
			}
		}
		else
		{
			CTableViewFrame * pTableViewFrame=new CTableViewFrame;
			if(pTableViewFrame!=NULL)
			{
				m_pITableViewFrame = QUERY_OBJECT_PTR_INTERFACE(pTableViewFrame,ITableViewFrame);
			}
		}

		//创建桌子
		m_pITableViewFrame->CreateTableFrame(this,IDC_SERVER_TABLE,QUERY_ME_INTERFACE(IUnknownEx));

		//创建提示
		m_DlgStatus.ShowStatusWindow(TEXT("创建游戏房间结束..."));

		return true;
	}
	catch(...)
	{
		ASSERT(FALSE);
	}

	return false;
}

VOID CPlazaViewEntry::ResetViewFrame( bool isGameing )
{
	//获取对象
	CContainerUI * pButtonGoBack = (CContainerUI *) GetControlByName( szButtonGoBackControlName );
	if(pButtonGoBack==NULL) return;
	pButtonGoBack->SetVisible(!isGameing);

	//获取对象
	CContainerUI * pButtonQuit = (CContainerUI *) GetControlByName( szButtonQuitControlName );
	if(pButtonQuit==NULL) return;
	pButtonQuit->SetVisible(!isGameing);

	//获取对象
	CContainerUI * pLabelLoading = (CContainerUI *) GetControlByName( szLabelLoadingControlName );
	if(pLabelLoading==NULL) return;
	pLabelLoading->SetVisible(isGameing);
}

//加入游戏
VOID CPlazaViewEntry::InitGameServer(  CGameServerItem * pServerItem )
{
	//状态提示
	m_DlgStatus.ShowStatusWindow(TEXT("初始化游戏参数..."));

	//游戏参数
	m_GameServer = pServerItem->m_GameServer;
	
	CGameKindItem * pGameKindItem = pServerItem->m_pGameKindItem;
	if (pGameKindItem==NULL) return;
	m_GameKind = pGameKindItem->m_GameKind;

	//创建组件
	if (m_GameLevelParserModule.CreateInstance()==false) throw TEXT("等级解释组件创建失败！");
	if (m_PlazaUserManagerModule.CreateInstance()==false) throw TEXT("用户管理组件创建失败！");
	
	//设置组件
	m_ProcessManager.SetProcessManagerSink(QUERY_ME_INTERFACE(IUnknownEx));
	
	//配置组件
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
	CUserInformation * pUserInformation=CUserInformation::GetInstance();
	if (m_PlazaUserManagerModule->SetUserManagerSink(pIUnknownEx)==false) throw TEXT("用户管理组件配置失败！");
	if (m_PlazaUserManagerModule->SetUserInformation(pUserInformation)==false) throw TEXT("用户管理组件配置失败！");
	
	//创建组件
	if (m_TCPSocketModule.CreateInstance()==false) {
		throw TEXT("无法创建网络连接组件，游戏房间进入失败！");
	}
	
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//设置网络
	m_TCPSocketModule->SetTCPSocketSink(QUERY_ME_INTERFACE(IUnknownEx));
	m_TCPSocketModule->SetProxyServerInfo(pParameterGlobal->m_cbProxyType,pParameterGlobal->m_ProxyServer);

	//发起连接
	if (m_TCPSocketModule->Connect(m_GameServer.szServerAddr,m_GameServer.wServerPort)!=CONNECT_SUCCESS)
	{
		throw TEXT("游戏房间连接失败，请检查系统网络配置参数是否正确！");
		return;
	}

	ResetViewFrame(false);

	m_DlgStatus.ShowStatusWindow(TEXT("初始化游戏参数结束...") );
}

//加入游戏
VOID CPlazaViewEntry::PerformJoinGame()
{
	//搜索变量
	tagSearchTable SearchTable;
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;
	SearchTable.wStartTableID=0;
	SearchGameTable(SearchTable);
	
	//没有坐位了
	/* if(SearchTable.bNotFull==false) 
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox( TEXT("未找到合适的房间和坐位，请选择其它游戏！...!") );
		throw(TEXT("..."));
		return;
	} */

	//查找位置
	WORD wChairID=INVALID_CHAIR;
	m_wResultTableID = SearchTable.wResultTableID;
	m_pITableViewFrame->GetNullChairCount(m_wResultTableID,wChairID);

	PerformSitDownAction(m_wResultTableID, wChairID, false);
}

//大厅关闭
bool CPlazaViewEntry::PlazaClosing()
{
	return m_ProcessManager.IsProcessReady();
}

//鼠标双击
VOID CPlazaViewEntry::OnDButtonHitTable(WORD wTableID)
{
	return;
}

//左键按下
VOID CPlazaViewEntry::OnLButtonHitTable(WORD wTableID, WORD wChairID)
{
	//动作过虑
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return;
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return;
	if ((wTableID==m_wReqTableID)&&(wChairID==m_wReqChairID)) return;

	//获取用户
	IClientUserItem * pTableUserItem=NULL;
	if (wChairID!=INDEX_ENTER_CHAIR) pTableUserItem=m_pITableViewFrame->GetClientUserItem(wTableID,wChairID);

	//随机座位
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true)
	{
		PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
		return;
	}

	//消息处理
	if (pTableUserItem!=NULL)
	{
		if (pTableUserItem==m_pIMySelfUserItem)
		{
			//状态过滤
			if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING) return;
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) return;

			//起立动作
			PerformStandUpAction(wTableID,wChairID);
		}
		else
		{
			//状态过滤
			WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
			WORD wMeChairID=m_pIMySelfUserItem->GetChairID();
			if ((wMeTableID==wTableID)&&(wMeChairID==wChairID)) return;
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) return;

			//旁观动作
			// PerformLookonAction(wTableID,wChairID);
		}
	}
	else
	{
		//激活界面
		if (wChairID==INDEX_ENTER_CHAIR)
		{
			//变量定义
			WORD wUserTableID=m_pIMySelfUserItem->GetTableID();
			BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

			//激活界面
			if ((wTableID==wUserTableID)&&(cbUserStatus!=US_LOOKON))
			{
				m_ProcessManager.SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_ACTIVE_PROCESS);
				return;
			}
		}

		//坐下动作
		PerformSitDownAction(wTableID,wChairID,true);
	}

	return;
}

//右键按下
VOID CPlazaViewEntry::OnRButtonHitTable(WORD wTableID, WORD wChairID)
{
	//点击位置
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR)&&(wChairID!=INDEX_ENTER_CHAIR))
	{
		//获取用户
		IClientUserItem * pIClientUserItem=m_pITableViewFrame->GetClientUserItem(wTableID,wChairID);

		//用户菜单
		if (pIClientUserItem!=NULL)
		{
			// TrackUserItemMenu(pIClientUserItem);
		}

		return;
	}

	return;
}

//桌子按钮
VOID CPlazaViewEntry::OnHitTableButton(BYTE cbButton)
{
	//按钮处理
	switch (cbButton)
	{
	case BT_ENTER_GAME:		//进入游戏
		{
			//进入游戏
			if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()==INVALID_TABLE))
			{
				PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
			}

			break;
		}
	case BT_LEAVE_MATCH:	//退出游戏
		{
			//退出游戏
			if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()==INVALID_TABLE))
			{
				m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_LEAVE_MATCH);
			}
		}
	}
}


VOID CPlazaViewEntry::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
	return;
}

//位置消息
VOID CPlazaViewEntry::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);
	return;
}

//创建消息
INT CPlazaViewEntry::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	//加载资源
	m_ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("MATCH_BK"));

	//设置大小
	CSize SizeWindow(m_ImageBack.GetWidth(),m_ImageBack.GetHeight());
	SetWindowPos(NULL, 0, 0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	
	return 0;
}


//连接事件
bool CPlazaViewEntry::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
	//错误提示
	if (nErrorCode!=0L)
	{
		//关闭提示
		m_DlgStatus.HideStatusWindow();

		//关闭房间
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("很抱歉，游戏房间连接失败，请稍后再试或留意网站公告！"),MB_ICONSTOP);

		return true;
	}

	//发送登录
	SendLogonPacket(m_szServerPassword);

	//设置状态
	SetServiceStatus(ServiceStatus_Validate);

	return true;
}

//关闭事件
bool CPlazaViewEntry::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	//关闭状态
	m_DlgStatus.HideStatusWindow();

	//关闭游戏
	m_ProcessManager.CloseGameProcess(false);

	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		if (m_ServiceStatus==ServiceStatus_ServiceIng)
		{
			//获取指针
			// IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
			// ASSERT(pIStringMessage!=NULL);

			//提示消息
			// pIStringMessage->InsertSystemString(TEXT("与游戏服务器的连接中断了"));

			//关闭提示
			CInformation Information(this);
			INT_PTR nResult=Information.ShowMessageBox(m_GameServer.szServerName,TEXT("与游戏服务器的连接已经中断，是否关闭当前游戏房间？"),
				MB_ICONERROR|MB_YESNO|MB_DEFBUTTON2,0L);

			//关闭房间
			if (nResult==IDYES)
			{
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM);
			}
		}
		else
		{
			//关闭提示
			CInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,TEXT("由于意外的原因，房间连接失败，请稍后重新尝试！"),MB_ICONERROR);

			//关闭房间
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
		}
	}

	//设置状态
	m_ServiceStatus=ServiceStatus_NetworkDown;

	return true;
}

//读取事件
bool CPlazaViewEntry::OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	try
	{
		bool br=false;
		switch (Command.wMainCmdID)
		{
		case MDM_GR_LOGON:		//登录消息
			{
				br=OnSocketMainLogon(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_CONFIG:		//配置信息
			{
				br=OnSocketMainConfig(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_USER:		//用户信息
			{
				br=OnSocketMainUser(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_STATUS:		//状态信息
			{
				br=OnSocketMainStatus(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_INSURE:		//银行消息
			{
				br=OnSocketMainInsure(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_TASK:		//任务消息
			{
				br=OnSocketMainTask(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_EXCHANGE:	//兑换消息
			{
				br=OnSocketMainExchange(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_MANAGE:		//管理消息
			{
				br=OnSocketMainManager(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_CM_SYSTEM:		//系统消息
			{
				br=OnSocketMainSystem(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GF_GAME:		//游戏消息
		case MDM_GF_FRAME:		//框架消息
			{
				br=OnSocketMainGameFrame(Command.wMainCmdID,Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_MATCH:		//比赛消息
			{
				br=OnSocketMainMatch(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		}

	}
	catch (...)
	{
		CString strInfo;
		strInfo.Format(TEXT("CmdMainID:%d\tCmdSubID:%d"),Command.wMainCmdID,Command.wSubCmdID);
		AfxMessageBox(strInfo);
	}

	return true;
}


//进程错误
bool CPlazaViewEntry::OnGameProcessError()
{
	OnGameProcessClose(0);

	return true;
}

//进程关闭
bool CPlazaViewEntry::OnGameProcessClose(DWORD dwExitCode)
{
	//效验状态
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return false;

	//变量定义
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();
	BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

	//退出游戏
	if ((wTableID!=INVALID_TABLE)&&(m_ServiceStatus==ServiceStatus_ServiceIng))
	{
		SendStandUpPacket(wTableID,wChairID,TRUE);
	}

	m_pITableViewFrame->SetTableStatus(false);

	//还原窗口
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	pPlatformFrame->PostMessage(WM_COMMAND,IDM_RESTORE,0);
	pPlatformFrame->m_PlazaViewUser.PostMessage(WM_UPDATE_USERINFO,0,0);

	//视图重置
	ResetViewFrame(false);

	return true;
}

//进程启动
bool CPlazaViewEntry::OnGameProcessCreate(bool bAlreadyExist)
{
	//变量定义
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();

	//房间配置
	if (bAlreadyExist==false)
	{
		//获取等级
		tagLevelItem LevelItem[64];
		BYTE cbLevelCount=(BYTE)m_GameLevelParserModule->GetGameLevelItem(LevelItem,CountArray(LevelItem));

		//等级配置
		WORD wLevelCount;
		tagGrowLevelConfig GrowLevelItem[60];
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		wLevelCount=pUserItemElement->GetExperienceInfo(GrowLevelItem,CountArray(GrowLevelItem));

		//发送配置
		m_ProcessManager.SendLevelInfo(LevelItem,cbLevelCount);
		m_ProcessManager.SendColumnInfo(m_ColumnItem,m_cbColumnCount);
		m_ProcessManager.SendGrowLevelConfig(GrowLevelItem,wLevelCount);		
		m_ProcessManager.SendPropertyInfo(m_PropertyInfo,m_cbPropertyCount);
	}

	//房间信息
	m_ProcessManager.SendServerInfo(m_pIMySelfUserItem,m_dwUserRight,m_dwMasterRight,m_dwServerRule,m_wServerType,m_GameServer,m_wAVServerPort,m_dwAVServerAddr);
	
	//比赛信息
	CServerListData * pServerListData = CServerListData::GetInstance();
	CGameServerItem * pGameServerItem=pServerListData->SearchGameServer(m_GameServer.wServerID);
	if (pGameServerItem!=NULL) m_ProcessManager.SendMatchInfo(pGameServerItem->m_GameMatch);

	//发送用户
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		//变量定义
		WORD wEnumIndex=0;
		IPlazaUserManager * pIPlazaUserManager=m_PlazaUserManagerModule.GetInterface();

		//发送玩家
		while (true)
		{
			//获取用户
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//发送判断
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()==US_LOOKON) continue;

			//发送用户
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};

		//旁观用户
		wEnumIndex=0;
		while (true)
		{
			//获取用户
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//发送判断
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()!=US_LOOKON) continue;

			//发送用户
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};
	}

	//配置完成
	m_ProcessManager.SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_CONFIG_FINISH);
	
	//还原窗口
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	pPlatformFrame->PostMessage(WM_COMMAND,IDM_MINIMIZE,0);

	return true;
}

//进程数据
bool CPlazaViewEntry::OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//用户属性
	if ((wMainCmdID==IPC_CMD_GF_USER_INFO)&&(wSubCmdID==IPC_SUB_GF_USER_ATTRIB))
	{
		//效验参数
		ASSERT(wDataSize==sizeof(IPC_GF_UserAttrib));
		if (wDataSize!=sizeof(IPC_GF_UserAttrib)) return false;

		//寻找用户
		IPC_GF_UserAttrib * pUserAttrib=(IPC_GF_UserAttrib *)pData;
		IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserAttrib->dwUserID);

		//用户判断
		ASSERT(pIClientUserItem!=NULL);
		if (pIClientUserItem==NULL) return true;

		//变量定义
		ASSERT(CUserInformation::GetInstance()!=NULL);
		CUserInformation * pUserInformation=CUserInformation::GetInstance();

		//设置关系
		m_PlazaUserManagerModule->UpdateUserItemAttrib(pIClientUserItem,&pUserAttrib->UserAttrib);
		pUserInformation->InsertCompanionInfo(pIClientUserItem,pUserAttrib->UserAttrib.cbCompanion);

		return true;
	}

	//网络消息
	if ((wMainCmdID==IPC_CMD_GF_SOCKET)&&(wSubCmdID==IPC_SUB_GF_SOCKET_SEND))
	{
		//效验数据
		ASSERT(wDataSize>=sizeof(TCP_Command));
		if (wDataSize<sizeof(TCP_Command)) return false;

		//变量定义
		IPC_GF_SocketSend * pSocketPackage=(IPC_GF_SocketSend *)pData;
		WORD wPacketSize=wDataSize-(sizeof(IPC_GF_SocketSend)-sizeof(pSocketPackage->cbBuffer));

		//处理数据
		if (wPacketSize==0)
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID);
		}
		else 
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			if(pCommand->wMainCmdID==MDM_GR_INSURE && pCommand->wSubCmdID==SUB_GR_QUERY_INSURE_INFO)
			{
				//变量定义
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//变量定义
				CMD_GR_C_QueryInsureInfoRequest *pQueryInsureInfo=(CMD_GR_C_QueryInsureInfoRequest *)pSocketPackage->cbBuffer;
				pQueryInsureInfo->cbActivityGame=TRUE;
				lstrcpyn(pQueryInsureInfo->szInsurePass,pGlobalUserData->szPassword,CountArray(pQueryInsureInfo->szInsurePass));
			}
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID,pSocketPackage->cbBuffer,wPacketSize);
		}

		return true;
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}


//发送配置
bool CPlazaViewEntry::SendUserRulePacket()
{
	//变量定义
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	CMD_GR_UserRule * pUserRule=(CMD_GR_UserRule *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GR_UserRule),sizeof(cbBuffer)-sizeof(CMD_GR_UserRule));

	//变量定义
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//构造数据
	pUserRule->cbRuleMask=0x00;

	//限制地址
	if (pParameterGlobal->m_bLimitSameIP==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_SAME_IP;
	}

	//限制胜率
	if (m_pParameterGame->m_bLimitWinRate==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_WIN_RATE;
		pUserRule->wMinWinRate=m_pParameterGame->m_wMinWinRate;
	}

	//限制逃率
	if (m_pParameterGame->m_bLimitFleeRate==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_FLEE_RATE;
		pUserRule->wMaxFleeRate=m_pParameterGame->m_wMaxFleeRate;
	}

	//限制积分
	if (m_pParameterGame->m_bLimitGameScore==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_GAME_SCORE;
		pUserRule->lMaxGameScore=m_pParameterGame->m_lMaxGameScore;
		pUserRule->lMinGameScore=m_pParameterGame->m_lMinGameScore;
	}

	//携带密码
	if ((CServerRule::IsForfendLockTable(m_dwServerRule)==false)&&(m_pParameterServer->m_bTakePassword==true))
	{
		SendPacket.AddPacket(m_pParameterServer->m_szPassword,DTP_GR_TABLE_PASSWORD);
	}

	//发送数据
	WORD wSendSize=sizeof(CMD_GR_UserRule)+SendPacket.GetDataSize();
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_RULE,cbBuffer,wSendSize);

	return true;
}

//发送登录
bool CPlazaViewEntry::SendLogonPacket(LPCTSTR pszServerPasswd)
{
	//变量定义
	CMD_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//变量定义
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//游戏版本
	LPCTSTR pszProcessName=m_GameKind.szProcessName;
	CWHService::GetModuleVersion(pszProcessName,LogonUserID.dwProcessVersion);

	//附加信息
	LogonUserID.dwPlazaVersion=pGlobalUnits->GetPlazaVersion();
	LogonUserID.dwFrameVersion=pGlobalUnits->GetFrameVersion();

	//登录信息
	LogonUserID.wKindID=m_GameServer.wKindID;
	LogonUserID.dwUserID=pGlobalUserData->dwUserID;	
	lstrcpyn(LogonUserID.szServerPasswd,pszServerPasswd,CountArray(LogonUserID.szServerPasswd));
	lstrcpyn(LogonUserID.szPassword,pGlobalUserData->szDynamicPass,CountArray(LogonUserID.szPassword));	

	//机器标识
	CWHService::GetMachineIDEx(LogonUserID.szMachineID);

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_LOGON,SUB_GR_LOGON_USERID,&LogonUserID,sizeof(LogonUserID));

	return true;
}

//发送旁观
bool CPlazaViewEntry::SendLookonPacket(WORD wTableID, WORD wChairID)
{
	//变量定义
	CMD_GR_UserLookon UserLookon;
	ZeroMemory(&UserLookon,sizeof(UserLookon));

	//构造数据
	UserLookon.wTableID=wTableID;
	UserLookon.wChairID=wChairID;

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_LOOKON,&UserLookon,sizeof(UserLookon));

	return true;
}

//执行起立
bool CPlazaViewEntry::PerformStandUpAction(WORD wTableID, WORD wChairID)
{
	//效验数据
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID==wTableID)&&(m_wReqChairID==wChairID)) return false;

	//自己状态
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"));

		return false;
	}

	//设置变量
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;

	//设置界面
	// m_pITableViewFrame->VisibleTable(wTableID);

	//发送命令
	SendStandUpPacket(wTableID,wChairID,FALSE);

	return true;
}

//发送坐下
bool CPlazaViewEntry::SendSitDownPacket(WORD wTableID, WORD wChairID, LPCTSTR lpszPassword)
{
	//变量定义
	CMD_GR_UserSitDown UserSitDown;
	ZeroMemory(&UserSitDown,sizeof(UserSitDown));

	//构造数据
	UserSitDown.wTableID=wTableID;
	UserSitDown.wChairID=wChairID;
	if ((lpszPassword!=NULL)&&(lpszPassword[0]!=0))
	{
		lstrcpyn(UserSitDown.szPassword,lpszPassword,CountArray(UserSitDown.szPassword));
	}

	//比赛报名时候
	if((m_wServerType&GAME_GENRE_MATCH)!=0&&m_ProcessManager.IsProcessReady())
	{
		ShowInformation(TEXT("请先关闭游戏客户端！"));
		return true;
	}

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&UserSitDown,sizeof(UserSitDown));

	return true;
}

//发送起立
bool CPlazaViewEntry::SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave)
{
	//变量定义
	CMD_GR_UserStandUp UserStandUp;
	ZeroMemory(&UserStandUp,sizeof(UserStandUp));

	//构造数据
	UserStandUp.wTableID=wTableID;
	UserStandUp.wChairID=wChairID;
	UserStandUp.cbForceLeave=cbForceLeave;

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_STANDUP,&UserStandUp,sizeof(UserStandUp));

	return true;
}

//发送聊天
bool CPlazaViewEntry::SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor)
{
	//构造信息
	CMD_GR_C_UserChat UserChat;
	lstrcpyn(UserChat.szChatString,pszChatString,CountArray(UserChat.szChatString));

	//构造数据
	UserChat.dwChatColor=crColor;
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//发送命令
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]));

	return true;
}

//发送表情
bool CPlazaViewEntry::SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex)
{
	//变量定义
	CMD_GR_C_UserExpression UserExpression;
	ZeroMemory(&UserExpression,sizeof(UserExpression));

	//构造信息
	UserExpression.wItemIndex=wItemIndex;
	UserExpression.dwTargetUserID=dwTargetUserID;

	//发送命令
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));

	return true;
}


//用户激活
VOID CPlazaViewEntry::OnUserItemAcitve(IClientUserItem * pIClientUserItem)
{
	//判断自己
	if (m_pIMySelfUserItem==NULL)
	{
		m_pIMySelfUserItem=pIClientUserItem;
	}

	//设置桌子
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
	if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
	{
		WORD wTableID=pIClientUserItem->GetTableID();
		WORD wChairID=pIClientUserItem->GetChairID();
		m_pITableViewFrame->SetClientUserItem(wTableID,wChairID,pIClientUserItem);
	}

	return;
}

//用户删除
VOID CPlazaViewEntry::OnUserItemDelete(IClientUserItem * pIClientUserItem)
{
	//变量定义
	WORD wLastTableID=pIClientUserItem->GetTableID();
	WORD wLastChairID=pIClientUserItem->GetChairID();
	DWORD dwLeaveUserID=pIClientUserItem->GetUserID();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//离开处理
	if ((wLastTableID!=INVALID_TABLE)&&(wLastChairID!=INVALID_CHAIR))
	{
		//获取用户
		IClientUserItem * pITableUserItem=m_pITableViewFrame->GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_pITableViewFrame->SetClientUserItem(wLastTableID,wLastChairID,NULL);

		//离开通知
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.cbUserStatus=US_NULL;
			UserStatus.wTableID=INVALID_TABLE;
			UserStatus.wChairID=INVALID_CHAIR;
			m_ProcessManager.SendUserStatus(pIClientUserItem->GetUserID(),UserStatus);
		}
	}
		
	return;
}

//用户更新
VOID CPlazaViewEntry::OnUserFaceUpdate(IClientUserItem * pIClientUserItem)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//更新游戏
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(m_pIMySelfUserItem->GetTableID()==pUserInfo->wTableID))
	{
		m_ProcessManager.SendCustomFace(pUserInfo->dwUserID,pUserInfo->dwCustomID,pCustomFaceInfo);
	}

	return;
}

//用户更新
VOID CPlazaViewEntry::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	
	//界面通知
	if (pIClientUserItem==m_pIMySelfUserItem)
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//设置变量
		if(m_wServerType==GAME_GENRE_GOLD)
		{
			pGlobalUserData->lUserScore=pIClientUserItem->GetUserScore();
		}
		
		//更新财富
		pGlobalUserData->dwLoveLiness=pUserInfo->lLoveLiness;
		pGlobalUserData->lUserIngot=pIClientUserItem->GetUserIngot();
		pGlobalUserData->lUserInsure=pIClientUserItem->GetUserInsure();
		pGlobalUserData->dUserBeans=pUserInfo->dBeans;

		//事件通知
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	//游戏通知
	if ((pMeUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->wTableID==pMeUserInfo->wTableID))
	{
		//变量定义
		tagUserScore UserScore;
		ZeroMemory(&UserScore,sizeof(UserScore));

		//设置变量
		UserScore.lScore=pUserInfo->lScore;
		UserScore.lGrade=pUserInfo->lGrade;
		UserScore.lIngot=pUserInfo->lIngot;
		UserScore.lInsure=pUserInfo->lInsure;
		UserScore.dBeans=pUserInfo->dBeans;
		UserScore.dwWinCount=pUserInfo->dwWinCount;
		UserScore.dwLostCount=pUserInfo->dwLostCount;
		UserScore.dwDrawCount=pUserInfo->dwDrawCount;
		UserScore.dwFleeCount=pUserInfo->dwFleeCount;
		UserScore.dwExperience=pUserInfo->dwExperience;
		UserScore.lLoveLiness=pUserInfo->lLoveLiness;

		//发送数据
		m_ProcessManager.SendUserScore(pUserInfo->dwUserID,UserScore);
	}

	return;
}

//用户更新
VOID CPlazaViewEntry::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	WORD wNowTableID=pIClientUserItem->GetTableID(),wLastTableID=LastStatus.wTableID;
	WORD wNowChairID=pIClientUserItem->GetChairID(),wLastChairID=LastStatus.wChairID;
	BYTE cbNowStatus=pIClientUserItem->GetUserStatus(),cbLastStatus=LastStatus.cbUserStatus;
	
	//桌子离开
	if ((wLastTableID!=INVALID_TABLE)&&((wLastTableID!=wNowTableID)||(wLastChairID!=wNowChairID)))
	{
		IClientUserItem * pITableUserItem= m_pITableViewFrame->GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_pITableViewFrame->SetClientUserItem(wLastTableID,wLastChairID,NULL);
	}

	//桌子加入
	if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//厌恶判断（黑名单）
		if(pUserInfo->dwUserID != pMeUserInfo->dwUserID && cbNowStatus == US_SIT && pMeUserInfo->wTableID == wNowTableID)
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//厌恶玩家
			if (pParameterGlobal->m_bLimitDetest==true)
			{
				if (pIClientUserItem->GetUserCompanion()==CP_DETEST)
				{
					CMD_GR_UserRepulseSit UserRepulseSit;
					UserRepulseSit.wTableID=wNowTableID;
					UserRepulseSit.wChairID=wNowChairID;
					UserRepulseSit.dwUserID=pMeUserInfo->dwUserID;
					UserRepulseSit.dwRepulseUserID=pUserInfo->dwUserID;
					m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_REPULSE_SIT,&UserRepulseSit,sizeof(UserRepulseSit));
				}
			}
		}
		m_pITableViewFrame->SetClientUserItem(wNowTableID,wNowChairID,pIClientUserItem);
	}

	//桌子状态
	if ((m_pITableViewFrame->GetChairCount() < MAX_CHAIR)&&(wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(wNowChairID==wLastChairID))
	{
		m_pITableViewFrame->UpdateTableView(wNowTableID);
	}

	//离开通知
	if ((wLastTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//游戏通知
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.wTableID=wNowTableID;
			UserStatus.wChairID=wNowChairID;
			UserStatus.cbUserStatus=cbNowStatus;
			m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);
		}

		return;
	}

	//加入处理
	if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//游戏通知
		if ((m_pIMySelfUserItem!=pIClientUserItem)&&(pMeUserInfo->wTableID==wNowTableID))
		{
			ASSERT(wNowChairID!=INVALID_CHAIR);
			m_ProcessManager.SendUserItem(pIClientUserItem);
		}

		//自己判断
		if (m_pIMySelfUserItem==pIClientUserItem)
		{
			//设置变量
			m_wReqTableID=INVALID_TABLE;
			m_wReqChairID=INVALID_CHAIR;

			//启动进程
			m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);
		}

		return;
	}

	//状态改变
	if ((wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(pMeUserInfo->wTableID==wNowTableID))
	{
		//游戏通知
		tagUserStatus UserStatus;
		UserStatus.wTableID=wNowTableID;
		UserStatus.wChairID=wNowChairID;
		UserStatus.cbUserStatus=cbNowStatus;
		m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);

		return;
	}

	return;
}

//用户更新
VOID CPlazaViewEntry::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserAttrib & UserAttrib)
{
	//变量定义
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
	WORD wUserTableID=pIClientUserItem->GetTableID();

	//通知游戏
	if ((wMeTableID!=INVALID_TABLE)&&(wMeTableID==wUserTableID))
	{
		//变量定义
		tagUserAttrib UserAttrib;
		UserAttrib.cbCompanion=pIClientUserItem->GetUserCompanion();

		//发送通知
		m_ProcessManager.SendUserAttrib(pIClientUserItem->GetUserID(),UserAttrib);
	}

	return;
}

//执行坐下
bool CPlazaViewEntry::PerformSitDownAction(WORD wTableID, WORD wChairID, bool bEfficacyPass)
{
	
	//状态过滤
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqTableID==wTableID)) return false;
	if ((m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID==wChairID)) return false;
	
	//自己状态
	if (m_pIMySelfUserItem!=NULL)
	{
		if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"));

			return false;
		}
	}

	//权限判断
	if (CUserRight::CanPlay(m_dwUserRight)==false)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("抱歉，您暂时没有加入游戏的权限！"));

		return false;
	}

	//桌子效验
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		CString strDescribe;
		if ((wChairID!=INDEX_ENTER_CHAIR)&&(EfficacyTableRule(wTableID,wChairID,false,strDescribe)==false))
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,strDescribe,MB_ICONINFORMATION,30L);

			return false;
		}
	}

	//密码判断
	TCHAR szPassword[LEN_PASSWORD] = {0};

	//清理界面
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID!=INDEX_ENTER_CHAIR))
	{
		IClientUserItem * pIClientUserItem=m_pITableViewFrame->GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pIClientUserItem==m_pIMySelfUserItem) m_pITableViewFrame->SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//设置界面
	if ((wChairID!=INDEX_ENTER_CHAIR)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		// m_pITableViewFrame->VisibleTable(wTableID);
		m_pITableViewFrame->SetClientUserItem(wTableID,wChairID,m_pIMySelfUserItem);
	}

	//设置变量
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;

	//发送命令
	SendSitDownPacket(wTableID,wChairID,szPassword);

	return true;
}


//桌子效验
bool CPlazaViewEntry::EfficacyTableRule(WORD wTableID, WORD wChairID, bool bReqLookon, CString & strDescribe)
{
	//效验参数
	ASSERT(wTableID<m_pITableViewFrame->GetTableCount());
	ASSERT(wChairID<m_pITableViewFrame->GetChairCount());

	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if (wTableID>=m_pITableViewFrame->GetTableCount()) return false;
	if (wChairID>=m_pITableViewFrame->GetChairCount()) return false;

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//变量定义
	ITableView * pITableView=m_pITableViewFrame->GetTableViewItem(wTableID);
	IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(wChairID);

	//变量定义
	bool bGameStart=pITableView->GetPlayFlag();
	bool bDynamicJoin=CServerRule::IsAllowDynamicJoin(m_dwServerRule);

	//游戏状态
	if ((bGameStart==true)&&(bDynamicJoin==false)&&(bReqLookon==false))
	{
		strDescribe=TEXT("此游戏桌已经开始游戏了，暂时不能加入！");
		return false;
	}

	//其他判断
	if ((bReqLookon==false)&&(m_pIMySelfUserItem->GetMasterOrder()==0))
	{
		//规则判断
		for (WORD i=0;i<m_pITableViewFrame->GetChairCount();i++)
		{
			//获取用户
			IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(i);
			if ((pITableUserItem==NULL)||(pITableUserItem==m_pIMySelfUserItem)) continue;

			//积分效验
			if (m_pParameterGame->m_bLimitGameScore)
			{
				//最低积分
				if (pITableUserItem->GetUserScore()<m_pParameterGame->m_lMinGameScore)
				{
					strDescribe.Format(TEXT("[ %s ] 的积分太低了，与您的设置不符！"),pITableUserItem->GetNickName());
					return false;
				}
			}
		}
	}
	return true;
}



//用户处理
bool CPlazaViewEntry::OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_USER_ENTER:			//用户进入
		{
			return OnSocketSubUserEnter(pData,wDataSize);
		}
	case SUB_GR_USER_SCORE:			//用户积分
		{
			return OnSocketSubUserScore(pData,wDataSize);
		}
	case SUB_GR_USER_STATUS:		//用户状态
		{
			return OnSocketSubUserStatus(pData,wDataSize);
		}
	case SUB_GR_USER_GAME_DATA:		//用户游戏数据
		{
			return OnSocketSubUserItemGameData(pData,wDataSize);
		}
	case SUB_GR_REQUEST_FAILURE:	//请求失败
		{
			return OnSocketSubRequestFailure(pData,wDataSize);
		}
	case SUB_GR_USER_CHAT:			//用户聊天
		{
			return OnSocketSubUserChat(pData,wDataSize);
		}
	case SUB_GR_USER_EXPRESSION:	//用户表情
		{
			return OnSocketSubExpression(pData,wDataSize);
		}
	case SUB_GR_WISPER_CHAT:		//用户私聊
		{
			return OnSocketSubWisperUserChat(pData,wDataSize);
		}
	case SUB_GR_WISPER_EXPRESSION:	//私聊表情
		{
			return OnSocketSubWisperExpression(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_SUCCESS:	//道具成功
		{
			return OnSocketSubPropertySuccess(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_FAILURE:  //道具失败
		{
			return OnSocketSubPropertyFailure(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_EFFECT:   //道具效应
		{
			return OnSocketSubPropertyEffect(pData,wDataSize);
		}
	case SUB_GR_USER_INVITE:	   //邀请玩家
		{
			return OnSocketSubUserInvite(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_MESSAGE:   //礼物消息
		{
			return OnSocketPropertyMessage(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_TRUMPET:  //喇叭消息
		{
			return OnSocketPropertyTrumpet(pData,wDataSize);
		}
	case SUB_GR_USER_WAIT_DISTRIBUTE: //等待分配
		{
			return OnSocketSubWaitDistribute(pData,wDataSize);
		}
	}

	return true;
}

//登录处理
bool CPlazaViewEntry::OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_SUCCESS:	//登录成功
		{
			return OnSocketSubLogonSuccess(pData,wDataSize);
		}
	case SUB_GR_LOGON_FAILURE:	//登录失败
		{
			return OnSocketSubLogonFailure(pData,wDataSize);
		}
	case SUB_GR_LOGON_FINISH:	//登录完成
		{
			return OnSocketSubLogonFinish(pData,wDataSize);
		}
	case SUB_GR_UPDATE_NOTIFY:	//更新提示
		{
			return OnSocketSubUpdateNotify(pData,wDataSize);
		}
	}

	return true;
}

//配置处理
bool CPlazaViewEntry::OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_CONFIG_COLUMN:		//列表配置
		{
			//变量定义
			CMD_GR_ConfigColumn * pConfigColumn=(CMD_GR_ConfigColumn *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ConfigColumn)-sizeof(pConfigColumn->ColumnItem);

			//效验参数
			ASSERT((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))==wDataSize);
			if ((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))!=wDataSize) return false;

			//保存信息
			m_cbColumnCount=__min(pConfigColumn->cbColumnCount,CountArray(m_ColumnItem));
			CopyMemory(m_ColumnItem,pConfigColumn->ColumnItem,sizeof(tagColumnItem)*m_cbColumnCount);

			return true;
		}
	case SUB_GR_CONFIG_SERVER:		//房间配置
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_GR_ConfigServer));
			if (wDataSize<sizeof(CMD_GR_ConfigServer)) return false;

			//消息处理
			CMD_GR_ConfigServer * pConfigServer=(CMD_GR_ConfigServer *)pData;

			//房间属性
			m_wServerType=pConfigServer->wServerType;
			m_dwServerRule=pConfigServer->dwServerRule;

          	//构造参数
			WORD wTableCount=pConfigServer->wTableCount;
			WORD wChairCount=pConfigServer->wChairCount;
			m_wChairCount=pConfigServer->wChairCount;
			IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);

			//资源目录
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//配置桌子
			bool bSuccess=m_pITableViewFrame->ConfigTableFrame(wTableCount,wChairCount,m_dwServerRule,m_wServerType,m_GameServer.wServerID,szResDirectory);
			WORD pCount = m_pITableViewFrame->GetChairCount();

			//错误处理
			if (bSuccess==false)
			{
				//隐藏提示
				m_DlgStatus.HideStatusWindow();

				//关闭连接
				m_TCPSocketModule->CloseSocket();

				//提示信息
				CInformation Information(AfxGetMainWnd());
				INT nResult=Information.ShowMessageBox(TEXT("游戏资源文件加载失败或者格式错误，是否现在进行修复？"),MB_ICONERROR|MB_YESNO);

				//下载游戏
				if (nResult==IDYES)
				{
					CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//关闭房间
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			return true;
		}
	case SUB_GR_CONFIG_PROPERTY:	//道具配置
		{
			//变量定义
			CMD_GR_ConfigProperty * pConfigProperty=(CMD_GR_ConfigProperty *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ConfigProperty)-sizeof(pConfigProperty->PropertyInfo);

			//效验参数
			ASSERT((wHeadSize+pConfigProperty->cbPropertyCount*sizeof(pConfigProperty->PropertyInfo[0]))==wDataSize);
			if ((wHeadSize+pConfigProperty->cbPropertyCount*sizeof(pConfigProperty->PropertyInfo[0]))!=wDataSize) return false;

			//保存信息
			m_cbPropertyCount=__min(pConfigProperty->cbPropertyCount,CountArray(m_PropertyInfo));
			CopyMemory(m_PropertyInfo,pConfigProperty->PropertyInfo,sizeof(tagPropertyInfo)*m_cbPropertyCount);

			//设置道具
			// m_pPlazaViewServerCtrl->SetPropertyDescribe(m_PropertyInfo,m_cbPropertyCount);

			return true;
		}
	case SUB_GR_CONFIG_FINISH:		//配置完成
		{
			//资源目录
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//游戏等级
			
			ASSERT(m_GameLevelParserModule.GetInterface()!=NULL);
			bool bSuccess=m_GameLevelParserModule->LoadGameLevelItem(m_GameKind.szKindName,szResDirectory,m_wServerType);

			//错误处理
			if (bSuccess==false)
			{
				//关闭连接
				m_TCPSocketModule->CloseSocket();

				//提示信息
				INT nResult=ShowInformation(TEXT("游戏等级配置读取失败或者格式错误，是否现在进行修复？"),MB_ICONERROR|MB_YESNO);

				//下载游戏
				if (nResult==IDYES)
				{
					CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//关闭房间
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			return true;
		}
	case SUB_GR_CONFIG_USER_RIGHT:	//配置玩家权限
		{
			//效验数据
			ASSERT(wDataSize>=sizeof(CMD_GR_ConfigUserRight));
			if (wDataSize<sizeof(CMD_GR_ConfigUserRight)) return false;

			//消息处理
			CMD_GR_ConfigUserRight * pConfigUserRight=(CMD_GR_ConfigUserRight *)pData;

			//玩家权限
			m_dwUserRight=pConfigUserRight->dwUserRight;

			//设置私聊窗口

			/* for( INT_PTR i = 0; i < m_DlgWhisperItemArray.GetCount(); i++ )
			{
				CDlgWhisper *pdlg = m_DlgWhisperItemArray[i];
				if( pdlg ) pdlg->SetUserRight(m_dwUserRight);
			} */

			//通知客户端
			if(IsGameClientReady())
			{
				//构造函数
				IPC_GF_UserRight ur = {0};
				ur.dwUserRight = m_dwUserRight;

				m_ProcessManager.SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_USER_RIGHT,&ur,sizeof(ur) );
			}

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//状态处理
bool CPlazaViewEntry::OnSocketMainStatus(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_TABLE_INFO:		//桌子信息
		{
			//变量定义
			CMD_GR_TableInfo * pTableInfo=(CMD_GR_TableInfo *)pData;
			WORD wHeadSize=sizeof(CMD_GR_TableInfo)-sizeof(pTableInfo->TableStatusArray);

			//效验数据
			ASSERT((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))==wDataSize);
			if ((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))!=wDataSize) return false;

			//消息处理
			for (WORD i=0;i<pTableInfo->wTableCount;i++)
			{
				BYTE cbTableLock=pTableInfo->TableStatusArray[i].cbTableLock;
				BYTE cbPlayStatus=pTableInfo->TableStatusArray[i].cbPlayStatus;
				m_pITableViewFrame->SetTableStatus(i,(cbPlayStatus==TRUE),(cbTableLock==TRUE),pTableInfo->TableStatusArray[i].lCellScore);
			}

			return true;
		}
	case SUB_GR_TABLE_STATUS:	//桌子状态
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GR_TableStatus));
			if (wDataSize!=sizeof(CMD_GR_TableStatus)) return false;

			//变量定义
			CMD_GR_TableStatus * pTableStatus=(CMD_GR_TableStatus *)pData;

			//设置桌子
			BYTE cbTableLock=pTableStatus->TableStatus.cbTableLock;
			BYTE cbPlayStatus=pTableStatus->TableStatus.cbPlayStatus;
			m_pITableViewFrame->SetTableStatus(pTableStatus->wTableID,(cbPlayStatus==TRUE),(cbTableLock==TRUE),pTableStatus->TableStatus.lCellScore);

			//设置桌子
			if(cbPlayStatus==TRUE && m_pIMySelfUserItem->GetTableID()==pTableStatus->wTableID && CServerRule::IsAllowAvertCheatMode(m_dwServerRule))
			{
				m_pITableViewFrame->SetTableStatus(false);
			}

			return true;
		}
	}

	return true;
}

//银行处理
bool CPlazaViewEntry::OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验状态
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//游戏处理
	BYTE *pActivityGame = (BYTE*)pData;
	if(*pActivityGame==TRUE)
	{
		return OnSocketMainGameFrame(MDM_GR_INSURE,wSubCmdID,pData,wDataSize);
	}

	//消息处理
	/* ASSERT(m_pDlgInsureServer!=NULL);
	if (m_pDlgInsureServer==NULL) return true;
	if (m_pDlgInsureServer->OnServerInsureMessage(MDM_GR_INSURE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	} */

	return true;
}

//任务处理
bool CPlazaViewEntry::OnSocketMainTask(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验状态
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//消息过滤
	if(wSubCmdID==SUB_GR_TASK_FINISH)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GR_S_TaskFinish));
		if(wDataSize>sizeof(CMD_GR_S_TaskFinish)) return true;

		//提取数据
		CMD_GR_S_TaskFinish * pTaskFinish = (CMD_GR_S_TaskFinish *)pData;

		//显示提示框
		/* CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
		pPlatformFrame->ShowTaskNotifyer(pTaskFinish->szTaskName); */

		return true;
	}
    
	//消息处理
	
	/* ASSERT(m_pDlgTaskServerCenter!=NULL);
	if (m_pDlgTaskServerCenter==NULL) return true;
	if (m_pDlgTaskServerCenter->OnTaskMessage(MDM_GR_TASK,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	} */

	return true;
}

//兑换处理
bool CPlazaViewEntry::OnSocketMainExchange(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验状态
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;
   
	//消息处理
	
	/* ASSERT(m_pDlgExchangeServer!=NULL);
	if (m_pDlgExchangeServer==NULL) return true;
	if (m_pDlgExchangeServer->OnExchangeMessage(MDM_GR_EXCHANGE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	} */

	return true;
}

//管理处理
bool CPlazaViewEntry::OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_OPTION_CURRENT:	//当前配置
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GR_OptionCurrent));
			if (wDataSize!=sizeof(CMD_GR_OptionCurrent)) return false;

			//消息处理
			/* if (m_pDlgServerManager!=NULL)
			{
				CMD_GR_OptionCurrent * pOptionCurrent=(CMD_GR_OptionCurrent *)pData;
				m_pDlgServerManager->SetServerType(m_wServerType);
				m_pDlgServerManager->SetServerOptionInfo(pOptionCurrent->ServerOptionInfo,pOptionCurrent->dwRuleMask);
			} */

			return true;
		}
	}

	return true;
}

//系统处理
bool CPlazaViewEntry::OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CM_SYSTEM_MESSAGE:	//系统消息
		{
			return OnSocketSubSystemMessage(pData,wDataSize);
		}
	case SUB_CM_ACTION_MESSAGE:	//动作消息
		{
			return OnSocketSubActionMessage(pData,wDataSize);
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏处理
bool CPlazaViewEntry::OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize<=SOCKET_TCP_PACKET);
	if (wDataSize>SOCKET_TCP_PACKET) return false;
	if(!IsGameClientReady()) return true;

	//构造数据
	IPC_GF_SocketRecv SocketRecv;
	SocketRecv.CommandInfo.wSubCmdID=wSubCmdID;
	SocketRecv.CommandInfo.wMainCmdID=wMainCmdID;

	//叠加数据
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketRecv.cbBuffer,pData,wDataSize);
	}

	//发送数据
	WORD wSendSize=sizeof(SocketRecv)-sizeof(SocketRecv.cbBuffer)+wDataSize;
	m_ProcessManager.SendProcessData(IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_RECV,&SocketRecv,wSendSize);

	return true;
}

//比赛处理
bool CPlazaViewEntry::OnSocketMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_MATCH_FEE:		//费用查询
		{
			//参数效验
			ASSERT(wDataSize<=sizeof(CMD_GR_Match_Fee));
			if(wDataSize>sizeof(CMD_GR_Match_Fee)) return false;

			//提取数据
			CMD_GR_Match_Fee * pMatchFee = (CMD_GR_Match_Fee *)pData;
			if(pMatchFee->lMatchFee>0)
			{
				INT nResult=ShowInformation(pMatchFee->szNotifyContent,MB_ICONINFORMATION|MB_YESNO,30L);
				if (nResult==IDYES) m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_FEE,&pMatchFee->lMatchFee,sizeof(pMatchFee->lMatchFee));
			}

			return true;
		}
	case SUB_GR_MATCH_GOLDUPDATE:	//金币更新
		{
			//参数校验
			ASSERT(sizeof(CMD_GR_MatchGoldUpdate)==wDataSize);
			if(sizeof(CMD_GR_MatchGoldUpdate)!=wDataSize) return false;

			//提取数据
			CMD_GR_MatchGoldUpdate * pMatchGoldUpdate=(CMD_GR_MatchGoldUpdate*)pData;

			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//设置变量
			pGlobalUserData->lUserScore=pMatchGoldUpdate->lCurrGold;
			pGlobalUserData->lUserIngot=pMatchGoldUpdate->lCurrIngot;
			pGlobalUserData->dwExperience=pMatchGoldUpdate->dwCurrExprience;			

			//事件通知
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

			return true;
		}
	case SUB_GR_MATCH_ELIMINATE:	//比赛淘汰
		{
			//获取对象
			CServerListData * pServerListData = CServerListData::GetInstance();
			CGameServerItem * pGameServerItem=pServerListData->SearchGameServer(m_GameServer.wServerID);

			//修改报名状态
			if(pGameServerItem!=NULL) pGameServerItem->m_bSignuped=false;

			return true;
		}
	}

	return m_pITableViewFrame->OnEventViewData(wSubCmdID,pData,wDataSize);
}


//用户进入
bool CPlazaViewEntry::OnSocketSubUserEnter(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(tagUserInfoHead));
	if (wDataSize<sizeof(tagUserInfoHead)) return false;

	//变量定义
	tagUserInfo UserInfo;
	tagCustomFaceInfo CustomFaceInfo;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

	//消息处理
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pData;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
	bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserInfoHead->dwUserID;
	bool bMasterUserOrder=((pUserInfoHead->cbMasterOrder>0)||((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetMasterOrder()>0)));

	//读取信息
	if ((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		//用户属性
		UserInfo.wFaceID=pUserInfoHead->wFaceID;
		UserInfo.dwGameID=pUserInfoHead->dwGameID;
		UserInfo.dwUserID=pUserInfoHead->dwUserID;
		UserInfo.dwGroupID=pUserInfoHead->dwGroupID;
		UserInfo.dwCustomID=pUserInfoHead->dwCustomID;

		//用户状态
		UserInfo.wTableID=pUserInfoHead->wTableID;
		UserInfo.wChairID=pUserInfoHead->wChairID;
		UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

		//用户属性
		UserInfo.cbGender=pUserInfoHead->cbGender;
		UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;

		//用户积分
		UserInfo.lScore=pUserInfoHead->lScore;
		UserInfo.lGrade=pUserInfoHead->lGrade;
		UserInfo.lInsure=pUserInfoHead->lInsure;
		UserInfo.lIngot=pUserInfoHead->lIngot;
		UserInfo.dBeans=pUserInfoHead->dBeans;
		UserInfo.dwWinCount=pUserInfoHead->dwWinCount;
		UserInfo.dwLostCount=pUserInfoHead->dwLostCount;
		UserInfo.dwDrawCount=pUserInfoHead->dwDrawCount;
		UserInfo.dwFleeCount=pUserInfoHead->dwFleeCount;		
		UserInfo.dwExperience=pUserInfoHead->dwExperience;
		UserInfo.lLoveLiness=pUserInfoHead->lLoveLiness;

		//变量定义
		VOID * pDataBuffer=NULL;
		tagDataDescribe DataDescribe;
		CRecvPacketHelper RecvPacket(pUserInfoHead+1,wDataSize-sizeof(tagUserInfoHead));

		//扩展信息
		while (true)
		{
			pDataBuffer=RecvPacket.GetData(DataDescribe);
			if (DataDescribe.wDataDescribe==DTP_NULL) break;
			switch (DataDescribe.wDataDescribe)
			{
			case DTP_GR_NICK_NAME:		//用户昵称
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szNickName));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szNickName))
					{
						CopyMemory(&UserInfo.szNickName,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szNickName[CountArray(UserInfo.szNickName)-1]=0;
					}
					break;
				}
			case DTP_GR_GROUP_NAME:		//用户社团
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName))
					{
						CopyMemory(&UserInfo.szGroupName,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szGroupName[CountArray(UserInfo.szGroupName)-1]=0;
					}
					break;
				}
			case DTP_GR_UNDER_WRITE:	//个性签名
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite))
					{
						CopyMemory(UserInfo.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szUnderWrite[CountArray(UserInfo.szUnderWrite)-1]=0;
					}
					break;
				}
			case DTP_GR_USER_ADDRESS:	//用户地址
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szUserAddress));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szUserAddress))
					{
						CopyMemory(UserInfo.szUserAddress,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szUserAddress[CountArray(UserInfo.szUserAddress)-1]=0;
					}
					break;
				}
			}
		}

		//自定头像
		if (pUserInfoHead->dwCustomID!=0L)
		{
			//加载头像
			CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
			bool bSuccess=pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID,CustomFaceInfo);

			//下载头像
			if (bSuccess==false)
			{
				pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID);
			}
		}
	}
	else
	{
		//用户信息
		UserInfo.dwUserID=pUserInfoHead->dwUserID;
		lstrcpyn(UserInfo.szNickName,TEXT("游戏玩家"),CountArray(UserInfo.szNickName));

		//用户状态
		UserInfo.wTableID=pUserInfoHead->wTableID;
		UserInfo.wChairID=pUserInfoHead->wChairID;
		UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

		//用户属性
		UserInfo.cbGender=pUserInfoHead->cbGender;
		UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;
	}

	//激活用户
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(UserInfo.dwUserID);
	if (pIClientUserItem==NULL) pIClientUserItem=m_PlazaUserManagerModule->ActiveUserItem(UserInfo,CustomFaceInfo,pUserInfoHead->bIsAndroid);

	//获取对象
	CServerListData * pServerListData=CServerListData::GetInstance();

	//人数更新
	pServerListData->SetServerOnLineCount(m_GameServer.wServerID,m_PlazaUserManagerModule->GetActiveUserCount());

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//好友提示
	if (((bHideUserInfo==false)&&(bMySelfUserItem==false))||(bMasterUserOrder==true))
	{
		if(pParameterGlobal->m_bNotifyFriendCome && pIClientUserItem->GetUserCompanion()==CP_FRIEND)
		{
			// 获取指针
			/* IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
			ASSERT(pIStringMessage!=NULL);

			//提示消息
			CString strDescribe;
			strDescribe.Format(TEXT("您的好友 [%s] 进来了！"),pIClientUserItem->GetNickName());
			pIStringMessage->InsertSystemString(strDescribe); */
		}
	}

	return true;
}

//用户积分
bool CPlazaViewEntry::OnSocketSubUserScore(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserScore));
	if (wDataSize<sizeof(CMD_GR_UserScore)) return false;

	//寻找用户
	CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserScore->dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserScore->dwUserID;
	bool bAvertCheatMode=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
	bool bMasterUserOrder=((pIClientUserItem->GetMasterOrder()>0)||(m_pIMySelfUserItem->GetMasterOrder()>0));

	//更新用户
	if ((bAvertCheatMode==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		m_PlazaUserManagerModule->UpdateUserItemScore(pIClientUserItem,&pUserScore->UserScore);
	}

	return true;
}

//用户状态
bool CPlazaViewEntry::OnSocketSubUserStatus(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserStatus));
	if (wDataSize<sizeof(CMD_GR_UserStatus)) return false;

	//寻找用户
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserStatus->dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//设置状态
	if (pUserStatus->UserStatus.cbUserStatus==US_NULL) 
	{
		//删除用户
		m_PlazaUserManagerModule->DeleteUserItem(pIClientUserItem);

		//获取对象
		CServerListData * pServerListData=CServerListData::GetInstance();

		//人数更新
		pServerListData->SetServerOnLineCount(m_GameServer.wServerID,m_PlazaUserManagerModule->GetActiveUserCount());
	}
	else
	{
		//更新用户
		m_PlazaUserManagerModule->UpdateUserItemStatus(pIClientUserItem,&pUserStatus->UserStatus);
	}

	return true;
}

//用户游戏数据
bool CPlazaViewEntry::OnSocketSubUserItemGameData(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_UserGameData * pUserGameData=(CMD_GR_UserGameData *)pData;
	WORD wHeadSize=sizeof(CMD_GR_UserGameData)-sizeof(pUserGameData->szUserGameData);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+CountStringBuffer(pUserGameData->szUserGameData))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+CountStringBuffer(pUserGameData->szUserGameData)))) return false;

	//寻找用户
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserGameData->dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	return true;
}

//登录完成
bool CPlazaViewEntry::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize==sizeof(CMD_GR_LogonFinish));
	if(wDataSize!=sizeof(CMD_GR_LogonFinish)) return false;

	//提取数据
	CMD_GR_LogonFinish * pLogonFinish = (CMD_GR_LogonFinish *)pData;

	//关闭提示
	m_DlgStatus.HideStatusWindow();

	//设置状态
	SetServiceStatus(ServiceStatus_ServiceIng);

	//重入判断
	if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()!=INVALID_TABLE))
	{
		//启动进程	
		bool bSuccess=m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);

		//错误处理
		if (bSuccess==false)
		{
			//中断连接
			m_TCPSocketModule->CloseSocket();
			m_ServiceStatus=ServiceStatus_NetworkDown;
		}	
	}

	//追踪玩家
	CGlobalUnits * pGlobalUnits = CGlobalUnits::GetInstance();
	DWORD dwTrackUserID = pGlobalUnits->GetTrackUserID();
	if(pGlobalUnits!=NULL && dwTrackUserID!=0)
	{
		//激活用户
		ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
		IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwTrackUserID);
		if(pIClientUserItem && pIClientUserItem->GetTableID()!=INVALID_TABLE && pIClientUserItem->GetChairID()!=INVALID_CHAIR)
		{
			//变量定义
			tagSearchTable SearchTable;
			ZeroMemory(&SearchTable,sizeof(SearchTable));

			//搜索条件
			SearchTable.bAllNull=true;
			SearchTable.bNotFull=true;
			SearchTable.bOneNull=true;
			SearchTable.bFilterPass=true;
			SearchTable.bTwoNull=(m_pITableViewFrame->GetChairCount()!=2);

			//搜索结果
			SearchTable.wResultChairID=INVALID_CHAIR;
			SearchTable.wResultTableID=pIClientUserItem->GetTableID();			
			SearchTable.wStartTableID=SearchTable.wResultTableID;

			//搜索桌子
			SearchGameTable(SearchTable);
			if(SearchTable.wResultChairID!=INVALID_CHAIR && SearchTable.wResultTableID==SearchTable.wStartTableID  && CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==false)
			{
				//设置数据
				WORD wChairID=SearchTable.wResultChairID;
				// m_pITableViewFrame->VisibleTable(SearchTable.wResultTableID);
				m_pITableViewFrame->FlashGameChair(SearchTable.wResultTableID,wChairID);

				//执行坐下
				PerformSitDownAction(SearchTable.wResultTableID,wChairID,true);
			}
			else
			{
				//执行旁观
				// PerformLookonAction(SearchTable.wStartTableID,pIClientUserItem->GetChairID());
			}			

			return true;
		}
	}

	return true;
}

//登录成功
bool CPlazaViewEntry::OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_LogonSuccess));
	if (wDataSize!=sizeof(CMD_GR_LogonSuccess)) return false;

	//消息处理
	CMD_GR_LogonSuccess * pLogonSuccess=(CMD_GR_LogonSuccess *)pData;

	//玩家属性
	m_dwUserRight=pLogonSuccess->dwUserRight;
	m_dwMasterRight=pLogonSuccess->dwMasterRight;

	//设置状态
	SetServiceStatus(ServiceStatus_RecvInfo);

	//关闭提示
	// m_DlgUpdateStatus.StartEnterServer(/*TEXT("正在读取房间信息...")*/);

	//获取对象
	/* CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/GameRoom.aspx?KindID=%ld&ServerID=%ld"),pGlobalWebLink->GetPlatformLink(),m_GameKind.wKindID,m_GameServer.wServerID);

	////连接广告
	m_PlatformPublicize.Navigate(szBillUrl);
	m_PlatformPublicize.SetBoradColor(RGB(0,0,0));
	ASSERT(m_ImageGameName.IsNull()==true);
	if (m_ImageGameName.IsNull()==false) m_ImageGameName.DestroyImage();

	//工作路径
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//获取文件
	TCHAR szGameName[64]=TEXT("");
	CString strProcessName(m_GameKind.szProcessName);
	INT nPos=strProcessName.Find(TEXT("."));
	_sntprintf(szGameName,CountArray(szGameName),TEXT("%s"),strProcessName.Left(nPos));

	//构造路径
	TCHAR szFilePath[MAX_PATH]=TEXT("");
	_sntprintf(szFilePath,CountArray(szFilePath),TEXT("%s\\GameKindLogo\\Room_%s.png"),szDirectory,szGameName);

	//加载图片
	m_ImageGameName.LoadImage(szFilePath); */

	//登录成功处理
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	pPlatformFrame->PostMessage( WM_LOGON_SUCCESS, 0, 0);

	return true;
}

//登录失败
bool CPlazaViewEntry::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	//效验参数
	CMD_GR_LogonFailure * pLogonFailure=(CMD_GR_LogonFailure *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString))) return false;

	//关闭提示
	m_DlgStatus.HideStatusWindow();

	//关闭处理
	m_TCPSocketModule->CloseSocket();

	//设置状态
	SetServiceStatus(ServiceStatus_NetworkDown);

	//更新信息
	if(pLogonFailure->lErrorCode == LOGON_FAIL_SERVER_INVALIDATION)
	{
		//设置任务
		//CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
		//if (pPlatformFrame!=NULL) pPlatformFrame->ListGameServerInvalidation(m_GameKind.wKindID, m_GameServer.wServerID);
	}

	//显示消息
	LPCTSTR pszDescribeString=pLogonFailure->szDescribeString;
	if (lstrlen(pszDescribeString)>0) ShowInformation(pszDescribeString,MB_ICONERROR,60);

	//关闭房间 
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
	
	return true;
}

//更新提示
bool CPlazaViewEntry::OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UpdateNotify));
	if (wDataSize!=sizeof(CMD_GR_UpdateNotify)) return false;

	//变量定义
	CMD_GR_UpdateNotify * pUpdateNotify=(CMD_GR_UpdateNotify *)pData;

	//大厅更新
	if (pUpdateNotify->cbMustUpdatePlaza==TRUE)
	{
		//关闭处理
		m_DlgStatus.HideStatusWindow();
		m_TCPSocketModule->CloseSocket();

		//关闭房间
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//下载大厅
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		if (pGlobalUnits!=NULL) pGlobalUnits->DownLoadClient(TEXT("游戏广场"),0,0);

		return true;
	}

	//游戏更新
	if ((pUpdateNotify->cbMustUpdateClient==TRUE)||(pUpdateNotify->cbAdviceUpdateClient==TRUE))
	{
		//关闭处理
		if (pUpdateNotify->cbMustUpdateClient==TRUE)
		{
			m_DlgStatus.HideStatusWindow();
			m_TCPSocketModule->CloseSocket();
		}

		//构造提示
		if (pUpdateNotify->cbMustUpdateClient==FALSE)
		{
			//构造提示
			TCHAR szDescribe[512]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("“%s”已经更新了，我们强烈建议您进行更新，现在进行更新吗？"),m_GameKind.szKindName);

			//提示消息
			CInformation Information(this);
			if (Information.ShowMessageBox(szDescribe,MB_ICONINFORMATION|MB_YESNO,0)!=IDYES) return true;
		}

		//关闭房间
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//更新提示
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wKindID,m_GameServer.wServerID);
	}

	return true;
}

//请求失败
bool CPlazaViewEntry::OnSocketSubRequestFailure(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_RequestFailure * pRequestFailure=(CMD_GR_RequestFailure *)pData;

	//效验参数
	ASSERT(wDataSize>(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString)));
	if (wDataSize<=(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString))) return false;

	//变量定义
	IClientUserItem * pITableUserItem=NULL;
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
	WORD wMeChairID=m_pIMySelfUserItem->GetChairID();

	//界面还原
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_TABLE)&&(m_wReqChairID!=INDEX_ENTER_CHAIR))
	{
		IClientUserItem * pITableUserItem=m_pITableViewFrame->GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pITableUserItem==m_pIMySelfUserItem) m_pITableViewFrame->SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//设置变量
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;

	//提示消息
	if (pRequestFailure->szDescribeString[0]!=0)
	{
		CInformation Information;
		Information.ShowMessageBox(pRequestFailure->szDescribeString,MB_ICONINFORMATION,30L);
	}

	return true;
}

//用户聊天
bool CPlazaViewEntry::OnSocketSubUserChat(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_S_UserChat * pUserChat=(CMD_GR_S_UserChat *)pData;

	//效验参数
	ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	//效验参数
	if (wDataSize<(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserChat->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//消息过虑
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//社团变量
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//获取指针
	/* IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//显示消息
	if (pUserChat->dwTargetUserID!=0L)
	{
		//获取用户
		IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserChat->dwTargetUserID);
		if (pIRecvUserItem==NULL) return true;

		//显示消息
		LPCTSTR pszSendUser=pISendUserItem->GetNickName();
		LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
		pIStringMessage->InsertUserChat(pszSendUser,pszRecvUser,pUserChat->szChatString,pUserChat->dwChatColor);
	}
	else 
	{
		//显示消息
		LPCTSTR pszSendUser=pISendUserItem->GetNickName();
		pIStringMessage->InsertUserChat(pszSendUser,pUserChat->szChatString,pUserChat->dwChatColor);
	} */

	return true;
}

//用户表情
bool CPlazaViewEntry::OnSocketSubExpression(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_S_UserExpression));
	if (wDataSize!=sizeof(CMD_GR_S_UserExpression)) return false;

	//变量定义
	CMD_GR_S_UserExpression * pUserExpression=(CMD_GR_S_UserExpression *)pData;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//获取指针
	/* IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//消息过虑
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//社团变量
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	} */

	//公聊消息
	if (pUserExpression->dwTargetUserID==0L)
	{
		//变量定义
		CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
		CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

		//插入表情
		if (pExpressionItem!=NULL)
		{
			//获取路径
			TCHAR szImagePath[MAX_PATH]=TEXT("");
			pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

			//插入消息
			/* LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			pIStringMessage->InsertExpression(pszSendUser,szImagePath); */
		}
	}

	//私聊消息
	if (pUserExpression->dwTargetUserID!=0L)
	{
		//变量定义
		CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
		CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

		//插入表情
		if (pExpressionItem!=NULL)
		{
			//获取路径
			TCHAR szImagePath[MAX_PATH]=TEXT("");
			pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

			//获取用户
			IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserExpression->dwTargetUserID);
			if (pIRecvUserItem==NULL) return true;

			//显示消息
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
			// pIStringMessage->InsertExpression(pszSendUser,pszRecvUser,szImagePath);
		}
	}

	return true;
}

//用户私聊
bool CPlazaViewEntry::OnSocketSubWisperUserChat(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_S_WisperChat * pWisperChat=(CMD_GR_S_WisperChat *)pData;

	//效验参数
	ASSERT(wDataSize>=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0])));

	//效验参数
	if (wDataSize<(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0]))) return false;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperChat->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//接收用户
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperChat->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//消息过虑
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//社团变量
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}

		//屏蔽用户
		if(cbUserCompanion == CP_SHIELD) return true;
	}

	return true;
}

//私聊表情
bool CPlazaViewEntry::OnSocketSubWisperExpression(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_S_WisperExpression));
	if (wDataSize!=sizeof(CMD_GR_S_WisperExpression)) return false;

	//变量定义
	CMD_GR_S_WisperExpression * pWisperExpression=(CMD_GR_S_WisperExpression *)pData;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//接收用户
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperExpression->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//消息过虑
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//社团变量
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}

		//屏蔽用户
		if(cbUserCompanion == CP_SHIELD) return true;
	}

	//创建窗口
	/* DWORD dwUserID[2]={0L,0L};
	dwUserID[0]=m_pIMySelfUserItem->GetUserID();
	dwUserID[1]=(pISendUserItem==m_pIMySelfUserItem)?pIRecvUserItem->GetUserID():pISendUserItem->GetUserID();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,pWisperExpression->dwTargetUserID,dwUserID,CountArray(dwUserID),(pISendUserItem!=m_pIMySelfUserItem));

	//显示窗口
	if (pDlgWhisper!=NULL)
	{
		pDlgWhisper->OnEventExpression(pISendUserItem,pWisperExpression->wItemIndex);
	} */

	return true;
}

//道具成功
bool CPlazaViewEntry::OnSocketSubPropertySuccess(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_S_PropertySuccess));
	if (wDataSize!=sizeof(CMD_GR_S_PropertySuccess)) return false;

	//变量定义
	CMD_GR_S_PropertySuccess * pPropertySuccess=(CMD_GR_S_PropertySuccess *)pData;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertySuccess->dwSourceUserID);
	if (pISendUserItem==NULL) return true;

	//获取用户
	IClientUserItem * pTargetUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertySuccess->dwTargetUserID);
	if (pTargetUserItem==NULL) return true;

	//使用范围
	if(pPropertySuccess->cbRequestArea==PT_ISSUE_AREA_GAME && IsGameClientReady())
	{
		//设置变量
		IPC_GR_PropertySuccess  PropertySuccess;  
		PropertySuccess.wPropertyIndex=pPropertySuccess->wPropertyIndex;
		PropertySuccess.wPropertyCount=pPropertySuccess->wItemCount;
		PropertySuccess.dwSourceUserID=pPropertySuccess->dwSourceUserID;
		PropertySuccess.dwTargetUserID=pPropertySuccess->dwTargetUserID;
		CopyMemory(PropertySuccess.szSourceNickName,pISendUserItem->GetNickName(),sizeof(PropertySuccess.szSourceNickName));

		//发送数据
		bool bSendSuccess = m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_PROPERTY_SUCCESS,&PropertySuccess,sizeof(PropertySuccess));

		if(bSendSuccess==true) return true;
	}
	
	//获取对象
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//寻找道具
	ASSERT(pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex)!=NULL);
	CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex);

	//道具处理
	if (pGamePropertyItem!=NULL && pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PRESENT)
	{
		//图片信息
		tagPropertyImage PropertyImage;
		pGamePropertyItem->GetImageInfo(PropertyImage);

		//播放动画
		if ((PropertyImage.pszFlash!=NULL)&&(PropertyImage.pszFlash[0]!=0))
		{
			//工作路径
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//构造路径
			TCHAR szFlashPath[MAX_PATH]=TEXT("");
			_sntprintf(szFlashPath,CountArray(szFlashPath),TEXT("%s\\Property\\%s"),szDirectory,PropertyImage.pszFlash);

			/* try
			{
				//播放动画
				if (m_FlashControl.LoadMovieFile(szFlashPath)==false)
				{
					throw TEXT("动画文件加载失败");
				}

				//动画控件
				CRect rcWindow;
				m_pITableViewFrame->GetViewWnd()->GetWindowRect(&rcWindow);
				m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);

				//播放动画
				m_FlashControl.PlayMovie();
				m_FlashControl.ShowControl(SW_SHOW);
			}
			catch (...)
			{
				ASSERT(FALSE);
			} */
		}
	}

	//道具提示
	if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY)
	{
		//变量定义
		TCHAR  szMessage[256]=TEXT("");

		//获取提示
		if(pPropertySuccess->dwSourceUserID==m_pIMySelfUserItem->GetUserID() && pPropertySuccess->dwSourceUserID!=pPropertySuccess->dwTargetUserID)
            _sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您,道具购买成功!"));
		else
			pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),m_pIMySelfUserItem->GetNickName(),pPropertySuccess->wItemCount,szMessage);

		//弹出消息
		CInformation Information;
		Information.ShowMessageBox(szMessage,MB_ICONINFORMATION);

		//更新喇叭
		//if(m_pTrumpetItem!=NULL)  m_pTrumpetItem->UpdateControlSurface();
	}

	return true;
}

//道具失败
bool CPlazaViewEntry::OnSocketSubPropertyFailure(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_PropertyFailure * pPropertyFailure=(CMD_GR_PropertyFailure *)pData;
	if(pPropertyFailure==NULL) return false;

	//使用范围
	if(pPropertyFailure->wRequestArea==PT_ISSUE_AREA_GAME && IsGameClientReady())
	{
		//设置变量
		IPC_GR_PropertyFailure  PropertyFailure;  
		PropertyFailure.lErrorCode=pPropertyFailure->lErrorCode;
		CopyMemory(PropertyFailure.szDescribeString,pPropertyFailure->szDescribeString,sizeof(PropertyFailure.szDescribeString));

		//发送数据
		bool bSendSuccess = m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_PROPERTY_FAILURE,&PropertyFailure,sizeof(PropertyFailure));

		if(bSendSuccess==true) return true; 
	}

	//弹出消息
	CInformation Information;
	Information.ShowMessageBox(pPropertyFailure->szDescribeString,MB_ICONINFORMATION);

	return true;
}

//道具效应
bool CPlazaViewEntry::OnSocketSubPropertyEffect(VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize==sizeof(CMD_GR_S_PropertyEffect));
	if(wDataSize!=sizeof(CMD_GR_S_PropertyEffect))  return false;

	//变量定义
	CMD_GR_S_PropertyEffect * pPropertyEffect=(CMD_GR_S_PropertyEffect *)pData;
	if(pPropertyEffect==NULL) return false;

	//查找用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertyEffect->wUserID);
	if(pISendUserItem==NULL) return true;

	//设置变量
	tagUserInfo * pUserInfo = pISendUserItem->GetUserInfo();
	pUserInfo->cbMemberOrder=pPropertyEffect->cbMemberOrder;

	//更新信息
	// m_pPlazaViewServerCtrl->OnEventUserUpdate(pISendUserItem);
    
    return true;
}

//邀请玩家
bool CPlazaViewEntry::OnSocketSubUserInvite(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserInvite));
	if (wDataSize!=sizeof(CMD_GR_UserInvite)) return false;

	//消息处理
	CMD_GR_UserInvite * pUserInvite=(CMD_GR_UserInvite *)pData;
	ASSERT(pUserInvite->wTableID<m_pITableViewFrame->GetTableCount());
	if (m_pITableViewFrame->GetPlayFlag(pUserInvite->wTableID)==true) return true;

	//寻找用户
	IClientUserItem * pIUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserInvite->dwUserID);
	if (pIUserItem==NULL) return true;
	if (pIUserItem->GetTableID()==INVALID_TABLE) return true;

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//用户过虑
	if (pIUserItem->GetUserCompanion()==CP_DETEST) return true;
	if (pParameterGlobal->m_cbInviteMode==INVITE_MODE_NONE) return true;
	if ((pParameterGlobal->m_cbInviteMode==INVITE_MODE_FRIEND)&&(pIUserItem->GetUserCompanion()!=CP_FRIEND)) return true;

	//提示消息
	TCHAR szMessage[256]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 邀请您加入第 [ %ld ] 游戏桌进行游戏，是否同意？"),pIUserItem->GetNickName(),pUserInvite->wTableID+1);
	//提示消息
	CInformation Information(this);
	if (Information.ShowMessageBox(szMessage,MB_ICONINFORMATION|MB_YESNO)==IDYES)
	{
		WORD wChairID=INVALID_CHAIR;
		WORD wNullCount=m_pITableViewFrame->GetNullChairCount(pUserInvite->wTableID,wChairID);
		if (wNullCount==0) 
		{
			CInformation Information2(this);
			Information2.ShowMessageBox(TEXT("此游戏桌已经没有空位置了！"),MB_ICONINFORMATION);
			return true;
		}
		PerformSitDownAction(pUserInvite->wTableID,wChairID,false);
	}

	return true;
}

//道具消息
bool CPlazaViewEntry::OnSocketPropertyMessage(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_S_PropertyMessage));
	if (wDataSize!=sizeof(CMD_GR_S_PropertyMessage)) return false;

	//校验数据
	CMD_GR_S_PropertyMessage * pPropertyMessage = (CMD_GR_S_PropertyMessage *)pData;
	if(pPropertyMessage==NULL) return false;

    //获取对象
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//寻找道具
	ASSERT(pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex)!=NULL);
	CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex);
	if(pGamePropertyItem==NULL) return true;

	//类型判断
	if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY) return true;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertyMessage->dwSourceUserID);
	if (pISendUserItem==NULL) return true;

	//获取用户
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertyMessage->dwTargerUserID);
	if (pIRecvUserItem==NULL) return true;

	//提取信息
	TCHAR szNotifyMessage[128]=TEXT("");	
	pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),pIRecvUserItem->GetNickName(),pPropertyMessage->wPropertyCount,szNotifyMessage);

	//获取指针
	/* IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//插入消息
	pIStringMessage->InsertPromptString(szNotifyMessage);

	if(IsGameClientReady())
	{
		//构造结构
		IPC_GR_PropertyMessage  PropertyMessage;
		PropertyMessage.wPropertyIndex=pPropertyMessage->wPropertyIndex;
		PropertyMessage.wPropertyCount=pPropertyMessage->wPropertyCount;
		CopyMemory(PropertyMessage.szSourceNickName,pISendUserItem->GetNickName(),sizeof(PropertyMessage.szSourceNickName));
		CopyMemory(PropertyMessage.szTargerNickName,pIRecvUserItem->GetNickName(),sizeof(PropertyMessage.szTargerNickName));

		//发送数据
		m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_MESSAGE,&PropertyMessage,sizeof(PropertyMessage));
	} */

	return true;
}

//道具喇叭
bool CPlazaViewEntry::OnSocketPropertyTrumpet(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_S_SendTrumpet));
	if (wDataSize!=sizeof(CMD_GR_S_SendTrumpet)) return false;

	//变量定义
	CMD_GR_S_SendTrumpet * pSendTrumpet=(CMD_GR_S_SendTrumpet *)pData;

	//获取指针
	/* IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

    //插入消息
	if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)
		pIStringMessage->InsertUserTrumpet(pSendTrumpet->szSendNickName,pSendTrumpet->szTrumpetContent,pSendTrumpet->TrumpetColor);

	//插入消息
	if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TYPHON)
        pIStringMessage->InsertUserTyphon(pSendTrumpet->szSendNickName,pSendTrumpet->szTrumpetContent,pSendTrumpet->TrumpetColor);

	//更新包裹
	if(pSendTrumpet->dwSendUserID==m_pIMySelfUserItem->GetUserID())
	{
		//道具包裹
		tagPropertyPackage * pPropertyPackage=m_pIMySelfUserItem->GetPropertyPackage();

		//更新包裹
		if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET) --pPropertyPackage->wTrumpetCount;
		if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TYPHON) --pPropertyPackage->wTyphonCount;
		//if(m_pTrumpetItem!=NULL) m_pTrumpetItem->UpdateControlSurface();
	} */

	if(IsGameClientReady())
	{
		//构造结构
		IPC_GR_PropertyTrumpet  PropertyTrumpet;
		PropertyTrumpet.dwSendUserID=pSendTrumpet->dwSendUserID;
		PropertyTrumpet.wPropertyIndex=pSendTrumpet->wPropertyIndex;
		PropertyTrumpet.TrumpetColor=pSendTrumpet->TrumpetColor;
		CopyMemory(PropertyTrumpet.szSendNickName,pSendTrumpet->szSendNickName,sizeof(PropertyTrumpet.szSendNickName));
		CopyMemory(PropertyTrumpet.szTrumpetContent,pSendTrumpet->szTrumpetContent,sizeof(PropertyTrumpet.szTrumpetContent));

		//发送数据
		m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_TRUMPET,&PropertyTrumpet,sizeof(PropertyTrumpet));
	}

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//播放声音
	if(pParameterGlobal->m_bAllowSound)
	{
		//变量定义
		HINSTANCE hInstance = AfxGetInstanceHandle();
		bool bPlaying=false;

		//获取对象
		CD3DSound * pD3DSound=CD3DSound::GetInstance();
		if(pD3DSound)
		{
			bPlaying = pD3DSound->PlaySound(hInstance, (pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?TEXT("BUGLE"):TEXT("BUGLE_EX"), TEXT("WAVE"), false);
		}

		//系统播放
		if(!bPlaying)
		{
			HRSRC hResour=FindResource(hInstance,(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?TEXT("BUGLE"):TEXT("BUGLE_EX"),TEXT("WAVE"));
			if (hResour!=NULL)
			{
				HGLOBAL hGlobal=LoadResource(hInstance,hResour);
				if (hGlobal!=NULL) 
				{
					//播放声音
					LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
					BOOL bResult = ::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY|SND_NODEFAULT);

					//清理资源
					UnlockResource(hGlobal);
				}
				FreeResource(hGlobal);
			}
		}
	}

    return true;
}

//等待分配
bool CPlazaViewEntry::OnSocketSubWaitDistribute(VOID *pData, WORD wDataSize)
{
	//更新桌面
	m_pITableViewFrame->SetTableStatus(true);

	//游戏通知
	OnSocketMainGameFrame(MDM_GF_FRAME,SUB_GR_USER_WAIT_DISTRIBUTE,pData,wDataSize);

	return true;
}

//系统消息
bool CPlazaViewEntry::OnSocketSubSystemMessage(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR)))) return false;

	WORD wType = pSystemMessage->wType;

	//关闭处理
	if ((wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//关闭提示
		m_DlgStatus.HideStatusWindow();

		//关闭处理
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//状态设置
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//获取指针
	/* IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//显示消息
	if (wType&SMT_CHAT) 
	{
		pIStringMessage->InsertSystemString(pSystemMessage->szString);
	}

	//关闭银行
	if (wType&SMT_CLOSE_INSURE)
	{
		if(m_pDlgInsureServer != NULL) 
		{
			m_pDlgInsureServer->EndDialog(IDOK);
			m_pDlgInsureServer->DestroyWindow();
		}
	} */

	//关闭游戏
	if (wType&SMT_CLOSE_GAME)
	{
		m_ProcessManager.CloseGameProcess(false);
	}

	//弹出消息
	if (wType&SMT_EJECT)
	{
		CInformation Information;
		Information.ShowMessageBox(pSystemMessage->szString,MB_ICONINFORMATION);
	}

	//关闭房间
	if (wType&SMT_CLOSE_ROOM)
	{
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
	}

	return true;
}

//动作消息
bool CPlazaViewEntry::OnSocketSubActionMessage(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize>=(wHeadSize+pActionMessage->wLength*sizeof(WCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize<(wHeadSize+pActionMessage->wLength*sizeof(WCHAR)))) return false;

	//关闭处理
	if ((pActionMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//关闭提示
		m_DlgStatus.HideStatusWindow();

		//关闭处理
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//状态设置
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//弹出消息
	CInformation Information;
	INT nResultCode=Information.ShowMessageBox(pActionMessage->szString,MB_ICONINFORMATION);

	//变量定义
	WORD wExcursion=wHeadSize+pActionMessage->wLength*sizeof(TCHAR);

	//提取动作
	while (wExcursion<wDataSize)
	{
		//变量定义
		tagActionHead * pActionHead=(tagActionHead *)((BYTE *)pData+wExcursion);

		//效验参数
		ASSERT((wExcursion+sizeof(tagActionHead))<=wDataSize);
		ASSERT((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)<=wDataSize);

		//效验参数
		if ((wExcursion+sizeof(tagActionHead))>wDataSize) return false;
		if ((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)>wDataSize) return false;

		//动作出来
		if (nResultCode==pActionHead->uResponseID)
		{
			switch (pActionHead->cbActionType)
			{
			case ACT_BROWSE:	//浏览动作
				{
					//变量定义
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionBrowse * pActionBrowse=(tagActionBrowse *)((BYTE *)pData+wDataPos);

					//I E 浏览
					if (pActionBrowse->cbBrowseType&BRT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionBrowse->szBrowseUrl,NULL,NULL,SW_NORMAL);
					}

					//大厅浏览
					/* if (pActionBrowse->cbBrowseType&BRT_PLAZA)
					{
						CPlatformFrame::GetInstance()->WebBrowse(pActionBrowse->szBrowseUrl,true);
					} */

					break;
				}
			case ACT_DOWN_LOAD:	//下载动作
				{
					//变量定义
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionDownLoad * pActionDownLoad=(tagActionDownLoad *)((BYTE *)pData+wDataPos);

					//I E 下载
					if (pActionDownLoad->cbDownLoadMode&DLT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					//下载模块
					if (pActionDownLoad->cbDownLoadMode&DLT_MODULE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					break;
				}
			}
		}

		//增加偏移
		wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	}

	//关闭房间
	if (pActionMessage->wType&SMT_CLOSE_ROOM) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//设置状态
VOID CPlazaViewEntry::SetServiceStatus(enServiceStatus ServiceStatus)
{
	//设置变量
	m_ServiceStatus=ServiceStatus;

	return;
}

//查询游戏客户端
bool CPlazaViewEntry::IsGameClientReady()
{
	return m_ProcessManager.IsProcessReady();
}

//资源目录
VOID CPlazaViewEntry::GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount)
{
	//变量定义
	WORD wStringIndex=0;

	//构造目录
	ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szResDirectory,m_GameKind.szProcessName,wBufferCount);
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

	//字符终止
	szResDirectory[wStringIndex]=0;

	return;
}

//查找桌子
VOID CPlazaViewEntry::SearchGameTable(tagSearchTable & SearchTable)
{
	//变量定义
	WORD wNullCount = 0;
	WORD wChairUser = m_pITableViewFrame->GetChairCount();
	WORD wMeTableID = INVALID_TABLE;
	if (m_pIMySelfUserItem!=NULL) {
		wMeTableID = m_pIMySelfUserItem->GetTableID();
	}

	//百人游戏
	if (CServerRule::IsAllowDynamicJoin(m_dwServerRule)==true)
	{
		if(m_pITableViewFrame->GetChairCount() >= INDEX_ENTER_CHAIR)
		{
			SearchTable.wResultTableID=0;
			SearchTable.wResultChairID=INDEX_ENTER_CHAIR;

			return;
		}
	}

	//搜索桌子
	for (WORD i=0;i<m_pITableViewFrame->GetTableCount();i++)
	{
		//设置索引
		SearchTable.wResultTableID=(SearchTable.wStartTableID+i)%m_pITableViewFrame->GetTableCount();

		//搜索处理
		if (wMeTableID!=SearchTable.wResultTableID)
		{
			//获取桌子
			ITableView * pITableView=m_pITableViewFrame->GetTableViewItem(SearchTable.wResultTableID);

			//搜索过虑
			if (pITableView->GetPlayFlag()==true) continue;
			if ((SearchTable.bFilterPass==true)&&(pITableView->GetLockerFlag()==true)) continue;

			//寻找空位置
			wNullCount=pITableView->GetNullChairCount(SearchTable.wResultChairID);

			//判断数目
			if (wNullCount>0)
			{
				//效验规则
				CString strDescribe;
				if (EfficacyTableRule(SearchTable.wResultTableID,SearchTable.wResultChairID,false,strDescribe)==false)
				{
					continue;
				}
		
				//数目判断
				if ((SearchTable.bOneNull==true)&&(wNullCount==1)) return;
				if ((SearchTable.bTwoNull==true)&&(wNullCount==2)) return;
				if ((SearchTable.bNotFull==true)&&(wNullCount<wChairUser)) return;
				if ((SearchTable.bAllNull==true)&&(wNullCount==wChairUser)) return;
			}
		}
	}

	//设置数据
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;

	return;
}

//取消连接
VOID CPlazaViewEntry::OnStatusCancel()
{
	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}