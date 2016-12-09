#include "Stdafx.h"
#include "PlazaViewEntry.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "TableViewFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//�ؼ���ʶ
#define IDC_SERVER_TABLE			101									//���ӿؼ�

//////////////////////////////////////////////////////////////////////////////////

//�ؼ�����
const TCHAR* const szButtonGoBackControlName	= TEXT("ButtonEnter");
const TCHAR* const szButtonQuitControlName		= TEXT("ButtonQuit");
const TCHAR* const szLabelLoadingControlName	= TEXT("LabelLoading");

//////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CPlazaViewEntry, CFGuiWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////
CPlazaViewEntry::CPlazaViewEntry()
{
	//��������
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;
	m_wResultTableID=INVALID_TABLE;
	m_wInvitedTableID=INVALID_TABLE;
	m_bUserInRoom=false;

	//�û�����
	m_dwUserRight=0L;
	m_dwMasterRight=0L;
	m_pIMySelfUserItem=NULL;

	//��Ϸ����
	m_pITableViewFrame=NULL;

	//��������
	m_wServerType=0L;
	m_dwServerRule=0L;
	m_ServiceStatus=ServiceStatus_Unknow;
	m_wAVServerPort=0;
	m_dwAVServerAddr=0;
	m_szServerPassword[0]=0;

	//���ò���
	m_pParameterGame=NULL;
	m_pParameterServer=NULL;

	//�������
	m_DlgStatus.SetStatusViewSink(this);

	//������Ϣ
	m_cbColumnCount=0;
	m_cbPropertyCount=0;
	ZeroMemory(m_ColumnItem,sizeof(m_ColumnItem));
	ZeroMemory(m_PropertyInfo,sizeof(m_PropertyInfo));

	//��������
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));
}

//��������
CPlazaViewEntry::~CPlazaViewEntry()
{

}

VOID CPlazaViewEntry::Release()
{
	//��ͼ����
	if (m_pITableViewFrame) 
	{
		m_pITableViewFrame->Release();
		m_pITableViewFrame = NULL;
	}

	//�������
	m_GameLevelParserModule.CloseInstance();

	//�û�����
	m_PlazaUserManagerModule.CloseInstance();
	m_pIMySelfUserItem = NULL;

	//�ر�������
	m_TCPSocketModule.CloseInstance();
}

//�ӿڲ�ѯ
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
	//��ȡ����
	//////////////////////////////////////////////////////////////////////////////////////////////
	m_PaintManager.AddFontAt(0,TEXT("����"), 14, false, false, false);
	m_PaintManager.AddFontAt(1,TEXT("����"), 16, false, false, false);

	//////////////////////////////////////////////////////////////////////////////////////////////
	CControlUI * pParent = static_cast<CControlUI *>( m_PaintManager.GetRoot() );
	if(pParent==NULL) return;

	// pLabelLoading->SetVisible(false);

	//������Ϸ
	CButtonUI * pButtonEnter = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonEnter") );
	if( pButtonEnter != NULL )  {
		pButtonEnter->SetFloat(true);
		pButtonEnter->SetPos(175,360);		
		pButtonEnter->SetFixedWidth(355);
		pButtonEnter->SetFixedHeight(315);
		pButtonEnter->SetStatusImage( TEXT("file='BT_JOIN' restype='PNG'") );
	}

	//�˳���ť
	CButtonUI * pButtonQuit = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonQuit") );
	if( pButtonQuit != NULL )  {
		pButtonQuit->SetFloat(true);
		pButtonQuit->SetPos(405,360);
		pButtonQuit->SetFixedWidth(585);
		pButtonQuit->SetFixedHeight(315);
		pButtonQuit->SetStatusImage( TEXT("file='BT_QUIT' restype='PNG'") );
	}

	//��¼����
	CButtonUI * pLabelLoading = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("LabelLoading") );
	if( pLabelLoading != NULL )  {
		pLabelLoading->SetFloat(true);
		pLabelLoading->SetPos(175,360);
		pLabelLoading->SetFixedWidth(555);
		pLabelLoading->SetFixedHeight(285);
		pLabelLoading->SetStatusImage( TEXT("file='LB_LOADING' restype='PNG'") );
	}

}

//��Ϣ����
void CPlazaViewEntry::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	if (pPlatformFrame==NULL) return;

	//����¼�
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


//�����滭
void CPlazaViewEntry::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	m_ImageBack.DrawImage(pDC,0,0);
}


//������ͼ
bool CPlazaViewEntry::CreateViewFrame()
{
	//״̬��ʾ
	m_DlgStatus.SetStatusViewSink(this);

	//������ʾ
	m_DlgStatus.ShowStatusWindow(TEXT("������Ϸ���䣬���Ժ�..."));

	try
	{
		//��������
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

		//��������
		m_pITableViewFrame->CreateTableFrame(this,IDC_SERVER_TABLE,QUERY_ME_INTERFACE(IUnknownEx));

		//������ʾ
		m_DlgStatus.ShowStatusWindow(TEXT("������Ϸ�������..."));

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
	//��ȡ����
	CContainerUI * pButtonGoBack = (CContainerUI *) GetControlByName( szButtonGoBackControlName );
	if(pButtonGoBack==NULL) return;
	pButtonGoBack->SetVisible(!isGameing);

	//��ȡ����
	CContainerUI * pButtonQuit = (CContainerUI *) GetControlByName( szButtonQuitControlName );
	if(pButtonQuit==NULL) return;
	pButtonQuit->SetVisible(!isGameing);

	//��ȡ����
	CContainerUI * pLabelLoading = (CContainerUI *) GetControlByName( szLabelLoadingControlName );
	if(pLabelLoading==NULL) return;
	pLabelLoading->SetVisible(isGameing);
}

//������Ϸ
VOID CPlazaViewEntry::InitGameServer(  CGameServerItem * pServerItem )
{
	//״̬��ʾ
	m_DlgStatus.ShowStatusWindow(TEXT("��ʼ����Ϸ����..."));

	//��Ϸ����
	m_GameServer = pServerItem->m_GameServer;
	
	CGameKindItem * pGameKindItem = pServerItem->m_pGameKindItem;
	if (pGameKindItem==NULL) return;
	m_GameKind = pGameKindItem->m_GameKind;

	//�������
	if (m_GameLevelParserModule.CreateInstance()==false) throw TEXT("�ȼ������������ʧ�ܣ�");
	if (m_PlazaUserManagerModule.CreateInstance()==false) throw TEXT("�û������������ʧ�ܣ�");
	
	//�������
	m_ProcessManager.SetProcessManagerSink(QUERY_ME_INTERFACE(IUnknownEx));
	
	//�������
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
	CUserInformation * pUserInformation=CUserInformation::GetInstance();
	if (m_PlazaUserManagerModule->SetUserManagerSink(pIUnknownEx)==false) throw TEXT("�û������������ʧ�ܣ�");
	if (m_PlazaUserManagerModule->SetUserInformation(pUserInformation)==false) throw TEXT("�û������������ʧ�ܣ�");
	
	//�������
	if (m_TCPSocketModule.CreateInstance()==false) {
		throw TEXT("�޷��������������������Ϸ�������ʧ�ܣ�");
	}
	
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	m_TCPSocketModule->SetTCPSocketSink(QUERY_ME_INTERFACE(IUnknownEx));
	m_TCPSocketModule->SetProxyServerInfo(pParameterGlobal->m_cbProxyType,pParameterGlobal->m_ProxyServer);

	//��������
	if (m_TCPSocketModule->Connect(m_GameServer.szServerAddr,m_GameServer.wServerPort)!=CONNECT_SUCCESS)
	{
		throw TEXT("��Ϸ��������ʧ�ܣ�����ϵͳ�������ò����Ƿ���ȷ��");
		return;
	}

	ResetViewFrame(false);

	m_DlgStatus.ShowStatusWindow(TEXT("��ʼ����Ϸ��������...") );
}

//������Ϸ
VOID CPlazaViewEntry::PerformJoinGame()
{
	//��������
	tagSearchTable SearchTable;
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;
	SearchTable.wStartTableID=0;
	SearchGameTable(SearchTable);
	
	//û����λ��
	/* if(SearchTable.bNotFull==false) 
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox( TEXT("δ�ҵ����ʵķ������λ����ѡ��������Ϸ��...!") );
		throw(TEXT("..."));
		return;
	} */

	//����λ��
	WORD wChairID=INVALID_CHAIR;
	m_wResultTableID = SearchTable.wResultTableID;
	m_pITableViewFrame->GetNullChairCount(m_wResultTableID,wChairID);

	PerformSitDownAction(m_wResultTableID, wChairID, false);
}

//�����ر�
bool CPlazaViewEntry::PlazaClosing()
{
	return m_ProcessManager.IsProcessReady();
}

//���˫��
VOID CPlazaViewEntry::OnDButtonHitTable(WORD wTableID)
{
	return;
}

//�������
VOID CPlazaViewEntry::OnLButtonHitTable(WORD wTableID, WORD wChairID)
{
	//��������
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return;
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return;
	if ((wTableID==m_wReqTableID)&&(wChairID==m_wReqChairID)) return;

	//��ȡ�û�
	IClientUserItem * pTableUserItem=NULL;
	if (wChairID!=INDEX_ENTER_CHAIR) pTableUserItem=m_pITableViewFrame->GetClientUserItem(wTableID,wChairID);

	//�����λ
	if (CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==true)
	{
		PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
		return;
	}

	//��Ϣ����
	if (pTableUserItem!=NULL)
	{
		if (pTableUserItem==m_pIMySelfUserItem)
		{
			//״̬����
			if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING) return;
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) return;

			//��������
			PerformStandUpAction(wTableID,wChairID);
		}
		else
		{
			//״̬����
			WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
			WORD wMeChairID=m_pIMySelfUserItem->GetChairID();
			if ((wMeTableID==wTableID)&&(wMeChairID==wChairID)) return;
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) return;

			//�Թ۶���
			// PerformLookonAction(wTableID,wChairID);
		}
	}
	else
	{
		//�������
		if (wChairID==INDEX_ENTER_CHAIR)
		{
			//��������
			WORD wUserTableID=m_pIMySelfUserItem->GetTableID();
			BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

			//�������
			if ((wTableID==wUserTableID)&&(cbUserStatus!=US_LOOKON))
			{
				m_ProcessManager.SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_ACTIVE_PROCESS);
				return;
			}
		}

		//���¶���
		PerformSitDownAction(wTableID,wChairID,true);
	}

	return;
}

//�Ҽ�����
VOID CPlazaViewEntry::OnRButtonHitTable(WORD wTableID, WORD wChairID)
{
	//���λ��
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR)&&(wChairID!=INDEX_ENTER_CHAIR))
	{
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=m_pITableViewFrame->GetClientUserItem(wTableID,wChairID);

		//�û��˵�
		if (pIClientUserItem!=NULL)
		{
			// TrackUserItemMenu(pIClientUserItem);
		}

		return;
	}

	return;
}

//���Ӱ�ť
VOID CPlazaViewEntry::OnHitTableButton(BYTE cbButton)
{
	//��ť����
	switch (cbButton)
	{
	case BT_ENTER_GAME:		//������Ϸ
		{
			//������Ϸ
			if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()==INVALID_TABLE))
			{
				PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
			}

			break;
		}
	case BT_LEAVE_MATCH:	//�˳���Ϸ
		{
			//�˳���Ϸ
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

//λ����Ϣ
VOID CPlazaViewEntry::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);
	return;
}

