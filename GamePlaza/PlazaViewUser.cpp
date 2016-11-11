#include "Stdafx.h"
#include "PlazaViewUser.h"
#include "PlatformEvent.h"

//////////////////////////////////////////////////////////////////////////////////

//按钮控件
const TCHAR* const szLabelNickNameControl			= TEXT("LabelNickNameR");
const TCHAR* const szLabelGameScoreControl			= TEXT("LabelGameScoreR");
const TCHAR* const szLabelBankScoreControl			= TEXT("LabelBankScoreR");
const TCHAR* const szButtonNickNameControl			= TEXT("ButtonNickNameR");
const TCHAR* const szButtonGameScoreControl			= TEXT("ButtonGameScoreR");
const TCHAR* const szButtonBankScoreControl			= TEXT("ButtonBankScoreR");

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewUser, CFGuiWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()

	//自定消息
	ON_MESSAGE( WM_UPDATE_USERINFO, OnMessageUpdateDate )
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

CPlazaViewUser::CPlazaViewUser()
{

}

//析构函数
CPlazaViewUser::~CPlazaViewUser()
{

}

// 添加逗号
CString CPlazaViewUser::AddComma( LONGLONG lScore , bool bPlus /*= false*/)
{
	CString strScore;
	CString strReturn;
	LONGLONG lNumber = lScore;
	if ( lScore < 0 )
		lNumber = -lNumber;

	strScore.Format(TEXT("%I64d"), lNumber);

	//长度
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

//消息提醒
void CPlazaViewUser::Notify(TNotifyUI &  msg)
{	
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//变量定义
		ASSERT(CParameterGlobal::GetInstance()!=NULL);
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//获取对象
		ASSERT(CGlobalWebLink::GetInstance()!=NULL);
		CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

		//构造地址
		TCHAR szNavigation[256]=TEXT("");
		_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sUserinfo%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(),pControlUI->GetTag());

		//打开页面
		ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
	}
}


void CPlazaViewUser::InitControlUI()
{
	//获取对象

}

VOID CPlazaViewUser::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
	return;
}

//位置消息
VOID CPlazaViewUser::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);
	return;
}

//创建消息
INT CPlazaViewUser::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("PF_USERINFO_BK"));

	//设置大小
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL, 0, 0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	return 0;
}

LRESULT CPlazaViewUser::OnMessageUpdateDate(WPARAM wParam, LPARAM lParam)
{
	//用户变量
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	if( pGlobalUserInfo == NULL) return -1;
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if( pGlobalUserData == NULL) return -1;
	
	//文本串
	TCHAR pUserText[32];
	
	//昵称显示
	CLabelUI * pLabelNickName = (CLabelUI *) GetControlByName( szLabelNickNameControl );
	if( pLabelNickName == NULL) return -1;
	_sntprintf(pUserText,CountArray(pUserText), TEXT(" %s"), pGlobalUserData->szNickName);
	pLabelNickName->SetFont(3);
	pLabelNickName->SetTextColor(ARGB(0xFF,0x00,0x00,0x00));
	pLabelNickName->SetText( pUserText );

	//游戏成绩
	CLabelUI * pLabelGameScore = (CLabelUI *) GetControlByName( szLabelGameScoreControl );
	if( pLabelNickName == NULL) return -1;
	_sntprintf(pUserText ,CountArray(pUserText), TEXT(" %s"), AddComma(pGlobalUserData->lUserScore) );
	pLabelGameScore->SetFont(3);
	pLabelGameScore->SetTextColor(ARGB(0xFF,0xb0,0x02,0x02));
	pLabelGameScore->SetText(pUserText);

	//银行成绩
	CLabelUI * pLabelBankScore = (CLabelUI *) GetControlByName( szLabelBankScoreControl );
	if( pLabelNickName == NULL) return -1;
	_sntprintf(pUserText,CountArray(pUserText), TEXT(" %s"), AddComma(pGlobalUserData->lUserInsure) );
	pLabelBankScore->SetFont(3);
	pLabelBankScore->SetTextColor(ARGB(0xFF,0x8c,0x01,0xaa));
	pLabelBankScore->SetText(pUserText);

	return 0;
}
