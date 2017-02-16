#ifndef HTTP_CLIENT_HEAD_FILE
#define HTTP_CLIENT_HEAD_FILE

////////////////////////////////////////////////////////////////////////////////

#define  HTTP_ADRR_TOPNEWS					(0x00000001L)		//新闻地址
#define  HTTP_ADRR_TOPRECHARGE				(0x00000002L)		//充值地址

////////////////////////////////////////////////////////////////////////////////

#define  HTTP_NAME_SIZE						(256)				//文件大小
#define  HTTP_FILE_SIZE						(8192)				//文件大小

////////////////////////////////////////////////////////////////////////////////

// 顶部新闻
struct tagTopNews
{
	CString szcTitle;
	CString szcWeblink;
};

typedef CWHArray<tagTopNews> CTopNewsArray;

////////////////////////////////////////////////////////////////////////////////

// 充值数据
struct tagTopRecharge
{
	std::string szName;
	std::string szScore;
};

typedef CWHArray<tagTopRecharge> CTopRechargeArray;

////////////////////////////////////////////////////////////////////////////////


// 读取网页
class CHttpClient
{
	// 类变量
private:
	BOOL							m_bHttpComplete;						//Http完成
	WCHAR							m_szwHttpContext[HTTP_FILE_SIZE];		//Http文本
	CHAR							m_szHttpContext[HTTP_FILE_SIZE];		//Http文本

//初始函数
public:
	//CTopNewsArray					m_cTopNewsArray;
	//CTopRechargeArray				m_cTopRechargeArray;

	//初始函数
public:
	//构造函数
	CHttpClient();
	//析构函数
	~CHttpClient();

	//功能函数
public:
	//访问连接
	VOID Navigate(DWORD AddressStyle);		
	//访问完成
	BOOL IsComplete() {	return m_bHttpComplete; }	
	//新闻数据
	WORD TopNews(CTopNewsArray & cTopNewsArray);
	//充值数据
	WORD TopRecharge(CTopRechargeArray & m_cTopRechargeArray);
	//数据转化
	CString GetString(std::string szText);
};


#endif