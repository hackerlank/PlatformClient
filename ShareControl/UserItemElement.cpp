#include "StdAfx.h"
#include "UserItemElement.h"

//////////////////////////////////////////////////////////////////////////////////
//������

#define MAX_EXPERIENCE				5									//�����

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CUserItemElement * CUserItemElement::m_pUserItemElement=NULL;			//����ָ��

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CUserItemElement::CUserItemElement()
{
	//��������
	m_wLevelCount=0;
	m_pGrowLevelConfig=NULL;

	//���ñ���
	m_pIUserOrderParser=NULL;
	m_pIFaceItemControl=NULL;

	//���ö���
	ASSERT(m_pUserItemElement==NULL);
	if (m_pUserItemElement==NULL) m_pUserItemElement=this;

	return;
}

//��������
CUserItemElement::~CUserItemElement()
{
	//�ͷŶ���
	ASSERT(m_pUserItemElement==this);
	if (m_pUserItemElement==this) 
	{
		//�ͷ���Դ
		SafeDeleteArray(m_pGrowLevelConfig);

		m_pUserItemElement=NULL;
	}

	return;
}

//���ýӿ�
VOID CUserItemElement::SetUserOrderParser(IUserOrderParser * pIUserOrderParser)
{
	//���ñ���
	m_pIUserOrderParser=pIUserOrderParser;

	return;
}

//���ýӿ�
VOID CUserItemElement::SetFaceItemControl(IFaceItemControl * pIFaceItemControl)
{
	//���ñ���
	m_pIFaceItemControl=pIFaceItemControl;

	return;
}

//���õȼ�
VOID CUserItemElement::SetGrowLevelConfig(WORD wLevelCount,tagGrowLevelConfig * pGrowLevelConfig)
{
	//����У��
	ASSERT(wLevelCount>0 && pGrowLevelConfig!=NULL);
	if(wLevelCount==0 || pGrowLevelConfig==NULL) return;

	try
	{
		//������Դ
		m_pGrowLevelConfig = new tagGrowLevelConfig[wLevelCount];
		if(m_pGrowLevelConfig==NULL) throw TEXT("�ڴ治��,��Դ����ʧ�ܣ�");

		//���ñ���
		m_wLevelCount=wLevelCount;
		CopyMemory(m_pGrowLevelConfig,pGrowLevelConfig,sizeof(tagGrowLevelConfig)*m_wLevelCount);
	}
	catch(...)
	{
		ASSERT(FALSE);

		//�ͷ���Դ
		SafeDeleteArray(m_pGrowLevelConfig);
	}

	return;
}

//�滭����
VOID CUserItemElement::DrawExperience(CDC * pDC, INT nXPos, INT nYPos, DWORD dwExperience)
{
	if (dwExperience>=0L)
	{
		//��ȡ���
		HINSTANCE hInstance = GetModuleHandle(SHARE_CONTROL_DLL_NAME);

		//������Դ
		CPngImage ImageLevelBack,ImageLevelNumber;
		ImageLevelBack.LoadImage(hInstance,TEXT("IMAGE_LEVEL_BACK"));
		ImageLevelNumber.LoadImage(hInstance,TEXT("IMAGE_LEVEL_NUMBER"));

		//��������
		CSize SizeLevelBack(ImageLevelBack.GetWidth(),ImageLevelBack.GetHeight());

		//��ȡ����
		WORD wExperienceLevel=GetExperienceLevel(dwExperience);

		//�滭�ȼ�
		ImageLevelBack.DrawImage(pDC,nXPos,nYPos);
		DrawNumber(pDC,&ImageLevelNumber,nXPos+SizeLevelBack.cx/2-2,nYPos+8,TEXT("0123456789"),wExperienceLevel,TEXT("%d"));
	}

	return;
}

