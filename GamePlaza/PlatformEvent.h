#ifndef PLAT_FORM_EVENT_HEAD_FILE
#define PLAT_FORM_EVENT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//控件事件
#define	WM_ENTER_GAME			(WM_USER+3601)							//进入游戏
#define	WM_QUIT_GAME			(WM_USER+3602)							//退出游戏
#define	WM_ENTER_SERVER			(WM_USER+3603)							//进入房间
#define	WM_LOGON_SERVER			(WM_USER+3604)							//登入房间
#define	WM_LOGON_SUCCESS		(WM_USER+3605)							//退出房间
#define	WM_QUIT_SERVER			(WM_USER+3606)							//退出房间
#define WM_UPDATE_USERINFO		(WM_USER+3610)							//用户更新	

//删除房间
#define WM_DELETESERVERITEM		(WM_USER+3615)							//退出房间	

//////////////////////////////////////////////////////////////////////////////////

#define	WM_VIEW_NOTICE			(WM_USER+3701)							//广告信息
#define WM_VIEW_NEWS			(WM_USER+3702)							//新闻信息	

//////////////////////////////////////////////////////////////////////////////////

//用户事件
#define EVENT_USER_LOGON			1									//登录成功
#define EVENT_USER_LOGOUT			2									//注销成功
#define EVENT_USER_INFO_UPDATE		3									//资料更新

//系统消息
#define EVENT_DOWN_LOAD_FINISH		20									//下载成功
#define EVENT_SKIN_CONFIG_UPDATE	21									//资源更新

//////////////////////////////////////////////////////////////////////////////////

//通知消息
#define WM_PLATFORM_EVENT			(WM_USER+1000)						//平台事件

//////////////////////////////////////////////////////////////////////////////////


//事件管理
class CPlatformEvent
{
	//变量定义
protected:
	CWHArray<HWND>					m_EventWndArray;					//事件窗口

	//静态变量
protected:
	static CPlatformEvent *			m_pPlatformEvent;					//对象指针

	//函数定义
public:
	//构造函数
	CPlatformEvent();
	//析构函数
	virtual ~CPlatformEvent();

	//注册窗口
public:
	//注册窗口
	bool RegisterEventWnd(HWND hEventWnd);
	//注销窗口
	bool UnRegisterEventWnd(HWND hEventWnd);

	//发送事件
public:
	//发送事件
	VOID SendPlatformEvent(WORD wEventID, LPARAM lParam);
	//发送事件
	VOID PostPlatformEvent(WORD wEventID, LPARAM lParam);

	//静态函数
public:
	//获取对象
	static CPlatformEvent * GetInstance() { return m_pPlatformEvent; }
};


//////////////////////////////////////////////////////////////////////////////////

#endif