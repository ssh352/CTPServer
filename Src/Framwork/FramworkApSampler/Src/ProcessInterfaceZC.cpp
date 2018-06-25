#include "ProcessInterfaceZC.h"
#include "Logger.h"
#include "strutils.h"

using namespace strutils;

CProcessInterfaceZC::CProcessInterfaceZC()
:ulSeqNo(0)
,m_pCfg(0)
,m_blIsLogin(false)
,m_uiCountNoAlive(0)
,m_uiCountSended(0)
,m_subscripType("0")
,m_uiCountNoQuote(0)
{
	m_GessPktInfo.ulIdleInterval = 5;//DEFAULT_IDLE_TIMEOUT;
	m_GessPktInfo.ulIntervalReSend = DEFAULT_HELLO_RESEND_INTERVAL;
	m_GessPktInfo.ulHelloReSend = DEFAULT_HELLO_RESEND_COUNT;
	m_GessPktInfo.blNeedLogin = false;
	m_GessPktInfo.nNeedCheckServicePkt = 0;
}

CProcessInterfaceZC::~CProcessInterfaceZC(void)
{
}

int CProcessInterfaceZC::Init(CConfig * pCfg)
{
	CTcpAppProcessClient::Init(pCfg);

	if (0 != pCfg)
	{
		string sTmp;
		if (0 == pCfg->GetProperty("login",sTmp))
		{
			if (1 == FromString<int>(sTmp))
				m_GessPktInfo.blNeedLogin = true;
		}
	
		if (0 == pCfg->GetProperty("serviec_pkt_check",sTmp))
		{
			if (1 == FromString<int>(sTmp))
			{
				m_GessPktInfo.nNeedCheckServicePkt = 1;
			}
		}

		HandleCheckTime(pCfg);

		pCfg->GetProperty("user_id",m_GessPktInfo.sUserName);
		pCfg->GetProperty("user_pwd",m_GessPktInfo.sPassword);

		string sNodeID = "0001";
		unsigned int ulNodeID = 1;
		if (0 != pCfg->GetProperty("node_id",sNodeID))
		{
			CConfig* p = pCfg->GetCfgGlobal();				
			p->GetProperty("node_id",sNodeID);
			ulNodeID = FromString<unsigned int>(sNodeID);
			m_GessPktInfo.node_id = ToHexString<unsigned int>(ulNodeID);
		}
		else
		{
			ulNodeID = FromString<unsigned int>(sNodeID);
			m_GessPktInfo.node_id = ToHexString<unsigned int>(ulNodeID);
		}

		if(0 != pCfg->GetProperty("order_type",m_subscripType))
		{
			m_subscripType = "0";
		}
		pCfg->GetProperty("order_item",m_subscripItem);		
	}

	m_pCfg = pCfg;
	return 0;
}

