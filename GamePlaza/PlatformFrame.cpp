#include "Stdafx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "PlatformFrame.h"
#include "DlgEnquire.h"

//////////////////////////////////////////////////////////////////////////////////

#define LAYERED_SIZE				5									//�ֲ��С

//////////////////////////////////////////////////////////////////////////////////

//��Ļ����
#define LESS_LIST_CX				285									//�б���
#define SPLITTER_WIDTH				8									//��ֿ��

//��Ļ����
#define LESS_SCREEN_CY				740									//��С�߶�
#define LESS_SCREEN_CX				1024								//��С���

//////////////////////////////////////////////////////////////////////////////////

//�ؼ���ʶ
#define IDC_SERVER_LIST				300									//��Ϸ�б�
#define IDC_WEB_PUBLICIZE			301									//����ؼ�
#define IDC_SKIN_SPLITTER			302									//��ֿؼ�
#define IDC_SYSTEM_TRAY_ICON		303									//����ͼ��
#define IDC_PLAZA_VIEW_GAME_TYPE	304									//���Ϳؼ�
#define IDC_PLAZA_VIEW_GAME			305									//��Ϸ��ͼ
#define IDC_PLAZA_GAME_CONTAINER	306									//��Ϸ����
#define IDC_PLAZA_SERVER_CONTAINER	307									//��������

//�ؼ���ʶ
#define IDC_PLAZA_VIEW_AFFICHE		310									//�������
#define IDC_PLAZA_VIEW_USER			311									//�û���Ϣ
#define IDC_PLAZA_VIEW_SERVER		312									//�������
#define IDC_PLAZA_VIEW_ENTRY		313									//���뷿��
#define IDC_PLAZA_VIEW_NEWS			314									//������Ϣ
#define IDC_PLAZA_VIEW_NOTICE		315									//������Ϣ

//////////////////////////////////////////////////////////////////////////////////

//������Ŀ
#define MAX_SERVER_COUNT			5									//������Ŀ

//ʱ���ʶ
#define IDI_FLASH_TRAY_ICON			10									//����ͼ��

//ʱ���ʶ
#define IDI_UPDATA_ONLINE			16									//��������
#define TIME_UPDATA_ONLINE			10000								//��������

//////////////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define WM_TASK_BAR					0x0313								//�������Ҽ�
#define WM_INSTALL_CANCEL_EVENT		8888								//ȡ����װ

//////////////////////////////////////////////////////////////////////////////////

//��ť�ؼ�
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

//��̬����
CPlatformFrame * CPlatformFrame::m_pPlatformFrame=NULL;					//���ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlatformFrame, CFGuiFrameWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()

	//�Զ���Ϣ
	ON_MESSAGE(WM_HOTKEY, OnMessageHotKey)
	ON_MESSAGE(WM_TASK_BAR, OnMessageTaskBar)	
	ON_MESSAGE(WM_TRAY_ICON, OnMessageTrayIcon)
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)
	ON_MESSAGE(WM_INSTALL_CANCEL_EVENT, OnMessageInstallCancelEvent)

	//�Զ���Ϣ
	ON_MESSAGE(WM_INSUREPLAZA_EVENT, OnMessageInsurePlazaEvent)
	
	//�Զ���Ϣ
	ON_MESSAGE(WM_ENTER_GAME, OnMessageEnterGame)
	ON_MESSAGE(WM_QUIT_GAME, OnMessageQuitGame)
	ON_MESSAGE(WM_ENTER_SERVER, OnMessageEnterServer)
	ON_MESSAGE(WM_LOGON_SERVER, OnMessageLogonServer)
	ON_MESSAGE(WM_LOGON_SUCCESS, OnMessageLogonSuccess)
	ON_MESSAGE(WM_QUIT_SERVER, OnMessageQuitServer)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlatformFrame::CPlatformFrame()
{
	//�������
	m_MissionManager.InsertMissionItem(&m_MissionLogon);
	m_MissionManager.InsertMissionItem(&m_MissionUpdate);
	m_MissionManager.InsertMissionItem(&m_MissionList);

	//ƽ̨����
	ASSERT(m_pPlatformFrame==NULL);
	if (m_pPlatformFrame==NULL) m_pPlatformFrame=this;

	m_bServerStatu = false;
	return;
}


