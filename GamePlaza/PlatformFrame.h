#ifndef PLATFORM_FRAME_HEAD_FILE
#define PLATFORM_FRAME_HEAD_FILE

#include "ServerListData.h"
#include "MissionList.h"
#include "MissionLogon.h"
#include "PlazaViewGame.h"
#include "PlazaViewUser.h"
#include "PlazaViewServer.h"
#include "PlazaViewEntry.h"
#include "PlazaViewNews.h"
#include "PlatformPublicize.h"
#include "PlazaViewNotice.h"

//////////////////////////////////////////////////////////////////////////////////

//�ȼ�����
#define IDI_HOT_KEY_BOSS			0x0100								//�ϰ��ȼ�
#define IDI_HOT_KEY_WHISPER			0x0200								//˽���ȼ�

//�����
#define IDM_CLOSE					105                                 //�رմ���
#define IDM_RESTORE                 106                                 //��ԭ��ʾ
#define IDM_MINIMIZE                107                                 //��С����ʾ
#define IDM_MAXIMIZE                108                                 //�����ʾ

//�����
#define IDM_SHOW_LOGON				200									//��ʾ��¼

//////////////////////////////////////////////////////////////////////////////////


//ƽ̨���
class CPlatformFrame : public CFGuiFrameWnd, public ICustomFaceEvent,public IServerListDataSink
{
	friend class CPlazaViewGame;
	friend class CPlazaViewUser;
	friend class CPlazaViewServer;
	friend class CPlazaViewEntry;
	
//�������
protected:
	bool							m_bServerStatu;						//����״̬
	bool							m_bRectify;							//������־
	CRect							m_rcNormalSize;						//����λ��

	//�������
protected:
	CMissionLogon					m_MissionLogon;						//��¼����
	CMissionList					m_MissionList;						//�б�����
	CMissionManager					m_MissionManager;					//�������

	//�������
protected:
	CServerListData					m_ServerListData;					//��Ϸ����

	//Ӧ�����
protected:
	CPlazaViewGame					m_PlazaViewGame;					//�㳡��Ϸ
	CPlazaViewUser					m_PlazaViewUser;					//�û���Ϣ
	CPlazaViewServer				m_PlazaViewServer;					//��Ϸ����
	CPlazaViewEntry					m_PlazaViewEntry;					//��¼��Ϸ
	CPlazaViewNews					m_PlazaViewNews;					//������Ϣ
	CPlazaViewNotice				m_PlazaViewNotice;					//������Ϣ

	//��Ƶ����
protected:
	WORD							m_wAVServerPort;					//��Ƶ�˿�
	DWORD							m_dwAVServerAddr;					//��Ƶ��ַ

protected:
	CPlatformPublicize				m_PlatformNews;						//����ؼ�
	CPlatformPublicize				m_PlatformPublicize;				//����ؼ�.

	//��̬����
protected:
	static CPlatformFrame *			m_pPlatformFrame;					//���ָ��

public:
	//���캯��
	CPlatformFrame();
	//��������
	virtual ~CPlatformFrame();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���غ���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("PlatformFrame.xml"); }

	//��Ƶ����
public:
	void SetVideoOption(WORD wAVServerPort, DWORD dwAVServerAddr);

	//ͷ��ӿ�
protected:
	//����ʧ��
	virtual VOID OnEventSystemFace(DWORD dwUserID, WORD wFaceID);
	//ͷ������
	virtual VOID OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo);

	//״̬֪ͨ
public:
	//��ȡ֪ͨ
	virtual VOID OnGameItemFinish();
	//���֪ͨ
	virtual VOID OnGameMatchFinish();
	//��ȡ֪ͨ
	virtual VOID OnGameKindFinish(WORD wKindID);

	//����֪ͨ
public:
	//����֪ͨ
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//����֪ͨ
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//ɾ��֪ͨ
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//��Ϣ����
protected:
	//�ر���Ϣ
	VOID OnClose();
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//λ����Ϣ
	VOID OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//�ı���Ϣ
	VOID OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	//�Զ���Ϣ
protected:
	//�ȼ���Ϣ
	LRESULT OnMessageHotKey(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnMessageTaskBar(WPARAM wParam, LPARAM lParam);
	//ͼ����Ϣ
	LRESULT OnMessageTrayIcon(WPARAM wParam, LPARAM lParam);
	//�¼���Ϣ
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);
	//�¼���Ϣ
	LRESULT OnMessageInstallCancelEvent(WPARAM wParam, LPARAM lParam);
	//�¼���Ϣ
	LRESULT OnMessageInsurePlazaEvent(WPARAM wParam, LPARAM lParam);

	//��Ϸ��Ϣ
protected:
	//������Ϸ
	LRESULT OnMessageEnterGame(WPARAM wParam, LPARAM lParam);
	//�˳���Ϸ
	LRESULT OnMessageQuitGame(WPARAM wParam, LPARAM lParam);
	//���뷿��
	LRESULT OnMessageEnterServer(WPARAM wParam, LPARAM lParam);
	//��¼����
	LRESULT OnMessageLogonServer(WPARAM wParam, LPARAM lParam);
	//��¼�ɹ�
	LRESULT OnMessageLogonSuccess(WPARAM wParam, LPARAM lParam);
	//�˳�����
	LRESULT OnMessageQuitServer(WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//�����ж�
	bool IsEnteredServerItem();
	//�û�����
	VOID UpdateUserInfo();
	//��ȡʵ��
	static CPlatformFrame * GetInstance() { return m_pPlatformFrame; }

	DECLARE_MESSAGE_MAP()
};


#endif