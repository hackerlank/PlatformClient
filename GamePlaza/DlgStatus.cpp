#include "StdAfx.h"
#include "DlgStatus.h"
#include "Resource.h"
#include "ServerListData.h"
#include "PlatformFrame.h"


//////////////////////////////////////////////////////////////////////////////////

//Բ�Ǵ�С
#define ROUND_CX					5									//Բ�ǿ��
#define ROUND_CY					5									//Բ�Ǹ߶�

//ʱ���ʶ
#define IDT_STATUS					10									//״̬ʱ��
#define TIMER_STATUS				20									//״̬ʱ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgStatus, CFGuiDialog)

	//ϵͳ��Ϣ
	ON_WM_TIMER()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgStatus::CDlgStatus() : CFGuiDialog(IDD_DLG_POPUP)
{
	//״̬����
	m_uImageIndex=0;

	//�ؼ�����
	m_pbtGiveUp=NULL;

	//�ӿڱ���
	m_pIStatusViewSink=NULL;

	return;
}

//��������
CDlgStatus::~CDlgStatus()
{
}

//��Ϣ����
BOOL CDlgStatus::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//���ú���
BOOL CDlgStatus::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(szProduct);

	//���ÿؼ�
	SetTimer(IDT_STATUS,TIMER_STATUS,NULL);

	//������Դ
	CPngImage ImageStatus;
	ImageStatus.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_STATUS_BACK"));

	//���ô�С
	CSize SizeWindow(ImageStatus.GetWidth(),ImageStatus.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//���д���
	CenterWindow();

	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	CRect rcUnLayered;
	rcUnLayered.top=5;
	rcUnLayered.left=5;
	rcUnLayered.right=rcWindow.Width()-5;
	rcUnLayered.bottom=rcWindow.Height()-5;

	//��������
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_CX,ROUND_CY);

	//��������
	SetWindowRgn(RgnWindow,FALSE);

	//�ֲ㴰��
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageStatus,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return TRUE;
}

//��ʼ�ؼ�
void CDlgStatus::InitControlUI()
{
	//��������
	m_PaintManager.SetCaptionRect(CRect(0,0,0,67));

	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//�鿴��ť
	m_pbtGiveUp = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtGiveUp->SetStatusImage(TEXT("file='BT_STATUS_CANCLE' restype='PNG'"));
	m_pbtGiveUp->SetPos(CRect(329,14,353,37));
}

//��Ϣ����
void CDlgStatus::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(m_pbtGiveUp==pControlUI) 
		{
			return OnBnClickedGiveUp(); 
		}
	}
}

//��ʼ�滭
void CDlgStatus::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//��������
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//�滭����
	CPngImage ImageStatus;
	ImageStatus.LoadImage(hInstance,TEXT("DLG_STATUS_BACK"));
	ImageStatus.DrawImage(pDC,0,0,rcClient.Width(),rcClient.Height(),0,0);

	//������Դ
	CPngImage ImageStatusLine;
	CPngImage ImageStatusPoint;
	ImageStatusLine.LoadImage(hInstance,TEXT("STATUS_LINE"));
	ImageStatusPoint.LoadImage(hInstance,TEXT("STATUS_POINT"));

	//��ȡ��С
	CSize SizeStatusLine;
	CSize SizeStatusPoint;
	SizeStatusLine.SetSize(ImageStatusLine.GetWidth(),ImageStatusLine.GetHeight()/2);
	SizeStatusPoint.SetSize(ImageStatusPoint.GetWidth(),ImageStatusPoint.GetHeight());

	//�滭����
	ImageStatusLine.DrawImage(pDC,16,17,SizeStatusLine.cx,SizeStatusLine.cy,0,0);
	ImageStatusLine.DrawImage(pDC,16,17,SizeStatusLine.cx*m_uImageIndex/100L,SizeStatusLine.cy,0,SizeStatusLine.cy);

	////�滭����
	INT nXPoint=SizeStatusLine.cx*m_uImageIndex/100L;
	ImageStatusPoint.DrawImage(pDC,16+nXPoint-SizeStatusPoint.cx/2,17+(SizeStatusLine.cy-SizeStatusPoint.cy)/2);

	//����λ��
	CRect rcDescribe;
	rcDescribe.SetRect(14,42,353,55);

	//�滭����
	pDC->SetTextColor(RGB(241,204,129));
	pDC->DrawText(m_strDescribe,rcDescribe,DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE);

	return;
}

//�رմ���
VOID CDlgStatus::HideStatusWindow()
{
	//�رմ���
	if (m_hWnd!=NULL) DestroyWindow();

	return;
}

//������Ϣ
VOID CDlgStatus::ShowStatusWindow(LPCTSTR pszDescribe)
{
	//��������
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

	//��������
	m_strDescribe=pszDescribe;

	//��ʾ����
	ShowWindow(SW_SHOW);

	//���´���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

	return;
}

//���ýӿ�
VOID CDlgStatus::SetStatusViewSink(IStatusViewSink * pIStatusViewSink)
{
	//���ñ���
	m_pIStatusViewSink=pIStatusViewSink;

	return;
}

//ʱ����Ϣ
VOID CDlgStatus::OnTimer(UINT nIDEvent)
{
	//���ý���
	if (nIDEvent==IDT_STATUS)
	{
		//��������
		m_uImageIndex=(m_uImageIndex>=100)?0:(m_uImageIndex+1L);

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//ȡ����ť
VOID CDlgStatus::OnBnClickedGiveUp()
{
	//���ؽ���
	HideStatusWindow();

	//Ͷ����Ϣ
	ASSERT(m_pIStatusViewSink!=NULL);
	if (m_pIStatusViewSink!=NULL) m_pIStatusViewSink->OnStatusCancel();

	return;
}
//////////////////////////////////////////////////////////////////////////////////