//������Ϣ
INT CPlazaViewEntry::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	//������Դ
	m_ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("MATCH_BK"));

	//���ô�С
	CSize SizeWindow(m_ImageBack.GetWidth(),m_ImageBack.GetHeight());
	SetWindowPos(NULL, 0, 0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	
	return 0;
}


//�����¼�
bool CPlazaViewEntry::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
	//������ʾ
	if (nErrorCode!=0L)
	{
		//�ر���ʾ
		m_DlgStatus.HideStatusWindow();

		//�رշ���
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("�ܱ�Ǹ����Ϸ��������ʧ�ܣ����Ժ����Ի�������վ���棡"),MB_ICONSTOP);

		return true;
	}

	//���͵�¼
	SendLogonPacket(m_szServerPassword);

	//����״̬
	SetServiceStatus(ServiceStatus_Validate);

	return true;
}

//�ر��¼�
bool CPlazaViewEntry::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	//�ر�״̬
	m_DlgStatus.HideStatusWindow();

	//�ر���Ϸ
	m_ProcessManager.CloseGameProcess(false);

	//�رմ���
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		if (m_ServiceStatus==ServiceStatus_ServiceIng)
		{
			//��ȡָ��
			// IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
			// ASSERT(pIStringMessage!=NULL);

			//��ʾ��Ϣ
			// pIStringMessage->InsertSystemString(TEXT("����Ϸ�������������ж���"));

			//�ر���ʾ
			CInformation Information(this);
			INT_PTR nResult=Information.ShowMessageBox(m_GameServer.szServerName,TEXT("����Ϸ�������������Ѿ��жϣ��Ƿ�رյ�ǰ��Ϸ���䣿"),
				MB_ICONERROR|MB_YESNO|MB_DEFBUTTON2,0L);

			//�رշ���
			if (nResult==IDYES)
			{
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM);
			}
		}
		else
		{
			//�ر���ʾ
			CInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,TEXT("���������ԭ�򣬷�������ʧ�ܣ����Ժ����³��ԣ�"),MB_ICONERROR);

			//�رշ���
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
		}
	}

	//����״̬
	m_ServiceStatus=ServiceStatus_NetworkDown;

	return true;
}

//��ȡ�¼�
bool CPlazaViewEntry::OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	try
	{
		bool br=false;
		switch (Command.wMainCmdID)
		{
		case MDM_GR_LOGON:		//��¼��Ϣ
			{
				br=OnSocketMainLogon(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_CONFIG:		//������Ϣ
			{
				br=OnSocketMainConfig(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_USER:		//�û���Ϣ
			{
				br=OnSocketMainUser(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_STATUS:		//״̬��Ϣ
			{
				br=OnSocketMainStatus(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_INSURE:		//������Ϣ
			{
				br=OnSocketMainInsure(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_TASK:		//������Ϣ
			{
				br=OnSocketMainTask(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_EXCHANGE:	//�һ���Ϣ
			{
				br=OnSocketMainExchange(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_MANAGE:		//������Ϣ
			{
				br=OnSocketMainManager(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_CM_SYSTEM:		//ϵͳ��Ϣ
			{
				br=OnSocketMainSystem(Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GF_GAME:		//��Ϸ��Ϣ
		case MDM_GF_FRAME:		//�����Ϣ
			{
				br=OnSocketMainGameFrame(Command.wMainCmdID,Command.wSubCmdID,pData,wDataSize);

				return true;
			}
		case MDM_GR_MATCH:		//������Ϣ
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


//���̴���
bool CPlazaViewEntry::OnGameProcessError()
{
	OnGameProcessClose(0);

	return true;
}

//���̹ر�
bool CPlazaViewEntry::OnGameProcessClose(DWORD dwExitCode)
{
	//Ч��״̬
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return false;

	//��������
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();
	BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

	//�˳���Ϸ
	if ((wTableID!=INVALID_TABLE)&&(m_ServiceStatus==ServiceStatus_ServiceIng))
	{
		SendStandUpPacket(wTableID,wChairID,TRUE);
	}

	m_pITableViewFrame->SetTableStatus(false);

	//��ԭ����
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	pPlatformFrame->PostMessage(WM_COMMAND,IDM_RESTORE,0);
	pPlatformFrame->m_PlazaViewUser.PostMessage(WM_UPDATE_USERINFO,0,0);

	//��ͼ����
	ResetViewFrame(false);

	return true;
}

//��������
bool CPlazaViewEntry::OnGameProcessCreate(bool bAlreadyExist)
{
	//��������
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();

	//��������
	if (bAlreadyExist==false)
	{
		//��ȡ�ȼ�
		tagLevelItem LevelItem[64];
		BYTE cbLevelCount=(BYTE)m_GameLevelParserModule->GetGameLevelItem(LevelItem,CountArray(LevelItem));

		//�ȼ�����
		WORD wLevelCount;
		tagGrowLevelConfig GrowLevelItem[60];
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		wLevelCount=pUserItemElement->GetExperienceInfo(GrowLevelItem,CountArray(GrowLevelItem));

		//��������
		m_ProcessManager.SendLevelInfo(LevelItem,cbLevelCount);
		m_ProcessManager.SendColumnInfo(m_ColumnItem,m_cbColumnCount);
		m_ProcessManager.SendGrowLevelConfig(GrowLevelItem,wLevelCount);		
		m_ProcessManager.SendPropertyInfo(m_PropertyInfo,m_cbPropertyCount);
	}

	//������Ϣ
	m_ProcessManager.SendServerInfo(m_pIMySelfUserItem,m_dwUserRight,m_dwMasterRight,m_dwServerRule,m_wServerType,m_GameServer,m_wAVServerPort,m_dwAVServerAddr);
	
	//������Ϣ
	CServerListData * pServerListData = CServerListData::GetInstance();
	CGameServerItem * pGameServerItem=pServerListData->SearchGameServer(m_GameServer.wServerID);
	if (pGameServerItem!=NULL) m_ProcessManager.SendMatchInfo(pGameServerItem->m_GameMatch);

	//�����û�
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		//��������
		WORD wEnumIndex=0;
		IPlazaUserManager * pIPlazaUserManager=m_PlazaUserManagerModule.GetInterface();

		//�������
		while (true)
		{
			//��ȡ�û�
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//�����ж�
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()==US_LOOKON) continue;

			//�����û�
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};

		//�Թ��û�
		wEnumIndex=0;
		while (true)
		{
			//��ȡ�û�
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//�����ж�
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()!=US_LOOKON) continue;

			//�����û�
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};
	}

	//�������
	m_ProcessManager.SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_CONFIG_FINISH);
	
	//��ԭ����
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	pPlatformFrame->PostMessage(WM_COMMAND,IDM_MINIMIZE,0);

	return true;
}

//��������
bool CPlazaViewEntry::OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//�û�����
	if ((wMainCmdID==IPC_CMD_GF_USER_INFO)&&(wSubCmdID==IPC_SUB_GF_USER_ATTRIB))
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(IPC_GF_UserAttrib));
		if (wDataSize!=sizeof(IPC_GF_UserAttrib)) return false;

		//Ѱ���û�
		IPC_GF_UserAttrib * pUserAttrib=(IPC_GF_UserAttrib *)pData;
		IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserAttrib->dwUserID);

		//�û��ж�
		ASSERT(pIClientUserItem!=NULL);
		if (pIClientUserItem==NULL) return true;

		//��������
		ASSERT(CUserInformation::GetInstance()!=NULL);
		CUserInformation * pUserInformation=CUserInformation::GetInstance();

		//���ù�ϵ
		m_PlazaUserManagerModule->UpdateUserItemAttrib(pIClientUserItem,&pUserAttrib->UserAttrib);
		pUserInformation->InsertCompanionInfo(pIClientUserItem,pUserAttrib->UserAttrib.cbCompanion);

		return true;
	}

	//������Ϣ
	if ((wMainCmdID==IPC_CMD_GF_SOCKET)&&(wSubCmdID==IPC_SUB_GF_SOCKET_SEND))
	{
		//Ч������
		ASSERT(wDataSize>=sizeof(TCP_Command));
		if (wDataSize<sizeof(TCP_Command)) return false;

		//��������
		IPC_GF_SocketSend * pSocketPackage=(IPC_GF_SocketSend *)pData;
		WORD wPacketSize=wDataSize-(sizeof(IPC_GF_SocketSend)-sizeof(pSocketPackage->cbBuffer));

		//��������
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
				//��������
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//��������
				CMD_GR_C_QueryInsureInfoRequest *pQueryInsureInfo=(CMD_GR_C_QueryInsureInfoRequest *)pSocketPackage->cbBuffer;
				pQueryInsureInfo->cbActivityGame=TRUE;
				lstrcpyn(pQueryInsureInfo->szInsurePass,pGlobalUserData->szPassword,CountArray(pQueryInsureInfo->szInsurePass));
			}
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID,pSocketPackage->cbBuffer,wPacketSize);
		}

		return true;
	}

	//�������
	ASSERT(FALSE);

	return false;
}


//��������
bool CPlazaViewEntry::SendUserRulePacket()
{
	//��������
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	CMD_GR_UserRule * pUserRule=(CMD_GR_UserRule *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GR_UserRule),sizeof(cbBuffer)-sizeof(CMD_GR_UserRule));

	//��������
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	pUserRule->cbRuleMask=0x00;

	//���Ƶ�ַ
	if (pParameterGlobal->m_bLimitSameIP==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_SAME_IP;
	}

	//����ʤ��
	if (m_pParameterGame->m_bLimitWinRate==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_WIN_RATE;
		pUserRule->wMinWinRate=m_pParameterGame->m_wMinWinRate;
	}

	//��������
	if (m_pParameterGame->m_bLimitFleeRate==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_FLEE_RATE;
		pUserRule->wMaxFleeRate=m_pParameterGame->m_wMaxFleeRate;
	}

	//���ƻ���
	if (m_pParameterGame->m_bLimitGameScore==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_GAME_SCORE;
		pUserRule->lMaxGameScore=m_pParameterGame->m_lMaxGameScore;
		pUserRule->lMinGameScore=m_pParameterGame->m_lMinGameScore;
	}

	//Я������
	if ((CServerRule::IsForfendLockTable(m_dwServerRule)==false)&&(m_pParameterServer->m_bTakePassword==true))
	{
		SendPacket.AddPacket(m_pParameterServer->m_szPassword,DTP_GR_TABLE_PASSWORD);
	}

	//��������
	WORD wSendSize=sizeof(CMD_GR_UserRule)+SendPacket.GetDataSize();
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_RULE,cbBuffer,wSendSize);

	return true;
}

//���͵�¼
bool CPlazaViewEntry::SendLogonPacket(LPCTSTR pszServerPasswd)
{
	//��������
	CMD_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//��������
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��Ϸ�汾
	LPCTSTR pszProcessName=m_GameKind.szProcessName;
	CWHService::GetModuleVersion(pszProcessName,LogonUserID.dwProcessVersion);

	//������Ϣ
	LogonUserID.dwPlazaVersion=pGlobalUnits->GetPlazaVersion();
	LogonUserID.dwFrameVersion=pGlobalUnits->GetFrameVersion();

	//��¼��Ϣ
	LogonUserID.wKindID=m_GameServer.wKindID;
	LogonUserID.dwUserID=pGlobalUserData->dwUserID;	
	lstrcpyn(LogonUserID.szServerPasswd,pszServerPasswd,CountArray(LogonUserID.szServerPasswd));
	lstrcpyn(LogonUserID.szPassword,pGlobalUserData->szDynamicPass,CountArray(LogonUserID.szPassword));	

	//������ʶ
	CWHService::GetMachineIDEx(LogonUserID.szMachineID);

	//��������
	m_TCPSocketModule->SendData(MDM_GR_LOGON,SUB_GR_LOGON_USERID,&LogonUserID,sizeof(LogonUserID));

	return true;
}

//�����Թ�
bool CPlazaViewEntry::SendLookonPacket(WORD wTableID, WORD wChairID)
{
	//��������
	CMD_GR_UserLookon UserLookon;
	ZeroMemory(&UserLookon,sizeof(UserLookon));

	//��������
	UserLookon.wTableID=wTableID;
	UserLookon.wChairID=wChairID;

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_LOOKON,&UserLookon,sizeof(UserLookon));

	return true;
}

//ִ������
bool CPlazaViewEntry::PerformStandUpAction(WORD wTableID, WORD wChairID)
{
	//Ч������
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID==wTableID)&&(m_wReqChairID==wChairID)) return false;

	//�Լ�״̬
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"));

		return false;
	}

	//���ñ���
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;

	//���ý���
	// m_pITableViewFrame->VisibleTable(wTableID);

	//��������
	SendStandUpPacket(wTableID,wChairID,FALSE);

	return true;
}

//��������
bool CPlazaViewEntry::SendSitDownPacket(WORD wTableID, WORD wChairID, LPCTSTR lpszPassword)
{
	//��������
	CMD_GR_UserSitDown UserSitDown;
	ZeroMemory(&UserSitDown,sizeof(UserSitDown));

	//��������
	UserSitDown.wTableID=wTableID;
	UserSitDown.wChairID=wChairID;
	if ((lpszPassword!=NULL)&&(lpszPassword[0]!=0))
	{
		lstrcpyn(UserSitDown.szPassword,lpszPassword,CountArray(UserSitDown.szPassword));
	}

	//��������ʱ��
	if((m_wServerType&GAME_GENRE_MATCH)!=0&&m_ProcessManager.IsProcessReady())
	{
		ShowInformation(TEXT("���ȹر���Ϸ�ͻ��ˣ�"));
		return true;
	}

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&UserSitDown,sizeof(UserSitDown));

	return true;
}

//��������
bool CPlazaViewEntry::SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave)
{
	//��������
	CMD_GR_UserStandUp UserStandUp;
	ZeroMemory(&UserStandUp,sizeof(UserStandUp));

	//��������
	UserStandUp.wTableID=wTableID;
	UserStandUp.wChairID=wChairID;
	UserStandUp.cbForceLeave=cbForceLeave;

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_STANDUP,&UserStandUp,sizeof(UserStandUp));

	return true;
}

