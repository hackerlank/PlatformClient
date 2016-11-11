#include "StdAfx.h"
#include "Resource.h"
#include "Information.h"
#include "DlgInsureCounter.h"
#include "UserItemElement.h"

//////////////////////////////////////////////////////////////////////////////////

//控件标识
#define IDC_INSRUE_SAVE					100							//银行存取	
#define IDC_INSURE_TRANSFER				101							//银行转帐
#define IDC_INSURE_RECORD				102							//操作记录
#define IDC_INSURE_SETUP				200							//银行设置
#define IDC_INSURE_OPERATER				201							//银行操作
#define IDC_PULICIZE_RECORD				300							//操作记录

//////////////////////////////////////////////////////////////////////////////////

//消息映射
BEGIN_MESSAGE_MAP(CDlgInsureOperater, CFGuiWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//消息映射
BEGIN_MESSAGE_MAP(CDlgInsureRecord, CFGuiWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//获取游戏币
SCORE CDlgInsureItem::GetUserControlScore(LPCTSTR pszScore)
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
VOID CDlgInsureItem::DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos)
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
VOID CDlgInsureItem::SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize)
{
	//变量定义
	LPCTSTR pszNumber[]={TEXT("零"),TEXT("壹"),TEXT("贰"),TEXT("叁"),TEXT("肆"),TEXT("伍"),TEXT("陆"),TEXT("柒"),TEXT("捌"),TEXT("玖")};
	LPCTSTR pszWeiName[]={TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("万"),TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("亿"),TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("万")};

	//转换数值
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//变量定义
	bool bNeedFill=false;
	bool bNeedZero=false;
	UINT uTargetIndex=0,uSourceIndex=0;

	//字符长度
	UINT uItemLength=lstrlen(pszNumber[0]);
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//转换操作
	for (UINT i=0;i<uSwitchLength;i++)
	{
		//变量定义
		WORD wNumberIndex=szSwitchScore[i]-TEXT('0');

		//补零操作
		if ((bNeedZero==true)&&(wNumberIndex!=0L))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszNumber[0],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//拷贝数字
		if (wNumberIndex!=0L)
		{
			_tcsncat(pszBuffer,pszNumber[wNumberIndex],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//拷贝位名
		if ((wNumberIndex!=0L)&&((uSwitchLength-i)>=2))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//补零判断
		if ((bNeedZero==false)&&(wNumberIndex==0))
		{
			bNeedZero=true;
		}

		//补位判断
		if ((bNeedFill==false)&&(wNumberIndex!=0))
		{
			bNeedFill=true;
		}

		//填补位名
		if (((uSwitchLength-i)==5)||((uSwitchLength-i)==9))
		{
			//拷贝位名
			if ((bNeedFill==true)&&(wNumberIndex==0L))
			{
				_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
			}

			//设置变量
			bNeedZero=false;
			bNeedFill=false;
		}
	}

	return;
}

//转换字符
VOID CDlgInsureItem::SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize)
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

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgInsureSave::CDlgInsureSave()
{
	//设置变量
	m_bSwitchIng=false;

	//窗口指针
	m_pDlgInsureMain=NULL;
	m_pDlgInsureOperater=NULL;

	//界面控件
	m_pEditAmount=NULL;
	m_pEditInsurePass=NULL;
	m_pTextForget=NULL;
	m_pButtonSave=NULL;
	m_pButtonTake=NULL;
	m_pButtonExchange=NULL;

	return;
}

//析构函数
CDlgInsureSave::~CDlgInsureSave()
{
}

//开始绘画
VOID CDlgInsureSave::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画背景
	pDC->FillSolidRect(&rcClient,m_pDlgInsureOperater->GetItemBackColor());

	//资源句柄
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//加载图片
	CPngImage ImageMoney;
	CPngImage ImageBank;
	CPngImage ImageLine;
	ImageMoney.LoadImage(hInstance,TEXT("IMAGE_MONEY"));
	ImageBank.LoadImage(hInstance,TEXT("IMAGE_BANK"));
	ImageLine.LoadImage(hInstance,TEXT("IMAGE_LINE"));

	//绘画图片
	ImageMoney.DrawImage(pDC,16,4);
	ImageBank.DrawImage(pDC,298,4);
	ImageLine.DrawImage(pDC,0,134);

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	TCHAR szControl[128]=TEXT("");
	TCHAR szGameScore[64]=TEXT("");
	TCHAR szInsureScore[64]=TEXT("");
	TCHAR szServiceFee[64]=TEXT("");

	//区域定义
	CRect rcGameScore(16,84,134,100);
	CRect rcInsureScore(300,84,418,100);
	CRect rcServiceFee(70,251,439,273);

	//绘制游戏币		
	CDlgInsureItem::SwitchScoreFormat(pGlobalUserData->lUserScore,3L,szControl,CountArray(szControl));
	_sntprintf(szGameScore,CountArray(szGameScore),TEXT("游戏币：%s"),szControl);
	pDC->DrawText(szGameScore,rcGameScore,DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//绘制存款
	CDlgInsureItem::SwitchScoreFormat(pGlobalUserData->lUserInsure,3L,szControl,CountArray(szControl));
	_sntprintf(szInsureScore,CountArray(szInsureScore),TEXT("存款：%s"),szControl);
	pDC->DrawText(szInsureScore,rcInsureScore,DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//服务费用	
	_sntprintf(szServiceFee,CountArray(szServiceFee),TEXT("温馨提示：存入游戏币免手续费，取出将扣除%d‰的手续费。"),m_pUserInsureInfo->wRevenueTake);
	pDC->DrawText(szServiceFee,rcServiceFee,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//初始控件
VOID CDlgInsureSave::InitControlUI()
{
	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//变量定义
	CLabelUI * pLabelControl=NULL;

	//数量标签
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("输入存取金额："));
	pLabelControl->SetFixedWidth(86);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetFixedXY(CPoint(70,150));
	
	//密码标签
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("输入银行密码："));
	pLabelControl->SetFixedXY(CPoint(70,182));
	pLabelControl->SetFixedWidth(86);
	pLabelControl->SetFixedHeight(17);	

	//刷新标签
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetBkImage(TEXT("file='IMAGE_INSURE_REFRESH' restype='PNG'"));
	pLabelControl->SetFixedXY(CPoint(173,16));
	pLabelControl->SetFixedWidth(86);
	pLabelControl->SetFixedHeight(86);

	//数量编辑
	m_pEditAmount = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditAmount->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG' corner='3,3,3,3'"));
	m_pEditAmount->SetFixedXY(CPoint(158,145));
	m_pEditAmount->SetFixedWidth(170);
	m_pEditAmount->SetFixedHeight(24);	
	m_pEditAmount->SetMaxChar(16);
	m_pEditAmount->SetBkColor(0);
	m_pEditAmount->SetOnlyNumber();
	
	//数量编辑
	m_pEditInsurePass = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditInsurePass->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG'corner='3,3,3,3'"));
	m_pEditInsurePass->SetFixedXY(CPoint(158,176));
	m_pEditInsurePass->SetFixedWidth(170);
	m_pEditInsurePass->SetFixedHeight(24);	
	m_pEditInsurePass->SetPasswordMode(true);	
	m_pEditInsurePass->SetMaxChar(LEN_PASSWORD-1);
	m_pEditInsurePass->SetBkColor(0);	

	//忘记密码
	m_pTextForget = CTextUI::Create(&m_PaintManager,pParent,TEXT(""));	
	m_pTextForget->SetText(TEXT("{a}{f 2}忘记密码?{/f}{/a}"));	
	m_pTextForget->SetFixedXY(CPoint(342,185));
	m_pTextForget->SetFixedWidth(100);
	m_pTextForget->SetFixedHeight(18);	
	m_pTextForget->SetShowHtml();	

	//存款按钮
	m_pButtonSave = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonSave->SetStatusImage(TEXT("file='BT_INSURE_SAVE' restype='PNG'"));	
	m_pButtonSave->SetFixedXY(CPoint(118,212));

	//取款按钮
	m_pButtonTake = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonTake->SetStatusImage(TEXT("file='BT_INSURE_TAKE' restype='PNG'"));	
	m_pButtonTake->SetFixedXY(CPoint(268,212));

	//兑换按钮
	m_pButtonExchange = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonExchange->SetStatusImage(TEXT("file='BT_INSURE_EXCHANGE' restype='PNG'"));	
	m_pButtonExchange->SetFixedXY(CPoint(40,106));
}

//消息提醒
VOID CDlgInsureSave::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{		
		if(pControlUI==m_pButtonSave)			//存款操作
		{
			return OnBnClickedSaveScore();
		}
		else if(pControlUI==m_pButtonTake)		//取款操作	
		{
			return OnBnClickedTakeScore();
		}
		else if(pControlUI==m_pButtonExchange)	//兑换操作
		{
			return OnBnClickedExChangeScore();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("link")) ==0 )
	{
		if(pControlUI==m_pTextForget)			//忘记密码
		{
			return OnBnClickedForgetInsure();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{
		if(pControlUI==m_pEditAmount)			//存取分数
		{
			return OnEnChangeScore();
		}
	}	
}

//取款按钮
VOID CDlgInsureSave::OnBnClickedTakeScore()
{
	//获取变量
	SCORE lTakeScore= CDlgInsureItem::GetUserControlScore(m_pEditAmount->GetText());

	//获取密码
	TCHAR szPassword[LEN_PASSWORD];
	lstrcpyn(szPassword,m_pEditInsurePass->GetText(),CountArray(szPassword));

	//加密密码
	TCHAR szInsurePass[LEN_MD5];
	CWHEncrypt::MD5Encrypt(szPassword,szInsurePass);

	//数据判断
	if (lTakeScore<=0L)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("取出的游戏币数量不能为空，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditAmount->SetFocus();

		return;
	}

	//数目判断
	if (lTakeScore>m_pUserInsureInfo->lUserInsure)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您银行游戏币的数目余额不足，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditAmount->SetFocus();

		return;
	}

	//密码判断
	if (szPassword[0]==0)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("银行密码不能为空，请重新输入银行密码！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditInsurePass->SetFocus();

		return;
	}
	
	//事件通知
	m_pDlgInsureMain->OnMissionStart();
	m_pDlgInsureMain->PerformTakeScore(lTakeScore,szInsurePass);

	return;
}

//分数变更
VOID CDlgInsureSave::OnEnChangeScore()
{
	//状态变量
	if (m_bSwitchIng==false)
	{
		//设置变量
		m_bSwitchIng=true;

		//变量定义
		SCORE lScore=CDlgInsureItem::GetUserControlScore(m_pEditAmount->GetText());
		SCORE lMaxScore=__max(m_pUserInsureInfo->lUserScore, m_pUserInsureInfo->lUserInsure);
		if((m_pUserInsureInfo->lUserScore==0) || (m_pUserInsureInfo->lUserInsure==0))
		{
			lMaxScore=__max(m_pUserInsureInfo->lUserScore, m_pUserInsureInfo->lUserInsure);
		}

		//输入限制
		if(lScore > lMaxScore) lScore = lMaxScore;

		//设置界面
		if (lScore>0L)
		{
			//转换逗号
			TCHAR szControl[128]=TEXT("");
			CDlgInsureItem::SwitchScoreFormat(lScore,3L,szControl,CountArray(szControl));

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

	return;
}

//存款按钮
VOID CDlgInsureSave::OnBnClickedSaveScore()
{
	//变量定义
	SCORE lSaveScore=CDlgInsureItem::GetUserControlScore(m_pEditAmount->GetText());

	//数据判断
	if (lSaveScore<=0L)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("存入的游戏币数量不能为空，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditAmount->SetFocus();

		return;
	}

	//数目判断
	if (lSaveScore>m_pUserInsureInfo->lUserScore)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您所携带游戏币的数目余额不足，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditAmount->SetFocus();

		return;
	}

	//执行存款
	m_pDlgInsureMain->OnMissionStart();
	m_pDlgInsureMain->PerformSaveScore(lSaveScore);

	return;
}

//忘记密码
VOID CDlgInsureSave::OnBnClickedForgetInsure()
{
	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//构造地址
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/%sMember/ReInsurePass.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	return;
}

//兑换游戏币
VOID CDlgInsureSave::OnBnClickedExChangeScore()
{
	//获取对象
	ASSERT(AfxGetMainWnd()!=NULL);
	if(AfxGetMainWnd()==NULL) return;

	//构造结构
	tagInsurePlazaEvent * pInsurePlazaEvent = new tagInsurePlazaEvent;
	ZeroMemory(pInsurePlazaEvent,sizeof(tagInsurePlazaEvent));

	//消息通知
	pInsurePlazaEvent->bHeapData=true;
	pInsurePlazaEvent->wCommandID=INSURE_COMMAND_EXCHANGE;
	AfxGetMainWnd()->PostMessage(WM_INSUREPLAZA_EVENT,(WPARAM)pInsurePlazaEvent,0L);
	
	return m_pDlgInsureMain->OnOK();
}


//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgInsureTransfer::CDlgInsureTransfer()
{
	//设置变量
	m_bSwitchIng=false;

	//窗口指针
	m_pDlgInsureMain=NULL;
	m_pDlgInsureOperater=NULL;

	//界面控件
	m_pEditAccount=NULL;
	m_pEditAmount=NULL;
	m_pEditInsurePass=NULL;
	m_pEditRemarks=NULL;

	//界面控件
	m_pLabelTarget=NULL;
	m_pButtonConfirm=NULL;
	m_pRadioByUserID=NULL;
	m_pRadioByNickName=NULL;

	return;
}

//析构函数
CDlgInsureTransfer::~CDlgInsureTransfer()
{
}

//开始绘画
VOID CDlgInsureTransfer::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画背景
	pDC->FillSolidRect(&rcClient,m_pDlgInsureOperater->GetItemBackColor());

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//绘制昵称
	CRect rcNickName(168,16,268,32);
	pDC->DrawText(pGlobalUserData->szNickName,rcNickName,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//变量定义
	TCHAR szUserInsure[32]=TEXT("");
	CDlgInsureItem::SwitchScoreFormat(pGlobalUserData->lUserInsure,3,szUserInsure,CountArray(szUserInsure));

	//银行余额
	CRect rcUserInsure(168,37,268,53);
	pDC->DrawText(szUserInsure,rcUserInsure,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//服务费用
	TCHAR szServiceFee[128]=TEXT("");
	CRect rcServiceFee(20,251,rcClient.Width()-20,273);
	_sntprintf(szServiceFee,CountArray(szServiceFee),TEXT("温馨提示：转帐普通玩家扣除%ld‰的手续费，会员玩家扣除%ld‰的手续费。"),m_pUserInsureInfo->wRevenueTransfer,m_pUserInsureInfo->wRevenueTransferMember);
	pDC->DrawText(szServiceFee,rcServiceFee,DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//初始控件
VOID CDlgInsureTransfer::InitControlUI()
{
	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//变量定义
	CLabelUI * pLabelControl = NULL;

	//操作帐号
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("操作帐号："));
	pLabelControl->SetFixedWidth(62);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetFixedXY(CPoint(100,17));
	
	//帐号余额
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("银行存款："));
	pLabelControl->SetFixedXY(CPoint(100,37));
	pLabelControl->SetFixedWidth(62);
	pLabelControl->SetFixedHeight(17);

	//接收帐号
	m_pLabelTarget = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pLabelTarget->SetText(TEXT("接收昵称："));
	m_pLabelTarget->SetFixedXY(CPoint(100,87));
	m_pLabelTarget->SetFixedWidth(62);
	m_pLabelTarget->SetFixedHeight(17);	

	//赠送金额
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("赠送金额："));
	pLabelControl->SetFixedXY(CPoint(100,117));
	pLabelControl->SetFixedWidth(62);
	pLabelControl->SetFixedHeight(17);

	//银行密码
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("银行密码："));
	pLabelControl->SetFixedXY(CPoint(100,147));
	pLabelControl->SetFixedWidth(62);
	pLabelControl->SetFixedHeight(17);	

	//备注说明
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("备注说明："));
	pLabelControl->SetFixedXY(CPoint(100,177));
	pLabelControl->SetFixedWidth(62);
	pLabelControl->SetFixedHeight(17);

	//接收帐号
	m_pEditAccount = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditAccount->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG' corner='3,3,3,3'"));
	m_pEditAccount->SetMaxChar(LEN_ACCOUNTS-1);
	m_pEditAccount->SetFixedXY(CPoint(162,82));
	m_pEditAccount->SetFixedWidth(170);
	m_pEditAccount->SetFixedHeight(24);		
	m_pEditAccount->SetBkColor(0);

	//转帐金额
	m_pEditAmount = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditAmount->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG' corner='3,3,3,3'"));
	m_pEditAmount->SetFixedXY(CPoint(162,112));
	m_pEditAmount->SetFixedWidth(170);
	m_pEditAmount->SetFixedHeight(24);	
	m_pEditAmount->SetMaxChar(16);
	m_pEditAmount->SetBkColor(0);
	m_pEditAmount->SetOnlyNumber();

	//银行密码
	m_pEditInsurePass = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditInsurePass->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG' corner='3,3,3,3'"));
	m_pEditInsurePass->SetPasswordMode(true);
	m_pEditInsurePass->SetMaxChar(LEN_PASSWORD-1);
	m_pEditInsurePass->SetFixedXY(CPoint(162,142));
	m_pEditInsurePass->SetFixedWidth(170);
	m_pEditInsurePass->SetFixedHeight(24);		
	m_pEditInsurePass->SetBkColor(0);
	
	//转帐备注
	m_pEditRemarks = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditRemarks->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG' corner='3,3,3,3'"));
	m_pEditRemarks->SetMaxChar(LEN_TRANS_REMARK-1);
	m_pEditRemarks->SetFixedXY(CPoint(162,172));
	m_pEditRemarks->SetFixedWidth(170);
	m_pEditRemarks->SetFixedHeight(24);		
	m_pEditRemarks->SetBkColor(0);

	//单选按钮
	m_pRadioByUserID = CRadioButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pRadioByUserID->SetNormalImage(TEXT("file='RADIO_NORMAL' restype='PNG'"));
	m_pRadioByUserID->SetCheckedImage(TEXT("file='RADIO_CHECKED' restype='PNG'"));
	m_pRadioByUserID->SetTextPadding(CRect(12,1,0,0));
	m_pRadioByUserID->SetText(TEXT(" 依据 I D"));	
	m_pRadioByUserID->SetFixedWidth(70);
	m_pRadioByUserID->SetFixedHeight(12);
	m_pRadioByUserID->SetPos(164,62);
	m_pRadioByUserID->SetGrouped(true);
	m_pRadioByUserID->SetGroupID(1);

	//单选按钮
	m_pRadioByNickName = CRadioButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pRadioByNickName->SetNormalImage(TEXT("file='RADIO_NORMAL' restype='PNG'"));
	m_pRadioByNickName->SetCheckedImage(TEXT("file='RADIO_CHECKED' restype='PNG'"));
	m_pRadioByNickName->SetTextPadding(CRect(12,1,0,0));
	m_pRadioByNickName->SetText(TEXT(" 依据昵称"));
	m_pRadioByNickName->SetFixedWidth(70);
	m_pRadioByNickName->SetFixedHeight(12);
	m_pRadioByNickName->SetPos(252,62);
	m_pRadioByNickName->SetGrouped(true);
	m_pRadioByNickName->SetGroupID(1);
	m_pRadioByNickName->SetCheck(true);

	//确认按钮
	m_pButtonConfirm = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonConfirm->SetStatusImage(TEXT("file='BT_EXCHANGE_CONFIRM' restype='PNG'"));	
	m_pButtonConfirm->SetFixedXY(CPoint(176,213));
	m_pButtonConfirm->SetWantReturn();
}

//消息提醒
VOID CDlgInsureTransfer::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{		
		if(pControlUI==m_pButtonConfirm) return OnBnClickedTransferScore();
		if(pControlUI==m_pRadioByUserID) return OnBnCheckedTransferByUserID();
		if(pControlUI==m_pRadioByNickName) return OnBnCheckedTransferByNickName();
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{
		if(pControlUI==m_pEditAmount) return OnEnChangeScore();
	}
}

//分数变更
VOID CDlgInsureTransfer::OnEnChangeScore()
{
	//状态变量
	if (m_bSwitchIng==false)
	{
		//设置变量
		m_bSwitchIng=true;

		//变量定义
		SCORE lScore=CDlgInsureItem::GetUserControlScore(m_pEditAmount->GetText());

		//输入限制
		if(lScore > m_pUserInsureInfo->lUserInsure) lScore = m_pUserInsureInfo->lUserInsure;

		//设置界面
		if (lScore>0L)
		{
			//转换逗号
			TCHAR szControl[128]=TEXT("");
			CDlgInsureItem::SwitchScoreFormat(lScore,3L,szControl,CountArray(szControl));

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

//转帐按钮
VOID CDlgInsureTransfer::OnBnClickedTransferScore()
{
	//条件判断
	if (m_pUserInsureInfo->lUserInsure < m_pUserInsureInfo->lTransferPrerequisite)
	{
		//提示消息
		TCHAR szTipInfo[128]={0};
		_sntprintf(szTipInfo,CountArray(szTipInfo),TEXT("您银行游戏币的数目余额不足最低转帐条件 %I64d，无法进行转帐操作！"),m_pUserInsureInfo->lTransferPrerequisite);
		CInformation Information(this);
		Information.ShowMessageBox(szTipInfo,MB_ICONERROR,30);

		return;
	}

	//转帐金额
	SCORE lTransferScore= CDlgInsureItem::GetUserControlScore(m_pEditAmount->GetText());

	//转帐备注
	TCHAR szTransRemark[LEN_TRANS_REMARK]=TEXT("");
	lstrcpyn(szTransRemark,m_pEditRemarks->GetText(),CountArray(szTransRemark));

	//获取密码
	TCHAR szPassword[LEN_PASSWORD];
	lstrcpyn(szPassword,m_pEditInsurePass->GetText(),CountArray(szPassword));

	//加密密码
	TCHAR szInsurePass[LEN_MD5];
	CWHEncrypt::MD5Encrypt(szPassword,szInsurePass);

	//获取昵称
	CString strAccounts = m_pEditAccount->GetText();

	//格式昵称
	strAccounts.TrimLeft();
	strAccounts.TrimRight();

	//昵称判断
	if (m_pRadioByNickName->GetCheck()==true && strAccounts.GetLength()==0)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请输入要赠送的玩家昵称！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditAccount->SetFocus();

		return;
	}

	//ID号判断
	if(m_pRadioByUserID->GetCheck()==true && strAccounts.GetLength()==0)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请输入要赠送的玩家ID！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditAccount->SetFocus();

		return;
	}

	//数据判断
	if (lTransferScore<=0L)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("赠送的游戏币数量不能为空，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditAmount->SetFocus();

		return;
	}

	//数目判断
	if (lTransferScore>m_pUserInsureInfo->lUserInsure)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您银行游戏币的数目余额不足，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditAmount->SetFocus();

		return;
	}

	//数目判断
	if (lTransferScore<m_pUserInsureInfo->lTransferPrerequisite)
	{
		//提示消息
		TCHAR szTipInfo[128]={0};
		_sntprintf(szTipInfo,CountArray(szTipInfo),TEXT("从银行转帐的游戏币数目不能少于 %I64d，无法进行转帐操作！"),m_pUserInsureInfo->lTransferPrerequisite);
		CInformation Information(this);
		Information.ShowMessageBox(szTipInfo,MB_ICONERROR,30);

		//设置焦点
		m_pEditAmount->SetFocus();

		return;
	}

	//密码判断
	if (szPassword[0]==0)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("银行密码不能为空，请重新输入银行密码！"),MB_ICONERROR,30);

		//设置焦点
		m_pEditInsurePass->SetFocus();

		return;
	}
	
	//事件通知
	m_pDlgInsureMain->OnMissionStart();
	m_pDlgInsureMain->PerformTransferScore(strAccounts,lTransferScore,szInsurePass,szTransRemark,m_pRadioByUserID->GetCheck()?FALSE:TRUE);

	//设置焦点
	m_pEditInsurePass->SetFocus();
	m_pEditInsurePass->SetText(TEXT(""));

	return;
}

