#include "MyBCTrade.h"
#include "MyBCQuote.h"


#include "strutils.h"
#include <filesystem>
//#include <string>

MyBCTrade::MyBCTrade()
{
	m_bLoginSuccessed = false;

	//��־��ʼ��
	m_pConfig = new CConfigImpl();
}

MyBCTrade::~MyBCTrade()
{

	if (0 != m_pConfig)
	{
		delete m_pConfig;
		m_pConfig = nullptr;
	}

}

void    MyBCTrade::setMainWidget(void *widget)
{
	mainHandle = widget;
}

void MyBCTrade::setLog(const string&  str)
{
	CRLog(E_DEBUG, str.c_str());
}


MyBCTrade * MyBCTrade::Instance()
{
	static  MyBCTrade  trade;
	return  &trade;
}
/////////////////////////////////////////////////////////////////////////
void MyBCTrade::Init(const char  *User,const char  *pswd ,const char  *broker, const char *pszAddress)
{
	//m_bQryIns   = false;// �Ƿ����ڲ�ѯ��Լ��������ڲ�ѯ���������Ͳ��Լ�����ȥ�������飬��Ϊ��ѯ��Լ��ɺ󣬻��ж�������Ķ���
	//m_bQryInsOK = false;// �Ƿ��ѯ��Լ�ɹ����ɹ���������Ķ���


	//if(m_userLoginInfo == nullptr)
	//	m_userLoginInfo = new CThostFtdcReqUserLoginField();

	//strcpy(m_userLoginInfo->BrokerID, broker);
	//strcpy(m_userLoginInfo->UserID, User);
	//strcpy(m_userLoginInfo->Password, pswd);

	////��ǰ·��
	//std::tr2::sys::path path1  = std::tr2::sys::current_path();
	//path1.append("/CTP/TradeCon");
	////ʵ����
	//m_reqApi  = CThostFtdcTraderApi::CreateFtdcTraderApi(path1.string().c_str());
	////ע���¼�
	//m_reqApi->RegisterSpi(m_RecvSpi);
	////ע��ǰ�û�
	//m_reqApi->RegisterFront((char *)pszAddress);// (char *)"tcp://27.17.62.149:40205");	//ģ��
	//m_reqApi->SubscribePrivateTopic(THOST_TERT_QUICK);
	//m_reqApi->SubscribePublicTopic(THOST_TERT_QUICK);
	//m_reqApi->Init();
	////m_reqApi->Join();

	////��ʼ�����ã�������־��ַ
	//auto   path = std::tr2::sys::current_path();
	//if (m_pConfig->Load(path.string() + "\\config.cfg") != 0)
	//	return;

	//// ��ʼ����־����Ӧ�Ķ����У����������־�����ò����ڣ���Ҫ��ʼ��
	//if (CLogger::Instance()->Initial(m_pConfig->GetProperties("logger")) != 0)
	//	return;

	//// ������־
	//if (CLogger::Instance()->Start() != 0)
	//	return;

	//CRLog(E_DEBUG, "CTP����ģ���ʼ���ɹ�");
}


