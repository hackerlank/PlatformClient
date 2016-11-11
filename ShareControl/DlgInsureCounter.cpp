#include "StdAfx.h"
#include "Resource.h"
#include "Information.h"
#include "DlgInsureCounter.h"
#include "UserItemElement.h"

//////////////////////////////////////////////////////////////////////////////////

//�ؼ���ʶ
#define IDC_INSRUE_SAVE					100							//���д�ȡ	
#define IDC_INSURE_TRANSFER				101							//����ת��
#define IDC_INSURE_RECORD				102							//������¼
#define IDC_INSURE_SETUP				200							//��������
#define IDC_INSURE_OPERATER				201							//���в���
#define IDC_PULICIZE_RECORD				300							//������¼

//////////////////////////////////////////////////////////////////////////////////

//��Ϣӳ��
BEGIN_MESSAGE_MAP(CDlgInsureOperater, CFGuiWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//��Ϣӳ��
BEGIN_MESSAGE_MAP(CDlgInsureRecord, CFGuiWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//��ȡ��Ϸ��
SCORE CDlgInsureItem::GetUserControlScore(LPCTSTR pszScore)
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
VOID CDlgInsureItem::DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos)
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
VOID CDlgInsureItem::SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize)
{
	//��������
	LPCTSTR pszNumber[]={TEXT("��"),TEXT("Ҽ"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("½"),TEXT("��"),TEXT("��"),TEXT("��")};
	LPCTSTR pszWeiName[]={TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��"),TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��"),TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��")};

	//ת����ֵ
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//��������
	bool bNeedFill=false;
	bool bNeedZero=false;
	UINT uTargetIndex=0,uSourceIndex=0;

	//�ַ�����
	UINT uItemLength=lstrlen(pszNumber[0]);
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//ת������
	for (UINT i=0;i<uSwitchLength;i++)
	{
		//��������
		WORD wNumberIndex=szSwitchScore[i]-TEXT('0');

		//�������
		if ((bNeedZero==true)&&(wNumberIndex!=0L))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszNumber[0],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//��������
		if (wNumberIndex!=0L)
		{
			_tcsncat(pszBuffer,pszNumber[wNumberIndex],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//����λ��
		if ((wNumberIndex!=0L)&&((uSwitchLength-i)>=2))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//�����ж�
		if ((bNeedZero==false)&&(wNumberIndex==0))
		{
			bNeedZero=true;
		}

		//��λ�ж�
		if ((bNeedFill==false)&&(wNumberIndex!=0))
		{
			bNeedFill=true;
		}

		//�λ��
		if (((uSwitchLength-i)==5)||((uSwitchLength-i)==9))
		{
			//����λ��
			if ((bNeedFill==true)&&(wNumberIndex==0L))
			{
				_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
			}

			//���ñ���
			bNeedZero=false;
			bNeedFill=false;
		}
	}

	return;
}

//ת���ַ�
VOID CDlgInsureItem::SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize)
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

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgInsureSave::CDlgInsureSave()
{
	//���ñ���
	m_bSwitchIng=false;

	//����ָ��
	m_pDlgInsureMain=NULL;
	m_pDlgInsureOperater=NULL;

	//����ؼ�
	m_pEditAmount=NULL;
	m_pEditInsurePass=NULL;
	m_pTextForget=NULL;
	m_pButtonSave=NULL;
	m_pButtonTake=NULL;
	m_pButtonExchange=NULL;

	return;
}

//��������
CDlgInsureSave::~CDlgInsureSave()
{
}

//��ʼ�滭
VOID CDlgInsureSave::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�滭����
	pDC->FillSolidRect(&rcClient,m_pDlgInsureOperater->GetItemBackColor());

	//��Դ���
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//����ͼƬ
	CPngImage ImageMoney;
	CPngImage ImageBank;
	CPngImage ImageLine;
	ImageMoney.LoadImage(hInstance,TEXT("IMAGE_MONEY"));
	ImageBank.LoadImage(hInstance,TEXT("IMAGE_BANK"));
	ImageLine.LoadImage(hInstance,TEXT("IMAGE_LINE"));

	//�滭ͼƬ
	ImageMoney.DrawImage(pDC,16,4);
	ImageBank.DrawImage(pDC,298,4);
	ImageLine.DrawImage(pDC,0,134);

	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//��������
	TCHAR szControl[128]=TEXT("");
	TCHAR szGameScore[64]=TEXT("");
	TCHAR szInsureScore[64]=TEXT("");
	TCHAR szServiceFee[64]=TEXT("");

	//������
	CRect rcGameScore(16,84,134,100);
	CRect rcInsureScore(300,84,418,100);
	CRect rcServiceFee(70,251,439,273);

	//������Ϸ��		
	CDlgInsureItem::SwitchScoreFormat(pGlobalUserData->lUserScore,3L,szControl,CountArray(szControl));
	_sntprintf(szGameScore,CountArray(szGameScore),TEXT("��Ϸ�ң�%s"),szControl);
	pDC->DrawText(szGameScore,rcGameScore,DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//���ƴ��
	CDlgInsureItem::SwitchScoreFormat(pGlobalUserData->lUserInsure,3L,szControl,CountArray(szControl));
	_sntprintf(szInsureScore,CountArray(szInsureScore),TEXT("��%s"),szControl);
	pDC->DrawText(szInsureScore,rcInsureScore,DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//�������	
	_sntprintf(szServiceFee,CountArray(szServiceFee),TEXT("��ܰ��ʾ��������Ϸ���������ѣ�ȡ�����۳�%d��������ѡ�"),m_pUserInsureInfo->wRevenueTake);
	pDC->DrawText(szServiceFee,rcServiceFee,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//��ʼ�ؼ�
VOID CDlgInsureSave::InitControlUI()
{
	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//��������
	CLabelUI * pLabelControl=NULL;

	//������ǩ
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("�����ȡ��"));
	pLabelControl->SetFixedWidth(86);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetFixedXY(CPoint(70,150));
	
	//�����ǩ
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("�����������룺"));
	pLabelControl->SetFixedXY(CPoint(70,182));
	pLabelControl->SetFixedWidth(86);
	pLabelControl->SetFixedHeight(17);	

	//ˢ�±�ǩ
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetBkImage(TEXT("file='IMAGE_INSURE_REFRESH' restype='PNG'"));
	pLabelControl->SetFixedXY(CPoint(173,16));
	pLabelControl->SetFixedWidth(86);
	pLabelControl->SetFixedHeight(86);

	//�����༭
	m_pEditAmount = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditAmount->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG' corner='3,3,3,3'"));
	m_pEditAmount->SetFixedXY(CPoint(158,145));
	m_pEditAmount->SetFixedWidth(170);
	m_pEditAmount->SetFixedHeight(24);	
	m_pEditAmount->SetMaxChar(16);
	m_pEditAmount->SetBkColor(0);
	m_pEditAmount->SetOnlyNumber();
	
	//�����༭
	m_pEditInsurePass = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditInsurePass->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG'corner='3,3,3,3'"));
	m_pEditInsurePass->SetFixedXY(CPoint(158,176));
	m_pEditInsurePass->SetFixedWidth(170);
	m_pEditInsurePass->SetFixedHeight(24);	
	m_pEditInsurePass->SetPasswordMode(true);	
	m_pEditInsurePass->SetMaxChar(LEN_PASSWORD-1);
	m_pEditInsurePass->SetBkColor(0);	

	//��������
	m_pTextForget = CTextUI::Create(&m_PaintManager,pParent,TEXT(""));	
	m_pTextForget->SetText(TEXT("{a}{f 2}��������?{/f}{/a}"));	
	m_pTextForget->SetFixedXY(CPoint(342,185));
	m_pTextForget->SetFixedWidth(100);
	m_pTextForget->SetFixedHeight(18);	
	m_pTextForget->SetShowHtml();	

	//��ť
	m_pButtonSave = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonSave->SetStatusImage(TEXT("file='BT_INSURE_SAVE' restype='PNG'"));	
	m_pButtonSave->SetFixedXY(CPoint(118,212));

	//ȡ�ť
	m_pButtonTake = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonTake->SetStatusImage(TEXT("file='BT_INSURE_TAKE' restype='PNG'"));	
	m_pButtonTake->SetFixedXY(CPoint(268,212));

	//�һ���ť
	m_pButtonExchange = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonExchange->SetStatusImage(TEXT("file='BT_INSURE_EXCHANGE' restype='PNG'"));	
	m_pButtonExchange->SetFixedXY(CPoint(40,106));
}

//��Ϣ����
VOID CDlgInsureSave::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{		
		if(pControlUI==m_pButtonSave)			//������
		{
			return OnBnClickedSaveScore();
		}
		else if(pControlUI==m_pButtonTake)		//ȡ�����	
		{
			return OnBnClickedTakeScore();
		}
		else if(pControlUI==m_pButtonExchange)	//�һ�����
		{
			return OnBnClickedExChangeScore();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("link")) ==0 )
	{
		if(pControlUI==m_pTextForget)			//��������
		{
			return OnBnClickedForgetInsure();
		}
	}
	else if(lstrcmp(msg.sType,TEXT("textchanged")) ==0 )
	{
		if(pControlUI==m_pEditAmount)			//��ȡ����
		{
			return OnEnChangeScore();
		}
	}	
}

//ȡ�ť
VOID CDlgInsureSave::OnBnClickedTakeScore()
{
	//��ȡ����
	SCORE lTakeScore= CDlgInsureItem::GetUserControlScore(m_pEditAmount->GetText());

	//��ȡ����
	TCHAR szPassword[LEN_PASSWORD];
	lstrcpyn(szPassword,m_pEditInsurePass->GetText(),CountArray(szPassword));

	//��������
	TCHAR szInsurePass[LEN_MD5];
	CWHEncrypt::MD5Encrypt(szPassword,szInsurePass);

	//�����ж�
	if (lTakeScore<=0L)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("ȡ������Ϸ����������Ϊ�գ�������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_pEditAmount->SetFocus();

		return;
	}

	//��Ŀ�ж�
	if (lTakeScore>m_pUserInsureInfo->lUserInsure)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��������Ϸ�ҵ���Ŀ���㣬������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_pEditAmount->SetFocus();

		return;
	}

	//�����ж�
	if (szPassword[0]==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�������벻��Ϊ�գ������������������룡"),MB_ICONERROR,30);

		//���ý���
		m_pEditInsurePass->SetFocus();

		return;
	}
	
	//�¼�֪ͨ
	m_pDlgInsureMain->OnMissionStart();
	m_pDlgInsureMain->PerformTakeScore(lTakeScore,szInsurePass);

	return;
}

//�������
VOID CDlgInsureSave::OnEnChangeScore()
{
	//״̬����
	if (m_bSwitchIng==false)
	{
		//���ñ���
		m_bSwitchIng=true;

		//��������
		SCORE lScore=CDlgInsureItem::GetUserControlScore(m_pEditAmount->GetText());
		SCORE lMaxScore=__max(m_pUserInsureInfo->lUserScore, m_pUserInsureInfo->lUserInsure);
		if((m_pUserInsureInfo->lUserScore==0) || (m_pUserInsureInfo->lUserInsure==0))
		{
			lMaxScore=__max(m_pUserInsureInfo->lUserScore, m_pUserInsureInfo->lUserInsure);
		}

		//��������
		if(lScore > lMaxScore) lScore = lMaxScore;

		//���ý���
		if (lScore>0L)
		{
			//ת������
			TCHAR szControl[128]=TEXT("");
			CDlgInsureItem::SwitchScoreFormat(lScore,3L,szControl,CountArray(szControl));

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

	return;
}

//��ť
VOID CDlgInsureSave::OnBnClickedSaveScore()
{
	//��������
	SCORE lSaveScore=CDlgInsureItem::GetUserControlScore(m_pEditAmount->GetText());

	//�����ж�
	if (lSaveScore<=0L)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�������Ϸ����������Ϊ�գ�������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_pEditAmount->SetFocus();

		return;
	}

	//��Ŀ�ж�
	if (lSaveScore>m_pUserInsureInfo->lUserScore)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("����Я����Ϸ�ҵ���Ŀ���㣬������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_pEditAmount->SetFocus();

		return;
	}

	//ִ�д��
	m_pDlgInsureMain->OnMissionStart();
	m_pDlgInsureMain->PerformSaveScore(lSaveScore);

	return;
}

//��������
VOID CDlgInsureSave::OnBnClickedForgetInsure()
{
	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/%sMember/ReInsurePass.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	return;
}

//�һ���Ϸ��
VOID CDlgInsureSave::OnBnClickedExChangeScore()
{
	//��ȡ����
	ASSERT(AfxGetMainWnd()!=NULL);
	if(AfxGetMainWnd()==NULL) return;

	//����ṹ
	tagInsurePlazaEvent * pInsurePlazaEvent = new tagInsurePlazaEvent;
	ZeroMemory(pInsurePlazaEvent,sizeof(tagInsurePlazaEvent));

	//��Ϣ֪ͨ
	pInsurePlazaEvent->bHeapData=true;
	pInsurePlazaEvent->wCommandID=INSURE_COMMAND_EXCHANGE;
	AfxGetMainWnd()->PostMessage(WM_INSUREPLAZA_EVENT,(WPARAM)pInsurePlazaEvent,0L);
	
	return m_pDlgInsureMain->OnOK();
}


//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgInsureTransfer::CDlgInsureTransfer()
{
	//���ñ���
	m_bSwitchIng=false;

	//����ָ��
	m_pDlgInsureMain=NULL;
	m_pDlgInsureOperater=NULL;

	//����ؼ�
	m_pEditAccount=NULL;
	m_pEditAmount=NULL;
	m_pEditInsurePass=NULL;
	m_pEditRemarks=NULL;

	//����ؼ�
	m_pLabelTarget=NULL;
	m_pButtonConfirm=NULL;
	m_pRadioByUserID=NULL;
	m_pRadioByNickName=NULL;

	return;
}

//��������
CDlgInsureTransfer::~CDlgInsureTransfer()
{
}

//��ʼ�滭
VOID CDlgInsureTransfer::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�滭����
	pDC->FillSolidRect(&rcClient,m_pDlgInsureOperater->GetItemBackColor());

	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�����ǳ�
	CRect rcNickName(168,16,268,32);
	pDC->DrawText(pGlobalUserData->szNickName,rcNickName,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//��������
	TCHAR szUserInsure[32]=TEXT("");
	CDlgInsureItem::SwitchScoreFormat(pGlobalUserData->lUserInsure,3,szUserInsure,CountArray(szUserInsure));

	//�������
	CRect rcUserInsure(168,37,268,53);
	pDC->DrawText(szUserInsure,rcUserInsure,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//�������
	TCHAR szServiceFee[128]=TEXT("");
	CRect rcServiceFee(20,251,rcClient.Width()-20,273);
	_sntprintf(szServiceFee,CountArray(szServiceFee),TEXT("��ܰ��ʾ��ת����ͨ��ҿ۳�%ld��������ѣ���Ա��ҿ۳�%ld��������ѡ�"),m_pUserInsureInfo->wRevenueTransfer,m_pUserInsureInfo->wRevenueTransferMember);
	pDC->DrawText(szServiceFee,rcServiceFee,DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//��ʼ�ؼ�
VOID CDlgInsureTransfer::InitControlUI()
{
	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//��������
	CLabelUI * pLabelControl = NULL;

	//�����ʺ�
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("�����ʺţ�"));
	pLabelControl->SetFixedWidth(62);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetFixedXY(CPoint(100,17));
	
	//�ʺ����
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("���д�"));
	pLabelControl->SetFixedXY(CPoint(100,37));
	pLabelControl->SetFixedWidth(62);
	pLabelControl->SetFixedHeight(17);

	//�����ʺ�
	m_pLabelTarget = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pLabelTarget->SetText(TEXT("�����ǳƣ�"));
	m_pLabelTarget->SetFixedXY(CPoint(100,87));
	m_pLabelTarget->SetFixedWidth(62);
	m_pLabelTarget->SetFixedHeight(17);	

	//���ͽ��
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("���ͽ�"));
	pLabelControl->SetFixedXY(CPoint(100,117));
	pLabelControl->SetFixedWidth(62);
	pLabelControl->SetFixedHeight(17);

	//��������
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("�������룺"));
	pLabelControl->SetFixedXY(CPoint(100,147));
	pLabelControl->SetFixedWidth(62);
	pLabelControl->SetFixedHeight(17);	

	//��ע˵��
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("��ע˵����"));
	pLabelControl->SetFixedXY(CPoint(100,177));
	pLabelControl->SetFixedWidth(62);
	pLabelControl->SetFixedHeight(17);

	//�����ʺ�
	m_pEditAccount = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditAccount->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG' corner='3,3,3,3'"));
	m_pEditAccount->SetMaxChar(LEN_ACCOUNTS-1);
	m_pEditAccount->SetFixedXY(CPoint(162,82));
	m_pEditAccount->SetFixedWidth(170);
	m_pEditAccount->SetFixedHeight(24);		
	m_pEditAccount->SetBkColor(0);

	//ת�ʽ��
	m_pEditAmount = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditAmount->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG' corner='3,3,3,3'"));
	m_pEditAmount->SetFixedXY(CPoint(162,112));
	m_pEditAmount->SetFixedWidth(170);
	m_pEditAmount->SetFixedHeight(24);	
	m_pEditAmount->SetMaxChar(16);
	m_pEditAmount->SetBkColor(0);
	m_pEditAmount->SetOnlyNumber();

	//��������
	m_pEditInsurePass = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditInsurePass->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG' corner='3,3,3,3'"));
	m_pEditInsurePass->SetPasswordMode(true);
	m_pEditInsurePass->SetMaxChar(LEN_PASSWORD-1);
	m_pEditInsurePass->SetFixedXY(CPoint(162,142));
	m_pEditInsurePass->SetFixedWidth(170);
	m_pEditInsurePass->SetFixedHeight(24);		
	m_pEditInsurePass->SetBkColor(0);
	
	//ת�ʱ�ע
	m_pEditRemarks = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditRemarks->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG' corner='3,3,3,3'"));
	m_pEditRemarks->SetMaxChar(LEN_TRANS_REMARK-1);
	m_pEditRemarks->SetFixedXY(CPoint(162,172));
	m_pEditRemarks->SetFixedWidth(170);
	m_pEditRemarks->SetFixedHeight(24);		
	m_pEditRemarks->SetBkColor(0);

	//��ѡ��ť
	m_pRadioByUserID = CRadioButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pRadioByUserID->SetNormalImage(TEXT("file='RADIO_NORMAL' restype='PNG'"));
	m_pRadioByUserID->SetCheckedImage(TEXT("file='RADIO_CHECKED' restype='PNG'"));
	m_pRadioByUserID->SetTextPadding(CRect(12,1,0,0));
	m_pRadioByUserID->SetText(TEXT(" ���� I D"));	
	m_pRadioByUserID->SetFixedWidth(70);
	m_pRadioByUserID->SetFixedHeight(12);
	m_pRadioByUserID->SetPos(164,62);
	m_pRadioByUserID->SetGrouped(true);
	m_pRadioByUserID->SetGroupID(1);

	//��ѡ��ť
	m_pRadioByNickName = CRadioButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pRadioByNickName->SetNormalImage(TEXT("file='RADIO_NORMAL' restype='PNG'"));
	m_pRadioByNickName->SetCheckedImage(TEXT("file='RADIO_CHECKED' restype='PNG'"));
	m_pRadioByNickName->SetTextPadding(CRect(12,1,0,0));
	m_pRadioByNickName->SetText(TEXT(" �����ǳ�"));
	m_pRadioByNickName->SetFixedWidth(70);
	m_pRadioByNickName->SetFixedHeight(12);
	m_pRadioByNickName->SetPos(252,62);
	m_pRadioByNickName->SetGrouped(true);
	m_pRadioByNickName->SetGroupID(1);
	m_pRadioByNickName->SetCheck(true);

	//ȷ�ϰ�ť
	m_pButtonConfirm = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonConfirm->SetStatusImage(TEXT("file='BT_EXCHANGE_CONFIRM' restype='PNG'"));	
	m_pButtonConfirm->SetFixedXY(CPoint(176,213));
	m_pButtonConfirm->SetWantReturn();
}