//ҵ���ļ��ʱ��
void CProcessInterfaceZC::HandleCheckTime(CConfig * pCfg)
{
	if (0 == pCfg)
		return;

	string sCheckTime;
	for (int n = 0; n <=6; n++)
	{
		string sWeekDay = "week";
		sWeekDay += ToString<int>(n);
		if (0 != pCfg->GetProperty(sWeekDay,sCheckTime))
			continue;
	
		vector<PairInterval> vPairInterval;

		vector<string> vTmCheck = explodeQuoted(",", sCheckTime);
		for (vector<string>::iterator itCheck = vTmCheck.begin(); itCheck != vTmCheck.end(); ++itCheck)
		{
			string sTmPair = trim(*itCheck);
			if (sTmPair == "")
				continue;

			vector<string> vTmPair = explodeQuoted("-", sTmPair);
			if (vTmPair.size() != 2)
				continue;

			vector<string> vSatrtTime, vEndTime;								
			vSatrtTime = explodeQuoted(":", vTmPair[0]);
			size_t nStartSize = vSatrtTime.size();
			if (3 != nStartSize)
				continue;

			int nHour,nMinute,nSecond;
			nHour = nMinute = nSecond = 0xFF;
			for (size_t j = 0; j < nStartSize; j++)
			{
				switch (j)
				{
				case 0:
					nHour = FromString<int>(vSatrtTime[j]);
					break;
				case 1:
					nMinute = FromString<int>(vSatrtTime[j]);
					break;
				case 2:
					nSecond = FromString<int>(vSatrtTime[j]);
					break;
				}
			}

			if (nHour < 0 || nHour > 24 || nMinute > 60 || nMinute < 0 || nSecond > 60 || nSecond< 0)
				continue;

			if (nHour == 24 && nMinute == 0 && nSecond == 0)
			{
				nHour = 23;
				nMinute = 59;
				nSecond = 59;
			}
			CGessTime oTmStart(nHour,nMinute,nSecond);
						
			vEndTime = explodeQuoted(":", vTmPair[1]);
			size_t nEndSize = vEndTime.size();
			if (3 != nEndSize)
				continue;

			nHour = nMinute = nSecond = 0xFF;
			for (size_t j = 0; j < nEndSize; j++)
			{
				switch (j)
				{
				case 0:
					nHour = FromString<int>(vEndTime[j]);
					break;
				case 1:
					nMinute = FromString<int>(vEndTime[j]);
					break;
				case 2:
					nSecond = FromString<int>(vEndTime[j]);
					break;
				}
			}

			if (nHour < 0 || nHour > 24 || nMinute > 60 || nMinute < 0 || nSecond > 60 || nSecond< 0)
				continue;

			if (nHour == 24 && nMinute == 0 && nSecond == 0)
			{
				nHour = 23;
				nMinute = 59;
				nSecond = 59;
			}
			CGessTime oTmEnd(nHour,nMinute,nSecond);

			PairInterval p;
			p.first = oTmStart;
			p.second = oTmEnd;
			vPairInterval.push_back(p);				
		}

		m_mapCheckWeek[n] = vPairInterval;
	}	
}

