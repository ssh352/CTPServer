#include "ProcessInterfaceYC.h"
#include "Logger.h"
#include "strutils.h"
using namespace strutils;

CProcessInterfaceYC::CProcessInterfaceYC()
:m_pCfg(0)
,m_blIsLogin(false)
,m_uiCountNoAlive(0)
,m_uiCountSended(0)
,m_uiCountNoQuote(0)
,m_bGessMarketActive(false)
,m_bGessInstActive(false)
{
	m_GessPktInfo.ulIdleInterval = 5;//DEFAULT_IDLE_TIMEOUT;
	m_GessPktInfo.ulIntervalReSend = DEFAULT_HELLO_RESEND_INTERVAL;
	m_GessPktInfo.ulHelloReSend = DEFAULT_HELLO_RESEND_COUNT;
	m_GessPktInfo.blNeedLogin = false;
}

CProcessInterfaceYC::~CProcessInterfaceYC(void)
{
}

int CProcessInterfaceYC::Init(CConfig * pCfg)
{
	CTcpAppProcessClient::Init(pCfg);

	if (0 != pCfg)
	{
		string sLoginFlag;
		if (0 == pCfg->GetProperty("login",sLoginFlag))
		{
			if (1 == FromString<int>(sLoginFlag))
				m_GessPktInfo.blNeedLogin = true;
		}

		pCfg->GetProperty("user_type",m_GessPktInfo.user_type);
		pCfg->GetProperty("user_id",m_GessPktInfo.user_id);
		pCfg->GetProperty("branch_id",m_GessPktInfo.branch_id);
		pCfg->GetProperty("user_pwd",m_GessPktInfo.user_pwd);
	}

	m_pCfg = pCfg;
	return 0;
}

/******************************************************************************
��������:��ȡЭ�鶨��ı��ĸ�ʽ��Ϣ
���ú���:�������ص�
�������:PacketInfo & stInfo ���ĸ�ʽ��Ϣ
        eLengthType �����ֶ����� ����/10�����ַ���/16�����ַ���
		nLengthBytes ��Ϣ�������ֶεĳ���,Ϊ����ʱֻ֧�� 1 2 4�ֽ�
		nLengthPos �����ֶ���ʼλ��
		blLenIncludeHeader �����Ƿ񺬱���ͷ
		nFixHeadLen  �������ֶ�ָʾ�ĳ�����,���Ĺ̶����ֳ���
������  :��ΰ
��������:2008.07.22
�޸ļ�¼:
******************************************************************************/
void CProcessInterfaceYC::GetPacketInfo(PacketInfo & stInfo)
{
	stInfo.eLengthType = ltCharactersDec;
	stInfo.nLengthPos = 0;
	stInfo.nLengthBytes = BROADCAST_LENGTH_BYTES;
	stInfo.blLenIncludeHeader = false;
	stInfo.nFixHeadLen = stInfo.nLengthPos + stInfo.nLengthBytes;
}

/******************************************************************************
��������:Э�鱨�Ĵ�������Ҫ����,���ݱ��������ֽ���ֱ�Ӵ����ת��
���ú���:�������ص�
��������:OnRecvPacket,�ϴ�����
�������:char * pData���ջ���, size_t nSize�����С
����ֵ  :int �������⴦��
������  :��ΰ
��������:2008.07.22
�޸ļ�¼:
******************************************************************************/
int CProcessInterfaceYC::OnPacket(char * pData, size_t nSize)
{
	CBroadcastPacket GessPacket;
	GessPacket.Decode(pData, nSize);

	m_csYC.Lock();
	m_uiCountNoAlive = 0;
	m_csYC.Unlock();

	std::string sCmdID = GessPacket.GetCmdID();

	if ( sCmdID == "RspLogin" )
	{
		OnRspLogin(GessPacket);
	}
	else if (sCmdID == "ConnectTest")
	{
		return OnHello(GessPacket);
	}
	else if (sCmdID == "ConnectTestRsp")
	{
		return 0;
	}
	else if (sCmdID == "onRecvRtnDeferMarketStateUpdate"
		/*|| sCmdID == "onRecvRtnSpotMarketStateUpdate"
		|| sCmdID == "onRecvRtnForwardMarketStateUpdate"*/)
	{//�г�״̬
		string sMarketID;
		GessPacket.GetParameterVal("marketID",sMarketID);

		string sState;
		if (0 == GessPacket.GetParameterVal("marketState",sState))
		{
			bool bActive = false;
			if (sState == "0" || sState == "1" || sState == "2" || sState == "3")
				bActive = true;
			//else //M_PAUSE  M_CLOSE
				//m_bGessMarketActive = false;
			m_csYC.Lock();
			m_bGessMarketActive = bActive;
			m_uiCountNoQuote = 0;
			m_csYC.Unlock();
		}
		return 0;
	}
	else if (sCmdID == "onRecvRtnDeferInstStateUpdate"
		/*|| sCmdID == "onRecvRtnSpotInstStateUpdate"
		|| sCmdID == "onRecvRtnForwardInstStateUpdate"*/)
	{//�г�״̬
		string sState;
		if (0 == GessPacket.GetParameterVal("tradeState",sState))
		{
			bool bActive = false;
			if (sState == "3" || sState == "5" || sState == "7" || sState == "9")
				bActive = true;
			m_csYC.Lock();
			m_bGessInstActive = bActive;
			m_uiCountNoQuote = 0;
			m_csYC.Unlock();
		}
		return 0;
	}
	else
	{
		m_csYC.Lock();
		m_uiCountNoQuote = 0;
		m_csYC.Unlock();

		CRLog(E_PROINFO,"YC %s",GessPacket.Print().c_str());

		OnRecvPacket(GessPacket);
	}
	return 0;
}

