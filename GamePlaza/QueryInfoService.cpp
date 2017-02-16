#include "StdAfx.h"
#include "QueryInfoService.h"

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CQueryInfoService::CQueryInfoService()
{
	m_bQueryUserScore=false;

	//任务组件
	m_MissionManager.InsertMissionItem(this);

	return;
}

//析够函数
CQueryInfoService::~CQueryInfoService()
{
}

//连接事件
bool CQueryInfoService::OnEventMissionLink(INT nErrorCode)
{
	//错误处理
	if (nErrorCode!=0L)
	{
		//任务结束
		OnMissionConclude();
	}
	else
	{
		if (m_bQueryUserScore==true)
		{
			//变量定义
			CMD_GP_QueryInsureInfo QueryInsureInfo;
			ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//设置变量
			QueryInsureInfo.dwUserID=pGlobalUserData->dwUserID;
			lstrcpyn(QueryInsureInfo.szPassword,pGlobalUserData->szPassword,CountArray(QueryInsureInfo.szPassword));

			//发送数据
			ASSERT(GetMissionManager()!=NULL);
			GetMissionManager()->SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));

			return true;
		}
	}

	return true;
}

//关闭事件
bool CQueryInfoService::OnEventMissionShut(BYTE cbShutReason)
{
	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
		CMissionManager * pMissionManager=GetMissionManager();
		if (pMissionManager->AvtiveMissionItem(this,true)==true) return true;

		//任务结束
		OnMissionConclude();
	}

	return true;
}

//读取事件
bool CQueryInfoService::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_USER_INSURE_INFO:		//银行资料
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GP_UserInsureInfo));
				if (wDataSize<sizeof(CMD_GP_UserInsureInfo)) return false;

				//变量定义
				CMD_GP_UserInsureInfo * pUserInsureInfo=(CMD_GP_UserInsureInfo *)pData;

				//任务完成
				OnMissionConclude();

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//更新大厅
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//构造结构
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureInfo->lUserScore;
				InsureEvent.lUserInsure=pUserInsureInfo->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);

				return true;
			}
		case SUB_GP_USER_INSURE_ENABLE_RESULT:	//开通结果
			{
				//效验参数
				ASSERT(wDataSize<=sizeof(CMD_GP_UserInsureEnableResult));
				if (wDataSize>sizeof(CMD_GP_UserInsureEnableResult)) return false;

				//任务完成
				OnMissionConclude();

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//变量定义
				CMD_GP_UserInsureEnableResult * pUserInsureEnableResult=(CMD_GP_UserInsureEnableResult *)pData;

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//设置变量
				pGlobalUserData->cbInsureEnabled = pUserInsureEnableResult->cbInsureEnabled;

				return true;
			}
		case SUB_GP_USER_INSURE_SUCCESS:	//银行成功
			{
				//效验参数
				CMD_GP_UserInsureSuccess * pUserInsureSuccess=(CMD_GP_UserInsureSuccess *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

				//任务完成
				OnMissionConclude();

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//更新大厅
				tagInsurePlazaEvent InsureEvent;
				ZeroMemory(&InsureEvent,sizeof(InsureEvent));

				//构造结构
				InsureEvent.wCommandID=INSURE_COMMAND_UPDATE;
				InsureEvent.lUserScore=pUserInsureSuccess->lUserScore;
				InsureEvent.lUserInsure=pUserInsureSuccess->lUserInsure;
				AfxGetMainWnd()->SendMessage(WM_INSUREPLAZA_EVENT,(WPARAM)&InsureEvent,0);							

				return true;
			}
		case SUB_GP_USER_INSURE_FAILURE:	//银行失败
			{
				//效验参数
				CMD_GP_UserInsureFailure * pUserInsureFailure=(CMD_GP_UserInsureFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

				//失败处理
				OnMissionConclude();

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				return true;
			}
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//任务开始
void CQueryInfoService::OnMissionStart()
{

}

//任务结束
void CQueryInfoService::OnMissionConclude()
{
	m_bQueryUserScore=false;
}

//查询金币
void CQueryInfoService::QueryUserScore()
{
	ASSERT(m_bQueryUserScore==false);
	if (m_bQueryUserScore==true) return;

	m_bQueryUserScore=true;

	//任务开始
	OnMissionStart();

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}
	
	return;
}
//////////////////////////////////////////////////////////////////////////////////
