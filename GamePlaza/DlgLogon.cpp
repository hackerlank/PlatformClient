#include "Stdafx.h"
#include "Resource.h"
#include "DlgLogon.h"
#include "MissionLogon.h"

//////////////////////////////////////////////////////////////////////////////////
//��Ļλ��
#define LAYERED_SIZE				5									//�ֲ��С

//�ؼ���ʶ
#define IDC_WEB_PUBLICIZE			300									//����ؼ�


//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�

const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonMinControlName = TEXT("ButtonMin");
const TCHAR* const szButtonLogonControlName = TEXT("ButtonLogon");
const TCHAR* const szButtonRegisterControlName = TEXT("ButtonRegister");
const TCHAR* const szButtonCleanAccountsControlName = TEXT("ButtonCleanAccounts");
const TCHAR* const szButtonForgetPasswdControlName = TEXT("ButtonForgetPasswd");
const TCHAR* const szButtonWebLink1ControlName = TEXT("ButtonWebLink1");
const TCHAR* const szButtonWebLink2ControlName = TEXT("ButtonWebLink2");
const TCHAR* const szButtonWebLink3ControlName = TEXT("ButtonWebLink3");
const TCHAR* const szButtonWebLink4ControlName = TEXT("ButtonWebLink4");

//��Ͽؼ�
const TCHAR* const szComboServerControlName = TEXT("ComboServer");
const TCHAR* const szComboAccountsControlName = TEXT("ComboAccounts");

//�༭�ؼ�
const TCHAR* const szEditServerControlName = TEXT("EditServer");
const TCHAR* const szEditAccountsControlName = TEXT("EditAccounts");
const TCHAR* const szEditPasswordControlName = TEXT("EditPassword");

//��ǩ�ؼ�
const TCHAR* const szLabelTitleControlName = TEXT("LabelTitle");
const TCHAR* const szLabelLogoControlName = TEXT("LabelLogo");

//��ѡ��ť
const TCHAR* const szCheckButtonPasswdControlName = TEXT("CheckButtonPasswd");
const TCHAR* const szCheckButtonAgreeControlName = TEXT("CheckButtonAgree");

//�ؼ���ʶ
//////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDlgLogon, CFGuiDialog)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_TIMER()

	//�ؼ���Ϣ
	ON_CBN_SELCHANGE(IDC_ACCOUNTS, OnSelchangeAccounts)
	ON_CBN_EDITCHANGE(IDC_ACCOUNTS, OnCbnEditchangeAccounts)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////



//���캯��
CDlgLogon::CDlgLogon() : CFGuiDialog(IDD_DLG_POPUP)
{
	//��¼��Ϣ
	m_dwGameID=0;
	m_szAccounts[0]=0;
	m_szPassword[0]=0;
	m_szLogonServer[0]=0;
	m_szPassPortID[0]=0;

	//���ñ���
	m_cbRemPassword=FALSE;
	m_cbLogonMode=LOGON_BY_ACCOUNTS;
	
	return;
}

//��������
CDlgLogon::~CDlgLogon()
{
	//ɾ������
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		SafeDelete(m_AccountsInfoArray[i]);
	}

	return;
}

//��������
BOOL CDlgLogon::OnInitDialog()
{
	__super::OnInitDialog();

	//���ô���
	ModifyStyle(0, WS_MINIMIZEBOX);
	ModifyStyle(0, WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("�û���¼ -- Version 6603s"));

	//����ͼ��
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)),TRUE);
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE);

	//��������
	CSkinDialog::SetWndFont(this,NULL);
	
	//������Դ
	m_ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_LOGON_BACK"));

	//���ô�С
	CSize SizeWindow(m_ImageBack.GetWidth(),m_ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/LogonLogo.aspx"),pGlobalWebLink->GetPlatformLink());

	//���д���
	CenterWindow(this);

	//������Ϣ
	LoadAccountsInfo();
	LoadLogonServerInfo();

	//���Ҷ���
	CComboUI * pComboAccounts= (CComboUI * )GetControlByName(szComboAccountsControlName);
	if(pComboAccounts!=NULL)
	{
		//���Ҷ���
		CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
		if(pEditPassword==NULL) return FALSE;

		//���ý���
		if (pComboAccounts->GetCurSel()==LB_ERR) pEditPassword->SetFocus();
		else pComboAccounts->SetFocus();

	}
	
	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	CRect rcUnLayered;
	rcUnLayered.top=LAYERED_SIZE;
	rcUnLayered.left=LAYERED_SIZE;
	rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//��������
	CSize rcRoundCornor = GetRoundCornor();

	//��������
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,rcRoundCornor.cx,rcRoundCornor.cy);

	//��������
	SetWindowRgn(RgnWindow,FALSE);


	return TRUE;
}

