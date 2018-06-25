#include "TradeManager.h"
//#include <boost/filesystem.hpp>
//ȥ��boost��ʹ�ñ�׼C++system
#include <filesystem>

#include "Logger.h"
#include "strutils.h"
#include "NanoMsgService.h"


TradeManager *TradeManager::Instance()
{
	static TradeManager tradeMgr;
	return &tradeMgr;
}

TradeManager::TradeManager()
{
	//��־��ʼ��
	m_pConfig = new CConfigImpl();

	
}


TradeManager::~TradeManager()
{
	if (0 != m_pConfig)
	{
		delete m_pConfig;
		m_pConfig = nullptr;
	}
}
void TradeManager::Init()
{
	readCfg();
	InitCTP("","","");
}

void TradeManager::readCfg()
{
	//��ȡ����
	//auto   path   =  std::experimental::filesystem::current_path();//ok
	auto   path   = std::tr2::sys::current_path();

	m_path = path.string() + "\\config.cfg";
	
	InitLogger(m_path);
	
}

void TradeManager::InitLogger(const string& strPath)
{
	//CRLog(E_DEBUG, "�Ƿ��ĳ����ֶγߴ�: %d", m_PacketInfo.nLengthBytes);//E_APPINFO       E_SYSINFO

	//��ʼ�����ã�������־��ַ
	if (m_pConfig->Load(strPath) != 0)
		return ;


	// ��ʼ����־����Ӧ�Ķ����У����������־�����ò����ڣ���Ҫ��ʼ��
	if (CLogger::Instance()->Initial(m_pConfig->GetProperties("logger")) != 0)
		return ;

	// ������־
	if (CLogger::Instance()->Start() != 0)
		return ;

	CRLog(E_DEBUG, "���ó�ʼ���ɹ�");
	//�����ʼ��
	//�ڲ��߼���ʼ��
	//CTP��ʼ��
	//�ڲ�����·�ɵĳ�ʼ��
	//��ʼ���������ڴ�
	string sTmp;
	unsigned int uiXQueNum = 2;
	if (0 == m_pConfig->GetProperty("XQUE_NUM", sTmp))
	{
		uiXQueNum = strutils::FromString<unsigned int>(sTmp);
		if (uiXQueNum > 10)
			uiXQueNum = 2;
	}

	for (unsigned int uiIndex = 1; uiIndex <= uiXQueNum; uiIndex++)
	{
		string sCfgName = "XQUE" + strutils::ToString<unsigned int>(uiIndex);

		CConfig *pCfgWriter;
		pCfgWriter = m_pConfig->GetProperties(sCfgName);
		if (0 != pCfgWriter && !pCfgWriter->IsEmpty())
		{
		}
		else
		{
			pCfgWriter = m_pConfig;
		}
		CRLog(E_APPINFO, "��ʼ��[%s]������", sCfgName.c_str());
		CXQueueIo<QUOTATION>* pWriter = new CXQueueIo<QUOTATION>();
		pWriter->Init(pCfgWriter);//�������ļ�������һ��
		m_vecQueueIo.push_back(pWriter);

	}


	//����
	int nCount = 0;
	for (vector< CXQueueIo<QUOTATION>* >::iterator it = m_vecQueueIo.begin(); it != m_vecQueueIo.end(); ++it)
	{
		nCount++;
		if (0 != *it)
		{
			CRLog(E_APPINFO, "����[XQUE%d]", nCount);
			(*it)->Start();
		}
	}


}

/***************************/
string UserID = "087614";
string Pswd   = "ypdxcn";
string Broker = "9999";
string INSTRU = "a1805";
/***************************/
void TradeManager::InitCTP(const char *user, const char*pswd, const char *broker)
{
	MyCTPTrade::Instance()->setMainWidget(this);
	MyCTPQuote::Instance()->setMainWidget(this);

	int  loginNum = 0;
	while (true)
	{
		static int num = 1;//���������ֶ�
		string  strTradeFront;
		string  strQuoteFront;
		string strTradeFiled = "CTPFront.trade";
		string strQuoteFiled = "CTPFront.quote";

		strTradeFiled += strutils::intstr(num % 4);
		strQuoteFiled += strutils::intstr(num % 4);

		num = num == 0 ? 1 : num;

		m_pConfig->GetProperty(strTradeFiled, strTradeFront);
		m_pConfig->GetProperty(strQuoteFiled, strQuoteFront);
		if(!MyCTPTrade::Instance()->m_bLoginSuccessed)
			MyCTPTrade::Instance()->Init(UserID.c_str(), Pswd.c_str(), Broker.c_str(), strTradeFront.c_str());//��ʼ������
		if (!MyCTPQuote::Instance()->m_bLoginSuccessed)
			MyCTPQuote::Instance()->Init(UserID.c_str(), Pswd.c_str(), Broker.c_str(), strQuoteFront.c_str());//��ʼ������
		
		Sleep(7000);
		loginNum++;
		num++;

		if (MyCTPTrade::Instance()->m_bLoginSuccessed && MyCTPQuote::Instance()->m_bLoginSuccessed)
		{
			CRLog(E_APPINFO, "��¼CTP�ɹ�����½����[%d]���ɹ���������", loginNum);

			MyCTPQuote::Instance()->Subscribe(this);
			break;
		}

		if (loginNum > 20)
			break;
    }

	//MyCTPTrade::Instance()->Init(UserID.c_str(), Pswd.c_str(), Broker.c_str(), (char *)"tcp://180.168.146.187:10001");//��ʼ������
	//MyCTPQuote::Instance()->Init(UserID.c_str(), Pswd.c_str(), Broker.c_str(), (char *)"tcp://180.168.146.187:10011");//��ʼ������
																													  
    //����ģ��   ----ok    �����գ�16��00������09��00���ǽ����գ�16��00������15��00��
	//MyCTPTrade::Instance()->Init(UserID.c_str(), Pswd.c_str(), Broker.c_str(), (char *)"tcp://180.168.146.187:10030");//��ʼ������
	//MyCTPQuote::Instance()->Init(UserID.c_str(), Pswd.c_str(), Broker.c_str(), (char *)"tcp://180.168.146.187:10031");//��ʼ������
	
}

void TradeManager::setLog(const string &strLog)
{
	CRLog(E_DEBUG, strLog.c_str());
}


void TradeManager::setQuotation(const CThostFtdcDepthMarketDataField &pDepthMarketData)
{

}

QUOTATION quo = { 0 };
void TradeManager::Receive(CBroadcastPacket &pkt)
{
	//����nanomsg����
	unsigned int  nLen = 0;
	const char* pcBuf = pkt.Encode(nLen);

	//�ṩʵʱ����
	NanoMsgService::Instance()->send(pcBuf, nLen);

	//�ַ�����Ӧ���д���
	quo.Decode(pcBuf, nLen);

	int uiCount = 0;
	string sXQueName, sTmp;
	for (vector< CXQueueIo<QUOTATION>* >::iterator it = m_vecQueueIo.begin(); it != m_vecQueueIo.end(); ++it)
	{
		uiCount++;
		if (0 != *it)
		{
			(*it)->Enque(quo);

			sXQueName = "XQUE" + strutils::ToString<unsigned int>(uiCount);
			sXQueName += ".XQUE_NAME";

			if (0 == m_pConfig->GetProperty(sXQueName, sTmp))
				sXQueName = sTmp;

			CRLog(E_APPINFO, "�������XQueue[%s]д���", sXQueName.c_str());
		}

	}//end for

	//Sleep(10); // 1 s
}