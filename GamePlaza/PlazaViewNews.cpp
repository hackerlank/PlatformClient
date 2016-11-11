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

	//系统消息
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWebLinkUI::CWebLinkUI()  
{  
	m_bHoverServer = false;
}

//析构函数
CWebLinkUI::~CWebLinkUI()  
{

}

//获得类名
LPCTSTR CWebLinkUI::GetClass() const  
{  
    return kWebLinkClassName;  
}  

//接口函数
LPVOID CWebLinkUI::GetInterface(LPCTSTR pstrName)  
{  
    if( _tcscmp(pstrName, kWebLinkInterface) == 0 ) {
		return static_cast<CWebLinkUI*>(this);
	}
    return CButtonUI::GetInterface(pstrName);  
}

//重构属性
void CWebLinkUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CButtonUI::SetAttribute(pstrName, pstrValue);  
}

//重新绘制
void CWebLinkUI::PaintStatusImage(HDC hDC)
{
	//重载函数
	CButtonUI::PaintStatusImage(hDC);

	//还原参数
	if (m_bHoverServer==true) 
	{
		SetTextColor(RGB(0,0,255));
	}
	else
	{
		SetTextColor(RGB(0,0,0));
	}
}
	
//初始函数
void CWebLinkUI::InitControlUI()
{
	//还原参数
	SetTextColor(RGB(0,0,0));
}

//事件处理
void CWebLinkUI::DoEvent(TEventUI& event)
{
	//鼠标进入
	if( event.Type == UIEVENT_MOUSEENTER ) 
    {
		m_bHoverServer = true;
		Invalidate();
        return;
    }

	//鼠标离开
    if( event.Type == UIEVENT_MOUSELEAVE ) 
    {
		m_bHoverServer = false;
		Invalidate();
        return;
    }

	//点击事件
	if(event.Type == UIEVENT_BUTTONDOWN )
	{
		//更新视图
		m_bHoverServer = false;
		Invalidate();
		
		//框架指针
		CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
		if (pPlatformFrame==NULL) return;

		//获取对象
		ASSERT(CGlobalWebLink::GetInstance()!=NULL);
		CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

		//构造地址
		TCHAR szNavigation[256]=TEXT("");
		_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sWebLinke%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(), GetTag() );
		
		//打开页面
		ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);

		return;
	}

	if( m_pParent != NULL ) {
		m_pParent->DoEvent(event);
	}
}


//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaViewNews::CPlazaViewNews()
{

}

//析构函数
CPlazaViewNews::~CPlazaViewNews()
{

}

//创建控件
CControlUI* CPlazaViewNews::CreateControl(LPCTSTR pstrClass)
{  
   if (_tcsicmp(pstrClass, kWebLinkInterface) == 0)  
		return  new CWebLinkUI();  
   return NULL;
}

//初始函数
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

	// 判断文本
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

//消息提醒
void CPlazaViewNews::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	CPlatformFrame * pPlatformFrame = CPlatformFrame::GetInstance();
	if (pPlatformFrame==NULL) return;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//获取对象
		ASSERT(CGlobalWebLink::GetInstance()!=NULL);
		CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

		//构造地址
		TCHAR szNavigation[256]=TEXT("");
		_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sWebLinke%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(),pControlUI->GetTag());
		
		//打开页面
		ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
	}
}


//时钟消息
VOID CPlazaViewNews::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
	return;
}

//位置消息
VOID CPlazaViewNews::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);
	return;
}

//创建消息
INT CPlazaViewNews::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("PF_NEWS_CONTEXT_BK"));

	//设置大小
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL, 0, 0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	return 0;
}