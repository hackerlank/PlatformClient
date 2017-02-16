#ifndef MISSION_UPDATE_HEAD_FILE
#define MISSION_UPDATE_HEAD_FILE
#pragma once

#include "Stdafx.h"
#include "DlgStatus.h"

//////////////////////////////////////////////////////////////////////////////////

//�ռ䶨��
#define VIEW_RECHARGE_UPDATE			(12)							//��ֵ��Ϣ
#define VIEW_NOTICE_UPDATE				(8)								//������Ϣ

//////////////////////////////////////////////////////////////////////////////////

//��¼����
class CMissionUpdate : public CDlgStatus, public CMissionItem, public IStatusViewSink
{
	//��Ԫ����
	friend class CPlazaViewNews;
	friend class CPlazaViewNotice;

	//�ؼ�ָ��
protected:
	CPlazaViewNews					  * m_pViewNews;
	CPlazaViewNotice				  * m_pViewNotice;

	//��̬����
protected:
	static CMissionUpdate *				m_pMissionUpdate;					//����ָ��

	//��������
public:
	//���캯��
	CMissionUpdate();
	//��������
	virtual ~CMissionUpdate();

	//״̬�ӿ�
public:
	//ȡ������
	virtual VOID OnStatusCancel();

	//���غ���
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//���ܺ���
protected:
	//��ֵ��Ϣ
	bool SendRechargePacket();
	//������Ϣ
	bool SendNoticePacket();

	//��������
public:
	//ִ�е�¼
	bool PerformUpdateMission( CPlazaViewNews * pViewNews, CPlazaViewNotice * pViewNotice );

	//�����¼�
protected:
	//��ֵ��Ϣ
	bool OnSocketSubQueryRecharge(VOID * pData, WORD wDataSize);
	//��Ϣ����
	bool OnSocketSubQueryNotice(VOID * pData, WORD wDataSize);
	
	//��̬����
public:
	//��ȡ����
	static CMissionUpdate * GetInstance() { return m_pMissionUpdate; }

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif