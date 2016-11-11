#include "stdafx.h"
#include "resource.h"
#include "DlgInsureGame.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

//常量定义
#define SPACE_ROUND					3									//间距大小	

///////////////////////////////////////////////////////////////////////////////////////////////////

//消息映射
BEGIN_MESSAGE_MAP(CDlgInsureGame, CSkinDialog)

	ON_WM_TIMER()	
	ON_WM_SIZE()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//构造函数
CDlgInsureGame::CDlgInsureGame(CWnd* pParent) : CSkinDialog(IDD_BANK_STORAGE, pParent)
{
	//标识变量
	m_bSwitchIng=false;
	m_blCanStore=false;
	m_blCanGetOnlyFree=false;

	//银行信息
	m_wRevenueTake=0;
	m_wRevenueTransfer=0;
	m_lUserScore=0;
	m_lUserInsure=0;

	//接口变量
	m_pIMeUserData=NULL;
	m_pIClientKernel=NULL;	

	//界面控件
	m_pEditAmount=NULL;
	m_pLabelRound=NULL;
	m_pTextForget=NULL;
	m_pEditPassword=NULL;	

	//界面控件
	m_pButtonSave=NULL;
	m_pButtonTake=NULL;
	m_pButtonRefresh=NULL;

	//加载资源
	m_ImageNumber.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("BANKNUM"));
}

//析构函数
CDlgInsureGame::~CDlgInsureGame()
{
}

//回车
VOID CDlgInsureGame::OnOK()
{
	DestroyWindow();
}

//取消
VOID CDlgInsureGame::OnCancel()
{
	DestroyWindow();
}

//初始化函数
BOOL CDlgInsureGame::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("银行"));

	return TRUE;
}

VOID CDlgInsureGame::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

//大小消息
void CDlgInsureGame::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);
}

//初始控件
void CDlgInsureGame::InitControlUI()
{
	__super::InitControlUI();

	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());	

	//变量定义
	CLabelUI * pLabelControl=NULL;

	//银行框架
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetBkImage(TEXT("file='BANK_FRAME' restype='PNG'"));
	pLabelControl->SetFixedWidth(435);
	pLabelControl->SetFixedHeight(85);
	pLabelControl->SetFixedXY(CPoint(32,55));	

	//数量标签
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("输入存取金额："));
	pLabelControl->SetFixedWidth(86);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetFixedXY(CPoint(70,163));
	
	//密码标签
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("输入银行密码："));	
	pLabelControl->SetFixedWidth(86);
	pLabelControl->SetFixedHeight(17);	
	pLabelControl->SetFixedXY(CPoint(70,194));

	//数量编辑
	m_pEditAmount = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditAmount->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG' corner='3,3,3,3'"));	
	m_pEditAmount->SetMaxChar(16);
	m_pEditAmount->SetBkColor(0);
	m_pEditAmount->SetOnlyNumber();
	
	//数量编辑
	m_pEditPassword = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditPassword->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG'corner='3,3,3,3'"));
	m_pEditPassword->SetPasswordMode(true);	
	m_pEditPassword->SetMaxChar(LEN_PASSWORD-1);
	m_pEditPassword->SetBkColor(0);	

	//忘记密码
	m_pTextForget = CTextUI::Create(&m_PaintManager,pParent,TEXT(""));	
	m_pTextForget->SetText(TEXT("{a}{f 2}忘记密码?{/f}{/a}"));	
	m_pTextForget->SetShowHtml();

	//存款按钮
	m_pButtonSave = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonSave->SetStatusImage(TEXT("file='BT_INSURE_SAVE' restype='PNG'"));		

	//取款按钮
	m_pButtonTake = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonTake->SetStatusImage(TEXT("file='BT_INSURE_TAKE' restype='PNG'"));	

	//兑换按钮
	m_pButtonRefresh = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonRefresh->SetStatusImage(TEXT("file='BT_INSURE_REFRESH' restype='PNG'"));

	//调整控件
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());
}

