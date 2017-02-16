#include "Stdafx.h"
#include "Json/cJSON.h"
#include "HttpClient.h"

////////////////////////////////////////////////////////////////////////////////

//构造函数
CHttpClient::CHttpClient()
{
	m_bHttpComplete = FALSE;
	ZeroMemory( m_szHttpContext, sizeof(m_szHttpContext) );
}


//析构函数
CHttpClient::~CHttpClient()
{

}


//访问连接
VOID CHttpClient::Navigate(DWORD AddressStyle)
{
	//初始化
	m_bHttpComplete = FALSE;

	//获取对象
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
				
	//构造地址
	TCHAR szNavigation[256]=TEXT("");
	switch(AddressStyle)
	{
	case HTTP_ADRR_TOPNEWS:
		{
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/TopNews.aspx"), pGlobalWebLink->GetPlatformLink() );
			break;
		}
	case HTTP_ADRR_TOPRECHARGE:
		{
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/TopRecharge.aspx"), pGlobalWebLink->GetPlatformLink() );
			break;
		}
	default:
		{
			//安全退出
			return;
		}
	}
	
	// 网页数据
	try
	{
		CInternetSession cSession;   
		CHttpFile * pHttpFile = (CHttpFile*) cSession.OpenURL( szNavigation, 1, INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_RELOAD, NULL, 0);

		if (pHttpFile==NULL)	return;

		//读取状态
		DWORD dwStatusCode;
		pHttpFile->QueryInfoStatusCode(dwStatusCode); 
			
		int nLenght = 0;
		ZeroMemory( m_szHttpContext, sizeof(m_szHttpContext) );
		if(dwStatusCode == HTTP_STATUS_OK)  
		{  
			//读取网页数据
			std::string strHtml;
			char szRead[1024] = {0};
			while( pHttpFile->Read((void*)szRead,1000) > 0)
				strHtml += szRead;
	
			//计算长度
			nLenght = MultiByteToWideChar (CP_UTF8, 0, strHtml.c_str(), -1, NULL, 0);
			if(strHtml.size() < HTTP_FILE_SIZE) {
				MultiByteToWideChar (CP_UTF8, 0, strHtml.c_str(), -1, m_szwHttpContext, nLenght);
			}

			//计算长度
			nLenght = WideCharToMultiByte (CP_UTF8, 0, m_szwHttpContext, -1, NULL, 0, NULL, FALSE);
			if(nLenght < HTTP_FILE_SIZE) {
				WideCharToMultiByte(CP_UTF8, 0, m_szwHttpContext, -1, m_szHttpContext, nLenght, NULL, FALSE);
			}
		}

		// 结束
		pHttpFile->Close();
		cSession.Close(); 
	}
	catch(CInternetException)
	{
		return;
	}
	catch(...)
	{
		return;
	}

	//完整化退出
	m_bHttpComplete = TRUE;
}


//新闻数据
WORD CHttpClient::TopNews(CTopNewsArray & _cTopNewsArray)
{
	/*　cJSON *json_root, *json_item, *json_title, *json_weblink;
	 
	// 解析数据包  
    json_root = cJSON_Parse( m_szHttpContext ); 
	if (json_root==NULL) {
		return NULL;
	}

	tagTopNews pTemp;
	int nSize = cJSON_GetArraySize(json_root);
	for(int i=0; i<nSize; i++)
	{
		json_item = cJSON_GetArrayItem( json_root, i);
		if (json_item==NULL) {
			return NULL;
		}

		//名称
		json_title = cJSON_GetObjectItem( json_item, "Title");
		if (json_title==NULL) {
			continue;
		}
		
		if (json_title->type==cJSON_String) {
			pTemp.szTitle = json_title->valuestring;
		}
		
		//链接
		json_weblink = cJSON_GetObjectItem( json_item, "WebLink");
		if (json_weblink==NULL) {
			continue;
		}

		if (json_weblink->type==cJSON_String) {
			pTemp.szWeblink = json_title->valuestring;
		}

		_cTopNewsArray.Add(pTemp);
	}

	return _cTopNewsArray.GetCount();　*/

	return 0;
}


//充值数据
WORD CHttpClient::TopRecharge(CTopRechargeArray & _cTopRechargeArray)
{
	/*  cJSON *json_root, *json_item, *json_name, *json_score;
	 
	// 解析数据包  
    json_root = cJSON_Parse( m_szHttpContext ); 
	if (json_root==NULL) {
		return NULL;
	}

	tagTopRecharge pTemp;
	int nSize = cJSON_GetArraySize(json_root);
	for(int i=0; i<nSize; i++)
	{
		json_item = cJSON_GetArrayItem( json_root, i);
		if (json_item==NULL) {
			return NULL;
		}

		//名称
		json_name = cJSON_GetObjectItem( json_item, "Name");
		if (json_name==NULL) {
			continue;
		}
		
		if (json_name->type==cJSON_String) {
			pTemp.szName = json_name->valuestring;
		}
		
		//链接
		json_score = cJSON_GetObjectItem( json_item, "Score");
		if (json_score==NULL) {
			continue;
		}

		if (json_score->type==cJSON_String) {
			pTemp.szScore = json_score->valuestring;
		}

		_cTopRechargeArray.Add(pTemp);
	}

	return _cTopRechargeArray.GetCount(); */
	return 0;
}


//数据转化
CString CHttpClient::GetString(std::string szText)
{
	WCHAR szwText[HTTP_NAME_SIZE] = {0};

	//计算长度
	int nLenght = MultiByteToWideChar (CP_UTF8, 0, szText.c_str(), -1, NULL, 0);
	if(szText.size() < HTTP_FILE_SIZE) {
		MultiByteToWideChar (CP_UTF8, 0, szText.c_str(), -1, szwText, nLenght);
	}

	return szwText;
}
