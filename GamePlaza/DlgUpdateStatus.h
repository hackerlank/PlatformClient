#ifndef DLG_UPDATE_STATUS_HEAD_FILE
#define DLG_UPDATE_STATUS_HEAD_FILE


//�ӿڶ���
interface IUpdateStatusSink
{
	//�����¼�
public:
	//�������
	virtual VOID OnEventDownLoadTaskFinish()=NULL;
	//������
	virtual VOID OnEventUpdateCheckFinish(bool bNeedUpdate)=NULL;	
};


#endif