//��Ϣ����
BOOL CDlgLogon::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//ȷ������
VOID CDlgLogon::OnOK()
{
	//��ȡ��Ϣ
	if (GetInformation()==false) return;

	//���ش���
	ShowWindow(SW_HIDE);

	//ִ�е�¼
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) {
		pMissionLogon->PerformLogonMission(m_cbRemPassword==TRUE);
	}
	return;
}

//ȡ����Ϣ
VOID CDlgLogon::OnCancel()
{
	//�رմ���
	DestroyWindow();
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);

	return;
}

//��ʼ�ؼ�
void CDlgLogon::InitControlUI()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	m_PaintManager.AddFontAt(0,TEXT("����"), 14, false, false, false);
	m_PaintManager.AddFontAt(1,TEXT("����"), 16, false, false, false);

	//////////////////////////////////////////////////////////////////////////////////////////////
	CControlUI * pParent = static_cast<CControlUI *>( m_PaintManager.GetRoot() );
	if(pParent==NULL) return;


	//LOGO����
	CLabelUI * pLabelLogo = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelLogo") );
	if( pLabelLogo != NULL )  {
		pLabelLogo->SetFloat(true);
		pLabelLogo->SetPos(275,60);
		pLabelLogo->SetFixedWidth(187);
		pLabelLogo->SetFixedHeight(67);
		pLabelLogo->SetBkImage( TEXT("file='LOGON_LOGO' restype='PNG'") );
	}

	//ͷ�ı���
	CLabelUI * pLabelTitile = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelTitile") );
	if( pLabelTitile != NULL )  {
		pLabelTitile->SetFloat(true);
		pLabelTitile->SetPos(15,15);
		pLabelTitile->SetFixedWidth(122);
		pLabelTitile->SetFixedHeight(24);
		pLabelTitile->SetBkImage( TEXT("file='LOGON_TITILE' restype='PNG'") );
	}

	//WebLink1
	CButtonUI * pButtonWebLink1 = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonWebLink1") );
	if( pButtonWebLink1 != NULL )  {
		pButtonWebLink1->SetFloat(true);
		pButtonWebLink1->SetPos(20,455);		
		pButtonWebLink1->SetFixedWidth(175);
		pButtonWebLink1->SetFixedHeight(35);
		pButtonWebLink1->SetStatusImage( TEXT("file='BT_LOGON_LINK_1' restype='PNG'") );
	}

	//WebLink2
	CButtonUI * pButtonWebLink2 = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonWebLink2") );
	if( pButtonWebLink2 != NULL )  {
		pButtonWebLink2->SetFloat(true);
		pButtonWebLink2->SetPos(195,455);
		pButtonWebLink2->SetFixedWidth(175);
		pButtonWebLink2->SetFixedHeight(35);
		pButtonWebLink2->SetStatusImage( TEXT("file='BT_LOGON_LINK_2' restype='PNG'") );
	}

	//WebLink3
	CButtonUI * pButtonWebLink3 = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonWebLink3") );
	if( pButtonWebLink3 != NULL )  {
		pButtonWebLink3->SetFloat(true);
		pButtonWebLink3->SetPos(370,455);
		pButtonWebLink3->SetFixedWidth(175);
		pButtonWebLink3->SetFixedHeight(35);
		pButtonWebLink3->SetStatusImage( TEXT("file='BT_LOGON_LINK_3' restype='PNG'") );
	}

	//WebLink4
	CButtonUI * pButtonWebLink4 = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonWebLink4") );
	if( pButtonWebLink4 != NULL )  {
		pButtonWebLink4->SetFloat(true);
		pButtonWebLink4->SetPos(545,455);
		pButtonWebLink4->SetFixedWidth(175);
		pButtonWebLink4->SetFixedHeight(35);
		pButtonWebLink4->SetStatusImage( TEXT("file='BT_LOGON_LINK_4' restype='PNG'") );
	}

	//ButtonMin
	CButtonUI * pButtonMin = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonMin") );
	if( pButtonMin != NULL )  {
		pButtonMin->SetFloat(true);
		pButtonMin->SetPos(662,5);
		pButtonMin->SetFixedWidth(30);
		pButtonMin->SetFixedHeight(35);
		pButtonMin->SetStatusImage( TEXT("file='BT_LOGON_MIN' restype='PNG'") );
	}

	//ButtonClose
	CButtonUI * pButtonClose = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonClose") );
	if( pButtonClose != NULL )  {
		pButtonClose->SetFloat(true);
		pButtonClose->SetPos(698,5);
		pButtonClose->SetFixedWidth(30);
		pButtonClose->SetFixedHeight(35);
		pButtonClose->SetStatusImage( TEXT("file='BT_LOGON_CLOSE' restype='PNG'") );
	}
		
	//LabelAccounts
	CLabelUI * pLabelAccounts = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelAccounts") );
	if( pLabelAccounts != NULL )  {
		pLabelAccounts->SetFloat(true);
		pLabelAccounts->SetPos(100,160);
		pLabelAccounts->SetFixedWidth(63);
		pLabelAccounts->SetFixedHeight(23);
		pLabelAccounts->SetBkImage( TEXT("file='LB_LOGON_USER' restype='PNG'") );
	}	
	
	//LabelPassword
	CLabelUI * pLabelPassword = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelPassword") );
	if( pLabelPassword != NULL )  {
		pLabelPassword->SetFloat(true);
		pLabelPassword->SetPos(100,250);
		pLabelPassword->SetFixedWidth(63);
		pLabelPassword->SetFixedHeight(23);
		pLabelPassword->SetBkImage( TEXT("file='LB_LOGON_PWD' restype='PNG'") );
	}

	//�ʻ���
	CEditUI * pEditAccounts = (CEditUI *) CEditUI::Create( &m_PaintManager, pParent, TEXT("EditAccounts") );
	if( pEditAccounts != NULL )  {
		pEditAccounts->SetFont(1);
		pEditAccounts->SetFloat(true);
		pEditAccounts->SetFixedXY( CPoint(210,145) );
		pEditAccounts->SetFixedWidth(300);
		pEditAccounts->SetFixedHeight(50);
		pEditAccounts->SetBkImage( TEXT("file='LOGON_BLANKET' restype='PNG'") );
		pEditAccounts->SetBkColor(0x00000000);
		pEditAccounts->SetNativeEditBkColor(0xFF750B0B);
		pEditAccounts->SetTextPadding( CRect(4,3,4,3) );
		pEditAccounts->SetTextColor( 0x00F0F0F0 );
		pEditAccounts->SetDisabledTextColor( 0x00000000 );
	}
	
	//�����
	CEditUI * pEditPassword = (CEditUI *) CEditUI::Create( &m_PaintManager, pParent, TEXT("EditPassword") );
	if( pEditPassword != NULL )  {;	
		pEditAccounts->SetFont(0);
		pEditPassword->SetFloat(true);
		pEditPassword->SetFixedXY( CPoint(210,235) );
		pEditPassword->SetFixedWidth(300);
		pEditPassword->SetFixedHeight(50);
		pEditPassword->SetBkImage( TEXT("file='LOGON_BLANKET' restype='PNG'") );
		pEditPassword->SetBkColor(0x00000000);
		pEditPassword->SetNativeEditBkColor(0xFF750B0B);
		pEditPassword->SetTextPadding( CRect(4,3,4,3) );
		pEditPassword->SetTextColor( 0x00F0F0F0 );
		pEditPassword->SetDisabledTextColor( 0x00000000 );
		pEditPassword->SetPasswordMode(true);
		pEditPassword->SetMaxChar(LEN_PASSWORD-1);
	}
	
	//ButtonClose
	CButtonUI * pButtonLogon = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonLogon") );
	if( pButtonLogon != NULL )  {
		pButtonLogon->SetFloat(true);
		pButtonLogon->SetPos(540,145);
		pButtonLogon->SetFixedWidth(150);
		pButtonLogon->SetFixedHeight(40);
		pButtonLogon->SetStatusImage( TEXT("file='BT_ENTER' restype='PNG'") );
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	//��ס����
	CCheckButtonUI * pCheckButtonPasswd = (CCheckButtonUI * ) CCheckButtonUI::Create( &m_PaintManager, pParent, TEXT("CheckButtonPasswd") );
	if( pCheckButtonPasswd!=NULL ) {		
		pCheckButtonPasswd->SetCheck(false);
		pCheckButtonPasswd->SetFixedXY( CPoint(210,295) );
		pCheckButtonPasswd->SetFixedWidth(130);
		pCheckButtonPasswd->SetFixedHeight(30);
		pCheckButtonPasswd->SetText( TEXT(" ��ס����") );
		pCheckButtonPasswd->SetTextColor(0xFFe49843);
		pCheckButtonPasswd->SetTextPadding( CRect(30,1,0,0) );
		pCheckButtonPasswd->SetNormalImage( TEXT("file='CHECK_NORMAL' restype='PNG'") );
		pCheckButtonPasswd->SetCheckedImage( TEXT("file='CHECK_CHECKED' restype='PNG'") );
		pCheckButtonPasswd->SetVisible(true);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	//ͬ������
	CCheckButtonUI * pCheckButtonAgree = (CCheckButtonUI * ) CCheckButtonUI::Create( &m_PaintManager, pParent, TEXT("CheckButtonAgree") );
	if( pCheckButtonAgree!=NULL ) {		
		pCheckButtonAgree->SetCheck(true);
		pCheckButtonAgree->SetFixedXY( CPoint(410,295) );
		pCheckButtonAgree->SetFixedWidth(130);
		pCheckButtonAgree->SetFixedHeight(30);
		pCheckButtonAgree->SetText( TEXT(" ͬ������") );
		pCheckButtonAgree->SetTextColor(0xFFe49843);
		pCheckButtonAgree->SetTextPadding( CRect(30,1,0,0) );
		pCheckButtonAgree->SetNormalImage( TEXT("file='CHECK_NORMAL' restype='PNG'") );
		pCheckButtonAgree->SetCheckedImage( TEXT("file='CHECK_CHECKED' restype='PNG'") );
		pCheckButtonPasswd->SetVisible(true);
	}
}

//��Ϣ����
void CDlgLogon::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0) 
		{
			return OnCancel(); 
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonMinControlName)==0)
		{
			ShowWindow(SW_MINIMIZE);
			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonLogonControlName)==0)
		{
			return OnOK();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonRegisterControlName)==0)
		{
			return OnBnClickedRegister();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonCleanAccountsControlName)==0)
		{
			return OnBnClickedDeleteRecord();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonForgetPasswdControlName)==0)
		{
			return OnBnForgetPassword();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonWebLink1ControlName)==0)
		{
			return OnBnClickedRegister();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonWebLink1ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonWebLink2ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonWebLink3ControlName)==0 ||
				lstrcmp(pControlUI->GetName(), szButtonWebLink4ControlName)==0  )
		{
			//��ȡ����
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//�����ַ
			TCHAR szLogonLink[256]=TEXT("");
			_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink%d.aspx"),pGlobalWebLink->GetPlatformLink(),pControlUI->GetTag());

			//��ҳ��
			ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

			return;
		}
	}
	else if(lstrcmp(msg.sType,TEXT("itemselect")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(), szComboServerControlName)==0)
		{
			//���Ҷ���
			CEditUI * pEditServer = (CEditUI * )GetControlByName(szEditServerControlName);
			if(pEditServer!=NULL) pEditServer->SetText(pControlUI->GetText());

			return;
		}
		else if(lstrcmp(pControlUI->GetName(), szComboAccountsControlName)==0)
		{
			return OnSelchangeAccounts();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szEditAccountsControlName)==0)
		{
			return OnCbnEditchangeAccounts();
		}
	}

	return;
}


