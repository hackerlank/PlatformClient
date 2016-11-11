#include "StdAfx.h"
#include "GamePlaza.h"
#include "DlgRegister.h"
#include "MissionLogon.h"

//////////////////////////////////////////////////////////////////////////////////

//屏幕位置
#define LAYERED_SIZE				5									//分层大小

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgRegister, CFGuiDialog)

	//系统消息
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()

	//按钮消息
	ON_BN_CLICKED(IDC_QUIT, OnBnClickedQuit)
	ON_BN_CLICKED(IDC_SELECT_FACE, OnBnClickedSelectFace)

	//特殊消息
	ON_EN_CHANGE(IDC_LOGON_PASS1, OnEnChangeLogonPass)
	ON_STN_CLICKED(IDC_CHANGE_VALIDATE, OnBnClickedChangeValidate)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////
//按钮控件
const TCHAR * szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR * szButtonRegisterControlName = TEXT("ButtonRegister");
const TCHAR * szButtonSelectFaceControlName = TEXT("ButtonSelectFace");

//编辑控件
const TCHAR * szEditAccountsControlName = TEXT("EditAccounts");
const TCHAR * szEditNickNameControlName = TEXT("EditNickName");
const TCHAR * szEditLogonPassControlName = TEXT("EditLogonPass");
const TCHAR * szEditLogonPass1ControlName = TEXT("EditLogonPass1");
const TCHAR * szEditCompellationControlName = TEXT("EditCompellation");
const TCHAR * szEditPassPortIDControlName = TEXT("EditPassPortID");
const TCHAR * szEditSpreaderControlName = TEXT("EditSpreader");
const TCHAR * szEditValidateCodeControlName = TEXT("EditValidateCode");

//标签控件
const TCHAR * szLabelAccountsValidateControlName = TEXT("LabelAccountsValidate");
const TCHAR * szLabelNickNameValidateControlName = TEXT("LabelNickNameValidate");
const TCHAR * szLabelAccountsInvalidControlName = TEXT("LabelAccountsInvalid");
const TCHAR * szLabelNickNameInvalidControlName = TEXT("LabelNickNameInvalid");

//文本控件
const TCHAR * szTextArticleControlName = TEXT("TextArticle");
const TCHAR * szTextRefreshControlName = TEXT("TextRefresh");

//扩展按钮
const TCHAR * szCheckButtonAgreeControlName = TEXT("CheckButtonAgree");
const TCHAR * szRadioButtonMankindeControlName = TEXT("RadioButtonMankind");
const TCHAR * szRadioButtonFemaleControlName = TEXT("RadioButtonFemale");

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgRegister::CDlgRegister() : CFGuiDialog(IDD_DLG_REGISTER)
{
	//验证变量
	m_wVerifyMask=0;
	m_bVerifyIndividual=false;	
	m_bQueryVerifyCode=false;

	//设置变量
	m_wFaceID=0;
	m_cbGender=0;
	m_szAccounts[0]=0;
	m_szNickName[0]=0;
	m_szSpreader[0]=0;
	m_szLogonPass[0]=0;
	m_szInsurePass[0]=0;
	m_szPassPortID[0]=0;
	m_szCompellation[0]=0;
	m_szVerifyCode[0]=0;

	//设置变量
	m_cbRemPassword=FALSE;
	m_cbLogonPassLevel=PASSWORD_LEVEL_0;

	//创建画刷
	m_brBrush.CreateSolidBrush(RGB(215,223,228));

	return;
}

//析构函数
CDlgRegister::~CDlgRegister()
{
}

//控件绑定
VOID CDlgRegister::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//其他控件
	DDX_Control(pDX, IDC_FACE_ITEM, m_FaceItemView);
	DDX_Control(pDX, IDC_VALIDATE_CODE_WND, m_WndValidateCode);
}

//消息解释
BOOL CDlgRegister::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if (pMsg->message==WM_CHAR)
	{
		//真实姓名
		CEditUI * pEditCompellationUI = (CEditUI *)GetControlByName(szEditCompellationControlName);
		if(pEditCompellationUI!=NULL && pEditCompellationUI->GetEditWindow()==pMsg->hwnd)
		{
			//变量定义
			CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
			TCHAR chr=static_cast<TCHAR>(pMsg->wParam);

			//字符判断
			if (pUserItemElement->EfficacyChineseCharacter(chr)==false)
			{
				return TRUE;
			}
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//配置函数
BOOL CDlgRegister::OnInitDialog()
{
	__super::OnInitDialog();

	//设置窗口
	ModifyStyle(0, WS_MINIMIZEBOX);
	ModifyStyle(0, WS_MAXIMIZEBOX);
	m_SkinDPI.Attach(_AtlBaseModule.GetResourceInstance(),m_hWnd,m_lpszTemplateName,96.0);

	//设置标题
	SetWindowText(TEXT("用户注册"));

	//变量定义
	ASSERT(CFaceItemControl::GetInstance()!=NULL);
	IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_REGISTER_BACK"));

	//设置大小
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//验证控件
	//m_WndValidateCode.RandValidateCode();
	//查询验证码
	PerformQueryVerifyCode();

	//获取头像
	m_wFaceID=rand()%pIFaceItemControl->GetFaceCount();

	//设置头像
	/*CRect rcCreate(0,0,0,0);
	m_FaceItemView.Create(NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_OVERLAPPED,rcCreate,this,100);*/
	m_FaceItemView.SetSystemFace(m_wFaceID);

	//居中窗口
	CenterWindow(this);

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
	CSize SizeRoundCornor = GetRoundCornor();

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,SizeRoundCornor.cx,SizeRoundCornor.cy);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);

	//分层窗口
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(SizeRoundCornor.cx,SizeRoundCornor.cy),false);

	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szPath, sizeof(szPath));

	//读取配置
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\Spreader.ini"),szPath);

	//读取推荐人
	TCHAR szSpreader[LEN_ACCOUNTS]=TEXT("");
	GetPrivateProfileString(TEXT("SpreaderInfo"),TEXT("SpreaderName"),TEXT(""),szSpreader,CountArray(szSpreader),szFileName);

	//设置名字
	if (szSpreader[0]!=0)
	{
		//推荐人
		CEditUI * pEditUI = static_cast<CEditUI *>(GetControlByName(szEditSpreaderControlName));
		if(pEditUI != NULL)
		{
			pEditUI->SetText(szSpreader);
			pEditUI->SetReadOnly(true);
		}
	}

	return FALSE;
}

//确定函数
VOID CDlgRegister::OnOK()
{
	//获取信息
	if (GetInformation()==false) return;

	//设置变量
	m_bVerifyIndividual=false;
	m_bQueryVerifyCode=false;

	//隐藏窗口
	ShowWindow(SW_HIDE);

	//执行注册
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(m_cbRemPassword==TRUE);

	return;
}

//取消函数
VOID CDlgRegister::OnCancel()
{
	//隐藏窗口
	ShowWindow(SW_HIDE);

	//显示登录
	ASSERT(CMissionLogon::GetInstance()!=NULL);
	if (CMissionLogon::GetInstance()!=NULL) CMissionLogon::GetInstance()->ShowLogon();
	
	return;
}

