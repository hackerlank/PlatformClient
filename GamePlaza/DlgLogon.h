#ifndef DLG_LOGON_HEAD_FILE
#define DLG_LOGON_HEAD_FILE

#include "DlgAccredit.h"

//////////////////////////////////////////////////////////////////////////////////

//��¼��ʽ
#define LOGON_BY_VISITOR			2									//�ο͵�¼
#define LOGON_BY_GAME_ID			1									//I D ��¼
#define LOGON_BY_ACCOUNTS			0									//�ʺŵ�¼

//////////////////////////////////////////////////////////////////////////////////

//�ʺ���Ϣ
struct tagAccountsInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szGameID[16];						//��Ϸ��ʶ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};

//���鶨��
typedef CWHArray<tagAccountsInfo *>	CAccountsInfoArray;					//�ʺ�����

//////////////////////////////////////////////////////////////////////////////////


//��¼����
class CDlgLogon : public CFGuiDialog
{
	//��������
public:	
	CPngImage						m_ImageBack;						//����ͼƬ

	//���ñ���
public:
	BYTE							m_cbLogonMode;						//��¼ģʽ
	BYTE							m_cbRemPassword;					//��ס����

	//��¼��Ϣ
public:
	DWORD							m_dwGameID;							//��Ϸ I D
	TCHAR							m_szAccounts[LEN_ACCOUNTS];			//��Ϸ�ʺ�
	TCHAR							m_szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							m_szLogonServer[LEN_SERVER];		//��¼��ַ
	TCHAR							m_szPassPortID[LEN_PASS_PORT_ID];	//���֤��

	//����ؼ�
protected:	
	CDlgAccredit *					m_pDlgAccedit;						//��֤����
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��

	//��������
protected:
	CAccountsInfoArray				m_AccountsInfoArray;				//�ʺ���Ϣ

	//��������
public:
	//���캯��
	CDlgLogon();
	//��������
	virtual ~CDlgLogon();

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);

	//�滭�ӿ�
public:
	//�����滭
	virtual void OnBeginPaintWindow(HDC hDC);

	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT(""); } 

	//���ܺ���
public:
	//��ʾ��֤
	bool ShowAccreditWindow();
	//��������
	WORD ConstructLogonPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags);

	//��������
private:
	//��ȡ��Ϣ
	bool GetInformation();
	//�����ʺ�
	VOID LoadAccountsInfo();
	//��ַ��Ϣ
	VOID LoadLogonServerInfo();
	//ɾ����¼
	VOID DeleteRecord();

	//��ť��Ϣ
protected:
	//ע���ʺ�
	VOID OnBnClickedRegister();
	//��������
	VOID OnBnForgetPassword();

	//�ؼ�����
protected:
	//ѡ��ı�
	VOID OnSelchangeAccounts();
	//�༭�ı�
	VOID OnCbnEditchangeAccounts();

	//ϵͳ��Ϣ
protected:
	//�ؼ���ɫ
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	//�ߴ�仯
	VOID OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()	
};

//////////////////////////////////////////////////////////////////////////////////

#endif