//依据ID
VOID CDlgInsureTransfer::OnBnCheckedTransferByUserID()
{
	//设置控件
	m_pEditAccount->SetText(TEXT(""));
	m_pEditAccount->SetOnlyNumber(true);
	m_pLabelTarget->SetText(TEXT("接收 I D："));		
}

//依据昵称
VOID CDlgInsureTransfer::OnBnCheckedTransferByNickName()
{
	//设置控件
	m_pEditAccount->SetText(TEXT(""));
	m_pEditAccount->SetOnlyNumber(false);
	m_pLabelTarget->SetText(TEXT("接收昵称："));	
}

//////////////////////////////////////////////////////////////////////////////////
//构造函数
CDlgInsureRecord::CDlgInsureRecord()
{
}

//析构函数
CDlgInsureRecord::~CDlgInsureRecord()
{
}

//开始绘画
VOID CDlgInsureRecord::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置环境
	pDC->SetBkMode(TRANSPARENT);

	//绘画背景
	pDC->FillSolidRect(&rcClient,m_pDlgInsureOperater->GetItemBackColor());
}

//刷新纪录
VOID CDlgInsureRecord::RefreshInsureRecord()
{
	CWebBrowser * pWebBrowser=m_PublicizeRecord.GetWebBrowser();
	if (pWebBrowser!=NULL) pWebBrowser->Refresh();
}

