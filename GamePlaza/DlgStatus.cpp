#include "StdAfx.h"
#include "DlgStatus.h"
#include "Resource.h"
#include "ServerListData.h"
#include "PlatformFrame.h"


//////////////////////////////////////////////////////////////////////////////////

//圆角大小
#define ROUND_CX					5									//圆角宽度
#define ROUND_CY					5									//圆角高度

//时间标识
#define IDT_STATUS					10									//状态时间
#define TIMER_STATUS				20									//状态时间

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgStatus, CFGuiDialog)

	//系统消息
	ON_WM_TIMER()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgStatus::CDlgStatus() : CFGuiDialog(IDD_DLG_POPUP)
{
	//状态变量
	m_uImageIndex=0;

	//控件变量
	m_pbtGiveUp=NULL;

	//接口变量
	m_pIStatusViewSink=NULL;

	return;
}

//析构函数
CDlgStatus::~CDlgStatus()
{
}

//消息解释
BOOL CDlgStatus::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//配置函数
BOOL CDlgStatus::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(szProduct);

	//设置控件
	SetTimer(IDT_STATUS,TIMER_STATUS,NULL);

	//加载资源
	CPngImage ImageStatus;
	ImageStatus.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_STATUS_BACK"));

	//设置大小
	CSize SizeWindow(ImageStatus.GetWidth(),ImageStatus.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//居中窗口
	CenterWindow();

	//获取窗口
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	CRect rcUnLayered;
	rcUnLayered.top=5;
	rcUnLayered.left=5;
	rcUnLayered.right=rcWindow.Width()-5;
	rcUnLayered.bottom=rcWindow.Height()-5;

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_CX,ROUND_CY);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);

	//分层窗口
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageStatus,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return TRUE;
}

//初始控件
void CDlgStatus::InitControlUI()
{
	//标题区域
	m_PaintManager.SetCaptionRect(CRect(0,0,0,67));

	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//查看按钮
	m_pbtGiveUp = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtGiveUp->SetStatusImage(TEXT("file='BT_STATUS_CANCLE' restype='PNG'"));
	m_pbtGiveUp->SetPos(CRect(329,14,353,37));
}

//消息提醒
void CDlgStatus::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(m_pbtGiveUp==pControlUI) 
		{
			return OnBnClickedGiveUp(); 
		}
	}
}

//开始绘画
void CDlgStatus::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//变量定义
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//绘画背景
	CPngImage ImageStatus;
	ImageStatus.LoadImage(hInstance,TEXT("DLG_STATUS_BACK"));
	ImageStatus.DrawImage(pDC,0,0,rcClient.Width(),rcClient.Height(),0,0);

	//加载资源
	CPngImage ImageStatusLine;
	CPngImage ImageStatusPoint;
	ImageStatusLine.LoadImage(hInstance,TEXT("STATUS_LINE"));
	ImageStatusPoint.LoadImage(hInstance,TEXT("STATUS_POINT"));

	//获取大小
	CSize SizeStatusLine;
	CSize SizeStatusPoint;
	SizeStatusLine.SetSize(ImageStatusLine.GetWidth(),ImageStatusLine.GetHeight()/2);
	SizeStatusPoint.SetSize(ImageStatusPoint.GetWidth(),ImageStatusPoint.GetHeight());

	//绘画进度
	ImageStatusLine.DrawImage(pDC,16,17,SizeStatusLine.cx,SizeStatusLine.cy,0,0);
	ImageStatusLine.DrawImage(pDC,16,17,SizeStatusLine.cx*m_uImageIndex/100L,SizeStatusLine.cy,0,SizeStatusLine.cy);

	////绘画进度
	INT nXPoint=SizeStatusLine.cx*m_uImageIndex/100L;
	ImageStatusPoint.DrawImage(pDC,16+nXPoint-SizeStatusPoint.cx/2,17+(SizeStatusLine.cy-SizeStatusPoint.cy)/2);

	//文字位置
	CRect rcDescribe;
	rcDescribe.SetRect(14,42,353,55);

	//绘画文字
	pDC->SetTextColor(RGB(241,204,129));
	pDC->DrawText(m_strDescribe,rcDescribe,DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE);

	return;
}

//关闭窗口
VOID CDlgStatus::HideStatusWindow()
{
	//关闭窗口
	if (m_hWnd!=NULL) DestroyWindow();

	return;
}

//设置消息
VOID CDlgStatus::ShowStatusWindow(LPCTSTR pszDescribe)
{
	//创建窗口
	if (m_hWnd==NULL)
	{
		if (CPlatformFrame::GetInstance()->IsWindowVisible()==FALSE)
		{
			Create(IDD_DLG_POPUP,GetDesktopWindow());
		}
		else
		{
			Create(IDD_DLG_POPUP,CPlatformFrame::GetInstance());
		}
	}

	//设置描述
	m_strDescribe=pszDescribe;

	//显示窗口
	ShowWindow(SW_SHOW);

	//更新窗口
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

	return;
}

//设置接口
VOID CDlgStatus::SetStatusViewSink(IStatusViewSink * pIStatusViewSink)
{
	//设置变量
	m_pIStatusViewSink=pIStatusViewSink;

	return;
}

//时间消息
VOID CDlgStatus::OnTimer(UINT nIDEvent)
{
	//设置进度
	if (nIDEvent==IDT_STATUS)
	{
		//设置索引
		m_uImageIndex=(m_uImageIndex>=100)?0:(m_uImageIndex+1L);

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//取消按钮
VOID CDlgStatus::OnBnClickedGiveUp()
{
	//隐藏界面
	HideStatusWindow();

	//投递消息
	ASSERT(m_pIStatusViewSink!=NULL);
	if (m_pIStatusViewSink!=NULL) m_pIStatusViewSink->OnStatusCancel();

	return;
}
//////////////////////////////////////////////////////////////////////////////////
