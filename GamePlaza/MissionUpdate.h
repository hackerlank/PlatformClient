#ifndef MISSION_UPDATE_HEAD_FILE
#define MISSION_UPDATE_HEAD_FILE
#pragma once

#include "Stdafx.h"
#include "DlgStatus.h"

//////////////////////////////////////////////////////////////////////////////////

//空间定义
#define VIEW_RECHARGE_UPDATE			(12)							//充值信息
#define VIEW_NOTICE_UPDATE				(8)								//新闻信息

//////////////////////////////////////////////////////////////////////////////////

//登录任务
class CMissionUpdate : public CDlgStatus, public CMissionItem, public IStatusViewSink
{
	//友元定义
	friend class CPlazaViewNews;
	friend class CPlazaViewNotice;

	//控件指针
protected:
	CPlazaViewNews					  * m_pViewNews;
	CPlazaViewNotice				  * m_pViewNotice;

	//静态变量
protected:
	static CMissionUpdate *				m_pMissionUpdate;					//对象指针

	//函数定义
public:
	//构造函数
	CMissionUpdate();
	//析构函数
	virtual ~CMissionUpdate();

	//状态接口
public:
	//取消连接
	virtual VOID OnStatusCancel();

	//重载函数
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//功能函数
protected:
	//充值信息
	bool SendRechargePacket();
	//公告信息
	bool SendNoticePacket();

	//辅助函数
public:
	//执行登录
	bool PerformUpdateMission( CPlazaViewNews * pViewNews, CPlazaViewNotice * pViewNotice );

	//网络事件
protected:
	//充值信息
	bool OnSocketSubQueryRecharge(VOID * pData, WORD wDataSize);
	//信息链接
	bool OnSocketSubQueryNotice(VOID * pData, WORD wDataSize);
	
	//静态函数
public:
	//获取对象
	static CMissionUpdate * GetInstance() { return m_pMissionUpdate; }

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif