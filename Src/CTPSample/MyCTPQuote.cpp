#include "MyCTPQuote.h"
#include "MyCTPTrade.h"
#include <atomic>
#include "BroadcastPacket.h"

#include <filesystem>

MyCTPQuote::MyCTPQuote( )
{
	m_RecvSpi = new MyRecv();
	m_RecvSpi->Bind(this);

	requestID = 0;
	m_userLoginInfo = nullptr;

	m_bLoginSuccessed = false;
	//��־��ʼ��
	//m_pConfig = new CConfigImpl();
}

MyCTPQuote::~MyCTPQuote()
{
	m_reqApi->Join();//2-180416

	if (m_userLoginInfo != nullptr)
		delete m_userLoginInfo;

	if (m_RecvSpi != nullptr)
		delete m_RecvSpi;

	//if (0 != m_pConfig)
	//{
	//	delete m_pConfig;
	//	m_pConfig = nullptr;
	//}
}

void MyCTPQuote::setLog(const string&  str)
{
	CRLog(E_DEBUG, str.c_str());
}

void    MyCTPQuote::setMainWidget(void *widget)
{
	mainHandle = widget;
}

  MyCTPQuote *MyCTPQuote::Instance()
{
	static MyCTPQuote  quote;
	return  &quote;
}
///1.�ر�-���ӳɹ�
void MyCTPQuote::OnFrontConnected()
{
	//ctpInit
	
	++requestID;
	m_reqApi->ReqUserLogin(m_userLoginInfo, requestID);

	setLog("Quote SDK: OnFrontConnected();");

}
///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void MyCTPQuote::OnFrontDisconnected(int nReason)
{
	setLog("Trade SDK: OnFrontDisconnected();");
	m_bLoginSuccessed = false;

}

///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void MyCTPQuote::OnHeartBeatWarning(int nTimeLapse)
{
	setLog("Trade SDK: OnHeartBeatWarning();");

}

///��¼������Ӧ
void MyCTPQuote::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	string str  = ("Quote SDK: OnRspUserLogin();");


	if ( pRspInfo->ErrorID == 0)		//��¼�ɹ�����������鶩��
	{
		m_TradingDay = pRspUserLogin->TradingDay;
		m_session = pRspUserLogin->SessionID;
		m_frontId  = pRspUserLogin->FrontID;
		OrderRef = pRspUserLogin->MaxOrderRef;

		int Qry   = atomic_exchange(&MyCTPTrade::Instance()->m_bQryIns, false);
		int QryOK = atomic_exchange(&MyCTPTrade::Instance()->m_bQryInsOK, true);

		if(!Qry && QryOK)
		  FinishQryInstrument();

		str += " Login Successful! ";
		setLog(str);

		m_bLoginSuccessed = true;
	}
	else
	{
		string strTmp = str + " Error msg : ";
		strTmp += pRspInfo->ErrorMsg;
		setLog(pRspInfo->ErrorMsg);
	}

}

///�ǳ�������Ӧ
void MyCTPQuote::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

///����Ӧ��
void MyCTPQuote::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

///��������Ӧ��
void MyCTPQuote::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


}

///ȡ����������Ӧ��
void MyCTPQuote::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

///����ѯ��Ӧ��
void MyCTPQuote::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

///ȡ������ѯ��Ӧ��
void MyCTPQuote::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}


CBroadcastPacket   pack;
///�������֪ͨ
///ÿ����������
void MyCTPQuote::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if (pDepthMarketData->OpenPrice >= LDBL_MAX)
		pDepthMarketData->OpenPrice = 0;

	if (pDepthMarketData->HighestPrice >= LDBL_MAX)
		pDepthMarketData->HighestPrice = 0;

	if (pDepthMarketData->LowestPrice >= LDBL_MAX)
		pDepthMarketData->LowestPrice = 0;

	if (pDepthMarketData->ClosePrice >= LDBL_MAX)
		pDepthMarketData->ClosePrice = 0;

	if (pDepthMarketData->PreClosePrice >= LDBL_MAX)
		pDepthMarketData->PreClosePrice = 0;

	string str = "Quote SDK ����������Ϣ:InstrumentID:";
	str  += pDepthMarketData->InstrumentID ;
	str += " TradingDay:";   str += pDepthMarketData->TradingDay;
	str += " LastPrice:"; str += to_string(pDepthMarketData->LastPrice);
	str += " Volume:"; str += to_string(pDepthMarketData->Volume);
	str += " OpenPrice:"; str += to_string(pDepthMarketData->OpenPrice);
	str += " HighestPrice:"; str += to_string(pDepthMarketData->HighestPrice);
	str += " LowestPrice:";  str += to_string(pDepthMarketData->LowestPrice);
	str += " ClosePrice:"; str += to_string(pDepthMarketData->ClosePrice);
	str += " PreClosePrice:"; str += to_string(pDepthMarketData->PreClosePrice);
	str += " UpdateTime:"; str += pDepthMarketData->UpdateTime;
	CRLog(E_APPINFO, str.c_str());
	//���ر���
	//ת��������
	//CThostFtdcDepthMarketDataField  DepthMarketData;
	//memcpy_s(&DepthMarketData, sizeof(DepthMarketData) ,pDepthMarketData , sizeof(CThostFtdcDepthMarketDataField));
	//m_mapInstrumentQuote[pDepthMarketData->InstrumentID] = DepthMarketData;
	m_mapInstrumentQuote[pDepthMarketData->InstrumentID] = *pDepthMarketData;

	pack.SetParameter("InstrumentId", pDepthMarketData->InstrumentID);
	pack.SetParameter("TradingDay", pDepthMarketData->TradingDay);
	pack.SetParameter("OpenPrice", pDepthMarketData->OpenPrice);
	pack.SetParameter("HighestPrice", pDepthMarketData->HighestPrice);
	pack.SetParameter("LowestPrice", pDepthMarketData->LowestPrice);
	pack.SetParameter("ClosePrice", pDepthMarketData->ClosePrice);
	pack.SetParameter("PreClosePrice", pDepthMarketData->PreClosePrice);
	pack.SetParameter("UpperLimitPrice", pDepthMarketData->UpperLimitPrice);
	pack.SetParameter("LowerLimitPrice", pDepthMarketData->LowerLimitPrice);
	pack.SetParameter("AskPrice1", pDepthMarketData->AskPrice1);
	pack.SetParameter("AskVolume1", pDepthMarketData->AskVolume1);
	pack.SetParameter("BidPrice1", pDepthMarketData->BidPrice1);
	pack.SetParameter("BidVolume1", pDepthMarketData->BidVolume1);
	pack.SetParameter("LastPrice", pDepthMarketData->LastPrice);
	pack.SetParameter("Volume", pDepthMarketData->Volume);


	//���ϲ�ת��
	// �㲥����
	for (VPKTRECEIVER::iterator it = m_vPketReceiver.begin(); it < m_vPketReceiver.end(); it++)
	{
		(*it)->Receive(pack);
	}

	//((TradeManager *)mainHandle)->setQuotation(DepthMarketData);

	//QParamEvent * msgQuote = new QParamEvent(WM_RECV_QUOTATION);
	//msgQuote->setQuotation(pDepthMarketData);
	//QApplication::postEvent(mainwiget, msgQuote);

}

