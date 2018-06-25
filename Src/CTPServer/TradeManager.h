#pragma once



/***
 *@����:���׹�����
 *@  ����1:��־����
 *   ����2:���׹���
 *   ����3:�������
 *   ����4:����·�ɹ���
 *   ����5:����������
 *   ����6:��ع�����

 @�滮����ʱͳһ�����𲽷ֲ��������Ϊ������������(OM)����ع�����(RM)�����Թ�����(SM)

 @ʹ�õĻ�������:
 1.Poco c++
 2.Nanomsg
 3.C++11
 4.Boost

***/

#include "..\CTPSample\MyCTPQuote.h"
#include "..\CTPSample\MyCTPTrade.h"
#include "ConfigImpl.h"
#include "Singleton.h"

#include "XQueue.h"
#include "XQueueIO.h"
#include "BroadcastPacket.h"//�ڲ�K-value�㲥��ʽ
#include "samplerpacket.h"//��׼�����ʽ

#include <string>
//template<typename >
//class CSingleton;

class CConfigImpl;

class  TradeManager:public CPacketReceiver
{
	//friend class CSingleton< TradeManager>;
protected:
	TradeManager();
	~TradeManager();

private:
	//�ڲ��߼�
	void readCfg();
	void InitLogger(const string& strPath);

public:
	void  Init();
	static TradeManager *Instance();
	void InitCTP(const char *user ,const char*pswd ,const char *broker);

	void setLog(const string &strLog);
	void setQuotation(const CThostFtdcDepthMarketDataField &pDepthMarketData);
	virtual void Receive(CBroadcastPacket &pkt) override;
private:
	CConfigImpl*	 m_pConfig;
	string			 m_user;
	string			 m_pswd;
	string			 m_broker;
	string           m_path;

	vector< CXQueueIo<QUOTATION>* > m_vecQueueIo;
};