//�����滭
void CDlgLogon::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	m_ImageBack.DrawImage(pDC,0,0);
}


//��ʾ��֤
bool CDlgLogon::ShowAccreditWindow()
{
	//��������
	CDlgAccredit DlgAccredit;
	m_pDlgAccedit = &DlgAccredit;
	INT_PTR nEndCode = DlgAccredit.DoModal();
	m_pDlgAccedit=NULL;

	//��������
	if(nEndCode==IDOK)
	{		
		lstrcpyn(m_szPassPortID,DlgAccredit.GetPassPortNumber(),CountArray(m_szPassPortID));
	}

	return nEndCode==IDOK;
}

//��������
WORD CDlgLogon::ConstructLogonPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags)
{
	//��������
	TCHAR szPassword[LEN_MD5];
	CWHEncrypt::MD5Encrypt(m_szPassword,szPassword);

	//��������
	switch (m_cbLogonMode)
	{
	case LOGON_BY_VISITOR:			//�ο͵�¼
		{
			//��������
			CMD_GP_LogonVisitor * pLogonVisitor=(CMD_GP_LogonVisitor *)cbBuffer;
			ZeroMemory(pLogonVisitor,sizeof(CMD_GP_LogonVisitor));

			//ϵͳ��Ϣ
			pLogonVisitor->dwPlazaVersion=VERSION_PLAZA;
			pLogonVisitor->cbValidateFlags=cbValidateFlags;
			CWHService::GetMachineID(pLogonVisitor->szMachineID);

			return sizeof(CMD_GP_LogonVisitor);
		}
	case LOGON_BY_GAME_ID:			//I D ��¼
		{
			//��������
			CMD_GP_LogonGameID * pLogonGameID=(CMD_GP_LogonGameID *)cbBuffer;

			//ϵͳ��Ϣ
			pLogonGameID->dwPlazaVersion=VERSION_PLAZA;

			//������ʶ
			CWHService::GetMachineIDEx(pLogonGameID->szMachineID);

			//��¼��Ϣ
			pLogonGameID->dwGameID=m_dwGameID;
			lstrcpyn(pLogonGameID->szPassword,szPassword,CountArray(pLogonGameID->szPassword));
			pLogonGameID->cbValidateFlags=cbValidateFlags;

			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			lstrcpyn(pGlobalUserData->szPassword,pLogonGameID->szPassword,CountArray(pGlobalUserData->szPassword));

			return sizeof(CMD_GP_LogonGameID);
		}
	case LOGON_BY_ACCOUNTS:			//�ʺŵ�¼
		{
			//��������
			CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)cbBuffer;

			//ϵͳ��Ϣ
			pLogonAccounts->dwPlazaVersion=VERSION_PLAZA;

			//������ʶ
			CWHService::GetMachineIDEx(pLogonAccounts->szMachineID);

			//��¼��Ϣ
			pLogonAccounts->cbValidateFlags=cbValidateFlags;
			lstrcpyn(pLogonAccounts->szPassword,szPassword,CountArray(pLogonAccounts->szPassword));
			lstrcpyn(pLogonAccounts->szAccounts,m_szAccounts,CountArray(pLogonAccounts->szAccounts));
			lstrcpyn(pLogonAccounts->szPassPortID,m_szPassPortID,CountArray(pLogonAccounts->szPassPortID));	

			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			lstrcpyn(pGlobalUserData->szPassword,pLogonAccounts->szPassword,CountArray(pGlobalUserData->szPassword));

			//�����С
			WORD wDataSize = sizeof(CMD_GP_LogonAccounts)-sizeof(pLogonAccounts->szPassPortID);
			wDataSize += CountStringBuffer(pLogonAccounts->szPassPortID);

			return wDataSize;
		}
	}

	return 0;
}

//�����ʺ�
VOID CDlgLogon::LoadAccountsInfo()
{
	//��ȡ��Ϣ
	CWHRegKey RegUserInfo;
	if (RegUserInfo.OpenRegKey(REG_USER_INFO,false)==false) return;

	//��������
	DWORD dwRegIndex=0L;
	DWORD dwLastUserID=RegUserInfo.GetValue(TEXT("LastUserID"),0L);

	//��ȡ��Ϣ
	do
	{
		//��������
		tagAccountsInfo AccountsInfo;
		ZeroMemory(&AccountsInfo,sizeof(AccountsInfo));

		//��ȡ����
		TCHAR szKeyItemName[16]=TEXT("");
		if (RegUserInfo.EnumItemString(dwRegIndex++,szKeyItemName,CountArray(szKeyItemName))==false) break;

		//���Ӽ�
		CWHRegKey RegUserItem;
		if (RegUserItem.Attach(RegUserInfo.OpenItemKey(szKeyItemName))==NULL) continue;

		//�ʻ���Ϣ
		AccountsInfo.dwUserID=_tstol(szKeyItemName);
		RegUserItem.GetString(TEXT("GameID"),AccountsInfo.szGameID,CountArray(AccountsInfo.szGameID));
		RegUserItem.GetString(TEXT("UserAccount"),AccountsInfo.szAccounts,CountArray(AccountsInfo.szAccounts));

		//��ȡ����
		TCHAR szPassword[MAX_ENCRYPT_LEN]=TEXT("");
		RegUserItem.GetString(TEXT("UserPassword"),szPassword,CountArray(szPassword));

		//�⿪����
		if (szPassword[0]!=0)
		{
			CWHEncrypt::XorCrevasse(szPassword,AccountsInfo.szPassword,CountArray(AccountsInfo.szPassword));
		}

		//��������
		tagAccountsInfo * pAccountsInfo=new tagAccountsInfo;
		CopyMemory(pAccountsInfo,&AccountsInfo,sizeof(AccountsInfo));

		//��������
		m_AccountsInfoArray.InsertAt(m_AccountsInfoArray.GetCount(),pAccountsInfo);

	} while (true);

	

	//��������
	if (m_AccountsInfoArray.GetCount()>0) 
	{
		//���Ҷ���
		CEditUI * pEditAccounts = (CEditUI * ) GetControlByName( szEditAccountsControlName );
		if(pEditAccounts==NULL) 
			return;

		pEditAccounts->SetText( m_AccountsInfoArray[0]->szAccounts );

		//���Ҷ���
		CEditUI * pEditPassword = (CEditUI * ) GetControlByName( szEditPasswordControlName );
		if(pEditPassword==NULL) 
			return;

		pEditPassword->SetText( m_AccountsInfoArray[0]->szPassword );
	}
	return;
}

//��ַ��Ϣ
VOID CDlgLogon::LoadLogonServerInfo()
{
	//��ȡ��Ŀ
	CGlobalServer GlobalServer;
	WORD wItemCount=GlobalServer.GetItemCount();

	//�ϴε�¼
	TCHAR szLastServer[LEN_SERVER]=TEXT("");
	GlobalServer.GetLastServerName(szLastServer);

	//���Ҷ���
	CComboUI * pComboServer= (CComboUI * )GetControlByName(szComboServerControlName);
	if(pComboServer==NULL) return;

	//��������
	CListLabelElementUI * pListLabelElement=NULL;

	//��ȡ��Ϣ
	for (WORD i=0;i<wItemCount;i++)
	{
		//��ȡ��Ϣ
		TCHAR szServerItem[LEN_SERVER]=TEXT("");
		GlobalServer.GetServerItemName(i,szServerItem);

		//������Ϣ
		if (szServerItem[0]!=0)
		{
			try
			{
				//��������
				pListLabelElement = new CListLabelElementUI;
				if(pListLabelElement==NULL) throw TEXT("�ڴ治�㣡");

				//��������
				pListLabelElement->SetAttribute(TEXT("text"),szServerItem);
				pListLabelElement->SetAttribute(TEXT("height"),TEXT("23"));
				pListLabelElement->SetManager(&m_PaintManager,pComboServer,true);
				pComboServer->Add(pListLabelElement);
			}
			catch(...)
			{
				ASSERT(FALSE);
				break;
			}

			//�����¼
			if ((szLastServer[0]!=0)&&(lstrcmp(szLastServer,szServerItem)==0))
			{
				szLastServer[0]=0;
				pComboServer->SelectItem(i);
			}
		}
	}

	//�����¼
	if (szLastServer[0]!=0)
	{
		try
		{
			//��������
			pListLabelElement = new CListLabelElementUI;
			if(pListLabelElement==NULL) throw TEXT("�ڴ治�㣡");

			//��������
			pListLabelElement->SetAttribute(TEXT("text"),szLastServer);
			pListLabelElement->SetAttribute(TEXT("height"),TEXT("23"));
			pListLabelElement->SetManager(&m_PaintManager,pComboServer,true);
			pComboServer->Add(pListLabelElement);
			pComboServer->SelectItem(pListLabelElement->GetIndex());
		}
		catch(...)
		{
			ASSERT(FALSE);
		}
	}

	//����ѡ��
	if (pComboServer->GetCurSel()==LB_ERR)
	{
		if (pComboServer->GetCount()==0L)
		{
			try
			{
				//��������
				pListLabelElement = new CListLabelElementUI;
				if(pListLabelElement==NULL) throw TEXT("�ڴ治�㣡");

				//��������
				pListLabelElement->SetAttribute(TEXT("text"),szLogonServer);
				pListLabelElement->SetAttribute(TEXT("height"),TEXT("23"));
				pListLabelElement->SetManager(&m_PaintManager,pComboServer,true);
				pComboServer->Add(pListLabelElement);
				pComboServer->SelectItem(pListLabelElement->GetIndex());
			}
			catch(...)
			{
				ASSERT(FALSE);
			}
		}
		else
		{
			pComboServer->SelectItem(0);
		}
	}
	return;
}