//消息提醒
void CDlgInsureGame::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);
	
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{			
		if(m_pButtonSave==pControlUI)				//存款事件
		{
			return OnBnClickedSaveScore();
		}
		else if(m_pButtonTake==pControlUI)			//取款事件
		{
			return OnBnClickedTakeScore();
		}
		else if(m_pButtonRefresh==pControlUI)  		//刷新事件
		{
			return OnBnClickedRefreshScore();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("link")) ==0 )
	{		
		if(m_pTextForget==pControlUI)				//忘记密码
		{
			return OnBnClickedForgetInsure();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{		
		if(pControlUI==m_pEditAmount)				//存取金额
		{
			return OnEnChangeScore();
		}
	}
}

//绘画界面
VOID CDlgInsureGame::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	//绘画数字
	DrawNumberString(pDC,m_lUserScore,200,78);
	DrawNumberString(pDC,m_lUserInsure,200,104);

	//当前游戏币
	CRect rcScore(100,80,200,95);
	pDC->DrawText(TEXT("当前游戏币："),rcScore,DT_END_ELLIPSIS | DT_WORDBREAK|DT_RIGHT);

	//当前存款
	CRect rcBank(100,105,200,120);
	pDC->DrawText(TEXT("银行游戏币："),rcBank,DT_END_ELLIPSIS | DT_WORDBREAK|DT_RIGHT);

	//提示信息
	CString strInfo;
	CRect rect(0,305,nWidth,nHeight);	
	strInfo.Format(TEXT("温馨提示：存入游戏币免手续费，取出将扣除%ld‰的手续费"), m_wRevenueTake);
	pDC->DrawText(strInfo,rect,DT_END_ELLIPSIS | DT_CENTER | DT_WORDBREAK);
}

//调整控件
void CDlgInsureGame::RectifyControl(INT nWidth, INT nHeight)
{
	//参数校验
	if(nWidth==0 || nHeight==0) return;

	//变量定义
	INT nTViewBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
	INT nBViewBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
	INT nLViewBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRViewBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;

	//调整控件
	if(m_pEditAmount!=NULL)
	{
		//编辑控件
		m_pEditAmount->SetFixedWidth(170);
		m_pEditAmount->SetFixedHeight(24);	
		m_pEditAmount->SetFixedXY(CPoint(nLViewBorder+SPACE_ROUND+148,nTViewBorder+SPACE_ROUND+115));

		//编辑控件		
		m_pEditPassword->SetFixedWidth(170);
		m_pEditPassword->SetFixedHeight(24);
		m_pEditPassword->SetFixedXY(CPoint(nLViewBorder+SPACE_ROUND+148,nTViewBorder+SPACE_ROUND+146));

		//忘记密码		
		m_pTextForget->SetFixedWidth(100);
		m_pTextForget->SetFixedHeight(18);
		m_pTextForget->SetFixedXY(CPoint(nLViewBorder+SPACE_ROUND+332,nTViewBorder+SPACE_ROUND+153));

		//按钮控件
		m_pButtonSave->SetPos(nLViewBorder+SPACE_ROUND+108,nTViewBorder+SPACE_ROUND+190);	
		m_pButtonTake->SetPos(nLViewBorder+SPACE_ROUND+258,nTViewBorder+SPACE_ROUND+190);
		m_pButtonRefresh->SetPos(nLViewBorder+SPACE_ROUND+355,nTViewBorder+SPACE_ROUND+12);
	}
}


//获取游戏币
SCORE CDlgInsureGame::GetUserControlScore(LPCTSTR pszScore)
{
	//变量定义
	WORD wIndex=0;
	SCORE lScore=0L;

	//获取数值
	TCHAR szBuffer[32]=TEXT("");
	lstrcpyn(szBuffer,pszScore,CountArray(szBuffer));

	//构造数值
	while (szBuffer[wIndex]!=0)
	{
		//变量定义
		WORD wCurrentIndex=wIndex++;

		//构造数值
		if ((szBuffer[wCurrentIndex]>=TEXT('0'))&&(szBuffer[wCurrentIndex]<=TEXT('9')))
		{
			lScore=lScore*10L+(szBuffer[wCurrentIndex]-TEXT('0'));
		}
	}

	return lScore;
}

