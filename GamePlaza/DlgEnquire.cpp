#include "Stdafx.h"
#include "DlgEnquire.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//Բ�Ǵ�С
#define ROUND_CX					9									//Բ�ǿ��
#define ROUND_CY					10									//Բ�Ǹ߶�

//��Ļλ��
#define LAYERED_SIZE				6									//�ֲ��С
#define CAPTION_SIZE				38									//�����С

//�ؼ���ʶ
#define IDC_WEB_PUBLICIZE			300									//����ؼ�

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgEnquire::CDlgEnquire() : CFGuiDialog(IDD_DLG_POPUP)
{
	//���ñ���
	m_pbtCancel=NULL;
	m_pbtClosePlaza=NULL;
	m_pbtCloseServer=NULL;
	m_pbtSwitchAccounts=NULL;
}

//��������
CDlgEnquire::~CDlgEnquire()
{
}

//���ú���
BOOL CDlgEnquire::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��ܰ��ʾ"));

	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_ENQUIRE_BACK"));

	//���ô�С
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//���ؼ�
	//CRect rcCreate(0,0,0,0);
	//m_PlatformPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	//��ȡ����
	//CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	//TCHAR szBillUrl[256]=TEXT("");
	//_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/ClosePlaza.aspx"),pGlobalWebLink->GetPlatformLink());//

	//���ù��
	//m_PlatformPublicize.Navigate(szBillUrl);
	//m_PlatformPublicize.SetWindowPos(NULL,3,43,SizeWindow.cx-6,95,SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOACTIVATE);

	//���д���
	CenterWindow(this);

	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	CRect rcUnLayered;
	rcUnLayered.top=LAYERED_SIZE;
	rcUnLayered.left=LAYERED_SIZE;
	rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//��������
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,ROUND_CX,ROUND_CY);

	//��������
	SetWindowRgn(RgnWindow,FALSE);

	//�ֲ㴰��
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageBack,0,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	//�����ؼ�
	RectifyControlUI();

	return FALSE;
}

//ȷ������
VOID CDlgEnquire::OnOK()
{
	return;
}

//��ʼ�ؼ�
void CDlgEnquire::InitControlUI()
{
	//���ñ���
	m_PaintManager.SetCaptionRect(CRect(0,0,0,CAPTION_SIZE));

	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//�رհ�ť
	m_pbtCancel = CButtonUI::Create(&m_PaintManager, pParent, TEXT(""));
	m_pbtCancel->SetStatusImage(TEXT("file='BT_ENQUIRE_CANCEL' restype='PNG'"));

	//�رմ���
	m_pbtClosePlaza = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtClosePlaza->SetStatusImage(TEXT("file='BT_CLOSE_PLAZA' restype='PNG'"));

	//�رշ���
	m_pbtCloseServer = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtCloseServer->SetStatusImage(TEXT("file='BT_CLOSE_SERVER' restype='PNG'"));

	//�л��ʺ�
	m_pbtSwitchAccounts = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtSwitchAccounts->SetStatusImage(TEXT("file='BT_SWITCH_ACCOUNTS' restype='PNG'"));	


	//��ȡ����
	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	m_pbtCloseServer->SetEnabled( pPlatformFrame->IsEnteredServerItem() );

	//�����ؼ�
	RectifyControlUI();
}

//��Ϣ����
void CDlgEnquire::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(pControlUI==m_pbtCancel) return OnCancel();
		if(pControlUI==m_pbtClosePlaza) return OnBnClickedClosePlaza();
		if(pControlUI==m_pbtCloseServer) return OnBnClickedCloseServer();
		if(pControlUI==m_pbtSwitchAccounts) return OnBnClickedSwitchAccounts();
	}

}

//�����滭
void CDlgEnquire::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_ENQUIRE_BACK"));

	//������Դ
	CPngImage ImageTitle;
	ImageTitle.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_ENQUIRE_TAG"));

	//�滭����
	ImageBack.DrawImage(pDC,0,0);
	ImageTitle.DrawImage(pDC,30,70);
}

//�رմ���
VOID CDlgEnquire::OnBnClickedClosePlaza()
{
	EndDialog(IDC_CLOSE_PLAZA);

	return;
}

//�رշ���
VOID CDlgEnquire::OnBnClickedCloseServer()
{
	EndDialog(IDC_CLOSE_SERVER);

	return;
}

//�л��ʺ�
VOID CDlgEnquire::OnBnClickedSwitchAccounts()
{
	EndDialog(IDC_SWITCH_ACCOUNTS);

	return;
}

//�����ؼ�
VOID CDlgEnquire::RectifyControlUI()
{
	//��ȡ����
	CRect rcClientRect;
	GetClientRect(&rcClientRect);

	//��������
	CPoint ptControl;
	CSize SizeControl;

	//ȡ����ť
	ptControl.SetPoint(rcClientRect.right-36,8);
	SizeControl.SetSize(m_pbtCancel->GetFixedWidth(),m_pbtCancel->GetFixedHeight());
	m_pbtCancel->SetPos(CRect(ptControl.x,ptControl.y,ptControl.x+SizeControl.cx,SizeControl.cy+ptControl.y));

	//��������
	INT nButtonWidth = m_pbtClosePlaza->GetFixedWidth();
	INT nButtonSpace=(rcClientRect.Width()-nButtonWidth*3)/4;

	//���óߴ�
	SizeControl.SetSize(m_pbtSwitchAccounts->GetFixedWidth(),m_pbtSwitchAccounts->GetFixedHeight());

	//�л��ʺ�
	ptControl.SetPoint(nButtonSpace,154);	
	m_pbtSwitchAccounts->SetPos(CRect(ptControl.x,ptControl.y,ptControl.x+SizeControl.cx,SizeControl.cy+ptControl.y));

	//�رշ���
	ptControl.SetPoint(nButtonSpace*2+nButtonWidth,154);
	m_pbtCloseServer->SetPos(CRect(ptControl.x,ptControl.y,ptControl.x+SizeControl.cx,SizeControl.cy+ptControl.y));

	//�رմ���
	ptControl.SetPoint(nButtonSpace*3+nButtonWidth*2,154);
	m_pbtClosePlaza->SetPos(CRect(ptControl.x,ptControl.y,ptControl.x+SizeControl.cx,SizeControl.cy+ptControl.y));
}

//////////////////////////////////////////////////////////////////////////////////
