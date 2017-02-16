#ifndef PLAZA_VIEW_ENTRY_HEAD_FILE
#define PLAZA_VIEW_ENTRY_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

#include "ProcessManager.h"
#include "DlgStatus.h"
#include "GlobalUnits.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////

//服务状态
enum enServiceStatus
{
	ServiceStatus_Unknow,			//未知状态
	ServiceStatus_Entering,			//进入状态
	ServiceStatus_Validate,			//验证状态
	ServiceStatus_RecvInfo,			//读取状态
	ServiceStatus_ServiceIng,		//服务状态
	ServiceStatus_NetworkDown,		//中断状态
};

//////////////////////////////////////////////////////////////////////////////////

//查找桌子
struct tagSearchTable
{
	bool							bOneNull;							//一个空位
	bool							bTwoNull;							//两个空位
	bool							bAllNull;							//全空位置
	bool							bNotFull;							//不全满位
	bool							bFilterPass;						//过滤密码
	WORD							wStartTableID;						//起始桌子
	WORD							wResultTableID;						//结果桌子
	WORD							wResultChairID;						//结果椅子
};

//////////////////////////////////////////////////////////////////////////////////

//子窗口
#define PARENT_VIEWENTRY_X			256		//用户信息
#define PARENT_VIEWENTRY_Y			154		//用户信息

//////////////////////////////////////////////////////////////////////////////////

//游戏视图
class CPlazaViewEntry : public CFGuiWnd, public IServerViewItem, public ITCPSocketSink, public ITableViewFrameSink, public IUserManagerSink, public IProcessManagerSink,public IStatusViewSink
{
	//背景变量
public:	
	CPngImage						m_ImageBack;						//背景图片

	//辅助变量
protected:
	WORD							m_wReqTableID;						//请求桌子
	WORD							m_wReqChairID;						//请求位置
	WORD							m_wFindTableID;						//查找桌子
	WORD							m_wResultTableID;					//结果桌子
	WORD							m_wInvitedTableID;					//被邀桌子
	bool							m_bUserInRoom;						//是否在房间

	//房间属性
protected:
	WORD							m_wServerType;						//房间类型
	DWORD							m_dwServerRule;						//房间规则
	enServiceStatus					m_ServiceStatus;					//房间状态
	WORD							m_wAVServerPort;					//视频端口
	DWORD							m_dwAVServerAddr;					//视频地址
	TCHAR							m_szServerPassword[LEN_PASSWORD];	//房间密码

	//用户属性
protected:
	DWORD							m_dwUserRight;						//用户权限
	DWORD							m_dwMasterRight;					//管理权限
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针

	//数据存储
public:
	//游戏房间
	WORD							m_wChairCount;						//椅子数量
	tagGameKind						m_GameKind;							//类型信息
	tagGameServer					m_GameServer;						//房间信息

	//组件变量
protected:
	CProcessManager					m_ProcessManager;					//进程管理
	CTCPSocketHelper				m_TCPSocketModule;					//网络连接
	CGameLevelParserHelper			m_GameLevelParserModule;			//游戏等级
	CPlazaUserManagerHelper			m_PlazaUserManagerModule;			//用户管理

	//配置参数
protected:
	CParameterGame *				m_pParameterGame;					//游戏配置
	CParameterServer *				m_pParameterServer;					//房间配置
	
	//配置信息
protected:
	BYTE							m_cbColumnCount;					//列表数目
	BYTE							m_cbPropertyCount;					//道具数目
	tagColumnItem					m_ColumnItem[MAX_COLUMN];			//列表描述
	tagPropertyInfo					m_PropertyInfo[MAX_PROPERTY];		//道具描述

	//功能组件
public:
	CDlgStatus						m_DlgStatus;						//状态窗口	

	//接口变量
public:
	ITableViewFrame *				m_pITableViewFrame;					//桌子框架

	//函数定义
public:
	//构造函数
	CPlazaViewEntry();
	//析构函数
	virtual ~CPlazaViewEntry();

	//基础接口
public:
	//释放对象
	virtual VOID Release();
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

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

	//游戏消息
public:
	//创建视图
	bool CreateViewFrame();
	//重置视图
	VOID ResetViewFrame(bool isGameing = true);
	//初始游戏
	VOID InitGameServer( CGameServerItem * pServerItem );
	//加入游戏
	VOID PerformJoinGame();
	//大厅关闭
	bool PlazaClosing();

	//桌子接口
public:
	//鼠标双击
	virtual VOID OnDButtonHitTable(WORD wTableID);
	//左键按下
	virtual VOID OnLButtonHitTable(WORD wTableID, WORD wChairID);
	//右键按下
	virtual VOID OnRButtonHitTable(WORD wTableID, WORD wChairID);
	//桌子按钮
	virtual VOID OnHitTableButton(BYTE cbButton);

	//网络接口
protected:
	//连接事件
	virtual bool OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode);
	//关闭事件
	virtual bool OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason);
	//读取事件
	virtual bool OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//进程接口
