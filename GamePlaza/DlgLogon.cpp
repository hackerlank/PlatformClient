#include "Stdafx.h"
#include "Resource.h"
#include "DlgLogon.h"
#include "MissionLogon.h"

//////////////////////////////////////////////////////////////////////////////////
//屏幕位置
#define LAYERED_SIZE				5									//分层大小

//控件标识
#define IDC_WEB_PUBLICIZE			300									//浏览控件


//////////////////////////////////////////////////////////////////////////////////
//按钮控件

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

//组合控件
const TCHAR* const szComboServerControlName = TEXT("ComboServer");
const TCHAR* const szComboAccountsControlName = TEXT("ComboAccounts");

//编辑控件
const TCHAR* const szEditServerControlName = TEXT("EditServer");
const TCHAR* const szEditAccountsControlName = TEXT("EditAccounts");
const TCHAR* const szEditPasswordControlName = TEXT("EditPassword");

//标签控件
const TCHAR* const szLabelTitleControlName = TEXT("LabelTitle");
const TCHAR* const szLabelLogoControlName = TEXT("LabelLogo");

//单选按钮
const TCHAR* const szCheckButtonPasswdControlName = TEXT("CheckButtonPasswd");
const TCHAR* const szCheckButtonAgreeControlName = TEXT("CheckButtonAgree");

//控件标识
//////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDlgLogon, CFGuiDialog)

	//系统消息
	ON_WM_SIZE()
	ON_WM_TIMER()

	//控件消息
	ON_CBN_SELCHANGE(IDC_ACCOUNTS, OnSelchangeAccounts)
	ON_CBN_EDITCHANGE(IDC_ACCOUNTS, OnCbnEditchangeAccounts)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////



//构造函数
CDlgLogon::CDlgLogon() : CFGuiDialog(IDD_DLG_POPUP)
{
	//登录信息
	m_dwGameID=0;
	m_szAccounts[0]=0;
	m_szPassword[0]=0;
	m_szLogonServer[0]=0;
	m_szPassPortID[0]=0;

	//配置变量
	m_cbRemPassword=FALSE;
	m_cbLogonMode=LOGON_BY_ACCOUNTS;
	
	return;
}

//析构函数
CDlgLogon::~CDlgLogon()
{
	//删除对象
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		SafeDelete(m_AccountsInfoArray[i]);
	}

	return;
}

//创建函数
BOOL CDlgLogon::OnInitDialog()
{
	__super::OnInitDialog();

	//设置窗口
	ModifyStyle(0, WS_MINIMIZEBOX);
	ModifyStyle(0, WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("用户登录 -- Version 6603s"));

	//设置图标
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)),TRUE);
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE);

	//设置字体
	CSkinDialog::SetWndFont(this,NULL);
	
	//加载资源
	m_ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_LOGON_BACK"));

	//设置大小
	CSize SizeWindow(m_ImageBack.GetWidth(),m_ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/LogonLogo.aspx"),pGlobalWebLink->GetPlatformLink());

	//居中窗口
	CenterWindow(this);

	//配置信息
	LoadAccountsInfo();
	LoadLogonServerInfo();

	//查找对象
	CComboUI * pComboAccounts= (CComboUI * )GetControlByName(szComboAccountsControlName);
	if(pComboAccounts!=NULL)
	{
		//查找对象
		CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
		if(pEditPassword==NULL) return FALSE;

		//设置焦点
		if (pComboAccounts->GetCurSel()==LB_ERR) pEditPassword->SetFocus();
		else pComboAccounts->SetFocus();

	}
	
	//获取窗口
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	CRect rcUnLayered;
	rcUnLayered.top=LAYERED_SIZE;
	rcUnLayered.left=LAYERED_SIZE;
	rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//变量定义
	CSize rcRoundCornor = GetRoundCornor();

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,rcRoundCornor.cx,rcRoundCornor.cy);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);


	return TRUE;
}

//消息解释
BOOL CDlgLogon::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//确定函数
VOID CDlgLogon::OnOK()
{
	//获取信息
	if (GetInformation()==false) return;

	//隐藏窗口
	ShowWindow(SW_HIDE);

	//执行登录
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) {
		pMissionLogon->PerformLogonMission(m_cbRemPassword==TRUE);
	}
	return;
}