//��������
bool CPlazaViewEntry::SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor)
{
	//������Ϣ
	CMD_GR_C_UserChat UserChat;
	lstrcpyn(UserChat.szChatString,pszChatString,CountArray(UserChat.szChatString));

	//��������
	UserChat.dwChatColor=crColor;
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//��������
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]));

	return true;
}

//���ͱ���
bool CPlazaViewEntry::SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex)
{
	//��������
	CMD_GR_C_UserExpression UserExpression;
	ZeroMemory(&UserExpression,sizeof(UserExpression));

	//������Ϣ
	UserExpression.wItemIndex=wItemIndex;
	UserExpression.dwTargetUserID=dwTargetUserID;

	//��������
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));

	return true;
}


//�û�����
VOID CPlazaViewEntry::OnUserItemAcitve(IClientUserItem * pIClientUserItem)
{
	//�ж��Լ�
	if (m_pIMySelfUserItem==NULL)
	{
		m_pIMySelfUserItem=pIClientUserItem;
	}

	//��������
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
	if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
	{
		WORD wTableID=pIClientUserItem->GetTableID();
		WORD wChairID=pIClientUserItem->GetChairID();
		m_pITableViewFrame->SetClientUserItem(wTableID,wChairID,pIClientUserItem);
	}

	return;
}

//�û�ɾ��
VOID CPlazaViewEntry::OnUserItemDelete(IClientUserItem * pIClientUserItem)
{
	//��������
	WORD wLastTableID=pIClientUserItem->GetTableID();
	WORD wLastChairID=pIClientUserItem->GetChairID();
	DWORD dwLeaveUserID=pIClientUserItem->GetUserID();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//�뿪����
	if ((wLastTableID!=INVALID_TABLE)&&(wLastChairID!=INVALID_CHAIR))
	{
		//��ȡ�û�
		IClientUserItem * pITableUserItem=m_pITableViewFrame->GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_pITableViewFrame->SetClientUserItem(wLastTableID,wLastChairID,NULL);

		//�뿪֪ͨ
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

//�û�����
VOID CPlazaViewEntry::OnUserFaceUpdate(IClientUserItem * pIClientUserItem)
{
	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//������Ϸ
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(m_pIMySelfUserItem->GetTableID()==pUserInfo->wTableID))
	{
		m_ProcessManager.SendCustomFace(pUserInfo->dwUserID,pUserInfo->dwCustomID,pCustomFaceInfo);
	}

	return;
}

//�û�����
VOID CPlazaViewEntry::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore)
{
	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	
	//����֪ͨ
	if (pIClientUserItem==m_pIMySelfUserItem)
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//���ñ���
		if(m_wServerType==GAME_GENRE_GOLD)
		{
			pGlobalUserData->lUserScore=pIClientUserItem->GetUserScore();
		}
		
		//���²Ƹ�
		pGlobalUserData->dwLoveLiness=pUserInfo->lLoveLiness;
		pGlobalUserData->lUserIngot=pIClientUserItem->GetUserIngot();
		pGlobalUserData->lUserInsure=pIClientUserItem->GetUserInsure();
		pGlobalUserData->dUserBeans=pUserInfo->dBeans;

		//�¼�֪ͨ
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	//��Ϸ֪ͨ
	if ((pMeUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->wTableID==pMeUserInfo->wTableID))
	{
		//��������
		tagUserScore UserScore;
		ZeroMemory(&UserScore,sizeof(UserScore));

		//���ñ���
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

		//��������
		m_ProcessManager.SendUserScore(pUserInfo->dwUserID,UserScore);
	}

	return;
}

//�û�����
VOID CPlazaViewEntry::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus)
{
	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	WORD wNowTableID=pIClientUserItem->GetTableID(),wLastTableID=LastStatus.wTableID;
	WORD wNowChairID=pIClientUserItem->GetChairID(),wLastChairID=LastStatus.wChairID;
	BYTE cbNowStatus=pIClientUserItem->GetUserStatus(),cbLastStatus=LastStatus.cbUserStatus;
	
	//�����뿪
	if ((wLastTableID!=INVALID_TABLE)&&((wLastTableID!=wNowTableID)||(wLastChairID!=wNowChairID)))
	{
		IClientUserItem * pITableUserItem= m_pITableViewFrame->GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_pITableViewFrame->SetClientUserItem(wLastTableID,wLastChairID,NULL);
	}

	//���Ӽ���
	if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//����жϣ���������
		if(pUserInfo->dwUserID != pMeUserInfo->dwUserID && cbNowStatus == US_SIT && pMeUserInfo->wTableID == wNowTableID)
		{
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//������
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

	//����״̬
	if ((m_pITableViewFrame->GetChairCount() < MAX_CHAIR)&&(wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(wNowChairID==wLastChairID))
	{
		m_pITableViewFrame->UpdateTableView(wNowTableID);
	}

	//�뿪֪ͨ
	if ((wLastTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//��Ϸ֪ͨ
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

	//���봦��
	if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//��Ϸ֪ͨ
		if ((m_pIMySelfUserItem!=pIClientUserItem)&&(pMeUserInfo->wTableID==wNowTableID))
		{
			ASSERT(wNowChairID!=INVALID_CHAIR);
			m_ProcessManager.SendUserItem(pIClientUserItem);
		}

		//�Լ��ж�
		if (m_pIMySelfUserItem==pIClientUserItem)
		{
			//���ñ���
			m_wReqTableID=INVALID_TABLE;
			m_wReqChairID=INVALID_CHAIR;

			//��������
			m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);
		}

		return;
	}

	//״̬�ı�
	if ((wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(pMeUserInfo->wTableID==wNowTableID))
	{
		//��Ϸ֪ͨ
		tagUserStatus UserStatus;
		UserStatus.wTableID=wNowTableID;
		UserStatus.wChairID=wNowChairID;
		UserStatus.cbUserStatus=cbNowStatus;
		m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);

		return;
	}

	return;
}

//�û�����
VOID CPlazaViewEntry::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserAttrib & UserAttrib)
{
	//��������
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
	WORD wUserTableID=pIClientUserItem->GetTableID();

	//֪ͨ��Ϸ
	if ((wMeTableID!=INVALID_TABLE)&&(wMeTableID==wUserTableID))
	{
		//��������
		tagUserAttrib UserAttrib;
		UserAttrib.cbCompanion=pIClientUserItem->GetUserCompanion();

		//����֪ͨ
		m_ProcessManager.SendUserAttrib(pIClientUserItem->GetUserID(),UserAttrib);
	}

	return;
}

//ִ������
bool CPlazaViewEntry::PerformSitDownAction(WORD wTableID, WORD wChairID, bool bEfficacyPass)
{
	
	//״̬����
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqTableID==wTableID)) return false;
	if ((m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID==wChairID)) return false;
	
	//�Լ�״̬
	if (m_pIMySelfUserItem!=NULL)
	{
		if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"));

			return false;
		}
	}

	//Ȩ���ж�
	if (CUserRight::CanPlay(m_dwUserRight)==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("��Ǹ������ʱû�м�����Ϸ��Ȩ�ޣ�"));

		return false;
	}

	//����Ч��
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		CString strDescribe;
		if ((wChairID!=INDEX_ENTER_CHAIR)&&(EfficacyTableRule(wTableID,wChairID,false,strDescribe)==false))
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,strDescribe,MB_ICONINFORMATION,30L);

			return false;
		}
	}

	//�����ж�
	TCHAR szPassword[LEN_PASSWORD] = {0};

	//�������
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID!=INDEX_ENTER_CHAIR))
	{
		IClientUserItem * pIClientUserItem=m_pITableViewFrame->GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pIClientUserItem==m_pIMySelfUserItem) m_pITableViewFrame->SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//���ý���
	if ((wChairID!=INDEX_ENTER_CHAIR)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		// m_pITableViewFrame->VisibleTable(wTableID);
		m_pITableViewFrame->SetClientUserItem(wTableID,wChairID,m_pIMySelfUserItem);
	}

	//���ñ���
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;

	//��������
	SendSitDownPacket(wTableID,wChairID,szPassword);

	return true;
}