public:
	//进程错误
	virtual bool OnGameProcessError();
	//进程关闭
	virtual bool OnGameProcessClose(DWORD dwExitCode);
	//进程启动
	virtual bool OnGameProcessCreate(bool bAlreadyExist);
	//进程数据
	virtual bool OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//网络命令
public:
	//发送规则
	bool SendUserRulePacket();
	//发送登录
	bool SendLogonPacket(LPCTSTR pszServerPasswd);	
	//发送旁观
	bool SendLookonPacket(WORD wTableID, WORD wChairID);
	//发送坐下
	bool SendSitDownPacket(WORD wTableID, WORD wChairID, LPCTSTR lpszPassword=NULL);
	//发送起立
	bool SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave);
	//发送表情
	bool SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex);
	//发送聊天
	bool SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor);

	//用户接口
public:
	//用户激活
	virtual VOID OnUserItemAcitve(IClientUserItem * pIClientUserItem);
	//用户删除
	virtual VOID OnUserItemDelete(IClientUserItem * pIClientUserItem);
	//用户更新
	virtual VOID OnUserFaceUpdate(IClientUserItem * pIClientUserItem);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserAttrib & UserAttrib);

	//动作处理
public:
	//执行起立
	bool PerformStandUpAction(WORD wTableID, WORD wChairID);
	//执行坐下
	bool PerformSitDownAction(WORD wTableID, WORD wChairID, bool bEfficacyPass);

	//内部函数
protected:
	//桌子效验
	bool EfficacyTableRule(WORD wTableID, WORD wChairID, bool bReqLookon, CString & strDescribe);

	//网络事件
protected:
	//用户处理
	bool OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//登录处理
	bool OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//配置处理
	bool OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//状态处理
	bool OnSocketMainStatus(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//银行处理
	bool OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//任务处理
	bool OnSocketMainTask(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//兑换处理
	bool OnSocketMainExchange(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//管理处理
	bool OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//系统处理
	bool OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//游戏处理
	bool OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//比赛处理
	bool OnSocketMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//网络事件
protected:
	//用户进入
	bool OnSocketSubUserEnter(VOID * pData, WORD wDataSize);
	//用户积分
	bool OnSocketSubUserScore(VOID * pData, WORD wDataSize);
	//用户状态
	bool OnSocketSubUserStatus(VOID * pData, WORD wDataSize);
	//用户游戏数据
	bool OnSocketSubUserItemGameData(VOID * pData, WORD wDataSize);
	//登录完成
	bool OnSocketSubLogonFinish(VOID * pData, WORD wDataSize);
	//登录成功
	bool OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize);
	//登录失败
	bool OnSocketSubLogonFailure(VOID * pData, WORD wDataSize);
	//更新提示
	bool OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize);
	//请求失败
	bool OnSocketSubRequestFailure(VOID * pData, WORD wDataSize);
	//用户聊天
	bool OnSocketSubUserChat(VOID * pData, WORD wDataSize);
	//用户表情
	bool OnSocketSubExpression(VOID * pData, WORD wDataSize);
	//用户私聊
	bool OnSocketSubWisperUserChat(VOID * pData, WORD wDataSize);
	//私聊表情
	bool OnSocketSubWisperExpression(VOID * pData, WORD wDataSize);
	//系统消息
	bool OnSocketSubSystemMessage(VOID * pData, WORD wDataSize);
	//动作消息
	bool OnSocketSubActionMessage(VOID * pData, WORD wDataSize);
	//邀请玩家
	bool OnSocketSubUserInvite(VOID * pData, WORD wDataSize);
	//道具成功
	bool OnSocketSubPropertySuccess(VOID * pData, WORD wDataSize);
	//道具失败
	bool OnSocketSubPropertyFailure(VOID * pData, WORD wDataSize);
	//道具效应
	bool OnSocketSubPropertyEffect(VOID * pData, WORD wDataSize);
	//道具消息
	bool OnSocketPropertyMessage(VOID * pData, WORD wDataSize);
	//道具喇叭
	bool OnSocketPropertyTrumpet(VOID * pData, WORD wDataSize);
	//等待分配
	bool OnSocketSubWaitDistribute(VOID *pData, WORD wDataSize);

	//辅助函数
protected:
	//设置状态
	VOID SetServiceStatus(enServiceStatus ServiceStatus);
	//查询游戏客户端
	bool IsGameClientReady();
	//资源目录
	VOID GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount);
	//查找桌子
	VOID SearchGameTable(tagSearchTable & SearchTable);
	//自己状态
	bool IsPlayingMySelf();
	//游戏消息

	//游戏消息
protected:
	//删除游戏
	LRESULT OnDeleteServerItem(WPARAM wParam, LPARAM lParam);

	//事件接口
protected:
	//取消连接
	virtual VOID OnStatusCancel();

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