//������Ƭ
VOID CUserItemElement::ConstructNameCard(IClientUserItem * pIClientUserItem, IGameLevelParser * pIGameLevelParser, CImage&ImageNameCard)
{
	//������Ƭ
	CSize ImageSize;
	ImageSize.SetSize(NAME_CARD_CX,NAME_CARD_CY);
	ImageNameCard.Create(ImageSize.cx,ImageSize.cy,32);

	//��������
	TCHAR szString[128]=TEXT("");
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//���� DC
	CDC * pDC=CDC::FromHandle(ImageNameCard.GetDC());
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//���� DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(CSkinMenuKernel::m_SkinAttribute.m_crNormalText);

	//λ�ö���
	CRect rcNickName(60,5,ImageSize.cx-5,5+12);
	CRect rcGameLevel(60,25,ImageSize.cx-5,25+12);
	CRect rcExperience(60,45,ImageSize.cx-5,45+12);
	CRect rcUserGameID(60,65,ImageSize.cx-5,65+12);
	CRect rcUserUnderWrite(60,85,ImageSize.cx-5,85+12);

	//�滭����
	pDC->FillSolidRect(0,0,ImageSize.cx,ImageSize.cy,CSkinMenuKernel::m_SkinAttribute.m_crNormalBack);

	//�滭ͷ��
	if ((pCustomFaceInfo->dwDataSize!=0L)&&(pIClientUserItem->GetCustomID()!=0L))
	{
		m_pIFaceItemControl->DrawFaceItemFrame(pDC,2,16,ITEM_FRAME_NORMAL);
		m_pIFaceItemControl->DrawFaceNormal(pDC,2,16,pCustomFaceInfo->dwCustomFace);
	}
	else
	{
		m_pIFaceItemControl->DrawFaceItemFrame(pDC,2,16,ITEM_FRAME_NORMAL);
		m_pIFaceItemControl->DrawFaceNormal(pDC,2,16,pIClientUserItem->GetFaceID());
	}

	//�û���Ϣ
	LPCTSTR pszMember=m_pIUserOrderParser->GetMemberDescribe(pIClientUserItem->GetMemberOrder());
	LPCTSTR pszMaster=m_pIUserOrderParser->GetMasterDescribe(pIClientUserItem->GetMasterOrder());

	//�û���Ϣ
	LPCTSTR pszUserOrder=(pszMaster[0]!=0)?pszMaster:((pszMember[0]!=0)?pszMember:TEXT("��ͨ���"));
	_sntprintf(szString,CountArray(szString),TEXT("�ǳƣ�%s [ %s ]"),pIClientUserItem->GetNickName(),pszUserOrder);
	pDC->DrawText(szString,lstrlen(szString),&rcNickName,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//��Ϸ�ȼ�
	LPCTSTR pszKindName=pIGameLevelParser->GetKindName();
	LPCTSTR pszGameLevel=pIGameLevelParser->GetLevelDescribe(pIClientUserItem);
	_sntprintf(szString,CountArray(szString),TEXT("����%s [ %s ]"),pszGameLevel,pszKindName);
	pDC->DrawText(szString,lstrlen(szString),&rcGameLevel,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//�û��ȼ�	
	_sntprintf(szString,CountArray(szString),TEXT("�ȼ���"));
	pDC->DrawText(szString,lstrlen(szString),&rcExperience,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//�ȼ���־
	DWORD dwExperience=pIClientUserItem->GetUserExperience();
	DrawExperience(pDC,rcExperience.left+36,rcExperience.top-6L,dwExperience);

	//�û���ʶ
	_sntprintf(szString,CountArray(szString),TEXT("���룺%ld"),pIClientUserItem->GetGameID());
	pDC->DrawText(szString,lstrlen(szString),&rcUserGameID,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//����ǩ��
	_sntprintf(szString,CountArray(szString),TEXT("����ǩ����%s"),pIClientUserItem->GetUnderWrite());
	pDC->DrawText(szString,lstrlen(szString),&rcUserUnderWrite,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//�ͷ���Դ
	ImageNameCard.ReleaseDC();

	return;
}

//����ȼ�
BYTE CUserItemElement::GetPasswordLevel(LPCTSTR pszPassword)
{
	//��������
	BYTE cbPasswordInfo[4]={0,0,0,0};
	UINT uPasswordLength=lstrlen(pszPassword);

	//�����ж�
	if (lstrlen(pszPassword)<LEN_LESS_PASSWORD)
	{
		return PASSWORD_LEVEL_0;
	}

	//Сд��ĸ
	for (UINT i=0;i<uPasswordLength;i++)
	{
		//Сд��ĸ
		if ((pszPassword[i]>=TEXT('a'))&&((pszPassword[i]<=TEXT('z'))))
		{
			cbPasswordInfo[0]=1;
			continue;
		}

		//��д��ĸ
		if ((pszPassword[i]>=TEXT('A'))&&((pszPassword[i]<=TEXT('Z'))))
		{
			cbPasswordInfo[1]=1;
			continue;
		}

		//�����ַ�
		if ((pszPassword[i]>=TEXT('0'))&&((pszPassword[i]<=TEXT('9'))))
		{
			cbPasswordInfo[2]=1;
			continue;
		}

		//�����ַ�
		cbPasswordInfo[3]=1;
	}

	//�жϵȼ�
	BYTE cbLevelCount=0;
	for (BYTE i=0;i<CountArray(cbPasswordInfo);i++)
	{
		if (cbPasswordInfo[i]>0)
		{
			cbLevelCount++;
		}
	}

	return PASSWORD_LEVEL_0+__min(cbLevelCount,(PASSWORD_LEVEL_3-PASSWORD_LEVEL_0));
}

//��ȡ�ȼ�
WORD CUserItemElement::GetExperienceLevel(DWORD dwExperience)
{
	if (dwExperience>=0L && m_pGrowLevelConfig!=NULL)
	{
		//��������
		WORD wUserLevel=1;

		//�ȼ�����
		for (WORD wLevelIndex=0;wUserLevel<=m_wLevelCount;wLevelIndex++)
		{
			if(dwExperience<m_pGrowLevelConfig[wLevelIndex].dwExperience) break;			

			//���ñ���
			wUserLevel=m_pGrowLevelConfig[wLevelIndex].wLevelID;
		}

		return wUserLevel;
	}

	return 0L;
}

//�ȼ���Ϣ
WORD CUserItemElement::GetExperienceIndex(WORD wUserLevel, BYTE cbOrderIndex[], WORD wMaxCount)
{
	//��������
	WORD wIndexCount=0;
	WORD wLevelVaule[]={1L,5L,10L};

	//���õȼ�
	for (WORD i=0;i<wMaxCount;i++)
	{
		//��ȡ����
		for (INT j=0;j<CountArray(wLevelVaule);j++)
		{
			if (wUserLevel>=wLevelVaule[CountArray(wLevelVaule)-j-1])
			{
				//���ñ���
				wUserLevel-=wLevelVaule[CountArray(wLevelVaule)-j-1];
				cbOrderIndex[wIndexCount++]=CountArray(wLevelVaule)-j-1;

				break;
			}
		}

		//��ֹ�ж�
		if (wUserLevel==0L) break;
	}

	return wIndexCount;
}

//�ȼ���Ϣ
WORD CUserItemElement::GetExperienceInfo(tagGrowLevelConfig GrowLevelConfig[],WORD wBufferCount)
{
	//��������
	WORD wLevelCount=__min(m_wLevelCount,wBufferCount);

	//��������
	CopyMemory(GrowLevelConfig,m_pGrowLevelConfig,wLevelCount*sizeof(GrowLevelConfig[0]));

	return wLevelCount;
}

//������֤
bool CUserItemElement::EfficacyChineseCharacter(TCHAR chr)
{
	//У���ַ�
	if (chr != 0x08 && chr <= 0x80) return false;
	if (chr == 0xb7) return false;	
	if ((chr&0xff00) == 0xff00) return false;	
	if ((chr&0xff00) == 0x2000 && ::IsDBCSLeadByte(BYTE(chr&0xFF)) == FALSE) return false;
	if ((chr&0xff00) == 0x3000 && ::IsDBCSLeadByte(BYTE(chr&0xFF)) == FALSE) return false;

	return true;
}

//Ч������
bool CUserItemElement::EfficacyEmail(LPCTSTR pszEmail, TCHAR szDescribe[], WORD wMaxCount)
{
	//��������
	bool bIsValid=false;
	CString strEmail(pszEmail);

	//����У��
	if(strEmail.GetLength() < LEN_EMAIL)
	{
		//ͳ�Ʊ���		
		int nAtPos=0;
		int nAtCount=0;
		int nDotCount=0;

		//��������
		TCHAR chEmail = strEmail[0];

		// ���ַ��ж�
		if((chEmail >= 'a' && chEmail <= 'z') || (chEmail >= 'A' && chEmail <= 'Z') || (chEmail >= '0' && chEmail <= '9'))
		{ 
			//���ñ���
			bIsValid=true;

			for(int i = 1; i < strEmail.GetLength(); i++)
			{
				chEmail = strEmail[i];

				// �ַ�У��
				if((chEmail >= 'a' && chEmail <= 'z') || 
				   (chEmail >= 'A' && chEmail <= 'Z') || 
				   (chEmail >= '0' && chEmail <= '9') || 
				   (chEmail == '_') || (chEmail == '-') || (chEmail == '.') || (chEmail == '@'))
				{
					// ͳ��'@'����
					if(chEmail == '@')
					{
						nAtPos=i;
						nAtCount++;						
					}
					// ͳ��'@'֮��'.'�ĸ���
					else if((nAtCount > 0) && (chEmail == '.'))
					{
						nDotCount++;
					}
				}
				else
				{
					bIsValid=false;
					break;
				}
			}
		}
		// ��β�ַ��ж�
		if((bIsValid == true) && (chEmail == '.') || (chEmail == '-'))
		{
			bIsValid=false;
		}

		// �ַ������ж�
		if((bIsValid == true) && (nAtCount != 1) || (nDotCount < 1) || (nDotCount > 3)) 
		{
			bIsValid=false;
		}

		// �����ַ��ж�
		if(bIsValid == true)
		{
			//��������
			LPCTSTR pszInvalid[]={TEXT("@."),TEXT(".@"),TEXT(".."),TEXT("--"),TEXT("-@"),TEXT("@-")};

			for(int i = 0; i < CountArray(pszInvalid); i++)
			{
				if(strEmail.Find(pszInvalid[i]) > 0)
				{
					bIsValid=false;
					break;
				}
			}
		}

		if(bIsValid == true)
		{
			//����׺,���ú�׺�������һЩ��׺����ʱ������
			LPCTSTR pszInvalid[]={TEXT(".com"),TEXT(".cn"),TEXT(".net"),TEXT(".org"),TEXT(".hk")};
			bool bFind = false;
			for(int i = 0; i < CountArray(pszInvalid); i++)
			{
				if(strEmail.Find(pszInvalid[i]) > 0)
				{
					bFind=true;
					break;
				}
			}
			bIsValid = bFind;
		}
	}

	//������Ч
	if(bIsValid == false)
	{
		CopyMemory(szDescribe, TEXT("�������������������������룡"), sizeof(TCHAR)*wMaxCount);
		return false;
	}

	return true;
}

//Ч���ʺ�
bool CUserItemElement::EfficacyAccounts(LPCTSTR pszAccounts, TCHAR szDescribe[], WORD wMaxCount)
{
	//��������
	TCHAR szSpecialChar=TEXT('_');	
	TCHAR szNumber[]={TEXT('0'),TEXT('9')};
	TCHAR szLowerLetter[]={TEXT('a'),TEXT('z')};
	TCHAR szUpperLetter[]={TEXT('A'),TEXT('Z')};

	//��������
	INT nChinessCount=0;
	INT nCharLength=lstrlen(pszAccounts);

	//�����ַ�
	for (INT i=0;i<nCharLength;i++)
	{
		if (EfficacyChineseCharacter(pszAccounts[i])==false)
		{
			//�����ַ�
			if(pszAccounts[i]==szSpecialChar) continue;

			//�����ַ�
            if(pszAccounts[i]>=szNumber[0] && pszAccounts[i]<=szNumber[1]) continue;

			//Сд��ĸ
            if(pszAccounts[i]>=szLowerLetter[0] && pszAccounts[i]<=szLowerLetter[1]) continue;

			//��д��ĸ
            if(pszAccounts[i]>=szUpperLetter[0] && pszAccounts[i]<=szUpperLetter[1]) continue;

			//�Ƿ��ַ�
			_sntprintf(szDescribe,wMaxCount,TEXT("��Ϸ�ʺű�������ĸ�����֡��»��ߺͺ�����ɣ�"),LEN_LESS_ACCOUNTS);

			return false;
		}
		else
		{
			nChinessCount++;
		}
	}

	//�����ж�
	if ((nChinessCount+nCharLength)<LEN_LESS_ACCOUNTS)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("��Ϸ�ʺű������%ldλ���ϣ����������룡"),LEN_LESS_ACCOUNTS);
		return false;
	}

	return true;
}

//Ч���ǳ�
bool CUserItemElement::EfficacyNickName(LPCTSTR pszNickName, TCHAR szDescribe[], WORD wMaxCount)
{
	//��������
	INT nCharLength=lstrlen(pszNickName);
	INT nDataLength=nCharLength*sizeof(TCHAR);

	//�����ж�
	if (nCharLength<LEN_LESS_NICKNAME)
	{
		//��������
		WORD nChinessCount=0;

		//�����ַ�
		for (INT i=0;i<nCharLength;i++)
		{
			if (pszNickName[i]>=0x80)
			{
				nChinessCount++;
			}
		}

		//�����ж�
		if ((nChinessCount+nCharLength)<LEN_LESS_NICKNAME)
		{
			_sntprintf(szDescribe,wMaxCount,TEXT("��Ϸ�ǳƱ������%ldλ���ϣ����������룡"),LEN_LESS_NICKNAME);
			return false;
		}
	}

	return true;
}

//Ч������
bool CUserItemElement::EfficacyPassword(LPCTSTR pszPassword, TCHAR szDescribe[], WORD wMaxCount)
{
	//��������
	INT nCharLength=lstrlen(pszPassword);
	INT nDataLength=nCharLength*sizeof(TCHAR);

	//�����ж�
	if (nCharLength<LEN_LESS_PASSWORD)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("���������� %ld λ���ϣ����������룡"),LEN_LESS_PASSWORD);
		return false;
	}

	return true;
}

//Ч��֤��
bool CUserItemElement::EfficacyPassPortID(LPCTSTR pszPassPortID, TCHAR szDescribe[], WORD wMaxCount)
{
	//�����ж�
	UINT nCharLength=lstrlen(pszPassPortID);
	if(nCharLength==0L) return true;

	//��������
	bool bNumber=true;

	//�����ж�
	for (UINT i=0;i<nCharLength;i++)
	{
		if ((pszPassPortID[i]<TEXT('0'))||(pszPassPortID[i]>TEXT('9')))
		{
			if(pszPassPortID[i] != TEXT('x') && pszPassPortID[i] != TEXT('X'))
			{
				bNumber=false;
				break;
			}
		}
	}

	//�Ϸ��ж�
	if ((bNumber==false)||((nCharLength!=15L)&&(nCharLength!=18L)))
	{
		lstrcpyn(szDescribe,TEXT("���֤�������Ϊ 15 λ���� 18 λ���֣����������룡"),wMaxCount);
		return false;
	}

	//��������
	bool bNewPassPortID = (nCharLength==18L);
	bool bYear=true;
	bool bMonth=true;
	bool bDay=true;
	bool bCheck=true;

	//�Ϸ��ж�
	if(bNewPassPortID)
	{
		TCHAR szYear[10]={0};
		for(BYTE i=0; i<4; i++)
		{
			szYear[i]=pszPassPortID[i+6];
		}
		int nYear = StrToInt(szYear);
		if(nYear < 1900 || nYear > 2100)
		{
			bYear=false;
		}
	}

	//�Ϸ��ж�
	BYTE cbIndex=8;
	if(bNewPassPortID)cbIndex=10;
	TCHAR szMonth[10]={0};
	for(BYTE i=0; i<2; i++)
	{
		szMonth[i]=pszPassPortID[i+cbIndex];
	}
	int nMonth = StrToInt(szMonth);
	if(nMonth < 1 || nMonth > 12)
	{
		bMonth=false;
	}

	//�Ϸ��ж�
	cbIndex=10;
	if(bNewPassPortID)cbIndex=12;
	TCHAR szDay[10]={0};
	for(BYTE i=0; i<2; i++)
	{
		szDay[i]=pszPassPortID[i+cbIndex];
	}
	int nDay = StrToInt(szDay);
	if(nDay < 1 || nDay > 31)
	{
		bDay=false;
	}

	//�Ϸ��ж�
	if(bNewPassPortID)
	{
		TCHAR szTemp[10]={0};
		int nSum=0;
		int nWi[]={7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2,1};
		TCHAR cCheckArry[]={TEXT('1'),TEXT('0'),TEXT('X'),TEXT('9'),TEXT('8'),TEXT('7'),TEXT('6'),TEXT('5'),TEXT('4'),TEXT('3'),TEXT('2')};
		for(BYTE i=0; i<17; i++)
		{
			szTemp[0]=pszPassPortID[i];
			int nAi=StrToInt(szTemp);
			nSum+=(nAi*nWi[i]);
		}
		TCHAR cCheck=cCheckArry[nSum%11];
		TCHAR cCheck2=pszPassPortID[17];

		if((cCheck2 != cCheck) && !((cCheck==TEXT('X')) && (cCheck2==TEXT('x'))))
		{
			bCheck=false;
		}
	}

	//�Ϸ��ж�
	if(!bYear||!bMonth||!bDay||!bCheck)
	{
		lstrcpyn(szDescribe,TEXT("����������֤���벻��ȷ��������������ȷ�����֤���룡"),wMaxCount);
		return false;
	}

	return true;
}

//Ч������
bool CUserItemElement::EfficacyCompellation(LPCTSTR pszCompellation, TCHAR szDescribe[], WORD wMaxCount)
{
	//��������
	INT nChinessCount=0;
	INT nCharLength=lstrlen(pszCompellation);

	//�����ַ�
	for (INT i=0;i<nCharLength;i++)
	{
		if (EfficacyChineseCharacter(pszCompellation[i])==false)
		{
			//�Ƿ��ַ�
			_sntprintf(szDescribe,wMaxCount,TEXT("��ʵ����ֻ���ɺ�����ɣ�"),LEN_LESS_ACCOUNTS);

			return false;
		}
		else
		{
			nChinessCount++;
		}
	}

	//�����ж�
	if ((nChinessCount+nCharLength)>LEN_COMPELLATION)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("��ʵ��������С�ڵ���%ldλ�����������룡"),LEN_COMPELLATION);
		return false;
	}

	//�����ж�
	if ((nChinessCount+nCharLength)<LEN_LESS_COMPELLATION)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("��ʵ����������ڵ���%ldλ�����������룡"),LEN_LESS_COMPELLATION);
		return false;
	}	

	return true;
}

//Ч���ֻ�
bool CUserItemElement::EfficacyMobilePhone(LPCTSTR pszMobilePhone, TCHAR szDescribe[], WORD wMaxCount)
{
	//����У��
	if(lstrlen(pszMobilePhone) == LEN_MOBILE_PHONE-1)
	{
		//�׺�У��
		LPCTSTR pszFirstNo=TEXT("1");
		if(StrChr(pszFirstNo,pszMobilePhone[0]) != NULL)
		{
			//�Ŷζ���
			LPCTSTR pszDXMobilePhone[]={TEXT("3578"),TEXT("3"),TEXT("3"),TEXT("7"),TEXT("019")};
			LPCTSTR pszLTMobilePhone[]={TEXT("34578"),TEXT("012"),TEXT("5"),TEXT("56"),TEXT("6"),TEXT("56")};
			LPCTSTR pszYDMobilePhone[]={TEXT("34578"),TEXT("456789"),TEXT("7"),TEXT("012789"),TEXT("8"),TEXT("23478")};

			//��������
			LPCTSTR pszMobileNo=NULL;

			//���źŶ�
			if(pszMobileNo = StrChr(pszDXMobilePhone[0],pszMobilePhone[1]))
			{
				INT_PTR nPos = pszMobileNo-pszDXMobilePhone[0];
				if(StrChr(pszDXMobilePhone[nPos+1],pszMobilePhone[2]) != NULL)
				{
					CopyMemory(szDescribe,TEXT("���źŶ�"),sizeof(TCHAR)*wMaxCount);
					return true;
				}
			}

			//��ͨ�Ŷ�
			if(pszMobileNo = StrChr(pszLTMobilePhone[0],pszMobilePhone[1]))
			{
				INT_PTR nPos = pszMobileNo-pszLTMobilePhone[0];
				if(StrChr(pszLTMobilePhone[nPos+1],pszMobilePhone[2]) != NULL)
				{
					CopyMemory(szDescribe,TEXT("��ͨ�Ŷ�"),sizeof(TCHAR)*wMaxCount);
					return true;
				}
			}

			//�ƶ��Ŷ�
			if(pszMobileNo = StrChr(pszYDMobilePhone[0],pszMobilePhone[1]))
			{
				INT_PTR nPos = pszMobileNo-pszYDMobilePhone[0];
				if(StrChr(pszYDMobilePhone[nPos+1],pszMobilePhone[2]) != NULL)
				{
					CopyMemory(szDescribe,TEXT("�ƶ��Ŷ�"),sizeof(TCHAR)*wMaxCount);
					return true;
				}
			}
		}
	}

	//�Ƿ�����
	CopyMemory(szDescribe,TEXT("�����ֻ�������������,���������룡"),sizeof(TCHAR)*wMaxCount);

	return false;
}