//取消消息
VOID CDlgLogon::OnCancel()
{
	//关闭窗口
	DestroyWindow();
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);

	return;
}

//初始控件
void CDlgLogon::InitControlUI()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	m_PaintManager.AddFontAt(0,TEXT("宋体"), 14, false, false, false);
	m_PaintManager.AddFontAt(1,TEXT("黑体"), 16, false, false, false);

	//////////////////////////////////////////////////////////////////////////////////////////////
	CControlUI * pParent = static_cast<CControlUI *>( m_PaintManager.GetRoot() );
	if(pParent==NULL) return;


	//LOGO对象
	CLabelUI * pLabelLogo = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelLogo") );
	if( pLabelLogo != NULL )  {
		pLabelLogo->SetFloat(true);
		pLabelLogo->SetPos(275,60);
		pLabelLogo->SetFixedWidth(187);
		pLabelLogo->SetFixedHeight(67);
		pLabelLogo->SetBkImage( TEXT("file='LOGON_LOGO' restype='PNG'") );
	}

	//头文标题
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

	//帐户框
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
	
	//密码框
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
	//记住密码
	CCheckButtonUI * pCheckButtonPasswd = (CCheckButtonUI * ) CCheckButtonUI::Create( &m_PaintManager, pParent, TEXT("CheckButtonPasswd") );
	if( pCheckButtonPasswd!=NULL ) {		
		pCheckButtonPasswd->SetCheck(false);
		pCheckButtonPasswd->SetFixedXY( CPoint(210,295) );
		pCheckButtonPasswd->SetFixedWidth(130);
		pCheckButtonPasswd->SetFixedHeight(30);
		pCheckButtonPasswd->SetText( TEXT(" 记住密码") );
		pCheckButtonPasswd->SetTextColor(0xFFe49843);
		pCheckButtonPasswd->SetTextPadding( CRect(30,1,0,0) );
		pCheckButtonPasswd->SetNormalImage( TEXT("file='CHECK_NORMAL' restype='PNG'") );
		pCheckButtonPasswd->SetCheckedImage( TEXT("file='CHECK_CHECKED' restype='PNG'") );
		pCheckButtonPasswd->SetVisible(true);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	//同意条款
	CCheckButtonUI * pCheckButtonAgree = (CCheckButtonUI * ) CCheckButtonUI::Create( &m_PaintManager, pParent, TEXT("CheckButtonAgree") );
	if( pCheckButtonAgree!=NULL ) {		
		pCheckButtonAgree->SetCheck(true);
		pCheckButtonAgree->SetFixedXY( CPoint(410,295) );
		pCheckButtonAgree->SetFixedWidth(130);
		pCheckButtonAgree->SetFixedHeight(30);
		pCheckButtonAgree->SetText( TEXT(" 同意条款") );
		pCheckButtonAgree->SetTextColor(0xFFe49843);
		pCheckButtonAgree->SetTextPadding( CRect(30,1,0,0) );
		pCheckButtonAgree->SetNormalImage( TEXT("file='CHECK_NORMAL' restype='PNG'") );
		pCheckButtonAgree->SetCheckedImage( TEXT("file='CHECK_CHECKED' restype='PNG'") );
		pCheckButtonPasswd->SetVisible(true);
	}
}

//消息提醒
void CDlgLogon::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
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
			//获取对象
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

			//构造地址
			TCHAR szLogonLink[256]=TEXT("");
			_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink%d.aspx"),pGlobalWebLink->GetPlatformLink(),pControlUI->GetTag());

			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

			return;
		}
	}
	else if(lstrcmp(msg.sType,TEXT("itemselect")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(), szComboServerControlName)==0)
		{
			//查找对象
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


//结束绘画
void CDlgLogon::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画背景
	m_ImageBack.DrawImage(pDC,0,0);
}


//显示验证
bool CDlgLogon::ShowAccreditWindow()
{
	//变量定义
	CDlgAccredit DlgAccredit;
	m_pDlgAccedit = &DlgAccredit;
	INT_PTR nEndCode = DlgAccredit.DoModal();
	m_pDlgAccedit=NULL;

	//拷贝号码
	if(nEndCode==IDOK)
	{		
		lstrcpyn(m_szPassPortID,DlgAccredit.GetPassPortNumber(),CountArray(m_szPassPortID));
	}

	return nEndCode==IDOK;
}

