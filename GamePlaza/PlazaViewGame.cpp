#include "StdAfx.h"
#include "PlazaViewGame.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"

//////////////////////////////////////////////////////////////////////////////////

//��ť�ؼ�
const TCHAR* const szButtonGameBaccaratControlName	= TEXT("ButtonGameBaccarat");
const TCHAR* const szButtonGameFiveCardControlName	= TEXT("ButtonGameFiveCard");
const TCHAR* const szButtonGameLandControlName		= TEXT("ButtonGameLand");
const TCHAR* const szButtonGameOxBattleControlName	= TEXT("ButtonGameOxBattle");
const TCHAR* const szButtonGameOxNewControlName		= TEXT("ButtonGameOxNew");
const TCHAR* const szButtonGameZaJinHuaControlName	= TEXT("ButtonGameZaJinHua");
const TCHAR* const szButtonGameDeZhouControlName	= TEXT("ButtonGameDeZhou");
const TCHAR* const szButtonGameSparrowXZControlName	= TEXT("ButtonGameSparrowXZ");

//////////////////////////////////////////////////////////////////////////////////

//�ؼ�ͼƬ
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

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()

	//�Զ�����Ϣ
	ON_MESSAGE(WM_CLICKED_LEFT, OnClickedLeftPageView)
	ON_MESSAGE(WM_CLICKED_RIGHT, OnClickedRightPageView)

END_MESSAGE_MAP()

//���캯��
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

//��������
CPlazaViewGame::~CPlazaViewGame()
{
	WORD wKey=0;
	
	//ɾ����ť��
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


//���͵��
void CPlazaViewGame::OnViewPageKindItemClicked(tagGameKindInfo * pGameKindInfo)
{
	return;
}

//������
void CPlazaViewGame::OnViewPageServerItemClicked(tagGameServerInfo * pGameServerInfo)
{
	return;
}

//�������
void CPlazaViewGame::OnViewPageMatchItemClicked(tagGameMatchInfo * pGameMatchInfo,WORD wCommandID)
{
	
}


//�������
VOID CPlazaViewGame::OnEventDownLoadTaskFinish()
{
	//������Դ

	/*
	UpdateServerViewItem();

	//��ȡ��ʶ
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	WORD wServerID = CGlobalUnits::GetInstance()->GetTrackServerID();
	if(wServerID != 0)
	{
		//��������
		tagGameServerInfo GameServerInfo;
		ZeroMemory(&GameServerInfo,sizeof(GameServerInfo));

		//����ṹ
		GameServerInfo.wServerID=wServerID;
		OnViewPageServerItemClicked(&GameServerInfo);
	}
	*/
}

//������
VOID CPlazaViewGame::OnEventUpdateCheckFinish(bool bNeedUpdate)
{	
	/*
	if(bNeedUpdate == false)
	{
		//��ȡ��ʶ
		ASSERT(CGlobalUnits::GetInstance()!=NULL);
		WORD wServerID = CGlobalUnits::GetInstance()->GetTrackServerID();
		if(wServerID != 0)
		{
			//��������
			tagGameServerInfo GameServerInfo;
			ZeroMemory(&GameServerInfo,sizeof(GameServerInfo));

			//����ṹ
			GameServerInfo.wServerID=wServerID;
			OnViewPageServerItemClicked(&GameServerInfo);
		}
	}
	*/
}


//��Ϣ����
void CPlazaViewGame::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	if (pPlatformFrame==NULL) return;

	//����¼�
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

		//���Ҷ���
		CGameButtonImage * pGameButtonImage=NULL;
		if ( m_ButtonImageMap.Lookup(wParam, pGameButtonImage)==FALSE ) {
			return;
		}	
	
		//��Ϸ����
		if ( pGameButtonImage->dwVersion == 0L ) {
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox( TEXT("����Ϸδ���ػ����ʧ�ܣ���ѡ��������Ϸ��") );
			return;
		}

		pPlatformFrame->PostMessage( WM_ENTER_GAME, 0, wParam);
	}
	return;
}



//��ʼ�ؼ�
void CPlazaViewGame::InitControlUI()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	m_PaintManager.AddFontAt(0,TEXT("����"), 14, false, false, false);
	m_PaintManager.AddFontAt(1,TEXT("����"), 16, false, false, false);

	//////////////////////////////////////////////////////////////////////////////////////////////
	CControlUI * pParent = static_cast<CControlUI *>( m_PaintManager.GetRoot() );
	if(pParent==NULL) return;

	//�ټ���
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

	//������
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

	//������
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

	//����ţţ
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

	//����ţţ
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

	//թ��
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

	//�����˿� 
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

	//Ѫս�齫
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


//��������
void CPlazaViewGame::SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue)
{
	//��������
	return;
}


//�����滭
void CPlazaViewGame::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	m_ImageBack.DrawImage(pDC,0,0);
}

//��ҳ���
LRESULT CPlazaViewGame::OnClickedLeftPageView( WPARAM wParam, LPARAM lParam )
{
	//״̬У��
	return 0;
}

//��ҳ���
LRESULT CPlazaViewGame::OnClickedRightPageView( WPARAM wParam, LPARAM lParam )
{
	//״̬У��
	return 0;
}


VOID CPlazaViewGame::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
	return;
}

//λ����Ϣ
VOID CPlazaViewGame::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	return;
}

//������Ϣ
INT CPlazaViewGame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	//������Դ
	//CPngImage ImageBack;
	m_ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("CONTENT_BK"));

	//���ô�С
	CSize SizeWindow(m_ImageBack.GetWidth(),m_ImageBack.GetHeight());
	SetWindowPos(NULL, 0, 0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	return 0;
}

// ������Ϸ
VOID CPlazaViewGame::InsertGameItem(WORD wGameID, DWORD dwProcessVersion)
{
	//���Ҷ���
	CGameButtonImage * pGameButtonImage=NULL;
	if ( m_ButtonImageMap.Lookup(wGameID, pGameButtonImage)==FALSE ) {
		return;
	}	
	
	//��Ϸ����
	pGameButtonImage->dwVersion = 0L;
	if ( dwProcessVersion==0L ) {
		return;
	}

	//���̰汾
	pGameButtonImage->dwVersion = dwProcessVersion;

	//������ť
	if ( wGameID==pGameButtonImage->wGameID ) {
		CButtonUI * pButton= (CButtonUI * )GetControlByName(pGameButtonImage->wButtonName);
		if(pButton!=NULL) {
				pButton->SetStatusImage( pGameButtonImage->wButtonImage );
		}
	}
}

// �����б�
VOID CPlazaViewGame::InsertServerItem( CGameServerItem * pGameServerItem )
{
	if ( pGameServerItem==0 )	return;
	m_GameServerArray.Add( pGameServerItem );
}

// ���·���
VOID CPlazaViewGame::UpdateServerItem( WORD wKindID )
{
	//���ָ��
	CPlatformFrame * pPlatformFrame = NULL;
	pPlatformFrame = CPlatformFrame::GetInstance();
	if (pPlatformFrame==NULL) return;
	
	//��ܷ���
	CPlazaViewServer * pPlazaViewServer = NULL;
	pPlazaViewServer = static_cast<CPlazaViewServer *> (&pPlatformFrame->m_PlazaViewServer);
	if (pPlazaViewServer==NULL) return;

	//�����б�
	WORD wCount = m_GameServerArray.GetCount();
	if (wCount==0) return;

	//�����б�
	pPlazaViewServer->RemoveServerItem();

	//�����б�
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