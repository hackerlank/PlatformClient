#ifndef PLAZA_VIEW_SERVER_HEAD_FILE
#define PLAZA_VIEW_SERVER_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

#include "ComplexUI.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��ʶ
#define GID_BACCARAT				122			//�ټ���
#define GID_FIVECARD				25			//��ʽ5��
#define GID_LAND					200			//������
#define GID_OXBATTLE				104			//����ţţ
#define GID_OXNEW					27			//ţţ
#define GID_ZAJINHUA				6			//����
#define GID_DEZHOU					3			//�����˿�
#define GID_SPARROWXZ				302			//Ѫս�齫

//////////////////////////////////////////////////////////////////////////////////

#define PAGE_SERVER			6					//ҳ�淿��

//////////////////////////////////////////////////////////////////////////////////

//�Ӵ���
#define PARENT_VIEWSERVER_X			256			//�û���Ϣ
#define PARENT_VIEWSERVER_Y			154			//�û���Ϣ

//////////////////////////////////////////////////////////////////////////////////

//�����б�
class CGameServerItem;

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
typedef CWHArray<CGameServerItem*> CGameServerArray;				

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CPlazaViewServer : public CFGuiWnd
{	
	//��������
public:	
	CPngImage						m_ImageBack;						//����ͼƬ

	//���Ʋ���
public:
	WORD							m_GameServerPage;
	CComplexUI*						m_pButtonArray[PAGE_SERVER];
	CLabelUI*						m_pLabelArray[PAGE_SERVER];

	//���ݴ洢
public:
	WORD							m_wUserClickIndex;					//ѡ�񷿼�
	CGameServerArray				m_GameServerArray;					//�����б�

	//��������
public:
	//���캯��
	CPlazaViewServer();
	//��������
	virtual ~CPlazaViewServer();

	//����֪ͨ
public:
	// ���·���
	VOID RemoveServerItem();
	// ����Ŀ¼
	VOID InsertServerItem( CGameServerItem * pGameServerItem );
	// ���·���
	VOID UpdateServerItem( WORD wKindID );

	//���غ���
public:
	//�����滭
	virtual void OnBeginPaintWindow(HDC hDC);
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("Server.xml"); }
	//�����ؼ�
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	//��ʼ�ؼ�
	void CreateRealControl( CControlUI * pControl, int x, int y, int cx, int cy);
	
	//�������
public:
	CGameServerItem * GetUserChickServerItem();

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