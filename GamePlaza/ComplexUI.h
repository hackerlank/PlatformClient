#ifndef COMPLEX_UI_HEAD_FILE
#define COMPLEX_UI_HEAD_FILE

///////////////////////////////////////////////////////////////////////////////////////

//xml sample:<SkinPikerPictureItem name="" width="118" height="70" bkimage="UI\BKImage\1small.jpg" bkname="����" author="Redrain" />  
//�����ͽӿ�������CreateControl�����л��õ�  
const TCHAR kComplexClassName[] = _T("ComplexUI");  
const TCHAR kComplexInterface[] = _T("Complex");  

//////////////////////////////////////////////////////////////////////////////////////

class CComplexUI : public CButtonUI  
{
	//��ʾ����
protected:
	DWORD			m_dwMaxCount;					//�������
	DWORD			m_dwOnLineCount;				//��������
	LONGLONG		m_lEnterScore;					//��������
	LONGLONG		m_lCellScore;					//�����ע

	//��ʾ����
protected:
	CStdString		m_szEntryScore;				//����ɼ�
	CStdString		m_szBaseScore;				//��ע�ɼ�
	CStdString		m_szOnline;					//��������

	//���غ���
public:
	//���캯��
    CComplexUI();  
	//��������
	~CComplexUI();  

public:
	//�������
    LPCTSTR GetClass() const;  
    //�ӿں���
	LPVOID GetInterface(LPCTSTR pstrName);  
	
	//�̳к���
public: 
    //�ع�����
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);  
	//���»���
    void PaintStatusImage(HDC hDC);

	//���ܺ���
public: 
	//�������
	void SetMaxOnline(DWORD dwMaxCount);
	//�������
	void SetOnline(DWORD dwOnLineCount);
	//����ɼ�
	void SetEnterScore(LONGLONG lEnterScore);
	//��ע�ɼ�
	void SetCellScore(LONGLONG lCellScore);

	//��������
public: 
	//������Ϣ
	void UpdateInfo();
	// ���С����
	CString AddDecimal( LONGLONG lScore ,  bool bComma = true, bool bPlus = false);
	// ��Ӷ���
	CString AddComma( LONGLONG lScore , bool bPlus = false );
};  



#endif
