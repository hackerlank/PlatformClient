#include "Stdafx.h"
#include "DlgEnquire.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//圆角大小
#define ROUND_CX					9									//圆角宽度
#define ROUND_CY					10									//圆角高度

//屏幕位置
#define LAYERED_SIZE				6									//分层大小
#define CAPTION_SIZE				38									//标题大小

//控件标识
#define IDC_WEB_PUBLICIZE			300									//浏览控件

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgEnquire::CDlgEnquire() : CFGuiDialog(IDD_DLG_POPUP)
{
	//设置变量
	m_pbtCancel=NULL;
	m_pbtClosePlaza=NULL;
	m_pbtCloseServer=NULL;
	m_pbtSwitchAccounts=NULL;
}

//析构函数
CDlgEnquire::~CDlgEnquire()
{
}

//配置函数
BOOL CDlgEnquire::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("温馨提示"));

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_ENQUIRE_BACK"));

	//设置大小
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//广告控件
	//CRect rcCreate(0,0,0,0);
	//m_PlatformPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	//获取对象
	//CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	//TCHAR szBillUrl[256]=TEXT("");
	//_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/ClosePlaza.aspx"),pGlobalWebLink->GetPlatformLink());//

	//设置广告
	//m_PlatformPublicize.Navigate(szBillUrl);
	//m_PlatformPublicize.SetWindowPos(NULL,3,43,SizeWindow.cx-6,95,SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOACTIVATE);

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

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,ROUND_CX,ROUND_CY);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);

	//分层窗口
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageBack,0,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	//调整控件
	RectifyControlUI();

	return FALSE;
}

//确定函数
VOID CDlgEnquire::OnOK()
{
	return;
}

//初始控件
void CDlgEnquire::InitControlUI()
{
	//设置标题
	m_PaintManager.SetCaptionRect(CRect(0,0,0,CAPTION_SIZE));

	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//关闭按钮
	m_pbtCancel = CButtonUI::Create(&m_PaintManager, pParent, TEXT(""));
	m_pbtCancel->SetStatusImage(TEXT("file='BT_ENQUIRE_CANCEL' restype='PNG'"));

	//关闭大厅
	m_pbtClosePlaza = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtClosePlaza->SetStatusImage(TEXT("file='BT_CLOSE_PLAZA' restype='PNG'"));

	//关闭房间
	m_pbtCloseServer = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtCloseServer->SetStatusImage(TEXT("file='BT_CLOSE_SERVER' restype='PNG'"));

	//切换帐号
	m_pbtSwitchAccounts = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtSwitchAccounts->SetStatusImage(TEXT("file='BT_SWITCH_ACCOUNTS' restype='PNG'"));	


	//获取对象
	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	m_pbtCloseServer->SetEnabled( pPlatformFrame->IsEnteredServerItem() );

	//调整控件
	RectifyControlUI();
}

//消息提醒
void CDlgEnquire::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(pControlUI==m_pbtCancel) return OnCancel();
		if(pControlUI==m_pbtClosePlaza) return OnBnClickedClosePlaza();
		if(pControlUI==m_pbtCloseServer) return OnBnClickedCloseServer();
		if(pControlUI==m_pbtSwitchAccounts) return OnBnClickedSwitchAccounts();
	}

}

//结束绘画
void CDlgEnquire::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_ENQUIRE_BACK"));

	//加载资源
	CPngImage ImageTitle;
	ImageTitle.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_ENQUIRE_TAG"));

	//绘画背景
	ImageBack.DrawImage(pDC,0,0);
	ImageTitle.DrawImage(pDC,30,70);
}

//关闭大厅
VOID CDlgEnquire::OnBnClickedClosePlaza()
{
	EndDialog(IDC_CLOSE_PLAZA);

	return;
}

//关闭房间
VOID CDlgEnquire::OnBnClickedCloseServer()
{
	EndDialog(IDC_CLOSE_SERVER);

	return;
}

//切换帐号
VOID CDlgEnquire::OnBnClickedSwitchAccounts()
{
	EndDialog(IDC_SWITCH_ACCOUNTS);

	return;
}

//调整控件
VOID CDlgEnquire::RectifyControlUI()
{
	//获取区域
	CRect rcClientRect;
	GetClientRect(&rcClientRect);

	//变量定义
	CPoint ptControl;
	CSize SizeControl;

	//取消按钮
	ptControl.SetPoint(rcClientRect.right-36,8);
	SizeControl.SetSize(m_pbtCancel->GetFixedWidth(),m_pbtCancel->GetFixedHeight());
	m_pbtCancel->SetPos(CRect(ptControl.x,ptControl.y,ptControl.x+SizeControl.cx,SizeControl.cy+ptControl.y));

	//变量定义
	INT nButtonWidth = m_pbtClosePlaza->GetFixedWidth();
	INT nButtonSpace=(rcClientRect.Width()-nButtonWidth*3)/4;

	//设置尺寸
	SizeControl.SetSize(m_pbtSwitchAccounts->GetFixedWidth(),m_pbtSwitchAccounts->GetFixedHeight());

	//切换帐号
	ptControl.SetPoint(nButtonSpace,154);	
	m_pbtSwitchAccounts->SetPos(CRect(ptControl.x,ptControl.y,ptControl.x+SizeControl.cx,SizeControl.cy+ptControl.y));

	//关闭房间
	ptControl.SetPoint(nButtonSpace*2+nButtonWidth,154);
	m_pbtCloseServer->SetPos(CRect(ptControl.x,ptControl.y,ptControl.x+SizeControl.cx,SizeControl.cy+ptControl.y));

	//关闭大厅
	ptControl.SetPoint(nButtonSpace*3+nButtonWidth*2,154);
	m_pbtClosePlaza->SetPos(CRect(ptControl.x,ptControl.y,ptControl.x+SizeControl.cx,SizeControl.cy+ptControl.y));
}

//////////////////////////////////////////////////////////////////////////////////
