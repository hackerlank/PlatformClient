#include "StdAfx.h"
#include "GamePlaza.h"
#include "DlgRegister.h"
#include "MissionLogon.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ļλ��
#define LAYERED_SIZE				5									//�ֲ��С

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgRegister, CFGuiDialog)

	//ϵͳ��Ϣ
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()

	//��ť��Ϣ
	ON_BN_CLICKED(IDC_QUIT, OnBnClickedQuit)
	ON_BN_CLICKED(IDC_SELECT_FACE, OnBnClickedSelectFace)

	//������Ϣ
	ON_EN_CHANGE(IDC_LOGON_PASS1, OnEnChangeLogonPass)
	ON_STN_CLICKED(IDC_CHANGE_VALIDATE, OnBnClickedChangeValidate)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR * szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR * szButtonRegisterControlName = TEXT("ButtonRegister");
const TCHAR * szButtonSelectFaceControlName = TEXT("ButtonSelectFace");

//�༭�ؼ�
const TCHAR * szEditAccountsControlName = TEXT("EditAccounts");
const TCHAR * szEditNickNameControlName = TEXT("EditNickName");
const TCHAR * szEditLogonPassControlName = TEXT("EditLogonPass");
const TCHAR * szEditLogonPass1ControlName = TEXT("EditLogonPass1");
const TCHAR * szEditCompellationControlName = TEXT("EditCompellation");
const TCHAR * szEditPassPortIDControlName = TEXT("EditPassPortID");
const TCHAR * szEditSpreaderControlName = TEXT("EditSpreader");
const TCHAR * szEditValidateCodeControlName = TEXT("EditValidateCode");

//��ǩ�ؼ�
const TCHAR * szLabelAccountsValidateControlName = TEXT("LabelAccountsValidate");
const TCHAR * szLabelNickNameValidateControlName = TEXT("LabelNickNameValidate");
const TCHAR * szLabelAccountsInvalidControlName = TEXT("LabelAccountsInvalid");
const TCHAR * szLabelNickNameInvalidControlName = TEXT("LabelNickNameInvalid");

//�ı��ؼ�
const TCHAR * szTextArticleControlName = TEXT("TextArticle");
const TCHAR * szTextRefreshControlName = TEXT("TextRefresh");

//��չ��ť
const TCHAR * szCheckButtonAgreeControlName = TEXT("CheckButtonAgree");
const TCHAR * szRadioButtonMankindeControlName = TEXT("RadioButtonMankind");
const TCHAR * szRadioButtonFemaleControlName = TEXT("RadioButtonFemale");

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgRegister::CDlgRegister() : CFGuiDialog(IDD_DLG_REGISTER)
{
	//��֤����
	m_wVerifyMask=0;
	m_bVerifyIndividual=false;	
	m_bQueryVerifyCode=false;

	//���ñ���
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

	//���ñ���
	m_cbRemPassword=FALSE;
	m_cbLogonPassLevel=PASSWORD_LEVEL_0;

	//������ˢ
	m_brBrush.CreateSolidBrush(RGB(215,223,228));

	return;
}

//��������
CDlgRegister::~CDlgRegister()
{
}

//�ؼ���
VOID CDlgRegister::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//�����ؼ�
	DDX_Control(pDX, IDC_FACE_ITEM, m_FaceItemView);
	DDX_Control(pDX, IDC_VALIDATE_CODE_WND, m_WndValidateCode);
}