//创建消息
int CDlgInsureRecord::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) return -1;

	//创建控件
	CRect rcCreate(0,0,0,0);
	m_PublicizeRecord.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_PULICIZE_RECORD);
	m_PublicizeRecord.SetBackGroundColor(m_pDlgInsureOperater->GetItemBackColor());	

	//构造地址
	TCHAR szBillUrl[256]=TEXT("");

	//获取对象
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//操作记录
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/%sUserService/UserService7.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());
	m_PublicizeRecord.Navigate(szBillUrl);	
	
	return 0;
}

//调整大小
VOID CDlgInsureRecord::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//设置控件
	m_PublicizeRecord.SetWindowPos(NULL,0,0,cx,cy,SWP_NOZORDER);
}

//////////////////////////////////////////////////////////////////////////////////
//构造函数
CDlgInsureOperater::CDlgInsureOperater()
{
	//选择控件
	m_cbInsureMode=INSURE_SAVE;
	m_cbInsureMask=INSURE_MASK_SAVE|INSURE_MASK_RECORD;
	ZeroMemory(m_TabButton,sizeof(m_TabButton));

	//控件变量
	m_pLabelRound=NULL;

	//设置变量
	m_pUserInsureInfo=NULL;
}

//析构函数
CDlgInsureOperater::~CDlgInsureOperater()
{
}

