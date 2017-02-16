#ifndef PLAZA_VIEW_NOTICE_HEAD_FILE
#define PLAZA_VIEW_NOTICE_HEAD_FILE

#include "HttpClient.h"
//////////////////////////////////////////////////////////////////////////////////

//子窗口
#define PARENT_VIEWNOTICE_X			327				//公告位置
#define PARENT_VIEWNOTICE_Y			122				//公告位置

//////////////////////////////////////////////////////////////////////////////////

//公告队列
typedef CWHArray<CLabelUI *> CNoticeTextArray;

//////////////////////////////////////////////////////////////////////////////////

class CPlazaViewNotice : public CFGuiWnd
{	
	//背景变量
public:	
	CPngImage				m_ImageBack;			//背景图片

	//移动参数
protected:
	int						m_nFixedWidth;				//标签宽度
	int						m_nFixedHeight;				//标签高度

	int						m_nCurrentX;				//通用高度
	int						m_nCurrentY;				//通用宽度
	int						m_nCurrentW;				//现有宽度
	
	int						m_nMaxCount;				//最大长度
	int						m_nMainWidth;				//主窗宽度
	int						m_nSumWidth;				//队列宽度
		
	int						m_nMoveUint;				//位移长度

	//移动参数
protected:	
	CNoticeTextArray		m_wndNoticeArray;			//通告队列

	//新闻数据
public:	
	CHttpClient					m_cHttpJson;
	CTopRechargeArray			m_cTopRechargeArray;	//顶部新闻

	//函数定义
public:
	//构造函数
	CPlazaViewNotice();
	//析构函数
	~CPlazaViewNotice();
	
	//功能函数
public:
	// Http引导
	void OnHttpNavigate(tagRechargeParameter * Recharge, WORD wSize);
	//添加公告
	void AddNotice(CControlUI * pParent, TCHAR * szNotice);

	//重载函数
public:
	//初始函数
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT(""); }	
	//结束绘画
	virtual void OnBeginPaintWindow(HDC hDC);

	//消息函数
protected:	
	//时钟消息
	VOID OnTimer(UINT nIDEvent);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);	
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	DECLARE_MESSAGE_MAP()
};


#endif