//����Ч��
bool CPlazaViewEntry::EfficacyTableRule(WORD wTableID, WORD wChairID, bool bReqLookon, CString & strDescribe)
{
	//Ч�����
	ASSERT(wTableID<m_pITableViewFrame->GetTableCount());
	ASSERT(wChairID<m_pITableViewFrame->GetChairCount());

	//״̬����
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if (wTableID>=m_pITableViewFrame->GetTableCount()) return false;
	if (wChairID>=m_pITableViewFrame->GetChairCount()) return false;

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	ITableView * pITableView=m_pITableViewFrame->GetTableViewItem(wTableID);
	IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(wChairID);

	//��������
	bool bGameStart=pITableView->GetPlayFlag();
	bool bDynamicJoin=CServerRule::IsAllowDynamicJoin(m_dwServerRule);

	//��Ϸ״̬
	if ((bGameStart==true)&&(bDynamicJoin==false)&&(bReqLookon==false))
	{
		strDescribe=TEXT("����Ϸ���Ѿ���ʼ��Ϸ�ˣ���ʱ���ܼ��룡");
		return false;
	}

	//�����ж�
	if ((bReqLookon==false)&&(m_pIMySelfUserItem->GetMasterOrder()==0))
	{
		//�����ж�
		for (WORD i=0;i<m_pITableViewFrame->GetChairCount();i++)
		{
			//��ȡ�û�
			IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(i);
			if ((pITableUserItem==NULL)||(pITableUserItem==m_pIMySelfUserItem)) continue;

			//����Ч��
			if (m_pParameterGame->m_bLimitGameScore)
			{
				//��ͻ���
				if (pITableUserItem->GetUserScore()<m_pParameterGame->m_lMinGameScore)
				{
					strDescribe.Format(TEXT("[ %s ] �Ļ���̫���ˣ����������ò�����"),pITableUserItem->GetNickName());
					return false;
				}
			}
		}
	}
	return true;
}



//�û�����
bool CPlazaViewEntry::OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_USER_ENTER:			//�û�����
		{
			return OnSocketSubUserEnter(pData,wDataSize);
		}
	case SUB_GR_USER_SCORE:			//�û�����
		{
			return OnSocketSubUserScore(pData,wDataSize);
		}
	case SUB_GR_USER_STATUS:		//�û�״̬
		{
			return OnSocketSubUserStatus(pData,wDataSize);
		}
	case SUB_GR_USER_GAME_DATA:		//�û���Ϸ����
		{
			return OnSocketSubUserItemGameData(pData,wDataSize);
		}
	case SUB_GR_REQUEST_FAILURE:	//����ʧ��
		{
			return OnSocketSubRequestFailure(pData,wDataSize);
		}
	case SUB_GR_USER_CHAT:			//�û�����
		{
			return OnSocketSubUserChat(pData,wDataSize);
		}
	case SUB_GR_USER_EXPRESSION:	//�û�����
		{
			return OnSocketSubExpression(pData,wDataSize);
		}
	case SUB_GR_WISPER_CHAT:		//�û�˽��
		{
			return OnSocketSubWisperUserChat(pData,wDataSize);
		}
	case SUB_GR_WISPER_EXPRESSION:	//˽�ı���
		{
			return OnSocketSubWisperExpression(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_SUCCESS:	//���߳ɹ�
		{
			return OnSocketSubPropertySuccess(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_FAILURE:  //����ʧ��
		{
			return OnSocketSubPropertyFailure(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_EFFECT:   //����ЧӦ
		{
			return OnSocketSubPropertyEffect(pData,wDataSize);
		}
	case SUB_GR_USER_INVITE:	   //�������
		{
			return OnSocketSubUserInvite(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_MESSAGE:   //������Ϣ
		{
			return OnSocketPropertyMessage(pData,wDataSize);
		}
	case SUB_GR_PROPERTY_TRUMPET:  //������Ϣ
		{
			return OnSocketPropertyTrumpet(pData,wDataSize);
		}
	case SUB_GR_USER_WAIT_DISTRIBUTE: //�ȴ�����
		{
			return OnSocketSubWaitDistribute(pData,wDataSize);
		}
	}

	return true;
}

//��¼����
bool CPlazaViewEntry::OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_SUCCESS:	//��¼�ɹ�
		{
			return OnSocketSubLogonSuccess(pData,wDataSize);
		}
	case SUB_GR_LOGON_FAILURE:	//��¼ʧ��
		{
			return OnSocketSubLogonFailure(pData,wDataSize);
		}
	case SUB_GR_LOGON_FINISH:	//��¼���
		{
			return OnSocketSubLogonFinish(pData,wDataSize);
		}
	case SUB_GR_UPDATE_NOTIFY:	//������ʾ
		{
			return OnSocketSubUpdateNotify(pData,wDataSize);
		}
	}

	return true;
}

//���ô���
bool CPlazaViewEntry::OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_CONFIG_COLUMN:		//�б�����
		{
			//��������
			CMD_GR_ConfigColumn * pConfigColumn=(CMD_GR_ConfigColumn *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ConfigColumn)-sizeof(pConfigColumn->ColumnItem);

			//Ч�����
			ASSERT((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))==wDataSize);
			if ((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))!=wDataSize) return false;

			//������Ϣ
			m_cbColumnCount=__min(pConfigColumn->cbColumnCount,CountArray(m_ColumnItem));
			CopyMemory(m_ColumnItem,pConfigColumn->ColumnItem,sizeof(tagColumnItem)*m_cbColumnCount);

			return true;
		}
	case SUB_GR_CONFIG_SERVER:		//��������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_ConfigServer));
			if (wDataSize<sizeof(CMD_GR_ConfigServer)) return false;

			//��Ϣ����
			CMD_GR_ConfigServer * pConfigServer=(CMD_GR_ConfigServer *)pData;

			//��������
			m_wServerType=pConfigServer->wServerType;
			m_dwServerRule=pConfigServer->dwServerRule;

          	//�������
			WORD wTableCount=pConfigServer->wTableCount;
			WORD wChairCount=pConfigServer->wChairCount;
			m_wChairCount=pConfigServer->wChairCount;
			IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);

			//��ԴĿ¼
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//��������
			bool bSuccess=m_pITableViewFrame->ConfigTableFrame(wTableCount,wChairCount,m_dwServerRule,m_wServerType,m_GameServer.wServerID,szResDirectory);
			WORD pCount = m_pITableViewFrame->GetChairCount();

			//������
			if (bSuccess==false)
			{
				//������ʾ
				m_DlgStatus.HideStatusWindow();

				//�ر�����
				m_TCPSocketModule->CloseSocket();

				//��ʾ��Ϣ
				CInformation Information(AfxGetMainWnd());
				INT nResult=Information.ShowMessageBox(TEXT("��Ϸ��Դ�ļ�����ʧ�ܻ��߸�ʽ�����Ƿ����ڽ����޸���"),MB_ICONERROR|MB_YESNO);

				//������Ϸ
				if (nResult==IDYES)
				{
					CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//�رշ���
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			return true;
		}
	case SUB_GR_CONFIG_PROPERTY:	//��������
		{
			//��������
			CMD_GR_ConfigProperty * pConfigProperty=(CMD_GR_ConfigProperty *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ConfigProperty)-sizeof(pConfigProperty->PropertyInfo);

			//Ч�����
			ASSERT((wHeadSize+pConfigProperty->cbPropertyCount*sizeof(pConfigProperty->PropertyInfo[0]))==wDataSize);
			if ((wHeadSize+pConfigProperty->cbPropertyCount*sizeof(pConfigProperty->PropertyInfo[0]))!=wDataSize) return false;

			//������Ϣ
			m_cbPropertyCount=__min(pConfigProperty->cbPropertyCount,CountArray(m_PropertyInfo));
			CopyMemory(m_PropertyInfo,pConfigProperty->PropertyInfo,sizeof(tagPropertyInfo)*m_cbPropertyCount);

			//���õ���
			// m_pPlazaViewServerCtrl->SetPropertyDescribe(m_PropertyInfo,m_cbPropertyCount);

			return true;
		}
	case SUB_GR_CONFIG_FINISH:		//�������
		{
			//��ԴĿ¼
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//��Ϸ�ȼ�
			
			ASSERT(m_GameLevelParserModule.GetInterface()!=NULL);
			bool bSuccess=m_GameLevelParserModule->LoadGameLevelItem(m_GameKind.szKindName,szResDirectory,m_wServerType);

			//������
			if (bSuccess==false)
			{
				//�ر�����
				m_TCPSocketModule->CloseSocket();

				//��ʾ��Ϣ
				INT nResult=ShowInformation(TEXT("��Ϸ�ȼ����ö�ȡʧ�ܻ��߸�ʽ�����Ƿ����ڽ����޸���"),MB_ICONERROR|MB_YESNO);

				//������Ϸ
				if (nResult==IDYES)
				{
					CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//�رշ���
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			return true;
		}
	case SUB_GR_CONFIG_USER_RIGHT:	//�������Ȩ��
		{
			//Ч������
			ASSERT(wDataSize>=sizeof(CMD_GR_ConfigUserRight));
			if (wDataSize<sizeof(CMD_GR_ConfigUserRight)) return false;

			//��Ϣ����
			CMD_GR_ConfigUserRight * pConfigUserRight=(CMD_GR_ConfigUserRight *)pData;

			//���Ȩ��
			m_dwUserRight=pConfigUserRight->dwUserRight;

			//����˽�Ĵ���

			/* for( INT_PTR i = 0; i < m_DlgWhisperItemArray.GetCount(); i++ )
			{
				CDlgWhisper *pdlg = m_DlgWhisperItemArray[i];
				if( pdlg ) pdlg->SetUserRight(m_dwUserRight);
			} */

			//֪ͨ�ͻ���
			if(IsGameClientReady())
			{
				//���캯��
				IPC_GF_UserRight ur = {0};
				ur.dwUserRight = m_dwUserRight;

				m_ProcessManager.SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_USER_RIGHT,&ur,sizeof(ur) );
			}

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//״̬����
bool CPlazaViewEntry::OnSocketMainStatus(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_TABLE_INFO:		//������Ϣ
		{
			//��������
			CMD_GR_TableInfo * pTableInfo=(CMD_GR_TableInfo *)pData;
			WORD wHeadSize=sizeof(CMD_GR_TableInfo)-sizeof(pTableInfo->TableStatusArray);

			//Ч������
			ASSERT((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))==wDataSize);
			if ((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))!=wDataSize) return false;

			//��Ϣ����
			for (WORD i=0;i<pTableInfo->wTableCount;i++)
			{
				BYTE cbTableLock=pTableInfo->TableStatusArray[i].cbTableLock;
				BYTE cbPlayStatus=pTableInfo->TableStatusArray[i].cbPlayStatus;
				m_pITableViewFrame->SetTableStatus(i,(cbPlayStatus==TRUE),(cbTableLock==TRUE),pTableInfo->TableStatusArray[i].lCellScore);
			}

			return true;
		}
	case SUB_GR_TABLE_STATUS:	//����״̬
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GR_TableStatus));
			if (wDataSize!=sizeof(CMD_GR_TableStatus)) return false;

			//��������
			CMD_GR_TableStatus * pTableStatus=(CMD_GR_TableStatus *)pData;

			//��������
			BYTE cbTableLock=pTableStatus->TableStatus.cbTableLock;
			BYTE cbPlayStatus=pTableStatus->TableStatus.cbPlayStatus;
			m_pITableViewFrame->SetTableStatus(pTableStatus->wTableID,(cbPlayStatus==TRUE),(cbTableLock==TRUE),pTableStatus->TableStatus.lCellScore);

			//��������
			if(cbPlayStatus==TRUE && m_pIMySelfUserItem->GetTableID()==pTableStatus->wTableID && CServerRule::IsAllowAvertCheatMode(m_dwServerRule))
			{
				m_pITableViewFrame->SetTableStatus(false);
			}

			return true;
		}
	}

	return true;
}

