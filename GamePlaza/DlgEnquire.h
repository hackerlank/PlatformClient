#ifndef DLG_ENQUIRE_HEAD_FILE
#define DLG_ENQUIRE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//�رմ���
class CDlgEnquire : public CFGuiDialog
{
	//�ؼ�����
protected:
	CButtonUI *						m_pbtCancel;						//ȡ������
	CButtonUI *						m_pbtClosePlaza;					//�رմ���
	CButtonUI *						m_pbtCloseServer;					//ȡ������
	CButtonUI *						m_pbtSwitchAccounts;				//�л��ʺ�

	//������Դ
protected:
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��
	CPlatformPublicize				m_PlatformPublicize;				//����ؼ�

	//��������
public:
	//���캯��
	CDlgEnquire();
	//��������
	virtual ~CDlgEnquire();

	//���غ���
protected:
	//���ú���
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

	//�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);

	//�滭�ӿ�
public:
	//�����滭
	virtual void OnBeginPaintWindow(HDC hDC);

	//��ť��Ϣ
protected:
	//�رմ���
	VOID OnBnClickedClosePlaza();
	//ȡ������
	VOID OnBnClickedCloseServer();
	//�л��ʺ�
	VOID OnBnClickedSwitchAccounts();

	//��������
protected:
	//�����ؼ�
	VOID RectifyControlUI();
};

//////////////////////////////////////////////////////////////////////////////////

#endif