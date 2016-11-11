#ifndef DLG_ENQUIRE_HEAD_FILE
#define DLG_ENQUIRE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//关闭窗口
class CDlgEnquire : public CFGuiDialog
{
	//控件变量
protected:
	CButtonUI *						m_pbtCancel;						//取消操作
	CButtonUI *						m_pbtClosePlaza;					//关闭大厅
	CButtonUI *						m_pbtCloseServer;					//取消操作
	CButtonUI *						m_pbtSwitchAccounts;				//切换帐号

	//界面资源
protected:
	CSkinLayered					m_SkinLayered;						//分层窗口
	CPlatformPublicize				m_PlatformPublicize;				//浏览控件

	//函数定义
public:
	//构造函数
	CDlgEnquire();
	//析构函数
	virtual ~CDlgEnquire();

	//重载函数
protected:
	//配置函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();

	//接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);

	//绘画接口
public:
	//结束绘画
	virtual void OnBeginPaintWindow(HDC hDC);

	//按钮消息
protected:
	//关闭大厅
	VOID OnBnClickedClosePlaza();
	//取消操作
	VOID OnBnClickedCloseServer();
	//切换帐号
	VOID OnBnClickedSwitchAccounts();

	//辅助函数
protected:
	//调整控件
	VOID RectifyControlUI();
};

//////////////////////////////////////////////////////////////////////////////////

#endif