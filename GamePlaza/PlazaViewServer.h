#ifndef PLAZA_VIEW_SERVER_HEAD_FILE
#define PLAZA_VIEW_SERVER_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

#include "ComplexUI.h"

//////////////////////////////////////////////////////////////////////////////////

//游戏标识
#define GID_BACCARAT				122			//百家乐
#define GID_FIVECARD				25			//港式5张
#define GID_LAND					200			//斗地主
#define GID_OXBATTLE				104			//百人牛牛
#define GID_OXNEW					27			//牛牛
#define GID_ZAJINHUA				6			//扎金花
#define GID_DEZHOU					3			//德洲扑克
#define GID_SPARROWXZ				302			//血战麻将

//////////////////////////////////////////////////////////////////////////////////

#define PAGE_SERVER			6					//页面房间

//////////////////////////////////////////////////////////////////////////////////

//子窗口
#define PARENT_VIEWSERVER_X			256			//用户信息
#define PARENT_VIEWSERVER_Y			154			//用户信息

//////////////////////////////////////////////////////////////////////////////////

//房间列表
class CGameServerItem;

//////////////////////////////////////////////////////////////////////////////////

//游戏服务
typedef CWHArray<CGameServerItem*> CGameServerArray;				

//////////////////////////////////////////////////////////////////////////////////

//游戏视图
class CPlazaViewServer : public CFGuiWnd
{	
	//背景变量
public:	
	CPngImage						m_ImageBack;						//背景图片

	//控制参数
public:
	WORD							m_GameServerPage;
	CComplexUI*						m_pButtonArray[PAGE_SERVER];
	CLabelUI*						m_pLabelArray[PAGE_SERVER];

	//数据存储
public:
	WORD							m_wUserClickIndex;					//选择房间
	CGameServerArray				m_GameServerArray;					//房间列表

	//函数定义
public:
	//构造函数
	CPlazaViewServer();
	//析构函数
	virtual ~CPlazaViewServer();

	//更新通知
public:
	// 更新房间
	VOID RemoveServerItem();
	// 房间目录
	VOID InsertServerItem( CGameServerItem * pGameServerItem );
	// 更新房间
	VOID UpdateServerItem( WORD wKindID );

	//重载函数
public:
	//结束绘画
	virtual void OnBeginPaintWindow(HDC hDC);
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("Server.xml"); }
	//创建控件
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	//初始控件
	void CreateRealControl( CControlUI * pControl, int x, int y, int cx, int cy);
	
	//服务参数
public:
	CGameServerItem * GetUserChickServerItem();

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