#include "Stdafx.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"


//////////////////////////////////////////////////////////////////////////////////
//��̬����

CGlobalUnits * CGlobalUnits::m_pGlobalUnits=NULL;						//����ָ��

//////////////////////////////////////////////////////////////////////////////////



//���캯��
CGlobalUnits::CGlobalUnits()
{
	//�汾��Ϣ
	m_dwPlazaVersion=VERSION_PLAZA;
	m_dwFrameVersion=VERSION_FRAME;

	//׷�ٱ���
	m_dwTrackUserID=0;
	m_wTrackServerID=0;

	//���ö���
	ASSERT(m_pGlobalUnits==NULL);
	if (m_pGlobalUnits==NULL) m_pGlobalUnits=this;

	return;
}

//��������
CGlobalUnits::~CGlobalUnits()
{
	//�ͷŶ���
	ASSERT(m_pGlobalUnits==this);
	if (m_pGlobalUnits==this) m_pGlobalUnits=NULL;

	return;
}


//��������
bool CGlobalUnits::PlayGameSound(LPCTSTR pszSoundName)
{
	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//�����ж�
	if (pParameterGlobal->m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//��������
	PlaySound(pszSoundName,AfxGetInstanceHandle(),SND_ASYNC|SND_NODEFAULT);

	return true;
}

//��������
bool CGlobalUnits::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//�����ж�
	if (pParameterGlobal->m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//������Դ
	HRSRC hResource=FindResource(hInstance,pszSoundName,TEXT("WAVE"));
	if (hResource==NULL) return false;

	//������Դ
	HGLOBAL hGlobal=LoadResource(hInstance,hResource);
	if (hGlobal==NULL) return false;

	//��������
	LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
	PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY);

	//������Դ
	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return true;
}


//��ȡ��ʶ
DWORD CGlobalUnits::GetTrackUserID()
{
	//��������
	DWORD dwTrackUserID=m_dwTrackUserID;

	//���ñ���
	m_dwTrackUserID=0;

	return dwTrackUserID;
}

//��ȡ����
WORD CGlobalUnits::GetTrackServerID()
{
	//��������
	WORD wTrackServerID=m_wTrackServerID;

	//���ñ���
	m_wTrackServerID=0;

	return wTrackServerID;
}


//������Ϸ
bool CGlobalUnits::DownLoadClient(LPCTSTR pszClientName, WORD wKindID, WORD wServerID)
{
	//��������
	
	/*
	if (m_pDlgDownLoad==NULL)
	{
		m_pDlgDownLoad=new CDlgDownLoad;
	}

	//��������
	if (m_pDlgDownLoad->m_hWnd==NULL)
	{
		m_pDlgDownLoad->Create(IDD_DLG_POPUP,CPlatformFrame::GetInstance());
	}

	//������Ϸ
	m_pDlgDownLoad->DownLoadClient(pszClientName,wKindID,wServerID);
	*/

	return true;
}