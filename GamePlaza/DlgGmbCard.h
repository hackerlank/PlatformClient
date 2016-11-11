#pragma once
#include "stdafx.h"

// CDLGMBCard dialog

class CDlgGmbCard : public CSkinDialog
{
public:
	UINT								m_uMBCardID;						//��������
	CHAR								m_szRandLetter[3];
	BYTE								m_szRandNumber[3];
	BYTE								m_cbErrorCount;
	UINT								m_uCoorNumber[6][4];

	CSkinButton							m_btOK;
	CSkinButton							m_btCancel;

	//��������
public:
	CDlgGmbCard(CWnd* pParent = NULL); 
	virtual ~CDlgGmbCard();

	//��������
public:
	virtual void DoDataExchange(CDataExchange* pDX); 

	//���غ���
public:
	//������Ϣ
	virtual BOOL OnInitDialog();
	//��ť��Ϣ
	void OnOK();
	//�滭��Ϣ
	virtual VOID OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);

	//��������
public:
	void RandCoordinate();
	void SetMBCard(UINT uMBCardID);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CDlgGmbCard)
};