//发送登录
WORD CDlgRegister::ConstructRegisterPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags)
{
	if (m_bQueryVerifyCode==true)
	{
		//变量定义
		CMD_GP_QueryVerifyCode * pQueryVerifyCode=(CMD_GP_QueryVerifyCode *)cbBuffer;
		CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_QueryVerifyCode),sizeof(cbBuffer)-sizeof(CMD_GP_QueryVerifyCode));
		ZeroMemory(pQueryVerifyCode,sizeof(CMD_GP_QueryVerifyCode));

		//获取机器码
		CWHService::GetMachineIDEx(pQueryVerifyCode->szMachineID);

		//发送数据
		return sizeof(CMD_GP_QueryVerifyCode)+SendPacket.GetDataSize();		 
	}
	
	//注册数据
	if(m_bVerifyIndividual==false)
	{
		//变量定义
		TCHAR szLogonPass[LEN_MD5];
		TCHAR szInsurePass[LEN_MD5];
		CWHEncrypt::MD5Encrypt(m_szLogonPass,szLogonPass);
		CWHEncrypt::MD5Encrypt(m_szInsurePass,szInsurePass);

		//变量定义
		CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)cbBuffer;

		//系统信息
		pRegisterAccounts->dwPlazaVersion=VERSION_PLAZA;

		//机器标识
		CWHService::GetMachineIDEx(pRegisterAccounts->szMachineID);

		//基本信息
		pRegisterAccounts->wFaceID=m_wFaceID;
		pRegisterAccounts->cbGender=m_cbGender;
		pRegisterAccounts->cbValidateFlags=cbValidateFlags;
		lstrcpyn(pRegisterAccounts->szAccounts,m_szAccounts,CountArray(pRegisterAccounts->szAccounts));
		lstrcpyn(pRegisterAccounts->szNickName,m_szNickName,CountArray(pRegisterAccounts->szNickName));
		lstrcpyn(pRegisterAccounts->szSpreader,m_szSpreader,CountArray(pRegisterAccounts->szSpreader));
		lstrcpyn(pRegisterAccounts->szLogonPass,szLogonPass,CountArray(pRegisterAccounts->szLogonPass));
		lstrcpyn(pRegisterAccounts->szPassPortID,m_szPassPortID,CountArray(pRegisterAccounts->szPassPortID));
		lstrcpyn(pRegisterAccounts->szCompellation,m_szCompellation,CountArray(pRegisterAccounts->szCompellation));
		lstrcpyn(pRegisterAccounts->szVerifyCode,m_szVerifyCode,CountArray(pRegisterAccounts->szVerifyCode));

		//保存密码
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		lstrcpyn(pGlobalUserData->szPassword,pRegisterAccounts->szLogonPass,CountArray(pGlobalUserData->szPassword));

		//发送数据
		return sizeof(CMD_GP_RegisterAccounts);
	}
	else
	{
		//变量定义
		CMD_GP_VerifyIndividual * pVerifyIndividual=(CMD_GP_VerifyIndividual *)cbBuffer;
		CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_VerifyIndividual),sizeof(cbBuffer)-sizeof(CMD_GP_VerifyIndividual));

		//构造结构
		pVerifyIndividual->dwPlazaVersion=VERSION_PLAZA;
		pVerifyIndividual->wVerifyMask=m_wVerifyMask;

		//用户帐号
		if(pVerifyIndividual->wVerifyMask==VERIFY_ACCOUNTS)
		{
			SendPacket.AddPacket(m_szAccounts,DTP_GP_UI_ACCOUNTS);
		}

		//用户昵称
		if(pVerifyIndividual->wVerifyMask==VERIFY_NICKNAME)
		{
			SendPacket.AddPacket(m_szNickName,DTP_GP_UI_NICKNAME);
		}

		//发送数据
		return sizeof(CMD_GP_ModifyIndividual)+SendPacket.GetDataSize();		 
	}
}

//校验结果
VOID CDlgRegister::OnVerifypIndividualResult(WORD wVerifyMask,bool bVerifyPassage,LPCTSTR pszErrorMsg)
{
	if(bVerifyPassage==true)
	{
		//验证帐号
		if(wVerifyMask==VERIFY_ACCOUNTS)
		{
			//隐藏标签
			CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
			if(pLabelAccountInvalid!=NULL) pLabelAccountInvalid->SetVisible(false);

			//验证通过
			CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
			if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(true);

			return;
		}

		//验证昵称
		if(wVerifyMask==VERIFY_NICKNAME)
		{
			//隐藏标签
			CControlUI * pLabelNickNameInvalid = GetControlByName(szLabelNickNameInvalidControlName);
			if(pLabelNickNameInvalid!=NULL) pLabelNickNameInvalid->SetVisible(false);

			//验证通过
			CControlUI * pLabelNickNameValidate = GetControlByName(szLabelNickNameValidateControlName);
			if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(true);

			return;
		}
	}
	else
	{
		//验证帐号
		if(wVerifyMask==VERIFY_ACCOUNTS)
		{
			//验证通过
			CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
			if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(false);

			//帐号无效
			CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
			if(pLabelAccountInvalid!=NULL)
			{
				pLabelAccountInvalid->SetVisible(true);
				pLabelAccountInvalid->SetText(pszErrorMsg);
			}	

			return;
		}

		//验证帐号
		if(wVerifyMask==VERIFY_NICKNAME)
		{
			//验证通过
			CControlUI * pLabelNickNameValidate = GetControlByName(szLabelNickNameValidateControlName);
			if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(false);

			//帐号无效
			CControlUI * pLabelNickNameInvalid = GetControlByName(szLabelNickNameInvalidControlName);
			if(pLabelNickNameInvalid!=NULL)
			{
				pLabelNickNameInvalid->SetVisible(true);
				pLabelNickNameInvalid->SetText(pszErrorMsg);
			}			
		}
	}

	return;
}