//绘画数字
VOID CDlgInsureGame::DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos)
{
	//转换逗号
	TCHAR szControl[128]=TEXT("");
	SwitchScoreFormat(lScore,3L,szControl,CountArray(szControl));

	//变量定义
	INT nXDrawPos=nXPos;
	INT nScoreLength=lstrlen(szControl);

	//绘画判断
	if (nScoreLength>0L)
	{
		//加载资源
		CPngImage ImageNumber;
		ImageNumber.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("SCORE_NUMBER"));

		//获取大小
		CSize SizeNumber;
		SizeNumber.SetSize(ImageNumber.GetWidth()/12L,ImageNumber.GetHeight());

		//绘画数字
		for (INT i=0;i<nScoreLength;i++)
		{
			//绘画逗号
			if (szControl[i]==TEXT(','))
			{
				ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*10L,0L);
			}

			//绘画点号
			if (szControl[i]==TEXT('.'))
			{
				ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*11L,0L);
			}

			//绘画数字
			if (szControl[i]>=TEXT('0')&&szControl[i]<=TEXT('9'))
			{
				ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*(szControl[i]-TEXT('0')),0L);
			}

			//设置位置
			nXDrawPos+=SizeNumber.cx;
		}
	}

	return;
}

//转换字符
VOID CDlgInsureGame::SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize)
{
	//转换数值
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//变量定义
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//转换字符
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//拷贝字符
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

		//插入逗号
		if ((uSwitchLength!=wSourceIndex)&&(((uSwitchLength-wSourceIndex)%uSpace)==0L))
		{
			pszBuffer[wTargetIndex++]=TEXT(',');
		}
	}

	//结束字符
	pszBuffer[wTargetIndex++]=0;

	return;
}

//分数变更
VOID CDlgInsureGame::OnEnChangeScore()
{
	//状态变量
	if (m_bSwitchIng==false)
	{
		//设置变量
		m_bSwitchIng=true;

		//变量定义
		SCORE lScore=GetUserControlScore(m_pEditAmount->GetText());
		SCORE lMaxScore=__max(m_lUserScore, m_lUserInsure);

		//输入限制
		if(lScore > lMaxScore) lScore = lMaxScore;

		//设置界面
		if (lScore>0L)
		{
			//转换逗号
			TCHAR szControl[128]=TEXT("");
			SwitchScoreFormat(lScore,3L,szControl,CountArray(szControl));

			//设置控件
			m_pEditAmount->SetText(szControl);

			//设置光标
			INT nScoreLength=lstrlen(szControl);
			m_pEditAmount->SetSel(nScoreLength,nScoreLength);
		}
		else
		{
			//设置控件
			m_pEditAmount->SetText(TEXT(""));
		}

		//设置变量
		m_bSwitchIng=false;
	}
}

//存款按钮
VOID CDlgInsureGame::OnBnClickedSaveScore()
{
	//输入金额
	SCORE lInputScore = GetUserControlScore(m_pEditAmount->GetText());

	//参数验证
	if (lInputScore <= 0 || lInputScore > m_lUserScore )
	{
		//变量定义
		TCHAR szMessage[128]=TEXT("");

		//构造消息
		if(m_lUserScore>0)
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("你输入的游戏币金额必须在1和%I64d之间"), m_lUserScore);
		}
		else 
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("你身上携带的游戏币不足,不能进行存款操作!"));
		}

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(szMessage,MB_ICONERROR,10);

		//设置控件
		m_pEditAmount->SetFocus();
		m_pEditAmount->SetText(TEXT(""));

		return;
	}

	//状态判断
	if (US_PLAYING==m_pIMeUserData->GetUserStatus())
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请结束游戏后再存款！"),MB_ICONERROR,30);
		return;	
	}

	//构造数据
	CMD_GR_C_SaveScoreRequest SaveScoreRequest;
	SaveScoreRequest.cbActivityGame=TRUE;
	SaveScoreRequest.lSaveScore=lInputScore;

	//发送数据
	SendSocketData(MDM_GR_INSURE,SUB_GR_SAVE_SCORE_REQUEST,&SaveScoreRequest,sizeof(SaveScoreRequest));

	return;
}