//开始绘画
VOID CDlgInsureOperater::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画背景
	pDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);
}

//消息提醒
VOID CDlgInsureOperater::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(pControlUI==m_TabButton[INSURE_SAVE]) 
		{
			return SwitchInsureModeView(INSURE_SAVE);
		}
		else if(pControlUI==m_TabButton[INSURE_TRANSFER]) 
		{
			return SwitchInsureModeView(INSURE_TRANSFER);
		}
		else if(pControlUI==m_TabButton[INSURE_RECORD]) 
		{
			return SwitchInsureModeView(INSURE_RECORD);
		}
	}
}

//初始控件
VOID CDlgInsureOperater::InitControlUI()
{
	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//背景环绕
	m_pLabelRound = CLabelUI::Create(&m_PaintManager,pParent,TEXT("")); 
	m_pLabelRound->SetBkImage(TEXT("file='IMAGE_ROUND_FRAME1' restype='PNG' corner='6,6,6,6'"));
	m_pLabelRound->SetFixedWidth(452);
	m_pLabelRound->SetFixedHeight(275);
	m_pLabelRound->SetPos(CRect(4,35,456,310));

	//游戏币存取
	m_TabButton[0] = CTabButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_TabButton[0]->SetStatusImage(TEXT("file='TAB_INSURE_SAVE' restype='PNG'"));
	m_TabButton[0]->SetGrouped();
	m_TabButton[0]->SetVisible(false);

	//游戏币转帐
	m_TabButton[1] = CTabButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_TabButton[1]->SetStatusImage(TEXT("file='TAB_INSURE_TRANSFER' restype='PNG'"));
	m_TabButton[1]->SetGrouped();
	m_TabButton[1]->SetVisible(false);

	//操作记录
	m_TabButton[2] = CTabButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_TabButton[2]->SetStatusImage(TEXT("file='TAB_INSURE_RECORD' restype='PNG'"));
	m_TabButton[2]->SetGrouped();
	m_TabButton[2]->SetVisible(false);

	//调整子项
	RectifyTabItem();
}

//调整子项
VOID CDlgInsureOperater::RectifyTabItem()
{
	//获取变量
	CRect rcRound = m_pLabelRound->GetPos();
	CSize SizeItem(m_TabButton[0]->GetFixedWidth(),m_TabButton[0]->GetFixedHeight());

	//变量定义
	INT nXPos = rcRound.left+20;
	INT nYPos = rcRound.top-SizeItem.cy+1;
	INT nXSpace = 10;

	//游戏币存取
	m_TabButton[0]->SetVisible(false);
	if(m_cbInsureMask&INSURE_MASK_SAVE)
	{
		//设置按钮
		m_TabButton[0]->SetVisible();
		m_TabButton[0]->SetPos(nXPos,nYPos);

		//调整位置
		nXPos += SizeItem.cx+nXSpace;
	}

	//游戏币转帐
	m_TabButton[1]->SetVisible(false);
	if(m_cbInsureMask&INSURE_MASK_TRANSFER)
	{
		//设置按钮
		m_TabButton[1]->SetVisible();
		m_TabButton[1]->SetPos(nXPos,nYPos);

		//调整位置
		nXPos += SizeItem.cx+nXSpace;
	}

	//操作记录
	m_TabButton[2]->SetVisible(false);
	if(m_cbInsureMask&INSURE_MASK_RECORD)
	{
		//设置按钮
		m_TabButton[2]->SetVisible();
		m_TabButton[2]->SetPos(nXPos,nYPos);

		//调整位置
		nXPos += SizeItem.cx+nXSpace;
	}

	return;
}

//插入子项
VOID CDlgInsureOperater::InsertTabItem(BYTE cbItemMask)
{
	//参数判断
	if(m_cbInsureMask&cbItemMask) return;

	//设置变量
	m_cbInsureMask |= cbItemMask;

	//调整子项
	RectifyTabItem();
}

//删除子项
VOID CDlgInsureOperater::DeleteTabItem(BYTE cbItemMask)
{
	if((m_cbInsureMask&cbItemMask)==0) return;

	//设置变量
	m_cbInsureMask &= ~cbItemMask;

	//调整子项
	RectifyTabItem();
}

//设置模式
VOID CDlgInsureOperater::SwitchInsureModeView(BYTE cbInsureMode)
{
	switch (cbInsureMode)
	{
	case INSURE_SAVE:			//存取方式
		{
			//设置变量
			m_cbInsureMode=INSURE_SAVE;

			//设置控件
			if (m_DlgInsureSave.m_hWnd==NULL)
			{
				//获取位置
				CRect rcClient;
				GetClientRect(&rcClient);

				//创建控件
				CRect rcCreate(0,0,0,0);
				m_DlgInsureSave.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_INSRUE_SAVE);

				//移动位置
				CRect rcRound = m_pLabelRound->GetPos();
				m_DlgInsureSave.SetWindowPos(NULL,rcRound.left+5,rcRound.top+5,rcRound.Width()-10,rcRound.Height()-10,SWP_NOZORDER|SWP_SHOWWINDOW);
			}
			else
			{
				//显示窗口
				m_DlgInsureSave.ShowWindow(SW_SHOW);
			}

			//设置控件
			if (m_DlgInsureTransfer.m_hWnd!=NULL) m_DlgInsureTransfer.ShowWindow(SW_HIDE);
			if (m_DlgInsureRecord.m_hWnd!=NULL) m_DlgInsureRecord.ShowWindow(SW_HIDE);	

			//设置焦点
			m_DlgInsureSave.SetFocus();

			//设置选择
			m_TabButton[m_cbInsureMode]->SetCheck();

			//刷新界面
			InvalidateRect(NULL);

			break;
		}
	case INSURE_TRANSFER:		//转帐方式
		{
			//设置变量
			m_cbInsureMode=INSURE_TRANSFER;

			//设置控件
			if (m_DlgInsureTransfer.m_hWnd==NULL)
			{
				//获取位置
				CRect rcClient;
				GetClientRect(&rcClient);

				//创建控件
				CRect rcCreate(0,0,0,0);
				m_DlgInsureTransfer.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_INSURE_TRANSFER);

				//移动位置
				CRect rcRound = m_pLabelRound->GetPos();
				m_DlgInsureTransfer.SetWindowPos(NULL,rcRound.left+5,rcRound.top+5,rcRound.Width()-10,rcRound.Height()-10,SWP_NOZORDER|SWP_SHOWWINDOW);
			}
			else
			{
				//显示窗口
				m_DlgInsureTransfer.ShowWindow(SW_SHOW);
			}

			//设置控件
			if (m_DlgInsureSave.m_hWnd!=NULL) m_DlgInsureSave.ShowWindow(SW_HIDE);
			if (m_DlgInsureRecord.m_hWnd!=NULL) m_DlgInsureRecord.ShowWindow(SW_HIDE);			

			//设置焦点
			m_DlgInsureTransfer.SetFocus();

			//设置选择
			m_TabButton[m_cbInsureMode]->SetCheck();

			//刷新界面
			InvalidateRect(NULL);

			break;
		}
	case INSURE_RECORD:		//操作记录
		{
			//设置变量
			m_cbInsureMode=INSURE_RECORD;

			//设置控件
			if (m_DlgInsureRecord.m_hWnd==NULL)
			{
				//获取位置
				CRect rcClient;
				GetClientRect(&rcClient);

				//创建控件
				CRect rcCreate(0,0,0,0);
				m_DlgInsureRecord.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_INSURE_RECORD);

				//移动位置
				CRect rcRound = m_pLabelRound->GetPos();
				m_DlgInsureRecord.SetWindowPos(NULL,rcRound.left+5,rcRound.top+5,rcRound.Width()-10,rcRound.Height()-10,SWP_NOZORDER|SWP_SHOWWINDOW);
			}
			else
			{
				//显示窗口				
				m_DlgInsureRecord.ShowWindow(SW_SHOW);				
				m_DlgInsureRecord.RefreshInsureRecord();
			}

			//设置控件
			if (m_DlgInsureSave.m_hWnd!=NULL) m_DlgInsureSave.ShowWindow(SW_HIDE);
			if (m_DlgInsureTransfer.m_hWnd!=NULL) m_DlgInsureTransfer.ShowWindow(SW_HIDE);

			//设置焦点
			m_DlgInsureRecord.SetFocus();			

			//设置选择
			m_TabButton[m_cbInsureMode]->SetCheck();

			//刷新界面
			InvalidateRect(NULL);

			break;			
		};		
	default:
		{
			ASSERT(FALSE);
		}
	}

	return;
}

