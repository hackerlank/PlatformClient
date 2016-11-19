#ifndef DLG_REGISTER_HEAD_FILE
#define DLG_REGISTER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "WndValidateCode.h"

//////////////////////////////////////////////////////////////////////////////////

//注册窗口
class CDlgRegister : public CFGuiDialog
{
	//验证变量
public:	
	CPngImage						m_ImageBack;						//背景图片

	//验证变量
public:
	bool							m_bQueryVerifyCode;					//查询验证码
	bool							m_bVerifyIndividual;				//验证资料
	WORD							m_wVerifyMask;						//校验掩码							

	//变量定义
public:
	WORD							m_wFaceID;							//头像标识
	BYTE							m_cbGender;							//用户性别
	TCHAR							m_szAccounts[LEN_ACCOUNTS];			//注册帐号
	TCHAR							m_szNickName[LEN_NICKNAME];			//注册昵称
	TCHAR							m_szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							m_szLogonPass[LEN_PASSWORD];		//登录密码
	TCHAR							m_szInsurePass[LEN_PASSWORD];		//银行密码
	TCHAR							m_szPassPortID[LEN_PASS_PORT_ID];	//证件号码
	TCHAR							m_szQQNumber[LEN_QQ_NUMBER_ID];		//QQ号码
	TCHAR							m_szCompellation[LEN_COMPELLATION];	//真实名字
	TCHAR							m_szVerifyCode[LEN_VERIFY_CODE];	//验证码

	//配置变量
protected:
	BYTE							m_cbRemPassword;					//记住密码
	BYTE							m_cbLogonPassLevel;					//密码强度
	BYTE							m_cbInsurePassLevel;				//密码强度

	//控件变量
protected:
	CBrush							m_brBrush;							//背景画刷
	CSkintDPI                       m_SkinDPI;                          //设置DPI
	CFaceItemView					m_FaceItemView;						//头像视图

	//组件变量
protected:
	CSkinLayered					m_SkinLayered;						//分层窗口
	CWndValidateCode				m_WndValidateCode;					//验证控件
	CFaceSelectControlHelper		m_FaceSelectControl;				//头像选择

	//函数定义
public:
	//构造函数
	CDlgRegister();
	//析构函数
	virtual ~CDlgRegister();
	
	//绘画接口
public:
	//结束绘画
	virtual void OnBeginPaintWindow(HDC hDC);

	//重载函数
public:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//配置函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消函数
	virtual VOID OnCancel();

	//功能函数
public:
	//构造数据
	WORD ConstructRegisterPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags);
	//校验结果
	VOID OnVerifypIndividualResult(WORD wVerifyMask,bool bVerifyPassage,LPCTSTR pszErrorMsg=NULL);
	//查询验证结果
	VOID OnQueryVerifyCodeResult(LPCTSTR pszVerifyCode, UINT nMaxCount);

	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT(""); } 
	//初始控件
	virtual void InitControlUI();
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);
	
	//控件构建
public:
	//创建星号
	void CreateStarLabel(CLabelUI *pLabelUI, int x, int y, int cx, int cy);
	//追加文本
	void CreateTextLabel(CLabelUI *pLabelUI, LPCTSTR szName, int x, int y, int cx, int cy);
	//文本控件
	void CreateEditControl(CEditUI *pEditUI, LPCTSTR szName, int x, int y, int cx, int cy);
	//文本说明
	void CreateExplainLabel(CLabelUI *pLabelUI, LPCTSTR szName, int x, int y, int cx, int cy);
	//按钮控件
	void CreateButtonControl(CButtonUI *pButtonUI, LPCTSTR szImage, int x, int y, int cx, int cy);
	//单选控件
	void CreateRadioButtonControl(CRadioButtonUI *pRadioButtonUI, LPCTSTR szImage, int x, int y, int cx, int cy);

	//绘画接口
public:
	//结束绘画
	virtual void OnEndPaintWindow(HDC hDC);

	//辅助函数
private:
	//获取信息
	bool GetInformation();
	//验证资料
	VOID PerformVerifyIndividual(WORD wVerifyMask);
	//查询验证码
	VOID PerformQueryVerifyCode();
	//获取字符
	VOID GetControlString(CControlUI * pControlUI,TCHAR szString[], WORD wMaxCount);

	//消息处理
public:
	//关闭按钮
	VOID OnBnClickedQuit();
	//密码输入
	VOID OnEnChangeLogonPass();
	//密码输入
	VOID OnEnChangeInsurePass();
	//头像选择
	VOID OnBnClickedSelectFace();
	//更换验证
	VOID OnBnClickedChangeValidate();
	//同意条款
	VOID OnBnClickedAgrrArticle();

	//系统消息
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//控件颜色
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif