#ifndef PLAZA_VIEW_GAME_HEAD_FILE
#define PLAZA_VIEW_GAME_HEAD_FILE

#include "ServerListData.h"
#include "DlgUpdateStatus.h"
#include "PlazaViewServer.h"

//////////////////////////////////////////////////////////////////////////////////

//�Ӵ���
#define PARENT_VIEWGAME_X			256		//��Ϸ����
#define PARENT_VIEWGAME_Y			154		//��Ϸ����

//////////////////////////////////////////////////////////////////////////////////

//��������
#define VIEW_MODE_NONE				0									//��Чģʽ
#define VIEW_MODE_KIND				1									//����ģʽ
#define VIEW_MODE_MATCH				2									//����ģʽ
#define VIEW_MODE_SERVER			3									//����ģʽ

//��ť��ʶ
#define BT_VIEW_RULE				1									//�鿴����
#define BT_ENTER_KIND				2									//��������
#define BT_ENTER_SERVER				3									//���뷿��

//��������
#define VAR_TYPE_PAGECOUNT			0									//��������
#define VAR_TYPE_PAGECURRENT		1									//��������
#define VAR_TYPE_PAGECONTAINER		2									//��������

//��������
#define WM_CLICKED_LEFT				0									//��һҳ
#define WM_CLICKED_RIGHT			1									//��һҳ

//////////////////////////////////////////////////////////////////////////////////

//��������
class CGameViewSubItem;
class CGameViewKindItem;
class CGameViewServerItem;
class CGameViewMatchItem;
class CPlazaViewServer;

//////////////////////////////////////////////////////////////////////////////////

//���Ͷ���
struct tagGameKindInfo
{
	WORD							wSortID;							//�����ʶ
	CGameKindItem *					pGameKindItem;						//���Ͷ���
};

//////////////////////////////////////////////////////////////////////////////////

//��������
struct tagGameMatchInfo
{
	WORD							wSortID;							//�����ʶ
	CGameServerItem *				pGameServerItem;					//�������
	CGameViewMatchItem *			pGameViewMatchItem;					//��������
};

//////////////////////////////////////////////////////////////////////////////////

//���䶨��
struct tagGameServerInfo
{
	WORD							wSortID;							//�����ʶ
	WORD							wServerID;							//�����ʶ
	CGameServerItem *				pGameServerItem;					//�������
};

//////////////////////////////////////////////////////////////////////////////////

//��������
struct tagViewLayoutAttribute
{
	WORD							wItemXCount;						//������Ŀ
	WORD							wItemYCount;						//������Ŀ
	CSize							SizeItem;							//����ߴ�
	CRect							rcItemMargin;						//����߾�
};

//////////////////////////////////////////////////////////////////////////////////

//��ͼ����
struct tagViewPageControl
{
	WORD							wPageCount;							//ҳ������
	WORD							wPageCurrent;						//��ǰҳ��
	CContainerUI *					pPageContainer;						//ҳ������	
};

//////////////////////////////////////////////////////////////////////////////////

//��ͼ����
class CGameButtonImage
{
public:
	CGameButtonImage(WORD wID, const TCHAR * szName, const TCHAR* szImage);
	~CGameButtonImage();

public:
	WORD							wGameID;							//��Ϸ��ʶ
	DWORD							dwVersion;							//�汾��ʶ
	const TCHAR*					wButtonName;						//��Ϸ����
	const TCHAR*					wButtonImage;						//��ϷͼƬ	
};

//////////////////////////////////////////////////////////////////////////////////

//ҳ��ģʽ
enum emPageType
{
	pmNone=0,
	pmGameKind,
	pmGameMatch,
	pmGameServer
};

//////////////////////////////////////////////////////////////////////////////////

//���鶨��
typedef CWHArray<tagGameKindInfo *>	CGameKindInfoArray;					//��������
typedef CWHArray<tagGameMatchInfo *> CGameMatchInfoArray;				//��������
typedef CWHArray<tagGameServerInfo *> CGameServerInfoArray;				//��������

//////////////////////////////////////////////////////////////////////////////////

//����˵��
typedef CMap<WORD, WORD, CGameButtonImage *, CGameButtonImage * &> CButtonImageMap;

//////////////////////////////////////////////////////////////////////////////////
//�ӿڶ���
interface IViewPageSubItemSink
{
public:
	//���͵��
	virtual void OnViewPageKindItemClicked(tagGameKindInfo * pGameKindInfo)=NULL;	
	//������
	virtual void OnViewPageServerItemClicked(tagGameServerInfo * pGameServerInfo)=NULL;
	//�������
	virtual void OnViewPageMatchItemClicked(tagGameMatchInfo * pGameMatchInfo,WORD wCommandID)=NULL;
};


//��Ϸ��ͼ
class CPlazaViewGame : public CFGuiWnd, public IViewPageSubItemSink, public IUpdateStatusSink
{
	//��������
public:	
	CPngImage				m_ImageBack;						//����ͼƬ

	//��ť����
public:
	//��ťͼ��
	CButtonImageMap			m_ButtonImageMap;

	//���ݴ洢
public:
	//��Ϸ����
	CGameServerArray		m_GameServerArray;

	//��������
public:
	//���캯��
	CPlazaViewGame();
	//��������
	virtual ~CPlazaViewGame();

	//�����¼�
public:
	//�������
	virtual VOID OnEventDownLoadTaskFinish();
	//������
	virtual VOID OnEventUpdateCheckFinish(bool bNeedUpdate);	

	//����֪ͨ
public:
	// ������Ϸ
	VOID InsertGameItem( WORD wGameID, DWORD dwProcessVersion );
	// ���·���
	VOID InsertServerItem( CGameServerItem * pGameServerItem );
	// ����Ŀ¼
	VOID UpdateServerItem( WORD wKindID );

public:
	//���͵��
	virtual void OnViewPageKindItemClicked(tagGameKindInfo * pGameKindInfo);	
	//������
	virtual void OnViewPageServerItemClicked(tagGameServerInfo * pGameServerInfo);
	//�������
	virtual void OnViewPageMatchItemClicked(tagGameMatchInfo * pGameMatchInfo,WORD wCommandID);

	//���غ���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT(""); }	
	//��������
	virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue);
	//�����滭
	virtual void OnBeginPaintWindow(HDC hDC);

	//�¼�����
protected:
	//��ҳ���
	LRESULT OnClickedLeftPageView(WPARAM wParam, LPARAM lParam);
	//��ҳ���
	LRESULT OnClickedRightPageView(WPARAM wParam, LPARAM lParam);

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