//�Ƿ���ҵ����
bool CProcessInterfaceZC::IsCheckTime()
{
	int nWeekDay = CGessDate::ThisWeekDay();
	MapCheckWeek::iterator itCheck = m_mapCheckWeek.find(nWeekDay);
	if (itCheck == m_mapCheckWeek.end())
		return false;

	CGessTime oThisTime;
	oThisTime.ToNow();
	vector<PairInterval> & vTmPair = (*itCheck).second;
	size_t nSize = vTmPair.size();
	for (size_t i = 0; i < nSize; i++)
	{
		if (vTmPair[i].first <= oThisTime && oThisTime <= vTmPair[i].second)
		{
			return true;
		}
	}
    return false;
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
void CProcessInterfaceZC::GetPacketInfo(PacketInfo & stInfo)
{
	stInfo.eLengthType = ltInterger;
	stInfo.nLengthPos = 0;
	stInfo.nLengthBytes = 4;
	stInfo.blLenIncludeHeader = true;
	stInfo.nFixHeadLen = 16;
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
int CProcessInterfaceZC::OnPacket(char * pData, size_t nSize)
{
	CSamplerPacket oPacket;
	oPacket.Decode(pData, nSize);

	bool blFlag = false;
	m_csZC.Lock();
	if (m_uiCountSended > 1)
		blFlag = true;
	m_uiCountNoAlive = 0;
	m_csZC.Unlock();

	std::string sCmdID = oPacket.GetCmdID();
	if (strutils::ToHexString<unsigned int>(YL_HELLO_RSP) == sCmdID)
	{
		if (blFlag)
			CRLog(E_PROINFO,"ZC�յ�����Ӧ��(%u)",m_uiCountSended);
		return 0;
	}
	else if (strutils::ToHexString<unsigned int>(YL_HELLO) == sCmdID)
	{
		if (blFlag)
			CRLog(E_PROINFO,"ZC�յ�������������(%u)",m_uiCountSended);
		return OnHello(oPacket);
	}
	else if (strutils::ToHexString<unsigned int>(YL_LOGIN_RSP) == sCmdID)
	{
		if (blFlag)
			CRLog(E_PROINFO,"ZC�յ���¼��Ӧ��(%u)",m_uiCountSended);
		OnLoginRsp(oPacket);
		OnRecvPacket(oPacket);
	}
	else
	{
		// added by Ben 20110429
		m_csZC.Lock();
		m_uiCountNoQuote = 0; 
		m_csZC.Unlock();
		// end add
		OnRecvPacket(oPacket);
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
int CProcessInterfaceZC::OnConnect()
{
	CProcessInterfaceClnNm::OnConnect();

	string sLocalIp,sPeerIp;
	int nLocalPort,nPeerPort;
	GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
	CRLog(E_PROINFO,"ZC OnConnect socket (%d) Peer(%s:%d),Me(%s:%d)",SocketID(),sPeerIp.c_str(),nPeerPort,sLocalIp.c_str(),nLocalPort);
	CreateTimer(m_GessPktInfo.ulIdleInterval);  // modified 20110504 ������ʱ��

	string sEvt = "ZC OnConnect socket(";
	sEvt += ToString<int>(static_cast<int>(SocketID()));
	sEvt += ")";
	sEvt += " ,Peer(";
	sEvt += sPeerIp;
	sEvt += ":";
	sEvt += ToString<int>(nPeerPort);
	sEvt += ") ,Me(";
	sEvt += sLocalIp;
	sEvt += ":";
	sEvt +=  ToString<int>(nLocalPort);
	sEvt += ")";
	NotifyEvent(sEvt);

	CSamplerPacket oPacketLogin(YL_LOGIN);
	CMessage &  msg = oPacketLogin.GetMsg();
	msg.SetField(MSG_SEQ_ID,++ulSeqNo);
	msg.SetField(MSG_NODE_ID,m_GessPktInfo.node_id);
	msg.SetField(MSG_LOGIN_ID, m_GessPktInfo.sUserName);
	msg.SetField(MSG_LOGIN_PWD, m_GessPktInfo.sPassword);
	unsigned int ulPwdMode = 0;
	msg.SetField(MSG_LOGIN_PWD_ENC,ulPwdMode);

	CRLog(E_PROINFO,"Login:MSG_SEQ_ID=%u,MSG_NODE_ID=%s,MSG_LOGIN_ID=%s,MSG_LOGIN_PWD=%s,MSG_LOGIN_PWD_ENC=%u",ulSeqNo,m_GessPktInfo.node_id.c_str(),m_GessPktInfo.sUserName.c_str(),m_GessPktInfo.sPassword.c_str(),ulPwdMode);

	CAppProcess::SendPacket(oPacketLogin);
	return 0;
}


int CProcessInterfaceZC::HandleTimeout(unsigned long& ulTmSpan)
{
	int nRtn = 0;
	bool blNoSevicePkt = false;
	CMutexGuard oGuard(m_csZC);
	if (m_uiCountNoAlive >= 1)
	{//������·������ʱ��δ�յ����ģ���������
		if (m_uiCountSended >= m_GessPktInfo.ulHelloReSend)
		{//�ط��������������涨������׼���ر�
			CRLog(E_PROINFO,"ZC%s(%u-%u)","������ʱ!",m_uiCountNoAlive,m_uiCountSended);
			m_uiCountSended = 0;
			m_uiCountNoAlive = 0;

            // �ղ���������ģ���˳�����ɿ��Ź�����Ӧ��, Jerry Lee, 20110730 
/*            CSamplerPacket oPacket(YL_QUITMSG);
            CMessage &  msg = oPacket.GetMsg();

            msg.SetField(MSG_SEQ_ID,static_cast<unsigned int>(0));
            msg.SetField(MSG_NODE_ID,m_GessPktInfo.node_id);
			msg.SetField(MSG_BODY_NODEID,m_GessPktInfo.node_id);			

            OnRecvPacket(oPacket);
            // end
*/
			nRtn = -1;
			ReqClose();
		}
		else
		{//���ö�ʱ�����
			ulTmSpan = m_GessPktInfo.ulIntervalReSend;
			m_uiCountSended++;

			if (m_uiCountSended > 1)
				CRLog(E_PROINFO,"ZC��������,��%d��!",m_uiCountSended);

			int nSended = SendHello();
			if (nSended < 0 && nSended != -2)
				nRtn = -1;
		}
	}
	else
	{
		m_uiCountSended = 0;
		ulTmSpan = m_GessPktInfo.ulIdleInterval;
	}
	m_uiCountNoAlive++;

	// added 20110504
	if (m_uiCountNoQuote >= 4)
	{
		blNoSevicePkt = true;		
		m_uiCountNoQuote = 0;
	}
	m_uiCountNoQuote ++;
	// end add

	if (blNoSevicePkt && 1 == m_GessPktInfo.nNeedCheckServicePkt && IsCheckTime())
	{
		ReqClose();
		nRtn = -1;

		string sLocalIp,sPeerIp;
		int nLocalPort,nPeerPort;
		GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
		string sEvt = "ZC�ӿ�(";
		sEvt += ToString<int>(static_cast<int>(SocketID()));
		sEvt += ")";
		sEvt += " ,Peer(";
		sEvt += sPeerIp;
		sEvt += ":";
		sEvt +=  ToString<int>(nPeerPort);
		sEvt += ") ,Me(";
		sEvt += sLocalIp;
		sEvt += ":";
		sEvt +=  ToString<int>(nLocalPort);
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
void CProcessInterfaceZC::OnClose()
{
	m_blIsLogin = false;
	string sLocalIp,sPeerIp;
	int nLocalPort,nPeerPort;
	GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
	CRLog(E_PROINFO,"ZC OnClose socket (%d) Peer(%s:%d),Me(%s:%d)",SocketID(),sPeerIp.c_str(),nPeerPort,sLocalIp.c_str(),nLocalPort);
	DestroyTimer();
	CProcessInterfaceClnNm::OnClose();

	string sEvt = "ZC OnClose socket(";
	sEvt += ToString<int>(static_cast<int>(SocketID()));
	sEvt += ")";
	sEvt += " ,Peer(";
	sEvt += sPeerIp;
	sEvt += ":";
	sEvt +=  ToString<int>(nPeerPort);
	sEvt += ") ,Me(";
	sEvt += sLocalIp;
	sEvt += ":";
	sEvt +=  ToString<int>(nLocalPort);
	sEvt += ")";
	NotifyEvent(sEvt);
	return;
}

//�����ṩ����Ҫ�ӿں���,��ͨѶ���������ͱ���ʱ����
int CProcessInterfaceZC::SendPacket(CPacket & SendGessPacket)
{
	try
	{
		//if (!IsLogin())
		//	return -1;

		CSamplerPacket & pkt = dynamic_cast<CSamplerPacket&>(SendGessPacket);
		CMessage &  msg = pkt.GetMsg();
		msg.SetField(MSG_SEQ_ID,++ulSeqNo);
		msg.SetField(MSG_NODE_ID,m_GessPktInfo.node_id);

		CAppProcess::SendPacket(pkt);
		return 0;
	}
	catch(std::bad_cast)
	{
		CRLog(E_ERROR,"%s","packet error!");
		return -1;
	}
	catch(std::exception e)
	{
		CRLog(E_ERROR,"exception:%s!",e.what());
		return -1;
	}
	catch(...)
	{
		CRLog(E_ERROR,"%s","Unknown exception!");
		return -1;
	}
}

//�ӿڼ���������
int CProcessInterfaceZC::SendHello()
{
	CSamplerPacket oPacketHello(YL_HELLO);
	CMessage &  msg = oPacketHello.GetMsg();

	msg.SetField(MSG_SEQ_ID,static_cast<unsigned int>(0));
	msg.SetField(MSG_NODE_ID,m_GessPktInfo.node_id);

	return CAppProcess::SendPacket(oPacketHello);
}

int CProcessInterfaceZC::OnHello(CSamplerPacket & oPacket)
{
    CMessage &  msgReq = oPacket.GetMsg();
	unsigned int uiSeqID = 0;
	msgReq.GetField(MSG_SEQ_ID,uiSeqID);
	unsigned int uiNodeID = 0;
	msgReq.GetField(MSG_NODE_ID,uiNodeID);

	CSamplerPacket oPacketRsp(YL_HELLO_RSP);
	CMessage &  msgRsp = oPacket.GetMsg();
	msgRsp.SetField(MSG_SEQ_ID,uiSeqID);
	msgRsp.SetField(MSG_NODE_ID,uiNodeID);

	CAppProcess::SendPacket(oPacketRsp);
	return 0; 
}

int CProcessInterfaceZC::OnLoginRsp(CSamplerPacket & oPacket)
{
	CMessageImpl& msg = dynamic_cast<CMessageImpl&>(oPacket.GetMsg());

	unsigned int uiSeqNo = 0;
	msg.GetField(MSG_SEQ_ID,uiSeqNo);

	unsigned int uiNodeID = 0;
	msg.GetField(MSG_NODE_ID,uiNodeID);

	unsigned int uiRst = 0;
	msg.GetField(MSG_LOGIN_RESULT,uiRst);
	CRLog(E_PROINFO,"ZC OnRecvLoginRsp SeqNo:%u, NodeID:%u, Result:%u",uiSeqNo, uiNodeID, uiRst);

	if (0 != uiRst)
	{
		ReqClose();
		return -1;
	}

	CMessageImpl oMsgSub;
	oMsgSub.SetField(MSG_SEQ_ID,++ulSeqNo);
	oMsgSub.SetField(MSG_NODE_ID,m_GessPktInfo.node_id);
	oMsgSub.SetField(MSG_SUBSCRIP_TYPE,m_subscripType);

	string sItems;
	vector<string> vsVal = strutils::explode(",", m_subscripItem);
	int nSize = vsVal.size();
	if (m_subscripItem == "*" || m_subscripItem == "" || nSize == 0)
	{
		unsigned int uiVal = 0xFFFFFFFF;
		sItems.assign((char*)(&uiVal),sizeof(unsigned int));
	}
	else
	{
		unsigned int* puiVal = new unsigned int[nSize];
		unsigned int* puiTmp = puiVal;
		
		vector<string>::const_iterator it;
		int nCount = 0;
		for ( it = vsVal.begin(); it != vsVal.end(); ++it)
		{
			if (trim(*it) != "")
			{
				*puiTmp = htonl(strutils::FromString<unsigned int>(*it,16));
				puiTmp++;
				nCount++;
			}
		}

		if (nCount == 0)
		{
			*puiVal = 0xFFFFFFFF;
			nCount = 1;
		}

		sItems.assign((char*)(puiVal),nCount*sizeof(unsigned int));
		delete []puiVal;
	}
	
	oMsgSub.SetBinaryField(MSG_SUBSCRIP_RECS,sItems);

	CSamplerPacket oPktSub(oMsgSub,YL_SUBSCRIP);
	CRLog(E_PROINFO,"ZC Subscrib MSG_SEQ_ID=%u,MSG_NODE_ID=%s",ulSeqNo,m_GessPktInfo.node_id.c_str());

	CAppProcess::SendPacket(oPktSub);
	return 0;
}

int CProcessInterfaceZC::GetNmKey(string& sKey)
{
	sKey = "ZC�ӿڿͻ�������.";
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