//设置对象
VOID CDlgInsureOperater::SetDlgInsureMain(CDlgInsureMain * pDlgInsureMain)
{
	//设置对象
	m_pDlgInsureMain=pDlgInsureMain;
	m_DlgInsureSave.m_pDlgInsureMain=m_pDlgInsureMain;
	m_DlgInsureTransfer.m_pDlgInsureMain=m_pDlgInsureMain;
}

//设置信息
VOID CDlgInsureOperater::SetUserInsureInfo(tagUserInsureInfo * pUserInsureInfo)
{
	//设置对象
	m_pUserInsureInfo=pUserInsureInfo;
	m_DlgInsureSave.m_pUserInsureInfo=m_pUserInsureInfo;
	m_DlgInsureTransfer.m_pUserInsureInfo=m_pUserInsureInfo;
}

//查询按钮
VOID CDlgInsureOperater::OnBnClickedQueryInfo()
{
	//查询信息
	m_pDlgInsureMain->QueryUserInsureInfo();

	//设置焦点
	switch (m_cbInsureMode)
	{
	case INSURE_SAVE:		//存取方式
		{
			m_DlgInsureSave.SetFocus();
			break;
		}
	case INSURE_TRANSFER:	//转帐方式
		{
			m_DlgInsureTransfer.SetFocus();
			break;
		}
	}

	return;
}

//创建消息
int CDlgInsureOperater::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) return -1;

	//设置控件	
	m_DlgInsureSave.m_pDlgInsureOperater=this;
	m_DlgInsureRecord.m_pDlgInsureOperater=this;
	m_DlgInsureTransfer.m_pDlgInsureOperater=this;

	//获取大小
	CRect rcClient;
	GetClientRect(&rcClient);

	//加载资源
	CPngImage ImageInsureInfo;
	ImageInsureInfo.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("INFO_FRAME"));

	//绘画信息
	INT nYPosInfo=86L;
	INT nXPosInfo=(rcClient.Width()-ImageInsureInfo.GetWidth())/2L+350;

	//存取模式
	SwitchInsureModeView(INSURE_SAVE);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
//构造函数
CDlgInsureSetup::CDlgInsureSetup()
{
	//设置变量
	m_pDlgInsureMain=NULL;

	//控件指针
	m_pLabelRound=NULL;
	m_pEditLogonPass=NULL;
	m_pEditInsurePass=NULL;
	m_pEditInsurePass1=NULL;
	m_pButtonEnable=NULL;
}

//析构函数
CDlgInsureSetup::~CDlgInsureSetup()
{
}

//开始绘画
VOID CDlgInsureSetup::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画背景
	pDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);
}

//初始控件
VOID CDlgInsureSetup::InitControlUI()
{
	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//变量定义
	CLabelUI * pLabelControl = NULL;

	//背景环绕
	m_pLabelRound = CLabelUI::Create(&m_PaintManager,pParent,TEXT("")); 
	m_pLabelRound->SetBkImage(TEXT("file='IMAGE_ROUND_FRAME' restype='PNG' corner='6,6,6,6'"));
	m_pLabelRound->SetFixedWidth(451);
	m_pLabelRound->SetFixedHeight(306);
	m_pLabelRound->SetPos(4,4);	

	//标签文本
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("温馨提示："));
	pLabelControl->SetFixedWidth(62);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetPos(69,64);

	//标签文本
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("请先设置银行密码，以便保护您的个人财产安全"));
	pLabelControl->SetFixedWidth(300);
	pLabelControl->SetFixedHeight(18);
	pLabelControl->SetPos(99,85);

	//标签文本
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("设置银行密码："));
	pLabelControl->SetFixedWidth(84);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetPos(99,120);

	//标签文本
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("确定银行密码："));
	pLabelControl->SetFixedWidth(84);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetPos(99,150);

	//标签文本
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("平台登录密码："));
	pLabelControl->SetFixedWidth(84);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetPos(99,180);	

	//银行密码
	m_pEditInsurePass = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditInsurePass->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG'"));
	m_pEditInsurePass->SetPasswordMode(true);
	m_pEditInsurePass->SetMaxChar(LEN_PASSWORD-1);
	m_pEditInsurePass->SetFixedXY(CPoint(185,114));
	m_pEditInsurePass->SetFixedWidth(170);
	m_pEditInsurePass->SetFixedHeight(24);		
	m_pEditInsurePass->SetBkColor(0);

	//银行密码
	m_pEditInsurePass1 = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditInsurePass1->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG'"));
	m_pEditInsurePass1->SetPasswordMode(true);
	m_pEditInsurePass1->SetMaxChar(LEN_PASSWORD-1);
	m_pEditInsurePass1->SetFixedXY(CPoint(185,144));
	m_pEditInsurePass1->SetFixedWidth(170);
	m_pEditInsurePass1->SetFixedHeight(24);	
	m_pEditInsurePass1->SetBkColor(0);

	//登录密码
	m_pEditLogonPass = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditLogonPass->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG'"));
	m_pEditLogonPass->SetPasswordMode(true);
	m_pEditLogonPass->SetMaxChar(LEN_PASSWORD-1);
	m_pEditLogonPass->SetFixedXY(CPoint(185,174));
	m_pEditLogonPass->SetFixedWidth(170);
	m_pEditLogonPass->SetFixedHeight(24);		
	m_pEditLogonPass->SetBkColor(0);

	//开通银行
	m_pButtonEnable = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonEnable->SetStatusImage(TEXT("file='BT_OPEN_INSURE' restype='PNG'"));	
	m_pButtonEnable->SetFixedXY(CPoint(154,224));
	m_pButtonEnable->SetWantReturn();
}

//消息提醒
VOID CDlgInsureSetup::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{		
		if(pControlUI==m_pButtonEnable) return OnBnClickedEnableInsure();
	}
}

//开通银行
VOID CDlgInsureSetup::OnBnClickedEnableInsure()
{
	//密码信息
	TCHAR szLogonPass[LEN_PASSWORD]=TEXT("");
	TCHAR szInsurePass1[LEN_PASSWORD]=TEXT("");
	TCHAR szInsurePass2[LEN_PASSWORD]=TEXT("");

	//获取密码
	lstrcpyn(szLogonPass,m_pEditLogonPass->GetText(),CountArray(szLogonPass));
	lstrcpyn(szInsurePass1,m_pEditInsurePass->GetText(),CountArray(szInsurePass1));
	lstrcpyn(szInsurePass2,m_pEditInsurePass1->GetText(),CountArray(szInsurePass2));

	//变量定义
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//密码判断
	if (pUserItemElement->EfficacyPassword(szInsurePass1,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		m_pEditInsurePass->SetFocus();

		return;
	}

	//一致判断
	if(lstrcmp(szInsurePass1,szInsurePass2)!=0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您两次输入的密码不一致，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		m_pEditInsurePass1->SetFocus();

		return;
	}

	//登录密码
	if(szLogonPass[0] == 0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请输入平台登录密码！"),MB_ICONERROR,0);

		//设置焦点
		m_pEditLogonPass->SetFocus();
		
		return;
	}

	//密码加密
	TCHAR szEnciryptInsurePass[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szInsurePass1,szEnciryptInsurePass);

	//密码加密
	TCHAR szEnciryptLogonPass[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szLogonPass,szEnciryptLogonPass);

	//执行开通
	if(m_pDlgInsureMain!=NULL) m_pDlgInsureMain->PerformEnableInsure(szEnciryptInsurePass,szEnciryptLogonPass);
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgInsureMain::CDlgInsureMain() : CSkinDialog(IDD_INSURE_MAIN)
{
	//服务管理
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bEnableMission=false;

	//设置变量
	m_bInitInfo=false;
	ZeroMemory(&m_UserInsureInfo,sizeof(m_UserInsureInfo));

	return;
}

//析构函数
CDlgInsureMain::~CDlgInsureMain()
{
}

//控件绑定
VOID CDlgInsureMain::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//消息解释
BOOL CDlgInsureMain::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if (pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)	return TRUE;
		if(pMsg->wParam==VK_RETURN)	return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//创建函数
BOOL CDlgInsureMain::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("银行"));

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//设置字体
	CSkinDialog::SetWndFont(this,NULL);

	//变量定义
	CRect rcCreate(0,0,0,0);

	//创建窗口
	m_DlgInsureSetup.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_INSURE_SETUP);
	m_DlgInsureOperater.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_INSURE_OPERATER);

	//设置对象
	m_DlgInsureSetup.SetDlgInsureMain(this);
	m_DlgInsureOperater.SetDlgInsureMain(this);
	m_DlgInsureOperater.SetUserInsureInfo(&m_UserInsureInfo);

	//设置窗口
	SetWindowPos(NULL,0,0,471,362,SWP_NOZORDER|SWP_NOMOVE);

	//调整步骤
	RectifyInsureStep();

	return FALSE;
}

//确定按钮
VOID CDlgInsureMain::OnOK()
{
	return __super::OnOK();
}

//绘画标题
VOID CDlgInsureMain::OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight)
{
	//加载资源
	CPngImage ImageTitle;
	ImageTitle.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("INSURE_TITLE"));

	//绘制标题
	ImageTitle.DrawImage(pDC,10,10);
}

//绘画消息
VOID CDlgInsureMain::OnDrawClientAreaBegin(CDC * pDC, INT nWidth, INT nHeight)
{
	//获取位置
	CRect rcTabControl;
	//m_TabControl.GetWindowRect(&rcTabControl);

	return;

	//转换位置
	ScreenToClient(&rcTabControl);

	//绘画线条
	INT nLBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;
	pDC->FillSolidRect(nLBorder,rcTabControl.bottom-1L,nWidth-nLBorder-nRBorder,1,RGB(145,168,183));

	//加载资源
	CPngImage ImageInsureInfo;
	ImageInsureInfo.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("INFO_FRAME"));

	//绘画信息
	INT nYPosInfo=75L;
	INT nXPosInfo=(nWidth-ImageInsureInfo.GetWidth())/2L;
	ImageInsureInfo.DrawImage(pDC,nXPosInfo,nYPosInfo,ImageInsureInfo.GetWidth(),ImageInsureInfo.GetHeight(),0,0);

	//银行信息
	//if (m_bInitInfo==true)
	//{
	//	//加载资源
	//	CPngImage ImageInsureIcon;
	//	ImageInsureIcon.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("INSURE_ICON"));

	//	//绘画标志
	//	ImageInsureIcon.DrawImage(pDC,nXPosInfo+10,nYPosInfo+17);

	//	//绘画数字
	//	m_DlgInsureItem.DrawNumberString(pDC,m_UserInsureInfo.lUserScore,nXPosInfo+136,nYPosInfo+18L);
	//	m_DlgInsureItem.DrawNumberString(pDC,m_UserInsureInfo.lUserInsure,nXPosInfo+136,nYPosInfo+53L);

	//	//绘画提示
	//	LPCTSTR pszScoreString=TEXT("游戏币：");
	//	LPCTSTR pszInsureString=TEXT("保险柜：");
	//	pDC->TextOut(nXPosInfo+83,nYPosInfo+20L,pszScoreString,lstrlen(pszScoreString));
	//	pDC->TextOut(nXPosInfo+83,nYPosInfo+55L,pszInsureString,lstrlen(pszInsureString));

	//	//构造提示
	//	TCHAR szString[128]=TEXT("");
	//	if(m_cbInsureMode==INSURE_SAVE)
	//	{
	//		_sntprintf(szString,CountArray(szString),TEXT("温馨提示：存入游戏币免手续费，取出将扣除%ld‰的手续费"),			m_UserInsureInfo.wRevenueTake);
	//	}
	//	else if(m_cbInsureMode==INSURE_TRANSFER)
	//	{
	//		_sntprintf(szString,CountArray(szString),TEXT("温馨提示：赠送将扣除%ld‰的手续费"),m_UserInsureInfo.wRevenueTransfer);
	//	}

	//	//温馨提示
	//	pDC->SetTextColor(RGB(50,50,50));
	//	pDC->TextOut(18,nHeight-28,szString,lstrlen(szString));
	//}
	//else
	//{
	//}

	return;
}

//开始任务
VOID CDlgInsureMain::OnMissionStart()
{
	return;
}

//终止任务
VOID CDlgInsureMain::OnMissionConclude()
{
	return;
}

//调整步骤
VOID CDlgInsureMain::RectifyInsureStep()
{
	//获取区域
	CRect rcClient;
	GetClientRect(&rcClient);

	//环绕信息
	tagEncircleInfo EncircleInfoFrame = CSkinDialog::m_SkinAttribute.m_EncircleInfoFrame;

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//调整准备
	UINT nFlagEx=0;
	HDWP hDwp = BeginDeferWindowPos(32);	

	//设置银行
	nFlagEx = pGlobalUserData->cbInsureEnabled==TRUE?SWP_HIDEWINDOW:SWP_SHOWWINDOW;
	DeferWindowPos(hDwp,m_DlgInsureSetup,NULL,EncircleInfoFrame.nLBorder,EncircleInfoFrame.nTBorder,rcClient.Width()-EncircleInfoFrame.nLBorder-EncircleInfoFrame.nRBorder,rcClient.Height()-EncircleInfoFrame.nTBorder-EncircleInfoFrame.nBBorder,SWP_NOZORDER|nFlagEx);

	//操作银行
	nFlagEx = pGlobalUserData->cbInsureEnabled==FALSE?SWP_HIDEWINDOW:SWP_SHOWWINDOW;
	DeferWindowPos(hDwp,m_DlgInsureOperater,NULL,EncircleInfoFrame.nLBorder,EncircleInfoFrame.nTBorder,rcClient.Width()-EncircleInfoFrame.nLBorder-EncircleInfoFrame.nRBorder,rcClient.Height()-EncircleInfoFrame.nTBorder-EncircleInfoFrame.nBBorder,SWP_NOZORDER|nFlagEx);

	//调整结束
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	//查询银行
	if(pGlobalUserData->cbInsureEnabled==TRUE) QueryUserInsureInfo();
}

//查询资料
VOID CDlgInsureMain::QueryUserInsureInfo()
{
	//设置变量
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=true;
	m_bTransferMission=false;
	m_bEnableMission=false;

	//事件处理
	OnMissionStart();

	//执行查询
	PerformQueryInfo();

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgInsurePlaza::CDlgInsurePlaza()
{
	//设置任务
	m_MissionManager.InsertMissionItem(this);

	//设置变量
	m_lScore=0L;
	m_cbByNickName=TRUE;
	
	//设置变量
	m_szAccounts[0]=0;
	m_szLogonPass[0]=0;
	m_szInsurePass[0]=0;
	m_szTransRemark[0]=0;

	return;
}

//析构函数
CDlgInsurePlaza::~CDlgInsurePlaza()
{
}

//查询事件
VOID CDlgInsurePlaza::PerformQueryInfo()
{
	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//开通事件
VOID CDlgInsurePlaza::PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass)
{
	//设置变量
	m_bEnableMission=true;
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bTransferMission=false;

	//设置变量
	lstrcpyn(m_szLogonPass,pszLogonPass,CountArray(m_szLogonPass));
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));	

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//存入事件
VOID CDlgInsurePlaza::PerformSaveScore(SCORE lSaveScore)
{
	//设置变量	
	m_bSaveMission=true;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bEnableMission=false;
	m_bTransferMission=false;

	//设置变量
	m_lScore=lSaveScore;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//取出事件
VOID CDlgInsurePlaza::PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)
{
	//设置变量	
	m_bTakeMission=true;
	m_bSaveMission=false;	
	m_bQueryMission=false;
	m_bEnableMission=false;
	m_bTransferMission=false;

	//设置变量
	m_lScore=lTakeScore;
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//转帐事件
VOID CDlgInsurePlaza::PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName)
{
	//设置变量
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bEnableMission=false;
	m_bTransferMission=true;

	//设置变量
	m_lScore=lTransferScore;
	m_cbByNickName=cbByNickName;
	lstrcpyn(m_szAccounts,pszAccounts,CountArray(m_szAccounts));
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));
	lstrcpyn(m_szTransRemark,pszTransRemark,CountArray(m_szTransRemark));

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//连接事件
bool CDlgInsurePlaza::OnEventMissionLink(INT nErrorCode)
{
	//连接结果
	if (nErrorCode!=0)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//事件处理
		OnMissionConclude();

		//错误提示
		if (m_bEnableMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，开通银行失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}

		//错误提示
		if (m_bSaveMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，游戏币存入失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}

		//错误提示
		if (m_bTakeMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，游戏币取出失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}

		//错误提示
		if (m_bQueryMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，银行资料获取失败！"),MB_OK|MB_ICONSTOP,30);
		}

		//错误提示
		if (m_bTransferMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，游戏币赠送失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}

		return true;
	}

	//开通任务
	if (m_bEnableMission==true)
	{
		//变量定义
		CMD_GP_UserEnableInsure UserEnableInsure;
		ZeroMemory(&UserEnableInsure,sizeof(UserEnableInsure));

		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//设置变量
		UserEnableInsure.dwUserID=pGlobalUserData->dwUserID;
		lstrcpyn(UserEnableInsure.szLogonPass,m_szLogonPass,CountArray(UserEnableInsure.szLogonPass));
		lstrcpyn(UserEnableInsure.szInsurePass,m_szInsurePass,CountArray(UserEnableInsure.szInsurePass));		
		CWHService::GetMachineIDEx(UserEnableInsure.szMachineID);

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_ENABLE_INSURE,&UserEnableInsure,sizeof(UserEnableInsure));

		return true;
	}

	//存入任务
	if (m_bSaveMission==true)
	{
		//变量定义
		CMD_GP_UserSaveScore UserSaveScore;
		ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//设置变量
		UserSaveScore.lSaveScore=m_lScore;
		UserSaveScore.dwUserID=pGlobalUserData->dwUserID;
		CWHService::GetMachineIDEx(UserSaveScore.szMachineID);

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_SAVE_SCORE,&UserSaveScore,sizeof(UserSaveScore));

		return true;
	}

	//取出任务
	if (m_bTakeMission==true)
	{
		//变量定义
		CMD_GP_UserTakeScore UserTakeScore;
		ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//设置变量
		UserTakeScore.lTakeScore=m_lScore;
		UserTakeScore.dwUserID=pGlobalUserData->dwUserID;
		CWHService::GetMachineIDEx(UserTakeScore.szMachineID);
		lstrcpyn(UserTakeScore.szPassword,m_szInsurePass,CountArray(UserTakeScore.szPassword));

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_TAKE_SCORE,&UserTakeScore,sizeof(UserTakeScore));

		return true;
	}

	//转帐任务
	if (m_bTransferMission==true)
	{
		//变量定义
		CMD_GP_QueryUserInfoRequest QueryUserInfoRequest;
		ZeroMemory(&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));

		//设置变量
		QueryUserInfoRequest.cbByNickName=m_cbByNickName;
		lstrcpyn(QueryUserInfoRequest.szAccounts,m_szAccounts,CountArray(QueryUserInfoRequest.szAccounts));

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_USER_INFO_REQUEST,&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));
	}

	//查询任务
	if (m_bQueryMission==true)
	{
		//变量定义
		CMD_GP_QueryInsureInfo QueryInsureInfo;
		ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

		//设置变量
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		QueryInsureInfo.dwUserID=pGlobalUserInfo->GetGlobalUserData()->dwUserID;
		lstrcpyn(QueryInsureInfo.szPassword,pGlobalUserInfo->GetGlobalUserData()->szPassword,CountArray(QueryInsureInfo.szPassword));

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));
	}

	return true;
}

