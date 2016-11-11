#include "Stdafx.h"
#include "PlazaViewNews.h"
#include "PlatformFrame.h"

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
	if (m_bHoverServer==true) 
	{
		SetTextColor(RGB(0,0,255));
	}
	else
	{
		SetTextColor(RGB(0,0,0));
	}
}
	
//��ʼ����
void CWebLinkUI::InitControlUI()
{
	//��ԭ����
	SetTextColor(RGB(0,0,0));
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

//�����ؼ�
CControlUI* CPlazaViewNews::CreateControl(LPCTSTR pstrClass)
{  
   if (_tcsicmp(pstrClass, kWebLinkInterface) == 0)  
		return  new CWebLinkUI();  
   return NULL;
}

//��ʼ����
void CPlazaViewNews::InitControlUI()
{
	CWebLinkUI * pWebLink[8] = {NULL};
	pWebLink[0]=  (CWebLinkUI *) GetControlByName( szButtonNewsLink1ControlName );
	pWebLink[1] = (CWebLinkUI *) GetControlByName( szButtonNewsLink2ControlName );
	pWebLink[2] = (CWebLinkUI *) GetControlByName( szButtonNewsLink3ControlName );
	pWebLink[3] = (CWebLinkUI *) GetControlByName( szButtonNewsLink4ControlName );
	pWebLink[4] = (CWebLinkUI *) GetControlByName( szButtonNewsLink5ControlName );	
	pWebLink[5] = (CWebLinkUI *) GetControlByName( szButtonNewsLink6ControlName );
	pWebLink[6] = (CWebLinkUI *) GetControlByName( szButtonNewsLink7ControlName );
	pWebLink[7] = (CWebLinkUI *) GetControlByName( szButtonNewsLink8ControlName );

	// �ж��ı�
	for (int i=0; i<8; i++) 
	{
		CStdString szText = TEXT("");
		if(pWebLink[i]!=NULL)
			szText = pWebLink[i]->GetText();

		if(szText.IsEmpty()==true)
		{
			pWebLink[i]->SetVisible(false);
		}
	}
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
		TCHAR szNavigation[256]=TEXT("");
		_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sWebLinke%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(),pControlUI->GetTag());
		
		//��ҳ��
		ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
	}
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
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("PF_NEWS_CONTEXT_BK"));

	//���ô�С
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL, 0, 0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	return 0;
}