//构造数据
WORD CDlgLogon::ConstructLogonPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags)
{
	//变量定义
	TCHAR szPassword[LEN_MD5];
	CWHEncrypt::MD5Encrypt(m_szPassword,szPassword);

	//发送数据
	switch (m_cbLogonMode)
	{
	case LOGON_BY_VISITOR:			//游客登录
		{
			//变量定义
			CMD_GP_LogonVisitor * pLogonVisitor=(CMD_GP_LogonVisitor *)cbBuffer;
			ZeroMemory(pLogonVisitor,sizeof(CMD_GP_LogonVisitor));

			//系统信息
			pLogonVisitor->dwPlazaVersion=VERSION_PLAZA;
			pLogonVisitor->cbValidateFlags=cbValidateFlags;
			CWHService::GetMachineID(pLogonVisitor->szMachineID);

			return sizeof(CMD_GP_LogonVisitor);
		}
	case LOGON_BY_GAME_ID:			//I D 登录
		{
			//变量定义
			CMD_GP_LogonGameID * pLogonGameID=(CMD_GP_LogonGameID *)cbBuffer;

			//系统信息
			pLogonGameID->dwPlazaVersion=VERSION_PLAZA;

			//机器标识
			CWHService::GetMachineIDEx(pLogonGameID->szMachineID);

			//登录信息
			pLogonGameID->dwGameID=m_dwGameID;
			lstrcpyn(pLogonGameID->szPassword,szPassword,CountArray(pLogonGameID->szPassword));
			pLogonGameID->cbValidateFlags=cbValidateFlags;

			//保存密码
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			lstrcpyn(pGlobalUserData->szPassword,pLogonGameID->szPassword,CountArray(pGlobalUserData->szPassword));

			return sizeof(CMD_GP_LogonGameID);
		}
	case LOGON_BY_ACCOUNTS:			//帐号登录
		{
			//变量定义
			CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)cbBuffer;

			//系统信息
			pLogonAccounts->dwPlazaVersion=VERSION_PLAZA;

			//机器标识
			CWHService::GetMachineIDEx(pLogonAccounts->szMachineID);

			//登录信息
			pLogonAccounts->cbValidateFlags=cbValidateFlags;
			lstrcpyn(pLogonAccounts->szPassword,szPassword,CountArray(pLogonAccounts->szPassword));
			lstrcpyn(pLogonAccounts->szAccounts,m_szAccounts,CountArray(pLogonAccounts->szAccounts));
			lstrcpyn(pLogonAccounts->szPassPortID,m_szPassPortID,CountArray(pLogonAccounts->szPassPortID));	

			//保存密码
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			lstrcpyn(pGlobalUserData->szPassword,pLogonAccounts->szPassword,CountArray(pGlobalUserData->szPassword));

			//计算大小
			WORD wDataSize = sizeof(CMD_GP_LogonAccounts)-sizeof(pLogonAccounts->szPassPortID);
			wDataSize += CountStringBuffer(pLogonAccounts->szPassPortID);

			return wDataSize;
		}
	}

	return 0;
}