//��ȡ��Ϣ
bool CDlgLogon::GetInformation()
{
	//��ȡ��Ϣ
	m_dwGameID=0;
	m_szPassPortID[0]=0;

	//���Ҷ���
	// CEditUI * pEditServer = (CEditUI * ) GetControlByName(szEditServerControlName);
	// if(pEditServer!=NULL) lstrcpyn(m_szLogonServer,pEditServer->GetText(),CountArray(m_szLogonServer));

	TCHAR szEditServer[32] =  L"������������";
	lstrcpyn(m_szLogonServer, szEditServer, CountArray(szEditServer));

	//���Ҷ���
	CEditUI * pEditAccounts = (CEditUI * )GetControlByName(szEditAccountsControlName);
	if(pEditAccounts!=NULL) lstrcpyn(m_szAccounts,pEditAccounts->GetText(),CountArray(m_szAccounts));

	//���Ҷ���
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword!=NULL) lstrcpyn(m_szPassword,pEditPassword->GetText(),CountArray(m_szPassword));

	//��ַ�ж�
	if (m_szLogonServer[0]==0L)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��ѡ�񡰵�¼�����������ٵ�¼��������������������ָ�ϡ���ȡ������"),MB_ICONERROR,0);

		//��ʾ����
		ShowWindow(SW_SHOW);

		//���ý���
		// pEditServer->SetFocus();

		return false;
	}

	//�ʺ��ж�
	if ((m_cbLogonMode==LOGON_BY_ACCOUNTS)&&(m_szAccounts[0]==0L))
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���������ʺź��ٵ�¼��������������������ָ�ϡ���ȡ������"),MB_ICONERROR,0);

		//��ʾ����
		ShowWindow(SW_SHOW);

		//���ý���
		pEditAccounts->SetFocus();

		return false;
	}

	//�����ж�
	if (m_szPassword[0]==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��������������ٵ�¼��������������������ָ�ϡ���ȡ������"),MB_ICONERROR,0);

		//��ʾ����
		ShowWindow(SW_SHOW);

		//���Ҷ���
		CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
		if(pEditPassword!=NULL) pEditPassword->SetFocus();

		return false;
	}

	//ͬ������
	CCheckButtonUI * pCheckButtonAgree=(CCheckButtonUI *)GetControlByName(szCheckButtonAgreeControlName);;
	if(pCheckButtonAgree!=NULL)
	{
		if(pCheckButtonAgree->GetCheck()==false)
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("��ͬ�����ǵ����"),MB_ICONERROR,0);
			return false;
		}
	}

	//��ס����
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd!=NULL)
	{
		m_cbRemPassword=pCheckButtonPasswd->GetCheck();
	}

	return true;
}

