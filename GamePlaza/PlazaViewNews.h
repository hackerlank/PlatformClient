#ifndef BUTTON_ITEM_HEAD_FILE
#define BUTTON_ITEM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//�Ӵ���
#define PARENT_VIEWNEWS_X			23				//������Ϣ
#define PARENT_VIEWNEWS_Y			433				//������Ϣ

//////////////////////////////////////////////////////////////////////////////////

const TCHAR kWebLinkClassName[] = _T("WebLinkUI");  
const TCHAR kWebLinkInterface[] = _T("WebLink");  

//////////////////////////////////////////////////////////////////////////////////

class CWebLinkUI : public CButtonUI 
{
	//��ʶ����
protected:
	bool				m_bHoverServer;				//������ʶ

	//���غ���
public:
	//���캯��
    CWebLinkUI();  
	//��������
	~CWebLinkUI();

	//�ӿں���
public:
	//�������
    LPCTSTR GetClass() const;  
    //�ӿں���
	LPVOID GetInterface(LPCTSTR pstrName);  

	//���غ���
public:
	//��ʼ����
	virtual void InitControlUI();
	//�¼�����
	void DoEvent(TEventUI& event);

	//�̳к���
public: 
    //�ع�����
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);  
	//���»���
    void PaintStatusImage(HDC hDC);
};


//////////////////////////////////////////////////////////////////////////////////

class CPlazaViewNews : public CFGuiWnd
{
	//��������
public:
	//���캯��
	CPlazaViewNews();
	//��������
	~CPlazaViewNews();
	
	//���غ���
public:	
	//�����ؼ�
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	//��ʼ����
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("PlazaViewNews.xml"); }	

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