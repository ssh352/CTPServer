#pragma once

#include "BlcokSample.h"
#include <string>

using namespace  std;


BLCOKSAMPLE_EXPORT string parsePaires(string & str);

//3.�����г���������
BLCOKSAMPLE_EXPORT string parseMarketinfo(string & str);

//4.�����г�����API(���ֱҽ����г�)
BLCOKSAMPLE_EXPORT  string parseMarketlist(string & str);

//6.���ָ�����׵�����
//�������£���ߣ���� ��������ͽ�������ÿ10���Ӹ��� :
BLCOKSAMPLE_EXPORT  string parseMarketTicker(string & str);

//7.ָ����Լ���������
//�������£���ߣ���� ��������ͽ�������30������ :
BLCOKSAMPLE_EXPORT  string parseMarketOrderBooks(string & str);