//ѡ��ı�
VOID CDlgLogon::OnSelchangeAccounts()
{
	//��������
	CComboUI * pComboAccounts=(CComboUI *)GetControlByName(szComboAccountsControlName);;
	if(pComboAccounts==NULL) return;

	//���Ҷ���
	CEditUI * pEditAccounts = (CEditUI * )GetControlByName(szEditAccountsControlName);
	if(pEditAccounts!=NULL) pEditAccounts->SetText(pComboAccounts->GetText());	

	//���Ҷ���
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword==NULL) return;

	//���Ҷ���
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd==NULL) return;

	//��ȡѡ��
	INT nCurrentSel=pComboAccounts->GetCurSel();
	if(nCurrentSel==LB_ERR) return;
	tagAccountsInfo * pAccountsInfo=(tagAccountsInfo *)pComboAccounts->GetItemAt(nCurrentSel)->GetUserData();	

	//���ÿؼ�
	if ((nCurrentSel!=LB_ERR)&&(pAccountsInfo!=NULL))
	{
		//��������
		pEditPassword->SetText(pAccountsInfo->szPassword);

		//��ס����
		bool bRemPassword=(pAccountsInfo->szPassword[0]!=0);
		pCheckButtonPasswd->SetCheck(bRemPassword);
	}
	else
	{
		//��������
		pEditPassword->SetText(TEXT(""));

		//��ס����
		pCheckButtonPasswd->SetCheck(false);
	}

	return;
}

//�༭�ı�
VOID CDlgLogon::OnCbnEditchangeAccounts()
{
	//��ȫ����
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd!=NULL) pCheckButtonPasswd->SetCheck(false);

	//��������
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword) pEditPassword->SetText(TEXT(""));

	//��������
	CComboUI * pComboAccounts=(CComboUI *)GetControlByName(szComboAccountsControlName);;
	if(pComboAccounts==NULL) return;

	//��������
	CEditUI * pEditAccounts=(CEditUI *)GetControlByName(szEditAccountsControlName);;
	if(pEditAccounts==NULL) return;

	CStdString strAccounts=pEditAccounts->GetText();

	//������Ϣ
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		//������Ϣ
		INT nGameIDItem=CB_ERR;
		INT nAccountsItem=CB_ERR;
		tagAccountsInfo * pAccountsInfo=m_AccountsInfoArray[i];

		//������Ϣ
		if (pAccountsInfo->szAccounts[0]!=0 && lstrcmp(pAccountsInfo->szAccounts,strAccounts.GetData())==0)
		{
			pComboAccounts->SelectItem(i);
			pEditAccounts->SetText(pComboAccounts->GetText());

			//���ù��
			INT nLength=strAccounts.GetLength();
			pEditAccounts->SetSel(nLength,nLength);

			//��������
			pEditPassword->SetText(pAccountsInfo->szPassword);

			//��ס����
			bool bRemPassword=(pAccountsInfo->szPassword[0]!=0);
			pCheckButtonPasswd->SetCheck(bRemPassword);

			break;
		}
	}

	return;
}