//��������
CPlatformFrame::~CPlatformFrame()
{
	//ƽ̨����
	ASSERT( m_pPlatformFrame==this );

	if ( m_pPlatformFrame==this ) {
		m_pPlatformFrame=NULL;
	}

	return;
}

//�ӿڲ�ѯ
VOID * CPlatformFrame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICustomFaceEvent,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICustomFaceEvent,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ؼ�
void CPlatformFrame::InitControlUI()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	m_PaintManager.SetCaptionRect( CRect(0,0,0,38) );
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	CControlUI * pParent = static_cast<CControlUI *>( m_PaintManager.GetRoot() );
	if(pParent==NULL) return;

	//LOGO����
	CLabelUI * pFrameLogo = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelLogo") );
	if( pFrameLogo != NULL )  {
		pFrameLogo->SetFloat(true);
		pFrameLogo->SetPos(28,13);
		pFrameLogo->SetFixedWidth(187);
		pFrameLogo->SetFixedHeight(67);
		pFrameLogo->SetBkImage( TEXT("file='PLATFORM_LOGO' restype='PNG'") );
	}

	//���ű���
	CLabelUI * pFrameNewsBK = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("PlatformFrameNewsBK") );
	if( pFrameNewsBK != NULL )  {
		pFrameNewsBK->SetFloat(true);
		pFrameNewsBK->SetPos(10,395);
		pFrameNewsBK->SetFixedWidth(229);
		pFrameNewsBK->SetFixedHeight(230);
		pFrameNewsBK->SetBkImage( TEXT("file='PF_NEWS_BK' restype='PNG'") );
	}

	//���ű���
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

	//��С����ť
	CButtonUI * pButtonMin = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonMin") );
	if( pButtonMin != NULL )  {
		pButtonMin->SetFloat(true);
		pButtonMin->SetPos(940,8);
		pButtonMin->SetFixedWidth(30);
		pButtonMin->SetFixedHeight(35);
		pButtonMin->SetStatusImage( TEXT("file='BT_LOGON_MIN' restype='PNG'") );
	}

	//�رհ�ť
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

	//ƽ�ⰴť
	CLabelUI * pLblFrameBanner = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("ButtonFrameBanner") );
	if( pLblFrameBanner != NULL )  {
		pLblFrameBanner->SetTag(7);
		pLblFrameBanner->SetFloat(true);
		pLblFrameBanner->SetPos(10,632);
		pLblFrameBanner->SetFixedWidth(225);
		pLblFrameBanner->SetFixedHeight(98);
		pLblFrameBanner->SetBkImage( TEXT("file='BT_BANNER' restype='PNG'") );
	}

	//������Դ
	m_ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("PLATFORM_BK"));
}

//��Ϣ����
void CPlatformFrame::Notify(TNotifyUI &  msg)
{	
	//��ȡ����
	/////////////////////////////////////////////////////////////////////////////
	CControlUI * pControlUI = msg.pSender;

	//����¼�
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
			//��ȡ����
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
			
			//�����ַ
			TCHAR szBillUrl[256]=TEXT("");
			ZeroMemory(szBillUrl,CountArray(szBillUrl));
			_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/PlatformBanner.aspx"),pGlobalWebLink->GetPlatformLink());
			
			//��ҳ��
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
			//��������
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//��ȡ����
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//�����ַ
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sNavigation%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(),pControlUI->GetTag());

			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);

			return;
		}
	}
}


//�����滭
void CPlatformFrame::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	m_ImageBack.DrawImage(pDC,0,0);
}


//����ʧ��
VOID CPlatformFrame::OnEventSystemFace(DWORD dwUserID, WORD wFaceID)
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//���ñ���
		pGlobalUserData->wFaceID=wFaceID;

		//��������
		pGlobalUserData->dwCustomID=0L;
		ZeroMemory(&pGlobalUserData->CustomFaceInfo,sizeof(pGlobalUserData->CustomFaceInfo));

		//�����¼�
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	return;
}

//�����ж�
bool CPlatformFrame::IsEnteredServerItem()
{
	return m_bServerStatu;
}

//�û�����
VOID CPlatformFrame::UpdateUserInfo()
{
	m_PlazaViewUser.PostMessage(WM_UPDATE_USERINFO, 0, 0);
}

