//Э������ ������ 
#ifndef _PROCESS_INTERFACE_ZC_H
#define _PROCESS_INTERFACE_ZC_H

#include "CommApSampler.h"
#include "ProcessInterfaceNm.h"
#include "SamplerPacket.h"
#include "ApConstant.h"

using namespace std;

class COMMAPSAMPLER_CLASS CProcessInterfaceZC : public CProcessInterfaceClnNm
{
public:
	CProcessInterfaceZC(void);
	~CProcessInterfaceZC(void);

	typedef struct tagGessPktInfo
	{
		unsigned long ulIdleInterval;		//����ʱ��������Hello���ʱ�䣬Ҳ����·�ϵ������ʱ��
		unsigned long ulIntervalReSend;		//��Ӧ����ؼ����
		unsigned long ulHelloReSend;		//��Ӧ����ط������Ĵ���
							
		int	nNeedCheckServicePkt;			//�Ƿ���Ҫ���ҵ���ĵ�����
		std::string	node_type;
		std::string	node_id;
		std::string	node_name;
		std::string	host_id;
		std::string	sUserName;				//���ͻ�ʱ�����˵�¼�û���
		std::string	sPassword;				//���ͻ�ʱ�����˵�¼����

		bool blNeedLogin;					//�Ƿ���Ҫ��¼	
	} GessPktInfo,*PGessPktInfo;

	int Init(CConfig * pCfg);
	int SendPacket(CPacket & packet);

	virtual int OnConnect();		// ���ͻ������ӳɹ���ص�
	virtual void OnClose();	        // �Ͽ�����ʱ����
	int GetNmKey(string& sKey);
protected:
	//���ඨ��Ļص�����ʵ��
	virtual int OnPacket(char * pData, size_t nSize);
	virtual void GetPacketInfo(PacketInfo & stInfo);//���ĸ�ʽ��Ϣ
	int HandleTimeout(unsigned long& ulTmSpan);

	virtual bool IsLogin() {return m_blIsLogin;} //��¼״̬

	//Э�鶨��Ľ�������
	virtual int SendHello();
	virtual int OnHello(CSamplerPacket & GessPacket);
	int OnLoginRsp(CSamplerPacket & oPacket);

	unsigned int ulSeqNo;
private:
	GessPktInfo m_GessPktInfo;
	bool		m_blIsLogin;	//��¼״̬
	string m_subscripType;        //��¼����ʱ����ģʽ��־ 0-������1-���⣬*-����,����
	string m_subscripItem;
	
	CConfig *	m_pCfg;
	CGessMutex m_csZC;

	unsigned long m_uiCountNoAlive;			//��ʱ�������Ĵ���
	unsigned long m_uiCountSended;			//��Ӧ����ѷ�����������
	unsigned long m_uiCountNoQuote;			//��ʱ����ʵʱ���۴���  added by Ben 20110429

	typedef std::pair<CGessTime,CGessTime> PairInterval;
	typedef std::map<int,std::vector<PairInterval>> MapCheckWeek;
	MapCheckWeek	m_mapCheckWeek;
	void HandleCheckTime(CConfig * pCfg);
	bool IsCheckTime();
};
#endif