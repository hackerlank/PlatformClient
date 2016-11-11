#include "Stdafx.h"
#include "ComplexUI.h"

//构造函数
CComplexUI::CComplexUI()  
{  
	
}

//析构函数
CComplexUI::~CComplexUI()  
{

}

//获得类名
LPCTSTR CComplexUI::GetClass() const  
{  
    return kComplexClassName;  
}  

//接口函数
LPVOID CComplexUI::GetInterface(LPCTSTR pstrName)  
{  
    if( _tcscmp(pstrName, kComplexInterface) == 0 ) {
		return static_cast<CComplexUI*>(this);
	}
    return CButtonUI::GetInterface(pstrName);  
}

//重构属性
void CComplexUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CButtonUI::SetAttribute(pstrName, pstrValue);  
}

//重新绘制
void CComplexUI::PaintStatusImage(HDC hDC)
{
	//重载函数
	CButtonUI::PaintStatusImage(hDC);

	//获取坐标
	WORD wX = this->GetX();
	WORD wY = this->GetY();

	//设置环境
	int OldMode = ::SetBkMode(hDC,TRANSPARENT);

	//设置字体		
	HGDIOBJ  OldFont  = ::SelectObject(hDC,m_pManager->GetFont(1));
	COLORREF OldColor = ::SetTextColor(hDC,RGB(255,255,255));
		
	//进入积分
	CRect rcEnterScore(wX+45, wY+70, wX+145, wY+82);
	DrawText(hDC,m_szEntryScore,lstrlen(m_szEntryScore),rcEnterScore,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);

	//底注积分
	CRect rcBaseScore(wX+45, wY+85, wX+145, wY+97);
	DrawText(hDC,m_szBaseScore,lstrlen(m_szBaseScore),rcBaseScore,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);

	//设置字体		
	::SetTextColor(hDC,RGB(154,205,50));

	//进入人数
	CRect rcOnline(wX+70, wY+160, wX+170, wY+175);
	DrawText(hDC,m_szOnline,lstrlen(m_szOnline),rcOnline,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);
	
	//还原参数
	::SetTextColor(hDC, OldColor);
	::SelectObject(hDC, OldFont);
	::SetBkMode(hDC, OldMode);
}

//最大人数
void CComplexUI::SetMaxOnline(DWORD dwMaxCount)
{
	m_dwMaxCount = dwMaxCount;
}

//最大在线
void CComplexUI::SetOnline(DWORD dwOnLineCount)
{
	m_dwOnLineCount = dwOnLineCount;
}

//进入成绩
void CComplexUI::SetEnterScore(LONGLONG lEnterScore)
{
	m_lEnterScore = lEnterScore;
}

//底注成绩
void CComplexUI::SetCellScore(LONGLONG lCellScore)
{
	m_lCellScore = lCellScore;
}

//更新信息
void CComplexUI::UpdateInfo()
{
	//人数信息
	m_szOnline.Format( TEXT("%d/%d"), m_dwOnLineCount, m_dwMaxCount );

	//进入成绩
	m_szEntryScore.Format( TEXT("入场:%s元"), AddDecimal(m_lEnterScore) );

	//底注成绩
	m_szBaseScore.Format( TEXT("底注:%s元"), AddDecimal(m_lCellScore) );
}


// 添加小数点
CString CComplexUI::AddDecimal( LONGLONG lScore ,  bool bComma /*= true*/, bool bPlus /*= false*/)
{
	CString strScore;
	CString strReturn;
	LONGLONG lNumber = lScore;
	if ( lScore < 0 )
		lNumber = -lNumber;

	strScore.Format(TEXT("%I64d"), lNumber);

	//长度
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

// 添加逗号
CString CComplexUI::AddComma( LONGLONG lScore , bool bPlus /*= false*/)
{
	CString strScore;
	CString strReturn;
	LONGLONG lNumber = lScore;
	if ( lScore < 0 )
		lNumber = -lNumber;

	strScore.Format(TEXT("%I64d"), lNumber);

	//长度
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