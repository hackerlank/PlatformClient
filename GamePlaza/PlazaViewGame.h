#ifndef PLAZA_VIEW_GAME_HEAD_FILE
#define PLAZA_VIEW_GAME_HEAD_FILE

#include "ServerListData.h"
#include "DlgUpdateStatus.h"
#include "PlazaViewServer.h"

//////////////////////////////////////////////////////////////////////////////////

//子窗口
#define PARENT_VIEWGAME_X			256		//游戏区域
#define PARENT_VIEWGAME_Y			154		//游戏区域

//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define VIEW_MODE_NONE				0									//无效模式
#define VIEW_MODE_KIND				1									//类型模式
#define VIEW_MODE_MATCH				2									//比赛模式
#define VIEW_MODE_SERVER			3									//房间模式

//按钮标识
#define BT_VIEW_RULE				1									//查看规则
#define BT_ENTER_KIND				2									//进入类型
#define BT_ENTER_SERVER				3									//进入房间

//常量定义
#define VAR_TYPE_PAGECOUNT			0									//变量类型
#define VAR_TYPE_PAGECURRENT		1									//变量类型
#define VAR_TYPE_PAGECONTAINER		2									//变量类型

//常量定义
#define WM_CLICKED_LEFT				0									//上一页
#define WM_CLICKED_RIGHT			1									//下一页

//////////////////////////////////////////////////////////////////////////////////

//类型声明
class CGameViewSubItem;
class CGameViewKindItem;
class CGameViewServerItem;
class CGameViewMatchItem;
class CPlazaViewServer;

//////////////////////////////////////////////////////////////////////////////////

//类型定义
struct tagGameKindInfo
{
	WORD							wSortID;							//排序标识
	CGameKindItem *					pGameKindItem;						//类型对象
};

//////////////////////////////////////////////////////////////////////////////////

//比赛定义
struct tagGameMatchInfo
{
	WORD							wSortID;							//排序标识
	CGameServerItem *				pGameServerItem;					//房间对象
	CGameViewMatchItem *			pGameViewMatchItem;					//比赛子项
};

//////////////////////////////////////////////////////////////////////////////////

//房间定义
struct tagGameServerInfo
{
	WORD							wSortID;							//排序标识
	WORD							wServerID;							//房间标识
	CGameServerItem *				pGameServerItem;					//房间对象
};

//////////////////////////////////////////////////////////////////////////////////

//布局属性
struct tagViewLayoutAttribute
{
	WORD							wItemXCount;						//横向数目
	WORD							wItemYCount;						//纵向数目
	CSize							SizeItem;							//子项尺寸
	CRect							rcItemMargin;						//子项边距
};

//////////////////////////////////////////////////////////////////////////////////

//视图属性
struct tagViewPageControl
{
	WORD							wPageCount;							//页面总数
	WORD							wPageCurrent;						//当前页面
	CContainerUI *					pPageContainer;						//页面容器	
};

//////////////////////////////////////////////////////////////////////////////////

//视图属性
class CGameButtonImage
{
public:
	CGameButtonImage(WORD wID, const TCHAR * szName, const TCHAR* szImage);
	~CGameButtonImage();

public:
	WORD							wGameID;							//游戏标识
	DWORD							dwVersion;							//版本标识
	const TCHAR*					wButtonName;						//游戏名称
	const TCHAR*					wButtonImage;						//游戏图片	
};

//////////////////////////////////////////////////////////////////////////////////

//页面模式
enum emPageType
{
	pmNone=0,
	pmGameKind,
	pmGameMatch,
	pmGameServer
};

//////////////////////////////////////////////////////////////////////////////////

//数组定义
typedef CWHArray<tagGameKindInfo *>	CGameKindInfoArray;					//类型数组
typedef CWHArray<tagGameMatchInfo *> CGameMatchInfoArray;				//比赛数组
typedef CWHArray<tagGameServerInfo *> CGameServerInfoArray;				//房间数组

//////////////////////////////////////////////////////////////////////////////////

//索引说明
typedef CMap<WORD, WORD, CGameButtonImage *, CGameButtonImage * &> CButtonImageMap;

//////////////////////////////////////////////////////////////////////////////////
//接口定义
interface IViewPageSubItemSink
{
public:
	//类型点击
	virtual void OnViewPageKindItemClicked(tagGameKindInfo * pGameKindInfo)=NULL;	
	//房间点击
	virtual void OnViewPageServerItemClicked(tagGameServerInfo * pGameServerInfo)=NULL;
	//比赛点击
	virtual void OnViewPageMatchItemClicked(tagGameMatchInfo * pGameMatchInfo,WORD wCommandID)=NULL;
};


//游戏视图
class CPlazaViewGame : public CFGuiWnd, public IViewPageSubItemSink, public IUpdateStatusSink
{
	//背景变量
public:	
	CPngImage				m_ImageBack;						//背景图片

	//按钮参数
public:
	//按钮图像
	CButtonImageMap			m_ButtonImageMap;

	//数据存储
public:
	//游戏服务
	CGameServerArray		m_GameServerArray;

	//函数定义
public:
	//构造函数
	CPlazaViewGame();
	//析构函数
	virtual ~CPlazaViewGame();

	//更新事件
public:
	//下载完成
	virtual VOID OnEventDownLoadTaskFinish();
	//检测完成
	virtual VOID OnEventUpdateCheckFinish(bool bNeedUpdate);	

	//更新通知
public:
	// 更新游戏
	VOID InsertGameItem( WORD wGameID, DWORD dwProcessVersion );
	// 更新房间
	VOID InsertServerItem( CGameServerItem * pGameServerItem );
	// 房间目录
	VOID UpdateServerItem( WORD wKindID );

public:
	//类型点击
	virtual void OnViewPageKindItemClicked(tagGameKindInfo * pGameKindInfo);	
	//房间点击
	virtual void OnViewPageServerItemClicked(tagGameServerInfo * pGameServerInfo);
	//比赛点击
	virtual void OnViewPageMatchItemClicked(tagGameMatchInfo * pGameMatchInfo,WORD wCommandID);

	//重载函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT(""); }	
	//设置属性
	virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue);
	//结束绘画
	virtual void OnBeginPaintWindow(HDC hDC);

	//事件函数
protected:
	//上页点击
	LRESULT OnClickedLeftPageView(WPARAM wParam, LPARAM lParam);
	//下页点击
	LRESULT OnClickedRightPageView(WPARAM wParam, LPARAM lParam);

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