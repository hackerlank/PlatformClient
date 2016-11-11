#ifndef DLG_UPDATE_STATUS_HEAD_FILE
#define DLG_UPDATE_STATUS_HEAD_FILE


//接口定义
interface IUpdateStatusSink
{
	//更新事件
public:
	//下载完成
	virtual VOID OnEventDownLoadTaskFinish()=NULL;
	//检测完成
	virtual VOID OnEventUpdateCheckFinish(bool bNeedUpdate)=NULL;	
};


#endif