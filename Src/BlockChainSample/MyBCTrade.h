#pragma once

#include "BCbase.h"
#include <vector>
#include <thread>
#include <map>
#include <atomic>
#include <mutex>
#include "ConfigImpl.h"

using namespace std;


//����һ���������������������
class  BCSAMPLE_EXPORT  MyBCTrade
{

	//thread
public:
	MyBCTrade();
	~MyBCTrade();

	static  MyBCTrade *Instance();
	
	void    setMainWidget(void *widget);
	void    Init(const char  *User, const  char  *pswd, const  char  *broker, const char *pszAddress);
	

	void     setLog(const string&  str);

	bool      m_bLoginSuccessed;
public:

	string   m_TradingDay;

	int    m_session ;
	int    m_frontId ;
	string  m_serverOrderRef ;

    int		m_nOrderRef;


	mutex   m_getInstru;
	void    *mainHandle;
	// �����Լ
	//map<string, CThostFtdcInstrumentField>					m_mapInstrument;//��ԼID����Լ��Ϣ
	//map<wstring, map<wstring, map<wstring, CThostFtdcInstrumentField>>>	m_mapInstrument2;//����������飬��Լ��Ϣ

	

	CConfigImpl*	 m_pConfig;
};
