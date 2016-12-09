#ifndef PLAZA_USERINFO_HEAD_FILE
#define PLAZA_USERINFO_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//�Ӵ���
#define PARENT_USERINFO_X			10							//�û���Ϣ
#define PARENT_USERINFO_Y			120							//�û���Ϣ

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CPlazaViewUser : public CFGuiWnd
{
	//��������
public:	
	CPngImage						m_ImageBack; //����ͼƬ

	//��������
public:
	//���캯��
	CPlazaViewUser();
	//��������
	virtual ~CPlazaViewUser();
	
	//��������
public:
	//��ӷ���
	CString AddComma( LONGLONG lScore , bool bPlus = false );

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

	//��Ϣ����
protected:	
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);	
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	//��Ϣ����
protected:
	//��������
	LRESULT OnMessageUpdateDate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


#endif