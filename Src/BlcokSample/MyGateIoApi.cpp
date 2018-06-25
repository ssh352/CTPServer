
#include "MyGateIoApi.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include <Poco/Net/HTTPCredentials.h>
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <iostream>
#include <unordered_map>


using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;

#include "Poco/JSON/Object.h"  
#include "Poco/JSON/Parser.h"  
#include "Poco/Dynamic/Var.h"  

using namespace Poco::JSON;

std::unordered_map<string, int>g_mpairs;


//char  g_paires[][10] = { "eth_btc","etc_btc","etc_eth","zec_btc","dash_btc","ltc_btc","xrp_btc" ,"ae_usdt","ae_eth","ae_btc"};

char  g_paires[][10] = { "ae_usdt","ae_eth","ae_btc" };
int  pairNum = 2;
/*
[
"eth_btc","etc_btc","etc_eth","zec_btc","dash_btc","ltc_btc","bcc_btc","qtum_btc",
"qtum_eth","xrp_btc","zrx_btc","zrx_eth","dnt_eth","dpy_eth","oax_eth","lrc_eth",
"lrc_btc","pst_eth","tnt_eth","snt_eth","snt_btc","omg_eth","omg_btc","pay_eth",
"pay_btc","bat_eth","cvc_eth","storj_eth","storj_btc","eos_eth","eos_btc"
]
*/
//2.ϵͳ֧�ֵĽ��׶�
string parsePaires(string & str)
{
	std::cout << str << std::endl;
	string tp = "\"pairs\":" + str;
	Parser parser;
	Poco::Dynamic::Var result = parser.parse(tp);


	Object::Ptr object = result.extract<Object::Ptr>();
	Poco::Dynamic::Var success = object->get("result");

	std::cout << success.toString() << std::endl;

	return  "";
}
/*
{
"result": "true",
"pairs": [
{
"eth_btc": {
"decimal_places": 6,
"min_amount": 0.0001,
"fee": 0.2
}
},
{
"etc_btc": {
"decimal_places": 6,
"min_amount": 0.0001,
"fee": 0.2
}
}
]
}
*/
//3.�����г���������
string parseMarketinfo(string & str)
{
	try
	{
		//std::cout << str << std::endl;
		Parser parser;
		Poco::Dynamic::Var result = parser.parse(str);

		Object::Ptr object = result.extract<Object::Ptr>();
		Poco::Dynamic::Var success = object->get("result");
		Array::Ptr pArry = object->getArray("pairs");

		Array::ConstIterator it = pArry->begin();
		for (; it != pArry->end(); it++)
		{
			//std::cout << it->toString() << std::endl;

			//��������Ƕ��json
			Poco::Dynamic::Var subresult = parser.parse(it->toString());
			Object::Ptr subobject = subresult.extract<Object::Ptr>();

			Poco::Dynamic::Var stest;
			bool bFind = false;

			string name;
			for (int i = 0; i < pairNum; ++i)
			{
				stest = subobject->get(g_paires[i]);

				if (!stest.isEmpty())
				{
					name = g_paires[i];
					bFind = true;
					break;
				}
			}

			if (!bFind)
				continue;

			std::cout << "---------   " << name << "  �����г���������----------" << std::endl;
			//std::cout << stest.toString() << std::endl;

			Object::Ptr val = stest.extract<Object::Ptr>();

			Poco::Dynamic::Var decimal_places = val->get("decimal_places");
			Poco::Dynamic::Var fee = val->get("fee");
			Poco::Dynamic::Var min_amount = val->get("min_amount");

			std::cout << "�۸񾫶�   :" << decimal_places.toString() << std::endl;
			std::cout << "���׷�     :" << fee.toString() << std::endl;
			std::cout << "��С�µ��� :" << min_amount.toString() << std::endl;



		}


		return  "";
	}
	catch (...)
	{

	}

	return  "";
}


/*
{
"result": "true",
"data": [
{
"no": 1,
"symbol": "LTC",
"name": "Litecoin",
"name_en": "Litecoin",
"name_cn": "���ر�",
"pair": "ltc_btc",
"rate": "418.00",
"vol_a": 168120.2,
"vol_b": "65,616,561",
"curr_a": "LTC",
"curr_b": "BTC",
"curr_suffix": " BTC",
"rate_percent": "19.73",
"trend": "up",
"supply": 25760300,
"marketcap": "10,767,805,404",
"plot": null
},
{
"no": 2,
"symbol": "ETH",
"name": "ETH",
"name_en": "ETH",
"name_cn": "��̫��",
"pair": "etc_eth",
"rate": "0.7450",
"vol_a": 65227328.3,
"vol_b": "51,041,999",
"curr_a": "etc",
"curr_b": "eth",
"curr_suffix": " eth",
"rate_percent": "-1.84",
"trend": "up",
"supply": 1050000000,
"marketcap": "782,250,000",
"plot": null
}
]
}

*/
//4.�����г�����API
string parseMarketlist(string & str)
{
	try
	{
		//std::cout << str << std::endl;
		Parser parser;
		Poco::Dynamic::Var result = parser.parse(str);

		Object::Ptr object = result.extract<Object::Ptr>();
		Poco::Dynamic::Var success = object->get("result");

		//std::cout << success.toString() << std::endl;

		Array::Ptr pArry = object->getArray("data");

		Array::ConstIterator it = pArry->begin();

		for (; it != pArry->end(); it++)
		{
			//std::cout << it->toString() << std::endl;
			//��������Ƕ��json
			Poco::Dynamic::Var subresult = parser.parse(it->toString());
			Object::Ptr subobject = subresult.extract<Object::Ptr>();

			Poco::Dynamic::Var stest = subobject->get("pair");
			bool bFind = false;

			string proCode;

			//���˽��׶�
			/*for (int i = 0; i < pairNum; ++i)
			{
			if (stest.toString() == g_paires[i])
			{
			proCode = g_paires[i];
			bFind = true;
			break;
			}
			}

			if (!bFind)
			continue;*/

			std::cout << "---------   " << proCode << "  �г�����----------" << std::endl;
			//std::cout << stest.toString() << std::endl;

			//Object::Ptr val = stest.extract<Object::Ptr>();
			Poco::Dynamic::Var symbol = subobject->get("symbol");
			Poco::Dynamic::Var name = subobject->get("name");
			Poco::Dynamic::Var name_cn = subobject->get("name_cn");
			Poco::Dynamic::Var pair = subobject->get("pair");
			Poco::Dynamic::Var rate = subobject->get("rate");
			Poco::Dynamic::Var vol_a = subobject->get("vol_a");
			Poco::Dynamic::Var vol_b = subobject->get("vol_b");
			Poco::Dynamic::Var curr_a = subobject->get("curr_a");
			Poco::Dynamic::Var curr_b = subobject->get("curr_b");
			Poco::Dynamic::Var curr_suffix = subobject->get("curr_suffix");
			Poco::Dynamic::Var rate_percent = subobject->get("rate_percent");
			Poco::Dynamic::Var trend = subobject->get("trend");
			Poco::Dynamic::Var supply = subobject->get("supply");
			Poco::Dynamic::Var marketcap = subobject->get("marketcap");


			std::cout << "���ֱ�ʶ         :" << symbol.toString() << std::endl;
			std::cout << "��������         :" << name.toString() << std::endl;
			std::cout << "��������         :" << name_cn.toString() << std::endl;
			std::cout << "���׶�           :" << pair.toString() << std::endl;
			std::cout << "��ǰ�۸�         :" << rate.toString() << std::endl;
			std::cout << "���һ����ҽ����� :" << vol_a.toString() << std::endl;
			std::cout << "�һ����ҽ�����   :" << vol_b.toString() << std::endl;
			std::cout << "���һ�����       :" << curr_a.toString() << std::endl;
			std::cout << "�һ�����         :" << curr_b.toString() << std::endl;
			std::cout << "�������ͺ�׺     :" << curr_suffix.toString() << std::endl;
			std::cout << "�ǵ��ٷְ�       :" << rate_percent.toString() << std::endl;
			std::cout << "24Сʱ����       :" << trend.toString() << std::endl;
			std::cout << "���ֹ�Ӧ��       :" << supply.toString() << std::endl;
			std::cout << "����ֵ           :" << marketcap.toString() << std::endl;




		}


		return  "";
	}
	catch (...)
	{

	}

	return  "";
}