//���д���
bool CPlazaViewEntry::OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//��Ϸ����
	BYTE *pActivityGame = (BYTE*)pData;
	if(*pActivityGame==TRUE)
	{
		return OnSocketMainGameFrame(MDM_GR_INSURE,wSubCmdID,pData,wDataSize);
	}

	//��Ϣ����
	/* ASSERT(m_pDlgInsureServer!=NULL);
	if (m_pDlgInsureServer==NULL) return true;
	if (m_pDlgInsureServer->OnServerInsureMessage(MDM_GR_INSURE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	} */

	return true;
}

//������
bool CPlazaViewEntry::OnSocketMainTask(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;

	//��Ϣ����
	if(wSubCmdID==SUB_GR_TASK_FINISH)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GR_S_TaskFinish));
		if(wDataSize>sizeof(CMD_GR_S_TaskFinish)) return true;

		//��ȡ����
		CMD_GR_S_TaskFinish * pTaskFinish = (CMD_GR_S_TaskFinish *)pData;

		//��ʾ��ʾ��
		/* CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
		pPlatformFrame->ShowTaskNotifyer(pTaskFinish->szTaskName); */

		return true;
	}
    
	//��Ϣ����
	
	/* ASSERT(m_pDlgTaskServerCenter!=NULL);
	if (m_pDlgTaskServerCenter==NULL) return true;
	if (m_pDlgTaskServerCenter->OnTaskMessage(MDM_GR_TASK,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	} */

	return true;
}

//�һ�����
bool CPlazaViewEntry::OnSocketMainExchange(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	ASSERT(pData!=NULL);
	if (pData==NULL) return true;
   
	//��Ϣ����
	
	/* ASSERT(m_pDlgExchangeServer!=NULL);
	if (m_pDlgExchangeServer==NULL) return true;
	if (m_pDlgExchangeServer->OnExchangeMessage(MDM_GR_EXCHANGE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	} */

	return true;
}

//������
bool CPlazaViewEntry::OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_OPTION_CURRENT:	//��ǰ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GR_OptionCurrent));
			if (wDataSize!=sizeof(CMD_GR_OptionCurrent)) return false;

			//��Ϣ����
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

//ϵͳ����
bool CPlazaViewEntry::OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CM_SYSTEM_MESSAGE:	//ϵͳ��Ϣ
		{
			return OnSocketSubSystemMessage(pData,wDataSize);
		}
	case SUB_CM_ACTION_MESSAGE:	//������Ϣ
		{
			return OnSocketSubActionMessage(pData,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CPlazaViewEntry::OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize<=SOCKET_TCP_PACKET);
	if (wDataSize>SOCKET_TCP_PACKET) return false;
	if(!IsGameClientReady()) return true;

	//��������
	IPC_GF_SocketRecv SocketRecv;
	SocketRecv.CommandInfo.wSubCmdID=wSubCmdID;
	SocketRecv.CommandInfo.wMainCmdID=wMainCmdID;

	//��������
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketRecv.cbBuffer,pData,wDataSize);
	}

	//��������
	WORD wSendSize=sizeof(SocketRecv)-sizeof(SocketRecv.cbBuffer)+wDataSize;
	m_ProcessManager.SendProcessData(IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_RECV,&SocketRecv,wSendSize);

	return true;
}

//��������
bool CPlazaViewEntry::OnSocketMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_MATCH_FEE:		//���ò�ѯ
		{
			//����Ч��
			ASSERT(wDataSize<=sizeof(CMD_GR_Match_Fee));
			if(wDataSize>sizeof(CMD_GR_Match_Fee)) return false;

			//��ȡ����
			CMD_GR_Match_Fee * pMatchFee = (CMD_GR_Match_Fee *)pData;
			if(pMatchFee->lMatchFee>0)
			{
				INT nResult=ShowInformation(pMatchFee->szNotifyContent,MB_ICONINFORMATION|MB_YESNO,30L);
				if (nResult==IDYES) m_TCPSocketModule->SendData(MDM_GR_MATCH,SUB_GR_MATCH_FEE,&pMatchFee->lMatchFee,sizeof(pMatchFee->lMatchFee));
			}

			return true;
		}
	case SUB_GR_MATCH_GOLDUPDATE:	//��Ҹ���
		{
			//����У��
			ASSERT(sizeof(CMD_GR_MatchGoldUpdate)==wDataSize);
			if(sizeof(CMD_GR_MatchGoldUpdate)!=wDataSize) return false;

			//��ȡ����
			CMD_GR_MatchGoldUpdate * pMatchGoldUpdate=(CMD_GR_MatchGoldUpdate*)pData;

			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//���ñ���
			pGlobalUserData->lUserScore=pMatchGoldUpdate->lCurrGold;
			pGlobalUserData->lUserIngot=pMatchGoldUpdate->lCurrIngot;
			pGlobalUserData->dwExperience=pMatchGoldUpdate->dwCurrExprience;			

			//�¼�֪ͨ
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

			return true;
		}
	case SUB_GR_MATCH_ELIMINATE:	//������̭
		{
			//��ȡ����
			CServerListData * pServerListData = CServerListData::GetInstance();
			CGameServerItem * pGameServerItem=pServerListData->SearchGameServer(m_GameServer.wServerID);

			//�޸ı���״̬
			if(pGameServerItem!=NULL) pGameServerItem->m_bSignuped=false;

			return true;
		}
	}

	return m_pITableViewFrame->OnEventViewData(wSubCmdID,pData,wDataSize);
}


//�û�����
bool CPlazaViewEntry::OnSocketSubUserEnter(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(tagUserInfoHead));
	if (wDataSize<sizeof(tagUserInfoHead)) return false;

	//��������
	tagUserInfo UserInfo;
	tagCustomFaceInfo CustomFaceInfo;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

	//��Ϣ����
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pData;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
	bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserInfoHead->dwUserID;
	bool bMasterUserOrder=((pUserInfoHead->cbMasterOrder>0)||((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetMasterOrder()>0)));

	//��ȡ��Ϣ
	if ((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		//�û�����
		UserInfo.wFaceID=pUserInfoHead->wFaceID;
		UserInfo.dwGameID=pUserInfoHead->dwGameID;
		UserInfo.dwUserID=pUserInfoHead->dwUserID;
		UserInfo.dwGroupID=pUserInfoHead->dwGroupID;
		UserInfo.dwCustomID=pUserInfoHead->dwCustomID;

		//�û�״̬
		UserInfo.wTableID=pUserInfoHead->wTableID;
		UserInfo.wChairID=pUserInfoHead->wChairID;
		UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

		//�û�����
		UserInfo.cbGender=pUserInfoHead->cbGender;
		UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;

		//�û�����
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

		//��������
		VOID * pDataBuffer=NULL;
		tagDataDescribe DataDescribe;
		CRecvPacketHelper RecvPacket(pUserInfoHead+1,wDataSize-sizeof(tagUserInfoHead));

		//��չ��Ϣ
		while (true)
		{
			pDataBuffer=RecvPacket.GetData(DataDescribe);
			if (DataDescribe.wDataDescribe==DTP_NULL) break;
			switch (DataDescribe.wDataDescribe)
			{
			case DTP_GR_NICK_NAME:		//�û��ǳ�
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
			case DTP_GR_GROUP_NAME:		//�û�����
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
			case DTP_GR_UNDER_WRITE:	//����ǩ��
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
			case DTP_GR_USER_ADDRESS:	//�û���ַ
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

		//�Զ�ͷ��
		if (pUserInfoHead->dwCustomID!=0L)
		{
			//����ͷ��
			CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
			bool bSuccess=pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID,CustomFaceInfo);

			//����ͷ��
			if (bSuccess==false)
			{
				pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID);
			}
		}
	}
	else
	{
		//�û���Ϣ
		UserInfo.dwUserID=pUserInfoHead->dwUserID;
		lstrcpyn(UserInfo.szNickName,TEXT("��Ϸ���"),CountArray(UserInfo.szNickName));

		//�û�״̬
		UserInfo.wTableID=pUserInfoHead->wTableID;
		UserInfo.wChairID=pUserInfoHead->wChairID;
		UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

		//�û�����
		UserInfo.cbGender=pUserInfoHead->cbGender;
		UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;
	}

	//�����û�
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(UserInfo.dwUserID);
	if (pIClientUserItem==NULL) pIClientUserItem=m_PlazaUserManagerModule->ActiveUserItem(UserInfo,CustomFaceInfo,pUserInfoHead->bIsAndroid);

	//��ȡ����
	CServerListData * pServerListData=CServerListData::GetInstance();

	//��������
	pServerListData->SetServerOnLineCount(m_GameServer.wServerID,m_PlazaUserManagerModule->GetActiveUserCount());

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//������ʾ
	if (((bHideUserInfo==false)&&(bMySelfUserItem==false))||(bMasterUserOrder==true))
	{
		if(pParameterGlobal->m_bNotifyFriendCome && pIClientUserItem->GetUserCompanion()==CP_FRIEND)
		{
			// ��ȡָ��
			/* IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
			ASSERT(pIStringMessage!=NULL);

			//��ʾ��Ϣ
			CString strDescribe;
			strDescribe.Format(TEXT("���ĺ��� [%s] �����ˣ�"),pIClientUserItem->GetNickName());
			pIStringMessage->InsertSystemString(strDescribe); */
		}
	}

	return true;
}

//�û�����
bool CPlazaViewEntry::OnSocketSubUserScore(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserScore));
	if (wDataSize<sizeof(CMD_GR_UserScore)) return false;

	//Ѱ���û�
	CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserScore->dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserScore->dwUserID;
	bool bAvertCheatMode=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
	bool bMasterUserOrder=((pIClientUserItem->GetMasterOrder()>0)||(m_pIMySelfUserItem->GetMasterOrder()>0));

	//�����û�
	if ((bAvertCheatMode==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		m_PlazaUserManagerModule->UpdateUserItemScore(pIClientUserItem,&pUserScore->UserScore);
	}

	return true;
}

//�û�״̬
bool CPlazaViewEntry::OnSocketSubUserStatus(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserStatus));
	if (wDataSize<sizeof(CMD_GR_UserStatus)) return false;

	//Ѱ���û�
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserStatus->dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//����״̬
	if (pUserStatus->UserStatus.cbUserStatus==US_NULL) 
	{
		//ɾ���û�
		m_PlazaUserManagerModule->DeleteUserItem(pIClientUserItem);

		//��ȡ����
		CServerListData * pServerListData=CServerListData::GetInstance();

		//��������
		pServerListData->SetServerOnLineCount(m_GameServer.wServerID,m_PlazaUserManagerModule->GetActiveUserCount());
	}
	else
	{
		//�����û�
		m_PlazaUserManagerModule->UpdateUserItemStatus(pIClientUserItem,&pUserStatus->UserStatus);
	}

	return true;
}

