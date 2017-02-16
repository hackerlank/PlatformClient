#include "StdAfx.h"
#include "Resource.h"
#include "DlgGmbCard.h"
#include "GlobalUnits.h"
#include "MissionUpdate.h"
#include "PlatformEvent.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CMissionUpdate * CMissionUpdate::m_pMissionUpdate=NULL;					//对象指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMissionUpdate, CDlgStatus)
END_MESSAGE_MAP()

//构造函数
CMissionUpdate::CMissionUpdate()
{
	//设置组件
	SetStatusViewSink(this);

	//设置对象
	ASSERT(m_pMissionUpdate==NULL);
	if (m_pMissionUpdate==NULL) m_pMissionUpdate=this;

	return;
}

//析构函数
CMissionUpdate::~CMissionUpdate()
{
	//释放对象
	ASSERT(m_pMissionUpdate==this);
	if (m_pMissionUpdate==this) m_pMissionUpdate=NULL;

	return;
}


//取消连接
VOID CMissionUpdate::OnStatusCancel()
{
	//隐藏界面
	HideStatusWindow();

	//终止任务
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,true);
}


//连接事件
bool CMissionUpdate::OnEventMissionLink(INT nErrorCode)
{
	//错误处理
	if (nErrorCode!=0L) {

		//隐藏界面
		HideStatusWindow();

		return false;
	}
	else {
		//充值信息
		SendRechargePacket();

		//新闻信息
		SendNoticePacket();
	}

	return true;
}

//关闭事件
bool CMissionUpdate::OnEventMissionShut(BYTE cbShutReason)
{
	//隐藏界面
	HideStatusWindow();

	return true;
}

//读取事件
bool CMissionUpdate::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_LOAD_TOPRECHARGE:	//充值信息
			{
				CMD_GP_Recharge * pCMDRecharge=(CMD_GP_Recharge *)pData;

				return OnSocketSubQueryRecharge(pData,wDataSize);
			}
		case SUB_GP_LOAD_NOTICE:		//公告信息
			{
				return OnSocketSubQueryNotice(pData,wDataSize);
			}
		}
	}
	return false;
}

//执行登录
bool CMissionUpdate::PerformUpdateMission(CPlazaViewNews * pViewNews, CPlazaViewNotice * pViewNotice)
{
	//接收窗口
	if (pViewNews==NULL) return false;
	if (pViewNotice==NULL) return false;
	m_pViewNews = pViewNews;
	m_pViewNotice = pViewNotice;

	//效验状态
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true) return false;

	//获取地址
	TCHAR szCustomServer[LEN_SERVER]=TEXT("菲娱棋牌娱乐");

	//设置地址
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->SetCustomServer(szCustomServer);

	//发起连接
	if (pMissionManager->AvtiveMissionItem(this,false)==false)
	{
		//错误不提示
		return false;
	}

	return true;
}

//充值信息
bool CMissionUpdate::SendRechargePacket()
{
	//变量定义
	WORD wPacketSize,wSubCmdID;
	BYTE cbBuffer[SOCKET_TCP_PACKET]={0};

	//构造数据
	wSubCmdID = SUB_GP_QUERY_RECHARGE_DATA;
	wPacketSize = sizeof(CMD_GP_Recharge);

	//发送数据
	ASSERT(GetMissionManager()!=NULL);
	GetMissionManager()->SendData(MDM_GP_USER_SERVICE,wSubCmdID,cbBuffer,wPacketSize);
	return true;
}

//公告信息
bool CMissionUpdate::SendNoticePacket()
{
	//变量定义
	WORD wPacketSize,wSubCmdID;
	BYTE cbBuffer[SOCKET_TCP_PACKET]={0};

	//构造数据
	wSubCmdID= SUB_GP_QUERY_NOTICE_DATA;
	wPacketSize = sizeof(CMD_GP_Notices);

	//发送数据
	ASSERT(GetMissionManager()!=NULL);
	GetMissionManager()->SendData(MDM_GP_USER_SERVICE,wSubCmdID,cbBuffer,wPacketSize);
	return true;
}


//////////////////////////////////////////////////////////////////////////////////
//充值信息
bool CMissionUpdate::OnSocketSubQueryRecharge(VOID * pData, WORD wDataSize)
{

	//变量定义
	CMD_GP_Recharge * pCMDRecharge=(CMD_GP_Recharge *)pData;
	WORD wMinSize = __min(pCMDRecharge->wRechargeCount, VIEW_RECHARGE_UPDATE);

	//设置变量
	tagRechargeParameter RechargeParameter[VIEW_RECHARGE_UPDATE+1];
	ZeroMemory(RechargeParameter, sizeof(RechargeParameter));

	//读取数据
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pCMDRecharge+1, wDataSize-sizeof(CMD_GP_Recharge));

	//扩展信息
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) 
			break;

		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_RECHARGE_DATA:		//
			CopyMemory(RechargeParameter, pDataBuffer, sizeof(tagRechargeParameter)*wMinSize );
			break;
		}
	}

	m_pViewNotice->OnHttpNavigate(RechargeParameter, pCMDRecharge->wRechargeCount);
	return true;
}

//信息链接
bool CMissionUpdate::OnSocketSubQueryNotice(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GP_Notices * pCMDNotices=(CMD_GP_Notices *)pData;
	WORD wMinSize = __min(pCMDNotices->wNoticesCount, VIEW_NOTICE_UPDATE);

	//设置变量
	tagNoticesParameter NoticesParameter[VIEW_NOTICE_UPDATE+1];
	ZeroMemory(NoticesParameter, sizeof(NoticesParameter));

	//读取数据
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pCMDNotices+1, wDataSize-sizeof(CMD_GP_Recharge));

	//扩展信息
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) 
			break;

		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_NOTICE_DATA:		//
			CopyMemory(NoticesParameter, pDataBuffer, sizeof(tagNoticesParameter)*wMinSize );
			break;
		}
	}

	m_pViewNews->OnHttpNavigate(NoticesParameter, pCMDNotices->wNoticesCount);
	return true;
}