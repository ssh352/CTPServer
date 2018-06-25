#ifndef _SAMPLER_PACKET_H
#define _SAMPLER_PACKET_H
#include "CommApSampler.h"
#include "MsgBlock.h"
#include "MsgDef.h"
#include "SamplerMsgDef.h"
#include "strutils.h"

using namespace strutils;

//
typedef struct tagSamplerPktHead   //����ͷ
{
	unsigned int uiLen;
	unsigned int uiCmdID;
	unsigned int uiSeqID;
	unsigned int uiNodeID;
} SAMPLER_PKT_HEADER,*PSAMPLER_PKT_HEADER;
#define CODE_LEN  16
#define NAME_LEN  32
typedef struct tagCodeInfo
{
	unsigned int m_usMarketType;			//�г�����
	unsigned int m_uiUnit;					//ÿ�ֵ�λ
	char           m_acCode[CODE_LEN];		//��Լ����
	char           m_acName[NAME_LEN];		//��Լ����
}CODE_INFO,*PCODE_INFO;

typedef struct	tagBidAskUnit
{ 
	tagBidAskUnit()
	{
		m_uiVol = 0;
		m_uiPrice = 0;
	}
	unsigned int m_uiVol;		//ί��
	unsigned int m_uiPrice;	    //ί��
} BID_ASK_UNIT,*PBID_ASK_UNIT;

typedef struct tagQuotation
{
	CODE_INFO m_CodeInfo;		//��Լ��Ϣ[8]

	unsigned int m_uiSeqNo;     //����
	unsigned int m_uiDate;
	unsigned int m_uiTime; 
	unsigned int m_uilastClose; //����
	unsigned int m_uiLastSettle;//���
	unsigned int m_uiSettle;    //����
	unsigned int m_uiOpenPrice; //����
	unsigned int m_uiHigh;      //��߼�
	unsigned int m_uiLow;       //��ͼ�
	unsigned int m_uiClose;     //���̼�
	unsigned int m_uiHighLimit;
	unsigned int m_uiLowLimit;	
	unsigned int m_uiLast;      //���¼�
	unsigned int m_uiAverage;   //����
	unsigned int m_uiVolume;    //�ɽ���
	unsigned int m_uiWeight;    //�ɽ�����
	unsigned int m_uiTurnOver;  //�ɽ����
	unsigned int m_uiChiCangLiang;//�ֲ���
	unsigned int m_uiLastChiCangLiang;//���ճֲ���
	BID_ASK_UNIT     m_Bid[5];
	BID_ASK_UNIT     m_Ask[5];

	string Key() const
	{
		string sKey;
		sKey.assign((const char*)(&m_CodeInfo.m_usMarketType),sizeof(m_CodeInfo.m_usMarketType));
		sKey.append(m_CodeInfo.m_acCode, sizeof(m_CodeInfo.m_acCode)); 
		return sKey;
	}
	void Decode(const char* pBuf, unsigned int uiSize)
	{
		*this = *(QUOTATION*)(pBuf);
	}
	const char* Encode(unsigned int & uiLen)
	{
		uiLen = sizeof(QUOTATION);
		return (const char*)(this);
	}
} QUOTATION,*PQUOTATION;


typedef struct SubscriberContext
{
	unsigned int nodeId;
	char subType;		/*0--ֻ�����оٲ���,1-��ָ���б���,*--����������Ϣ */
	vector<unsigned int> vecInstId;  //��ԼID�б�s
}SUB_CONTEXT,*PSUB_CONTEXT;

//#define  FIELDKEY_LASTSETTLE  0x0000
//#define  FIELDKEY_LASTCLOSE   0x0001
//#define  FIELDKEY_OPEN        0x0002
//#define  FIELDKEY_HIGH        0x0003
//#define  FIELDKEY_LOW         0x0004
//#define  FIELDKEY_LAST        0x0005
//#define  FIELDKEY_CLOSE       0x0006
//#define  FIELDKEY_SETTLE      0x0007
//#define  FIELDKEY_BID1        0x0008
//#define  FIELDKEY_BIDLOT1     0x0009
//#define  FIELDKEY_BID2        0x000A
//#define  FIELDKEY_BIDLOT2     0x000B
//#define  FIELDKEY_BID3        0x000C
//#define  FIELDKEY_BIDLOT3     0x000D
//#define  FIELDKEY_BID4        0x000E
//#define  FIELDKEY_BIDLOT4     0x000F
//#define  FIELDKEY_BID5        0x0010
//#define  FIELDKEY_BIDLOT5     0x0011
//#define  FIELDKEY_ASK1        0x0012
//#define  FIELDKEY_ASKLOT1     0x0013
//#define  FIELDKEY_ASK2        0x0014
//#define  FIELDKEY_ASKLOT2     0x0015
//#define  FIELDKEY_ASK3        0x0016
//#define  FIELDKEY_ASKLOT3     0x0017
//#define  FIELDKEY_ASK4        0x0018
//#define  FIELDKEY_ASKLOT4     0x0019
//#define  FIELDKEY_ASK5        0x001A
//#define  FIELDKEY_ASKLOT5     0x001B
//#define  FIELDKEY_VOLUME      0x001C
//#define  FIELDKEY_WEIGHT      0x001D
//#define  FIELDKEY_HIGHLIMIT   0x001E
//#define  FIELDKEY_LOWLIMIT    0x001F
//#define  FIELDKEY_POSI        0x0020
//#define  FIELDKEY_UPDOWN      0x0021
//#define  FIELDKEY_TURNOVER    0x0022
//#define  FIELDKEY_AVERAGE     0x0023
//#define  FIELDKEY_SEQUENCENO  0x0024
//#define  FIELDKEY_QUOTETIME   0x0025
//#define  FIELDKEY_UPDOWNRATE  0x0026
//#define  FIELDKEY_QUOTEDATE   0x0027

typedef enum
{
    FIELDKEY_LASTSETTLE = 0x0000,
    FIELDKEY_LASTCLOSE  = 0x0001,
    FIELDKEY_OPEN       = 0x0002,
    FIELDKEY_HIGH       = 0x0003,
    FIELDKEY_LOW        = 0x0004,
    FIELDKEY_LAST       = 0x0005,
    FIELDKEY_CLOSE      = 0x0006,
    FIELDKEY_SETTLE     = 0x0007,
    FIELDKEY_BID1       = 0x0008,
    FIELDKEY_BIDLOT1    = 0x0009,
    FIELDKEY_BID2       = 0x000A,
    FIELDKEY_BIDLOT2    = 0x000B,
    FIELDKEY_BID3       = 0x000C,
    FIELDKEY_BIDLOT3    = 0x000D,
    FIELDKEY_BID4       = 0x000E,
    FIELDKEY_BIDLOT4    = 0x000F,
    FIELDKEY_BID5       = 0x0010,
    FIELDKEY_BIDLOT5    = 0x0011,
    FIELDKEY_ASK1       = 0x0012,
    FIELDKEY_ASKLOT1    = 0x0013,
    FIELDKEY_ASK2       = 0x0014,
    FIELDKEY_ASKLOT2    = 0x0015,
    FIELDKEY_ASK3       = 0x0016,
    FIELDKEY_ASKLOT3    = 0x0017,
    FIELDKEY_ASK4       = 0x0018,
    FIELDKEY_ASKLOT4    = 0x0019,
    FIELDKEY_ASK5       = 0x001A,
    FIELDKEY_ASKLOT5    = 0x001B,

    FIELDKEY_VOLUME     = 0x001C,
    FIELDKEY_WEIGHT     = 0x001D,
    FIELDKEY_HIGHLIMIT  = 0x001E,
    FIELDKEY_LOWLIMIT   = 0x001F,
    FIELDKEY_POSI       = 0x0020,
    FIELDKEY_UPDOWN     = 0x0021,
    FIELDKEY_TURNOVER   = 0x0022,
    FIELDKEY_AVERAGE    = 0x0023,
    FIELDKEY_SEQUENCENO = 0x0024,
    FIELDKEY_QUOTETIME  = 0x0025,
    FIELDKEY_UPDOWNRATE = 0x0026,

    FIELDKEY_QUOTEDATE  = 0x0027,
	FIELDKEY_UNIT		= 0x0028,
	
	//FIELDKEY_BID6       = 0x0027,
 //   FIELDKEY_BIDLOT6    = 0x0028,
 //   FIELDKEY_BID7       = 0x0029,
 //   FIELDKEY_BIDLOT7    = 0x002A,
 //   FIELDKEY_BID8       = 0x002B,
 //   FIELDKEY_BIDLOT8    = 0x002C,
 //   FIELDKEY_BID9       = 0x002D,
 //   FIELDKEY_BIDLOT9    = 0x002E,
 //   FIELDKEY_BID10      = 0x002F,
 //   FIELDKEY_BIDLOT10   = 0x0030,
 //   FIELDKEY_ASK6       = 0x0031,
 //   FIELDKEY_ASKLOT6    = 0x0032,
 //   FIELDKEY_ASK7       = 0x0033,
 //   FIELDKEY_ASKLOT7    = 0x0034,
 //   FIELDKEY_ASK8       = 0x0035,
 //   FIELDKEY_ASKLOT8    = 0x0036,
 //   FIELDKEY_ASK9       = 0x0037,
 //   FIELDKEY_ASKLOT9    = 0x0038,
 //   FIELDKEY_ASK10      = 0x0039,
 //   FIELDKEY_ASKLOT10   = 0x003A,

	//FIELDKEY_QUOTEDATE  = 0x003B,
	//FIELDKEY_UNIT		= 0x003C,
    FIELDKEY_UNKNOWN
} ENUM_FIELDKEY;

#define QUOTATION_REC_SIZE sizeof(QUOTATION) //128

// added by Jerry Lee, 2010-12-24, ֧����ʷ���ݵĽṹ����
// begin
// ��K������
typedef struct tagStockDay
{
	unsigned long	m_lDate;  			/*year-month-day ,example: 19960616
								�������ݵı�ʾ�������£�yymmddhhnn(������ʱ��)
								yyָ����year - 1990������ݱ�ﷶΧ��1990 - 2011
								��0905131045��ָ���ǣ�1999��5��13��10��45�֡�*/
	long	m_lOpenPrice;		//��
	long	m_lMaxPrice;		//��
	long	m_lMinPrice;		//��
	long	m_lClosePrice;		//��
	long	m_lMoney;			//�ɽ����
	unsigned long	m_lTotal;	//�ɽ���   ��λ���ٹɣ��֣�

	//#ifdef SUPPORT_NETVALUE
	long	m_lNationalDebtRatio; // ��ծ����(��λΪ0.1��),����ֵ
	//#endif

	union
	{
		long	m_lPrevClose;
		struct
		{
			short	m_nMessageMask;	//������Ϣ����
			short	m_nOpenVolumn;  //���̳ɽ���
		};
	};

	union
	{
		struct
		{
			short  m_nVolAmount;   //�ɽ�����
			short  m_nZeroVol;	   //�Ե��ɽ�����
		};
		long* m_pDataEx;		   // ����ǳ�Ȩ���� ChuQuanData �ṹ����ָ��
	};
} StockDayT;


// ��ʷ���ݽṹͷ
typedef struct tagHistoryDataHeader
{
    char marketType[18];      // �г�����, GP: ��Ʊ HJ: �ƽ� QH: �ڻ� WH: ��� WP: ����   kenny   18->2
    char productCode[6];     // Ʒ�ִ���
    char dataType[8];        // ��������, min1: ����, min15: 15����, min30: 30����, min60: 60����, min120: 120����
                             // min180: 180����, Day: ����, Week: ����, Month: ����, Season: ����, Year: ����
                             // tick: ʵ��, info: ��Ѷ  
    int length;              // content���ݳ���
} HistoryDataHeader;


// ��ʷ���ݽṹ����, ��convergenceʹ��
typedef struct tagHistoryData
{
    HistoryDataHeader header; 
    char *content;        // ���� 
} HistoryData;


// ��ʷ���ݽṹ����, ��samplerд���ڴ���к�recv��ȡ�ڴ����ʱʹ��
typedef struct tagHistoryDataBuf
{
    char content[512];

    string Key() const
    {
        string sKey;
        sKey.assign((const char*)(&content), sizeof(int));
        return sKey;
    }
    void Decode(const char* pBuf, unsigned int uiSize)
    {
        *this = *(HistoryDataBuf*)(pBuf);
    }
    const char* Encode(unsigned int& uiLen)
    {
        uiLen = sizeof(HistoryDataBuf);
        return (const char*)(this);
    }
} HistoryDataBuf;

// end

// added by Jerry Lee, 2011-1-17, tick���ݰ��ṹ
// begin

// tick���ݽṹ����, ��convergenceʹ��
typedef struct tagTickData
{
    HistoryDataHeader header;   // ����ͷ������ʷ������ͬ 
    int fileSize;               // ԭʼ�ļ��Ĵ�С
    int seqNo;                  // ���ݰ����к�, ��0��ʼ����
    int zipSize;                // ѹ�����ļ��Ĵ�С
    char *content;              // ���� 
} TickData;

// end

// added by Jerry Lee, 2011-2-13, ��Ѷ���ݰ��ṹ
// begin

// ��Ѷ
typedef struct tagInfoData
{
    HistoryDataHeader header;  
    int dateTime;         // ʱ��
    char title[128];      // ����
    char *content;        // ����
} InfoData;


// end

class COMMAPSAMPLER_CLASS CSamplerPacket : public CMsgBlock, public CPacketRouteable
{
public:
	CSamplerPacket();
	CSamplerPacket(unsigned int uiCmdID);
	CSamplerPacket(CMessage& msg, unsigned int uiCmdID = 0xFFFFFFFF);
	virtual ~CSamplerPacket(void);

	const string& GetCmdID();
	std::string RouteKey();

	void  Decode(const char * pData, unsigned int nLength);
	const char* Encode(unsigned int & usLen);


private:
	string m_sCmdID;

public:

	// ������Ϣ�����뺯��
	BEGIN_PKG_DECODE(4, 4)
		PKG_DECODE_HANDLE(YL_LOGIN, Login)
		PKG_DECODE_HANDLE(YL_LOGIN_RSP, LoginRsp)
		PKG_DECODE_HANDLE(YL_LOGOUT, Logout)
		PKG_DECODE_HANDLE(YL_LOGOUT_RSP, LogoutRsp)
		PKG_DECODE_HANDLE(YL_SUBSCRIP, Subscrip)
		PKG_DECODE_HANDLE(YL_SUBSCRIP_RSP, SubscripRsp)
		PKG_DECODE_HANDLE(YL_UNSUBSCRIP, UnSubscrip)
		PKG_DECODE_HANDLE(YL_UNSUBSCRIP_RSP, UnSubscripRsp)
		PKG_DECODE_HANDLE(YL_HELLO, Hello)
		PKG_DECODE_HANDLE(YL_HELLO_RSP, HelloRsp)
		PKG_DECODE_HANDLE(YL_QUOTATION, Quotation)
        // added by Jerry Lee, 2010-12-21, ������ʷ���ݴ���
        PKG_DECODE_HANDLE(YL_HISTORYDATA, HistoryDataPkg)
        // added by Jerry Lee, 2011-1-17, ����tick���ݴ���
        PKG_DECODE_HANDLE(YL_TICKDATA, TickDataPkg)
        // added by Jerry Lee, 2011-2-13, ������Ѷ���ݴ���
        PKG_DECODE_HANDLE(YL_INFODATA, InfoDataPkg)
		// added by Ben, 2011-5-29, �����˳�����
		PKG_DECODE_HANDLE(YL_QUITMSG, QuitMsgPkg)
		//ͬ��ʱ��
		PKG_DECODE_HANDLE(YL_SYNC_TIME, SyncTimeCmd)


		PKG_DECODE_HANDLE(YL_SV_REQ, IfSvReq)
		PKG_DECODE_HANDLE(YL_SV_RSP, IfSvRsp)
		PKG_DECODE_HANDLE(YL_SV_NTF, IfSvNtf)
		PKG_DECODE_HANDLE(YL_SV_SUB_NTF, SvSubNtf)
		PKG_DECODE_HANDLE(YL_SV_UNSUB_NTF, SvUnSubNtf)
	END_PKG_DECODE()

	// ������Ϣ�����뺯��
	BEGIN_PKG_ENCODE
		PKG_ENCODE_HANDLE(YL_LOGIN, Login)
		PKG_ENCODE_HANDLE(YL_LOGIN_RSP, LoginRsp)
		PKG_ENCODE_HANDLE(YL_LOGOUT, Logout)
		PKG_ENCODE_HANDLE(YL_LOGOUT_RSP, LogoutRsp)
		PKG_ENCODE_HANDLE(YL_SUBSCRIP, Subscrip)
		PKG_ENCODE_HANDLE(YL_SUBSCRIP_RSP, SubscripRsp)
		PKG_ENCODE_HANDLE(YL_UNSUBSCRIP, UnSubscrip)
		PKG_ENCODE_HANDLE(YL_UNSUBSCRIP_RSP, UnSubscripRsp)
		PKG_ENCODE_HANDLE(YL_HELLO, Hello)
		PKG_ENCODE_HANDLE(YL_HELLO_RSP, HelloRsp)
		PKG_ENCODE_HANDLE(YL_QUOTATION, Quotation)
        // added by Jerry Lee, 2010-12-21, ������ʷ���ݴ���
        PKG_ENCODE_HANDLE(YL_HISTORYDATA, HistoryDataPkg)
        // added by Jerry Lee, 2011-1-17, ����tick���ݴ���
        PKG_ENCODE_HANDLE(YL_TICKDATA, TickDataPkg)
        // added by Jerry Lee, 2011-2-13, ������Ѷ���ݴ���
        PKG_ENCODE_HANDLE(YL_INFODATA, InfoDataPkg)
		// added by Ben, 2011-5-29, �����˳�����
		PKG_ENCODE_HANDLE(YL_QUITMSG, QuitMsgPkg)
		//ͬ��ʱ��
		PKG_ENCODE_HANDLE(YL_SYNC_TIME, SyncTimeCmd)

		PKG_ENCODE_HANDLE(YL_SV_REQ, IfSvReq)
		PKG_ENCODE_HANDLE(YL_SV_RSP, IfSvRsp)
		PKG_ENCODE_HANDLE(YL_SV_NTF, IfSvNtf)
		PKG_ENCODE_HANDLE(YL_SV_SUB_NTF, SvSubNtf)
		PKG_ENCODE_HANDLE(YL_SV_UNSUB_NTF, SvUnSubNtf)
	END_PKG_ENCODE()

	// ����ÿ��ͨ��ԭ��
	// Header  
#define SAMP_HEADER() \
		FIELD_INT32(MSG_PKG_LENGTH) \
		FIELD_INT32(MSG_CMD_ID) \
		FIELD_INT32(MSG_SEQ_ID) \
		FIELD_INT32(MSG_NODE_ID)

	// Login  
	BEGIN_PKG_FIELD(Login)
		SAMP_HEADER()
		FIELD_OCTSTR(MSG_LOGIN_ID, 20)
		FIELD_OCTSTR(MSG_LOGIN_PWD, 20)
		FIELD_INT32(MSG_LOGIN_PWD_ENC)
	END_PKG_FIELD()

    //Login  Rsp
	BEGIN_PKG_FIELD(LoginRsp)
		SAMP_HEADER()
		FIELD_INT32(MSG_LOGIN_RESULT)
	END_PKG_FIELD()

	// Logout
	BEGIN_PKG_FIELD(Logout)
		SAMP_HEADER()
		FIELD_OCTSTR(MSG_LOGIN_ID, 20)
		FIELD_OCTSTR(MSG_LOGIN_PWD, 20)
		FIELD_INT32(MSG_LOGIN_PWD_ENC)
	END_PKG_FIELD()

    //Logout  Rsp
	BEGIN_PKG_FIELD(LogoutRsp)
		SAMP_HEADER()
		FIELD_INT32(MSG_LOGIN_RESULT)
	END_PKG_FIELD()

    // Subscrip
	BEGIN_PKG_FIELD(Subscrip)
		SAMP_HEADER()
		FIELD_OCTSTR(MSG_SUBSCRIP_TYPE,sizeof(char))
		FIELD_REC8(MSG_SUBSCRIP_RECS,sizeof(unsigned int))
	END_PKG_FIELD()

    // SubscripRsp
	BEGIN_PKG_FIELD(SubscripRsp)
		SAMP_HEADER()
		FIELD_INT32(MSG_SUBSCRIP_RESULT)
	END_PKG_FIELD()

	// UnSubscrip
	BEGIN_PKG_FIELD(UnSubscrip)
		SAMP_HEADER()
		FIELD_REC8(MSG_SUBSCRIP_RECS,sizeof(unsigned int))
	END_PKG_FIELD()

    // UnSubscripRsp
	BEGIN_PKG_FIELD(UnSubscripRsp)
		SAMP_HEADER()
		FIELD_INT32(MSG_SUBSCRIP_RESULT)
	END_PKG_FIELD()

	// Hello
	BEGIN_PKG_FIELD(Hello)
		SAMP_HEADER()
		FIELD_VLD16(MSG_HELLO_CONTENT)
	END_PKG_FIELD()

    // HelloRsp
	BEGIN_PKG_FIELD(HelloRsp)
		SAMP_HEADER()
		FIELD_VLD16(MSG_HELLO_CONTENT)
	END_PKG_FIELD()

	// Quotaion
	BEGIN_PKG_FIELD(Quotation)
		SAMP_HEADER()
		FIELD_VLD16(MSG_QUOTATION_RECS);
	END_PKG_FIELD()

    // added by Jerry Lee, 2010-12-21
    // History Data
    BEGIN_PKG_FIELD(HistoryDataPkg)
        SAMP_HEADER()
        FIELD_VLD32(MSG_HISTORY_DATA);
    END_PKG_FIELD()

    // added by Jerry Lee, 2011-1-17
    // History Data
    BEGIN_PKG_FIELD(TickDataPkg)
        SAMP_HEADER()
        FIELD_VLD32(MSG_TICK_DATA);
    END_PKG_FIELD()

    // added by Jerry Lee, 2011-2-13
    // Information Data
    BEGIN_PKG_FIELD(InfoDataPkg)
        SAMP_HEADER()
        FIELD_VLD32(MSG_INFO_DATA);
    END_PKG_FIELD()

	// added by Ben, 2011-5-29
	// Information Data
	BEGIN_PKG_FIELD(QuitMsgPkg)
		SAMP_HEADER()
		FIELD_INT32(MSG_BODY_NODEID);
	END_PKG_FIELD()

	//ͬ��ʱ��
	BEGIN_PKG_FIELD(SyncTimeCmd)
		SAMP_HEADER()
		FIELD_INT32(MSG_BODY_NODEID);
		FIELD_INT32(MSG_DATE);
		FIELD_INT32(MSG_TIME);
	END_PKG_FIELD()
		

	BEGIN_PKG_FIELD(IfSvReq)
		SAMP_HEADER()
		FIELD_VLD16(MSG_IFH1_DATA);
	END_PKG_FIELD()

	BEGIN_PKG_FIELD(IfSvRsp)
		SAMP_HEADER()
		FIELD_VLD16(MSG_IFH1_DATA);
	END_PKG_FIELD()

	BEGIN_PKG_FIELD(IfSvNtf)
		SAMP_HEADER()
		FIELD_VLD16(MSG_IFH2_DATA);
	END_PKG_FIELD()

	BEGIN_PKG_FIELD(SvSubNtf)
		SAMP_HEADER()
		FIELD_VLD16(MSG_SV_SUB_DATA);
	END_PKG_FIELD()

	BEGIN_PKG_FIELD(SvUnSubNtf)
		SAMP_HEADER()
		FIELD_VLD16(MSG_SV_SUB_DATA);
	END_PKG_FIELD()
};

#endif