//查询验证结果
VOID CDlgRegister::OnQueryVerifyCodeResult(LPCTSTR pszVerifyCode, UINT nMaxCount)
{
	m_WndValidateCode.SetValidateCode(pszVerifyCode,nMaxCount);
}

//消息提醒
 void CDlgRegister::Notify(TNotifyUI &  msg)
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
		else if(lstrcmp(pControlUI->GetName(), szButtonRegisterControlName)==0)
		{
			return OnOK();
		}
		else if(lstrcmp(pControlUI->GetName(), szButtonSelectFaceControlName)==0)
		{
			return OnBnClickedSelectFace();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("link")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szTextArticleControlName)==0)
		{
			return OnBnClickedAgrrArticle();
		}
		else if(lstrcmp(pControlUI->GetName(),szTextRefreshControlName)==0)
		{
			return OnBnClickedChangeValidate();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{	
		if(lstrcmp(pControlUI->GetName(),szEditLogonPassControlName)==0)
		{
			return OnEnChangeLogonPass();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("killfocus")) ==0 )
	{
		if(lstrcmp(pControlUI->GetName(),szEditAccountsControlName)==0)
		{
			return PerformVerifyIndividual(VERIFY_ACCOUNTS);
		}
		else if(lstrcmp(pControlUI->GetName(),szEditNickNameControlName)==0)
		{
			return PerformVerifyIndividual(VERIFY_NICKNAME);
		}
	}
 }

 //初始控件
void CDlgRegister::InitControlUI()
{
	//通过标签
	CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
	if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(false);

	//无效标签
	CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
	if(pLabelAccountInvalid!=NULL) pLabelAccountInvalid->SetVisible(false);

	//验证码
	CEditUI * pEditUI = static_cast<CEditUI *>(GetControlByName(szEditValidateCodeControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(VALIDATE_COUNT);

	//用户帐号
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditAccountsControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_ACCOUNTS-1);

	//用户昵称
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditNickNameControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_NICKNAME-1);

	//推荐人
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditSpreaderControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_ACCOUNTS-1);

	//登录密码
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditLogonPassControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_PASSWORD-1);

	//确认密码
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditLogonPass1ControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_PASSWORD-1);

	//身份证号
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditPassPortIDControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_PASS_PORT_ID-1);

	//真实姓名
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditCompellationControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_COMPELLATION-1);

	//查找控件
	CRadioButtonUI * pRadioButton = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonMankindeControlName));
	if(pRadioButton!=NULL) pRadioButton->SetCheck(true);
}

 //结束绘画
void CDlgRegister::OnEndPaintWindow(HDC hDC)
{
	CDC * pBufferDC = CDC::FromHandle(hDC);

	//加载资源
	CPngImage ImagePassword;
	ImagePassword.LoadImage(AfxGetInstanceHandle(),TEXT("PASSWORD_LEVEL"));

	//变量定义
	INT nImageWidth=ImagePassword.GetWidth();
	INT nImageHeight=ImagePassword.GetHeight()/2;

	//绘画等级
	ImagePassword.DrawImage(pBufferDC,124,273,nImageWidth,nImageHeight,0,0);	

	//绘画叠加
	if (m_cbLogonPassLevel>=PASSWORD_LEVEL_1)
	{
		INT nImageStartPos[] = {0,42,87,nImageWidth};
		INT nImageIndex=(m_cbLogonPassLevel-PASSWORD_LEVEL_1);
		if(nImageIndex<CountArray(nImageStartPos))
		{
			ImagePassword.DrawImage(pBufferDC,124+nImageStartPos[nImageIndex],273,nImageStartPos[nImageIndex+1]-nImageStartPos[nImageIndex],nImageHeight,nImageStartPos[nImageIndex],nImageHeight);
		}
	}
}

//获取信息
bool CDlgRegister::GetInformation()
{
	//密码信息
	TCHAR szLogonPass2[LEN_PASSWORD]=TEXT("");

	//用户帐号
	CControlUI * pEditAccountsUI = GetControlByName(szEditAccountsControlName);
	if(pEditAccountsUI!=NULL) GetControlString(pEditAccountsUI,m_szAccounts,CountArray(m_szAccounts));

	//用户昵称
	CControlUI * pEditNickNameUI = GetControlByName(szEditNickNameControlName);
	if(pEditNickNameUI!=NULL) GetControlString(pEditNickNameUI,m_szNickName,CountArray(m_szNickName));

	//推荐人
	CControlUI * pEditSpreaderUI = GetControlByName(szEditSpreaderControlName);
	if(pEditSpreaderUI!=NULL) GetControlString(pEditSpreaderUI,m_szSpreader,CountArray(m_szSpreader));

	//登录密码
	CControlUI *  pEditLogonPassUI = GetControlByName(szEditLogonPassControlName);
	if(pEditLogonPassUI!=NULL) GetControlString(pEditLogonPassUI,m_szLogonPass,CountArray(m_szLogonPass));

	//确认密码
	CControlUI * pEditLogonPass1UI = GetControlByName(szEditLogonPass1ControlName);
	if(pEditLogonPass1UI!=NULL) GetControlString(pEditLogonPass1UI,szLogonPass2,CountArray(szLogonPass2));

	//身份证号
	CControlUI * pEditPassPortUI = GetControlByName(szEditPassPortIDControlName);
	if(pEditPassPortUI!=NULL) GetControlString(pEditPassPortUI,m_szPassPortID,CountArray(m_szPassPortID));

	//真实姓名
	CControlUI * pEditCompellationUI = GetControlByName(szEditCompellationControlName);
	if(pEditCompellationUI!=NULL) GetControlString(pEditCompellationUI,m_szCompellation,CountArray(m_szCompellation));

	//验证码
	CControlUI * pEditValidateCodeUI = GetControlByName(szEditValidateCodeControlName);
	if(pEditValidateCodeUI!=NULL) GetControlString(pEditValidateCodeUI,m_szVerifyCode,CountArray(m_szVerifyCode));

	//变量定义
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//帐号判断
	if (pUserItemElement->EfficacyAccounts(m_szAccounts,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		pEditAccountsUI->SetFocus();

		return false;
	}

	//昵称判断
	if (pUserItemElement->EfficacyNickName(m_szNickName,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		pEditNickNameUI->SetFocus();

		return false;
	}

	//密码判断
	if (pUserItemElement->EfficacyPassword(m_szLogonPass,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		pEditLogonPassUI->SetFocus();

		return false;
	}

	//密码判断
	if (lstrcmp(m_szLogonPass,szLogonPass2)!=0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您两次输入的帐号密码不一致，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		pEditLogonPass1UI->SetFocus();

		return false;
	}

	//真实姓名
	if (pUserItemElement->EfficacyCompellation(m_szCompellation,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		pEditCompellationUI->SetFocus();

		return false;
	}	

	//身份证号
	if (m_szPassPortID[0]==0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("身份证号不能为空，请认真输入！"),MB_ICONERROR,0);

		//设置焦点
		pEditPassPortUI->SetFocus();

		return false;
	}

	//身份证号
	if (pUserItemElement->EfficacyPassPortID(m_szPassPortID,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		pEditPassPortUI->SetFocus();

		return false;
	}

	//效验判断
	if (lstrlen(m_szVerifyCode)!=LEN_VERIFY_CODE-1)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("验证码输入有误，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		pEditValidateCodeUI->SetFocus();

		return false;
	}

	//同意条款
	CCheckButtonUI * pCheckButtonAgree = static_cast<CCheckButtonUI*>(GetControlByName(szCheckButtonAgreeControlName));
	if(pCheckButtonAgree!=NULL && pCheckButtonAgree->GetCheck()==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请同意并阅读《游戏中心服务条款》！"),MB_ICONERROR,0);

		return false;
	}

	//性别选择
	CRadioButtonUI * pManButtonUI = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonMankindeControlName));
	CRadioButtonUI * pFemaleButtonUI = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonFemaleControlName));

	//用户性别
	if (pManButtonUI && pManButtonUI->GetCheck() ) m_cbGender=GENDER_MANKIND;
	if (pFemaleButtonUI && pFemaleButtonUI->GetCheck()) m_cbGender=GENDER_FEMALE;

	return true;
}