//��Ϣ����
BOOL CDlgRegister::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if (pMsg->message==WM_CHAR)
	{
		//��ʵ����
		CEditUI * pEditCompellationUI = (CEditUI *)GetControlByName(szEditCompellationControlName);
		if(pEditCompellationUI!=NULL && pEditCompellationUI->GetEditWindow()==pMsg->hwnd)
		{
			//��������
			CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
			TCHAR chr=static_cast<TCHAR>(pMsg->wParam);

			//�ַ��ж�
			if (pUserItemElement->EfficacyChineseCharacter(chr)==false)
			{
				return TRUE;
			}
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//���ú���
BOOL CDlgRegister::OnInitDialog()
{
	__super::OnInitDialog();

	//���ô���
	ModifyStyle(0, WS_MINIMIZEBOX);
	ModifyStyle(0, WS_MAXIMIZEBOX);
	m_SkinDPI.Attach(_AtlBaseModule.GetResourceInstance(),m_hWnd,m_lpszTemplateName,96.0);

	//���ñ���
	SetWindowText(TEXT("�û�ע��"));

	//��������
	ASSERT(CFaceItemControl::GetInstance()!=NULL);
	IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_REGISTER_BACK"));

	//���ô�С
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//��֤�ؼ�
	//m_WndValidateCode.RandValidateCode();
	//��ѯ��֤��
	PerformQueryVerifyCode();

	//��ȡͷ��
	m_wFaceID=rand()%pIFaceItemControl->GetFaceCount();

	//����ͷ��
	/*CRect rcCreate(0,0,0,0);
	m_FaceItemView.Create(NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_OVERLAPPED,rcCreate,this,100);*/
	m_FaceItemView.SetSystemFace(m_wFaceID);

	//���д���
	CenterWindow(this);

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
	CSize SizeRoundCornor = GetRoundCornor();

	//��������
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,SizeRoundCornor.cx,SizeRoundCornor.cy);

	//��������
	SetWindowRgn(RgnWindow,FALSE);

	//�ֲ㴰��
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(SizeRoundCornor.cx,SizeRoundCornor.cy),false);

	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szPath, sizeof(szPath));

	//��ȡ����
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\Spreader.ini"),szPath);

	//��ȡ�Ƽ���
	TCHAR szSpreader[LEN_ACCOUNTS]=TEXT("");
	GetPrivateProfileString(TEXT("SpreaderInfo"),TEXT("SpreaderName"),TEXT(""),szSpreader,CountArray(szSpreader),szFileName);

	//��������
	if (szSpreader[0]!=0)
	{
		//�Ƽ���
		CEditUI * pEditUI = static_cast<CEditUI *>(GetControlByName(szEditSpreaderControlName));
		if(pEditUI != NULL)
		{
			pEditUI->SetText(szSpreader);
			pEditUI->SetReadOnly(true);
		}
	}

	return FALSE;
}

//ȷ������
VOID CDlgRegister::OnOK()
{
	//��ȡ��Ϣ
	if (GetInformation()==false) return;

	//���ñ���
	m_bVerifyIndividual=false;
	m_bQueryVerifyCode=false;

	//���ش���
	ShowWindow(SW_HIDE);

	//ִ��ע��
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(m_cbRemPassword==TRUE);

	return;
}

//ȡ������
VOID CDlgRegister::OnCancel()
{
	//���ش���
	ShowWindow(SW_HIDE);

	//��ʾ��¼
	ASSERT(CMissionLogon::GetInstance()!=NULL);
	if (CMissionLogon::GetInstance()!=NULL) CMissionLogon::GetInstance()->ShowLogon();
	
	return;
}

//���͵�¼
WORD CDlgRegister::ConstructRegisterPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags)
{
	if (m_bQueryVerifyCode==true)
	{
		//��������
		CMD_GP_QueryVerifyCode * pQueryVerifyCode=(CMD_GP_QueryVerifyCode *)cbBuffer;
		CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_QueryVerifyCode),sizeof(cbBuffer)-sizeof(CMD_GP_QueryVerifyCode));
		ZeroMemory(pQueryVerifyCode,sizeof(CMD_GP_QueryVerifyCode));

		//��ȡ������
		CWHService::GetMachineIDEx(pQueryVerifyCode->szMachineID);

		//��������
		return sizeof(CMD_GP_QueryVerifyCode)+SendPacket.GetDataSize();		 
	}
	
	//ע������
	if(m_bVerifyIndividual==false)
	{
		//��������
		TCHAR szLogonPass[LEN_MD5];
		TCHAR szInsurePass[LEN_MD5];
		CWHEncrypt::MD5Encrypt(m_szLogonPass,szLogonPass);
		CWHEncrypt::MD5Encrypt(m_szInsurePass,szInsurePass);

		//��������
		CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)cbBuffer;

		//ϵͳ��Ϣ
		pRegisterAccounts->dwPlazaVersion=VERSION_PLAZA;

		//������ʶ
		CWHService::GetMachineIDEx(pRegisterAccounts->szMachineID);

		//������Ϣ
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

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		lstrcpyn(pGlobalUserData->szPassword,pRegisterAccounts->szLogonPass,CountArray(pGlobalUserData->szPassword));

		//��������
		return sizeof(CMD_GP_RegisterAccounts);
	}
	else
	{
		//��������
		CMD_GP_VerifyIndividual * pVerifyIndividual=(CMD_GP_VerifyIndividual *)cbBuffer;
		CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_VerifyIndividual),sizeof(cbBuffer)-sizeof(CMD_GP_VerifyIndividual));

		//����ṹ
		pVerifyIndividual->dwPlazaVersion=VERSION_PLAZA;
		pVerifyIndividual->wVerifyMask=m_wVerifyMask;

		//�û��ʺ�
		if(pVerifyIndividual->wVerifyMask==VERIFY_ACCOUNTS)
		{
			SendPacket.AddPacket(m_szAccounts,DTP_GP_UI_ACCOUNTS);
		}

		//�û��ǳ�
		if(pVerifyIndividual->wVerifyMask==VERIFY_NICKNAME)
		{
			SendPacket.AddPacket(m_szNickName,DTP_GP_UI_NICKNAME);
		}

		//��������
		return sizeof(CMD_GP_ModifyIndividual)+SendPacket.GetDataSize();		 
	}
}

//У����
VOID CDlgRegister::OnVerifypIndividualResult(WORD wVerifyMask,bool bVerifyPassage,LPCTSTR pszErrorMsg)
{
	if(bVerifyPassage==true)
	{
		//��֤�ʺ�
		if(wVerifyMask==VERIFY_ACCOUNTS)
		{
			//���ر�ǩ
			CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
			if(pLabelAccountInvalid!=NULL) pLabelAccountInvalid->SetVisible(false);

			//��֤ͨ��
			CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
			if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(true);

			return;
		}

		//��֤�ǳ�
		if(wVerifyMask==VERIFY_NICKNAME)
		{
			//���ر�ǩ
			CControlUI * pLabelNickNameInvalid = GetControlByName(szLabelNickNameInvalidControlName);
			if(pLabelNickNameInvalid!=NULL) pLabelNickNameInvalid->SetVisible(false);

			//��֤ͨ��
			CControlUI * pLabelNickNameValidate = GetControlByName(szLabelNickNameValidateControlName);
			if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(true);

			return;
		}
	}
	else
	{
		//��֤�ʺ�
		if(wVerifyMask==VERIFY_ACCOUNTS)
		{
			//��֤ͨ��
			CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
			if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(false);

			//�ʺ���Ч
			CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
			if(pLabelAccountInvalid!=NULL)
			{
				pLabelAccountInvalid->SetVisible(true);
				pLabelAccountInvalid->SetText(pszErrorMsg);
			}	

			return;
		}

		//��֤�ʺ�
		if(wVerifyMask==VERIFY_NICKNAME)
		{
			//��֤ͨ��
			CControlUI * pLabelNickNameValidate = GetControlByName(szLabelNickNameValidateControlName);
			if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(false);

			//�ʺ���Ч
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

//��ѯ��֤���
VOID CDlgRegister::OnQueryVerifyCodeResult(LPCTSTR pszVerifyCode, UINT nMaxCount)
{
	m_WndValidateCode.SetValidateCode(pszVerifyCode,nMaxCount);
}

//��Ϣ����
 void CDlgRegister::Notify(TNotifyUI &  msg)
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

 //��ʼ�ؼ�
void CDlgRegister::InitControlUI()
{
	//ͨ����ǩ
	CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
	if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(false);

	//��Ч��ǩ
	CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
	if(pLabelAccountInvalid!=NULL) pLabelAccountInvalid->SetVisible(false);

	//��֤��
	CEditUI * pEditUI = static_cast<CEditUI *>(GetControlByName(szEditValidateCodeControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(VALIDATE_COUNT);

	//�û��ʺ�
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditAccountsControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_ACCOUNTS-1);

	//�û��ǳ�
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditNickNameControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_NICKNAME-1);

	//�Ƽ���
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditSpreaderControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_ACCOUNTS-1);

	//��¼����
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditLogonPassControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_PASSWORD-1);

	//ȷ������
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditLogonPass1ControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_PASSWORD-1);

	//���֤��
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditPassPortIDControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_PASS_PORT_ID-1);

	//��ʵ����
	pEditUI = static_cast<CEditUI *>(GetControlByName(szEditCompellationControlName));
	if(pEditUI!=NULL) pEditUI->SetMaxChar(LEN_COMPELLATION-1);

	//���ҿؼ�
	CRadioButtonUI * pRadioButton = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonMankindeControlName));
	if(pRadioButton!=NULL) pRadioButton->SetCheck(true);
}

 //�����滭
