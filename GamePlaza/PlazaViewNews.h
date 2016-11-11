#ifndef BUTTON_ITEM_HEAD_FILE
#define BUTTON_ITEM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//子窗口
#define PARENT_VIEWNEWS_X			23				//新闻信息
#define PARENT_VIEWNEWS_Y			433				//新闻信息

//////////////////////////////////////////////////////////////////////////////////

const TCHAR kWebLinkClassName[] = _T("WebLinkUI");  
const TCHAR kWebLinkInterface[] = _T("WebLink");  

//////////////////////////////////////////////////////////////////////////////////

class CWebLinkUI : public CButtonUI 
{
	//标识变量
protected:
	bool				m_bHoverServer;				//盘旋标识

	//重载函数
public:
	//构造函数
    CWebLinkUI();  
	//析构函数
	~CWebLinkUI();

	//接口函数
public:
	//获得类名
    LPCTSTR GetClass() const;  
    //接口函数
	LPVOID GetInterface(LPCTSTR pstrName);  

	//重载函数
public:
	//初始函数
	virtual void InitControlUI();
	//事件重载
	void DoEvent(TEventUI& event);

	//继承函数
public: 
    //重构属性
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);  
	//重新绘制
    void PaintStatusImage(HDC hDC);
};


//////////////////////////////////////////////////////////////////////////////////

class CPlazaViewNews : public CFGuiWnd
{
	//函数定义
public:
	//构造函数
	CPlazaViewNews();
	//析构函数
	~CPlazaViewNews();
	
	//重载函数
public:	
	//创建控件
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	//初始函数
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("PlazaViewNews.xml"); }	

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