//加载帐号
VOID CDlgLogon::LoadAccountsInfo()
{
	//读取信息
	CWHRegKey RegUserInfo;
	if (RegUserInfo.OpenRegKey(REG_USER_INFO,false)==false) return;

	//变量定义
	DWORD dwRegIndex=0L;
	DWORD dwLastUserID=RegUserInfo.GetValue(TEXT("LastUserID"),0L);

	//读取信息
	do
	{
		//变量定义
		tagAccountsInfo AccountsInfo;
		ZeroMemory(&AccountsInfo,sizeof(AccountsInfo));

		//读取键项
		TCHAR szKeyItemName[16]=TEXT("");
		if (RegUserInfo.EnumItemString(dwRegIndex++,szKeyItemName,CountArray(szKeyItemName))==false) break;

		//打开子键
		CWHRegKey RegUserItem;
		if (RegUserItem.Attach(RegUserInfo.OpenItemKey(szKeyItemName))==NULL) continue;

		//帐户信息
		AccountsInfo.dwUserID=_tstol(szKeyItemName);
		RegUserItem.GetString(TEXT("GameID"),AccountsInfo.szGameID,CountArray(AccountsInfo.szGameID));
		RegUserItem.GetString(TEXT("UserAccount"),AccountsInfo.szAccounts,CountArray(AccountsInfo.szAccounts));

		//读取密码
		TCHAR szPassword[MAX_ENCRYPT_LEN]=TEXT("");
		RegUserItem.GetString(TEXT("UserPassword"),szPassword,CountArray(szPassword));

		//解开密码
		if (szPassword[0]!=0)
		{
			CWHEncrypt::XorCrevasse(szPassword,AccountsInfo.szPassword,CountArray(AccountsInfo.szPassword));
		}

		//构造数据
		tagAccountsInfo * pAccountsInfo=new tagAccountsInfo;
		CopyMemory(pAccountsInfo,&AccountsInfo,sizeof(AccountsInfo));

		//插入数据
		m_AccountsInfoArray.InsertAt(m_AccountsInfoArray.GetCount(),pAccountsInfo);

	} while (true);

	

	//设置子项
	if (m_AccountsInfoArray.GetCount()>0) 
	{
		//查找对象
		CEditUI * pEditAccounts = (CEditUI * ) GetControlByName( szEditAccountsControlName );
		if(pEditAccounts==NULL) 
			return;

		pEditAccounts->SetText( m_AccountsInfoArray[0]->szAccounts );

		//查找对象
		CEditUI * pEditPassword = (CEditUI * ) GetControlByName( szEditPasswordControlName );
		if(pEditPassword==NULL) 
			return;

		pEditPassword->SetText( m_AccountsInfoArray[0]->szPassword );
	}
	return;
}