//ͷ������
VOID CPlatformFrame::OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//���ñ���
		pGlobalUserData->dwCustomID=dwCustomID;
		CopyMemory(&pGlobalUserData->CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

		//�����¼�
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	//��ǰ����

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


//��ȡ֪ͨ
VOID CPlatformFrame::OnGameItemFinish()
{
	//��ʾ�б�
	// m_PlazaViewGame.ShowKindItemView(m_PlazaViewGameType.GetCurrActiveIndex(),FALSE);
	return;
}

//���֪ͨ
VOID CPlatformFrame::OnGameMatchFinish()
{
	//��ʾ����
	/* if(m_PlazaViewGame.GetShowItemMode()==VIEW_MODE_MATCH) {
		m_PlazaViewGame.ShowMatchItemView(FALSE);
	} */

	return;
}

//��ȡ֪ͨ
VOID CPlatformFrame::OnGameKindFinish(WORD wKindID)
{
	//��ʾ����
	if(wKindID!=INVALID_WORD) {
		// m_PlazaViewGame.ShowServerItemView(wKindID,TRUE);
	}

	return;
}

//����֪ͨ
VOID CPlatformFrame::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//Ч�����
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//����ʱ��
	pGameListItem->m_dwUpdateTime=(DWORD)time(NULL);

	//���봦��
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//��Ϸ����
		{
			break;
		}
	case ItemGenre_Kind:	//��Ϸ����
		{
			//��ȡ����
			CGameKindItem * pGameKindItem = (CGameKindItem *)pGameListItem;
			m_PlazaViewGame.InsertGameItem( pGameKindItem->m_GameKind.wKindID, pGameKindItem->m_dwProcessVersion);
			break;
		}
	case ItemGenre_Server:	//��Ϸ����
		{
			CGameServerItem * pGameServerItem = (CGameServerItem *)pGameListItem;
			m_PlazaViewGame.InsertServerItem( pGameServerItem );
			break;
		}
	}

	return;
}

//����֪ͨ
VOID CPlatformFrame::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	//����ʱ��
	pGameListItem->m_dwUpdateTime=(DWORD)time(NULL);

	switch(pGameListItem->GetItemGenre())
	{		
	case ItemGenre_Type:	//��������
		{
			break;
		}
	case ItemGenre_Kind:	//��Ϸ����
		{
			//��ȡ����
			// CGameKindItem * pGameKindItem = (CGameKindItem *)pGameListItem;
			// m_PlazaViewGame.UpdateGameItem( pGameKindItem->m_GameKind.wKindID, pGameKindItem->m_dwProcessVersion);
			break;
		}
	case ItemGenre_Server:	//��������
		{
			break;
		}
	}

	return;
}

//ɾ��֪ͨ
VOID CPlatformFrame::OnGameItemDelete(CGameListItem * pGameListItem)
{

	return;
}


