#ifndef GLOBAL_UNITES_HEAD_FILE
#define GLOBAL_UNITES_HEAD_FILE


#pragma once
//////////////////////////////////////////////////////////////////////////////////


//全局单元
class CGlobalUnits
{
	//版本信息
protected:
	DWORD							m_dwPlazaVersion;					//大厅版本
	DWORD							m_dwFrameVersion;					//框架版本

	//追踪变量
protected:
	WORD							m_wTrackServerID;					//追踪房间
	DWORD							m_dwTrackUserID;					//追踪标识

	//静态变量
protected:
	static CGlobalUnits *			m_pGlobalUnits;						//对象指针

	//函数定义
public:
	//构造函数
	CGlobalUnits();
	//析构函数
	virtual ~CGlobalUnits();

	//版本信息
public:
	//大厅版本
	virtual DWORD GetPlazaVersion() { return m_dwPlazaVersion; }
	//框架版本
	virtual DWORD GetFrameVersion() { return m_dwFrameVersion; }

	//声音函数
public:
	//播放声音
	bool PlayGameSound(LPCTSTR pszSoundName);
	//播放声音
	bool PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName);

	//追踪函数
public:
	//获取用户
	DWORD GetTrackUserID();
	//获取房间
	WORD GetTrackServerID();
	//设置标识
	VOID SetTrackUserID(DWORD dwTrackUserID) { m_dwTrackUserID=dwTrackUserID;}
	//设置标识
	VOID SetTrackServerID(WORD wTrackServerID) { m_wTrackServerID=wTrackServerID;}

	//功能函数
public:
	//下载游戏
	bool DownLoadClient(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);

	//静态函数
public:
	//获取对象
	static CGlobalUnits * GetInstance() { return m_pGlobalUnits; }
};


//////////////////////////////////////////////////////////////////////////////////

#endif