//关闭事件
bool CDlgInsurePlaza::OnEventMissionShut(BYTE cbShutReason)
{
	//提示消息
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//错误提示
		if (m_bEnableMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，开通银行失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}

		//存入任务
		if (m_bSaveMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，游戏币存入失败！"),MB_ICONERROR,30);
		}

		//取出任务
		if (m_bTakeMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，游戏币取出失败！"),MB_ICONERROR,30);
		}

		//查询任务
		if (m_bQueryMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，银行资料获取失败！"),MB_ICONERROR,30);
		}
	}

	//事件处理
	OnMissionConclude();

	return true;
}

//读取事件
bool CDlgInsurePlaza::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_USER_INSURE_INFO:		//银行资料
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GP_UserInsureInfo));
				if (wDataSize<sizeof(CMD_GP_UserInsureInfo)) return false;

				//变量定义
				CMD_GP_UserInsureInfo * pUserInsureInfo=(CMD_GP_UserInsureInfo *)pData;

				//设置变量
				m_bInitInfo=true;
				m_UserInsureInfo.cbEnjoinTransfer=pUserInsureInfo->cbEnjoinTransfer;
				m_UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
				m_UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
				m_UserInsureInfo.wRevenueTransferMember=pUserInsureInfo->wRevenueTransferMember;
				m_UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
				m_UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
				m_UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//更新大厅
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//构造结构
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureInfo->lUserScore;
				InsureEvent.lUserInsure=pUserInsureInfo->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);

				//切换页面
				if(m_DlgInsureOperater.m_cbInsureMode==INSURE_TRANSFER && m_UserInsureInfo.cbEnjoinTransfer!=1)
				{
					m_DlgInsureOperater.SwitchInsureModeView(INSURE_SAVE);
				}

				//插入页面				
				if(m_UserInsureInfo.cbEnjoinTransfer==1)
				{
					m_DlgInsureOperater.InsertTabItem(INSURE_MASK_TRANSFER);
				}
				else
				{
					m_DlgInsureOperater.DeleteTabItem(INSURE_MASK_TRANSFER);
				}

				//更新界面
				m_DlgInsureOperater.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				return true;
			}
		case SUB_GP_USER_INSURE_ENABLE_RESULT:	//开通结果
			{
				//效验参数
				ASSERT(wDataSize<=sizeof(CMD_GP_UserInsureEnableResult));
				if (wDataSize>sizeof(CMD_GP_UserInsureEnableResult)) return false;

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//变量定义
				CMD_GP_UserInsureEnableResult * pUserInsureEnableResult=(CMD_GP_UserInsureEnableResult *)pData;

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//设置变量
				pGlobalUserData->cbInsureEnabled = pUserInsureEnableResult->cbInsureEnabled;

				//显示消息
				if (pUserInsureEnableResult->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureEnableResult->szDescribeString,MB_ICONINFORMATION,60);
				}

				//调整步骤
				RectifyInsureStep();

				return true;
			}
		case SUB_GP_USER_INSURE_SUCCESS:	//银行成功
			{
				//效验参数
				CMD_GP_UserInsureSuccess * pUserInsureSuccess=(CMD_GP_UserInsureSuccess *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

				//设置变量
				m_UserInsureInfo.lUserScore=pUserInsureSuccess->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureSuccess->lUserInsure;

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//设置焦点
				if(m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass != NULL)
				{
					m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetFocus();
					m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetText(TEXT(""));
				}

				//设置焦点
				if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
				{
					m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
					m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
				}

				//更新大厅
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//构造结构
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureSuccess->lUserScore;
				InsureEvent.lUserInsure=pUserInsureSuccess->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);				

				//更新界面
				m_DlgInsureOperater.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				//显示消息
				if (pUserInsureSuccess->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}				

				return true;
			}
		case SUB_GP_USER_INSURE_FAILURE:	//银行失败
			{
				//效验参数
				CMD_GP_UserInsureFailure * pUserInsureFailure=(CMD_GP_UserInsureFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

				//失败处理
				OnMissionConclude();

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//设置焦点
				if(m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass!= NULL)
				{
					m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetFocus();
					m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetText(TEXT(""));
				}

				//设置焦点
				if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
				{
					m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
					m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
				}

				//显示消息
				if (pUserInsureFailure->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		case SUB_GP_QUERY_USER_INFO_RESULT:	//用户信息
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GP_UserTransferUserInfo));
				if (wDataSize<sizeof(CMD_GP_UserTransferUserInfo)) return false;

				//变量定义
				CMD_GP_UserTransferUserInfo * pTransferUserInfo=(CMD_GP_UserTransferUserInfo *)pData;

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//变量定义
				WORD wRevinueTransfer=pGlobalUserData->cbMemberOrder?m_UserInsureInfo.wRevenueTransferMember:m_UserInsureInfo.wRevenueTransfer;

				//构造消息
				TCHAR szScore[16]={0};
				TCHAR szMessage[128]={0};	
				CDlgInsureItem::SwitchScoreFormat(m_lScore,3,szScore,CountArray(szScore));
				_sntprintf(szMessage, CountArray(szMessage), TEXT("赠送游戏币收取%d‰手续费,您确定要给[%s], ID:%d 赠送%s 游戏币吗?"), wRevinueTransfer,pTransferUserInfo->szAccounts, pTransferUserInfo->dwTargetGameID,szScore);

				//提示消息
				CInformation Information(this);
				if (Information.ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO,0)==IDYES)
				{
					//变量定义
					CMD_GP_UserTransferScore UserTransferScore;
					ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

					//变量定义
					CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

					//设置变量
					UserTransferScore.lTransferScore=m_lScore;
					UserTransferScore.dwUserID=pGlobalUserData->dwUserID;
					CWHService::GetMachineIDEx(UserTransferScore.szMachineID);					
					lstrcpyn(UserTransferScore.szPassword,m_szInsurePass,CountArray(UserTransferScore.szPassword));
					lstrcpyn(UserTransferScore.szTransRemark,m_szTransRemark,CountArray(UserTransferScore.szTransRemark));
					lstrcpyn(UserTransferScore.szAccounts,pTransferUserInfo->szAccounts,CountArray(UserTransferScore.szAccounts));

					//发送数据
					m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_TRANSFER_SCORE,&UserTransferScore,sizeof(UserTransferScore));
				}

				return true;
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgInsureServer::CDlgInsureServer()
{
	//设置变量
	m_cbByNickName=TRUE;
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;

	//设置变量
	m_szAccounts[0]=0;
	m_szLogonPass[0]=0;
	m_szInsurePass[0]=0;
	m_szTransRemark[0]=0;

	return;
}

//析构函数
CDlgInsureServer::~CDlgInsureServer()
{
}

//查询事件
VOID CDlgInsureServer::PerformQueryInfo()
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	CMD_GR_C_QueryInsureInfoRequest QueryInsureInfo;
	ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

	//设置变量
	QueryInsureInfo.cbActivityGame=FALSE;
	lstrcpyn(QueryInsureInfo.szInsurePass,pGlobalUserData->szPassword,CountArray(QueryInsureInfo.szInsurePass));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));

	return;
}

//开通事件
VOID CDlgInsureServer::PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass)
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	CMD_GR_C_EnableInsureRequest EnableInsureResult;
	ZeroMemory(&EnableInsureResult,sizeof(EnableInsureResult));

	//设置变量
	EnableInsureResult.cbActivityGame=FALSE;
	EnableInsureResult.dwUserID=pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(EnableInsureResult.szMachineID);
	lstrcpyn(EnableInsureResult.szLogonPass,pszLogonPass,CountArray(EnableInsureResult.szLogonPass));
	lstrcpyn(EnableInsureResult.szInsurePass,pszInsurePass,CountArray(EnableInsureResult.szInsurePass));		

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_ENABLE_INSURE_REQUEST,&EnableInsureResult,sizeof(EnableInsureResult));

	return;
}

//存入事件
VOID CDlgInsureServer::PerformSaveScore(SCORE lSaveScore)
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//变量定义
	CMD_GR_C_SaveScoreRequest SaveScoreRequest;
	ZeroMemory(&SaveScoreRequest,sizeof(SaveScoreRequest));

	//设置变量
	SaveScoreRequest.lSaveScore=lSaveScore;

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_SAVE_SCORE_REQUEST,&SaveScoreRequest,sizeof(SaveScoreRequest));

	return;
}

//取出事件
VOID CDlgInsureServer::PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//变量定义
	CMD_GR_C_TakeScoreRequest TakeScoreRequest;
	ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));

	//设置变量
	TakeScoreRequest.lTakeScore=lTakeScore;
	lstrcpyn(TakeScoreRequest.szInsurePass,pszInsurePass,CountArray(TakeScoreRequest.szInsurePass));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));

	return;
}

//转帐事件
VOID CDlgInsureServer::PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName)
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//设置变量
	m_lScore=lTransferScore;
	m_cbByNickName=cbByNickName;
	lstrcpyn(m_szAccounts,pszAccounts,CountArray(m_szAccounts));
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));
	lstrcpyn(m_szTransRemark,pszTransRemark,CountArray(m_szTransRemark));

	//变量定义
	CMD_GR_C_QueryUserInfoRequest QueryUserInfoRequest;	
	ZeroMemory(&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));

	//设置变量
	QueryUserInfoRequest.cbByNickName=m_cbByNickName;
	lstrcpyn(QueryUserInfoRequest.szAccounts,pszAccounts,CountArray(QueryUserInfoRequest.szAccounts));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_QUERY_USER_INFO_REQUEST,&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));

	return;
}

//配置银行
VOID CDlgInsureServer::Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem)
{
	//设置变量
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;

	return;
}

//银行消息
bool CDlgInsureServer::OnServerInsureMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//消息判断
	if (wMainCmdID==MDM_GR_INSURE)
	{
		switch (wSubCmdID)
		{
		case SUB_GR_USER_INSURE_INFO:		//银行资料
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GR_S_UserInsureInfo));
				if (wDataSize<sizeof(CMD_GR_S_UserInsureInfo)) return false;

				//变量定义
				CMD_GR_S_UserInsureInfo * pUserInsureInfo=(CMD_GR_S_UserInsureInfo *)pData;

				//设置变量
				m_bInitInfo=true;
				m_UserInsureInfo.cbEnjoinTransfer=pUserInsureInfo->cbEnjoinTransfer;
				m_UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
				m_UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
				m_UserInsureInfo.wRevenueTransferMember=pUserInsureInfo->wRevenueTransferMember;
				m_UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
				m_UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
				m_UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

				//更新大厅
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//构造结构
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureInfo->lUserScore;
				InsureEvent.lUserInsure=pUserInsureInfo->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);

				//切换页面
				if(m_DlgInsureOperater.m_cbInsureMode==INSURE_TRANSFER && m_UserInsureInfo.cbEnjoinTransfer!=1)
				{
					m_DlgInsureOperater.SwitchInsureModeView(INSURE_SAVE);
				}

				//插入页面				
				if(m_UserInsureInfo.cbEnjoinTransfer==1)
				{
					m_DlgInsureOperater.InsertTabItem(INSURE_MASK_TRANSFER);
				}
				else
				{
					m_DlgInsureOperater.DeleteTabItem(INSURE_MASK_TRANSFER);
				}

				//更新界面
				m_DlgInsureOperater.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				return true;
			}
		case SUB_GR_USER_INSURE_ENABLE_RESULT:	//开通结果
			{
				//效验参数
				ASSERT(wDataSize<=sizeof(CMD_GR_S_UserInsureEnableResult));
				if (wDataSize>sizeof(CMD_GR_S_UserInsureEnableResult)) return false;

				//变量定义
				CMD_GR_S_UserInsureEnableResult * pUserInsureEnableResult=(CMD_GR_S_UserInsureEnableResult *)pData;

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//设置变量
				pGlobalUserData->cbInsureEnabled = pUserInsureEnableResult->cbInsureEnabled;

				//显示消息
				if (pUserInsureEnableResult->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureEnableResult->szDescribeString,MB_ICONINFORMATION,60);
				}

				//调整步骤
				RectifyInsureStep();

				return true;
			}
		case SUB_GR_USER_INSURE_SUCCESS:	//银行成功
			{
				//效验参数
				CMD_GR_S_UserInsureSuccess * pUserInsureSuccess=(CMD_GR_S_UserInsureSuccess *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

				//设置变量
				m_UserInsureInfo.lUserScore=pUserInsureSuccess->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureSuccess->lUserInsure;

				//更新大厅
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//构造结构
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureSuccess->lUserScore;
				InsureEvent.lUserInsure=pUserInsureSuccess->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);

				//设置焦点
				if(m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass!= NULL)
				{
					m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetFocus();
					m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetText(TEXT(""));
				}

				//设置焦点
				if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
				{
					m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
					m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
				}

				//更新界面
				m_DlgInsureOperater.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				//显示消息
				if (pUserInsureSuccess->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}				

				return true;
			}
		case SUB_GR_USER_INSURE_FAILURE:	//银行失败
			{
				//效验参数
				CMD_GR_S_UserInsureFailure * pUserInsureFailure=(CMD_GR_S_UserInsureFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GR_S_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

				//失败处理
				OnMissionConclude();

				//设置控件
				if (m_DlgInsureOperater.m_bInitInfo==true)
				{
					//设置焦点
					if(m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass != NULL)
					{
						m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetFocus();
						m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetText(TEXT(""));
					}

					//设置焦点
					if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
					{
						m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
						m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
					}
				}

				//显示消息
				if (pUserInsureFailure->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		case SUB_GR_USER_TRANSFER_USER_INFO:  //用户信息
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GR_S_UserTransferUserInfo));
				if (wDataSize<sizeof(CMD_GR_S_UserTransferUserInfo)) return false;

				//变量定义
				CMD_GR_S_UserTransferUserInfo * pTransferUserInfo=(CMD_GR_S_UserTransferUserInfo *)pData;

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//变量定义
				WORD wRevinueTransfer=pGlobalUserData->cbMemberOrder?m_UserInsureInfo.wRevenueTransferMember:m_UserInsureInfo.wRevenueTransfer;

				//构造消息
				TCHAR szScore[16]={0};
				TCHAR szMessage[128]={0};	
				CDlgInsureItem::SwitchScoreFormat(m_lScore,3,szScore,CountArray(szScore));
				_sntprintf(szMessage, CountArray(szMessage), TEXT("赠送游戏币收取%d‰手续费,您确定要给[%s], ID:%d 赠送%s 游戏币吗?"), wRevinueTransfer,pTransferUserInfo->szAccounts, pTransferUserInfo->dwTargetGameID,szScore);

				//提示消息
				CInformation Information(this);
				if (Information.ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO,0)==IDYES)
				{
					//变量定义
					CMD_GP_C_TransferScoreRequest TransferScoreRequest;
					ZeroMemory(&TransferScoreRequest,sizeof(TransferScoreRequest));

					//设置变量
					TransferScoreRequest.lTransferScore=m_lScore;					
					lstrcpyn(TransferScoreRequest.szInsurePass,m_szInsurePass,CountArray(TransferScoreRequest.szInsurePass));
					lstrcpyn(TransferScoreRequest.szTransRemark,m_szTransRemark,CountArray(TransferScoreRequest.szTransRemark));
					lstrcpyn(TransferScoreRequest.szAccounts,pTransferUserInfo->szAccounts,CountArray(TransferScoreRequest.szAccounts));

					//发送数据
					m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_TRANSFER_SCORE_REQUEST,&TransferScoreRequest,sizeof(TransferScoreRequest));
				}
				return true;
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