//��Ϣ����
VOID CDlgInsureTransfer::Notify(TNotifyUI &  msg)
{
	//��ȡ����
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

//�������
VOID CDlgInsureTransfer::OnEnChangeScore()
{
	//״̬����
	if (m_bSwitchIng==false)
	{
		//���ñ���
		m_bSwitchIng=true;

		//��������
		SCORE lScore=CDlgInsureItem::GetUserControlScore(m_pEditAmount->GetText());

		//��������
		if(lScore > m_pUserInsureInfo->lUserInsure) lScore = m_pUserInsureInfo->lUserInsure;

		//���ý���
		if (lScore>0L)
		{
			//ת������
			TCHAR szControl[128]=TEXT("");
			CDlgInsureItem::SwitchScoreFormat(lScore,3L,szControl,CountArray(szControl));

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

//ת�ʰ�ť
VOID CDlgInsureTransfer::OnBnClickedTransferScore()
{
	//�����ж�
	if (m_pUserInsureInfo->lUserInsure < m_pUserInsureInfo->lTransferPrerequisite)
	{
		//��ʾ��Ϣ
		TCHAR szTipInfo[128]={0};
		_sntprintf(szTipInfo,CountArray(szTipInfo),TEXT("��������Ϸ�ҵ���Ŀ�������ת������ %I64d���޷�����ת�ʲ�����"),m_pUserInsureInfo->lTransferPrerequisite);
		CInformation Information(this);
		Information.ShowMessageBox(szTipInfo,MB_ICONERROR,30);

		return;
	}

	//ת�ʽ��
	SCORE lTransferScore= CDlgInsureItem::GetUserControlScore(m_pEditAmount->GetText());

	//ת�ʱ�ע
	TCHAR szTransRemark[LEN_TRANS_REMARK]=TEXT("");
	lstrcpyn(szTransRemark,m_pEditRemarks->GetText(),CountArray(szTransRemark));

	//��ȡ����
	TCHAR szPassword[LEN_PASSWORD];
	lstrcpyn(szPassword,m_pEditInsurePass->GetText(),CountArray(szPassword));

	//��������
	TCHAR szInsurePass[LEN_MD5];
	CWHEncrypt::MD5Encrypt(szPassword,szInsurePass);

	//��ȡ�ǳ�
	CString strAccounts = m_pEditAccount->GetText();

	//��ʽ�ǳ�
	strAccounts.TrimLeft();
	strAccounts.TrimRight();

	//�ǳ��ж�
	if (m_pRadioByNickName->GetCheck()==true && strAccounts.GetLength()==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("������Ҫ���͵�����ǳƣ�"),MB_ICONERROR,30);

		//���ý���
		m_pEditAccount->SetFocus();

		return;
	}

	//ID���ж�
	if(m_pRadioByUserID->GetCheck()==true && strAccounts.GetLength()==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("������Ҫ���͵����ID��"),MB_ICONERROR,30);

		//���ý���
		m_pEditAccount->SetFocus();

		return;
	}

	//�����ж�
	if (lTransferScore<=0L)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���͵���Ϸ����������Ϊ�գ�������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_pEditAmount->SetFocus();

		return;
	}

	//��Ŀ�ж�
	if (lTransferScore>m_pUserInsureInfo->lUserInsure)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��������Ϸ�ҵ���Ŀ���㣬������������Ϸ��������"),MB_ICONERROR,30);

		//���ý���
		m_pEditAmount->SetFocus();

		return;
	}

	//��Ŀ�ж�
	if (lTransferScore<m_pUserInsureInfo->lTransferPrerequisite)
	{
		//��ʾ��Ϣ
		TCHAR szTipInfo[128]={0};
		_sntprintf(szTipInfo,CountArray(szTipInfo),TEXT("������ת�ʵ���Ϸ����Ŀ�������� %I64d���޷�����ת�ʲ�����"),m_pUserInsureInfo->lTransferPrerequisite);
		CInformation Information(this);
		Information.ShowMessageBox(szTipInfo,MB_ICONERROR,30);

		//���ý���
		m_pEditAmount->SetFocus();

		return;
	}

	//�����ж�
	if (szPassword[0]==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�������벻��Ϊ�գ������������������룡"),MB_ICONERROR,30);

		//���ý���
		m_pEditInsurePass->SetFocus();

		return;
	}
	
	//�¼�֪ͨ
	m_pDlgInsureMain->OnMissionStart();
	m_pDlgInsureMain->PerformTransferScore(strAccounts,lTransferScore,szInsurePass,szTransRemark,m_pRadioByUserID->GetCheck()?FALSE:TRUE);

	//���ý���
	m_pEditInsurePass->SetFocus();
	m_pEditInsurePass->SetText(TEXT(""));

	return;
}

//����ID
VOID CDlgInsureTransfer::OnBnCheckedTransferByUserID()
{
	//���ÿؼ�
	m_pEditAccount->SetText(TEXT(""));
	m_pEditAccount->SetOnlyNumber(true);
	m_pLabelTarget->SetText(TEXT("���� I D��"));		
}

//�����ǳ�
VOID CDlgInsureTransfer::OnBnCheckedTransferByNickName()
{
	//���ÿؼ�
	m_pEditAccount->SetText(TEXT(""));
	m_pEditAccount->SetOnlyNumber(false);
	m_pLabelTarget->SetText(TEXT("�����ǳƣ�"));	
}

//////////////////////////////////////////////////////////////////////////////////
//���캯��
CDlgInsureRecord::CDlgInsureRecord()
{
}

//��������
CDlgInsureRecord::~CDlgInsureRecord()
{
}

//��ʼ�滭
VOID CDlgInsureRecord::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���û���
	pDC->SetBkMode(TRANSPARENT);

	//�滭����
	pDC->FillSolidRect(&rcClient,m_pDlgInsureOperater->GetItemBackColor());
}

//ˢ�¼�¼
VOID CDlgInsureRecord::RefreshInsureRecord()
{
	CWebBrowser * pWebBrowser=m_PublicizeRecord.GetWebBrowser();
	if (pWebBrowser!=NULL) pWebBrowser->Refresh();
}

//������Ϣ
int CDlgInsureRecord::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) return -1;

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_PublicizeRecord.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_PULICIZE_RECORD);
	m_PublicizeRecord.SetBackGroundColor(m_pDlgInsureOperater->GetItemBackColor());	

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");

	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//������¼
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/%sUserService/UserService7.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink());
	m_PublicizeRecord.Navigate(szBillUrl);	
	
	return 0;
}

//������С
VOID CDlgInsureRecord::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//���ÿؼ�
	m_PublicizeRecord.SetWindowPos(NULL,0,0,cx,cy,SWP_NOZORDER);
}