//֤������
VOID CUserItemElement::ProtectPassPortID(LPCTSTR pszPassPortID,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar)
{
	//����У��
	ASSERT(pszPassPortID != NULL);
	if(pszPassPortID == NULL) return;

	//��������
	CopyMemory(szResult, pszPassPortID, sizeof(TCHAR)*wMaxCount);

	//��ȡ����
	int nLen = lstrlen(szResult);

	//�滻����
	int i = 0;
	for(i=0;i<nLen;i++)
	{
		if(i > 3 && i < nLen-4) szResult[i] = MaskChar;
	}

	szResult[i]=0;
}

//���뱣��
VOID CUserItemElement::ProtectSeatPhone(LPCTSTR pszModileMuber,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar)
{
	//����У��
	ASSERT(pszModileMuber!=NULL);
	if(pszModileMuber==NULL) return;

	//��������
	CopyMemory(szResult, pszModileMuber, sizeof(TCHAR)*wMaxCount);

	//��ȡ����
	int nLen = lstrlen(szResult);

	//�滻����
	int i = 0;
	for(i = 0; i < nLen; i++)
	{
		if(i > 3 && i < nLen-3) szResult[i] = MaskChar;
	}

	szResult[i] = 0;
}

//���뱣��
VOID CUserItemElement::ProtectMobilePhone(LPCTSTR pszModileMuber,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar)
{
	//����У��
	ASSERT(pszModileMuber!=NULL);
	if(pszModileMuber==NULL) return;

	//��������
	CopyMemory(szResult, pszModileMuber, sizeof(TCHAR)*wMaxCount);

	//��ȡ����
	int nLen = lstrlen(szResult);

	//�滻����
	int i = 0;
	for( i = 0; i < nLen; i++)
	{
		if(i >= 3 && i < nLen-4) szResult[i] = MaskChar;
	}

	szResult[i] = 0;
}


