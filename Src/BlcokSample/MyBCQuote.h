#pragma once

#include "BlcokSample.h"
#include <vector>
#include <thread>
#include <map>
#include <mutex>

#include "BroadcastPacket.h"
#include "ConfigImpl.h"








//����һ���������������������
class   BLCOKSAMPLE_EXPORT  MyBCQuote
{
public:
	MyBCQuote( );
	~MyBCQuote();

	void    setMainWidget(void *widget);
	//// ���Ĺ㲥����
	//void Subscribe(CPacketReceiver *pPacketReceiver);

	//// ȡ���㲥���Ķ���
	//void Unsubscribe(CPacketReceiver *pPacketReceiver);


	static  MyBCQuote *Instance();

	void   getQuote(const char *pszAddress);

	void   setLog(const string&  str);
	bool   m_bLoginSuccessed;

	//void    setGetParam(string &uri);
	//void    getMarketlist(string &uri);
private:

	vector<string /*InstrumentID*/> m_vecInstrumentId;//��ԼID
	int         requestID;
	string   m_TradingDay;

	

	mutex m_mutex;
	///�û���¼��Ϣ

	void  *mainHandle;

	//���ڸ��ϲ㶩��
	//typedef vector<CPacketReceiver*> VPKTRECEIVER;
	//VPKTRECEIVER            m_vPketReceiver;



	//CConfigImpl*	 m_pConfig;


	//web get
	 CConfigImpl  *m_pConfig;

};
