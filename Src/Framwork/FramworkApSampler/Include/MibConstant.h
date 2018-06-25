#ifndef _QUOTATION_MIB_CONSTANT_H_
#define _QUOTATION_MIB_CONSTANT_H_

#include <string>
using namespace std;

namespace MibConst
{
	//����MIB
	const std::string gc_sSgeLoginState			= "1.2.8.1";		//���������ĵ�¼״̬
	const std::string gc_sSgeLoginToken			= "1.2.8.2";		//��¼���Ƴ���״̬
	const std::string gc_sSgeLoginInd			= "1.2.8.3";		//��¼ָʾ״̬
	const std::string gc_sSgeLoginAlarm			= "1.2.8.4";		//��¼�澯״̬


	
	const std::string gc_sMemQueTotal			= "1.1.5.1.1.1";
	const std::string gc_sMemQueFree			= "1.1.5.1.1.2";
	const std::string gc_sMemQueUsed			= "1.1.5.1.1.3";

	const std::string gc_sFwdCount				= "1.3.1.1.1";		//�ۼƽ��հ���
	const std::string gc_sQuoPktMBytes			= "1.3.1.1.2";		//�ۼƷ���M�ֽ���
	const std::string gc_sNowBandWidth			= "1.3.1.1.3";		//����ͳ�ƴ���
	const std::string gc_sMaxBandWidth			= "1.3.1.1.4";		//���ͳ�ƴ���
	const std::string gc_sMinBandWidth			= "1.3.1.1.5";		//��Сͳ�ƴ���
	const std::string gc_sAvgBandWidth			= "1.3.1.1.6";		//ƽ��ͳ�ƴ���
	const std::string gc_sQuoPerPkt				= "1.3.1.1.7";		//ÿ��������
	const std::string gc_sBytesPerPkt			= "1.3.1.1.8";		//ÿ���ֽ���
	const std::string gc_sSubscribers			= "1.3.1.1.9";		//������

	const std::string gc_sSamplerInPktTotal		= "1.3.2.1.1";
	const std::string gc_sSamplerFwdPktTotal	= "1.3.2.1.2";

	const std::string gc_sDelayMin				= "1.3.3.1.1";		// 
	const std::string gc_sDelayMax				= "1.3.3.1.2";		// 
	const std::string gc_sDelayAvg				= "1.3.3.1.3";		// 
	const std::string gc_sDelayLess0s			= "1.3.3.1.4";		//
	const std::string gc_sDelayLess1s			= "1.3.3.1.5";		//
	const std::string gc_sDelayLess2s			= "1.3.3.1.6";		// 
	const std::string gc_sDelayLess3s			= "1.3.3.1.7";		// 
	const std::string gc_sDelayLess5s			= "1.3.3.1.8";		// 
	const std::string gc_sDelayLess10s			= "1.3.3.1.9";		// 
	const std::string gc_sDelayLess30s			= "1.3.3.1.A";		// 
	const std::string gc_sDelayLess60s			= "1.3.3.1.B";		// 
	const std::string gc_sDelayLess120s			= "1.3.3.1.C";		// 
	const std::string gc_sDelayMore120s			= "1.3.3.1.D";		// 
	
}

#endif