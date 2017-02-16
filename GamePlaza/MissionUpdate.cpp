#include "StdAfx.h"
#include "Resource.h"
#include "DlgGmbCard.h"
#include "GlobalUnits.h"
#include "MissionUpdate.h"
#include "PlatformEvent.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CMissionUpdate * CMissionUpdate::m_pMissionUpdate=NULL;					//����ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMissionUpdate, CDlgStatus)
END_MESSAGE_MAP()

//���캯��
CMissionUpdate::CMissionUpdate()
{
	//�������
	SetStatusViewSink(this);

	//���ö���
	ASSERT(m_pMissionUpdate==NULL);
	if (m_pMissionUpdate==NULL) m_pMissionUpdate=this;

	return;
}

//��������
CMissionUpdate::~CMissionUpdate()
{
	//�ͷŶ���
	ASSERT(m_pMissionUpdate==this);
	if (m_pMissionUpdate==this) m_pMissionUpdate=NULL;

	return;
}


//ȡ������
VOID CMissionUpdate::OnStatusCancel()
{
	//���ؽ���
	HideStatusWindow();

	//��ֹ����
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,true);
}


//�����¼�
bool CMissionUpdate::OnEventMissionLink(INT nErrorCode)
{
	//������
	if (nErrorCode!=0L) {

		//���ؽ���
		HideStatusWindow();

		return false;
	}
	else {
		//��ֵ��Ϣ
		SendRechargePacket();

		//������Ϣ
		SendNoticePacket();
	}

	return true;
}

//�ر��¼�
bool CMissionUpdate::OnEventMissionShut(BYTE cbShutReason)
{
	//���ؽ���
	HideStatusWindow();

	return true;
}

//��ȡ�¼�
bool CMissionUpdate::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_LOAD_TOPRECHARGE:	//��ֵ��Ϣ
			{
				CMD_GP_Recharge * pCMDRecharge=(CMD_GP_Recharge *)pData;

				return OnSocketSubQueryRecharge(pData,wDataSize);
			}
		case SUB_GP_LOAD_NOTICE:		//������Ϣ
			{
				return OnSocketSubQueryNotice(pData,wDataSize);
			}
		}
	}
	return false;
}

//ִ�е�¼
bool CMissionUpdate::PerformUpdateMission(CPlazaViewNews * pViewNews, CPlazaViewNotice * pViewNotice)
{
	//���մ���
	if (pViewNews==NULL) return false;
	if (pViewNotice==NULL) return false;
	m_pViewNews = pViewNews;
	m_pViewNotice = pViewNotice;

	//Ч��״̬
	ASSERT(GetActiveStatus()==false);
	if (GetActiveStatus()==true) return false;

	//��ȡ��ַ
	TCHAR szCustomServer[LEN_SERVER]=TEXT("������������");

	//���õ�ַ
	CMissionManager * pMissionManager=GetMissionManager();
	if (pMissionManager!=NULL) pMissionManager->SetCustomServer(szCustomServer);

	//��������
	if (pMissionManager->AvtiveMissionItem(this,false)==false)
	{
		//������ʾ
		return false;
	}

	return true;
}

//��ֵ��Ϣ
bool CMissionUpdate::SendRechargePacket()
{
	//��������
	WORD wPacketSize,wSubCmdID;
	BYTE cbBuffer[SOCKET_TCP_PACKET]={0};

	//��������
	wSubCmdID = SUB_GP_QUERY_RECHARGE_DATA;
	wPacketSize = sizeof(CMD_GP_Recharge);

	//��������
	ASSERT(GetMissionManager()!=NULL);
	GetMissionManager()->SendData(MDM_GP_USER_SERVICE,wSubCmdID,cbBuffer,wPacketSize);
	return true;
}

//������Ϣ
bool CMissionUpdate::SendNoticePacket()
{
	//��������
	WORD wPacketSize,wSubCmdID;
	BYTE cbBuffer[SOCKET_TCP_PACKET]={0};

	//��������
	wSubCmdID= SUB_GP_QUERY_NOTICE_DATA;
	wPacketSize = sizeof(CMD_GP_Notices);

	//��������
	ASSERT(GetMissionManager()!=NULL);
	GetMissionManager()->SendData(MDM_GP_USER_SERVICE,wSubCmdID,cbBuffer,wPacketSize);
	return true;
}


//////////////////////////////////////////////////////////////////////////////////
//��ֵ��Ϣ
bool CMissionUpdate::OnSocketSubQueryRecharge(VOID * pData, WORD wDataSize)
{

	//��������
	CMD_GP_Recharge * pCMDRecharge=(CMD_GP_Recharge *)pData;
	WORD wMinSize = __min(pCMDRecharge->wRechargeCount, VIEW_RECHARGE_UPDATE);

	//���ñ���
	tagRechargeParameter RechargeParameter[VIEW_RECHARGE_UPDATE+1];
	ZeroMemory(RechargeParameter, sizeof(RechargeParameter));

	//��ȡ����
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pCMDRecharge+1, wDataSize-sizeof(CMD_GP_Recharge));

	//��չ��Ϣ
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) 
			break;

		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_RECHARGE_DATA:		//
			CopyMemory(RechargeParameter, pDataBuffer, sizeof(tagRechargeParameter)*wMinSize );
			break;
		}
	}

	m_pViewNotice->OnHttpNavigate(RechargeParameter, pCMDRecharge->wRechargeCount);
	return true;
}

//��Ϣ����
bool CMissionUpdate::OnSocketSubQueryNotice(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GP_Notices * pCMDNotices=(CMD_GP_Notices *)pData;
	WORD wMinSize = __min(pCMDNotices->wNoticesCount, VIEW_NOTICE_UPDATE);

	//���ñ���
	tagNoticesParameter NoticesParameter[VIEW_NOTICE_UPDATE+1];
	ZeroMemory(NoticesParameter, sizeof(NoticesParameter));

	//��ȡ����
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pCMDNotices+1, wDataSize-sizeof(CMD_GP_Recharge));

	//��չ��Ϣ
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) 
			break;

		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_NOTICE_DATA:		//
			CopyMemory(NoticesParameter, pDataBuffer, sizeof(tagNoticesParameter)*wMinSize );
			break;
		}
	}

	m_pViewNews->OnHttpNavigate(NoticesParameter, pCMDNotices->wNoticesCount);
	return true;
}