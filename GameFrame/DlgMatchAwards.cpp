#include "StdAfx.h"
#include "Resource.h"
#include "Dlgmatchawards.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//��Ļλ��
#define LAYERED_SIZE						5									//�ֲ��С

//��������
#define ROUND_CX							6									//Բ�Ǵ�С
#define ROUND_CY							6									//Բ�Ǵ�С

//////////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgMatchAwards::CDlgMatchAwards() : CFGuiDialog(IDD_DLG_MATCH_AWARDS)
{
	//���ñ���
	m_pMatchResult=NULL;
	m_pbtClose=NULL;
}

CDlgMatchAwards::~CDlgMatchAwards()
{

}

//���ò���
VOID CDlgMatchAwards::SetMatchResult(CMD_GR_MatchResult *pMatchResult) 
{ 
	//���ñ���
	m_pMatchResult=pMatchResult; 

	//ˢ�½���
	InvalidateRect(NULL);
}

//��ʼ�ؼ�
VOID CDlgMatchAwards::InitControlUI()
{
	//���ñ���
	CRect rcCaption(0,0,0,200);
	m_PaintManager.SetCaptionRect(rcCaption);

	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//���ذ�ť
	m_pbtClose = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtClose->SetStatusImage(TEXT("file='BT_REWARD_CLOSE' restype='PNG'"));	
	m_pbtClose->SetRelativeSide(RelativeRight);
	m_pbtClose->SetRelativePos(CPoint(36,10));
}

//��Ϣ����
VOID CDlgMatchAwards::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//�˳���ť
		if(m_pbtClose==pControlUI) return OnCancel(); 
	}
}

//��ʼ�滭
VOID CDlgMatchAwards::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//���û���
	pDC->SetBkMode(TRANSPARENT);

	//��ȡ����
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CPngImage ImageStar,ImageAwards;

	//����ͼƬ
	ImageStar.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME), TEXT("MATCH_STAR"));
	ImageAwards.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME), TEXT("MATCH_AWARDS"));

	//����λ��
	INT nXPos=(rcClient.Width()-ImageAwards.GetWidth())/2;
	INT nYPos=(rcClient.Height()-ImageAwards.GetHeight())/2+10;

	//�滭����
	ImageAwards.DrawImage(pDC,0,0);

	//�滭����
	if(m_pMatchResult!=NULL)
	{	
		//��������
		pDC->SelectObject(m_PaintManager.GetFont(4));
		pDC->SetTextColor(RGB(255,255,255));

		//�滭����
		DrawMultLineText(pDC,m_pMatchResult->szDescribe,&CRect(nXPos+50,nYPos+100,rcClient.Width()-(nXPos+50),nYPos+180),8,DT_TOP|DT_WORDBREAK|DT_EXTERNALLEADING);

		//������ɫ
		pDC->SetTextColor(RGB(255,255,0));

		//��������
		int nIndex=0;
		CString strText;

		//��Ϸ�ҽ���
		if(m_pMatchResult->lGold>0)
		{
			ImageStar.DrawImage(pDC,nXPos+60,nYPos+160-2);
			strText.Format(TEXT("��Ϸ�ң�%d"), m_pMatchResult->lGold);			
			pDC->DrawText(strText, &CRect(nXPos+95,nYPos+160,nXPos+360,nYPos+185),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}

		//Ԫ������
		if(m_pMatchResult->dwIngot>0)
		{
			ImageStar.DrawImage(pDC,nXPos+60,nYPos+160+nIndex*30-2);
			strText.Format(TEXT("Ԫ  ����%d"), m_pMatchResult->dwIngot);
			pDC->DrawText(strText, &CRect(nXPos+95,nYPos+160+nIndex*30,nXPos+360,nYPos+185+nIndex*30),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}

		//���齱��
		if(m_pMatchResult->dwExperience>0)
		{
			ImageStar.DrawImage(pDC,nXPos+60,nYPos+160+nIndex*30-2);
			strText.Format(TEXT("��  �飺%d"), m_pMatchResult->dwExperience);
			pDC->DrawText(strText, &CRect(nXPos+95,nYPos+160+nIndex*30,nXPos+360,nYPos+185+nIndex*30),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}
	}
}

BOOL CDlgMatchAwards::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME), TEXT("MATCH_AWARDS"));

	//��������
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());

	//���ô���
	SetWindowPos(NULL, 0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//���д���
	CenterWindow();

	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	CRect rcUnLayered;
	rcUnLayered.left=10;
	rcUnLayered.top=8;	
	rcUnLayered.right=rcWindow.Width()-ROUND_CX;
	rcUnLayered.bottom=rcWindow.Height()-ROUND_CY;

	//��������
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_CX,ROUND_CY);

	//��������
	SetWindowRgn(RgnWindow,FALSE);

	//�ֲ㴰��
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return TRUE; 
}

//ȷ������
VOID CDlgMatchAwards::OnCancel()
{
	__super::OnCancel();

	//���ٴ���
	DestroyWindow();
}

//���ƶ����ı�
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