//////////////////////////////////////////////////////////////////////////////////
//���캯��
CDlgInsureOperater::CDlgInsureOperater()
{
	//ѡ��ؼ�
	m_cbInsureMode=INSURE_SAVE;
	m_cbInsureMask=INSURE_MASK_SAVE|INSURE_MASK_RECORD;
	ZeroMemory(m_TabButton,sizeof(m_TabButton));

	//�ؼ�����
	m_pLabelRound=NULL;

	//���ñ���
	m_pUserInsureInfo=NULL;
}

//��������
CDlgInsureOperater::~CDlgInsureOperater()
{
}

//��ʼ�滭
VOID CDlgInsureOperater::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�滭����
	pDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);
}

//��Ϣ����
VOID CDlgInsureOperater::Notify(TNotifyUI &  msg)
{
	//��ȡ����
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

//��ʼ�ؼ�
VOID CDlgInsureOperater::InitControlUI()
{
	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//��������
	m_pLabelRound = CLabelUI::Create(&m_PaintManager,pParent,TEXT("")); 
	m_pLabelRound->SetBkImage(TEXT("file='IMAGE_ROUND_FRAME1' restype='PNG' corner='6,6,6,6'"));
	m_pLabelRound->SetFixedWidth(452);
	m_pLabelRound->SetFixedHeight(275);
	m_pLabelRound->SetPos(CRect(4,35,456,310));

	//��Ϸ�Ҵ�ȡ
	m_TabButton[0] = CTabButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_TabButton[0]->SetStatusImage(TEXT("file='TAB_INSURE_SAVE' restype='PNG'"));
	m_TabButton[0]->SetGrouped();
	m_TabButton[0]->SetVisible(false);

	//��Ϸ��ת��
	m_TabButton[1] = CTabButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_TabButton[1]->SetStatusImage(TEXT("file='TAB_INSURE_TRANSFER' restype='PNG'"));
	m_TabButton[1]->SetGrouped();
	m_TabButton[1]->SetVisible(false);

	//������¼
	m_TabButton[2] = CTabButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_TabButton[2]->SetStatusImage(TEXT("file='TAB_INSURE_RECORD' restype='PNG'"));
	m_TabButton[2]->SetGrouped();
	m_TabButton[2]->SetVisible(false);

	//��������
	RectifyTabItem();
}

//��������
VOID CDlgInsureOperater::RectifyTabItem()
{
	//��ȡ����
	CRect rcRound = m_pLabelRound->GetPos();
	CSize SizeItem(m_TabButton[0]->GetFixedWidth(),m_TabButton[0]->GetFixedHeight());

	//��������
	INT nXPos = rcRound.left+20;
	INT nYPos = rcRound.top-SizeItem.cy+1;
	INT nXSpace = 10;

	//��Ϸ�Ҵ�ȡ
	m_TabButton[0]->SetVisible(false);
	if(m_cbInsureMask&INSURE_MASK_SAVE)
	{
		//���ð�ť
		m_TabButton[0]->SetVisible();
		m_TabButton[0]->SetPos(nXPos,nYPos);

		//����λ��
		nXPos += SizeItem.cx+nXSpace;
	}

	//��Ϸ��ת��
	m_TabButton[1]->SetVisible(false);
	if(m_cbInsureMask&INSURE_MASK_TRANSFER)
	{
		//���ð�ť
		m_TabButton[1]->SetVisible();
		m_TabButton[1]->SetPos(nXPos,nYPos);

		//����λ��
		nXPos += SizeItem.cx+nXSpace;
	}

	//������¼
	m_TabButton[2]->SetVisible(false);
	if(m_cbInsureMask&INSURE_MASK_RECORD)
	{
		//���ð�ť
		m_TabButton[2]->SetVisible();
		m_TabButton[2]->SetPos(nXPos,nYPos);

		//����λ��
		nXPos += SizeItem.cx+nXSpace;
	}

	return;
}

//��������
VOID CDlgInsureOperater::InsertTabItem(BYTE cbItemMask)
{
	//�����ж�
	if(m_cbInsureMask&cbItemMask) return;

	//���ñ���
	m_cbInsureMask |= cbItemMask;

	//��������
	RectifyTabItem();
}

//ɾ������
VOID CDlgInsureOperater::DeleteTabItem(BYTE cbItemMask)
{
	if((m_cbInsureMask&cbItemMask)==0) return;

	//���ñ���
	m_cbInsureMask &= ~cbItemMask;

	//��������
	RectifyTabItem();
}

//����ģʽ
VOID CDlgInsureOperater::SwitchInsureModeView(BYTE cbInsureMode)
{
	switch (cbInsureMode)
	{
	case INSURE_SAVE:			//��ȡ��ʽ
		{
			//���ñ���
			m_cbInsureMode=INSURE_SAVE;

			//���ÿؼ�
			if (m_DlgInsureSave.m_hWnd==NULL)
			{
				//��ȡλ��
				CRect rcClient;
				GetClientRect(&rcClient);

				//�����ؼ�
				CRect rcCreate(0,0,0,0);
				m_DlgInsureSave.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_INSRUE_SAVE);

				//�ƶ�λ��
				CRect rcRound = m_pLabelRound->GetPos();
				m_DlgInsureSave.SetWindowPos(NULL,rcRound.left+5,rcRound.top+5,rcRound.Width()-10,rcRound.Height()-10,SWP_NOZORDER|SWP_SHOWWINDOW);
			}
			else
			{
				//��ʾ����
				m_DlgInsureSave.ShowWindow(SW_SHOW);
			}

			//���ÿؼ�
			if (m_DlgInsureTransfer.m_hWnd!=NULL) m_DlgInsureTransfer.ShowWindow(SW_HIDE);
			if (m_DlgInsureRecord.m_hWnd!=NULL) m_DlgInsureRecord.ShowWindow(SW_HIDE);	

			//���ý���
			m_DlgInsureSave.SetFocus();

			//����ѡ��
			m_TabButton[m_cbInsureMode]->SetCheck();

			//ˢ�½���
			InvalidateRect(NULL);

			break;
		}
	case INSURE_TRANSFER:		//ת�ʷ�ʽ
		{
			//���ñ���
			m_cbInsureMode=INSURE_TRANSFER;

			//���ÿؼ�
			if (m_DlgInsureTransfer.m_hWnd==NULL)
			{
				//��ȡλ��
				CRect rcClient;
				GetClientRect(&rcClient);

				//�����ؼ�
				CRect rcCreate(0,0,0,0);
				m_DlgInsureTransfer.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_INSURE_TRANSFER);

				//�ƶ�λ��
				CRect rcRound = m_pLabelRound->GetPos();
				m_DlgInsureTransfer.SetWindowPos(NULL,rcRound.left+5,rcRound.top+5,rcRound.Width()-10,rcRound.Height()-10,SWP_NOZORDER|SWP_SHOWWINDOW);
			}
			else
			{
				//��ʾ����
				m_DlgInsureTransfer.ShowWindow(SW_SHOW);
			}

			//���ÿؼ�
			if (m_DlgInsureSave.m_hWnd!=NULL) m_DlgInsureSave.ShowWindow(SW_HIDE);
			if (m_DlgInsureRecord.m_hWnd!=NULL) m_DlgInsureRecord.ShowWindow(SW_HIDE);			

			//���ý���
			m_DlgInsureTransfer.SetFocus();

			//����ѡ��
			m_TabButton[m_cbInsureMode]->SetCheck();

			//ˢ�½���
			InvalidateRect(NULL);

			break;
		}
	case INSURE_RECORD:		//������¼
		{
			//���ñ���
			m_cbInsureMode=INSURE_RECORD;

			//���ÿؼ�
			if (m_DlgInsureRecord.m_hWnd==NULL)
			{
				//��ȡλ��
				CRect rcClient;
				GetClientRect(&rcClient);

				//�����ؼ�
				CRect rcCreate(0,0,0,0);
				m_DlgInsureRecord.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_INSURE_RECORD);

				//�ƶ�λ��
				CRect rcRound = m_pLabelRound->GetPos();
				m_DlgInsureRecord.SetWindowPos(NULL,rcRound.left+5,rcRound.top+5,rcRound.Width()-10,rcRound.Height()-10,SWP_NOZORDER|SWP_SHOWWINDOW);
			}
			else
			{
				//��ʾ����				
				m_DlgInsureRecord.ShowWindow(SW_SHOW);				
				m_DlgInsureRecord.RefreshInsureRecord();
			}

			//���ÿؼ�
			if (m_DlgInsureSave.m_hWnd!=NULL) m_DlgInsureSave.ShowWindow(SW_HIDE);
			if (m_DlgInsureTransfer.m_hWnd!=NULL) m_DlgInsureTransfer.ShowWindow(SW_HIDE);

			//���ý���
			m_DlgInsureRecord.SetFocus();			

			//����ѡ��
			m_TabButton[m_cbInsureMode]->SetCheck();

			//ˢ�½���
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

//���ö���
VOID CDlgInsureOperater::SetDlgInsureMain(CDlgInsureMain * pDlgInsureMain)
{
	//���ö���
	m_pDlgInsureMain=pDlgInsureMain;
	m_DlgInsureSave.m_pDlgInsureMain=m_pDlgInsureMain;
	m_DlgInsureTransfer.m_pDlgInsureMain=m_pDlgInsureMain;
}

//������Ϣ
VOID CDlgInsureOperater::SetUserInsureInfo(tagUserInsureInfo * pUserInsureInfo)
{
	//���ö���
	m_pUserInsureInfo=pUserInsureInfo;
	m_DlgInsureSave.m_pUserInsureInfo=m_pUserInsureInfo;
	m_DlgInsureTransfer.m_pUserInsureInfo=m_pUserInsureInfo;
}

//��ѯ��ť
VOID CDlgInsureOperater::OnBnClickedQueryInfo()
{
	//��ѯ��Ϣ
	m_pDlgInsureMain->QueryUserInsureInfo();

	//���ý���
	switch (m_cbInsureMode)
	{
	case INSURE_SAVE:		//��ȡ��ʽ
		{
			m_DlgInsureSave.SetFocus();
			break;
		}
	case INSURE_TRANSFER:	//ת�ʷ�ʽ
		{
			m_DlgInsureTransfer.SetFocus();
			break;
		}
	}

	return;
}

//������Ϣ
int CDlgInsureOperater::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) return -1;

	//���ÿؼ�	
	m_DlgInsureSave.m_pDlgInsureOperater=this;
	m_DlgInsureRecord.m_pDlgInsureOperater=this;
	m_DlgInsureTransfer.m_pDlgInsureOperater=this;

	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//������Դ
	CPngImage ImageInsureInfo;
	ImageInsureInfo.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("INFO_FRAME"));

	//�滭��Ϣ
	INT nYPosInfo=86L;
	INT nXPosInfo=(rcClient.Width()-ImageInsureInfo.GetWidth())/2L+350;

	//��ȡģʽ
	SwitchInsureModeView(INSURE_SAVE);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
