#include "Stdafx.h"
#include "PlazaViewNotice.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//定时器
#define  IDE_EVENT_MOVE			1000		//移动参数

//////////////////////////////////////////////////////////////////////////////////

//文本参数
const TCHAR * szLabelNoticeText = TEXT( "LabelNoticeText" );
const TCHAR * szNoticeText = 
	TEXT( "恭喜玩家JD...充值500.00元成功" );

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewNotice, CFGuiWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaViewNotice::CPlazaViewNotice()
{
	m_nFixedWidth = 300;
	m_nFixedHeight = 20;

	m_nCurrentX = 0;
	m_nCurrentY = 0;
	m_nCurrentW = 0;

	m_nMainWidth = 0;
	m_nSumWidth = 0;

	m_nMoveUint = 2;
}

//析构函数
CPlazaViewNotice::~CPlazaViewNotice()
{

}

	//添加公告
void CPlazaViewNotice::AddNotice(CContainerUI * pParent, TCHAR * szNotice)
{
	//创建标签
	CLabelUI * pLabel = (CLabelUI *) CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	if( pLabel==NULL ) return;

	//公告队例
	m_wndNoticeArray.Add(pLabel);

	//设置文本;
	pLabel->SetFont(3);
	pLabel->SetTextColor(0xFFFFC76B);
	pLabel->SetText(szNotice);

	//设置位置
	pLabel->SetPos(m_nCurrentX,m_nCurrentY);
	m_nCurrentX+=m_nFixedWidth;

	//设置高宽
	pLabel->SetFixedWidth(m_nCurrentX+m_nFixedWidth);
	pLabel->SetFixedHeight(m_nCurrentY+m_nFixedHeight);

	//最大长度
	m_nMaxCount = m_wndNoticeArray.GetCount();
	m_nSumWidth += m_nFixedWidth;
}


//初始函数
void CPlazaViewNotice::InitControlUI()
{
	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());
	
	//创建对象
	AddNotice(pParent, TEXT( "恭喜玩家JD...充值500.00元成功" ) );
	AddNotice(pParent, TEXT( "恭喜玩家wcj...充值2000.00元成功" ) );
	AddNotice(pParent, TEXT( "恭喜玩家Kim...充值100.00元成功" ) );
	AddNotice(pParent, TEXT( "恭喜玩家Joy...充值500.00元成功" ) );
	AddNotice(pParent, TEXT( "恭喜玩家大...充值1000.00元成功" ) );
	AddNotice(pParent, TEXT( "恭喜玩家Jel...充值1300.00元成功" ) );
	
	//初始化位置
	m_nCurrentX = 0;
	m_nCurrentY = 0;
	m_nCurrentW = m_nSumWidth;

	//定时器
	SetTimer(IDE_EVENT_MOVE,50,NULL);

}

//消息提醒
void CPlazaViewNotice::Notify(TNotifyUI &  msg)
{
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		// 点击后停止
	}
}

//时钟消息
VOID CPlazaViewNotice::OnTimer(UINT nIDEvent)
{
	if(IDE_EVENT_MOVE==nIDEvent) 
	{
		//重新归位
		if(m_nCurrentW<0)
		{
			m_nCurrentW = m_nSumWidth;
			m_nCurrentX = m_nMainWidth;
			for(int i=0; i<m_nMaxCount; i++)
			{
				CLabelUI * pLabel = m_wndNoticeArray[i];
				if (pLabel!=NULL)
				{
					pLabel->SetPos(m_nCurrentX,m_nCurrentY);
					pLabel->SetFixedWidth(m_nCurrentX+m_nFixedWidth);
					pLabel->SetFixedHeight(m_nCurrentY+m_nFixedHeight);
				}
				m_nCurrentX += m_nFixedWidth;
			}
			return;
		}

		//像素移动
		if(m_nMaxCount!=0)
		{
			m_nCurrentW -= m_nMoveUint;
			int nFristPosX = m_wndNoticeArray[0]->GetX();
			for(int i=0; i<m_nMaxCount; i++)
			{
				CLabelUI * pLabel = m_wndNoticeArray[i];
				if (pLabel!=NULL)
				{
					nFristPosX -= m_nMoveUint;
					pLabel->SetPos(nFristPosX,m_nCurrentY);
					pLabel->SetFixedWidth(nFristPosX+m_nFixedWidth);
					pLabel->SetFixedHeight(m_nCurrentY+m_nFixedHeight);
					nFristPosX += m_nFixedWidth;
				}
			}
		}
	}

	__super::OnTimer(nIDEvent);
}

//位置消息
VOID CPlazaViewNotice::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);
}

//创建消息
INT CPlazaViewNotice::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("PLAZA_NOTICE_BK"));

	//设置大小
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL, 0, 0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//主窗大小
	m_nMainWidth = ImageBack.GetWidth();

	return 0;
}