/******************************************************************************
��������:socket���ӳɹ�����Ϊ�ͻ��˱�֪ͨ,һ�����ڴ�����֤���ķ���
���ú���:ͨѶ����������ص�
����ֵ  :int �������⴦��
������  :��ΰ
��������:2008.07.22
�޸ļ�¼:
******************************************************************************/
int CProcessInterfaceYC::OnConnect()
{
	string sLocalIp,sPeerIp;
	int nLocalPort,nPeerPort;
	GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
	CRLog(E_PROINFO,"YC OnConnect socket (%d) Peer(%s:%d),Me(%s:%d)",SocketID(),sPeerIp.c_str(),nPeerPort,sLocalIp.c_str(),nLocalPort);
	SendLogin();
	CreateTimer(m_GessPktInfo.ulIdleInterval);

	CProcessInterfaceClnNm::OnConnect();
	string sEvt = "YC OnConnect socket(";
	sEvt += ToString<int>(static_cast<int>(SocketID()));
	sEvt += ")";
	sEvt += " ,Peer(";
	sEvt += sPeerIp;
	sEvt += ":";
	sEvt += ToString<int>(nPeerPort);
	sEvt += ") ,Me(";
	sEvt += sLocalIp;
	sEvt += ":";
	sEvt += ToString<int>(nLocalPort);
	sEvt += ")";
	NotifyEvent(sEvt);
	return 0;
}


int CProcessInterfaceYC::HandleTimeout(unsigned long& ulTmSpan)
{
	int nRtn = 0;
	bool blNoSevicePkt = false;
	m_csYC.Lock();
	if (m_uiCountNoAlive >= 1)
	{//������·������ʱ��δ�յ����ģ���������
		//if (m_uiCountSended >= m_GessPktInfo.ulHelloReSend)
		//{//�ط��������������涨������׼���ر�
		//	nRtn = -1;
		//	ReqClose();
		//}
		//else
		//{//���ö�ʱ�����
		//	ulTmSpan = m_GessPktInfo.ulIntervalReSend;
		//	m_uiCountSended++;
			if (0 > SendHello())
				nRtn = -1;
		//}
	}
	else
	{
		m_uiCountSended = 0;
		ulTmSpan = m_GessPktInfo.ulIdleInterval;
	}

	// added 20110504
	if (m_bGessMarketActive && m_bGessInstActive)
	{
		if (m_uiCountNoQuote >= 4)
		{
			blNoSevicePkt = true;
			m_uiCountNoQuote = 0;
		}
		m_uiCountNoQuote++;
	}	
	m_uiCountNoAlive ++;
	// end add

	m_csYC.Unlock();

	if (blNoSevicePkt)
	{
		ReqClose();
		nRtn = -1;			

		string sLocalIp,sPeerIp;
		int nLocalPort,nPeerPort;
		GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
		string sEvt = "YC�ӿ�(";
		sEvt += ToString<int>(static_cast<int>(SocketID()));
		sEvt += ")";
		sEvt += " ,Peer(";
		sEvt += sPeerIp;
		sEvt += ":";
		sEvt += ToString<int>(nPeerPort);
		sEvt += ") ,Me(";
		sEvt += sLocalIp;
		sEvt += ":";
		sEvt += ToString<int>(nLocalPort);
		sEvt += ")";
		sEvt += "����ָ��ʱ����ҵ����,���ڹر�����!";
		NotifyEvent(sEvt);
		CRLog(E_PROINFO, "%s", sEvt.c_str());
	}
	return nRtn;
}