//��������
VOID CDlgLogon::OnBnForgetPassword()
{
	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink5.aspx"),pGlobalWebLink->GetPlatformLink());

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);
}

//ɾ����¼
VOID CDlgLogon::OnBnClickedDeleteRecord()
{
	//���Ҷ���
	CComboUI * pComboAccounts= (CComboUI * )GetControlByName(szComboAccountsControlName);
	if(pComboAccounts==NULL) return;

	//���Ҷ���
	CEditUI * pEditAccounts = (CEditUI * )GetControlByName(szEditAccountsControlName);
	if(pEditAccounts==NULL) return;

	//���Ҷ���
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword==NULL) return;	

	//��ȡ��Ϣ
	tagAccountsInfo * pAccountsInfo=NULL;
	INT nCurrentSel=pComboAccounts->GetCurSel();

	//ɾ������
	if (nCurrentSel!=LB_ERR)
	{
		//��ȡ����
		ASSERT(pComboAccounts->GetItemAt(nCurrentSel)->GetUserData()!=NULL);
		pAccountsInfo=(tagAccountsInfo *)pComboAccounts->GetItemAt(nCurrentSel)->GetUserData();

		//�����ʶ
		CString strUserID;
		strUserID.Format(TEXT("%ld"),pAccountsInfo->dwUserID);

		//�û�����
		CWHRegKey RegUserInfo;
		RegUserInfo.OpenRegKey(REG_USER_INFO,false);

		//ɾ������
		RegUserInfo.RecurseDeleteKey(strUserID);

		//ɾ���б�
		for (INT i=0;i<pComboAccounts->GetCount();i++)
		{
			if (pComboAccounts->GetItemAt(i)->GetUserData()==pAccountsInfo)
			{
				//ɾ���ַ�
				pComboAccounts->SetText(TEXT(""));
				pEditAccounts->SetText(TEXT(""));
				pComboAccounts->RemoveAt(i);				

				//����ѡ��
				if ((pComboAccounts->GetCurSel()==LB_ERR)&&(pComboAccounts->GetCount()>0))
				{
					pComboAccounts->SelectItem(0);
					pEditAccounts->SetText(pComboAccounts->GetText());
				}

				break;
			}
		}
		m_AccountsInfoArray.RemoveAt(nCurrentSel);
	}

	//���ý���
	pComboAccounts->SetFocus();

	//��ȡ����
	nCurrentSel=pComboAccounts->GetCurSel();
	pAccountsInfo=(nCurrentSel!=LB_ERR)?pAccountsInfo=(tagAccountsInfo *)pComboAccounts->GetItemAt(nCurrentSel)->GetUserData():NULL;

	//��������
	pEditPassword->SetText((pAccountsInfo!=NULL)?pAccountsInfo->szPassword:TEXT(""));

	//���Ҷ���
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd==NULL) return;

	//��ס����
	bool bRemPassword=(pAccountsInfo!=NULL)?(pAccountsInfo->szPassword[0]!=0):false;
	pCheckButtonPasswd->SetCheck(bRemPassword);

	return;
}

//ע���ʺ�
VOID CDlgLogon::OnBnClickedRegister()
{
	//���ش���
	ShowWindow(SW_HIDE);

	//��ȡ��ַ
	CString strLogonServer;
	GetDlgItemText(IDC_LOGON_SERVER,strLogonServer);

	//ע���ʺ�
	CControlUI * pControl = GetControlByName(szEditServerControlName);
	if (pControl != NULL) strLogonServer=pControl->GetText();

	//�����ַ
	strLogonServer.TrimLeft();
	strLogonServer.TrimRight();
	lstrcpyn(m_szLogonServer,(strLogonServer.IsEmpty()==true)?szLogonServer:strLogonServer,CountArray(m_szLogonServer));

	//��ʾע��
	ASSERT(CMissionLogon::GetInstance()!=NULL);
	if (CMissionLogon::GetInstance()!=NULL) CMissionLogon::GetInstance()->ShowRegister();

	return;
}

//�ؼ���ɫ
HBRUSH CDlgLogon::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	/*switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_BTN:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(100,100,100));
			return m_brBrush;
		}
	}*/

	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//�ߴ�仯
VOID CDlgLogon::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);	
}

//////////////////////////////////////////////////////////////////////////////////