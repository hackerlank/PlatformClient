#include "stdafx.h"
#include "resource.h"
#include "DlgInsureGame.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

//��������
#define SPACE_ROUND					3									//����С	

///////////////////////////////////////////////////////////////////////////////////////////////////

//��Ϣӳ��
BEGIN_MESSAGE_MAP(CDlgInsureGame, CSkinDialog)

	ON_WM_TIMER()	
	ON_WM_SIZE()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//���캯��
CDlgInsureGame::CDlgInsureGame(CWnd* pParent) : CSkinDialog(IDD_BANK_STORAGE, pParent)
{
	//��ʶ����
	m_bSwitchIng=false;
	m_blCanStore=false;
	m_blCanGetOnlyFree=false;

	//������Ϣ
	m_wRevenueTake=0;
	m_wRevenueTransfer=0;
	m_lUserScore=0;
	m_lUserInsure=0;

	//�ӿڱ���
	m_pIMeUserData=NULL;
	m_pIClientKernel=NULL;	

	//����ؼ�
	m_pEditAmount=NULL;
	m_pLabelRound=NULL;
	m_pTextForget=NULL;
	m_pEditPassword=NULL;	

	//����ؼ�
	m_pButtonSave=NULL;
	m_pButtonTake=NULL;
	m_pButtonRefresh=NULL;

	//������Դ
	m_ImageNumber.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("BANKNUM"));
}

//��������
CDlgInsureGame::~CDlgInsureGame()
{
}

//�س�
VOID CDlgInsureGame::OnOK()
{
	DestroyWindow();
}

//ȡ��
VOID CDlgInsureGame::OnCancel()
{
	DestroyWindow();
}

//��ʼ������
BOOL CDlgInsureGame::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("����"));

	return TRUE;
}

VOID CDlgInsureGame::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

//��С��Ϣ
void CDlgInsureGame::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);
}

//��ʼ�ؼ�
void CDlgInsureGame::InitControlUI()
{
	__super::InitControlUI();

	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());	

	//��������
	CLabelUI * pLabelControl=NULL;

	//���п��
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetBkImage(TEXT("file='BANK_FRAME' restype='PNG'"));
	pLabelControl->SetFixedWidth(435);
	pLabelControl->SetFixedHeight(85);
	pLabelControl->SetFixedXY(CPoint(32,55));	

	//������ǩ
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("�����ȡ��"));
	pLabelControl->SetFixedWidth(86);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetFixedXY(CPoint(70,163));
	
	//�����ǩ
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("�����������룺"));	
	pLabelControl->SetFixedWidth(86);
	pLabelControl->SetFixedHeight(17);	
	pLabelControl->SetFixedXY(CPoint(70,194));

	//�����༭
	m_pEditAmount = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditAmount->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG' corner='3,3,3,3'"));	
	m_pEditAmount->SetMaxChar(16);
	m_pEditAmount->SetBkColor(0);
	m_pEditAmount->SetOnlyNumber();
	
	//�����༭
	m_pEditPassword = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditPassword->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG'corner='3,3,3,3'"));
	m_pEditPassword->SetPasswordMode(true);	
	m_pEditPassword->SetMaxChar(LEN_PASSWORD-1);
	m_pEditPassword->SetBkColor(0);	

	//��������
	m_pTextForget = CTextUI::Create(&m_PaintManager,pParent,TEXT(""));	
	m_pTextForget->SetText(TEXT("{a}{f 2}��������?{/f}{/a}"));	
	m_pTextForget->SetShowHtml();

	//��ť
	m_pButtonSave = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonSave->SetStatusImage(TEXT("file='BT_INSURE_SAVE' restype='PNG'"));		

	//ȡ�ť
	m_pButtonTake = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonTake->SetStatusImage(TEXT("file='BT_INSURE_TAKE' restype='PNG'"));	

	//�һ���ť
	m_pButtonRefresh = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonRefresh->SetStatusImage(TEXT("file='BT_INSURE_REFRESH' restype='PNG'"));

	//�����ؼ�
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());
}

