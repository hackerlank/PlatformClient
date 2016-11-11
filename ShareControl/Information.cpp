#include "StdAfx.h"
#include "Resource.h"
#include "Information.h"

//////////////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_MESSAGE					100									//��Ϣ��ʶ

//����λ��
#define SMB_WINDOW_WIDTH			230									//��С���

//ͼ��ƫ��
#define SMB_ICON_LEFT				20									//ͼ��ƫ��
#define SMB_ICON_WIDTH				32									//ͼ����
#define SMB_ICON_HEIGHT				32									//ͼ��߶�

//�ַ�ƫ��
#define SMB_STRING_TOP				25									//�ַ�ƫ��
#define SMB_STRING_LEFT				20									//�ַ�ƫ��
#define SMB_STRING_RIGHT			40									//�ַ�ƫ��
#define SMB_STRING_WIDTH			600									//�����
#define SMB_STRING_HEIGHT			600									//�����

//��ťƫ��
#define SMB_BUTTON_TOP				25									//��ťƫ��
#define SMB_BUTTON_LEFT				12									//��ťƫ��
#define SMB_BUTTON_RIGHT			12									//��ťƫ��
#define SMB_BUTTON_BOTTOM			12									//��ťƫ��
#define SMB_BUTTON_SPACE			20									//��ť���
#define SMB_BUTTON_WIDTH			65									//��ťƫ��
#define SMB_BUTTON_HEIGHT			24									//��ťƫ��

//����ƫ��
#define ROUND_XSPACE				2									//����ƫ��
#define ROUND_TSPACE				1									//����ƫ��
#define ROUND_BSPACE				3									//����ƫ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CInformation, CSkinDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CInformation::CInformation(CWnd * pParentWnd) : CSkinDialog(IDD_MESSAGE_BOX,pParentWnd)
{
	//���ñ���
	m_uType=MB_OK;

	//��������
	m_nElapse=0;
	m_nBtCount=0;
	m_hResInstance=NULL;

	//�ؼ�����
	m_pLabelRound=NULL;
	ZeroMemory(m_uButtonID,sizeof(m_uButtonID));
	ZeroMemory(m_pbtButton,sizeof(m_pbtButton));

	return;
}

//��������
CInformation::~CInformation()
{
}

//��ʼ����Ϣ
BOOL CInformation::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(m_strCaption);

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//��ȡ��Ϣ
	INT nTBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;

	//��ť����
	CreateBoxButton();
	INT nButtonAreaHeight=SMB_BUTTON_HEIGHT+SMB_BUTTON_TOP+SMB_BUTTON_BOTTOM;
	INT nButtonAreaWidth=m_nBtCount*SMB_BUTTON_WIDTH+(m_nBtCount+1)*SMB_BUTTON_SPACE+SMB_BUTTON_LEFT+SMB_BUTTON_RIGHT;

	//�ַ��ռ�
	CClientDC ClientDC(this);
	CRect rcString(0,0,SMB_STRING_WIDTH,SMB_STRING_HEIGHT);
	ClientDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	ClientDC.DrawText(m_strString,rcString,DT_CALCRECT|DT_EXPANDTABS|DT_NOCLIP|DT_WORD_ELLIPSIS);

	//�ַ�����
	INT nStringAreaHeight=rcString.Height()+SMB_STRING_TOP;
	INT nStringAreaWidth=rcString.Width()+SMB_ICON_LEFT+SMB_ICON_WIDTH+SMB_STRING_LEFT+SMB_STRING_RIGHT;

	//��������
	INT nWindowAreaHeight=nStringAreaHeight+nButtonAreaHeight;
	INT nWindowAreaWidth=__max(SMB_WINDOW_WIDTH,__max(nStringAreaWidth,nButtonAreaWidth));

	//���λ��
	if (nWindowAreaWidth>nStringAreaWidth)
	{
		m_rcString.top=SMB_STRING_TOP+nTBorder+ROUND_TSPACE;
		m_rcString.left=SMB_ICON_LEFT+SMB_ICON_WIDTH+SMB_STRING_LEFT+(nWindowAreaWidth-nStringAreaWidth)/2+ROUND_XSPACE;
		m_rcString.right=m_rcString.left+rcString.Width();
		m_rcString.bottom=m_rcString.top+rcString.Height();
	}
	else
	{
		m_rcString.top=SMB_STRING_TOP+nTBorder+ROUND_TSPACE;
		m_rcString.left=SMB_ICON_LEFT+SMB_ICON_WIDTH+SMB_STRING_LEFT+ROUND_XSPACE;
		m_rcString.right=m_rcString.left+rcString.Width();
		m_rcString.bottom=m_rcString.top+rcString.Height();
	}

	//��������
	INT nWindowWidth=nWindowAreaWidth+nLBorder+nRBorder+ROUND_XSPACE*2;
	INT nWindowHeight=nWindowAreaHeight+nTBorder+nBBorder+ROUND_TSPACE+ROUND_BSPACE;
	SetWindowPos(NULL,0,0,nWindowWidth,nWindowHeight,SWP_NOMOVE|SWP_NOZORDER|SWP_NOCOPYBITS);

	//����λ��
	INT nYButton=nTBorder+nBBorder+nWindowAreaHeight-SMB_BUTTON_BOTTOM-SMB_BUTTON_HEIGHT+ROUND_TSPACE;
	INT nXButton=(nWindowAreaWidth-m_nBtCount*SMB_BUTTON_WIDTH-(m_nBtCount-1)*SMB_BUTTON_SPACE)/2+ROUND_XSPACE;

	//������ť
	for (UINT i=0;i<m_nBtCount;i++)
	{
		m_pbtButton[i]->SetVisible(true);
		m_pbtButton[i]->SetFixedXY(CPoint(nXButton,nYButton));
		nXButton+=SMB_BUTTON_SPACE+SMB_BUTTON_WIDTH;
	}

	//Ĭ�ϰ�ť
	UINT uDefaultIndex=(m_uType&MB_DEFMASK)>>8;
	if (uDefaultIndex>=m_nBtCount) uDefaultIndex=0;
	m_pbtButton[uDefaultIndex]->SetFocus();
	m_pbtButton[uDefaultIndex]->SetWantReturn();

	//���д���
	SetActiveWindow();
	SetForegroundWindow();
	CenterWindow(GetParent());

	//����ʱ��
	if (m_nElapse!=0) SetTimer(IDI_MESSAGE,1000,NULL);

	return FALSE;
}

//��ʼ�ؼ�
void CInformation::InitControlUI()
{
	__super::InitControlUI();

	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//��С��ť
	m_pbtButton[0] = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtButton[0]->SetStatusImage(TEXT("file='BT_SKIN_ITEM' restype='PNG'"));
	//m_pbtButton[0]->SetTextColor(ARGB(255,255,255,255));
	m_pbtButton[0]->SetVisible(false);

	//���ť
	m_pbtButton[1] = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtButton[1]->SetStatusImage(TEXT("file='BT_SKIN_ITEM' restype='PNG'"));
	//m_pbtButton[1]->SetTextColor(ARGB(255,255,255,255));
	m_pbtButton[1]->SetVisible(false);

	//�رհ�ť
	m_pbtButton[2] = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtButton[2]->SetStatusImage(TEXT("file='BT_SKIN_ITEM' restype='PNG'"));
	//m_pbtButton[2]->SetTextColor(ARGB(255,255,255,255));
	m_pbtButton[2]->SetVisible(false);
}

//��Ϣ����
void CInformation::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(pControlUI==m_pbtButton[0] || pControlUI==m_pbtButton[1] || pControlUI==m_pbtButton[2])
		{
			//��������
			UINT uButtonID = (UINT)pControlUI->GetTag();

			//�����ж�
			for (UINT i=0;i<m_nBtCount;i++)
			{
				//��ʶ����
				if (m_uButtonID[i]==uButtonID)
				{
					EndDialog(uButtonID);
					return;
				}
			}

			//Ĭ�ϴ���
			if (uButtonID==IDOK)
			{
				UINT uDefaultIndex=(m_uType&MB_DEFMASK)>>8;
				if (uDefaultIndex>=m_nBtCount) uDefaultIndex=0;
				EndDialog(m_uButtonID[uDefaultIndex]);
				return;
			}

			//ȡ������
			if (uButtonID==IDCANCEL)
			{
				EndDialog(m_uButtonID[m_nBtCount-1]);
				return;
			}

			//Ĭ�ϴ���
			EndDialog(uButtonID);

			return;
		}
	}	
}

//��ʼ�滭
VOID CInformation::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//����ͼ��
	HICON hIcon=NULL;
	UINT uIconType=(m_uType&MB_ICONMASK);
	if (uIconType==MB_ICONHAND) hIcon=LoadIcon(NULL,IDI_HAND);
	else if (uIconType==MB_ICONQUESTION) hIcon=LoadIcon(NULL,IDI_QUESTION);
	else if (uIconType==MB_ICONEXCLAMATION) hIcon=LoadIcon(NULL,IDI_EXCLAMATION);
	else hIcon=LoadIcon(NULL,IDI_ASTERISK);

	//�滭ͼ��
	INT nYPos=(m_rcString.bottom+m_rcString.top)/2-SMB_ICON_HEIGHT/2;
	DrawIconEx(pDC->m_hDC,SMB_ICON_LEFT+ROUND_XSPACE,nYPos,hIcon,SMB_ICON_WIDTH,SMB_ICON_HEIGHT,0,NULL,DI_NORMAL);

	//�滭����
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(10,10,10));
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	pDC->DrawText(m_strString,&m_rcString,DT_EXPANDTABS|DT_NOCLIP|DT_WORD_ELLIPSIS);

	return;
}

