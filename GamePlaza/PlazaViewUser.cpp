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
	//////////////////////////////////////////////////////////////////////////////////////////////
	CControlUI * pParent = static_cast<CControlUI *>( m_PaintManager.GetRoot() );
	if(pParent==NULL) return;

	// 昵称标签
	CLabelUI * pLabelNick_L = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelNickNameL") );
	if( pLabelNick_L != NULL )  {
		pLabelNick_L->SetFloat(true);
		pLabelNick_L->SetPos(15,42);
		pLabelNick_L->SetFixedWidth(41);
		pLabelNick_L->SetFixedHeight(17);
		pLabelNick_L->SetBkImage( TEXT("file='LB_USERINFO_USER' restype='PNG'") );
	}

	// 游戏成绩
	CLabelUI * pLabelGame_L = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelGameScoreL") );
	if( pLabelGame_L != NULL )  {
		pLabelGame_L->SetFloat(true);
		pLabelGame_L->SetPos(15,72);
		pLabelGame_L->SetFixedWidth(41);
		pLabelGame_L->SetFixedHeight(17);
		pLabelGame_L->SetBkImage( TEXT("file='LB_USERINFO_MONEY' restype='PNG'") );
	}

	// 游戏银行
	CLabelUI * pLabelBank_L = (CLabelUI *) CLabelUI::Create( &m_PaintManager, pParent, TEXT("LabelBankScoreL") );
	if( pLabelBank_L != NULL )  {
		pLabelBank_L->SetFloat(true);
		pLabelBank_L->SetPos(15,102);
		pLabelBank_L->SetFixedWidth(41);
		pLabelBank_L->SetFixedHeight(17);
		pLabelBank_L->SetBkImage( TEXT("file='LB_USERINFO_BANKER' restype='PNG'") );
	}

	// 昵称标签
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

	// 游戏成绩
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

	// 游戏银行
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

	//用户成绩
	CButtonUI * pButtonUserScore = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonUserScore") );
	if( pButtonUserScore != NULL )  {
		pButtonUserScore->SetFloat(true);
		pButtonUserScore->SetPos(15,224);
		pButtonUserScore->SetFixedWidth(202);
		pButtonUserScore->SetFixedHeight(40);
		pButtonUserScore->SetStatusImage( TEXT("file='BT_USER_SCORE' restype='PNG'") );
	}

	//用户银行
	CButtonUI * pButtonUserBanker = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonUserBanker") );
	if( pButtonUserBanker != NULL )  {
		pButtonUserBanker->SetFloat(true);
		pButtonUserBanker->SetPos(15,182);
		pButtonUserBanker->SetFixedWidth(202);
		pButtonUserBanker->SetFixedHeight(40);
		pButtonUserBanker->SetStatusImage( TEXT("file='BT_USER_RECHARGE' restype='PNG'") );
	}
	
	//用户中心
	CButtonUI * pButtonUserCenter = (CButtonUI *) CButtonUI::Create( &m_PaintManager, pParent, TEXT("ButtonUserCenter") );
	if( pButtonUserCenter != NULL )  {
		pButtonUserCenter->SetFloat(true);
		pButtonUserCenter->SetPos(15,141);
		pButtonUserCenter->SetFixedWidth(202);
		pButtonUserCenter->SetFixedHeight(40);
		pButtonUserCenter->SetStatusImage( TEXT("file='BT_USER_CENTER' restype='PNG'") );
	}
}

//结束绘画
void CPlazaViewUser::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画背景
	m_ImageBack.DrawImage(pDC,0,0);

	//设置显示
	m_PaintManager.AddFontAt(0,TEXT("宋体"), 14, false, false, false);
	m_PaintManager.AddFontAt(1,TEXT("黑体"), 16, false, false, false);
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
	m_ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("PF_USERINFO_BK"));

	//设置大小
	CSize SizeWindow(m_ImageBack.GetWidth(),m_ImageBack.GetHeight());
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
	pLabelNickName->SetFont(1);
	pLabelNickName->SetTextColor(ARGB(0xFF,0x00,0x00,0x00));
	pLabelNickName->SetText( pUserText );

	//游戏成绩
	CLabelUI * pLabelGameScore = (CLabelUI *) GetControlByName( szLabelGameScoreControl );
	if( pLabelNickName == NULL) return -1;
	_sntprintf(pUserText ,CountArray(pUserText), TEXT(" %s"), AddComma(pGlobalUserData->lUserScore) );
	pLabelGameScore->SetFont(1);
	pLabelGameScore->SetTextColor(ARGB(0xFF,0xb0,0x02,0x02));
	pLabelGameScore->SetText(pUserText);

	//银行成绩
	CLabelUI * pLabelBankScore = (CLabelUI *) GetControlByName( szLabelBankScoreControl );
	if( pLabelNickName == NULL) return -1;
	_sntprintf(pUserText,CountArray(pUserText), TEXT(" %s"), AddComma(pGlobalUserData->lUserInsure) );
	pLabelBankScore->SetFont(1);
	pLabelBankScore->SetTextColor(ARGB(0xFF,0x8c,0x01,0xaa));
	pLabelBankScore->SetText(pUserText);

	return 0;
}
