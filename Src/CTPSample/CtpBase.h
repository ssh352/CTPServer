#pragma once

//#define WIN32

#ifdef WIN32
#ifdef CTPSAMPLE_LIB
#define CTPSAMPLE_EXPORT __declspec(dllexport)
#else
#define CTPSAMPLE_EXPORT __declspec(dllimport)

#endif

//#define CALLBACK __stdcall

#else

#define CTPSAMPLE_EXPORT
//#define CALLBACK
#endif

//// ��ͨ�ർ��
//class XJSON_API JObject����
//
////  ģ���ർ��
//
//template <class T>
//class XJSON_TEMPLATE JObjArray : public JObject��
//
//// ��������
//XJSON_API int GetVersion()����



