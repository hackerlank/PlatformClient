#ifndef MISSION_LOGON_HEAD_FILE
#define MISSION_LOGON_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgLogon.h"
#include "DlgRegister.h"
#include "DlgStatus.h"

//////////////////////////////////////////////////////////////////////////////////

//��¼����
class CMissionLogon : public CDlgStatus, public CMissionItem, public IStatusViewSink
{
	//��Ԫ����
	friend class CDlgLogon;
	friend class CDlgRegister;

	//��������
protected:
	bool							m_bRegister;						//ע���־
	bool							m_bRemPassword;						//��ס����
	bool                            m_bLowVerValidate;                  //�Ͱ汾Ч��
	bool							m_bQueyValidate;					//��ѯ��֤��

	//�ؼ�ָ��
protected:
	CDlgLogon *						m_pDlgLogon;						//��¼����
	CDlgRegister *					m_pDlgRegister;						//��¼����

	//��̬����
protected:
	static CMissionLogon *			m_pMissionLogon;					//����ָ��

	//��������
public:
	//���캯��
	CMissionLogon();
	//��������
	virtual ~CMissionLogon();

	//״̬�ӿ�
public:
	//ȡ������
	virtual VOID OnStatusCancel();

	//���غ���
public:
	// 
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//���ܺ���
public:
	//��ʾ��¼
	VOID ShowLogon(BYTE cbShowFalg=SW_NORMAL);
	//��ʾע��
	VOID ShowRegister(BYTE cbShowFalg=SW_NORMAL);	
	//������Ϣ
	bool SendLogonPacket(BYTE cbValidateFlags);

	//��������
protected:
	//�����û�
	VOID UpdateUserInfo();
	//ִ�е�¼
	bool PerformLogonMission(bool bRemPassword);

	//�����¼�
protected:
	//��¼�ɹ�
	bool OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnSocketSubLogonFailure(VOID * pData, WORD wDataSize);
	//��¼���
	bool OnSocketSubLogonFinish(VOID * pData, WORD wDataSize);
	//������ʾ
	bool OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize);
	//�����б�
	bool OnSocketSubMatchSignupInfo(VOID * pData, WORD wDataSize);
	//�ȼ�����
	bool OnSocketSubGrowLevelConfig(VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnSocketSubLogonValidateMBCard(VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnSocketSubLogonValidatePassPort(VOID * pData, WORD wDataSize);
	//��֤�ǳ�
	bool OnSocketSubVerifyIndividualResult(VOID * pData, WORD wDataSize);
	//��ȡ��֤��
	bool OnSocketSubQueryVerifyCode(VOID * pData, WORD wDataSize);
	//��֤���
	bool OnSocketSubVerifyCodeResult(VOID * pData, WORD wDataSize);
	
	//��̬����
public:
	//��ȡ����
	static CMissionLogon * GetInstance() { return m_pMissionLogon; }

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif