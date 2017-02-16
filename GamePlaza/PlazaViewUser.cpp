#include "Stdafx.h"
#include "PlazaViewUser.h"
#include "PlatformEvent.h"

//////////////////////////////////////////////////////////////////////////////////

//��ť�ؼ�
const TCHAR* const szLabelNickNameControl			= TEXT("LabelNickNameR");
const TCHAR* const szLabelGameScoreControl			= TEXT("LabelGameScoreR");
const TCHAR* const szLabelBankScoreControl			= TEXT("LabelBankScoreR");
const TCHAR* const szButtonNickNameControl			= TEXT("ButtonNickNameR");
const TCHAR* const szButtonGameScoreControl			= TEXT("ButtonGameScoreR");
const TCHAR* const szButtonBankScoreControl			= TEXT("ButtonBankScoreR");

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewUser, CFGuiWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()

	//�Զ���Ϣ
	ON_MESSAGE( WM_UPDATE_USERINFO, OnMessageUpdateDate )
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

CPlazaViewUser::CPlazaViewUser()
{

}

//��������
CPlazaViewUser::~CPlazaViewUser()
{

}

// ��Ӷ���
CString CPlazaViewUser::AddComma( LONGLONG lScore , bool bPlus /*= false*/)
{
	CString strScore;
	CString strReturn;
	LONGLONG lNumber = lScore;
	if ( lScore < 0 )
		lNumber = -lNumber;

	strScore.Format(TEXT("%I64d"), lNumber);

	//����
	int nLength = strScore.GetLength();
	if (nLength==1) {
		strReturn.Insert(0, strScore.GetAt(nLength-1) );
		strReturn.Insert(0, TEXT("0.0") );
	}
	if (nLength==2) 
	{
		strReturn.Insert(0, strScore.GetAt(nLength-1) );
		strReturn.Insert(0, strScore.GetAt(nLength-2) );
		strReturn.Insert(0, TEXT("0.") );
	}
	if(nLength>2)
	{
		strReturn.Insert(0, strScore.GetAt(nLength-1) );
		strReturn.Insert(0, strScore.GetAt(nLength-2) );
		strReturn.Insert(0, TEXT(".") );

		int nStrCount = 0;
		for( int i = strScore.GetLength() - 3; i >= 0; )
		{
			/* if( (nStrCount%3) == 0 && nStrCount != 0 )
			{
				strReturn.Insert(0, ',');
				nStrCount = 0;
			}
			else
			{
				strReturn.Insert(0, strScore.GetAt(i));
				nStrCount++;
				i--;
			}*/

			strReturn.Insert(0, strScore.GetAt(i));
			nStrCount++;
			i--;
		}
	}

	if ( lScore < 0 )
		strReturn.Insert(0, '-');

	if ( bPlus && lScore > 0 )
		strReturn.Insert(0, '+');

	return strReturn;
}

//��Ϣ����
void CPlazaViewUser::Notify(TNotifyUI &  msg)
{	
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//��������
		ASSERT(CParameterGlobal::GetInstance()!=NULL);
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//��ȡ����
		ASSERT(CGlobalWebLink::GetInstance()!=NULL);
		CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

		//�����ַ
		TCHAR szNavigation[256]=TEXT("");
		_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sUserinfo%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(),pControlUI->GetTag());

		//��ҳ��
		ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
	}
}


