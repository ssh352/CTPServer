//Э������ ������ 
#ifndef _PROCESS_INTERFACE_YC_H
#define _PROCESS_INTERFACE_YC_H

#include "CommApSampler.h"
#include "ProcessInterfaceNm.h"
#include "BroadcastPacket.h"

using namespace std;

class COMMAPSAMPLER_CLASS CProcessInterfaceYC : public CProcessInterfaceClnNm
{
public:
	CProcessInterfaceYC(void);
	~CProcessInterfaceYC(void);

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

		std::string	term_type;	//
		std::string	user_key;
		std::string	user_type;
		std::string	user_id;
		std::string	user_pwd;
		std::string	branch_id;
		std::string	again_flag;
	} GessPktInfo,*PGessPktInfo;

	int Init(CConfig * pCfg);

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
	virtual int OnHello(CBroadcastPacket & GessPacket);
	virtual int SendLogin();
    virtual int OnRspLogin(CBroadcastPacket & GessPacket);
private:
	GessPktInfo m_GessPktInfo;
	bool		m_blIsLogin;	//��¼״̬
	
	CConfig *	m_pCfg;
	//std::deque<CBroadcastPacket> m_qSend;
	CGessMutex m_csYC;

	unsigned long m_uiCountNoAlive;			//��ʱ�������Ĵ���
	unsigned long m_uiCountSended;			//��Ӧ����ѷ�����������
	unsigned long m_uiCountNoQuote;			//��ʱ����ʵʱ���۴���  added by Ben 20110429
	bool m_bGessMarketActive;               //�Ƿ��ڽ���ʱ��εı�־ added by Ben 20110429
	bool m_bGessInstActive;               
};
#endif