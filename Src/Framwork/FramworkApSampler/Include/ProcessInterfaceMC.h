//Э������ ���������½ӿڿͻ�������:
//���̻�����֮���M�ӿ�
#ifndef _PROCESS_INTERFACE_MC_H
#define _PROCESS_INTERFACE_MC_H

#include "CommAp.h"
#include "CommApSampler.h"
#include "ProcessInterfaceNm.h"
#include "IpcPacket.h"

using namespace std;

class COMMAPSAMPLER_CLASS CProcessInterfaceMC : public CProcessInterfaceClnNm
{

public:
	CProcessInterfaceMC(void);
	~CProcessInterfaceMC(void);

	typedef struct tagGessPktInfo
	{
		unsigned long ulIdleInterval;		//����ʱ��������Hello���ʱ�䣬Ҳ����·�ϵ������ʱ��
		unsigned long ulIntervalReSend;		//��Ӧ����ؼ����
		unsigned long ulHelloReSend;		//��Ӧ����ط������Ĵ���
		bool blNeedLogin;					//�Ƿ���Ҫ��¼

		std::string	node_type;
		std::string	node_id;
		std::string	node_name;
		std::string	host_id;
		std::string	sUserName;				//���ͻ�ʱ�����˵�¼�û���
		std::string	sPassword;				//���ͻ�ʱ�����˵�¼����
	} GessPktInfo,*PGessPktInfo;

	int Init(CConfig * pCfg);

	virtual int OnConnect();
	void OnClose();
	int GetNmKey(string& sKey);
protected:
	//���ඨ��Ļص�����ʵ��
	virtual int OnPacket(char * pData, size_t nSize);
	virtual void GetPacketInfo(PacketInfo & stInfo);//���ĸ�ʽ��Ϣ
	virtual int HandleTimeout(unsigned long& ulTmSpan);
private:
	int SendHello();
	int OnHello(CIpcPacket& pktHello);

	static GessPktInfo m_GessPktInfo;
	static bool	m_blGessPktInfoInited;
	
	CConfig *	m_pCfg;
	//std::deque<CIpcPacket> m_qSend;
	CGessMutex m_csMC;

	unsigned long m_uiCountNoAlive;			//��ʱ�������Ĵ���
	unsigned long m_uiCountSended;			//��Ӧ����ѷ�����������
};
#endif