//验证资料
VOID CDlgRegister::PerformVerifyIndividual(WORD wVerifyMask)
{
	//变量定义
	TCHAR szDescribe[128]=TEXT("");

	//验证帐号
	if(wVerifyMask==VERIFY_ACCOUNTS)
	{
		//变量定义
		TCHAR szAccounts[LEN_ACCOUNTS]=TEXT("");	

		//用户帐号
		CControlUI * pEditAccountsUI = GetControlByName(szEditAccountsControlName);
		if(pEditAccountsUI!=NULL) GetControlString(pEditAccountsUI,szAccounts,CountArray(szAccounts));
		if(m_szAccounts[0]!=0 && lstrcmp(m_szAccounts,szAccounts)==0) return;

		//帐号判断
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		if (pUserItemElement->EfficacyAccounts(szAccounts,szDescribe,CountArray(szDescribe))==false)
		{
			//验证通过
			CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
			if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(false);

			//帐号无效
			CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
			if(pLabelAccountInvalid!=NULL)
			{
				pLabelAccountInvalid->SetVisible(true);
				pLabelAccountInvalid->SetText(szDescribe);
			}

			//设置焦点
			pEditAccountsUI->SetFocus();
			lstrcpyn(m_szAccounts,szAccounts,CountArray(m_szAccounts));

			return;
		}

		//设置变量
		m_bQueryVerifyCode=false;
		m_bVerifyIndividual=true;
		m_wVerifyMask=wVerifyMask;
		lstrcpyn(m_szAccounts,szAccounts,CountArray(m_szAccounts));

		//执行注册
		CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
		if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(false);

		return;
	}

	//用户昵称
	if(wVerifyMask==VERIFY_NICKNAME)
	{
		//变量定义
		TCHAR szNickName[LEN_ACCOUNTS]=TEXT("");	

		//用户帐号
		CControlUI * pEditNickNameUI = GetControlByName(szEditNickNameControlName);
		if(pEditNickNameUI!=NULL) GetControlString(pEditNickNameUI,szNickName,CountArray(szNickName));
		if(m_szNickName[0]!=0 && lstrcmp(m_szNickName,szNickName)==0) return;

		//帐号判断
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		if (pUserItemElement->EfficacyNickName(szNickName,szDescribe,CountArray(szDescribe))==false)
		{
			//验证通过
			CControlUI * pLabelNickNameValidate = GetControlByName(szLabelNickNameValidateControlName);
			if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(false);

			//帐号无效
			CControlUI * pLabelNickNameInvalid = GetControlByName(szLabelNickNameInvalidControlName);
			if(pLabelNickNameInvalid!=NULL)
			{
				pLabelNickNameInvalid->SetVisible(true);
				pLabelNickNameInvalid->SetText(szDescribe);
			}

			//设置焦点
			pEditNickNameUI->SetFocus();

			return;
		}

		//设置变量
		m_bQueryVerifyCode=false;
		m_bVerifyIndividual=true;
		m_wVerifyMask=wVerifyMask;
		lstrcpyn(m_szNickName,szNickName,CountArray(m_szNickName));

		//执行注册
		CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
		if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(false);

		return;
	}
}

//查询验证码
VOID CDlgRegister::PerformQueryVerifyCode()
{
	//设置变量
	m_bVerifyIndividual=false;
	m_bQueryVerifyCode=true;

	//执行注册
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(false);

	return;
}

//获取字符
VOID CDlgRegister::GetControlString(CControlUI * pControlUI, TCHAR szString[], WORD wMaxCount)
{
	if(pControlUI==NULL) return;

	//获取字符
	CString strString(pControlUI->GetText());

	//去掉空格
	strString.TrimLeft();
	strString.TrimRight();

	//拷贝字符
	lstrcpyn(szString,strString,wMaxCount);

	return;
}

//关闭按钮
VOID CDlgRegister::OnBnClickedQuit()
{
	//关闭窗口
	OnCancel();

	return;
}