//取款按钮
VOID CDlgInsureGame::OnBnClickedTakeScore()
{
	//输入金额
	SCORE lInputScore = GetUserControlScore(m_pEditAmount->GetText());

	//参数验证
	if (lInputScore <= 0 || lInputScore > m_lUserInsure )
	{
		//变量定义
		TCHAR szMessage[128]=TEXT("");

		//构造消息
		if(m_lUserInsure>0)
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("您输入的游戏币金额必须在1和%I64d之间"), m_lUserInsure);
		}
		else 
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("您银行的游戏币余额不足,不能进行提款操作!"));
		}

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(szMessage,MB_ICONERROR,30);

		//设置控件
		m_pEditAmount->SetFocus();
		m_pEditAmount->SetText(TEXT(""));

		return;
	}

	//密码验证
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	lstrcpyn(szPassword,m_pEditPassword->GetText(),CountArray(szPassword));
	if (szPassword[0]==0)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("银行密码不能为空，请重新输入！"),MB_ICONERROR,30);

		//设置控件
		m_pEditPassword->SetText(TEXT(""));
		m_pEditPassword->SetFocus();

		return;
	}

	//密码加密
	TCHAR szTempPassword[LEN_PASSWORD]=TEXT("");
	CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
	CWHEncrypt::MD5Encrypt(szTempPassword,szPassword);

	//变量定义
	CMD_GR_C_TakeScoreRequest TakeScoreRequest;
	ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));

	//构造结构
	TakeScoreRequest.cbActivityGame=TRUE;
	TakeScoreRequest.lTakeScore=lInputScore;
	CopyMemory(TakeScoreRequest.szInsurePass, szPassword, sizeof(TakeScoreRequest.szInsurePass));

	//发送数据
	SendSocketData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));

	//清空密码
	m_pEditPassword->SetText(TEXT(""));

	return;
}

//刷新按钮
VOID CDlgInsureGame::OnBnClickedRefreshScore()
{
	//变量定义
	CMD_GR_C_QueryInsureInfoRequest QueryInsureInfo;
	QueryInsureInfo.cbActivityGame=TRUE;

	//发送数据
	SendSocketData(MDM_GR_INSURE,SUB_GR_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));
}

//忘记密码
VOID CDlgInsureGame::OnBnClickedForgetInsure()
{
	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/Member/ReInsurePass.aspx"),pGlobalWebLink->GetPlatformLink());

	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);
}

//命令信息
BOOL CDlgInsureGame::PreTranslateMessage(MSG* pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

//时间消息
void CDlgInsureGame::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
}

//设置信息
void CDlgInsureGame::SetSendInfo(IClientKernel *pClientKernel,IClientUserItem const*pMeUserData)
{
	//参数效验
	ASSERT(pClientKernel!=NULL);
	if(pClientKernel==NULL) return;

	//设置信息
	m_pIClientKernel = pClientKernel;
	m_pIMeUserData=const_cast<IClientUserItem *>(pMeUserData);

	//刷新银行
	OnBnClickedRefreshScore();

	return;
}

//设置位置
void CDlgInsureGame::SetPostPoint(CPoint Point)
{
	CRect Rect;
	GetWindowRect(&Rect);
	SetWindowPos(NULL,Point.x,Point.y-Rect.bottom+Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,/*SWP_NOMOVE|*/SWP_NOZORDER);
}

//发送信息
void CDlgInsureGame::SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	//参数效验
	ASSERT(m_pIClientKernel!=NULL);

	//发送信息
	if(m_pIClientKernel) m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return;
}

//显示银行
void CDlgInsureGame::ShowItem()
{
	//变量定义
	bool bShow = true;
	if(m_blCanGetOnlyFree && m_pIMeUserData->GetUserStatus() == US_PLAYING) bShow = false;

	//设置控件
	m_pButtonSave->SetEnabled(m_blCanStore);
	m_pButtonTake->SetEnabled(bShow);
}
///////////////////////////////////////////////////////////////////////////////////////////////////