void CDlgRegister::OnEndPaintWindow(HDC hDC)
{
	CDC * pBufferDC = CDC::FromHandle(hDC);

	//������Դ
	CPngImage ImagePassword;
	ImagePassword.LoadImage(AfxGetInstanceHandle(),TEXT("PASSWORD_LEVEL"));

	//��������
	INT nImageWidth=ImagePassword.GetWidth();
	INT nImageHeight=ImagePassword.GetHeight()/2;

	//�滭�ȼ�
	ImagePassword.DrawImage(pBufferDC,124,273,nImageWidth,nImageHeight,0,0);	

	//�滭����
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

//��ȡ��Ϣ
bool CDlgRegister::GetInformation()
{
	//������Ϣ
	TCHAR szLogonPass2[LEN_PASSWORD]=TEXT("");

	//�û��ʺ�
	CControlUI * pEditAccountsUI = GetControlByName(szEditAccountsControlName);
	if(pEditAccountsUI!=NULL) GetControlString(pEditAccountsUI,m_szAccounts,CountArray(m_szAccounts));

	//�û��ǳ�
	CControlUI * pEditNickNameUI = GetControlByName(szEditNickNameControlName);
	if(pEditNickNameUI!=NULL) GetControlString(pEditNickNameUI,m_szNickName,CountArray(m_szNickName));

	//�Ƽ���
	CControlUI * pEditSpreaderUI = GetControlByName(szEditSpreaderControlName);
	if(pEditSpreaderUI!=NULL) GetControlString(pEditSpreaderUI,m_szSpreader,CountArray(m_szSpreader));

	//��¼����
	CControlUI *  pEditLogonPassUI = GetControlByName(szEditLogonPassControlName);
	if(pEditLogonPassUI!=NULL) GetControlString(pEditLogonPassUI,m_szLogonPass,CountArray(m_szLogonPass));

	//ȷ������
	CControlUI * pEditLogonPass1UI = GetControlByName(szEditLogonPass1ControlName);
	if(pEditLogonPass1UI!=NULL) GetControlString(pEditLogonPass1UI,szLogonPass2,CountArray(szLogonPass2));

	//���֤��
	CControlUI * pEditPassPortUI = GetControlByName(szEditPassPortIDControlName);
	if(pEditPassPortUI!=NULL) GetControlString(pEditPassPortUI,m_szPassPortID,CountArray(m_szPassPortID));

	//��ʵ����
	CControlUI * pEditCompellationUI = GetControlByName(szEditCompellationControlName);
	if(pEditCompellationUI!=NULL) GetControlString(pEditCompellationUI,m_szCompellation,CountArray(m_szCompellation));

	//��֤��
	CControlUI * pEditValidateCodeUI = GetControlByName(szEditValidateCodeControlName);
	if(pEditValidateCodeUI!=NULL) GetControlString(pEditValidateCodeUI,m_szVerifyCode,CountArray(m_szVerifyCode));

	//��������
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//�ʺ��ж�
	if (pUserItemElement->EfficacyAccounts(m_szAccounts,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		pEditAccountsUI->SetFocus();

		return false;
	}

	//�ǳ��ж�
	if (pUserItemElement->EfficacyNickName(m_szNickName,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		pEditNickNameUI->SetFocus();

		return false;
	}

	//�����ж�
	if (pUserItemElement->EfficacyPassword(m_szLogonPass,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		pEditLogonPassUI->SetFocus();

		return false;
	}

	//�����ж�
	if (lstrcmp(m_szLogonPass,szLogonPass2)!=0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("������������ʺ����벻һ�£����������룡"),MB_ICONERROR,0);

		//���ý���
		pEditLogonPass1UI->SetFocus();

		return false;
	}

	//��ʵ����
	if (pUserItemElement->EfficacyCompellation(m_szCompellation,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		pEditCompellationUI->SetFocus();

		return false;
	}	

	//���֤��
	if (m_szPassPortID[0]==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���֤�Ų���Ϊ�գ����������룡"),MB_ICONERROR,0);

		//���ý���
		pEditPassPortUI->SetFocus();

		return false;
	}

	//���֤��
	if (pUserItemElement->EfficacyPassPortID(m_szPassPortID,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		pEditPassPortUI->SetFocus();

		return false;
	}

	//Ч���ж�
	if (lstrlen(m_szVerifyCode)!=LEN_VERIFY_CODE-1)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��֤�������������������룡"),MB_ICONERROR,0);

		//���ý���
		pEditValidateCodeUI->SetFocus();

		return false;
	}

	//ͬ������
	CCheckButtonUI * pCheckButtonAgree = static_cast<CCheckButtonUI*>(GetControlByName(szCheckButtonAgreeControlName));
	if(pCheckButtonAgree!=NULL && pCheckButtonAgree->GetCheck()==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��ͬ�Ⲣ�Ķ�����Ϸ���ķ��������"),MB_ICONERROR,0);

		return false;
	}

	//�Ա�ѡ��
	CRadioButtonUI * pManButtonUI = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonMankindeControlName));
	CRadioButtonUI * pFemaleButtonUI = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonFemaleControlName));

	//�û��Ա�
	if (pManButtonUI && pManButtonUI->GetCheck() ) m_cbGender=GENDER_MANKIND;
	if (pFemaleButtonUI && pFemaleButtonUI->GetCheck()) m_cbGender=GENDER_FEMALE;

	return true;
}

