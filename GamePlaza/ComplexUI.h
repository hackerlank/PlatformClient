#ifndef COMPLEX_UI_HEAD_FILE
#define COMPLEX_UI_HEAD_FILE

///////////////////////////////////////////////////////////////////////////////////////

//xml sample:<SkinPikerPictureItem name="" width="118" height="70" bkimage="UI\BKImage\1small.jpg" bkname="测试" author="Redrain" />  
//类名和接口名，在CreateControl函数中会用到  
const TCHAR kComplexClassName[] = _T("ComplexUI");  
const TCHAR kComplexInterface[] = _T("Complex");  

//////////////////////////////////////////////////////////////////////////////////////

class CComplexUI : public CButtonUI  
{
	//显示参数
protected:
	DWORD			m_dwMaxCount;					//最高人数
	DWORD			m_dwOnLineCount;				//在线人数
	LONGLONG		m_lEnterScore;					//进入人数
	LONGLONG		m_lCellScore;					//进入底注

	//显示变量
protected:
	CStdString		m_szEntryScore;				//进入成绩
	CStdString		m_szBaseScore;				//底注成绩
	CStdString		m_szOnline;					//在线人数

	//重载函数
public:
	//构造函数
    CComplexUI();  
	//析构函数
	~CComplexUI();  

public:
	//获得类名
    LPCTSTR GetClass() const;  
    //接口函数
	LPVOID GetInterface(LPCTSTR pstrName);  
	
	//继承函数
public: 
    //重构属性
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);  
	//重新绘制
    void PaintStatusImage(HDC hDC);

	//功能函数
public: 
	//最大人数
	void SetMaxOnline(DWORD dwMaxCount);
	//最大在线
	void SetOnline(DWORD dwOnLineCount);
	//进入成绩
	void SetEnterScore(LONGLONG lEnterScore);
	//底注成绩
	void SetCellScore(LONGLONG lCellScore);

	//辅助功能
public: 
	//更新信息
	void UpdateInfo();
	// 添加小数点
	CString AddDecimal( LONGLONG lScore ,  bool bComma = true, bool bPlus = false);
	// 添加逗号
	CString AddComma( LONGLONG lScore , bool bPlus = false );
};  



#endif