//地址信息
VOID CDlgLogon::LoadLogonServerInfo()
{
	//读取数目
	CGlobalServer GlobalServer;
	WORD wItemCount=GlobalServer.GetItemCount();

	//上次登录
	TCHAR szLastServer[LEN_SERVER]=TEXT("");
	GlobalServer.GetLastServerName(szLastServer);

	//查找对象
	CComboUI * pComboServer= (CComboUI * )GetControlByName(szComboServerControlName);
	if(pComboServer==NULL) return;

	//变量定义
	CListLabelElementUI * pListLabelElement=NULL;

	//读取信息
	for (WORD i=0;i<wItemCount;i++)
	{
		//读取信息
		TCHAR szServerItem[LEN_SERVER]=TEXT("");
		GlobalServer.GetServerItemName(i,szServerItem);

		//插入信息
		if (szServerItem[0]!=0)
		{
			try
			{
				//创建对象
				pListLabelElement = new CListLabelElementUI;
				if(pListLabelElement==NULL) throw TEXT("内存不足！");

				//设置子项
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

			//最近登录
			if ((szLastServer[0]!=0)&&(lstrcmp(szLastServer,szServerItem)==0))
			{
				szLastServer[0]=0;
				pComboServer->SelectItem(i);
			}
		}
	}

	//最近登录
	if (szLastServer[0]!=0)
	{
		try
		{
			//创建对象
			pListLabelElement = new CListLabelElementUI;
			if(pListLabelElement==NULL) throw TEXT("内存不足！");

			//设置子项
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

	//设置选择
	if (pComboServer->GetCurSel()==LB_ERR)
	{
		if (pComboServer->GetCount()==0L)
		{
			try
			{
				//创建对象
				pListLabelElement = new CListLabelElementUI;
				if(pListLabelElement==NULL) throw TEXT("内存不足！");

				//设置子项
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

//获取信息
bool CDlgLogon::GetInformation()
{
	//获取信息
	m_dwGameID=0;
	m_szPassPortID[0]=0;

	//查找对象
	// CEditUI * pEditServer = (CEditUI * ) GetControlByName(szEditServerControlName);
	// if(pEditServer!=NULL) lstrcpyn(m_szLogonServer,pEditServer->GetText(),CountArray(m_szLogonServer));

	TCHAR szEditServer[32] =  L"菲娱棋牌娱乐";
	lstrcpyn(m_szLogonServer, szEditServer, CountArray(szEditServer));

	//查找对象
	CEditUI * pEditAccounts = (CEditUI * )GetControlByName(szEditAccountsControlName);
	if(pEditAccounts!=NULL) lstrcpyn(m_szAccounts,pEditAccounts->GetText(),CountArray(m_szAccounts));

	//查找对象
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword!=NULL) lstrcpyn(m_szPassword,pEditPassword->GetText(),CountArray(m_szPassword));

	//地址判断
	if (m_szLogonServer[0]==0L)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请选择“登录服务器”后再登录，若有困难请点击“新手指南”获取帮助！"),MB_ICONERROR,0);

		//显示窗口
		ShowWindow(SW_SHOW);

		//设置焦点
		// pEditServer->SetFocus();

		return false;
	}

	//帐号判断
	if ((m_cbLogonMode==LOGON_BY_ACCOUNTS)&&(m_szAccounts[0]==0L))
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请您输入帐号后再登录，若有困难请点击“新手指南”获取帮助！"),MB_ICONERROR,0);

		//显示窗口
		ShowWindow(SW_SHOW);

		//设置焦点
		pEditAccounts->SetFocus();

		return false;
	}

	//密码判断
	if (m_szPassword[0]==0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请您输入密码后再登录，若有困难请点击“新手指南”获取帮助！"),MB_ICONERROR,0);

		//显示窗口
		ShowWindow(SW_SHOW);

		//查找对象
		CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
		if(pEditPassword!=NULL) pEditPassword->SetFocus();

		return false;
	}

	//同意条款
	CCheckButtonUI * pCheckButtonAgree=(CCheckButtonUI *)GetControlByName(szCheckButtonAgreeControlName);;
	if(pCheckButtonAgree!=NULL)
	{
		if(pCheckButtonAgree->GetCheck()==false)
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("请同意我们的条款！"),MB_ICONERROR,0);
			return false;
		}
	}

	//记住密码
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd!=NULL)
	{
		m_cbRemPassword=pCheckButtonPasswd->GetCheck();
	}

	return true;
}

//选择改变
VOID CDlgLogon::OnSelchangeAccounts()
{
	//变量定义
	CComboUI * pComboAccounts=(CComboUI *)GetControlByName(szComboAccountsControlName);;
	if(pComboAccounts==NULL) return;

	//查找对象
	CEditUI * pEditAccounts = (CEditUI * )GetControlByName(szEditAccountsControlName);
	if(pEditAccounts!=NULL) pEditAccounts->SetText(pComboAccounts->GetText());	

	//查找对象
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword==NULL) return;

	//查找对象
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd==NULL) return;

	//获取选择
	INT nCurrentSel=pComboAccounts->GetCurSel();
	if(nCurrentSel==LB_ERR) return;
	tagAccountsInfo * pAccountsInfo=(tagAccountsInfo *)pComboAccounts->GetItemAt(nCurrentSel)->GetUserData();	

	//设置控件
	if ((nCurrentSel!=LB_ERR)&&(pAccountsInfo!=NULL))
	{
		//设置密码
		pEditPassword->SetText(pAccountsInfo->szPassword);

		//记住密码
		bool bRemPassword=(pAccountsInfo->szPassword[0]!=0);
		pCheckButtonPasswd->SetCheck(bRemPassword);
	}
	else
	{
		//设置密码
		pEditPassword->SetText(TEXT(""));

		//记住密码
		pCheckButtonPasswd->SetCheck(false);
	}

	return;
}

//编辑改变
VOID CDlgLogon::OnCbnEditchangeAccounts()
{
	//安全设置
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd!=NULL) pCheckButtonPasswd->SetCheck(false);

	//密码重置
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword) pEditPassword->SetText(TEXT(""));

	//变量定义
	CComboUI * pComboAccounts=(CComboUI *)GetControlByName(szComboAccountsControlName);;
	if(pComboAccounts==NULL) return;

	//变量定义
	CEditUI * pEditAccounts=(CEditUI *)GetControlByName(szEditAccountsControlName);;
	if(pEditAccounts==NULL) return;

	CStdString strAccounts=pEditAccounts->GetText();

	//插入信息
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		//插入信息
		INT nGameIDItem=CB_ERR;
		INT nAccountsItem=CB_ERR;
		tagAccountsInfo * pAccountsInfo=m_AccountsInfoArray[i];

		//插入信息
		if (pAccountsInfo->szAccounts[0]!=0 && lstrcmp(pAccountsInfo->szAccounts,strAccounts.GetData())==0)
		{
			pComboAccounts->SelectItem(i);
			pEditAccounts->SetText(pComboAccounts->GetText());

			//设置光标
			INT nLength=strAccounts.GetLength();
			pEditAccounts->SetSel(nLength,nLength);

			//设置密码
			pEditPassword->SetText(pAccountsInfo->szPassword);

			//记住密码
			bool bRemPassword=(pAccountsInfo->szPassword[0]!=0);
			pCheckButtonPasswd->SetCheck(bRemPassword);

			break;
		}
	}

	return;
}