//��֤����
VOID CDlgRegister::PerformVerifyIndividual(WORD wVerifyMask)
{
	//��������
	TCHAR szDescribe[128]=TEXT("");

	//��֤�ʺ�
	if(wVerifyMask==VERIFY_ACCOUNTS)
	{
		//��������
		TCHAR szAccounts[LEN_ACCOUNTS]=TEXT("");	

		//�û��ʺ�
		CControlUI * pEditAccountsUI = GetControlByName(szEditAccountsControlName);
		if(pEditAccountsUI!=NULL) GetControlString(pEditAccountsUI,szAccounts,CountArray(szAccounts));
		if(m_szAccounts[0]!=0 && lstrcmp(m_szAccounts,szAccounts)==0) return;

		//�ʺ��ж�
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		if (pUserItemElement->EfficacyAccounts(szAccounts,szDescribe,CountArray(szDescribe))==false)
		{
			//��֤ͨ��
			CControlUI * pLabelAccountValidate = GetControlByName(szLabelAccountsValidateControlName);
			if(pLabelAccountValidate!=NULL) pLabelAccountValidate->SetVisible(false);

			//�ʺ���Ч
			CControlUI * pLabelAccountInvalid = GetControlByName(szLabelAccountsInvalidControlName);
			if(pLabelAccountInvalid!=NULL)
			{
				pLabelAccountInvalid->SetVisible(true);
				pLabelAccountInvalid->SetText(szDescribe);
			}

			//���ý���
			pEditAccountsUI->SetFocus();
			lstrcpyn(m_szAccounts,szAccounts,CountArray(m_szAccounts));

			return;
		}

		//���ñ���
		m_bQueryVerifyCode=false;
		m_bVerifyIndividual=true;
		m_wVerifyMask=wVerifyMask;
		lstrcpyn(m_szAccounts,szAccounts,CountArray(m_szAccounts));

		//ִ��ע��
		CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
		if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(false);

		return;
	}

	//�û��ǳ�
	if(wVerifyMask==VERIFY_NICKNAME)
	{
		//��������
		TCHAR szNickName[LEN_ACCOUNTS]=TEXT("");	

		//�û��ʺ�
		CControlUI * pEditNickNameUI = GetControlByName(szEditNickNameControlName);
		if(pEditNickNameUI!=NULL) GetControlString(pEditNickNameUI,szNickName,CountArray(szNickName));
		if(m_szNickName[0]!=0 && lstrcmp(m_szNickName,szNickName)==0) return;

		//�ʺ��ж�
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
		if (pUserItemElement->EfficacyNickName(szNickName,szDescribe,CountArray(szDescribe))==false)
		{
			//��֤ͨ��
			CControlUI * pLabelNickNameValidate = GetControlByName(szLabelNickNameValidateControlName);
			if(pLabelNickNameValidate!=NULL) pLabelNickNameValidate->SetVisible(false);

			//�ʺ���Ч
			CControlUI * pLabelNickNameInvalid = GetControlByName(szLabelNickNameInvalidControlName);
			if(pLabelNickNameInvalid!=NULL)
			{
				pLabelNickNameInvalid->SetVisible(true);
				pLabelNickNameInvalid->SetText(szDescribe);
			}

			//���ý���
			pEditNickNameUI->SetFocus();

			return;
		}

		//���ñ���
		m_bQueryVerifyCode=false;
		m_bVerifyIndividual=true;
		m_wVerifyMask=wVerifyMask;
		lstrcpyn(m_szNickName,szNickName,CountArray(m_szNickName));

		//ִ��ע��
		CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
		if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(false);

		return;
	}
}