//��Ϣ����
BOOL CPlatformFrame::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if (pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_RETURN) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CPlatformFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//�˵�����
	switch (nCommandID)
	{
	case IDM_USER_LOGON:		//�û���¼
		{
			m_MissionLogon.ShowLogon();

			return TRUE;
		}
	case IDM_DELETE_SERVER_ITEM:	//�رշ���
		{
			m_PlazaViewEntry.PostMessage(WM_DELETESERVERITEM,0,0);

			return TRUE;
		}
	case IDM_SWITCH_ACCOUNTS:	//�л��ʺ�
		{
			//�л�ѯ��
			if (m_PlazaViewServer.m_hWnd!=NULL)
			{
				CInformation Information(this);
				if (Information.ShowMessageBox(TEXT("��Ϸ���伴���رգ�ȷ��Ҫ���л��ʺš��� "),MB_YESNO)!=IDYES) 
					return TRUE;
			}

			//ȫ������
			//CGlobalUnits::GetInstance()->DeleteUserCookie();
			CGlobalUserInfo::GetInstance()->ResetUserInfoData();

			//��ȡ����
			CServerListData * pServerListData = CServerListData::GetInstance();

			//�������
			POSITION Position=NULL;
			do
			{
				//��ȡ����
				CGameServerItem * pGameServerItem=pServerListData->EmunGameServerItem(Position);
				if(pGameServerItem!=NULL)
				{
					pGameServerItem->m_bSignuped=false;
					ZeroMemory(&pGameServerItem->m_GameMatch,sizeof(pGameServerItem->m_GameMatch));
				}

			} while (Position!=NULL);

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) 
				pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGOUT,0L);

			return TRUE;
		}
	case IDM_CLOSE:		//�ر�
		{
			//Ͷ����Ϣ
			PostMessage(WM_CLOSE,0,0);

			return TRUE;
		}
	case IDM_RESTORE:   //��ԭ
		{
			if(IsIconic())
			{
				m_bServerStatu = false;
				ShowWindow(SW_RESTORE);
			}
			return TRUE;
		}
	case IDM_MINIMIZE:  //��С��
		{
			if(IsWindowVisible())
			{
				m_bServerStatu = true;
				ShowWindow(SW_MINIMIZE);
			}
			return TRUE;
		}
	case IDM_MAXIMIZE:  //���
		{
			return TRUE;
		}
	case IDM_SHOW_LOGON: //��ʾ��¼
		{
			m_MissionLogon.ShowLogon(SW_RESTORE);

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//�ر���Ϣ
VOID CPlatformFrame::OnClose()
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�ر���ʾ
	if ((pGlobalUserData->dwUserID!=0L)&&(m_MissionLogon.GetActiveStatus()==false))
	{
		//��ʾ����
		CDlgEnquire DlgEnquire;
		INT_PTR nResult=DlgEnquire.DoModal();

		//�����
		switch (nResult)
		{
		case IDCANCEL:				//ȡ������
			{
				return;
			}
		case IDC_CLOSE_SERVER:		//�˳�����
			{
				//�رշ���
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
				return;
			}
		case IDC_SWITCH_ACCOUNTS:	//�л��ʺ�
			{
				//Ͷ����Ϣ
				PostMessage(WM_COMMAND,IDM_SWITCH_ACCOUNTS,0);
				return;
			}
		}
	}

	//ע���ȼ�
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS);
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_WHISPER);

	__super::OnClose();
}

//ʱ����Ϣ
VOID CPlatformFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_FLASH_TRAY_ICON:	//����ͼ��
		{
			return;
		}
	}
	__super::OnTimer(nIDEvent);
}

//λ����Ϣ
VOID CPlatformFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	// RectifyControl(cx,cy);

	return;
}

//λ����Ϣ
VOID CPlatformFrame::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//����λ��
	lpMMI->ptMinTrackSize.x=__min(LESS_SCREEN_CX,rcArce.Width());
	lpMMI->ptMinTrackSize.y=__min(LESS_SCREEN_CY,rcArce.Height());

	return;
}

