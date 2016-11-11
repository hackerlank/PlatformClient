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
	if (nLength==0) 
	{
		strReturn.Insert(0, TEXT("0.00") );
	}
	else if (nLength==1) {
		strReturn.Insert(0, TEXT("0.0") );
	}
	if (nLength==2) 
	{
		strReturn.Insert(0, strScore.GetAt(nLength-1) );
		strReturn.Insert(0, strScore.GetAt(nLength-2) );
		strReturn.Insert(0, TEXT("0.") );
	}
	else if(nLength>2)
	{
		strReturn.Insert(0, strScore.GetAt(nLength-1) );
		strReturn.Insert(0, strScore.GetAt(nLength-2) );
		strReturn.Insert(0, TEXT(".") );

		int nStrCount = 0;
		for( int i = strScore.GetLength() - 3; i >= 0; )
		{
			if( (nStrCount%3) == 0 && nStrCount != 0 )
			{
				strReturn.Insert(0, ',');
				nStrCount = 0;
			}
			else
			{
				strReturn.Insert(0, strScore.GetAt(i));
				nStrCount++;
				i--;
			}
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
	//��ȡ����

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
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("PF_USERINFO_BK"));

	//���ô�С
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
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
	pLabelNickName->SetFont(3);
	pLabelNickName->SetTextColor(ARGB(0xFF,0x00,0x00,0x00));
	pLabelNickName->SetText( pUserText );

	//��Ϸ�ɼ�
	CLabelUI * pLabelGameScore = (CLabelUI *) GetControlByName( szLabelGameScoreControl );
	if( pLabelNickName == NULL) return -1;
	_sntprintf(pUserText ,CountArray(pUserText), TEXT(" %s"), AddComma(pGlobalUserData->lUserScore) );
	pLabelGameScore->SetFont(3);
	pLabelGameScore->SetTextColor(ARGB(0xFF,0xb0,0x02,0x02));
	pLabelGameScore->SetText(pUserText);

	//���гɼ�
	CLabelUI * pLabelBankScore = (CLabelUI *) GetControlByName( szLabelBankScoreControl );
	if( pLabelNickName == NULL) return -1;
	_sntprintf(pUserText,CountArray(pUserText), TEXT(" %s"), AddComma(pGlobalUserData->lUserInsure) );
	pLabelBankScore->SetFont(3);
	pLabelBankScore->SetTextColor(ARGB(0xFF,0x8c,0x01,0xaa));
	pLabelBankScore->SetText(pUserText);

	return 0;
}