//密码输入
VOID CDlgRegister::OnEnChangeLogonPass()
{
	//获取密码
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");

	//获取密码
	CControlUI * pEditLogonPass=GetControlByName(szEditLogonPassControlName);
	if(pEditLogonPass!=NULL) lstrcpyn(szPassword,pEditLogonPass->GetText(),CountArray(szPassword));

	//等级判断
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	BYTE cbPasswordLevel=pUserItemElement->GetPasswordLevel(szPassword);

	//更新窗口
	if (m_cbLogonPassLevel!=cbPasswordLevel)
	{
		//设置变量
		m_cbLogonPassLevel=cbPasswordLevel;

		//更新窗口
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//头像选择
VOID CDlgRegister::OnBnClickedSelectFace()
{
	//创建组件
	if ((m_FaceSelectControl.GetInterface()==NULL)&&(m_FaceSelectControl.CreateInstance()==false))
	{
		CInformation Information(this);
		Information.ShowMessageBox(m_FaceSelectControl.GetErrorDescribe(),MB_ICONERROR);
		return;
	}

	//隐藏界面
	ShowWindow(SW_HIDE);

	//设置控件
	m_FaceSelectControl->SetAllowItemFull();

	//选择头像
	if (m_FaceSelectControl->GetSelectFaceID(m_wFaceID)==true)
	{
		m_FaceItemView.SetSystemFace(m_wFaceID);
	}

	//显示界面
	ShowWindow(SW_SHOW);

	return;
}

//更换验证
VOID CDlgRegister::OnBnClickedChangeValidate()
{
	////更换验证
	//m_WndValidateCode.RandValidateCode();
	//查询验证码
	PerformQueryVerifyCode();

	return;
}

//同意条款
VOID CDlgRegister::OnBnClickedAgrrArticle()
{
	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szNavigation[256]=TEXT("");
	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/Agreement.aspx"),pGlobalWebLink->GetPlatformLink());

	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
}

//绘画背景
BOOL CDlgRegister::OnEraseBkgnd(CDC * pDC)
{
	////获取位置
	//CRect rcClient;
	//GetClientRect(&rcClient);

	////建立缓冲
	//CImage ImageBuffer;
	//ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	////创建 DC
	//CImageDC BufferDC(ImageBuffer);
	//CDC * pBufferDC=CDC::FromHandle(BufferDC);

	////设置缓冲
	//pBufferDC->SetBkMode(TRANSPARENT);
	//pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	////加载资源
	//CPngImage ImageBack;
	//CPngImage ImageTitle;
	//ImageTitle.LoadImage(AfxGetInstanceHandle(),TEXT("REGISTER_TITLE"));
	//ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_REGISTER_BACK"));

	////加载图标
	//CPngImage ImageLogo;
	//ImageLogo.LoadImage(GetModuleHandle(SKIN_CONTROL_DLL_NAME),TEXT("SKIN_WND_LOGO"));

	////绘画背景
	//ImageBack.DrawImage(pBufferDC,0,0);
	//ImageLogo.DrawImage(pBufferDC,11,6);
	//ImageTitle.DrawImage(pBufferDC,40,6);

	////加载资源
	//CPngImage ImagePassword;
	//ImagePassword.LoadImage(AfxGetInstanceHandle(),TEXT("PASSWORD_LEVEL"));

	////获取大小
	//CSize SizePassword;
	//SizePassword.SetSize(ImagePassword.GetWidth()/3,ImagePassword.GetHeight()/2);

	////绘画等级
	//ImagePassword.DrawImage(pBufferDC,249,258,SizePassword.cx*3,SizePassword.cy,0,0);
	//ImagePassword.DrawImage(pBufferDC,249,311,SizePassword.cx*3,SizePassword.cy,0,0);

	////绘画叠加
	//if (m_cbLogonPassLevel>=PASSWORD_LEVEL_1)
	//{
	//	INT nImagePos=(m_cbLogonPassLevel-PASSWORD_LEVEL_1)*SizePassword.cx;
	//	ImagePassword.DrawImage(pBufferDC,249+nImagePos,258,SizePassword.cx,SizePassword.cy,nImagePos,SizePassword.cy);
	//}

	////绘画叠加
	//if (m_cbInsurePassLevel>=PASSWORD_LEVEL_1)
	//{
	//	INT nImagePos=(m_cbInsurePassLevel-PASSWORD_LEVEL_1)*SizePassword.cx;
	//	ImagePassword.DrawImage(pBufferDC,249+nImagePos,311,SizePassword.cx,SizePassword.cy,nImagePos,SizePassword.cy);
	//}

	////绘画界面
	//pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//控件颜色
HBRUSH CDlgRegister::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_BTN:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(10,10,10));
			return m_brBrush;
		}
	}

	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//////////////////////////////////////////////////////////////////////////////////