//忘记密码
VOID CDlgLogon::OnBnForgetPassword()
{
	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink5.aspx"),pGlobalWebLink->GetPlatformLink());

	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);
}

//删除记录
VOID CDlgLogon::OnBnClickedDeleteRecord()
{
	//查找对象
	CComboUI * pComboAccounts= (CComboUI * )GetControlByName(szComboAccountsControlName);
	if(pComboAccounts==NULL) return;

	//查找对象
	CEditUI * pEditAccounts = (CEditUI * )GetControlByName(szEditAccountsControlName);
	if(pEditAccounts==NULL) return;

	//查找对象
	CEditUI * pEditPassword = (CEditUI * )GetControlByName(szEditPasswordControlName);
	if(pEditPassword==NULL) return;	

	//获取信息
	tagAccountsInfo * pAccountsInfo=NULL;
	INT nCurrentSel=pComboAccounts->GetCurSel();

	//删除处理
	if (nCurrentSel!=LB_ERR)
	{
		//获取变量
		ASSERT(pComboAccounts->GetItemAt(nCurrentSel)->GetUserData()!=NULL);
		pAccountsInfo=(tagAccountsInfo *)pComboAccounts->GetItemAt(nCurrentSel)->GetUserData();

		//构造标识
		CString strUserID;
		strUserID.Format(TEXT("%ld"),pAccountsInfo->dwUserID);

		//用户表项
		CWHRegKey RegUserInfo;
		RegUserInfo.OpenRegKey(REG_USER_INFO,false);

		//删除表项
		RegUserInfo.RecurseDeleteKey(strUserID);

		//删除列表
		for (INT i=0;i<pComboAccounts->GetCount();i++)
		{
			if (pComboAccounts->GetItemAt(i)->GetUserData()==pAccountsInfo)
			{
				//删除字符
				pComboAccounts->SetText(TEXT(""));
				pEditAccounts->SetText(TEXT(""));
				pComboAccounts->RemoveAt(i);				

				//更新选择
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

	//设置焦点
	pComboAccounts->SetFocus();

	//获取变量
	nCurrentSel=pComboAccounts->GetCurSel();
	pAccountsInfo=(nCurrentSel!=LB_ERR)?pAccountsInfo=(tagAccountsInfo *)pComboAccounts->GetItemAt(nCurrentSel)->GetUserData():NULL;

	//设置密码
	pEditPassword->SetText((pAccountsInfo!=NULL)?pAccountsInfo->szPassword:TEXT(""));

	//查找对象
	CCheckButtonUI * pCheckButtonPasswd=(CCheckButtonUI *)GetControlByName(szCheckButtonPasswdControlName);;
	if(pCheckButtonPasswd==NULL) return;

	//记住密码
	bool bRemPassword=(pAccountsInfo!=NULL)?(pAccountsInfo->szPassword[0]!=0):false;
	pCheckButtonPasswd->SetCheck(bRemPassword);

	return;
}

//注册帐号
VOID CDlgLogon::OnBnClickedRegister()
{
	//隐藏窗口
	ShowWindow(SW_HIDE);

	//获取地址
	CString strLogonServer;
	GetDlgItemText(IDC_LOGON_SERVER,strLogonServer);

	//注册帐号
	CControlUI * pControl = GetControlByName(szEditServerControlName);
	if (pControl != NULL) strLogonServer=pControl->GetText();

	//构造地址
	strLogonServer.TrimLeft();
	strLogonServer.TrimRight();
	lstrcpyn(m_szLogonServer,(strLogonServer.IsEmpty()==true)?szLogonServer:strLogonServer,CountArray(m_szLogonServer));

	//显示注册
	ASSERT(CMissionLogon::GetInstance()!=NULL);
	if (CMissionLogon::GetInstance()!=NULL) CMissionLogon::GetInstance()->ShowRegister();

	return;
}

//控件颜色
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

//尺寸变化
VOID CDlgLogon::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);	
}

//////////////////////////////////////////////////////////////////////////////////