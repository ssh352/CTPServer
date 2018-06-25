#include "MyCTPTrade.h"
#include "MyCTPQuote.h"


#include "strutils.h"
#include <filesystem>
//#include <string>

MyCTPTrade::MyCTPTrade()
{
	m_RecvSpi = new MyRecv();
	m_RecvSpi->Bind(this);

	m_userLoginInfo = nullptr;
	m_bQryIns       = false;// �Ƿ����ڲ�ѯ��Լ��������ڲ�ѯ���������Ͳ��Լ�����ȥ�������飬��Ϊ��ѯ��Լ��ɺ󣬻��ж�������Ķ���
	m_bQryInsOK     = false;// �Ƿ��ѯ��Լ�ɹ����ɹ���������Ķ���

	m_reqApi        = nullptr;
	m_bLoginSuccessed = false;

	//��־��ʼ��
	m_pConfig = new CConfigImpl();

}

MyCTPTrade::~MyCTPTrade()
{
	m_reqApi->Join();

	if (m_userLoginInfo != nullptr)
		delete m_userLoginInfo;

	if (m_RecvSpi != nullptr)
		delete m_RecvSpi;

	if (0 != m_pConfig)
	{
		delete m_pConfig;
		m_pConfig = nullptr;
	}

}

void    MyCTPTrade::setMainWidget(void *widget)
{
	mainHandle = widget;
}

void MyCTPTrade::setLog(const string&  str)
{
	CRLog(E_DEBUG, str.c_str());
}
///1.�ر�-���ӳɹ�
void MyCTPTrade::OnFrontConnected()
{
	m_reqApi->ReqUserLogin(m_userLoginInfo,m_RecvSpi->GetRequestId());


	setLog("Trade SDK: OnFrontConnected();");
}
///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void MyCTPTrade::OnFrontDisconnected(int nReason)
{

	setLog("Trade SDK: OnFrontDisconnected();");

}

///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void MyCTPTrade::OnHeartBeatWarning(int nTimeLapse)
{

	setLog("Trade SDK: OnHeartBeatWarning();");

}

///��¼������Ӧ
void MyCTPTrade::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	string  str = ("Trade SDK: OnRspUserLogin();");

	if ( pRspInfo->ErrorID == 0)		//��¼�ɹ�����������鶩��
	{
		m_TradingDay = pRspUserLogin->TradingDay;
		m_session = pRspUserLogin->SessionID;
		m_frontId  = pRspUserLogin->FrontID;
		m_serverOrderRef = pRspUserLogin->MaxOrderRef;

		///�����ѯ���к�Լ
		CThostFtdcQryInstrumentField req;
		memset(&req, 0, sizeof(req));
		strcpy_s(req.InstrumentID, "");//Ϊ�ձ�ʾ��ѯ���к�Լ


		// ��ѯ��Լ����һ������Ϊnull����ʾ��ѯ���еĺ�Լ
		if(m_reqApi != nullptr)
			m_reqApi->ReqQryInstrument(&req, m_RecvSpi->GetRequestId());

		m_bLoginSuccessed = true;
		setLog(str + " Login Successful ");
	}
	else
	{
		string strTmp = str + " Error msg : ";
		strTmp  +=  pRspInfo->ErrorMsg;
		setLog(strTmp);
	}

}