//���Ƶȼ�
VOID CUserItemElement::DrawNumber(CDC * pDC,CPngImage * pImageNumber,INT nXPos,INT nYPos,LPCTSTR pszNumber,INT nNumber,LPCTSTR pszFormat,UINT nFormat)
{
	//����У��
	if(pImageNumber==NULL) return;

	//��������
	CString strNumber(pszNumber);
	CSize SizeNumber(pImageNumber->GetWidth()/strNumber.GetLength(),pImageNumber->GetHeight());

	//��������	
	TCHAR szValue[32]=TEXT("");
	_sntprintf(szValue,CountArray(szValue),pszFormat,nNumber);

	//����������
	if((nFormat&DT_LEFT)!=0) nXPos -= 0;
	if((nFormat&DT_CENTER)!=0) nXPos -= (lstrlen(szValue)*SizeNumber.cx)/2;
	if((nFormat&DT_RIGHT)!=0) nXPos -= lstrlen(szValue)*SizeNumber.cx;
	
	//����������
	if((nFormat&DT_TOP)!=0) nYPos -= 0;
	if((nFormat&DT_VCENTER)!=0) nYPos -= SizeNumber.cy/2;
	if((nFormat&DT_BOTTOM)!=0) nYPos -= SizeNumber.cy;

	//�滭����
	INT nIndex=0;
	CStdString ImageModify;
	for(INT i=0; i<lstrlen(szValue);i++)
	{
		nIndex = strNumber.Find(szValue[i]);
		if(nIndex!=-1)
		{
			pImageNumber->DrawImage(pDC,nXPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*nIndex,0,SizeNumber.cx,SizeNumber.cy);
			nXPos += SizeNumber.cx;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
