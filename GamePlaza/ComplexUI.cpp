#include "Stdafx.h"
#include "ComplexUI.h"

//���캯��
CComplexUI::CComplexUI()  
{  
	
}

//��������
CComplexUI::~CComplexUI()  
{

}

//�������
LPCTSTR CComplexUI::GetClass() const  
{  
    return kComplexClassName;  
}  

//�ӿں���
LPVOID CComplexUI::GetInterface(LPCTSTR pstrName)  
{  
    if( _tcscmp(pstrName, kComplexInterface) == 0 ) {
		return static_cast<CComplexUI*>(this);
	}
    return CButtonUI::GetInterface(pstrName);  
}

//�ع�����
void CComplexUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CButtonUI::SetAttribute(pstrName, pstrValue);  
}

//���»���
void CComplexUI::PaintStatusImage(HDC hDC)
{
	//���غ���
	CButtonUI::PaintStatusImage(hDC);

	//��ȡ����
	WORD wX = this->GetX();
	WORD wY = this->GetY();

	//���û���
	int OldMode = ::SetBkMode(hDC,TRANSPARENT);

	//��������		
	HGDIOBJ  OldFont  = ::SelectObject(hDC,m_pManager->GetFont(1));
	COLORREF OldColor = ::SetTextColor(hDC,RGB(255,255,255));
		
	//�������
	CRect rcEnterScore(wX+45, wY+70, wX+145, wY+82);
	DrawText(hDC,m_szEntryScore,lstrlen(m_szEntryScore),rcEnterScore,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);

	//��ע����
	CRect rcBaseScore(wX+45, wY+85, wX+145, wY+97);
	DrawText(hDC,m_szBaseScore,lstrlen(m_szBaseScore),rcBaseScore,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);

	//��������		
	::SetTextColor(hDC,RGB(154,205,50));

	//��������
	CRect rcOnline(wX+70, wY+160, wX+170, wY+175);
	DrawText(hDC,m_szOnline,lstrlen(m_szOnline),rcOnline,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);
	
	//��ԭ����
	::SetTextColor(hDC, OldColor);
	::SelectObject(hDC, OldFont);
	::SetBkMode(hDC, OldMode);
}

//�������
void CComplexUI::SetMaxOnline(DWORD dwMaxCount)
{
	m_dwMaxCount = dwMaxCount;
}

//�������
void CComplexUI::SetOnline(DWORD dwOnLineCount)
{
	m_dwOnLineCount = dwOnLineCount;
}

//����ɼ�
void CComplexUI::SetEnterScore(LONGLONG lEnterScore)
{
	m_lEnterScore = lEnterScore;
}

//��ע�ɼ�
void CComplexUI::SetCellScore(LONGLONG lCellScore)
{
	m_lCellScore = lCellScore;
}

//������Ϣ
void CComplexUI::UpdateInfo()
{
	//������Ϣ
	m_szOnline.Format( TEXT("%d/%d"), m_dwOnLineCount, m_dwMaxCount );

	//����ɼ�
	m_szEntryScore.Format( TEXT("�볡:%sԪ"), AddDecimal(m_lEnterScore) );

	//��ע�ɼ�
	m_szBaseScore.Format( TEXT("��ע:%sԪ"), AddDecimal(m_lCellScore) );
}


// ���С����
CString CComplexUI::AddDecimal( LONGLONG lScore ,  bool bComma /*= true*/, bool bPlus /*= false*/)
{
	CString strScore;
	CString strReturn;
	LONGLONG lNumber = lScore;
	if ( lScore < 0 )
		lNumber = -lNumber;

	strScore.Format(TEXT("%I64d"), lNumber);

	//����
	int nLength = strScore.GetLength();
	if (nLength==0) 
	{
		strReturn.Insert(0, TEXT("0.00") );
	}
	else if (nLength==1) {
		strReturn.Insert(0, TEXT("0.0") );
	}
	if (nLength==2) 
	{
		strReturn.Insert(0, strScore.GetAt(nLength-1) );
		strReturn.Insert(0, strScore.GetAt(nLength-2) );
		strReturn.Insert(0, TEXT("0.") );
	}
	else if(nLength>2)
	{
		strReturn.Insert(0, strScore.GetAt(nLength-1) );
		strReturn.Insert(0, strScore.GetAt(nLength-2) );
		strReturn.Insert(0, TEXT(".") );

		int nStrCount = 0;
		for( int i = strScore.GetLength() - 3; i >= 0; )
		{
			if( (nStrCount%3)==0 && nStrCount!=0 && bComma)
			{
				strReturn.Insert(0, ',');
				nStrCount = 0;
			}
			else
			{
				strReturn.Insert(0, strScore.GetAt(i));
				nStrCount++;
				i--;
			}
		}
	}

	if ( lScore < 0 )
		strReturn.Insert(0, '-');

	if ( bPlus && lScore > 0 )
		strReturn.Insert(0, '+');

	return strReturn;
}

// ��Ӷ���
CString CComplexUI::AddComma( LONGLONG lScore , bool bPlus /*= false*/)
{
	CString strScore;
	CString strReturn;
	LONGLONG lNumber = lScore;
	if ( lScore < 0 )
		lNumber = -lNumber;

	strScore.Format(TEXT("%I64d"), lNumber);

	//����
	int nLength = strScore.GetLength();
	if (nLength==0) 
	{
		strReturn.Insert(0, TEXT("0.00") );
	}
	else if (nLength==1) {
		strReturn.Insert(0, TEXT("0.0") );
	}
	if (nLength==2) 
	{
		strReturn.Insert(0, TEXT("0.") );
	}
	else if(nLength>2)
	{
		strReturn.Insert(0, strScore.GetAt(nLength-1) );
		strReturn.Insert(0, strScore.GetAt(nLength-2) );
		strReturn.Insert(0, TEXT(".") );

		int nStrCount = 0;
		for( int i = strScore.GetLength() - 3; i >= 0; )
		{
			if( (nStrCount%3) == 0 && nStrCount != 0 )
			{
				strReturn.Insert(0, ',');
				nStrCount = 0;
			}
			else
			{
				strReturn.Insert(0, strScore.GetAt(i));
				nStrCount++;
				i--;
			}
		}
	}

	if ( lScore < 0 )
		strReturn.Insert(0, '-');

	if ( bPlus && lScore > 0 )
		strReturn.Insert(0, '+');

	return strReturn;
}