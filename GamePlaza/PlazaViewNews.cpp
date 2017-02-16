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

	//系统消息
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()

	// 自定消息
	// ON_MESSAGE(WM_VIEW_NEWS, OnHttpNavigate)

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
	if (m_bHoverServer==true) {
		SetTextColor(RGB(255,144,255));
	}
	else {
		SetTextColor(RGB(122,144,122));
	}
}
	
//初始函数
void CWebLinkUI::InitControlUI()
{
	//还原参数
	SetTextColor(RGB(255,255,255));
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

//Http引导
void CPlazaViewNews::OnHttpNavigate(tagNoticesParameter * Notices, WORD wSize)
{
	// 参数效验
	if(Notices==NULL || wSize==0) return;

	// 新闻数据
	CWebLinkUI * pWebLink[8] = {NULL};
	pWebLink[0]=  (CWebLinkUI *) GetControlByName( szButtonNewsLink1ControlName );
	pWebLink[1] = (CWebLinkUI *) GetControlByName( szButtonNewsLink2ControlName );
	pWebLink[2] = (CWebLinkUI *) GetControlByName( szButtonNewsLink3ControlName );
	pWebLink[3] = (CWebLinkUI *) GetControlByName( szButtonNewsLink4ControlName );
	pWebLink[4] = (CWebLinkUI *) GetControlByName( szButtonNewsLink5ControlName );	
	pWebLink[5] = (CWebLinkUI *) GetControlByName( szButtonNewsLink6ControlName );
	pWebLink[6] = (CWebLinkUI *) GetControlByName( szButtonNewsLink7ControlName );
	pWebLink[7] = (CWebLinkUI *) GetControlByName( szButtonNewsLink8ControlName );

	// 最小长度
	WORD wMinSize =  __min( wSize, 8 );

	// 判断文本
	for (int i=0; i<8; i++)  {
		if(pWebLink[i]!=NULL)
			pWebLink[i]->SetVisible(false);
	}
	
	// 变量定义
	tagTopNews szNews;
	m_cTopNewsArray.RemoveAll();

	// 字符文本
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

//创建控件
CControlUI* CPlazaViewNews::CreateControl(LPCTSTR pstrClass)
{  
   if (_tcsicmp(pstrClass, kWebLinkInterface) == 0)  
		return  new CWebLinkUI();  
   return NULL;
}

//创建WebLink
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

//初始函数
void CPlazaViewNews::InitControlUI()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	CControlUI * pParent = static_cast<CControlUI *>( m_PaintManager.GetRoot() );
	if(pParent==NULL) return;

	// 新闻数据
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
		/* TCHAR szNavigation[256]=TEXT("");
		_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/%sWebLinke%ld.aspx"),pGlobalWebLink->GetPlatformLink(),pGlobalWebLink->GetValidateLink(),pControlUI->GetTag()); */

		CString szwText;
		WORD wIndex = pControlUI->GetTag();
		if (wIndex <= m_cTopNewsArray.GetCount() ) {
			szwText =m_cTopNewsArray.GetAt(wIndex-1).szcWeblink;
		}

		//打开页面
		ShellExecute(NULL,TEXT("OPEN"),szwText, NULL, NULL, SW_NORMAL);
	}
}

//结束绘画
void CPlazaViewNews::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画背景
	m_ImageBack.DrawImage(pDC,0,0);

	//设置字体
	m_PaintManager.AddFontAt(0,TEXT("宋体"), 14, false, false, false);
	m_PaintManager.AddFontAt(1,TEXT("黑体"), 16, false, false, false);
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
	//CPngImage m_ImageBack;
	m_ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("PF_NEWS_CONTEXT_BK"));

	//设置大小
	CSize SizeWindow(m_ImageBack.GetWidth(),m_ImageBack.GetHeight());
	SetWindowPos(NULL, 0, 0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	return 0;
}