#include "StdAfx.h"
#include "Resource.h"
#include "Dlgmatchawards.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//屏幕位置
#define LAYERED_SIZE						5									//分层大小

//常量定义
#define ROUND_CX							6									//圆角大小
#define ROUND_CY							6									//圆角大小

//////////////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgMatchAwards::CDlgMatchAwards() : CFGuiDialog(IDD_DLG_MATCH_AWARDS)
{
	//设置变量
	m_pMatchResult=NULL;
	m_pbtClose=NULL;
}

CDlgMatchAwards::~CDlgMatchAwards()
{

}

//设置参数
VOID CDlgMatchAwards::SetMatchResult(CMD_GR_MatchResult *pMatchResult) 
{ 
	//设置变量
	m_pMatchResult=pMatchResult; 

	//刷新界面
	InvalidateRect(NULL);
}

//初始控件
VOID CDlgMatchAwards::InitControlUI()
{
	//设置标题
	CRect rcCaption(0,0,0,200);
	m_PaintManager.SetCaptionRect(rcCaption);

	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//返回按钮
	m_pbtClose = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtClose->SetStatusImage(TEXT("file='BT_REWARD_CLOSE' restype='PNG'"));	
	m_pbtClose->SetRelativeSide(RelativeRight);
	m_pbtClose->SetRelativePos(CPoint(36,10));
}

//消息提醒
VOID CDlgMatchAwards::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//退出按钮
		if(m_pbtClose==pControlUI) return OnCancel(); 
	}
}

//开始绘画
VOID CDlgMatchAwards::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//设置环境
	pDC->SetBkMode(TRANSPARENT);

	//获取区域
	CRect rcClient;
	GetClientRect(&rcClient);

	//变量定义
	CPngImage ImageStar,ImageAwards;

	//加载图片
	ImageStar.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME), TEXT("MATCH_STAR"));
	ImageAwards.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME), TEXT("MATCH_AWARDS"));

	//计算位置
	INT nXPos=(rcClient.Width()-ImageAwards.GetWidth())/2;
	INT nYPos=(rcClient.Height()-ImageAwards.GetHeight())/2+10;

	//绘画背景
	ImageAwards.DrawImage(pDC,0,0);

	//绘画奖励
	if(m_pMatchResult!=NULL)
	{	
		//设置字体
		pDC->SelectObject(m_PaintManager.GetFont(4));
		pDC->SetTextColor(RGB(255,255,255));

		//绘画描述
		DrawMultLineText(pDC,m_pMatchResult->szDescribe,&CRect(nXPos+50,nYPos+100,rcClient.Width()-(nXPos+50),nYPos+180),8,DT_TOP|DT_WORDBREAK|DT_EXTERNALLEADING);

		//设置颜色
		pDC->SetTextColor(RGB(255,255,0));

		//变量定义
		int nIndex=0;
		CString strText;

		//游戏币奖励
		if(m_pMatchResult->lGold>0)
		{
			ImageStar.DrawImage(pDC,nXPos+60,nYPos+160-2);
			strText.Format(TEXT("游戏币：%d"), m_pMatchResult->lGold);			
			pDC->DrawText(strText, &CRect(nXPos+95,nYPos+160,nXPos+360,nYPos+185),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}

		//元宝奖励
		if(m_pMatchResult->dwIngot>0)
		{
			ImageStar.DrawImage(pDC,nXPos+60,nYPos+160+nIndex*30-2);
			strText.Format(TEXT("元  宝：%d"), m_pMatchResult->dwIngot);
			pDC->DrawText(strText, &CRect(nXPos+95,nYPos+160+nIndex*30,nXPos+360,nYPos+185+nIndex*30),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}

		//经验奖励
		if(m_pMatchResult->dwExperience>0)
		{
			ImageStar.DrawImage(pDC,nXPos+60,nYPos+160+nIndex*30-2);
			strText.Format(TEXT("经  验：%d"), m_pMatchResult->dwExperience);
			pDC->DrawText(strText, &CRect(nXPos+95,nYPos+160+nIndex*30,nXPos+360,nYPos+185+nIndex*30),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}
	}
}

BOOL CDlgMatchAwards::OnInitDialog()
{
	__super::OnInitDialog();

	//变量定义
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME), TEXT("MATCH_AWARDS"));

	//变量定义
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());

	//设置窗口
	SetWindowPos(NULL, 0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//居中窗口
	CenterWindow();

	//获取区域
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	CRect rcUnLayered;
	rcUnLayered.left=10;
	rcUnLayered.top=8;	
	rcUnLayered.right=rcWindow.Width()-ROUND_CX;
	rcUnLayered.bottom=rcWindow.Height()-ROUND_CY;

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_CX,ROUND_CY);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);

	//分层窗口
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return TRUE; 
}

//确定函数
VOID CDlgMatchAwards::OnCancel()
{
	__super::OnCancel();

	//销毁窗口
	DestroyWindow();
}

//绘制多行文本
VOID CDlgMatchAwards::DrawMultLineText(CDC *pDC, CString strText, CRect rect, int nRowDis, UINT nFromat)
{
	if (strText.GetLength()<=0) return;

	INT nStrlen = strText.GetLength();
	TCHAR* pText = strText.GetBuffer(nStrlen);

	CSize size = pDC->GetTextExtent(pText + 0, 1);
	INT nRowHeight = size.cy + nRowDis;

	CRect rtChar;
	rtChar.top = rect.top;
	rtChar.left = rect.left;
	rtChar.bottom  = rtChar.top + nRowDis + size.cy;
	rtChar.right  = rtChar.left + size.cx; 

	CString strChar;
	for (int nCharIndex = 0; nCharIndex < nStrlen; nCharIndex++)
	{
		if( rtChar.right > rect.right )
		{
			rtChar.top = rtChar.bottom;
			rtChar.bottom += nRowHeight;
			size = pDC->GetTextExtent(pText + nCharIndex, 1);
			rtChar.left = rect.left;
			rtChar.right = rtChar.left + size.cx;
			if( rtChar.bottom > rect.bottom )
				break;
		}
		strChar = pText[nCharIndex];
		pDC->DrawText(strChar, rtChar, nFromat);
		size = pDC->GetTextExtent(pText + nCharIndex + 1, 1);
		rtChar.left = rtChar.right;
		rtChar.right += size.cx;
	}
}