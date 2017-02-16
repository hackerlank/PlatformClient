#include "Stdafx.h"
#include "PlazaViewNews.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "HttpClient.h"


//////////////////////////////////////////////////////////////////////////////////

const TCHAR* const szButtonNewsLink1ControlName	= TEXT("ButtonNewsLink1");
const TCHAR* const szButtonNewsLink2ControlName	= TEXT("ButtonNewsLink2");
const TCHAR* const szButtonNewsLink3ControlName	= TEXT("ButtonNewsLink3");
const TCHAR* const szButtonNewsLink4ControlName	= TEXT("ButtonNewsLink4");
const TCHAR* const szButtonNewsLink5ControlName	= TEXT("ButtonNewsLink5");
const TCHAR* const szButtonNewsLink6ControlName	= TEXT("ButtonNewsLink6");
const TCHAR* const szButtonNewsLink7ControlName	= TEXT("ButtonNewsLink7");
const TCHAR* const szButtonNewsLink8ControlName	= TEXT("ButtonNewsLink8");

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewNews, CFGuiWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()

	// �Զ���Ϣ
	// ON_MESSAGE(WM_VIEW_NEWS, OnHttpNavigate)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWebLinkUI::CWebLinkUI()  
{  
	m_bHoverServer = false;
}

//��������
CWebLinkUI::~CWebLinkUI()  
{

}

//�������
LPCTSTR CWebLinkUI::GetClass() const  
{  
    return kWebLinkClassName;  
}  

//�ӿں���
LPVOID CWebLinkUI::GetInterface(LPCTSTR pstrName)  
{  
    if( _tcscmp(pstrName, kWebLinkInterface) == 0 ) {
		return static_cast<CWebLinkUI*>(this);
	}
    return CButtonUI::GetInterface(pstrName);  
}

//�ع�����
void CWebLinkUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CButtonUI::SetAttribute(pstrName, pstrValue);  
}

//���»���
void CWebLinkUI::PaintStatusImage(HDC hDC)
{
	//���غ���
	CButtonUI::PaintStatusImage(hDC);

	//��ԭ����
	if (m_bHoverServer==true) {
		SetTextColor(RGB(255,144,255));
	}
	else {
		SetTextColor(RGB(122,144,122));
	}
}
	
//��ʼ����
void CWebLinkUI::InitControlUI()
{
	//��ԭ����
	SetTextColor(RGB(255,255,255));
}

//�¼�����
void CWebLinkUI::DoEvent(TEventUI& event)
{
	//������
	if( event.Type == UIEVENT_MOUSEENTER ) 
    {
		m_bHoverServer = true;
		Invalidate();
        return;
    }

	//����뿪
    if( event.Type == UIEVENT_MOUSELEAVE ) 
    {
		m_bHoverServer = false;
		Invalidate();
        return;
    }

	//����¼�
	if(event.Type == UIEVENT_BUTTONDOWN )
	{
		//������ͼ
		m_bHoverServer = false;
		Invalidate();
		
		//���ָ��
		CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
		if (pPlatformFrame==NULL) return;

		//��ȡ����
		ASSERT(CGlobalWebLink::GetInstance()!=NULL);
		CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

		//�����ַ
		TCHAR szNavigation[256]=TEXT("");
		_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sWebLinke%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(), GetTag() );
		
		//��ҳ��
		ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);

		return;
	}

	if( m_pParent != NULL ) {
		m_pParent->DoEvent(event);
	}
}


//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlazaViewNews::CPlazaViewNews()
{

}

//��������
CPlazaViewNews::~CPlazaViewNews()
{

}

//Http����
void CPlazaViewNews::OnHttpNavigate(tagNoticesParameter * Notices, WORD wSize)
{
	// ����Ч��
	if(Notices==NULL || wSize==0) return;

	// ��������
	CWebLinkUI * pWebLink[8] = {NULL};
	pWebLink[0]=  (CWebLinkUI *) GetControlByName( szButtonNewsLink1ControlName );
	pWebLink[1] = (CWebLinkUI *) GetControlByName( szButtonNewsLink2ControlName );
	pWebLink[2] = (CWebLinkUI *) GetControlByName( szButtonNewsLink3ControlName );
	pWebLink[3] = (CWebLinkUI *) GetControlByName( szButtonNewsLink4ControlName );
	pWebLink[4] = (CWebLinkUI *) GetControlByName( szButtonNewsLink5ControlName );	
	pWebLink[5] = (CWebLinkUI *) GetControlByName( szButtonNewsLink6ControlName );
	pWebLink[6] = (CWebLinkUI *) GetControlByName( szButtonNewsLink7ControlName );
	pWebLink[7] = (CWebLinkUI *) GetControlByName( szButtonNewsLink8ControlName );

	// ��С����
	WORD wMinSize =  __min( wSize, 8 );

	// �ж��ı�
	for (int i=0; i<8; i++)  {
		if(pWebLink[i]!=NULL)
			pWebLink[i]->SetVisible(false);
	}
	
	// ��������
	tagTopNews szNews;
	m_cTopNewsArray.RemoveAll();

	// �ַ��ı�
	for (int i=0; i< wMinSize; i++)  {
		if(pWebLink[i]!=NULL)
		{
			szNews.szcTitle = Notices[i].szTitle;
			szNews.szcWeblink = Notices[i].szLinkURL;
			m_cTopNewsArray.Add(szNews);
			pWebLink[i]->SetText( szNews.szcTitle );
			pWebLink[i]->SetVisible(true);
		}
	}
}

//�����ؼ�
CControlUI* CPlazaViewNews::CreateControl(LPCTSTR pstrClass)
{  
   if (_tcsicmp(pstrClass, kWebLinkInterface) == 0)  
		return  new CWebLinkUI();  
   return NULL;
}

//����WebLink
CWebLinkUI * CPlazaViewNews::CreateWeblike( CPaintManagerUI * _PaintManager, LPCTSTR szName, int tagIndex, int x, int y, int cx, int cy, CControlUI * pParent)
{
	CWebLinkUI * pWebLink = (CWebLinkUI *) CWebLinkUI::Create( _PaintManager, pParent, szName );
	if( pWebLink!=NULL )  {
		pWebLink->SetTag(tagIndex);
		pWebLink->SetFont(1);
		pWebLink->SetFloat(true);
		pWebLink->SetPos(x,y);
		pWebLink->SetFixedWidth(cx);
		pWebLink->SetFixedHeight(cy);
		pWebLink->SetAttribute( TEXT("align"), TEXT("left") );
		pWebLink->SetTextColor(RGB(80,80,10));
	}
	return pWebLink;
}

//��ʼ����
void CPlazaViewNews::InitControlUI()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	CControlUI * pParent = static_cast<CControlUI *>( m_PaintManager.GetRoot() );
	if(pParent==NULL) return;

	// ��������
	CWebLinkUI * pWebLink[8] = {NULL};
	pWebLink[0] = (CWebLinkUI *) CreateWeblike( &m_PaintManager, TEXT("ButtonNewsLink1"), 1, 8, 5,   200, 25, pParent );
	pWebLink[1] = (CWebLinkUI *) CreateWeblike( &m_PaintManager, TEXT("ButtonNewsLink2"), 2, 8, 25,  200, 25, pParent );
	pWebLink[2] = (CWebLinkUI *) CreateWeblike( &m_PaintManager, TEXT("ButtonNewsLink3"), 3, 8, 45,  200, 25, pParent );
	pWebLink[3] = (CWebLinkUI *) CreateWeblike( &m_PaintManager, TEXT("ButtonNewsLink4"), 4, 8, 65,  200, 25, pParent );
	pWebLink[4] = (CWebLinkUI *) CreateWeblike( &m_PaintManager, TEXT("ButtonNewsLink5"), 5, 8, 85,  200, 25, pParent );	
	pWebLink[5] = (CWebLinkUI *) CreateWeblike( &m_PaintManager, TEXT("ButtonNewsLink6"), 6, 8, 105, 200, 25, pParent );
	pWebLink[6] = (CWebLinkUI *) CreateWeblike( &m_PaintManager, TEXT("ButtonNewsLink7"), 7, 8, 125, 200, 25, pParent );
	pWebLink[7] = (CWebLinkUI *) CreateWeblike( &m_PaintManager, TEXT("ButtonNewsLink8"), 8, 8, 145, 200, 25, pParent );
}

//��Ϣ����
void CPlazaViewNews::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	if (pPlatformFrame==NULL) return;
	
	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//��ȡ����
		ASSERT(CGlobalWebLink::GetInstance()!=NULL);
		CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

		//�����ַ
		/* TCHAR szNavigation[256]=TEXT("");
		_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sWebLinke%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(),pControlUI->GetTag()); */

		CString szwText;
		WORD wIndex = pControlUI->GetTag();
		if (wIndex <= m_cTopNewsArray.GetCount() ) {
			szwText =m_cTopNewsArray.GetAt(wIndex-1).szcWeblink;
		}

		//��ҳ��
		ShellExecute(NULL,TEXT("OPEN"),szwText, NULL, NULL, SW_NORMAL);
	}
}

//�����滭
void CPlazaViewNews::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	m_ImageBack.DrawImage(pDC,0,0);

	//��������
	m_PaintManager.AddFontAt(0,TEXT("����"), 14, false, false, false);
	m_PaintManager.AddFontAt(1,TEXT("����"), 16, false, false, false);
}

//ʱ����Ϣ
VOID CPlazaViewNews::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
	return;
}

//λ����Ϣ
VOID CPlazaViewNews::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);
	return;
}

//������Ϣ
INT CPlazaViewNews::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	//������Դ
	//CPngImage m_ImageBack;
	m_ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("PF_NEWS_CONTEXT_BK"));

	//���ô�С
	CSize SizeWindow(m_ImageBack.GetWidth(),m_ImageBack.GetHeight());
	SetWindowPos(NULL, 0, 0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	return 0;
}