#ifndef PLAZA_VIEW_ENTRY_HEAD_FILE
#define PLAZA_VIEW_ENTRY_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

#include "ProcessManager.h"
#include "DlgStatus.h"
#include "GlobalUnits.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////

//����״̬
enum enServiceStatus
{
	ServiceStatus_Unknow,			//δ֪״̬
	ServiceStatus_Entering,			//����״̬
	ServiceStatus_Validate,			//��֤״̬
	ServiceStatus_RecvInfo,			//��ȡ״̬
	ServiceStatus_ServiceIng,		//����״̬
	ServiceStatus_NetworkDown,		//�ж�״̬
};

//////////////////////////////////////////////////////////////////////////////////

//��������
struct tagSearchTable
{
	bool							bOneNull;							//һ����λ
	bool							bTwoNull;							//������λ
	bool							bAllNull;							//ȫ��λ��
	bool							bNotFull;							//��ȫ��λ
	bool							bFilterPass;						//��������
	WORD							wStartTableID;						//��ʼ����
	WORD							wResultTableID;						//�������
	WORD							wResultChairID;						//�������
};

//////////////////////////////////////////////////////////////////////////////////

//�Ӵ���
#define PARENT_VIEWENTRY_X			256		//�û���Ϣ
#define PARENT_VIEWENTRY_Y			154		//�û���Ϣ

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CPlazaViewEntry : public CFGuiWnd, public IServerViewItem, public ITCPSocketSink, public ITableViewFrameSink, public IUserManagerSink, public IProcessManagerSink,public IStatusViewSink
{
	//��������
public:	
	CPngImage						m_ImageBack;						//����ͼƬ

	//��������
protected:
	WORD							m_wReqTableID;						//��������
	WORD							m_wReqChairID;						//����λ��
	WORD							m_wFindTableID;						//��������
	WORD							m_wResultTableID;					//�������
	WORD							m_wInvitedTableID;					//��������
	bool							m_bUserInRoom;						//�Ƿ��ڷ���

	//��������
protected:
	WORD							m_wServerType;						//��������
	DWORD							m_dwServerRule;						//�������
	enServiceStatus					m_ServiceStatus;					//����״̬
	WORD							m_wAVServerPort;					//��Ƶ�˿�
	DWORD							m_dwAVServerAddr;					//��Ƶ��ַ
	TCHAR							m_szServerPassword[LEN_PASSWORD];	//��������

	//�û�����
protected:
	DWORD							m_dwUserRight;						//�û�Ȩ��
	DWORD							m_dwMasterRight;					//����Ȩ��
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��

	//���ݴ洢
public:
	//��Ϸ����
	WORD							m_wChairCount;						//��������
	tagGameKind						m_GameKind;							//������Ϣ
	tagGameServer					m_GameServer;						//������Ϣ

	//�������
protected:
	CProcessManager					m_ProcessManager;					//���̹���
	CTCPSocketHelper				m_TCPSocketModule;					//��������
	CGameLevelParserHelper			m_GameLevelParserModule;			//��Ϸ�ȼ�
	CPlazaUserManagerHelper			m_PlazaUserManagerModule;			//�û�����

	//���ò���
protected:
	CParameterGame *				m_pParameterGame;					//��Ϸ����
	CParameterServer *				m_pParameterServer;					//��������
	
	//������Ϣ
protected:
	BYTE							m_cbColumnCount;					//�б���Ŀ
	BYTE							m_cbPropertyCount;					//������Ŀ
	tagColumnItem					m_ColumnItem[MAX_COLUMN];			//�б�����
	tagPropertyInfo					m_PropertyInfo[MAX_PROPERTY];		//��������

	//�������
public:
	CDlgStatus						m_DlgStatus;						//״̬����	

	//�ӿڱ���
public:
	ITableViewFrame *				m_pITableViewFrame;					//���ӿ��

	//��������
public:
	//���캯��
	CPlazaViewEntry();
	//��������
	virtual ~CPlazaViewEntry();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release();
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���غ���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT(""); }	
	//�����滭
	virtual void OnBeginPaintWindow(HDC hDC);

	//��Ϸ��Ϣ
public:
	//������ͼ
	bool CreateViewFrame();
	//������ͼ
	VOID ResetViewFrame(bool isGameing = true);
	//��ʼ��Ϸ
	VOID InitGameServer( CGameServerItem * pServerItem );
	//������Ϸ
	VOID PerformJoinGame();
	//�����ر�
	bool PlazaClosing();

	//���ӽӿ�
public:
	//���˫��
	virtual VOID OnDButtonHitTable(WORD wTableID);
	//�������
	virtual VOID OnLButtonHitTable(WORD wTableID, WORD wChairID);
	//�Ҽ�����
	virtual VOID OnRButtonHitTable(WORD wTableID, WORD wChairID);
	//���Ӱ�ť
	virtual VOID OnHitTableButton(BYTE cbButton);

	//����ӿ�
protected:
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//���̽ӿ�
public:
	//���̴���
	virtual bool OnGameProcessError();
	//���̹ر�
	virtual bool OnGameProcessClose(DWORD dwExitCode);
	//��������
	virtual bool OnGameProcessCreate(bool bAlreadyExist);
	//��������
	virtual bool OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//��������
public:
	//���͹���
	bool SendUserRulePacket();
	//���͵�¼
	bool SendLogonPacket(LPCTSTR pszServerPasswd);	
	//�����Թ�
	bool SendLookonPacket(WORD wTableID, WORD wChairID);
	//��������
	bool SendSitDownPacket(WORD wTableID, WORD wChairID, LPCTSTR lpszPassword=NULL);
	//��������
	bool SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave);
	//���ͱ���
	bool SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex);
	//��������
	bool SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor);

	//�û��ӿ�
public:
	//�û�����
	virtual VOID OnUserItemAcitve(IClientUserItem * pIClientUserItem);
	//�û�ɾ��
	virtual VOID OnUserItemDelete(IClientUserItem * pIClientUserItem);
	//�û�����
	virtual VOID OnUserFaceUpdate(IClientUserItem * pIClientUserItem);
	//�û�����
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore);
	//�û�����
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus);
	//�û�����
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserAttrib & UserAttrib);

	//��������
public:
	//ִ������
	bool PerformStandUpAction(WORD wTableID, WORD wChairID);
	//ִ������
	bool PerformSitDownAction(WORD wTableID, WORD wChairID, bool bEfficacyPass);

	//�ڲ�����
protected:
	//����Ч��
	bool EfficacyTableRule(WORD wTableID, WORD wChairID, bool bReqLookon, CString & strDescribe);

	//�����¼�
protected:
	//�û�����
	bool OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��¼����
	bool OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//���ô���
	bool OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//״̬����
	bool OnSocketMainStatus(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//���д���
	bool OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������
	bool OnSocketMainTask(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//�һ�����
	bool OnSocketMainExchange(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������
	bool OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//ϵͳ����
	bool OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��������
	bool OnSocketMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����¼�
protected:
	//�û�����
	bool OnSocketSubUserEnter(VOID * pData, WORD wDataSize);
	//�û�����
	bool OnSocketSubUserScore(VOID * pData, WORD wDataSize);
	//�û�״̬
	bool OnSocketSubUserStatus(VOID * pData, WORD wDataSize);
	//�û���Ϸ����
	bool OnSocketSubUserItemGameData(VOID * pData, WORD wDataSize);
	//��¼���
	bool OnSocketSubLogonFinish(VOID * pData, WORD wDataSize);
	//��¼�ɹ�
	bool OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnSocketSubLogonFailure(VOID * pData, WORD wDataSize);
	//������ʾ
	bool OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnSocketSubRequestFailure(VOID * pData, WORD wDataSize);
	//�û�����
	bool OnSocketSubUserChat(VOID * pData, WORD wDataSize);
	//�û�����
	bool OnSocketSubExpression(VOID * pData, WORD wDataSize);
	//�û�˽��
	bool OnSocketSubWisperUserChat(VOID * pData, WORD wDataSize);
	//˽�ı���
	bool OnSocketSubWisperExpression(VOID * pData, WORD wDataSize);
	//ϵͳ��Ϣ
	bool OnSocketSubSystemMessage(VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnSocketSubActionMessage(VOID * pData, WORD wDataSize);
	//�������
	bool OnSocketSubUserInvite(VOID * pData, WORD wDataSize);
	//���߳ɹ�
	bool OnSocketSubPropertySuccess(VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnSocketSubPropertyFailure(VOID * pData, WORD wDataSize);
	//����ЧӦ
	bool OnSocketSubPropertyEffect(VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnSocketPropertyMessage(VOID * pData, WORD wDataSize);
	//��������
	bool OnSocketPropertyTrumpet(VOID * pData, WORD wDataSize);
	//�ȴ�����
	bool OnSocketSubWaitDistribute(VOID *pData, WORD wDataSize);

	//��������
protected:
	//����״̬
	VOID SetServiceStatus(enServiceStatus ServiceStatus);
	//��ѯ��Ϸ�ͻ���
	bool IsGameClientReady();
	//��ԴĿ¼
	VOID GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount);
	//��������
	VOID SearchGameTable(tagSearchTable & SearchTable);
	//�Լ�״̬
	bool IsPlayingMySelf();
	//��Ϸ��Ϣ

	//��Ϸ��Ϣ
protected:
	//ɾ����Ϸ
	LRESULT OnDeleteServerItem(WPARAM wParam, LPARAM lParam);

	//�¼��ӿ�
protected:
	//ȡ������
	virtual VOID OnStatusCancel();

	//��Ϣ����
protected:	
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);	
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	DECLARE_MESSAGE_MAP()
};

#endif