//��Ϣ����
void CDlgInsureGame::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);
	
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{			
		if(m_pButtonSave==pControlUI)				//����¼�
		{
			return OnBnClickedSaveScore();
		}
		else if(m_pButtonTake==pControlUI)			//ȡ���¼�
		{
			return OnBnClickedTakeScore();
		}
		else if(m_pButtonRefresh==pControlUI)  		//ˢ���¼�
		{
			return OnBnClickedRefreshScore();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("link")) ==0 )
	{		
		if(m_pTextForget==pControlUI)				//��������
		{
			return OnBnClickedForgetInsure();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{		
		if(pControlUI==m_pEditAmount)				//��ȡ���
		{
			return OnEnChangeScore();
		}
	}
}

//�滭����
VOID CDlgInsureGame::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
	DrawNumberString(pDC,m_lUserScore,200,78);
	DrawNumberString(pDC,m_lUserInsure,200,104);

	//��ǰ��Ϸ��
	CRect rcScore(100,80,200,95);
	pDC->DrawText(TEXT("��ǰ��Ϸ�ң�"),rcScore,DT_END_ELLIPSIS | DT_WORDBREAK|DT_RIGHT);

	//��ǰ���
	CRect rcBank(100,105,200,120);
	pDC->DrawText(TEXT("������Ϸ�ң�"),rcBank,DT_END_ELLIPSIS | DT_WORDBREAK|DT_RIGHT);

	//��ʾ��Ϣ
	CString strInfo;
	CRect rect(0,305,nWidth,nHeight);	
	strInfo.Format(TEXT("��ܰ��ʾ��������Ϸ���������ѣ�ȡ�����۳�%ld���������"), m_wRevenueTake);
	pDC->DrawText(strInfo,rect,DT_END_ELLIPSIS | DT_CENTER | DT_WORDBREAK);
}

//�����ؼ�
void CDlgInsureGame::RectifyControl(INT nWidth, INT nHeight)
{
	//����У��
	if(nWidth==0 || nHeight==0) return;

	//��������
	INT nTViewBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
	INT nBViewBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
	INT nLViewBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRViewBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;

	//�����ؼ�
	if(m_pEditAmount!=NULL)
	{
		//�༭�ؼ�
		m_pEditAmount->SetFixedWidth(170);
		m_pEditAmount->SetFixedHeight(24);	
		m_pEditAmount->SetFixedXY(CPoint(nLViewBorder+SPACE_ROUND+148,nTViewBorder+SPACE_ROUND+115));

		//�༭�ؼ�		
		m_pEditPassword->SetFixedWidth(170);
		m_pEditPassword->SetFixedHeight(24);
		m_pEditPassword->SetFixedXY(CPoint(nLViewBorder+SPACE_ROUND+148,nTViewBorder+SPACE_ROUND+146));

		//��������		
		m_pTextForget->SetFixedWidth(100);
		m_pTextForget->SetFixedHeight(18);
		m_pTextForget->SetFixedXY(CPoint(nLViewBorder+SPACE_ROUND+332,nTViewBorder+SPACE_ROUND+153));

		//��ť�ؼ�
		m_pButtonSave->SetPos(nLViewBorder+SPACE_ROUND+108,nTViewBorder+SPACE_ROUND+190);	
		m_pButtonTake->SetPos(nLViewBorder+SPACE_ROUND+258,nTViewBorder+SPACE_ROUND+190);
		m_pButtonRefresh->SetPos(nLViewBorder+SPACE_ROUND+355,nTViewBorder+SPACE_ROUND+12);
	}
}


//��ȡ��Ϸ��
SCORE CDlgInsureGame::GetUserControlScore(LPCTSTR pszScore)
{
	//��������
	WORD wIndex=0;
	SCORE lScore=0L;

	//��ȡ��ֵ
	TCHAR szBuffer[32]=TEXT("");
	lstrcpyn(szBuffer,pszScore,CountArray(szBuffer));

	//������ֵ
	while (szBuffer[wIndex]!=0)
	{
		//��������
		WORD wCurrentIndex=wIndex++;

		//������ֵ
		if ((szBuffer[wCurrentIndex]>=TEXT('0'))&&(szBuffer[wCurrentIndex]<=TEXT('9')))
		{
			lScore=lScore*10L+(szBuffer[wCurrentIndex]-TEXT('0'));
		}
	}

	return lScore;
}

//�滭����
VOID CDlgInsureGame::DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos)
{
	//ת������
	TCHAR szControl[128]=TEXT("");
	SwitchScoreFormat(lScore,3L,szControl,CountArray(szControl));

	//��������
	INT nXDrawPos=nXPos;
	INT nScoreLength=lstrlen(szControl);

	//�滭�ж�
	if (nScoreLength>0L)
	{
		//������Դ
		CPngImage ImageNumber;
		ImageNumber.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("SCORE_NUMBER"));

		//��ȡ��С
		CSize SizeNumber;
		SizeNumber.SetSize(ImageNumber.GetWidth()/12L,ImageNumber.GetHeight());

		//�滭����
		for (INT i=0;i<nScoreLength;i++)
		{
			//�滭����
			if (szControl[i]==TEXT(','))
			{
				ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*10L,0L);
			}

			//�滭���
			if (szControl[i]==TEXT('.'))
			{
				ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*11L,0L);
			}

			//�滭����
			if (szControl[i]>=TEXT('0')&&szControl[i]<=TEXT('9'))
			{
				ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*(szControl[i]-TEXT('0')),0L);
			}

			//����λ��
			nXDrawPos+=SizeNumber.cx;
		}
	}

	return;
}

//ת���ַ�
VOID CDlgInsureGame::SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize)
{
	//ת����ֵ
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//��������
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//ת���ַ�
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//�����ַ�
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

		//���붺��
		if ((uSwitchLength!=wSourceIndex)&&(((uSwitchLength-wSourceIndex)%uSpace)==0L))
		{
			pszBuffer[wTargetIndex++]=TEXT(',');
		}
	}

	//�����ַ�
	pszBuffer[wTargetIndex++]=0;

	return;
}

//�������
VOID CDlgInsureGame::OnEnChangeScore()
{
	//״̬����
	if (m_bSwitchIng==false)
	{
		//���ñ���
		m_bSwitchIng=true;

		//��������
		SCORE lScore=GetUserControlScore(m_pEditAmount->GetText());
		SCORE lMaxScore=__max(m_lUserScore, m_lUserInsure);

		//��������
		if(lScore > lMaxScore) lScore = lMaxScore;

		//���ý���
		if (lScore>0L)
		{
			//ת������
			TCHAR szControl[128]=TEXT("");
			SwitchScoreFormat(lScore,3L,szControl,CountArray(szControl));

			//���ÿؼ�
			m_pEditAmount->SetText(szControl);

			//���ù��
			INT nScoreLength=lstrlen(szControl);
			m_pEditAmount->SetSel(nScoreLength,nScoreLength);
		}
		else
		{
			//���ÿؼ�
			m_pEditAmount->SetText(TEXT(""));
		}

		//���ñ���
		m_bSwitchIng=false;
	}
}

//��ť
VOID CDlgInsureGame::OnBnClickedSaveScore()
{
	//������
	SCORE lInputScore = GetUserControlScore(m_pEditAmount->GetText());

	//������֤
	if (lInputScore <= 0 || lInputScore > m_lUserScore )
	{
		//��������
		TCHAR szMessage[128]=TEXT("");

		//������Ϣ
		if(m_lUserScore>0)
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("���������Ϸ�ҽ�������1��%I64d֮��"), m_lUserScore);
		}
		else 
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("������Я������Ϸ�Ҳ���,���ܽ��д�����!"));
		}

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szMessage,MB_ICONERROR,10);

		//���ÿؼ�
		m_pEditAmount->SetFocus();
		m_pEditAmount->SetText(TEXT(""));

		return;
	}

	//״̬�ж�
	if (US_PLAYING==m_pIMeUserData->GetUserStatus())
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�������Ϸ���ٴ�"),MB_ICONERROR,30);
		return;	
	}

	//��������
	CMD_GR_C_SaveScoreRequest SaveScoreRequest;
	SaveScoreRequest.cbActivityGame=TRUE;
	SaveScoreRequest.lSaveScore=lInputScore;

	//��������
	SendSocketData(MDM_GR_INSURE,SUB_GR_SAVE_SCORE_REQUEST,&SaveScoreRequest,sizeof(SaveScoreRequest));

	return;
}

