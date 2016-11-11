#include "StdAfx.h"
#include "Resource.h"
#include "Information.h"

//////////////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_MESSAGE					100									//消息标识

//窗口位置
#define SMB_WINDOW_WIDTH			230									//最小宽度

//图标偏移
#define SMB_ICON_LEFT				20									//图标偏移
#define SMB_ICON_WIDTH				32									//图标宽度
#define SMB_ICON_HEIGHT				32									//图标高度

//字符偏移
#define SMB_STRING_TOP				25									//字符偏移
#define SMB_STRING_LEFT				20									//字符偏移
#define SMB_STRING_RIGHT			40									//字符偏移
#define SMB_STRING_WIDTH			600									//最大宽度
#define SMB_STRING_HEIGHT			600									//最大宽度

//按钮偏移
#define SMB_BUTTON_TOP				25									//按钮偏移
#define SMB_BUTTON_LEFT				12									//按钮偏移
#define SMB_BUTTON_RIGHT			12									//按钮偏移
#define SMB_BUTTON_BOTTOM			12									//按钮偏移
#define SMB_BUTTON_SPACE			20									//按钮间隔
#define SMB_BUTTON_WIDTH			65									//按钮偏移
#define SMB_BUTTON_HEIGHT			24									//按钮偏移

//环绕偏移
#define ROUND_XSPACE				2									//环绕偏移
#define ROUND_TSPACE				1									//环绕偏移
#define ROUND_BSPACE				3									//环绕偏移

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CInformation, CSkinDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CInformation::CInformation(CWnd * pParentWnd) : CSkinDialog(IDD_MESSAGE_BOX,pParentWnd)
{
	//设置变量
	m_uType=MB_OK;

	//辅助变量
	m_nElapse=0;
	m_nBtCount=0;
	m_hResInstance=NULL;

	//控件变量
	m_pLabelRound=NULL;
	ZeroMemory(m_uButtonID,sizeof(m_uButtonID));
	ZeroMemory(m_pbtButton,sizeof(m_pbtButton));

	return;
}

//析构函数
CInformation::~CInformation()
{
}

//初始化消息
BOOL CInformation::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(m_strCaption);

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//获取信息
	INT nTBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;

	//按钮区域
	CreateBoxButton();
	INT nButtonAreaHeight=SMB_BUTTON_HEIGHT+SMB_BUTTON_TOP+SMB_BUTTON_BOTTOM;
	INT nButtonAreaWidth=m_nBtCount*SMB_BUTTON_WIDTH+(m_nBtCount+1)*SMB_BUTTON_SPACE+SMB_BUTTON_LEFT+SMB_BUTTON_RIGHT;

	//字符空间
	CClientDC ClientDC(this);
	CRect rcString(0,0,SMB_STRING_WIDTH,SMB_STRING_HEIGHT);
	ClientDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	ClientDC.DrawText(m_strString,rcString,DT_CALCRECT|DT_EXPANDTABS|DT_NOCLIP|DT_WORD_ELLIPSIS);

	//字符区域
	INT nStringAreaHeight=rcString.Height()+SMB_STRING_TOP;
	INT nStringAreaWidth=rcString.Width()+SMB_ICON_LEFT+SMB_ICON_WIDTH+SMB_STRING_LEFT+SMB_STRING_RIGHT;

	//窗口区域
	INT nWindowAreaHeight=nStringAreaHeight+nButtonAreaHeight;
	INT nWindowAreaWidth=__max(SMB_WINDOW_WIDTH,__max(nStringAreaWidth,nButtonAreaWidth));

	//输出位置
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

	//调整窗口
	INT nWindowWidth=nWindowAreaWidth+nLBorder+nRBorder+ROUND_XSPACE*2;
	INT nWindowHeight=nWindowAreaHeight+nTBorder+nBBorder+ROUND_TSPACE+ROUND_BSPACE;
	SetWindowPos(NULL,0,0,nWindowWidth,nWindowHeight,SWP_NOMOVE|SWP_NOZORDER|SWP_NOCOPYBITS);

	//计算位置
	INT nYButton=nTBorder+nBBorder+nWindowAreaHeight-SMB_BUTTON_BOTTOM-SMB_BUTTON_HEIGHT+ROUND_TSPACE;
	INT nXButton=(nWindowAreaWidth-m_nBtCount*SMB_BUTTON_WIDTH-(m_nBtCount-1)*SMB_BUTTON_SPACE)/2+ROUND_XSPACE;

	//调整按钮
	for (UINT i=0;i<m_nBtCount;i++)
	{
		m_pbtButton[i]->SetVisible(true);
		m_pbtButton[i]->SetFixedXY(CPoint(nXButton,nYButton));
		nXButton+=SMB_BUTTON_SPACE+SMB_BUTTON_WIDTH;
	}

	//默认按钮
	UINT uDefaultIndex=(m_uType&MB_DEFMASK)>>8;
	if (uDefaultIndex>=m_nBtCount) uDefaultIndex=0;
	m_pbtButton[uDefaultIndex]->SetFocus();
	m_pbtButton[uDefaultIndex]->SetWantReturn();

	//居中窗口
	SetActiveWindow();
	SetForegroundWindow();
	CenterWindow(GetParent());

	//设置时间
	if (m_nElapse!=0) SetTimer(IDI_MESSAGE,1000,NULL);

	return FALSE;
}

//初始控件
void CInformation::InitControlUI()
{
	__super::InitControlUI();

	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//最小按钮
	m_pbtButton[0] = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtButton[0]->SetStatusImage(TEXT("file='BT_SKIN_ITEM' restype='PNG'"));
	//m_pbtButton[0]->SetTextColor(ARGB(255,255,255,255));
	m_pbtButton[0]->SetVisible(false);

	//最大按钮
	m_pbtButton[1] = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtButton[1]->SetStatusImage(TEXT("file='BT_SKIN_ITEM' restype='PNG'"));
	//m_pbtButton[1]->SetTextColor(ARGB(255,255,255,255));
	m_pbtButton[1]->SetVisible(false);

	//关闭按钮
	m_pbtButton[2] = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtButton[2]->SetStatusImage(TEXT("file='BT_SKIN_ITEM' restype='PNG'"));
	//m_pbtButton[2]->SetTextColor(ARGB(255,255,255,255));
	m_pbtButton[2]->SetVisible(false);
}

//消息提醒
void CInformation::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(pControlUI==m_pbtButton[0] || pControlUI==m_pbtButton[1] || pControlUI==m_pbtButton[2])
		{
			//变量定义
			UINT uButtonID = (UINT)pControlUI->GetTag();

			//存在判断
			for (UINT i=0;i<m_nBtCount;i++)
			{
				//标识处理
				if (m_uButtonID[i]==uButtonID)
				{
					EndDialog(uButtonID);
					return;
				}
			}

			//默认处理
			if (uButtonID==IDOK)
			{
				UINT uDefaultIndex=(m_uType&MB_DEFMASK)>>8;
				if (uDefaultIndex>=m_nBtCount) uDefaultIndex=0;
				EndDialog(m_uButtonID[uDefaultIndex]);
				return;
			}

			//取消处理
			if (uButtonID==IDCANCEL)
			{
				EndDialog(m_uButtonID[m_nBtCount-1]);
				return;
			}

			//默认处理
			EndDialog(uButtonID);

			return;
		}
	}	
}

//开始绘画
VOID CInformation::OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//加载图标
	HICON hIcon=NULL;
	UINT uIconType=(m_uType&MB_ICONMASK);
	if (uIconType==MB_ICONHAND) hIcon=LoadIcon(NULL,IDI_HAND);
	else if (uIconType==MB_ICONQUESTION) hIcon=LoadIcon(NULL,IDI_QUESTION);
	else if (uIconType==MB_ICONEXCLAMATION) hIcon=LoadIcon(NULL,IDI_EXCLAMATION);
	else hIcon=LoadIcon(NULL,IDI_ASTERISK);

	//绘画图标
	INT nYPos=(m_rcString.bottom+m_rcString.top)/2-SMB_ICON_HEIGHT/2;
	DrawIconEx(pDC->m_hDC,SMB_ICON_LEFT+ROUND_XSPACE,nYPos,hIcon,SMB_ICON_WIDTH,SMB_ICON_HEIGHT,0,NULL,DI_NORMAL);

	//绘画字体
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(10,10,10));
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	pDC->DrawText(m_strString,&m_rcString,DT_EXPANDTABS|DT_NOCLIP|DT_WORD_ELLIPSIS);

	return;
}

//显示消息
INT CInformation::ShowMessageBox(LPCTSTR pszString, UINT uType, UINT nElapse)
{
	return ShowMessageBox(TEXT("系统提示"),pszString,uType,nElapse);
}

//显示消息
INT CInformation::ShowMessageBox(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType, UINT nElapse)
{
	//设置变量
	m_uType=uType;
	m_nElapse=nElapse;
	m_strString=pszString;
	m_strCaption=pszCaption;

	//设置资源
	m_hResInstance=AfxGetResourceHandle();
	AfxSetResourceHandle(GetModuleHandle(SHARE_CONTROL_DLL_NAME));

	//显示窗口
	INT nResult=(INT)DoModal();

	return nResult;
}

//激活按钮
VOID CInformation::CreateBoxButton()
{
	//变量定义
	LPCTSTR pszString[3]={NULL,NULL,NULL};

	//设置按钮
	switch (m_uType&MB_TYPEMASK)
	{
	case MB_OKCANCEL:
		{
			m_nBtCount=2;
			m_uButtonID[0]=IDOK;
			m_uButtonID[1]=IDCANCEL;
			pszString[0]=TEXT("确定(O)");
			pszString[1]=TEXT("取消(C)");
			break;
		}
	case MB_YESNO:
		{
			m_nBtCount=2;
			m_uButtonID[0]=IDYES;
			m_uButtonID[1]=IDNO;
			pszString[0]=TEXT("是(Y)");
			pszString[1]=TEXT("否(N)");
			break;
		}
	case MB_YESNOCANCEL:
		{
			m_nBtCount=3;
			m_uButtonID[0]=IDYES;
			m_uButtonID[1]=IDNO;
			m_uButtonID[2]=IDCANCEL;
			pszString[0]=TEXT("是(Y)");
			pszString[1]=TEXT("否(N)");
			pszString[2]=TEXT("取消(C)");
			break;
		}
	case MB_RETRYCANCEL:
		{
			m_nBtCount=2;
			m_uButtonID[0]=IDRETRY;
			m_uButtonID[1]=IDCANCEL;
			pszString[0]=TEXT("重试(R)");
			pszString[1]=TEXT("取消(C)");
			break;
		}
	case MB_ABORTRETRYIGNORE:
		{
			m_nBtCount=3;
			m_uButtonID[0]=IDABORT;
			m_uButtonID[1]=IDRETRY;
			m_uButtonID[2]=IDIGNORE;
			pszString[0]=TEXT("中止(A)");
			pszString[1]=TEXT("重试(R)");
			pszString[2]=TEXT("忽略(I)");
			break;
		}
	default:
		{
			m_nBtCount=1;
			m_uButtonID[0]=IDOK;
			pszString[0]=TEXT("确定(O)");
			break;
		}
	}

	//设置按钮
	for (UINT i=0;i<m_nBtCount;i++) 
	{
		m_pbtButton[i]->SetTag(m_uButtonID[i]);
		m_pbtButton[i]->SetText(pszString[i]);
	}

	return;
}

//时间消息
VOID CInformation::OnTimer(UINT_PTR nIDEvent)
{
	//倒数时间
	if (nIDEvent==IDI_MESSAGE)
	{
		//删除判断
		if (m_nElapse==0)
		{
			PostMessage(WM_CLOSE,0,0);
			return;
		}

		//设置界面
		TCHAR szTitle[32]=TEXT("");
		_sntprintf(szTitle,CountArray(szTitle),TEXT("%s （%ld 秒）"),m_strCaption,m_nElapse--);
		SetWindowText(szTitle);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////

//显示消息
extern "C" SHARE_CONTROL_CLASS INT ShowInformation(LPCTSTR pszString, UINT uType, UINT nElapse)
{
	CInformation Information;
	return Information.ShowMessageBox(pszString,uType,nElapse);
}

//显示消息
extern "C" SHARE_CONTROL_CLASS INT ShowInformationEx(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType, UINT nElapse)
{
	CInformation Information;
	return Information.ShowMessageBox(pszCaption,pszString,uType,nElapse);
}

//////////////////////////////////////////////////////////////////////////////////