void CPlazaViewUser::InitControlUI()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	CControlUI * pParent = static_cast<CControlUI *>( m_PaintManager.GetRoot() );
	if(pParent==NULL) return;

	// �ǳƱ�ǩ
	CLabelUI * pLabelNick_L = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelNickNameL") );
	if( pLabelNick_L != NULL )  {
		pLabelNick_L->SetFloat(true);
		pLabelNick_L->SetPos(15,42);
		pLabelNick_L->SetFixedWidth(41);
		pLabelNick_L->SetFixedHeight(17);
		pLabelNick_L->SetBkImage( TEXT("file='LB_USERINFO_USER' restype='PNG'") );
	}

	// ��Ϸ�ɼ�
	CLabelUI * pLabelGame_L = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelGameScoreL") );
	if( pLabelGame_L != NULL )  {
		pLabelGame_L->SetFloat(true);
		pLabelGame_L->SetPos(15,72);
		pLabelGame_L->SetFixedWidth(41);
		pLabelGame_L->SetFixedHeight(17);
		pLabelGame_L->SetBkImage( TEXT("file='LB_USERINFO_MONEY' restype='PNG'") );
	}

	// ��Ϸ����
	CLabelUI * pLabelBank_L = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelBankScoreL") );
	if( pLabelBank_L != NULL )  {
		pLabelBank_L->SetFloat(true);
		pLabelBank_L->SetPos(15,102);
		pLabelBank_L->SetFixedWidth(41);
		pLabelBank_L->SetFixedHeight(17);
		pLabelBank_L->SetBkImage( TEXT("file='LB_USERINFO_BANKER' restype='PNG'") );
	}

	// �ǳƱ�ǩ
	CLabelUI * pLabelNick_R = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelNickNameR") );
	if( pLabelNick_R != NULL )  {
		pLabelNick_R->SetFont(1);
		pLabelNick_R->SetFloat(true);
		pLabelNick_R->SetPos(65,38);
		pLabelNick_R->SetFixedWidth(144);
		pLabelNick_R->SetFixedHeight(26);
		pLabelNick_R->SetTextColor(0xFF000000);
		pLabelNick_R->SetDisabledTextColor(0xFFA7A6AA);
		pLabelNick_R->SetBkImage( TEXT("file='LB_USERINFO_BK' restype='PNG'") );
	}

	// ��Ϸ�ɼ�
	CLabelUI * pLabelGame_R = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelGameScoreR") );
	if( pLabelGame_R != NULL )  {
		pLabelGame_R->SetFont(1);
		pLabelGame_R->SetFloat(true);
		pLabelGame_R->SetPos(65,69);
		pLabelGame_R->SetFixedWidth(144);
		pLabelGame_R->SetFixedHeight(26);
		pLabelGame_R->SetTextColor(0xFFB00202);
		pLabelGame_R->SetDisabledTextColor(0xFFA7A6AA);
		pLabelGame_R->SetBkImage( TEXT("file='LB_USERINFO_BK' restype='PNG'") );
	}

	// ��Ϸ����
	CLabelUI * pLabelBank_R = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelBankScoreR") );
	if( pLabelBank_R != NULL )  {
		pLabelBank_R->SetFont(1);
		pLabelBank_R->SetFloat(true);
		pLabelBank_R->SetPos(65,101);
		pLabelBank_R->SetFixedWidth(144);
		pLabelBank_R->SetFixedHeight(26);
		pLabelBank_R->SetTextColor(0xFF8C01AA);
		pLabelBank_R->SetDisabledTextColor(0xFFA7A6AA);
		pLabelBank_R->SetBkImage( TEXT("file='LB_USERINFO_BK' restype='PNG'") );
	}

	//�û��ɼ�
	CButtonUI * pButtonUserScore = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonUserScore") );
	if( pButtonUserScore != NULL )  {
		pButtonUserScore->SetFloat(true);
		pButtonUserScore->SetPos(15,224);
		pButtonUserScore->SetFixedWidth(202);
		pButtonUserScore->SetFixedHeight(40);
		pButtonUserScore->SetStatusImage( TEXT("file='BT_USER_SCORE' restype='PNG'") );
	}

	//�û�����
	CButtonUI * pButtonUserBanker = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonUserBanker") );
	if( pButtonUserBanker != NULL )  {
		pButtonUserBanker->SetFloat(true);
		pButtonUserBanker->SetPos(15,182);
		pButtonUserBanker->SetFixedWidth(202);
		pButtonUserBanker->SetFixedHeight(40);
		pButtonUserBanker->SetStatusImage( TEXT("file='BT_USER_RECHARGE' restype='PNG'") );
	}
	
	//�û�����
	CButtonUI * pButtonUserCenter = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonUserCenter") );
	if( pButtonUserCenter != NULL )  {
		pButtonUserCenter->SetFloat(true);
		pButtonUserCenter->SetPos(15,141);
		pButtonUserCenter->SetFixedWidth(202);
		pButtonUserCenter->SetFixedHeight(40);
		pButtonUserCenter->SetStatusImage( TEXT("file='BT_USER_CENTER' restype='PNG'") );
	}
}

//�����滭
void CPlazaViewUser::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	m_ImageBack.DrawImage(pDC,0,0);

	//������ʾ
	m_PaintManager.AddFontAt(0,TEXT("����"), 14, false, false, false);
	m_PaintManager.AddFontAt(1,TEXT("����"), 16, false, false, false);
}

VOID CPlazaViewUser::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
	return;
}

//λ����Ϣ
VOID CPlazaViewUser::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);
	return;
}

//������Ϣ
INT CPlazaViewUser::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	//������Դ
	m_ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("PF_USERINFO_BK"));

	//���ô�С
	CSize SizeWindow(m_ImageBack.GetWidth(),m_ImageBack.GetHeight());
	SetWindowPos(NULL, 0, 0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	return 0;
}

LRESULT CPlazaViewUser::OnMessageUpdateDate(WPARAM wParam, LPARAM lParam)
{
	//�û�����
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	if( pGlobalUserInfo == NULL) return -1;
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if( pGlobalUserData == NULL) return -1;
	
	//�ı���
	TCHAR pUserText[32];

	//�ǳ���ʾ
	CLabelUI * pLabelNickName = (CLabelUI *) GetControlByName( szLabelNickNameControl );
	if( pLabelNickName == NULL) return -1;
	_sntprintf(pUserText,CountArray(pUserText), TEXT(" %s"), pGlobalUserData->szNickName);
	pLabelNickName->SetFont(1);
	pLabelNickName->SetTextColor(ARGB(0xFF,0x00,0x00,0x00));
	pLabelNickName->SetText( pUserText );

	//��Ϸ�ɼ�
	CLabelUI * pLabelGameScore = (CLabelUI *) GetControlByName( szLabelGameScoreControl );
	if( pLabelNickName == NULL) return -1;
	_sntprintf(pUserText ,CountArray(pUserText), TEXT(" %s"), AddComma(pGlobalUserData->lUserScore) );
	pLabelGameScore->SetFont(1);
	pLabelGameScore->SetTextColor(ARGB(0xFF,0xb0,0x02,0x02));
	pLabelGameScore->SetText(pUserText);

	//���гɼ�
	CLabelUI * pLabelBankScore = (CLabelUI *) GetControlByName( szLabelBankScoreControl );
	if( pLabelNickName == NULL) return -1;
	_sntprintf(pUserText,CountArray(pUserText), TEXT(" %s"), AddComma(pGlobalUserData->lUserInsure) );
	pLabelBankScore->SetFont(1);
	pLabelBankScore->SetTextColor(ARGB(0xFF,0x8c,0x01,0xaa));
	pLabelBankScore->SetText(pUserText);

	return 0;
}
