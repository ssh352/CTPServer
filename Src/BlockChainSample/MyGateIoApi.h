#pragma once

#include "BCBase.h"
#include <string>

using namespace  std;


BCSAMPLE_EXPORT string parsePaires(string & str);

//3.�����г���������
BCSAMPLE_EXPORT string parseMarketinfo(string & str);

//4.�����г�����API(���ֱҽ����г�)
BCSAMPLE_EXPORT  string parseMarketlist(string & str);

//6.���ָ�����׵�����
//�������£���ߣ���� ��������ͽ�������ÿ10���Ӹ��� :
BCSAMPLE_EXPORT  string parseMarketTicker(string & str);

//7.ָ����Լ���������
//�������£���ߣ���� ��������ͽ�������30������ :
BCSAMPLE_EXPORT  string parseMarketOrderBooks(string & str);