//���캯��
CDlgInsureSetup::CDlgInsureSetup()
{
	//���ñ���
	m_pDlgInsureMain=NULL;

	//�ؼ�ָ��
	m_pLabelRound=NULL;
	m_pEditLogonPass=NULL;
	m_pEditInsurePass=NULL;
	m_pEditInsurePass1=NULL;
	m_pButtonEnable=NULL;
}

//��������
CDlgInsureSetup::~CDlgInsureSetup()
{
}

//��ʼ�滭
VOID CDlgInsureSetup::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�滭����
	pDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);
}

//��ʼ�ؼ�
VOID CDlgInsureSetup::InitControlUI()
{
	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//��������
	CLabelUI * pLabelControl = NULL;

	//��������
	m_pLabelRound = CLabelUI::Create(&m_PaintManager,pParent,TEXT("")); 
	m_pLabelRound->SetBkImage(TEXT("file='IMAGE_ROUND_FRAME' restype='PNG' corner='6,6,6,6'"));
	m_pLabelRound->SetFixedWidth(451);
	m_pLabelRound->SetFixedHeight(306);
	m_pLabelRound->SetPos(4,4);	

	//��ǩ�ı�
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("��ܰ��ʾ��"));
	pLabelControl->SetFixedWidth(62);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetPos(69,64);

	//��ǩ�ı�
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("���������������룬�Ա㱣�����ĸ��˲Ʋ���ȫ"));
	pLabelControl->SetFixedWidth(300);
	pLabelControl->SetFixedHeight(18);
	pLabelControl->SetPos(99,85);

	//��ǩ�ı�
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("�����������룺"));
	pLabelControl->SetFixedWidth(84);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetPos(99,120);

	//��ǩ�ı�
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("ȷ���������룺"));
	pLabelControl->SetFixedWidth(84);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetPos(99,150);

	//��ǩ�ı�
	pLabelControl = CLabelUI::Create(&m_PaintManager,pParent,TEXT(""));
	pLabelControl->SetText(TEXT("ƽ̨��¼���룺"));
	pLabelControl->SetFixedWidth(84);
	pLabelControl->SetFixedHeight(17);
	pLabelControl->SetPos(99,180);	

	//��������
	m_pEditInsurePass = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditInsurePass->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG'"));
	m_pEditInsurePass->SetPasswordMode(true);
	m_pEditInsurePass->SetMaxChar(LEN_PASSWORD-1);
	m_pEditInsurePass->SetFixedXY(CPoint(185,114));
	m_pEditInsurePass->SetFixedWidth(170);
	m_pEditInsurePass->SetFixedHeight(24);		
	m_pEditInsurePass->SetBkColor(0);

	//��������
	m_pEditInsurePass1 = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditInsurePass1->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG'"));
	m_pEditInsurePass1->SetPasswordMode(true);
	m_pEditInsurePass1->SetMaxChar(LEN_PASSWORD-1);
	m_pEditInsurePass1->SetFixedXY(CPoint(185,144));
	m_pEditInsurePass1->SetFixedWidth(170);
	m_pEditInsurePass1->SetFixedHeight(24);	
	m_pEditInsurePass1->SetBkColor(0);

	//��¼����
	m_pEditLogonPass = CEditUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pEditLogonPass->SetNormalImage(TEXT("file='EDIT_SKIN_NORMAL' restype='PNG'"));
	m_pEditLogonPass->SetPasswordMode(true);
	m_pEditLogonPass->SetMaxChar(LEN_PASSWORD-1);
	m_pEditLogonPass->SetFixedXY(CPoint(185,174));
	m_pEditLogonPass->SetFixedWidth(170);
	m_pEditLogonPass->SetFixedHeight(24);		
	m_pEditLogonPass->SetBkColor(0);

	//��ͨ����
	m_pButtonEnable = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pButtonEnable->SetStatusImage(TEXT("file='BT_OPEN_INSURE' restype='PNG'"));	
	m_pButtonEnable->SetFixedXY(CPoint(154,224));
	m_pButtonEnable->SetWantReturn();
}

//��Ϣ����
VOID CDlgInsureSetup::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{		
		if(pControlUI==m_pButtonEnable) return OnBnClickedEnableInsure();
	}
}

//��ͨ����
VOID CDlgInsureSetup::OnBnClickedEnableInsure()
{
	//������Ϣ
	TCHAR szLogonPass[LEN_PASSWORD]=TEXT("");
	TCHAR szInsurePass1[LEN_PASSWORD]=TEXT("");
	TCHAR szInsurePass2[LEN_PASSWORD]=TEXT("");

	//��ȡ����
	lstrcpyn(szLogonPass,m_pEditLogonPass->GetText(),CountArray(szLogonPass));
	lstrcpyn(szInsurePass1,m_pEditInsurePass->GetText(),CountArray(szInsurePass1));
	lstrcpyn(szInsurePass2,m_pEditInsurePass1->GetText(),CountArray(szInsurePass2));

	//��������
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//�����ж�
	if (pUserItemElement->EfficacyPassword(szInsurePass1,szDescribe,CountArray(szDescribe))==false)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//���ý���
		m_pEditInsurePass->SetFocus();

		return;
	}

	//һ���ж�
	if(lstrcmp(szInsurePass1,szInsurePass2)!=0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��������������벻һ�£����������룡"),MB_ICONERROR,0);

		//���ý���
		m_pEditInsurePass1->SetFocus();

		return;
	}

	//��¼����
	if(szLogonPass[0] == 0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("������ƽ̨��¼���룡"),MB_ICONERROR,0);

		//���ý���
		m_pEditLogonPass->SetFocus();
		
		return;
	}

	//�������
	TCHAR szEnciryptInsurePass[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szInsurePass1,szEnciryptInsurePass);

	//�������
	TCHAR szEnciryptLogonPass[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szLogonPass,szEnciryptLogonPass);

	//ִ�п�ͨ
	if(m_pDlgInsureMain!=NULL) m_pDlgInsureMain->PerformEnableInsure(szEnciryptInsurePass,szEnciryptLogonPass);
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgInsureMain::CDlgInsureMain() : CSkinDialog(IDD_INSURE_MAIN)
{
	//�������
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bEnableMission=false;

	//���ñ���
	m_bInitInfo=false;
	ZeroMemory(&m_UserInsureInfo,sizeof(m_UserInsureInfo));

	return;
}

//��������
CDlgInsureMain::~CDlgInsureMain()
{
}

//�ؼ���
VOID CDlgInsureMain::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��Ϣ����
BOOL CDlgInsureMain::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if (pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)	return TRUE;
		if(pMsg->wParam==VK_RETURN)	return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//��������
BOOL CDlgInsureMain::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("����"));

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//��������
	CSkinDialog::SetWndFont(this,NULL);

	//��������
	CRect rcCreate(0,0,0,0);

	//��������
	m_DlgInsureSetup.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_INSURE_SETUP);
	m_DlgInsureOperater.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_INSURE_OPERATER);

	//���ö���
	m_DlgInsureSetup.SetDlgInsureMain(this);
	m_DlgInsureOperater.SetDlgInsureMain(this);
	m_DlgInsureOperater.SetUserInsureInfo(&m_UserInsureInfo);

	//���ô���
	SetWindowPos(NULL,0,0,471,362,SWP_NOZORDER|SWP_NOMOVE);

	//��������
	RectifyInsureStep();

	return FALSE;
}

//ȷ����ť
VOID CDlgInsureMain::OnOK()
{
	return __super::OnOK();
}