/******************************************************************************
��������:socket�����жϺ���֪ͨ,�ɸ���Э��Ҫ����д���
���ú���:�������ص�
����ֵ  :int �������⴦��
������  :��ΰ
��������:2008.07.22
�޸ļ�¼:
******************************************************************************/
void CProcessInterfaceYC::OnClose()
{
	m_blIsLogin = false;
	string sLocalIp,sPeerIp;
	int nLocalPort,nPeerPort;
	GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
	CRLog(E_PROINFO,"YC OnClose socket (%d) Peer(%s:%d),Me(%s:%d)",SocketID(),sPeerIp.c_str(),nPeerPort,sLocalIp.c_str(),nLocalPort);
	DestroyTimer();

	CProcessInterfaceClnNm::OnClose();
	string sEvt = "YC OnClose socket(";
	sEvt += ToString<int>(static_cast<int>(SocketID()));
	sEvt += ")";
	sEvt += " ,Peer(";
	sEvt += sPeerIp;
	sEvt += ":";
	sEvt += ToString<int>(nPeerPort);
	sEvt += ") ,Me(";
	sEvt += sLocalIp;
	sEvt += ":";
	sEvt += ToString<int>(nLocalPort);
	sEvt += ")";
	NotifyEvent(sEvt);
	return;
}

//�ӿڼ���������
int CProcessInterfaceYC::SendHello()
{
	CBroadcastPacket pkt("ConnectTest");
	CAppProcess::SendPacket(pkt);
	return 0;
}

int CProcessInterfaceYC::OnHello(CBroadcastPacket & GessPacket)
{
	CBroadcastPacket pkt("ConnectTestRsp");
	CAppProcess::SendPacket(pkt);
	return 0;
}

//���ӳɹ����͵ĵ�һ����½����
int CProcessInterfaceYC::SendLogin()
{
	CBroadcastPacket pkt;//("Subscriber");

	srand(static_cast<unsigned int>(time(0)));
	int RANGE_MIN = 0;
    int RANGE_MAX = 10000;
    int nKey = rand() * (RANGE_MAX - RANGE_MIN) / RAND_MAX + RANGE_MIN;

	//pkt.AddParameter("term_type","12");
	pkt.AddParameter("user_key",ToString(nKey));
	pkt.AddParameter("user_type",m_GessPktInfo.user_type.c_str());
	pkt.AddParameter("user_id",m_GessPktInfo.user_id.c_str());
	//pkt.AddParameter("user_pwd",m_GessPktInfo.user_pwd.c_str());
	//pkt.AddParameter("branch_id",m_GessPktInfo.branch_id.c_str());
	pkt.AddParameter("again_flag","0");

	CRLog(E_PROINFO,"����������ӳɹ�,������֤����,%s",pkt.Print().c_str());

	CAppProcess::SendPacket(pkt);
	return 0;
}

int CProcessInterfaceYC::OnRspLogin(CBroadcastPacket & GessPacket)
{
	//���Ӧ���Ĳ���״̬
	char szRsp[128],szRspMsg[128];
	memset(szRsp,0x00,128);
	memset(szRspMsg,0x00,128);
	GessPacket.GetParameterVal("RspCode",szRsp);
	GessPacket.GetParameterVal("RspMsg",szRspMsg);
	if (strcmp(szRsp,"0000") == 0)
	{
		m_blIsLogin = true;
	}
	else
	{
		m_blIsLogin = false;
		//RequestDisconnect();
	}

	CRLog(E_PROINFO,"TS��֤Ӧ��:%s,%s",szRsp,szRspMsg);
	return 0;
}

int CProcessInterfaceYC::GetNmKey(string& sKey)
{
	sKey = "YC�ӿڿͻ�������.";
	int nSockeID = static_cast<int>( SocketID());
	if (INVALID_SOCKET != nSockeID)
	{
		sKey += ToString<int>(nSockeID);
	}

	srand(static_cast<unsigned int>(time(0)));
	int RANGE_MIN = 1;
	int RANGE_MAX = 99;
	int nRandom = rand() * (RANGE_MAX - RANGE_MIN) / RAND_MAX + RANGE_MIN;
	sKey += ".";
	sKey += ToString<int>(nRandom);
	return 0;
}