//�û���Ϸ����
bool CPlazaViewEntry::OnSocketSubUserItemGameData(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_UserGameData * pUserGameData=(CMD_GR_UserGameData *)pData;
	WORD wHeadSize=sizeof(CMD_GR_UserGameData)-sizeof(pUserGameData->szUserGameData);

	//Ч�����
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+CountStringBuffer(pUserGameData->szUserGameData))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+CountStringBuffer(pUserGameData->szUserGameData)))) return false;

	//Ѱ���û�
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserGameData->dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	return true;
}

//��¼���
bool CPlazaViewEntry::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize==sizeof(CMD_GR_LogonFinish));
	if(wDataSize!=sizeof(CMD_GR_LogonFinish)) return false;

	//��ȡ����
	CMD_GR_LogonFinish * pLogonFinish = (CMD_GR_LogonFinish *)pData;

	//�ر���ʾ
	m_DlgStatus.HideStatusWindow();

	//����״̬
	SetServiceStatus(ServiceStatus_ServiceIng);

	//�����ж�
	if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()!=INVALID_TABLE))
	{
		//��������	
		bool bSuccess=m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);

		//������
		if (bSuccess==false)
		{
			//�ж�����
			m_TCPSocketModule->CloseSocket();
			m_ServiceStatus=ServiceStatus_NetworkDown;
		}	
	}

	//׷�����
	CGlobalUnits * pGlobalUnits = CGlobalUnits::GetInstance();
	DWORD dwTrackUserID = pGlobalUnits->GetTrackUserID();
	if(pGlobalUnits!=NULL && dwTrackUserID!=0)
	{
		//�����û�
		ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
		IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwTrackUserID);
		if(pIClientUserItem && pIClientUserItem->GetTableID()!=INVALID_TABLE && pIClientUserItem->GetChairID()!=INVALID_CHAIR)
		{
			//��������
			tagSearchTable SearchTable;
			ZeroMemory(&SearchTable,sizeof(SearchTable));

			//��������
			SearchTable.bAllNull=true;
			SearchTable.bNotFull=true;
			SearchTable.bOneNull=true;
			SearchTable.bFilterPass=true;
			SearchTable.bTwoNull=(m_pITableViewFrame->GetChairCount()!=2);

			//�������
			SearchTable.wResultChairID=INVALID_CHAIR;
			SearchTable.wResultTableID=pIClientUserItem->GetTableID();			
			SearchTable.wStartTableID=SearchTable.wResultTableID;

			//��������
			SearchGameTable(SearchTable);
			if(SearchTable.wResultChairID!=INVALID_CHAIR && SearchTable.wResultTableID==SearchTable.wStartTableID  && CServerRule::IsAllowAvertCheatMode(m_dwServerRule)==false)
			{
				//��������
				WORD wChairID=SearchTable.wResultChairID;
				// m_pITableViewFrame->VisibleTable(SearchTable.wResultTableID);
				m_pITableViewFrame->FlashGameChair(SearchTable.wResultTableID,wChairID);

				//ִ������
				PerformSitDownAction(SearchTable.wResultTableID,wChairID,true);
			}
			else
			{
				//ִ���Թ�
				// PerformLookonAction(SearchTable.wStartTableID,pIClientUserItem->GetChairID());
			}			

			return true;
		}
	}

	return true;
}

//��¼�ɹ�
bool CPlazaViewEntry::OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_LogonSuccess));
	if (wDataSize!=sizeof(CMD_GR_LogonSuccess)) return false;

	//��Ϣ����
	CMD_GR_LogonSuccess * pLogonSuccess=(CMD_GR_LogonSuccess *)pData;

	//�������
	m_dwUserRight=pLogonSuccess->dwUserRight;
	m_dwMasterRight=pLogonSuccess->dwMasterRight;

	//����״̬
	SetServiceStatus(ServiceStatus_RecvInfo);

	//�ر���ʾ
	// m_DlgUpdateStatus.StartEnterServer(/*TEXT("���ڶ�ȡ������Ϣ...")*/);

	//��ȡ����
	/* CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/GameRoom.aspx?KindID=%ld&ServerID=%ld"),pGlobalWebLink->GetPlatformLink(),m_GameKind.wKindID,m_GameServer.wServerID);

	////���ӹ��
	m_PlatformPublicize.Navigate(szBillUrl);
	m_PlatformPublicize.SetBoradColor(RGB(0,0,0));
	ASSERT(m_ImageGameName.IsNull()==true);
	if (m_ImageGameName.IsNull()==false) m_ImageGameName.DestroyImage();

	//����·��
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//��ȡ�ļ�
	TCHAR szGameName[64]=TEXT("");
	CString strProcessName(m_GameKind.szProcessName);
	INT nPos=strProcessName.Find(TEXT("."));
	_sntprintf(szGameName,CountArray(szGameName),TEXT("%s"),strProcessName.Left(nPos));

	//����·��
	TCHAR szFilePath[MAX_PATH]=TEXT("");
	_sntprintf(szFilePath,CountArray(szFilePath),TEXT("%s\\GameKindLogo\\Room_%s.png"),szDirectory,szGameName);

	//����ͼƬ
	m_ImageGameName.LoadImage(szFilePath); */

	//��¼�ɹ�����
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	pPlatformFrame->PostMessage( WM_LOGON_SUCCESS, 0, 0);

	return true;
}

//��¼ʧ��
bool CPlazaViewEntry::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	//Ч�����
	CMD_GR_LogonFailure * pLogonFailure=(CMD_GR_LogonFailure *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString))) return false;

	//�ر���ʾ
	m_DlgStatus.HideStatusWindow();

	//�رմ���
	m_TCPSocketModule->CloseSocket();

	//����״̬
	SetServiceStatus(ServiceStatus_NetworkDown);

	//������Ϣ
	if(pLogonFailure->lErrorCode == LOGON_FAIL_SERVER_INVALIDATION)
	{
		//��������
		//CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
		//if (pPlatformFrame!=NULL) pPlatformFrame->ListGameServerInvalidation(m_GameKind.wKindID, m_GameServer.wServerID);
	}

	//��ʾ��Ϣ
	LPCTSTR pszDescribeString=pLogonFailure->szDescribeString;
	if (lstrlen(pszDescribeString)>0) ShowInformation(pszDescribeString,MB_ICONERROR,60);

	//�رշ��� 
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
	
	return true;
}

//������ʾ
bool CPlazaViewEntry::OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UpdateNotify));
	if (wDataSize!=sizeof(CMD_GR_UpdateNotify)) return false;

	//��������
	CMD_GR_UpdateNotify * pUpdateNotify=(CMD_GR_UpdateNotify *)pData;

	//��������
	if (pUpdateNotify->cbMustUpdatePlaza==TRUE)
	{
		//�رմ���
		m_DlgStatus.HideStatusWindow();
		m_TCPSocketModule->CloseSocket();

		//�رշ���
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//���ش���
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		if (pGlobalUnits!=NULL) pGlobalUnits->DownLoadClient(TEXT("��Ϸ�㳡"),0,0);

		return true;
	}

	//��Ϸ����
	if ((pUpdateNotify->cbMustUpdateClient==TRUE)||(pUpdateNotify->cbAdviceUpdateClient==TRUE))
	{
		//�رմ���
		if (pUpdateNotify->cbMustUpdateClient==TRUE)
		{
			m_DlgStatus.HideStatusWindow();
			m_TCPSocketModule->CloseSocket();
		}

		//������ʾ
		if (pUpdateNotify->cbMustUpdateClient==FALSE)
		{
			//������ʾ
			TCHAR szDescribe[512]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��%s���Ѿ������ˣ�����ǿ�ҽ��������и��£����ڽ��и�����"),m_GameKind.szKindName);

			//��ʾ��Ϣ
			CInformation Information(this);
			if (Information.ShowMessageBox(szDescribe,MB_ICONINFORMATION|MB_YESNO,0)!=IDYES) return true;
		}

		//�رշ���
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//������ʾ
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wKindID,m_GameServer.wServerID);
	}

	return true;
}

//����ʧ��
bool CPlazaViewEntry::OnSocketSubRequestFailure(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_RequestFailure * pRequestFailure=(CMD_GR_RequestFailure *)pData;

	//Ч�����
	ASSERT(wDataSize>(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString)));
	if (wDataSize<=(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString))) return false;

	//��������
	IClientUserItem * pITableUserItem=NULL;
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
	WORD wMeChairID=m_pIMySelfUserItem->GetChairID();

	//���滹ԭ
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_TABLE)&&(m_wReqChairID!=INDEX_ENTER_CHAIR))
	{
		IClientUserItem * pITableUserItem=m_pITableViewFrame->GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pITableUserItem==m_pIMySelfUserItem) m_pITableViewFrame->SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//���ñ���
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;

	//��ʾ��Ϣ
	if (pRequestFailure->szDescribeString[0]!=0)
	{
		CInformation Information;
		Information.ShowMessageBox(pRequestFailure->szDescribeString,MB_ICONINFORMATION,30L);
	}

	return true;
}

//�û�����
bool CPlazaViewEntry::OnSocketSubUserChat(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_S_UserChat * pUserChat=(CMD_GR_S_UserChat *)pData;

	//Ч�����
	ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	//Ч�����
	if (wDataSize<(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserChat->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//��Ϣ����
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//���ű���
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//��ȡָ��
	/* IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//��ʾ��Ϣ
	if (pUserChat->dwTargetUserID!=0L)
	{
		//��ȡ�û�
		IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserChat->dwTargetUserID);
		if (pIRecvUserItem==NULL) return true;

		//��ʾ��Ϣ
		LPCTSTR pszSendUser=pISendUserItem->GetNickName();
		LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
		pIStringMessage->InsertUserChat(pszSendUser,pszRecvUser,pUserChat->szChatString,pUserChat->dwChatColor);
	}
	else 
	{
		//��ʾ��Ϣ
		LPCTSTR pszSendUser=pISendUserItem->GetNickName();
		pIStringMessage->InsertUserChat(pszSendUser,pUserChat->szChatString,pUserChat->dwChatColor);
	} */

	return true;
}

//�û�����
bool CPlazaViewEntry::OnSocketSubExpression(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_S_UserExpression));
	if (wDataSize!=sizeof(CMD_GR_S_UserExpression)) return false;

	//��������
	CMD_GR_S_UserExpression * pUserExpression=(CMD_GR_S_UserExpression *)pData;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//��ȡָ��
	/* IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//��Ϣ����
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//���ű���
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	} */

	//������Ϣ
	if (pUserExpression->dwTargetUserID==0L)
	{
		//��������
		CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
		CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

		//�������
		if (pExpressionItem!=NULL)
		{
			//��ȡ·��
			TCHAR szImagePath[MAX_PATH]=TEXT("");
			pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

			//������Ϣ
			/* LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			pIStringMessage->InsertExpression(pszSendUser,szImagePath); */
		}
	}

	//˽����Ϣ
	if (pUserExpression->dwTargetUserID!=0L)
	{
		//��������
		CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
		CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

		//�������
		if (pExpressionItem!=NULL)
		{
			//��ȡ·��
			TCHAR szImagePath[MAX_PATH]=TEXT("");
			pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

			//��ȡ�û�
			IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserExpression->dwTargetUserID);
			if (pIRecvUserItem==NULL) return true;

			//��ʾ��Ϣ
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
			// pIStringMessage->InsertExpression(pszSendUser,pszRecvUser,szImagePath);
		}
	}

	return true;
}

//�û�˽��
bool CPlazaViewEntry::OnSocketSubWisperUserChat(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_S_WisperChat * pWisperChat=(CMD_GR_S_WisperChat *)pData;

	//Ч�����
	ASSERT(wDataSize>=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0])));

	//Ч�����
	if (wDataSize<(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0]))) return false;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperChat->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//�����û�
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperChat->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//��Ϣ����
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//���ű���
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}

		//�����û�
		if(cbUserCompanion == CP_SHIELD) return true;
	}

	return true;
}

//˽�ı���
bool CPlazaViewEntry::OnSocketSubWisperExpression(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_S_WisperExpression));
	if (wDataSize!=sizeof(CMD_GR_S_WisperExpression)) return false;

	//��������
	CMD_GR_S_WisperExpression * pWisperExpression=(CMD_GR_S_WisperExpression *)pData;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//�����û�
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperExpression->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//��Ϣ����
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//���ű���
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}

		//�����û�
		if(cbUserCompanion == CP_SHIELD) return true;
	}

	//��������
	/* DWORD dwUserID[2]={0L,0L};
	dwUserID[0]=m_pIMySelfUserItem->GetUserID();
	dwUserID[1]=(pISendUserItem==m_pIMySelfUserItem)?pIRecvUserItem->GetUserID():pISendUserItem->GetUserID();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,pWisperExpression->dwTargetUserID,dwUserID,CountArray(dwUserID),(pISendUserItem!=m_pIMySelfUserItem));

	//��ʾ����
	if (pDlgWhisper!=NULL)
	{
		pDlgWhisper->OnEventExpression(pISendUserItem,pWisperExpression->wItemIndex);
	} */

	return true;
}

//���߳ɹ�
bool CPlazaViewEntry::OnSocketSubPropertySuccess(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_S_PropertySuccess));
	if (wDataSize!=sizeof(CMD_GR_S_PropertySuccess)) return false;

	//��������
	CMD_GR_S_PropertySuccess * pPropertySuccess=(CMD_GR_S_PropertySuccess *)pData;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertySuccess->dwSourceUserID);
	if (pISendUserItem==NULL) return true;

	//��ȡ�û�
	IClientUserItem * pTargetUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertySuccess->dwTargetUserID);
	if (pTargetUserItem==NULL) return true;

	//ʹ�÷�Χ
	if(pPropertySuccess->cbRequestArea==PT_ISSUE_AREA_GAME && IsGameClientReady())
	{
		//���ñ���
		IPC_GR_PropertySuccess  PropertySuccess;  
		PropertySuccess.wPropertyIndex=pPropertySuccess->wPropertyIndex;
		PropertySuccess.wPropertyCount=pPropertySuccess->wItemCount;
		PropertySuccess.dwSourceUserID=pPropertySuccess->dwSourceUserID;
		PropertySuccess.dwTargetUserID=pPropertySuccess->dwTargetUserID;
		CopyMemory(PropertySuccess.szSourceNickName,pISendUserItem->GetNickName(),sizeof(PropertySuccess.szSourceNickName));

		//��������
		bool bSendSuccess = m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_PROPERTY_SUCCESS,&PropertySuccess,sizeof(PropertySuccess));

		if(bSendSuccess==true) return true;
	}
	
	//��ȡ����
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//Ѱ�ҵ���
	ASSERT(pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex)!=NULL);
	CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex);

	//���ߴ���
	if (pGamePropertyItem!=NULL && pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PRESENT)
	{
		//ͼƬ��Ϣ
		tagPropertyImage PropertyImage;
		pGamePropertyItem->GetImageInfo(PropertyImage);

		//���Ŷ���
		if ((PropertyImage.pszFlash!=NULL)&&(PropertyImage.pszFlash[0]!=0))
		{
			//����·��
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//����·��
			TCHAR szFlashPath[MAX_PATH]=TEXT("");
			_sntprintf(szFlashPath,CountArray(szFlashPath),TEXT("%s\\Property\\%s"),szDirectory,PropertyImage.pszFlash);

			/* try
			{
				//���Ŷ���
				if (m_FlashControl.LoadMovieFile(szFlashPath)==false)
				{
					throw TEXT("�����ļ�����ʧ��");
				}

				//�����ؼ�
				CRect rcWindow;
				m_pITableViewFrame->GetViewWnd()->GetWindowRect(&rcWindow);
				m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);

				//���Ŷ���
				m_FlashControl.PlayMovie();
				m_FlashControl.ShowControl(SW_SHOW);
			}
			catch (...)
			{
				ASSERT(FALSE);
			} */
		}
	}

	//������ʾ
	if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY)
	{
		//��������
		TCHAR  szMessage[256]=TEXT("");

		//��ȡ��ʾ
		if(pPropertySuccess->dwSourceUserID==m_pIMySelfUserItem->GetUserID() && pPropertySuccess->dwSourceUserID!=pPropertySuccess->dwTargetUserID)
            _sntprintf(szMessage,CountArray(szMessage),TEXT("��ϲ��,���߹���ɹ�!"));
		else
			pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),m_pIMySelfUserItem->GetNickName(),pPropertySuccess->wItemCount,szMessage);

		//������Ϣ
		CInformation Information;
		Information.ShowMessageBox(szMessage,MB_ICONINFORMATION);

		//��������
		//if(m_pTrumpetItem!=NULL)  m_pTrumpetItem->UpdateControlSurface();
	}

	return true;
}

//����ʧ��
bool CPlazaViewEntry::OnSocketSubPropertyFailure(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GR_PropertyFailure * pPropertyFailure=(CMD_GR_PropertyFailure *)pData;
	if(pPropertyFailure==NULL) return false;

	//ʹ�÷�Χ
	if(pPropertyFailure->wRequestArea==PT_ISSUE_AREA_GAME && IsGameClientReady())
	{
		//���ñ���
		IPC_GR_PropertyFailure  PropertyFailure;  
		PropertyFailure.lErrorCode=pPropertyFailure->lErrorCode;
		CopyMemory(PropertyFailure.szDescribeString,pPropertyFailure->szDescribeString,sizeof(PropertyFailure.szDescribeString));

		//��������
		bool bSendSuccess = m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GF_PROPERTY_FAILURE,&PropertyFailure,sizeof(PropertyFailure));

		if(bSendSuccess==true) return true; 
	}

	//������Ϣ
	CInformation Information;
	Information.ShowMessageBox(pPropertyFailure->szDescribeString,MB_ICONINFORMATION);

	return true;
}

//����ЧӦ
bool CPlazaViewEntry::OnSocketSubPropertyEffect(VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize==sizeof(CMD_GR_S_PropertyEffect));
	if(wDataSize!=sizeof(CMD_GR_S_PropertyEffect))  return false;

	//��������
	CMD_GR_S_PropertyEffect * pPropertyEffect=(CMD_GR_S_PropertyEffect *)pData;
	if(pPropertyEffect==NULL) return false;

	//�����û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertyEffect->wUserID);
	if(pISendUserItem==NULL) return true;

	//���ñ���
	tagUserInfo * pUserInfo = pISendUserItem->GetUserInfo();
	pUserInfo->cbMemberOrder=pPropertyEffect->cbMemberOrder;

	//������Ϣ
	// m_pPlazaViewServerCtrl->OnEventUserUpdate(pISendUserItem);
    
    return true;
}

//�������
bool CPlazaViewEntry::OnSocketSubUserInvite(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserInvite));
	if (wDataSize!=sizeof(CMD_GR_UserInvite)) return false;

	//��Ϣ����
	CMD_GR_UserInvite * pUserInvite=(CMD_GR_UserInvite *)pData;
	ASSERT(pUserInvite->wTableID<m_pITableViewFrame->GetTableCount());
	if (m_pITableViewFrame->GetPlayFlag(pUserInvite->wTableID)==true) return true;

	//Ѱ���û�
	IClientUserItem * pIUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserInvite->dwUserID);
	if (pIUserItem==NULL) return true;
	if (pIUserItem->GetTableID()==INVALID_TABLE) return true;

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//�û�����
	if (pIUserItem->GetUserCompanion()==CP_DETEST) return true;
	if (pParameterGlobal->m_cbInviteMode==INVITE_MODE_NONE) return true;
	if ((pParameterGlobal->m_cbInviteMode==INVITE_MODE_FRIEND)&&(pIUserItem->GetUserCompanion()!=CP_FRIEND)) return true;

	//��ʾ��Ϣ
	TCHAR szMessage[256]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] ����������� [ %ld ] ��Ϸ��������Ϸ���Ƿ�ͬ�⣿"),pIUserItem->GetNickName(),pUserInvite->wTableID+1);
	//��ʾ��Ϣ
	CInformation Information(this);
	if (Information.ShowMessageBox(szMessage,MB_ICONINFORMATION|MB_YESNO)==IDYES)
	{
		WORD wChairID=INVALID_CHAIR;
		WORD wNullCount=m_pITableViewFrame->GetNullChairCount(pUserInvite->wTableID,wChairID);
		if (wNullCount==0) 
		{
			CInformation Information2(this);
			Information2.ShowMessageBox(TEXT("����Ϸ���Ѿ�û�п�λ���ˣ�"),MB_ICONINFORMATION);
			return true;
		}
		PerformSitDownAction(pUserInvite->wTableID,wChairID,false);
	}

	return true;
}

//������Ϣ
bool CPlazaViewEntry::OnSocketPropertyMessage(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_S_PropertyMessage));
	if (wDataSize!=sizeof(CMD_GR_S_PropertyMessage)) return false;

	//У������
	CMD_GR_S_PropertyMessage * pPropertyMessage = (CMD_GR_S_PropertyMessage *)pData;
	if(pPropertyMessage==NULL) return false;

    //��ȡ����
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//Ѱ�ҵ���
	ASSERT(pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex)!=NULL);
	CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex);
	if(pGamePropertyItem==NULL) return true;

	//�����ж�
	if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY) return true;

	//��ȡ�û�
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertyMessage->dwSourceUserID);
	if (pISendUserItem==NULL) return true;

	//��ȡ�û�
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pPropertyMessage->dwTargerUserID);
	if (pIRecvUserItem==NULL) return true;

	//��ȡ��Ϣ
	TCHAR szNotifyMessage[128]=TEXT("");	
	pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),pIRecvUserItem->GetNickName(),pPropertyMessage->wPropertyCount,szNotifyMessage);

	//��ȡָ��
	/* IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//������Ϣ
	pIStringMessage->InsertPromptString(szNotifyMessage);

	if(IsGameClientReady())
	{
		//����ṹ
		IPC_GR_PropertyMessage  PropertyMessage;
		PropertyMessage.wPropertyIndex=pPropertyMessage->wPropertyIndex;
		PropertyMessage.wPropertyCount=pPropertyMessage->wPropertyCount;
		CopyMemory(PropertyMessage.szSourceNickName,pISendUserItem->GetNickName(),sizeof(PropertyMessage.szSourceNickName));
		CopyMemory(PropertyMessage.szTargerNickName,pIRecvUserItem->GetNickName(),sizeof(PropertyMessage.szTargerNickName));

		//��������
		m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_MESSAGE,&PropertyMessage,sizeof(PropertyMessage));
	} */

	return true;
}

//��������
bool CPlazaViewEntry::OnSocketPropertyTrumpet(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_S_SendTrumpet));
	if (wDataSize!=sizeof(CMD_GR_S_SendTrumpet)) return false;

	//��������
	CMD_GR_S_SendTrumpet * pSendTrumpet=(CMD_GR_S_SendTrumpet *)pData;

	//��ȡָ��
	/* IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

    //������Ϣ
	if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)
		pIStringMessage->InsertUserTrumpet(pSendTrumpet->szSendNickName,pSendTrumpet->szTrumpetContent,pSendTrumpet->TrumpetColor);

	//������Ϣ
	if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TYPHON)
        pIStringMessage->InsertUserTyphon(pSendTrumpet->szSendNickName,pSendTrumpet->szTrumpetContent,pSendTrumpet->TrumpetColor);

	//���°���
	if(pSendTrumpet->dwSendUserID==m_pIMySelfUserItem->GetUserID())
	{
		//���߰���
		tagPropertyPackage * pPropertyPackage=m_pIMySelfUserItem->GetPropertyPackage();

		//���°���
		if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET) --pPropertyPackage->wTrumpetCount;
		if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TYPHON) --pPropertyPackage->wTyphonCount;
		//if(m_pTrumpetItem!=NULL) m_pTrumpetItem->UpdateControlSurface();
	} */

	if(IsGameClientReady())
	{
		//����ṹ
		IPC_GR_PropertyTrumpet  PropertyTrumpet;
		PropertyTrumpet.dwSendUserID=pSendTrumpet->dwSendUserID;
		PropertyTrumpet.wPropertyIndex=pSendTrumpet->wPropertyIndex;
		PropertyTrumpet.TrumpetColor=pSendTrumpet->TrumpetColor;
		CopyMemory(PropertyTrumpet.szSendNickName,pSendTrumpet->szSendNickName,sizeof(PropertyTrumpet.szSendNickName));
		CopyMemory(PropertyTrumpet.szTrumpetContent,pSendTrumpet->szTrumpetContent,sizeof(PropertyTrumpet.szTrumpetContent));

		//��������
		m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_TRUMPET,&PropertyTrumpet,sizeof(PropertyTrumpet));
	}

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	if(pParameterGlobal->m_bAllowSound)
	{
		//��������
		HINSTANCE hInstance = AfxGetInstanceHandle();
		bool bPlaying=false;

		//��ȡ����
		CD3DSound * pD3DSound=CD3DSound::GetInstance();
		if(pD3DSound)
		{
			bPlaying = pD3DSound->PlaySound(hInstance, (pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?TEXT("BUGLE"):TEXT("BUGLE_EX"), TEXT("WAVE"), false);
		}

		//ϵͳ����
		if(!bPlaying)
		{
			HRSRC hResour=FindResource(hInstance,(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?TEXT("BUGLE"):TEXT("BUGLE_EX"),TEXT("WAVE"));
			if (hResour!=NULL)
			{
				HGLOBAL hGlobal=LoadResource(hInstance,hResour);
				if (hGlobal!=NULL) 
				{
					//��������
					LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
					BOOL bResult = ::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY|SND_NODEFAULT);

					//������Դ
					UnlockResource(hGlobal);
				}
				FreeResource(hGlobal);
			}
		}
	}

    return true;
}

//�ȴ�����
bool CPlazaViewEntry::OnSocketSubWaitDistribute(VOID *pData, WORD wDataSize)
{
	//��������
	m_pITableViewFrame->SetTableStatus(true);

	//��Ϸ֪ͨ
	OnSocketMainGameFrame(MDM_GF_FRAME,SUB_GR_USER_WAIT_DISTRIBUTE,pData,wDataSize);

	return true;
}

//ϵͳ��Ϣ
bool CPlazaViewEntry::OnSocketSubSystemMessage(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	//Ч�����
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR)))) return false;

	WORD wType = pSystemMessage->wType;

	//�رմ���
	if ((wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//�ر���ʾ
		m_DlgStatus.HideStatusWindow();

		//�رմ���
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//״̬����
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//��ȡָ��
	/* IStringMessage * pIStringMessage = m_pPlazaViewServerCtrl->GetStringMessage();
	ASSERT(pIStringMessage!=NULL);

	//��ʾ��Ϣ
	if (wType&SMT_CHAT) 
	{
		pIStringMessage->InsertSystemString(pSystemMessage->szString);
	}

	//�ر�����
	if (wType&SMT_CLOSE_INSURE)
	{
		if(m_pDlgInsureServer != NULL) 
		{
			m_pDlgInsureServer->EndDialog(IDOK);
			m_pDlgInsureServer->DestroyWindow();
		}
	} */

	//�ر���Ϸ
	if (wType&SMT_CLOSE_GAME)
	{
		m_ProcessManager.CloseGameProcess(false);
	}

	//������Ϣ
	if (wType&SMT_EJECT)
	{
		CInformation Information;
		Information.ShowMessageBox(pSystemMessage->szString,MB_ICONINFORMATION);
	}

	//�رշ���
	if (wType&SMT_CLOSE_ROOM)
	{
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
	}

	return true;
}

//������Ϣ
bool CPlazaViewEntry::OnSocketSubActionMessage(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	//Ч�����
	ASSERT((wDataSize>wHeadSize)&&(wDataSize>=(wHeadSize+pActionMessage->wLength*sizeof(WCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize<(wHeadSize+pActionMessage->wLength*sizeof(WCHAR)))) return false;

	//�رմ���
	if ((pActionMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//�ر���ʾ
		m_DlgStatus.HideStatusWindow();

		//�رմ���
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//״̬����
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//������Ϣ
	CInformation Information;
	INT nResultCode=Information.ShowMessageBox(pActionMessage->szString,MB_ICONINFORMATION);

	//��������
	WORD wExcursion=wHeadSize+pActionMessage->wLength*sizeof(TCHAR);

	//��ȡ����
	while (wExcursion<wDataSize)
	{
		//��������
		tagActionHead * pActionHead=(tagActionHead *)((BYTE *)pData+wExcursion);

		//Ч�����
		ASSERT((wExcursion+sizeof(tagActionHead))<=wDataSize);
		ASSERT((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)<=wDataSize);

		//Ч�����
		if ((wExcursion+sizeof(tagActionHead))>wDataSize) return false;
		if ((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)>wDataSize) return false;

		//��������
		if (nResultCode==pActionHead->uResponseID)
		{
			switch (pActionHead->cbActionType)
			{
			case ACT_BROWSE:	//�������
				{
					//��������
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionBrowse * pActionBrowse=(tagActionBrowse *)((BYTE *)pData+wDataPos);

					//I E ���
					if (pActionBrowse->cbBrowseType&BRT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionBrowse->szBrowseUrl,NULL,NULL,SW_NORMAL);
					}

					//�������
					/* if (pActionBrowse->cbBrowseType&BRT_PLAZA)
					{
						CPlatformFrame::GetInstance()->WebBrowse(pActionBrowse->szBrowseUrl,true);
					} */

					break;
				}
			case ACT_DOWN_LOAD:	//���ض���
				{
					//��������
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionDownLoad * pActionDownLoad=(tagActionDownLoad *)((BYTE *)pData+wDataPos);

					//I E ����
					if (pActionDownLoad->cbDownLoadMode&DLT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					//����ģ��
					if (pActionDownLoad->cbDownLoadMode&DLT_MODULE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					break;
				}
			}
		}

		//����ƫ��
		wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	}

	//�رշ���
	if (pActionMessage->wType&SMT_CLOSE_ROOM) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//����״̬
VOID CPlazaViewEntry::SetServiceStatus(enServiceStatus ServiceStatus)
{
	//���ñ���
	m_ServiceStatus=ServiceStatus;

	return;
}

//��ѯ��Ϸ�ͻ���
bool CPlazaViewEntry::IsGameClientReady()
{
	return m_ProcessManager.IsProcessReady();
}

//��ԴĿ¼
VOID CPlazaViewEntry::GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount)
{
	//��������
	WORD wStringIndex=0;

	//����Ŀ¼
	ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szResDirectory,m_GameKind.szProcessName,wBufferCount);
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

	//�ַ���ֹ
	szResDirectory[wStringIndex]=0;

	return;
}

//��������
VOID CPlazaViewEntry::SearchGameTable(tagSearchTable & SearchTable)
{
	//��������
	WORD wNullCount = 0;
	WORD wChairUser = m_pITableViewFrame->GetChairCount();
	WORD wMeTableID = INVALID_TABLE;
	if (m_pIMySelfUserItem!=NULL) {
		wMeTableID = m_pIMySelfUserItem->GetTableID();
	}

	//������Ϸ
	if (CServerRule::IsAllowDynamicJoin(m_dwServerRule)==true)
	{
		if(m_pITableViewFrame->GetChairCount() >= INDEX_ENTER_CHAIR)
		{
			SearchTable.wResultTableID=0;
			SearchTable.wResultChairID=INDEX_ENTER_CHAIR;

			return;
		}
	}

	//��������
	for (WORD i=0;i<m_pITableViewFrame->GetTableCount();i++)
	{
		//��������
		SearchTable.wResultTableID=(SearchTable.wStartTableID+i)%m_pITableViewFrame->GetTableCount();

		//��������
		if (wMeTableID!=SearchTable.wResultTableID)
		{
			//��ȡ����
			ITableView * pITableView=m_pITableViewFrame->GetTableViewItem(SearchTable.wResultTableID);

			//��������
			if (pITableView->GetPlayFlag()==true) continue;
			if ((SearchTable.bFilterPass==true)&&(pITableView->GetLockerFlag()==true)) continue;

			//Ѱ�ҿ�λ��
			wNullCount=pITableView->GetNullChairCount(SearchTable.wResultChairID);

			//�ж���Ŀ
			if (wNullCount>0)
			{
				//Ч�����
				CString strDescribe;
				if (EfficacyTableRule(SearchTable.wResultTableID,SearchTable.wResultChairID,false,strDescribe)==false)
				{
					continue;
				}
		
				//��Ŀ�ж�
				if ((SearchTable.bOneNull==true)&&(wNullCount==1)) return;
				if ((SearchTable.bTwoNull==true)&&(wNullCount==2)) return;
				if ((SearchTable.bNotFull==true)&&(wNullCount<wChairUser)) return;
				if ((SearchTable.bAllNull==true)&&(wNullCount==wChairUser)) return;
			}
		}
	}

	//��������
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;

	return;
}

//ȡ������
VOID CPlazaViewEntry::OnStatusCancel()
{
	//�رշ���
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}