//�滭����
VOID CDlgInsureMain::OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight)
{
	//������Դ
	CPngImage ImageTitle;
	ImageTitle.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("INSURE_TITLE"));

	//���Ʊ���
	ImageTitle.DrawImage(pDC,10,10);
}

//�滭��Ϣ
VOID CDlgInsureMain::OnDrawClientAreaBegin(CDC * pDC, INT nWidth, INT nHeight)
{
	//��ȡλ��
	CRect rcTabControl;
	//m_TabControl.GetWindowRect(&rcTabControl);

	return;

	//ת��λ��
	ScreenToClient(&rcTabControl);

	//�滭����
	INT nLBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;
	pDC->FillSolidRect(nLBorder,rcTabControl.bottom-1L,nWidth-nLBorder-nRBorder,1,RGB(145,168,183));

	//������Դ
	CPngImage ImageInsureInfo;
	ImageInsureInfo.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("INFO_FRAME"));

	//�滭��Ϣ
	INT nYPosInfo=75L;
	INT nXPosInfo=(nWidth-ImageInsureInfo.GetWidth())/2L;
	ImageInsureInfo.DrawImage(pDC,nXPosInfo,nYPosInfo,ImageInsureInfo.GetWidth(),ImageInsureInfo.GetHeight(),0,0);

	//������Ϣ
	//if (m_bInitInfo==true)
	//{
	//	//������Դ
	//	CPngImage ImageInsureIcon;
	//	ImageInsureIcon.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("INSURE_ICON"));

	//	//�滭��־
	//	ImageInsureIcon.DrawImage(pDC,nXPosInfo+10,nYPosInfo+17);

	//	//�滭����
	//	m_DlgInsureItem.DrawNumberString(pDC,m_UserInsureInfo.lUserScore,nXPosInfo+136,nYPosInfo+18L);
	//	m_DlgInsureItem.DrawNumberString(pDC,m_UserInsureInfo.lUserInsure,nXPosInfo+136,nYPosInfo+53L);

	//	//�滭��ʾ
	//	LPCTSTR pszScoreString=TEXT("��Ϸ�ң�");
	//	LPCTSTR pszInsureString=TEXT("���չ�");
	//	pDC->TextOut(nXPosInfo+83,nYPosInfo+20L,pszScoreString,lstrlen(pszScoreString));
	//	pDC->TextOut(nXPosInfo+83,nYPosInfo+55L,pszInsureString,lstrlen(pszInsureString));

	//	//������ʾ
	//	TCHAR szString[128]=TEXT("");
	//	if(m_cbInsureMode==INSURE_SAVE)
	//	{
	//		_sntprintf(szString,CountArray(szString),TEXT("��ܰ��ʾ��������Ϸ���������ѣ�ȡ�����۳�%ld���������"),			m_UserInsureInfo.wRevenueTake);
	//	}
	//	else if(m_cbInsureMode==INSURE_TRANSFER)
	//	{
	//		_sntprintf(szString,CountArray(szString),TEXT("��ܰ��ʾ�����ͽ��۳�%ld���������"),m_UserInsureInfo.wRevenueTransfer);
	//	}

	//	//��ܰ��ʾ
	//	pDC->SetTextColor(RGB(50,50,50));
	//	pDC->TextOut(18,nHeight-28,szString,lstrlen(szString));
	//}
	//else
	//{
	//}

	return;
}

//��ʼ����
VOID CDlgInsureMain::OnMissionStart()
{
	return;
}

//��ֹ����
VOID CDlgInsureMain::OnMissionConclude()
{
	return;
}

//��������
VOID CDlgInsureMain::RectifyInsureStep()
{
	//��ȡ����
	CRect rcClient;
	GetClientRect(&rcClient);

	//������Ϣ
	tagEncircleInfo EncircleInfoFrame = CSkinDialog::m_SkinAttribute.m_EncircleInfoFrame;

	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//����׼��
	UINT nFlagEx=0;
	HDWP hDwp = BeginDeferWindowPos(32);	

	//��������
	nFlagEx = pGlobalUserData->cbInsureEnabled==TRUE?SWP_HIDEWINDOW:SWP_SHOWWINDOW;
	DeferWindowPos(hDwp,m_DlgInsureSetup,NULL,EncircleInfoFrame.nLBorder,EncircleInfoFrame.nTBorder,rcClient.Width()-EncircleInfoFrame.nLBorder-EncircleInfoFrame.nRBorder,rcClient.Height()-EncircleInfoFrame.nTBorder-EncircleInfoFrame.nBBorder,SWP_NOZORDER|nFlagEx);

	//��������
	nFlagEx = pGlobalUserData->cbInsureEnabled==FALSE?SWP_HIDEWINDOW:SWP_SHOWWINDOW;
	DeferWindowPos(hDwp,m_DlgInsureOperater,NULL,EncircleInfoFrame.nLBorder,EncircleInfoFrame.nTBorder,rcClient.Width()-EncircleInfoFrame.nLBorder-EncircleInfoFrame.nRBorder,rcClient.Height()-EncircleInfoFrame.nTBorder-EncircleInfoFrame.nBBorder,SWP_NOZORDER|nFlagEx);

	//��������
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	//��ѯ����
	if(pGlobalUserData->cbInsureEnabled==TRUE) QueryUserInsureInfo();
}

//��ѯ����
VOID CDlgInsureMain::QueryUserInsureInfo()
{
	//���ñ���
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=true;
	m_bTransferMission=false;
	m_bEnableMission=false;

	//�¼�����
	OnMissionStart();

	//ִ�в�ѯ
	PerformQueryInfo();

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgInsurePlaza::CDlgInsurePlaza()
{
	//��������
	m_MissionManager.InsertMissionItem(this);

	//���ñ���
	m_lScore=0L;
	m_cbByNickName=TRUE;
	
	//���ñ���
	m_szAccounts[0]=0;
	m_szLogonPass[0]=0;
	m_szInsurePass[0]=0;
	m_szTransRemark[0]=0;

	return;
}

//��������
CDlgInsurePlaza::~CDlgInsurePlaza()
{
}

//��ѯ�¼�
VOID CDlgInsurePlaza::PerformQueryInfo()
{
	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//��ͨ�¼�
VOID CDlgInsurePlaza::PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass)
{
	//���ñ���
	m_bEnableMission=true;
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bTransferMission=false;

	//���ñ���
	lstrcpyn(m_szLogonPass,pszLogonPass,CountArray(m_szLogonPass));
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));	

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//�����¼�
VOID CDlgInsurePlaza::PerformSaveScore(SCORE lSaveScore)
{
	//���ñ���	
	m_bSaveMission=true;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bEnableMission=false;
	m_bTransferMission=false;

	//���ñ���
	m_lScore=lSaveScore;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//ȡ���¼�
VOID CDlgInsurePlaza::PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)
{
	//���ñ���	
	m_bTakeMission=true;
	m_bSaveMission=false;	
	m_bQueryMission=false;
	m_bEnableMission=false;
	m_bTransferMission=false;

	//���ñ���
	m_lScore=lTakeScore;
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//ת���¼�
VOID CDlgInsurePlaza::PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName)
{
	//���ñ���
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;
	m_bEnableMission=false;
	m_bTransferMission=true;

	//���ñ���
	m_lScore=lTransferScore;
	m_cbByNickName=cbByNickName;
	lstrcpyn(m_szAccounts,pszAccounts,CountArray(m_szAccounts));
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));
	lstrcpyn(m_szTransRemark,pszTransRemark,CountArray(m_szTransRemark));

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//�����¼�
bool CDlgInsurePlaza::OnEventMissionLink(INT nErrorCode)
{
	//���ӽ��
	if (nErrorCode!=0)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//�¼�����
		OnMissionConclude();

		//������ʾ
		if (m_bEnableMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����ͨ����ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_bSaveMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����Ϸ�Ҵ���ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_bTakeMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����Ϸ��ȡ��ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_bQueryMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ���������ϻ�ȡʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_bTransferMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����Ϸ������ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}

		return true;
	}

	//��ͨ����
	if (m_bEnableMission==true)
	{
		//��������
		CMD_GP_UserEnableInsure UserEnableInsure;
		ZeroMemory(&UserEnableInsure,sizeof(UserEnableInsure));

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//���ñ���
		UserEnableInsure.dwUserID=pGlobalUserData->dwUserID;
		lstrcpyn(UserEnableInsure.szLogonPass,m_szLogonPass,CountArray(UserEnableInsure.szLogonPass));
		lstrcpyn(UserEnableInsure.szInsurePass,m_szInsurePass,CountArray(UserEnableInsure.szInsurePass));		
		CWHService::GetMachineIDEx(UserEnableInsure.szMachineID);

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_ENABLE_INSURE,&UserEnableInsure,sizeof(UserEnableInsure));

		return true;
	}

	//��������
	if (m_bSaveMission==true)
	{
		//��������
		CMD_GP_UserSaveScore UserSaveScore;
		ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//���ñ���
		UserSaveScore.lSaveScore=m_lScore;
		UserSaveScore.dwUserID=pGlobalUserData->dwUserID;
		CWHService::GetMachineIDEx(UserSaveScore.szMachineID);

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_SAVE_SCORE,&UserSaveScore,sizeof(UserSaveScore));

		return true;
	}

	//ȡ������
	if (m_bTakeMission==true)
	{
		//��������
		CMD_GP_UserTakeScore UserTakeScore;
		ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//���ñ���
		UserTakeScore.lTakeScore=m_lScore;
		UserTakeScore.dwUserID=pGlobalUserData->dwUserID;
		CWHService::GetMachineIDEx(UserTakeScore.szMachineID);
		lstrcpyn(UserTakeScore.szPassword,m_szInsurePass,CountArray(UserTakeScore.szPassword));

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_TAKE_SCORE,&UserTakeScore,sizeof(UserTakeScore));

		return true;
	}

	//ת������
	if (m_bTransferMission==true)
	{
		//��������
		CMD_GP_QueryUserInfoRequest QueryUserInfoRequest;
		ZeroMemory(&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));

		//���ñ���
		QueryUserInfoRequest.cbByNickName=m_cbByNickName;
		lstrcpyn(QueryUserInfoRequest.szAccounts,m_szAccounts,CountArray(QueryUserInfoRequest.szAccounts));

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_USER_INFO_REQUEST,&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));
	}

	//��ѯ����
	if (m_bQueryMission==true)
	{
		//��������
		CMD_GP_QueryInsureInfo QueryInsureInfo;
		ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

		//���ñ���
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		QueryInsureInfo.dwUserID=pGlobalUserInfo->GetGlobalUserData()->dwUserID;
		lstrcpyn(QueryInsureInfo.szPassword,pGlobalUserInfo->GetGlobalUserData()->szPassword,CountArray(QueryInsureInfo.szPassword));

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));
	}

	return true;
}

//�ر��¼�
bool CDlgInsurePlaza::OnEventMissionShut(BYTE cbShutReason)
{
	//��ʾ��Ϣ
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//������ʾ
		if (m_bEnableMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ����ͨ����ʧ�ܣ����Ժ��ٴ����ԣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//��������
		if (m_bSaveMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ���Ϸ�Ҵ���ʧ�ܣ�"),MB_ICONERROR,30);
		}

		//ȡ������
		if (m_bTakeMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ���Ϸ��ȡ��ʧ�ܣ�"),MB_ICONERROR,30);
		}

		//��ѯ����
		if (m_bQueryMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ��������ϻ�ȡʧ�ܣ�"),MB_ICONERROR,30);
		}
	}

	//�¼�����
	OnMissionConclude();

	return true;
}

//��ȡ�¼�
bool CDlgInsurePlaza::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_USER_INSURE_INFO:		//��������
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GP_UserInsureInfo));
				if (wDataSize<sizeof(CMD_GP_UserInsureInfo)) return false;

				//��������
				CMD_GP_UserInsureInfo * pUserInsureInfo=(CMD_GP_UserInsureInfo *)pData;

				//���ñ���
				m_bInitInfo=true;
				m_UserInsureInfo.cbEnjoinTransfer=pUserInsureInfo->cbEnjoinTransfer;
				m_UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
				m_UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
				m_UserInsureInfo.wRevenueTransferMember=pUserInsureInfo->wRevenueTransferMember;
				m_UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
				m_UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
				m_UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//���´���
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//����ṹ
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureInfo->lUserScore;
				InsureEvent.lUserInsure=pUserInsureInfo->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);

				//�л�ҳ��
				if(m_DlgInsureOperater.m_cbInsureMode==INSURE_TRANSFER && m_UserInsureInfo.cbEnjoinTransfer!=1)
				{
					m_DlgInsureOperater.SwitchInsureModeView(INSURE_SAVE);
				}

				//����ҳ��				
				if(m_UserInsureInfo.cbEnjoinTransfer==1)
				{
					m_DlgInsureOperater.InsertTabItem(INSURE_MASK_TRANSFER);
				}
				else
				{
					m_DlgInsureOperater.DeleteTabItem(INSURE_MASK_TRANSFER);
				}

				//���½���
				m_DlgInsureOperater.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				return true;
			}
		case SUB_GP_USER_INSURE_ENABLE_RESULT:	//��ͨ���
			{
				//Ч�����
				ASSERT(wDataSize<=sizeof(CMD_GP_UserInsureEnableResult));
				if (wDataSize>sizeof(CMD_GP_UserInsureEnableResult)) return false;

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//��������
				CMD_GP_UserInsureEnableResult * pUserInsureEnableResult=(CMD_GP_UserInsureEnableResult *)pData;

				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//���ñ���
				pGlobalUserData->cbInsureEnabled = pUserInsureEnableResult->cbInsureEnabled;

				//��ʾ��Ϣ
				if (pUserInsureEnableResult->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureEnableResult->szDescribeString,MB_ICONINFORMATION,60);
				}

				//��������
				RectifyInsureStep();

				return true;
			}
		case SUB_GP_USER_INSURE_SUCCESS:	//���гɹ�
			{
				//Ч�����
				CMD_GP_UserInsureSuccess * pUserInsureSuccess=(CMD_GP_UserInsureSuccess *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

				//���ñ���
				m_UserInsureInfo.lUserScore=pUserInsureSuccess->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureSuccess->lUserInsure;

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//���ý���
				if(m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass != NULL)
				{
					m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetFocus();
					m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetText(TEXT(""));
				}

				//���ý���
				if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
				{
					m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
					m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
				}

				//���´���
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//����ṹ
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureSuccess->lUserScore;
				InsureEvent.lUserInsure=pUserInsureSuccess->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);				

				//���½���
				m_DlgInsureOperater.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				//��ʾ��Ϣ
				if (pUserInsureSuccess->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}				

				return true;
			}
		case SUB_GP_USER_INSURE_FAILURE:	//����ʧ��
			{
				//Ч�����
				CMD_GP_UserInsureFailure * pUserInsureFailure=(CMD_GP_UserInsureFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

				//ʧ�ܴ���
				OnMissionConclude();

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//���ý���
				if(m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass!= NULL)
				{
					m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetFocus();
					m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetText(TEXT(""));
				}

				//���ý���
				if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
				{
					m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
					m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
				}

				//��ʾ��Ϣ
				if (pUserInsureFailure->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		case SUB_GP_QUERY_USER_INFO_RESULT:	//�û���Ϣ
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GP_UserTransferUserInfo));
				if (wDataSize<sizeof(CMD_GP_UserTransferUserInfo)) return false;

				//��������
				CMD_GP_UserTransferUserInfo * pTransferUserInfo=(CMD_GP_UserTransferUserInfo *)pData;

				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//��������
				WORD wRevinueTransfer=pGlobalUserData->cbMemberOrder?m_UserInsureInfo.wRevenueTransferMember:m_UserInsureInfo.wRevenueTransfer;

				//������Ϣ
				TCHAR szScore[16]={0};
				TCHAR szMessage[128]={0};	
				CDlgInsureItem::SwitchScoreFormat(m_lScore,3,szScore,CountArray(szScore));
				_sntprintf(szMessage, CountArray(szMessage), TEXT("������Ϸ����ȡ%d��������,��ȷ��Ҫ��[%s], ID:%d ����%s ��Ϸ����?"), wRevinueTransfer,pTransferUserInfo->szAccounts, pTransferUserInfo->dwTargetGameID,szScore);

				//��ʾ��Ϣ
				CInformation Information(this);
				if (Information.ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO,0)==IDYES)
				{
					//��������
					CMD_GP_UserTransferScore UserTransferScore;
					ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

					//��������
					CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
					tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

					//���ñ���
					UserTransferScore.lTransferScore=m_lScore;
					UserTransferScore.dwUserID=pGlobalUserData->dwUserID;
					CWHService::GetMachineIDEx(UserTransferScore.szMachineID);					
					lstrcpyn(UserTransferScore.szPassword,m_szInsurePass,CountArray(UserTransferScore.szPassword));
					lstrcpyn(UserTransferScore.szTransRemark,m_szTransRemark,CountArray(UserTransferScore.szTransRemark));
					lstrcpyn(UserTransferScore.szAccounts,pTransferUserInfo->szAccounts,CountArray(UserTransferScore.szAccounts));

					//��������
					m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_TRANSFER_SCORE,&UserTransferScore,sizeof(UserTransferScore));
				}

				return true;
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgInsureServer::CDlgInsureServer()
{
	//���ñ���
	m_cbByNickName=TRUE;
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;

	//���ñ���
	m_szAccounts[0]=0;
	m_szLogonPass[0]=0;
	m_szInsurePass[0]=0;
	m_szTransRemark[0]=0;

	return;
}

//��������
CDlgInsureServer::~CDlgInsureServer()
{
}

//��ѯ�¼�
VOID CDlgInsureServer::PerformQueryInfo()
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	CMD_GR_C_QueryInsureInfoRequest QueryInsureInfo;
	ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

	//���ñ���
	QueryInsureInfo.cbActivityGame=FALSE;
	lstrcpyn(QueryInsureInfo.szInsurePass,pGlobalUserData->szPassword,CountArray(QueryInsureInfo.szInsurePass));

	//��������
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));

	return;
}

//��ͨ�¼�
VOID CDlgInsureServer::PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass)
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	CMD_GR_C_EnableInsureRequest EnableInsureResult;
	ZeroMemory(&EnableInsureResult,sizeof(EnableInsureResult));

	//���ñ���
	EnableInsureResult.cbActivityGame=FALSE;
	EnableInsureResult.dwUserID=pGlobalUserData->dwUserID;
	CWHService::GetMachineIDEx(EnableInsureResult.szMachineID);
	lstrcpyn(EnableInsureResult.szLogonPass,pszLogonPass,CountArray(EnableInsureResult.szLogonPass));
	lstrcpyn(EnableInsureResult.szInsurePass,pszInsurePass,CountArray(EnableInsureResult.szInsurePass));		

	//��������
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_ENABLE_INSURE_REQUEST,&EnableInsureResult,sizeof(EnableInsureResult));

	return;
}

//�����¼�
VOID CDlgInsureServer::PerformSaveScore(SCORE lSaveScore)
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��������
	CMD_GR_C_SaveScoreRequest SaveScoreRequest;
	ZeroMemory(&SaveScoreRequest,sizeof(SaveScoreRequest));

	//���ñ���
	SaveScoreRequest.lSaveScore=lSaveScore;

	//��������
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_SAVE_SCORE_REQUEST,&SaveScoreRequest,sizeof(SaveScoreRequest));

	return;
}

//ȡ���¼�
VOID CDlgInsureServer::PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//��������
	CMD_GR_C_TakeScoreRequest TakeScoreRequest;
	ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));

	//���ñ���
	TakeScoreRequest.lTakeScore=lTakeScore;
	lstrcpyn(TakeScoreRequest.szInsurePass,pszInsurePass,CountArray(TakeScoreRequest.szInsurePass));

	//��������
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));

	return;
}

//ת���¼�
VOID CDlgInsureServer::PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName)
{
	//Ч�����
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//���ñ���
	m_lScore=lTransferScore;
	m_cbByNickName=cbByNickName;
	lstrcpyn(m_szAccounts,pszAccounts,CountArray(m_szAccounts));
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));
	lstrcpyn(m_szTransRemark,pszTransRemark,CountArray(m_szTransRemark));

	//��������
	CMD_GR_C_QueryUserInfoRequest QueryUserInfoRequest;	
	ZeroMemory(&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));

	//���ñ���
	QueryUserInfoRequest.cbByNickName=m_cbByNickName;
	lstrcpyn(QueryUserInfoRequest.szAccounts,pszAccounts,CountArray(QueryUserInfoRequest.szAccounts));

	//��������
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_QUERY_USER_INFO_REQUEST,&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));

	return;
}

//��������
VOID CDlgInsureServer::Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem)
{
	//���ñ���
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;

	return;
}

//������Ϣ
bool CDlgInsureServer::OnServerInsureMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��Ϣ�ж�
	if (wMainCmdID==MDM_GR_INSURE)
	{
		switch (wSubCmdID)
		{
		case SUB_GR_USER_INSURE_INFO:		//��������
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GR_S_UserInsureInfo));
				if (wDataSize<sizeof(CMD_GR_S_UserInsureInfo)) return false;

				//��������
				CMD_GR_S_UserInsureInfo * pUserInsureInfo=(CMD_GR_S_UserInsureInfo *)pData;

				//���ñ���
				m_bInitInfo=true;
				m_UserInsureInfo.cbEnjoinTransfer=pUserInsureInfo->cbEnjoinTransfer;
				m_UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
				m_UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
				m_UserInsureInfo.wRevenueTransferMember=pUserInsureInfo->wRevenueTransferMember;
				m_UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
				m_UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
				m_UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

				//���´���
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//����ṹ
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureInfo->lUserScore;
				InsureEvent.lUserInsure=pUserInsureInfo->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);

				//�л�ҳ��
				if(m_DlgInsureOperater.m_cbInsureMode==INSURE_TRANSFER && m_UserInsureInfo.cbEnjoinTransfer!=1)
				{
					m_DlgInsureOperater.SwitchInsureModeView(INSURE_SAVE);
				}

				//����ҳ��				
				if(m_UserInsureInfo.cbEnjoinTransfer==1)
				{
					m_DlgInsureOperater.InsertTabItem(INSURE_MASK_TRANSFER);
				}
				else
				{
					m_DlgInsureOperater.DeleteTabItem(INSURE_MASK_TRANSFER);
				}

				//���½���
				m_DlgInsureOperater.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				return true;
			}
		case SUB_GR_USER_INSURE_ENABLE_RESULT:	//��ͨ���
			{
				//Ч�����
				ASSERT(wDataSize<=sizeof(CMD_GR_S_UserInsureEnableResult));
				if (wDataSize>sizeof(CMD_GR_S_UserInsureEnableResult)) return false;

				//��������
				CMD_GR_S_UserInsureEnableResult * pUserInsureEnableResult=(CMD_GR_S_UserInsureEnableResult *)pData;

				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//���ñ���
				pGlobalUserData->cbInsureEnabled = pUserInsureEnableResult->cbInsureEnabled;

				//��ʾ��Ϣ
				if (pUserInsureEnableResult->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureEnableResult->szDescribeString,MB_ICONINFORMATION,60);
				}

				//��������
				RectifyInsureStep();

				return true;
			}
		case SUB_GR_USER_INSURE_SUCCESS:	//���гɹ�
			{
				//Ч�����
				CMD_GR_S_UserInsureSuccess * pUserInsureSuccess=(CMD_GR_S_UserInsureSuccess *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

				//���ñ���
				m_UserInsureInfo.lUserScore=pUserInsureSuccess->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureSuccess->lUserInsure;

				//���´���
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//����ṹ
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureSuccess->lUserScore;
				InsureEvent.lUserInsure=pUserInsureSuccess->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);

				//���ý���
				if(m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass!= NULL)
				{
					m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetFocus();
					m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetText(TEXT(""));
				}

				//���ý���
				if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
				{
					m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
					m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
				}

				//���½���
				m_DlgInsureOperater.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				//��ʾ��Ϣ
				if (pUserInsureSuccess->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}				

				return true;
			}
		case SUB_GR_USER_INSURE_FAILURE:	//����ʧ��
			{
				//Ч�����
				CMD_GR_S_UserInsureFailure * pUserInsureFailure=(CMD_GR_S_UserInsureFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GR_S_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

				//ʧ�ܴ���
				OnMissionConclude();

				//���ÿؼ�
				if (m_DlgInsureOperater.m_bInitInfo==true)
				{
					//���ý���
					if(m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass != NULL)
					{
						m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetFocus();
						m_DlgInsureOperater.m_DlgInsureSave.m_pEditInsurePass->SetText(TEXT(""));
					}

					//���ý���
					if(m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass!= NULL)
					{
						m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetFocus();
						m_DlgInsureOperater.m_DlgInsureTransfer.m_pEditInsurePass->SetText(TEXT(""));
					}
				}

				//��ʾ��Ϣ
				if (pUserInsureFailure->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		case SUB_GR_USER_TRANSFER_USER_INFO:  //�û���Ϣ
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GR_S_UserTransferUserInfo));
				if (wDataSize<sizeof(CMD_GR_S_UserTransferUserInfo)) return false;

				//��������
				CMD_GR_S_UserTransferUserInfo * pTransferUserInfo=(CMD_GR_S_UserTransferUserInfo *)pData;

				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//��������
				WORD wRevinueTransfer=pGlobalUserData->cbMemberOrder?m_UserInsureInfo.wRevenueTransferMember:m_UserInsureInfo.wRevenueTransfer;

				//������Ϣ
				TCHAR szScore[16]={0};
				TCHAR szMessage[128]={0};	
				CDlgInsureItem::SwitchScoreFormat(m_lScore,3,szScore,CountArray(szScore));
				_sntprintf(szMessage, CountArray(szMessage), TEXT("������Ϸ����ȡ%d��������,��ȷ��Ҫ��[%s], ID:%d ����%s ��Ϸ����?"), wRevinueTransfer,pTransferUserInfo->szAccounts, pTransferUserInfo->dwTargetGameID,szScore);

				//��ʾ��Ϣ
				CInformation Information(this);
				if (Information.ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO,0)==IDYES)
				{
					//��������
					CMD_GP_C_TransferScoreRequest TransferScoreRequest;
					ZeroMemory(&TransferScoreRequest,sizeof(TransferScoreRequest));

					//���ñ���
					TransferScoreRequest.lTransferScore=m_lScore;					
					lstrcpyn(TransferScoreRequest.szInsurePass,m_szInsurePass,CountArray(TransferScoreRequest.szInsurePass));
					lstrcpyn(TransferScoreRequest.szTransRemark,m_szTransRemark,CountArray(TransferScoreRequest.szTransRemark));
					lstrcpyn(TransferScoreRequest.szAccounts,pTransferUserInfo->szAccounts,CountArray(TransferScoreRequest.szAccounts));

					//��������
					m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_TRANSFER_SCORE_REQUEST,&TransferScoreRequest,sizeof(TransferScoreRequest));
				}
				return true;
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

