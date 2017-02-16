#ifndef QUERY_INFO_SERVICE_HEAD_FILE
#define QUERY_INFO_SERVICE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////
//��������
class CQueryInfoService : public CMissionItem
{
protected:
	bool							m_bQueryUserScore;					//��ѯ����

	//�������
protected:
	CMissionManager					m_MissionManager;					//�������

	//��������
public:
	//���캯��
	CQueryInfoService();
	//��������
	virtual ~CQueryInfoService();

	//���غ���
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

protected:
	//����ʼ
	virtual void OnMissionStart();
	//�������
	virtual void OnMissionConclude();

	//����ӿ�
public:
	//��ѯ���
	void QueryUserScore();
};

//////////////////////////////////////////////////////////////////////////////////

#endif