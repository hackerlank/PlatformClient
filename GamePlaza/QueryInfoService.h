#ifndef QUERY_INFO_SERVICE_HEAD_FILE
#define QUERY_INFO_SERVICE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////
//锁机设置
class CQueryInfoService : public CMissionItem
{
protected:
	bool							m_bQueryUserScore;					//查询分数

	//组件变量
protected:
	CMissionManager					m_MissionManager;					//任务管理

	//函数定义
public:
	//构造函数
	CQueryInfoService();
	//析够函数
	virtual ~CQueryInfoService();

	//重载函数
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

protected:
	//任务开始
	virtual void OnMissionStart();
	//任务结束
	virtual void OnMissionConclude();

	//服务接口
public:
	//查询金币
	void QueryUserScore();
};

//////////////////////////////////////////////////////////////////////////////////

#endif