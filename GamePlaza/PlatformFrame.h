#ifndef PLATFORM_FRAME_HEAD_FILE
#define PLATFORM_FRAME_HEAD_FILE

#include "ServerListData.h"
#include "MissionList.h"
#include "MissionLogon.h"
#include "PlazaViewGame.h"
#include "PlazaViewUser.h"
#include "PlazaViewServer.h"
#include "PlazaViewEntry.h"
#include "PlazaViewNews.h"
#include "PlatformPublicize.h"
#include "PlazaViewNotice.h"

//////////////////////////////////////////////////////////////////////////////////

//热键定义
#define IDI_HOT_KEY_BOSS			0x0100								//老板热键
#define IDI_HOT_KEY_WHISPER			0x0200								//私聊热键

//命令定义
#define IDM_CLOSE					105                                 //关闭窗口
#define IDM_RESTORE                 106                                 //还原显示
#define IDM_MINIMIZE                107                                 //最小化显示
#define IDM_MAXIMIZE                108                                 //最大化显示

//命令定义
#define IDM_SHOW_LOGON				200									//显示登录

//////////////////////////////////////////////////////////////////////////////////


//平台框架
class CPlatformFrame : public CFGuiFrameWnd, public ICustomFaceEvent,public IServerListDataSink
{
	friend class CPlazaViewGame;
	friend class CPlazaViewUser;
	friend class CPlazaViewServer;
	friend class CPlazaViewEntry;
	
//界面变量
protected:
	bool							m_bServerStatu;						//服务状态
	bool							m_bRectify;							//调整标志
	CRect							m_rcNormalSize;						//正常位置

	//任务组件
protected:
	CMissionLogon					m_MissionLogon;						//登录任务
	CMissionList					m_MissionList;						//列表任务
	CMissionManager					m_MissionManager;					//任务管理

	//数据组件
protected:
	CServerListData					m_ServerListData;					//游戏数据

	//应用组件
protected:
	CPlazaViewGame					m_PlazaViewGame;					//广场游戏
	CPlazaViewUser					m_PlazaViewUser;					//用户信息
	CPlazaViewServer				m_PlazaViewServer;					//游戏房间
	CPlazaViewEntry					m_PlazaViewEntry;					//登录游戏
	CPlazaViewNews					m_PlazaViewNews;					//新闻信息
	CPlazaViewNotice				m_PlazaViewNotice;					//公告信息

	//视频配置
protected:
	WORD							m_wAVServerPort;					//视频端口
	DWORD							m_dwAVServerAddr;					//视频地址

protected:
	CPlatformPublicize				m_PlatformNews;						//浏览控件
	CPlatformPublicize				m_PlatformPublicize;				//浏览控件.

	//静态变量
protected:
	static CPlatformFrame *			m_pPlatformFrame;					//框架指针

public:
	//构造函数
	CPlatformFrame();
	//析构函数
	virtual ~CPlatformFrame();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//重载函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("PlatformFrame.xml"); }

	//视频配置
public:
	void SetVideoOption(WORD wAVServerPort, DWORD dwAVServerAddr);

	//头像接口
protected:
	//下载失败
	virtual VOID OnEventSystemFace(DWORD dwUserID, WORD wFaceID);
	//头像数据
	virtual VOID OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo);

	//状态通知
public:
	//获取通知
	virtual VOID OnGameItemFinish();
	//完成通知
	virtual VOID OnGameMatchFinish();
	//获取通知
	virtual VOID OnGameKindFinish(WORD wKindID);

	//更新通知
public:
	//插入通知
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//更新通知
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//删除通知
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//重载函数
protected:
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//消息函数
protected:
	//关闭消息
	VOID OnClose();
	//时间消息
	VOID OnTimer(UINT_PTR nIDEvent);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//位置消息
	VOID OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//改变消息
	VOID OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	//自定消息
protected:
	//热键消息
	LRESULT OnMessageHotKey(WPARAM wParam, LPARAM lParam);
	//任务消息
	LRESULT OnMessageTaskBar(WPARAM wParam, LPARAM lParam);
	//图标消息
	LRESULT OnMessageTrayIcon(WPARAM wParam, LPARAM lParam);
	//事件消息
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);
	//事件消息
	LRESULT OnMessageInstallCancelEvent(WPARAM wParam, LPARAM lParam);
	//事件消息
	LRESULT OnMessageInsurePlazaEvent(WPARAM wParam, LPARAM lParam);

	//游戏消息
protected:
	//进入游戏
	LRESULT OnMessageEnterGame(WPARAM wParam, LPARAM lParam);
	//退出游戏
	LRESULT OnMessageQuitGame(WPARAM wParam, LPARAM lParam);
	//进入房间
	LRESULT OnMessageEnterServer(WPARAM wParam, LPARAM lParam);
	//登录分组
	LRESULT OnMessageLogonServer(WPARAM wParam, LPARAM lParam);
	//登录成功
	LRESULT OnMessageLogonSuccess(WPARAM wParam, LPARAM lParam);
	//退出分组
	LRESULT OnMessageQuitServer(WPARAM wParam, LPARAM lParam);

	//功能函数
public:
	//服务判断
	bool IsEnteredServerItem();
	//用户更新
	VOID UpdateUserInfo();
	//获取实例
	static CPlatformFrame * GetInstance() { return m_pPlatformFrame; }

	DECLARE_MESSAGE_MAP()
};


#endif