///fanhui ��Լ��Ϣ
void MyCTPTrade::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	string strLog("Trade SDK: OnRspQryInstrument()");

	lock_guard<mutex> lock(m_getInstru);
	if (pRspInfo && pRspInfo->ErrorID)
	{
		atomic_exchange(&m_bQryIns, false);
		//����ʧ��
	}
	else if (pInstrument)
	{
		// �����Լ
		CThostFtdcInstrumentField instrument;
		memcpy_s(&instrument, sizeof(CThostFtdcInstrumentField), pInstrument, sizeof(CThostFtdcInstrumentField));

		// ���˵���Ч����efp��sp*��IF��
		string strTmp(instrument.InstrumentID);

		
		int emitFlag = 0;
		string str = instrument.InstrumentID;

		if (strutils::trim(strTmp).size() >= 9) //��ʱ��ȥ��һЩ���Ʒ�� �� SR811P6300
			emitFlag = 1;

		if (strTmp.compare("EFP") == 0 || strTmp.compare("SP") == 0 || strTmp.compare("IPS") == 0 || emitFlag == 1)
		{
			//��־��¼
			setLog(strLog + "�յ���Ч��Լ");
		}
		else
		{
			if (strlen(pInstrument->InstrumentID) > 0)
			{
				m_mapInstrument[pInstrument->InstrumentID] = instrument;

				if (strlen(pInstrument->ExchangeID) > 0)
				{
					string	strSecurityId = pInstrument->InstrumentID;
					//wstring	wstrBlockId = GetContrPrefix(strSecurityId);
					//m_mapInstrument2[CA2T(pInstrument->ExchangeID).m_psz][wstrBlockId][CA2T(pInstrument->InstrumentID).m_psz] = instrument;
				}


			}
			
			//��־��¼
			setLog(strLog + "Instrument ID :"+strTmp);

		}

		// ��ѯ���֪ͨ�������
		if (bIsLast)
		{
			atomic_exchange(&m_bQryInsOK, true);
			atomic_exchange(&m_bQryIns, false);

			//wstring strLog = CStringUtils::Format(_T("%s��Լ����(%d)"), LOG_CTP_RECV_INSTRUMENT_OK, m_mapInstrument.size());
			string strNum = strutils::intstr(m_mapInstrument.size());
			setLog(strLog + "Instrument num Totols :" + strNum);

			MyCTPQuote::Instance()->FinishQryInstrument();
		}
	}
	else
	{
		//_WRITELOG(_T("CTP���ף�OnRspQryInvestor�յ���������\r\n"));
	}

}

///�ǳ�������Ӧ
void MyCTPTrade::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void MyCTPTrade::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//
}

void  MyCTPTrade::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	setLog("Trade SDK: OnRspError();");
}



MyCTPTrade * MyCTPTrade::Instance()
{
	static  MyCTPTrade  trade;
	return  &trade;
}
/////////////////////////////////////////////////////////////////////////
void MyCTPTrade::Init(const char  *User,const char  *pswd ,const char  *broker, const char *pszAddress)
{
	m_bQryIns   = false;// �Ƿ����ڲ�ѯ��Լ��������ڲ�ѯ���������Ͳ��Լ�����ȥ�������飬��Ϊ��ѯ��Լ��ɺ󣬻��ж�������Ķ���
	m_bQryInsOK = false;// �Ƿ��ѯ��Լ�ɹ����ɹ���������Ķ���


	if(m_userLoginInfo == nullptr)
		m_userLoginInfo = new CThostFtdcReqUserLoginField();

	strcpy(m_userLoginInfo->BrokerID, broker);
	strcpy(m_userLoginInfo->UserID, User);
	strcpy(m_userLoginInfo->Password, pswd);

	//��ǰ·��
	std::tr2::sys::path path1  = std::tr2::sys::current_path();
	path1.append("/CTP/TradeCon");
	//ʵ����
	m_reqApi  = CThostFtdcTraderApi::CreateFtdcTraderApi(path1.string().c_str());
	//ע���¼�
	m_reqApi->RegisterSpi(m_RecvSpi);
	//ע��ǰ�û�
	m_reqApi->RegisterFront((char *)pszAddress);// (char *)"tcp://27.17.62.149:40205");	//ģ��
	m_reqApi->SubscribePrivateTopic(THOST_TERT_QUICK);
	m_reqApi->SubscribePublicTopic(THOST_TERT_QUICK);
	m_reqApi->Init();
	//m_reqApi->Join();

	//��ʼ�����ã�������־��ַ
	auto   path = std::tr2::sys::current_path();
	if (m_pConfig->Load(path.string() + "\\config.cfg") != 0)
		return;

	// ��ʼ����־����Ӧ�Ķ����У����������־�����ò����ڣ���Ҫ��ʼ��
	if (CLogger::Instance()->Initial(m_pConfig->GetProperties("logger")) != 0)
		return;

	// ������־
	if (CLogger::Instance()->Start() != 0)
		return;

	CRLog(E_DEBUG, "CTP����ģ���ʼ���ɹ�");
}


map<string, CThostFtdcInstrumentField>&	 MyCTPTrade::GetMapInstrument()
{
	//lock_guard<mutex> lock(m_getInstru);
	return m_mapInstrument;
}