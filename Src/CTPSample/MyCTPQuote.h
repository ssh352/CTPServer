#pragma once

#include "ctpbase.h"
#include <vector>
#include <thread>
#include <map>
#include "CTP\ThostFtdcMdApi.h"
#include <mutex>

using namespace std;

//#include "PairPacket.h"
#include "BroadcastPacket.h"
#include "ConfigImpl.h"

class  CPacketReceiver
{
public:
	virtual void Receive(CBroadcastPacket &pkt) = 0;
};


//����һ���������������������
class   CTPSAMPLE_EXPORT MyCTPQuote
{
	//Ƕ������ر���
	class  MyRecv : public  CThostFtdcMdSpi
	{
	public:
		MyRecv():m_Parent(nullptr) {};
		~MyRecv() {};
		void Bind(MyCTPQuote  *Parent) { m_Parent = Parent; }
	public:
		///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
		virtual void OnFrontConnected() { m_Parent->OnFrontConnected(); }

		///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
		///@param nReason ����ԭ��
		///        0x1001 �����ʧ��
		///        0x1002 ����дʧ��
		///        0x2001 ����������ʱ
		///        0x2002 ��������ʧ��
		///        0x2003 �յ�������
		virtual void OnFrontDisconnected(int nReason) { m_Parent->OnFrontDisconnected(nReason); }

		///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
		///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
		virtual void OnHeartBeatWarning(int nTimeLapse) { m_Parent->OnHeartBeatWarning(nTimeLapse); }

		///��¼������Ӧ
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspUserLogin(pRspUserLogin, pRspInfo, nRequestID, bIsLast); }

		///�ǳ�������Ӧ
		virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspUserLogout(pUserLogout, pRspInfo,  nRequestID,  bIsLast); }

		///����Ӧ��
		virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspError(pRspInfo, nRequestID,  bIsLast); }

		///��������Ӧ��
		virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspSubMarketData(pSpecificInstrument, pRspInfo, nRequestID, bIsLast); }

		///ȡ����������Ӧ��
		virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspUnSubMarketData(pSpecificInstrument, pRspInfo, nRequestID,  bIsLast); }

		///����ѯ��Ӧ��
		virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspSubForQuoteRsp(pSpecificInstrument, pRspInfo,  nRequestID,  bIsLast); }

		///ȡ������ѯ��Ӧ��
		virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspUnSubForQuoteRsp(pSpecificInstrument, pRspInfo,  nRequestID,  bIsLast); }

		///�������֪ͨ
		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) { m_Parent->OnRtnDepthMarketData(pDepthMarketData); }

		///ѯ��֪ͨ
		virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) { m_Parent->OnRtnForQuoteRsp(pForQuoteRsp); }

	public:

		MyCTPQuote  *m_Parent;
	};

	//thread
public:
	MyCTPQuote( );
	~MyCTPQuote();

	void    setMainWidget(void *widget);
	// ���Ĺ㲥����
	void Subscribe(CPacketReceiver *pPacketReceiver);

	// ȡ���㲥���Ķ���
	void Unsubscribe(CPacketReceiver *pPacketReceiver);


	static  MyCTPQuote *Instance();

	void  Init(const char  *User, const  char  *pswd, const  char  *broker, const char *pszAddress);
	void  FinishQryInstrument();

	void  setLog(const string&  str);
	bool      m_bLoginSuccessed;
public:
	///////////////////////////////////////�����ຯ��,�ṩ��Ҫ�߼������ⲿʹ��////////////////////////////////////////////////////////////////////////////
	///�û���¼����
	int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) 
	{ 
		return m_reqApi->ReqUserLogin(pReqUserLoginField, nRequestID	);
	}

	///�ǳ�����
	int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID) { return m_reqApi->ReqUserLogout(pUserLogout, nRequestID); }

	///��ȡ��ǰ������
	///@retrun ��ȡ���Ľ�����
	///@remark ֻ�е�¼�ɹ���,���ܵõ���ȷ�Ľ�����
	const char *GetTradingDay() { return m_reqApi->GetTradingDay(); }
	///�������顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	int SubscribeMarketData(char *ppInstrumentID[], int nCount) { return m_reqApi->SubscribeMarketData(ppInstrumentID, nCount); }

	///�˶����顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	int UnSubscribeMarketData(char *ppInstrumentID[], int nCount) { return m_reqApi->UnSubscribeMarketData(ppInstrumentID, nCount); }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


public:
	///////////////////////////////////////�ر��ຯ�����յ����ݣ����±���������Ϣ////////////////////////////////////////////////////////////////////////////


	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	void OnHeartBeatWarning(int nTimeLapse);

	///��¼������Ӧ
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ǳ�������Ӧ
	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����ѯ��Ӧ��
	void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ������ѯ��Ӧ��
	void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	///ѯ��֪ͨ
	void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	CThostFtdcMdApi* m_reqApi;
	MyRecv         *m_RecvSpi;
	map<string /*InstrumentID*/, CThostFtdcInstrumentField>					   m_mapInstrument;//��ԼID����Լ��Ϣ
	map<string /*InstrumentID*/,CThostFtdcDepthMarketDataField /*Quotation*/>  m_mapInstrumentQuote;//����  ��ԼID-��������  ��,��ԼIDΨһ��
	vector<string /*InstrumentID*/> m_vecInstrumentId;//��ԼID
	int         requestID;
	string   m_TradingDay;

	int    m_session ;
	int    m_frontId ;
	string  OrderRef ;


	mutex m_mutex;
	///�û���¼��Ϣ
	CThostFtdcReqUserLoginField  *m_userLoginInfo;
	bool m_bQryInstrumentOK;

	void  *mainHandle;

	//���ڸ��ϲ㶩��
	typedef vector<CPacketReceiver*> VPKTRECEIVER;
	VPKTRECEIVER            m_vPketReceiver;


	//CConfigImpl*	 m_pConfig;
	
};
