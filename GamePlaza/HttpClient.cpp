#include "Stdafx.h"
#include "Json/cJSON.h"
#include "HttpClient.h"

////////////////////////////////////////////////////////////////////////////////

//���캯��
CHttpClient::CHttpClient()
{
	m_bHttpComplete = FALSE;
	ZeroMemory( m_szHttpContext, sizeof(m_szHttpContext) );
}


//��������
CHttpClient::~CHttpClient()
{

}


//��������
VOID CHttpClient::Navigate(DWORD AddressStyle)
{
	//��ʼ��
	m_bHttpComplete = FALSE;

	//��ȡ����
	ASSERT(CGlobalWebLink::GetInstance()!=NULL);
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
				
	//�����ַ
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
			//��ȫ�˳�
			return;
		}
	}
	
	// ��ҳ����
	try
	{
		CInternetSession cSession;   
		CHttpFile * pHttpFile = (CHttpFile*) cSession.OpenURL( szNavigation, 1, INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_RELOAD, NULL, 0);

		if (pHttpFile==NULL)	return;

		//��ȡ״̬
		DWORD dwStatusCode;
		pHttpFile->QueryInfoStatusCode(dwStatusCode); 
			
		int nLenght = 0;
		ZeroMemory( m_szHttpContext, sizeof(m_szHttpContext) );
		if(dwStatusCode == HTTP_STATUS_OK)  
		{  
			//��ȡ��ҳ����
			std::string strHtml;
			char szRead[1024] = {0};
			while( pHttpFile->Read((void*)szRead,1000) > 0)
				strHtml += szRead;
	
			//���㳤��
			nLenght = MultiByteToWideChar (CP_UTF8, 0, strHtml.c_str(), -1, NULL, 0);
			if(strHtml.size() < HTTP_FILE_SIZE) {
				MultiByteToWideChar (CP_UTF8, 0, strHtml.c_str(), -1, m_szwHttpContext, nLenght);
			}

			//���㳤��
			nLenght = WideCharToMultiByte (CP_UTF8, 0, m_szwHttpContext, -1, NULL, 0, NULL, FALSE);
			if(nLenght < HTTP_FILE_SIZE) {
				WideCharToMultiByte(CP_UTF8, 0, m_szwHttpContext, -1, m_szHttpContext, nLenght, NULL, FALSE);
			}
		}

		// ����
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

	//�������˳�
	m_bHttpComplete = TRUE;
}


//��������
WORD CHttpClient::TopNews(CTopNewsArray & _cTopNewsArray)
{
	/*��cJSON *json_root, *json_item, *json_title, *json_weblink;
	 
	// �������ݰ�  
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

		//����
		json_title = cJSON_GetObjectItem( json_item, "Title");
		if (json_title==NULL) {
			continue;
		}
		
		if (json_title->type==cJSON_String) {
			pTemp.szTitle = json_title->valuestring;
		}
		
		//����
		json_weblink = cJSON_GetObjectItem( json_item, "WebLink");
		if (json_weblink==NULL) {
			continue;
		}

		if (json_weblink->type==cJSON_String) {
			pTemp.szWeblink = json_title->valuestring;
		}

		_cTopNewsArray.Add(pTemp);
	}

	return _cTopNewsArray.GetCount();��*/

	return 0;
}


//��ֵ����
WORD CHttpClient::TopRecharge(CTopRechargeArray & _cTopRechargeArray)
{
	/*  cJSON *json_root, *json_item, *json_name, *json_score;
	 
	// �������ݰ�  
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

		//����
		json_name = cJSON_GetObjectItem( json_item, "Name");
		if (json_name==NULL) {
			continue;
		}
		
		if (json_name->type==cJSON_String) {
			pTemp.szName = json_name->valuestring;
		}
		
		//����
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


//����ת��
CString CHttpClient::GetString(std::string szText)
{
	WCHAR szwText[HTTP_NAME_SIZE] = {0};

	//���㳤��
	int nLenght = MultiByteToWideChar (CP_UTF8, 0, szText.c_str(), -1, NULL, 0);
	if(szText.size() < HTTP_FILE_SIZE) {
		MultiByteToWideChar (CP_UTF8, 0, szText.c_str(), -1, szwText, nLenght);
	}

	return szwText;
}