//ȡ�ť
VOID CDlgInsureGame::OnBnClickedTakeScore()
{
	//������
	SCORE lInputScore = GetUserControlScore(m_pEditAmount->GetText());

	//������֤
	if (lInputScore <= 0 || lInputScore > m_lUserInsure )
	{
		//��������
		TCHAR szMessage[128]=TEXT("");

		//������Ϣ
		if(m_lUserInsure>0)
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("���������Ϸ�ҽ�������1��%I64d֮��"), m_lUserInsure);
		}
		else 
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("�����е���Ϸ������,���ܽ���������!"));
		}

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szMessage,MB_ICONERROR,30);

		//���ÿؼ�
		m_pEditAmount->SetFocus();
		m_pEditAmount->SetText(TEXT(""));

		return;
	}

	//������֤
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	lstrcpyn(szPassword,m_pEditPassword->GetText(),CountArray(szPassword));
	if (szPassword[0]==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�������벻��Ϊ�գ����������룡"),MB_ICONERROR,30);

		//���ÿؼ�
		m_pEditPassword->SetText(TEXT(""));
		m_pEditPassword->SetFocus();

		return;
	}

	//�������
	TCHAR szTempPassword[LEN_PASSWORD]=TEXT("");
	CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
	CWHEncrypt::MD5Encrypt(szTempPassword,szPassword);

	//��������
	CMD_GR_C_TakeScoreRequest TakeScoreRequest;
	ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));

	//����ṹ
	TakeScoreRequest.cbActivityGame=TRUE;
	TakeScoreRequest.lTakeScore=lInputScore;
	CopyMemory(TakeScoreRequest.szInsurePass, szPassword, sizeof(TakeScoreRequest.szInsurePass));

	//��������
	SendSocketData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));

	//�������
	m_pEditPassword->SetText(TEXT(""));

	return;
}

//ˢ�°�ť
VOID CDlgInsureGame::OnBnClickedRefreshScore()
{
	//��������
	CMD_GR_C_QueryInsureInfoRequest QueryInsureInfo;
	QueryInsureInfo.cbActivityGame=TRUE;

	//��������
	SendSocketData(MDM_GR_INSURE,SUB_GR_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));
}

//��������
VOID CDlgInsureGame::OnBnClickedForgetInsure()
{
	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/Member/ReInsurePass.aspx"),pGlobalWebLink->GetPlatformLink());

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);
}

//������Ϣ
BOOL CDlgInsureGame::PreTranslateMessage(MSG* pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

//ʱ����Ϣ
void CDlgInsureGame::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
}

//������Ϣ
void CDlgInsureGame::SetSendInfo(IClientKernel *pClientKernel,IClientUserItem const*pMeUserData)
{
	//����Ч��
	ASSERT(pClientKernel!=NULL);
	if(pClientKernel==NULL) return;

	//������Ϣ
	m_pIClientKernel = pClientKernel;
	m_pIMeUserData=const_cast<IClientUserItem *>(pMeUserData);

	//ˢ������
	OnBnClickedRefreshScore();

	return;
}

//����λ��
void CDlgInsureGame::SetPostPoint(CPoint Point)
{
	CRect Rect;
	GetWindowRect(&Rect);
	SetWindowPos(NULL,Point.x,Point.y-Rect.bottom+Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,/*SWP_NOMOVE|*/SWP_NOZORDER);
}

//������Ϣ
void CDlgInsureGame::SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	//����Ч��
	ASSERT(m_pIClientKernel!=NULL);

	//������Ϣ
	if(m_pIClientKernel) m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return;
}

//��ʾ����
void CDlgInsureGame::ShowItem()
{
	//��������
	bool bShow = true;
	if(m_blCanGetOnlyFree && m_pIMeUserData->GetUserStatus() == US_PLAYING) bShow = false;

	//���ÿؼ�
	m_pButtonSave->SetEnabled(m_blCanStore);
	m_pButtonTake->SetEnabled(bShow);
}
///////////////////////////////////////////////////////////////////////////////////////////////////