//������Ϣ
INT CPlatformFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ô���
	ModifyStyle(WS_CAPTION|WS_BORDER, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
	ModifyStyleEx(WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);	

	//��������
	CRect rcViewGame( PARENT_VIEWGAME_X, PARENT_VIEWGAME_Y, 0, 0);
	CRect rcUserInfo( PARENT_USERINFO_X, PARENT_USERINFO_Y, 0, 0);
	CRect rcViewServer( PARENT_VIEWSERVER_X, PARENT_VIEWSERVER_Y, 0, 0);
	CRect rcViewEntry( PARENT_VIEWENTRY_X, PARENT_VIEWENTRY_Y, 0, 0);
	CRect rcViewNews( PARENT_VIEWNEWS_X, PARENT_VIEWNEWS_Y, 0, 0);
	CRect rcViewNotice( PARENT_VIEWNOTICE_X, PARENT_VIEWNOTICE_Y, 0, 0);

	//��ͼ�ؼ�
	m_PlazaViewUser.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcUserInfo,this,IDC_PLAZA_VIEW_USER);
	m_PlazaViewGame.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcViewGame,this,IDC_PLAZA_VIEW_GAME);
	m_PlazaViewServer.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcViewServer,this,IDC_PLAZA_VIEW_SERVER);
	m_PlazaViewEntry.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcViewEntry,this,IDC_PLAZA_VIEW_ENTRY);
	m_PlazaViewNews.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcViewNews,this,IDC_PLAZA_VIEW_NEWS);
	m_PlazaViewNotice.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcViewNotice,this,IDC_PLAZA_VIEW_NEWS);

	//��ͼ����
	m_PlazaViewServer.ShowWindow(SW_HIDE);
	m_PlazaViewEntry.ShowWindow(SW_HIDE);

	//��Ϸ�б�
	ASSERT(CServerListData::GetInstance()!=NULL);
	CServerListData::GetInstance()->SetServerListDataSink(this);
	
	//ע���¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	//����λ��
	CSize SizeRestrict;
	SizeRestrict.SetSize(LESS_SCREEN_CX,LESS_SCREEN_CY);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//��ȡλ��
	CWHRegKey InfoKeyItem;
	if (InfoKeyItem.OpenRegKey(REG_INTERFACE_INFO,false)==true)
	{
		SizeRestrict.cx=InfoKeyItem.GetValue(TEXT("ScreenWidth"),rcArce.Width()/2);
		SizeRestrict.cy=InfoKeyItem.GetValue(TEXT("ScreenHeight"),rcArce.Height()/2);
	}

	//λ�õ���
	SizeRestrict.cx=__max(LESS_SCREEN_CX,SizeRestrict.cx);
	SizeRestrict.cy=__max(LESS_SCREEN_CY,SizeRestrict.cy);
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//�ƶ�����
	m_rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
	m_rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
	m_rcNormalSize.right=m_rcNormalSize.left+SizeRestrict.cx;
	m_rcNormalSize.bottom=m_rcNormalSize.top+SizeRestrict.cy;

	//�ƶ�����
	SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);

	//��ʾ����
	SetWindowRgn(NULL,TRUE);

	//��¼ϵͳ
	PostMessage(WM_COMMAND,IDM_USER_LOGON,0);

	//ע���ȼ�
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	CWHService::RegisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS,pParameterGlobal->m_wBossHotKey);
	CWHService::RegisterHotKey(m_hWnd,IDI_HOT_KEY_WHISPER,pParameterGlobal->m_wWhisperHotKey);

	//��������
	// m_SystemTrayIcon.InitTrayIcon(m_hWnd,IDC_SYSTEM_TRAY_ICON);
	// m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)),szProduct);

	//��������
	SetTimer(IDI_UPDATA_ONLINE, TIME_UPDATA_ONLINE, NULL);

	//��������
	CSkinDialog::SetWndFont(this,NULL);

	return 0;
}

//�����Ϣ
VOID CPlatformFrame::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//״̬�ж�
	if (Point.y>GetCaptionRect().Height()) return;

	//���ڿ���
	/*��if (m_bMaxShow==true)
	{
		RestoreWindow();
	}
	else
	{
		MaxSizeWindow();
	}��*/
	
	return;
}

//���øı�
VOID CPlatformFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	//���ڿ���
	/*��if (m_bMaxShow==true)
	{
		m_bMaxShow=false;
		MaxSizeWindow(false);
	}
	else
	{
		m_bMaxShow=true;
		RestoreWindow();
	}��*/

	return;
}

//�ȼ���Ϣ
LRESULT CPlatformFrame::OnMessageHotKey(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDI_HOT_KEY_BOSS:		//�ϰ��ȼ�
		{
			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//��¼�ж�
			if (pGlobalUserData->dwUserID!=0L)
			{
				//��������
				bool bBossCome=(IsWindowVisible()==FALSE)?false:true;

				//���ش���
				if (bBossCome==false)
				{
					//��ԭ����
					ShowWindow(SW_RESTORE);
					ShowWindow(SW_SHOW);

					//�ö�����
					SetActiveWindow();
					BringWindowToTop();
					SetForegroundWindow();

					//��ʾͼ��
					// m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
				}
				else
				{
					//���ش���
					ShowWindow(SW_MINIMIZE);
					ShowWindow(SW_HIDE);

					//����ͼ��
					// m_SystemTrayIcon.HideTrayIcon();
				}

				////������Ϣ
				//for (INT_PTR i=0;i<m_PlazaViewServerArray.GetCount();i++)
				//{
				//	ASSERT(m_PlazaViewServerArray[i]!=NULL);
				//	m_PlazaViewServerArray[i]->NotifyBossCome(bBossCome);
				//}
			}

			return 0;
		}
	case IDI_HOT_KEY_WHISPER:	//˽���ȼ�
		{
			//��ȡ��Ϣ
			/*��if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			}��*/

			return 0;
		}
	}

	return 0;
}

