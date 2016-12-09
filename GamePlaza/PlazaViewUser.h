#ifndef PLAZA_USERINFO_HEAD_FILE
#define PLAZA_USERINFO_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//子窗口
#define PARENT_USERINFO_X			10							//用户信息
#define PARENT_USERINFO_Y			120							//用户信息

//////////////////////////////////////////////////////////////////////////////////

//游戏视图
class CPlazaViewUser : public CFGuiWnd
{
	//背景变量
public:	
	CPngImage						m_ImageBack; //背景图片

	//函数定义
public:
	//构造函数
	CPlazaViewUser();
	//析构函数
	virtual ~CPlazaViewUser();
	
	//辅助功能
public:
	//添加符号
	CString AddComma( LONGLONG lScore , bool bPlus = false );

	//重载函数
public:
	//初始控件
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
	
	//消息处理
protected:
	//更新数据
	LRESULT OnMessageUpdateDate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


#endif