//��ѯ��֤��
VOID CDlgRegister::PerformQueryVerifyCode()
{
	//���ñ���
	m_bVerifyIndividual=false;
	m_bQueryVerifyCode=true;

	//ִ��ע��
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(false);

	return;
}

//��ȡ�ַ�
VOID CDlgRegister::GetControlString(CControlUI * pControlUI, TCHAR szString[], WORD wMaxCount)
{
	if(pControlUI==NULL) return;

	//��ȡ�ַ�
	CString strString(pControlUI->GetText());

	//ȥ���ո�
	strString.TrimLeft();
	strString.TrimRight();

	//�����ַ�
	lstrcpyn(szString,strString,wMaxCount);

	return;
}

//�رհ�ť
VOID CDlgRegister::OnBnClickedQuit()
{
	//�رմ���
	OnCancel();

	return;
}

//��������
VOID CDlgRegister::OnEnChangeLogonPass()
{
	//��ȡ����
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");

	//��ȡ����
	CControlUI * pEditLogonPass=GetControlByName(szEditLogonPassControlName);
	if(pEditLogonPass!=NULL) lstrcpyn(szPassword,pEditLogonPass->GetText(),CountArray(szPassword));

	//�ȼ��ж�
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	BYTE cbPasswordLevel=pUserItemElement->GetPasswordLevel(szPassword);

	//���´���
	if (m_cbLogonPassLevel!=cbPasswordLevel)
	{
		//���ñ���
		m_cbLogonPassLevel=cbPasswordLevel;

		//���´���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//ͷ��ѡ��
VOID CDlgRegister::OnBnClickedSelectFace()
{
	//�������
	if ((m_FaceSelectControl.GetInterface()==NULL)&&(m_FaceSelectControl.CreateInstance()==false))
	{
		CInformation Information(this);
		Information.ShowMessageBox(m_FaceSelectControl.GetErrorDescribe(),MB_ICONERROR);
		return;
	}

	//���ؽ���
	ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_FaceSelectControl->SetAllowItemFull();

	//ѡ��ͷ��
	if (m_FaceSelectControl->GetSelectFaceID(m_wFaceID)==true)
	{
		m_FaceItemView.SetSystemFace(m_wFaceID);
	}

	//��ʾ����
	ShowWindow(SW_SHOW);

	return;
}

//������֤
VOID CDlgRegister::OnBnClickedChangeValidate()
{
	////������֤
	//m_WndValidateCode.RandValidateCode();
	//��ѯ��֤��
	PerformQueryVerifyCode();

	return;
}

//ͬ������
VOID CDlgRegister::OnBnClickedAgrrArticle()
{
	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szNavigation[256]=TEXT("");
	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/Agreement.aspx"),pGlobalWebLink->GetPlatformLink());

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
}

//�滭����
BOOL CDlgRegister::OnEraseBkgnd(CDC * pDC)
{
	////��ȡλ��
	//CRect rcClient;
	//GetClientRect(&rcClient);

	////��������
	//CImage ImageBuffer;
	//ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	////���� DC
	//CImageDC BufferDC(ImageBuffer);
	//CDC * pBufferDC=CDC::FromHandle(BufferDC);

	////���û���
	//pBufferDC->SetBkMode(TRANSPARENT);
	//pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	////������Դ
	//CPngImage ImageBack;
	//CPngImage ImageTitle;
	//ImageTitle.LoadImage(AfxGetInstanceHandle(),TEXT("REGISTER_TITLE"));
	//ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_REGISTER_BACK"));

	////����ͼ��
	//CPngImage ImageLogo;
	//ImageLogo.LoadImage(GetModuleHandle(SKIN_CONTROL_DLL_NAME),TEXT("SKIN_WND_LOGO"));

	////�滭����
	//ImageBack.DrawImage(pBufferDC,0,0);
	//ImageLogo.DrawImage(pBufferDC,11,6);
	//ImageTitle.DrawImage(pBufferDC,40,6);

	////������Դ
	//CPngImage ImagePassword;
	//ImagePassword.LoadImage(AfxGetInstanceHandle(),TEXT("PASSWORD_LEVEL"));

	////��ȡ��С
	//CSize SizePassword;
	//SizePassword.SetSize(ImagePassword.GetWidth()/3,ImagePassword.GetHeight()/2);

	////�滭�ȼ�
	//ImagePassword.DrawImage(pBufferDC,249,258,SizePassword.cx*3,SizePassword.cy,0,0);
	//ImagePassword.DrawImage(pBufferDC,249,311,SizePassword.cx*3,SizePassword.cy,0,0);

	////�滭����
	//if (m_cbLogonPassLevel>=PASSWORD_LEVEL_1)
	//{
	//	INT nImagePos=(m_cbLogonPassLevel-PASSWORD_LEVEL_1)*SizePassword.cx;
	//	ImagePassword.DrawImage(pBufferDC,249+nImagePos,258,SizePassword.cx,SizePassword.cy,nImagePos,SizePassword.cy);
	//}

	////�滭����
	//if (m_cbInsurePassLevel>=PASSWORD_LEVEL_1)
	//{
	//	INT nImagePos=(m_cbInsurePassLevel-PASSWORD_LEVEL_1)*SizePassword.cx;
	//	ImagePassword.DrawImage(pBufferDC,249+nImagePos,311,SizePassword.cx,SizePassword.cy,nImagePos,SizePassword.cy);
	//}

	////�滭����
	//pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//�ؼ���ɫ
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
