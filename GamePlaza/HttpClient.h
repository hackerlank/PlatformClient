#ifndef HTTP_CLIENT_HEAD_FILE
#define HTTP_CLIENT_HEAD_FILE

////////////////////////////////////////////////////////////////////////////////

#define  HTTP_ADRR_TOPNEWS					(0x00000001L)		//���ŵ�ַ
#define  HTTP_ADRR_TOPRECHARGE				(0x00000002L)		//��ֵ��ַ

////////////////////////////////////////////////////////////////////////////////

#define  HTTP_NAME_SIZE						(256)				//�ļ���С
#define  HTTP_FILE_SIZE						(8192)				//�ļ���С

////////////////////////////////////////////////////////////////////////////////

// ��������
struct tagTopNews
{
	CString szcTitle;
	CString szcWeblink;
};

typedef CWHArray<tagTopNews> CTopNewsArray;

////////////////////////////////////////////////////////////////////////////////

// ��ֵ����
struct tagTopRecharge
{
	std::string szName;
	std::string szScore;
};

typedef CWHArray<tagTopRecharge> CTopRechargeArray;

////////////////////////////////////////////////////////////////////////////////


// ��ȡ��ҳ
class CHttpClient
{
	// �����
private:
	BOOL							m_bHttpComplete;						//Http���
	WCHAR							m_szwHttpContext[HTTP_FILE_SIZE];		//Http�ı�
	CHAR							m_szHttpContext[HTTP_FILE_SIZE];		//Http�ı�

//��ʼ����
public:
	//CTopNewsArray					m_cTopNewsArray;
	//CTopRechargeArray				m_cTopRechargeArray;

	//��ʼ����
public:
	//���캯��
	CHttpClient();
	//��������
	~CHttpClient();

	//���ܺ���
public:
	//��������
	VOID Navigate(DWORD AddressStyle);		
	//�������
	BOOL IsComplete() {	return m_bHttpComplete; }	
	//��������
	WORD TopNews(CTopNewsArray & cTopNewsArray);
	//��ֵ����
	WORD TopRecharge(CTopRechargeArray & m_cTopRechargeArray);
	//����ת��
	CString GetString(std::string szText);
};


#endif