//������Ϣ
LRESULT CPlatformFrame::OnMessageTaskBar(WPARAM wParam, LPARAM lParam)
{
	//�����˵�
	CSkinMenu Menu;
	Menu.CreateMenu();

	//����˵�
	Menu.AppendMenu(IDM_RESTORE,TEXT("��ԭ"), (IsIconic()||IsWindowVisible()==FALSE)?MF_ENABLED:MF_GRAYED);
	Menu.AppendMenu(IDM_MINIMIZE,TEXT("��С��"),(IsIconic()==FALSE||IsWindowVisible())?MF_ENABLED:MF_GRAYED);
	Menu.AppendMenu(IDM_MAXIMIZE,TEXT("���"));
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_CLOSE,TEXT("�˳��㳡"));

	//��ʾ�˵�
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);

	return 0L;
}

//ͼ����Ϣ
LRESULT CPlatformFrame::OnMessageTrayIcon(WPARAM wParam, LPARAM lParam)
{
	//�¼�����
	switch (lParam)
	{
	case WM_LBUTTONDOWN:		//��굥��
		{
			if(IsIconic())
			{
				ShowWindow(SW_RESTORE);
			}

			//�����
			SetActiveWindow();
			BringWindowToTop();
			SetForegroundWindow();

			return 1;
		}
	case WM_LBUTTONDBLCLK:		//���˫��
		{
			//��ȡ��Ϣ
			
			/* if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			} */

			return 1;
		}
	case WM_RBUTTONDOWN:		//��굥��
	case WM_RBUTTONDBLCLK:		//���˫��
		{
			//�����˵�
			CSkinMenu Menu;
			Menu.CreateMenu();

			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//�û�δ��½
			if(pGlobalUserData->dwUserID == 0)
			{
				Menu.AppendMenu(IDM_SHOW_LOGON, TEXT("��ʾ�����"), MF_ENABLED);
				Menu.AppendMenu(IDM_CLOSE, TEXT("�˳��㳡"), MF_ENABLED);
			}
			else
			{
				//����˵�
				Menu.AppendMenu(IDM_RESTORE,TEXT("��ԭ"), (IsIconic()||IsWindowVisible()==FALSE)?MF_ENABLED:MF_GRAYED);
				Menu.AppendMenu(IDM_MINIMIZE,TEXT("��С��"),(IsIconic()==FALSE||IsWindowVisible())?MF_ENABLED:MF_GRAYED);
				Menu.AppendMenu(IDM_MAXIMIZE,TEXT("���"));
				Menu.AppendSeparator();
				Menu.AppendMenu(IDM_CLOSE,TEXT("�˳��㳡"));
			}			
			
			//Menu.AppendMenu(IDM_NULL_COMMAND,TEXT("����..."));

			//��ʾ�˵�
			CPoint MousePoint;
			GetCursorPos(&MousePoint);
			Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);

			return 1;
		}
	}

	return 0;
}

//�¼���Ϣ
LRESULT CPlatformFrame::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	//�¼�����
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//��¼���
		{
			//������Ϣ
			m_MissionUpdate.PerformUpdateMission(&m_PlazaViewNews, &m_PlazaViewNotice);
			
			//��ʾ����
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
			return 0L;
		}
	case EVENT_USER_LOGOUT:			//ע���ɹ�
		{
			//���ش���
			ShowWindow(SW_HIDE);

			//�رշ���

			//��ʾ��¼
			m_MissionLogon.ShowLogon();

			return 0L;
		}
	case EVENT_DOWN_LOAD_FINISH:	//�������
		{
			//��ȡ����
			WORD wKindID=LOWORD(lParam);
			WORD wServerID=HIWORD(lParam);

			//�����б�
			CServerListData * pServerListData=CServerListData::GetInstance();
			if (pServerListData!=NULL) pServerListData->OnEventDownLoadFinish(wKindID);

			return 0L;
		}
	}

	return 0L;
}

//�¼���Ϣ
LRESULT CPlatformFrame::OnMessageInstallCancelEvent(WPARAM wParam, LPARAM lParam)
{
	//��ʾ��¼
	m_MissionLogon.ShowLogon();

	return 0L;
}

//�¼���Ϣ
LRESULT CPlatformFrame::OnMessageInsurePlazaEvent(WPARAM wParam, LPARAM lParam)
{
	tagInsurePlazaEvent * pInsureEvent = (tagInsurePlazaEvent*)wParam;
	if(pInsureEvent==NULL) return 0L;

	//��������
	if(pInsureEvent->wCommandID==INSURE_COMMAND_UPDATE)
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//����
		pGlobalUserData->lUserScore = pInsureEvent->lUserScore;
		pGlobalUserData->lUserInsure = pInsureEvent->lUserInsure;

		//������Ϣ
		UpdateUserInfo();
	}
	
	//�һ�����
	if(pInsureEvent->wCommandID==INSURE_COMMAND_EXCHANGE)
	{
		//��ʾ�һ�
		ASSERT(CGlobalUnits::GetInstance());
		// CGlobalUnits::GetInstance()->PerformShowExchange();		
	}

	//�ͷŶ���
	if(pInsureEvent->bHeapData==true) SafeDelete(pInsureEvent);

	return 0;
}


LRESULT CPlatformFrame::OnMessageEnterGame(WPARAM wParam, LPARAM lParam)
{
	WORD wKind = lParam;
	if (wKind==0L) return 0;

	//���·���
	m_PlazaViewGame.UpdateServerItem(wKind);
	m_PlazaViewServer.UpdateServerItem(wKind);

	//���·���
	m_PlazaViewGame.ShowWindow(SW_HIDE);
	m_PlazaViewServer.ShowWindow(SW_SHOW);
	return 0;
}

//�˳���Ϸ
LRESULT CPlatformFrame::OnMessageQuitGame(WPARAM wParam, LPARAM lParam)
{
	//�Ƴ�����
	m_PlazaViewServer.RemoveServerItem();
	
	//���·���
	m_PlazaViewServer.ShowWindow(SW_HIDE);
	m_PlazaViewGame.ShowWindow(SW_SHOW);
	return 0;
}

//���뷿��
LRESULT CPlatformFrame::OnMessageEnterServer(WPARAM wParam, LPARAM lParam)
{
	//������Ϸ
	try
	{
		//������
		m_PlazaViewEntry.Release();

		//������ͼ
		m_PlazaViewEntry.CreateViewFrame();

		//��ʼ��Ϸ
		CGameServerItem * pGameServerItem = m_PlazaViewServer.GetUserChickServerItem();
		if (pGameServerItem==NULL) return 0;
		m_PlazaViewEntry.InitGameServer(pGameServerItem);
		
		//������Ϸ��
		m_PlazaViewEntry.PerformSitDownAction(INVALID_TABLE, INVALID_TABLE, false);
	}
	catch(...)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox( TEXT("��¼��Ϸ����!") );

		return -1;
	}
	
	return 0;
}

//��¼�ɹ�
LRESULT CPlatformFrame::OnMessageLogonSuccess(WPARAM wParam, LPARAM lParam)
{	
	//���·���
	m_PlazaViewServer.ShowWindow(SW_HIDE);
	m_PlazaViewEntry.ShowWindow(SW_SHOW);
	return 0;
}

//�������
LRESULT CPlatformFrame::OnMessageLogonServer(WPARAM wParam, LPARAM lParam)
{
	try
	{
		// �μӷ���
		m_PlazaViewEntry.PerformJoinGame();
	}
	catch(...)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox( TEXT("ִ�з�������ʧ��!") );
		return -1;
	}

	m_PlazaViewEntry.ResetViewFrame(true);

	return 0;
}

//�˳�����
LRESULT CPlatformFrame::OnMessageQuitServer(WPARAM wParam, LPARAM lParam)
{
	m_PlazaViewEntry.Release();

	//���·���
	m_PlazaViewEntry.ShowWindow(SW_HIDE);
	m_PlazaViewServer.ShowWindow(SW_SHOW);
	m_PlazaViewEntry.ResetViewFrame(false);
	return 0;
}

//ɾ������
bool CPlatformFrame::DeleteServerItem()
{
	return true;
}