///ѯ��֪ͨ
void MyCTPQuote::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{

}


/////////////////////////////////////////////////////////////////////////
void MyCTPQuote::Init(const char  *User,const char  *pswd ,const char  *broker, const char *pszAddress)
{
	if(m_userLoginInfo == nullptr)
		m_userLoginInfo = new CThostFtdcReqUserLoginField();

	strcpy(m_userLoginInfo->BrokerID, broker);
	strcpy(m_userLoginInfo->UserID, User);
	strcpy(m_userLoginInfo->Password, pswd);

	//��ǰ·��
	std::tr2::sys::path path = std::tr2::sys::current_path();
	path.append("/CTP/MdCon");
	//ʵ����
	m_reqApi = CThostFtdcMdApi::CreateFtdcMdApi(path.string().c_str());
	//ע���¼�
	m_reqApi->RegisterSpi(m_RecvSpi);
	//ע��ǰ�û�
	m_reqApi->RegisterFront((char *)pszAddress);// (char *)"tcp://27.17.62.149:40205");	//ģ��

	//ע��ǰ�û�
	m_reqApi->RegisterFront((char *)pszAddress);//�ٴ����У��л���ַ

	m_reqApi->Init();
	//m_reqApi->Join();

	CRLog(E_DEBUG, "CTP����ģ���ʼ���ɹ�");

}

#define MAX_INSTRUMENT_NUM 1024
void  MyCTPQuote::FinishQryInstrument()
{
	// ��ʼ��������
	// ��ѯ����,����������
	char *ppInstrumentID[MAX_INSTRUMENT_NUM] = { 0 };

	lock_guard<mutex> lock(m_mutex);

	m_bQryInstrumentOK = true;	//��ѯȫ����Լ�ɹ���ʶ
	//����
	m_mapInstrument = MyCTPTrade::Instance()->GetMapInstrument();

	// ����������������
	while (m_mapInstrument.size() > MAX_INSTRUMENT_NUM)
	{
		m_mapInstrument.erase(m_mapInstrument.end());
	}

	int nInstrumentCout = 0;
	//auto mapInstrument2 = MyCTPTrader::GetInstance()->GetMapInstrument2();
	//CSampleManager::GetInstance()->CalcDYXY(mapInstrument2);


	for (auto it = m_mapInstrument.begin(); it != m_mapInstrument.end(); it++)
	{
		ppInstrumentID[nInstrumentCout++] = (char*)(it->first.c_str());
	}

	//wstring wstrInfo = CStringUtils::Format(L"�յ�CTP�����Ϣ���г�����%d����Լ����%d", (int)mapInstrument2.size(), nInstrumentCout);
	//ͳ�ƣ�����
	{
		//�յ�����ʱ������Ϊ0
		//COwner lock(m_mutex);
		//m_tmLastQuote = 0;
	}

	//ReadData();

	// ��������
	SubscribeMarketData(ppInstrumentID, nInstrumentCout);

	setLog("Quote SDK: FinishQryInstrument()  and SubscribeMarketData();");
	
}


void MyCTPQuote::Subscribe(CPacketReceiver *pPacketReceiver)
{
	VPKTRECEIVER::iterator it = find(m_vPketReceiver.begin(), m_vPketReceiver.end(), pPacketReceiver);
	if (it == m_vPketReceiver.end())
	{
		m_vPketReceiver.push_back(pPacketReceiver);
	}
}

void MyCTPQuote::Unsubscribe(CPacketReceiver *pPacketReceiver)
{
	VPKTRECEIVER::iterator it = find(m_vPketReceiver.begin(), m_vPketReceiver.end(), pPacketReceiver);
	if (it != m_vPketReceiver.end())
	{
		m_vPketReceiver.erase(it);
	}
}