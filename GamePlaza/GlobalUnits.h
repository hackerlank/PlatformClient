#ifndef GLOBAL_UNITES_HEAD_FILE
#define GLOBAL_UNITES_HEAD_FILE


#pragma once
//////////////////////////////////////////////////////////////////////////////////


//ȫ�ֵ�Ԫ
class CGlobalUnits
{
	//�汾��Ϣ
protected:
	DWORD							m_dwPlazaVersion;					//�����汾
	DWORD							m_dwFrameVersion;					//��ܰ汾

	//׷�ٱ���
protected:
	WORD							m_wTrackServerID;					//׷�ٷ���
	DWORD							m_dwTrackUserID;					//׷�ٱ�ʶ

	//��̬����
protected:
	static CGlobalUnits *			m_pGlobalUnits;						//����ָ��

	//��������
public:
	//���캯��
	CGlobalUnits();
	//��������
	virtual ~CGlobalUnits();

	//�汾��Ϣ
public:
	//�����汾
	virtual DWORD GetPlazaVersion() { return m_dwPlazaVersion; }
	//��ܰ汾
	virtual DWORD GetFrameVersion() { return m_dwFrameVersion; }

	//��������
public:
	//��������
	bool PlayGameSound(LPCTSTR pszSoundName);
	//��������
	bool PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName);

	//׷�ٺ���
public:
	//��ȡ�û�
	DWORD GetTrackUserID();
	//��ȡ����
	WORD GetTrackServerID();
	//���ñ�ʶ
	VOID SetTrackUserID(DWORD dwTrackUserID) { m_dwTrackUserID=dwTrackUserID;}
	//���ñ�ʶ
	VOID SetTrackServerID(WORD wTrackServerID) { m_wTrackServerID=wTrackServerID;}

	//���ܺ���
public:
	//������Ϸ
	bool DownLoadClient(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);

	//��̬����
public:
	//��ȡ����
	static CGlobalUnits * GetInstance() { return m_pGlobalUnits; }
};


//////////////////////////////////////////////////////////////////////////////////

#endif