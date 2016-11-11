#pragma once

#include "GlobalUnits.h"
#include "ServerListData.h"

#include "PlatformEvent.h"
#include "PlatformFrame.h"

#include "Other\IDispimp.h"
#include "Other\Custsite.h"

//Ӧ�ó���
class CGamePlazaApp : public CWinApp
{
	//��������
public:
	CImpIDispatch *					m_pIDispatch;						//�Զ��ӿ�
	CCustomOccManager *				m_pCustomOccManager;				//��������

	//ƽ̨����
protected:
	CGlobalUnits					m_GlobalUnits;						//ȫ�ֵ�Ԫ
	CGlobalWebLink					m_GlobalWebLink;					//������Ϣ
	CGlobalUserInfo					m_GlobalUserInfo;					//�û���Ϣ	

	//ȫ�����
protected:
	CPlatformEvent					m_PlatformEvent;					//ƽ̨�¼�
	CParameterGlobal				m_ParameterGlobal;					//ȫ������
	CUserItemElement				m_UserItemElement;					//�û�Ԫ��
	CUserInformation				m_UserInformation;					//�û���Ϣ
	CCustomFaceManager				m_CustomFaceManager;				//ͷ�����
	CExpressionManager				m_ExpressionManager;				//�������
	CGamePropertyManager			m_GamePropertyManager;				//���߹���
	CD3DSound						m_D3DSound;							//��������

	//��Դ���
public:
	CSkinRenderManager				m_SkinRenderManager;				//��Ⱦ����
	CSkinResourceManager			m_SkinResourceManager;				//��Դ����

	//�������
public:
	CFaceItemControlHelper			m_FaceItemControlModule;			//ͷ�����
	CUserOrderParserHelper			m_UserOrderParserModule;			//�ȼ����
	CPlatformResourceHelper			m_PlatformResourceModule;			//��Դ���

public:
	//���캯��
	CGamePlazaApp();

	//���غ���
public:
	//�˳�����
	virtual INT ExitInstance();

	//���뺯��
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


extern CGamePlazaApp theApp;