//��ʾ��Ϣ
INT CInformation::ShowMessageBox(LPCTSTR pszString, UINT uType, UINT nElapse)
{
	return ShowMessageBox(TEXT("ϵͳ��ʾ"),pszString,uType,nElapse);
}

//��ʾ��Ϣ
INT CInformation::ShowMessageBox(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType, UINT nElapse)
{
	//���ñ���
	m_uType=uType;
	m_nElapse=nElapse;
	m_strString=pszString;
	m_strCaption=pszCaption;

	//������Դ
	m_hResInstance=AfxGetResourceHandle();
	AfxSetResourceHandle(GetModuleHandle(SHARE_CONTROL_DLL_NAME));

	//��ʾ����
	INT nResult=(INT)DoModal();

	return nResult;
}

//���ť
VOID CInformation::CreateBoxButton()
{
	//��������
	LPCTSTR pszString[3]={NULL,NULL,NULL};

	//���ð�ť
	switch (m_uType&MB_TYPEMASK)
	{
	case MB_OKCANCEL:
		{
			m_nBtCount=2;
			m_uButtonID[0]=IDOK;
			m_uButtonID[1]=IDCANCEL;
			pszString[0]=TEXT("ȷ��(O)");
			pszString[1]=TEXT("ȡ��(C)");
			break;
		}
	case MB_YESNO:
		{
			m_nBtCount=2;
			m_uButtonID[0]=IDYES;
			m_uButtonID[1]=IDNO;
			pszString[0]=TEXT("��(Y)");
			pszString[1]=TEXT("��(N)");
			break;
		}
	case MB_YESNOCANCEL:
		{
			m_nBtCount=3;
			m_uButtonID[0]=IDYES;
			m_uButtonID[1]=IDNO;
			m_uButtonID[2]=IDCANCEL;
			pszString[0]=TEXT("��(Y)");
			pszString[1]=TEXT("��(N)");
			pszString[2]=TEXT("ȡ��(C)");
			break;
		}
	case MB_RETRYCANCEL:
		{
			m_nBtCount=2;
			m_uButtonID[0]=IDRETRY;
			m_uButtonID[1]=IDCANCEL;
			pszString[0]=TEXT("����(R)");
			pszString[1]=TEXT("ȡ��(C)");
			break;
		}
	case MB_ABORTRETRYIGNORE:
		{
			m_nBtCount=3;
			m_uButtonID[0]=IDABORT;
			m_uButtonID[1]=IDRETRY;
			m_uButtonID[2]=IDIGNORE;
			pszString[0]=TEXT("��ֹ(A)");
			pszString[1]=TEXT("����(R)");
			pszString[2]=TEXT("����(I)");
			break;
		}
	default:
		{
			m_nBtCount=1;
			m_uButtonID[0]=IDOK;
			pszString[0]=TEXT("ȷ��(O)");
			break;
		}
	}

	//���ð�ť
	for (UINT i=0;i<m_nBtCount;i++) 
	{
		m_pbtButton[i]->SetTag(m_uButtonID[i]);
		m_pbtButton[i]->SetText(pszString[i]);
	}

	return;
}

//ʱ����Ϣ
VOID CInformation::OnTimer(UINT_PTR nIDEvent)
{
	//����ʱ��
	if (nIDEvent==IDI_MESSAGE)
	{
		//ɾ���ж�
		if (m_nElapse==0)
		{
			PostMessage(WM_CLOSE,0,0);
			return;
		}

		//���ý���
		TCHAR szTitle[32]=TEXT("");
		_sntprintf(szTitle,CountArray(szTitle),TEXT("%s ��%ld �룩"),m_strCaption,m_nElapse--);
		SetWindowText(szTitle);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////

//��ʾ��Ϣ
extern "C" SHARE_CONTROL_CLASS INT ShowInformation(LPCTSTR pszString, UINT uType, UINT nElapse)
{
	CInformation Information;
	return Information.ShowMessageBox(pszString,uType,nElapse);
}

//��ʾ��Ϣ
extern "C" SHARE_CONTROL_CLASS INT ShowInformationEx(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType, UINT nElapse)
{
	CInformation Information;
	return Information.ShowMessageBox(pszCaption,pszString,uType,nElapse);
}

//////////////////////////////////////////////////////////////////////////////////