/*
{
"result": "true",
"last": 0.1,
"lowestAsk": 0.1,
"highestBid": "0.00000000",
"percentChange": 0,
"baseVolume": 0.001,
"quoteVolume": 0.01,
"high24hr": 0.1,
"low24hr": 0.1
}
*/
//6.���ָ�����׵�����
//�������£���ߣ���� ��������ͽ�������ÿ10���Ӹ��� :
string parseMarketTicker(string & str)
{
	try
	{
		//std::cout << str << std::endl;
		Parser parser;
		Poco::Dynamic::Var result = parser.parse(str);

		Object::Ptr subobject = result.extract<Object::Ptr>();
		Poco::Dynamic::Var success = subobject->get("result");

		std::cout << success.toString() << std::endl;

		Poco::Dynamic::Var symbol = subobject->get("last");
		Poco::Dynamic::Var name = subobject->get("lowestAsk");
		Poco::Dynamic::Var name_cn = subobject->get("highestBid");
		Poco::Dynamic::Var pair = subobject->get("percentChange");
		Poco::Dynamic::Var rate = subobject->get("baseVolume");
		Poco::Dynamic::Var vol_a = subobject->get("quoteVolume");
		Poco::Dynamic::Var vol_b = subobject->get("high24hr");
		Poco::Dynamic::Var curr_a = subobject->get("low24hr");

		std::cout << "���³ɽ���      :" << symbol.toString() << std::endl;
		std::cout << "������ͼ�      :" << name.toString() << std::endl;
		std::cout << "����߼�      :" << name_cn.toString() << std::endl;
		std::cout << "�ǵ��ٷֱ�      :" << pair.toString() << std::endl;
		std::cout << "������          :" << rate.toString() << std::endl;
		std::cout << "�һ����ҽ�����  :" << vol_a.toString() << std::endl;
		std::cout << "24Сʱ��߼�    :" << vol_b.toString() << std::endl;
		std::cout << "24Сʱ��ͼ�    :" << curr_a.toString() << std::endl;


		return  "";
	}
	catch (...)
	{

	}

	return  "";
}


/*
{
"result": "true",
"asks" : [
[29500, 4.07172355],
[29499, 0.00203397],
[29495, 1],
[29488, 0.0672],
[29475, 0.001]
],
"bids": [
[28001, 0.0477],
[28000, 0.35714018],
[28000, 2.56222976],
[27800, 0.0015],
[27777, 0.1]
]
}
*/

//7.ָ����Լ���������
//�������£���ߣ���� ��������ͽ�������30������ :
string parseMarketOrderBooks(string & str)
{
	try
	{
		std::cout << str << std::endl;
		Parser parser;
		Poco::Dynamic::Var result = parser.parse(str);

		Object::Ptr object = result.extract<Object::Ptr>();
		Poco::Dynamic::Var success = object->get("result");

		Array::Ptr pArry = object->getArray("asks");

		//Poco::Dynamic::Var  ttttttt = pArry->getElement<string>(0);
		//std::cout << ttttttt.toString() << std::endl;
		Array::ConstIterator it = pArry->begin();
		//����������������ݴ�ӡ����
		//��ȻҲ���԰�ÿ�������ó����á�
		for (; it != pArry->end(); it++)
		{
			Poco::Dynamic::Var result = parser.parse(it->toString());

			std::cout << it->toString() << std::endl;
			string  tt = it[0].toString();



		}

		pArry = object->getArray("bids");
		it = pArry->begin();
		for (; it != pArry->end(); it++)
		{
		}

		std::cout << success.toString() << std::endl;
		/*Poco::Dynamic::Var symbol = subobject->get("last");
		Poco::Dynamic::Var name = subobject->get("lowestAsk");
		Poco::Dynamic::Var name_cn = subobject->get("highestBid");
		Poco::Dynamic::Var pair = subobject->get("percentChange");
		Poco::Dynamic::Var rate = subobject->get("baseVolume");
		Poco::Dynamic::Var vol_a = subobject->get("quoteVolume");
		Poco::Dynamic::Var vol_b = subobject->get("high24hr");
		Poco::Dynamic::Var curr_a = subobject->get("low24hr");

		std::cout << "���³ɽ���      :" << symbol.toString() << std::endl;
		std::cout << "������ͼ�      :" << name.toString() << std::endl;
		std::cout << "����߼�      :" << name_cn.toString() << std::endl;
		std::cout << "�ǵ��ٷֱ�      :" << pair.toString() << std::endl;
		std::cout << "������          :" << rate.toString() << std::endl;
		std::cout << "�һ����ҽ�����  :" << vol_a.toString() << std::endl;
		std::cout << "24Сʱ��߼�    :" << vol_b.toString() << std::endl;
		std::cout << "24Сʱ��ͼ�    :" << curr_a.toString() << std::endl;*/


		return  "";
	}
	catch (...)
	{

	}

	return  "";
}
