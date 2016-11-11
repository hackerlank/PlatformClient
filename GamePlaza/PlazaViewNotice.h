#ifndef PLAZA_VIEW_NOTICE_HEAD_FILE
#define PLAZA_VIEW_NOTICE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//�Ӵ���
#define PARENT_VIEWNOTICE_X			327				//����λ��
#define PARENT_VIEWNOTICE_Y			122				//����λ��

//////////////////////////////////////////////////////////////////////////////////

//�������
typedef CWHArray<CLabelUI *> CNoticeTextArray;

//////////////////////////////////////////////////////////////////////////////////

class CPlazaViewNotice : public CFGuiWnd
{
	//�ƶ�����
protected:
	int						m_nFixedWidth;		//��ǩ���
	int						m_nFixedHeight;		//��ǩ�߶�

	int						m_nCurrentX;		//ͨ�ø߶�
	int						m_nCurrentY;		//ͨ�ÿ��
	int						m_nCurrentW;		//���п��
	
	int						m_nMaxCount;		//��󳤶�
	int						m_nMainWidth;		//�������
	int						m_nSumWidth;		//���п��
		
	int						m_nMoveUint;		//λ�Ƴ���

	//�ƶ�����
protected:	
	CNoticeTextArray		m_wndNoticeArray;	//ͨ�����

	//��������
public:
	//���캯��
	CPlazaViewNotice();
	//��������
	~CPlazaViewNotice();
	
	//���ܺ���
protected:
	//��ӹ���
	void AddNotice(CContainerUI * pParent, TCHAR * szNotice);

	//���غ���
public:	
	//��ʼ����
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("PlazaViewNotice.xml"); }	

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