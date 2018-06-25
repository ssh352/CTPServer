
#ifndef _SAMPLER_MSG_DEF_H_
#define _SAMPLER_MSG_DEF_H_
/**
 * ��Ϣ���ֶζ���
 * 
 * ��Ź���
 *  - 16λ������ʾ��ͬ��Ϣ�ֶ�
 *  - �����ֶδ�0x0001��ʼ��ţ��궨���ڱ�ͷ�ļ���
 *  - ˽���ֶδ�0x1000��ʼ��ţ�ÿ����Ŷΰ���0x0400����ţ��궨�岻�����ڱ�ͷ�ļ���
 *  - ˽���ֶεı�Ŷη������Ԥ���ڱ��ļ���ĩβ��ע
 */

/*
 * ��������ϢЭ��ʹ�õ�˽���ֶ�
 */
#define MSG_LOGIN_ID 		(int)0x00001000	// ��¼ID
#define MSG_LOGIN_PWD		(int)0x00001001	// ��¼����
#define MSG_LOGIN_PWD_ENC	(int)0x00001002	// ������ܷ�ʽ
#define MSG_LOGIN_RESULT	(int)0x00001003	// ��¼���
#define MSG_SUBSCRIP_RESULT	(int)0x00001004	// ���Ľ��

#define MSG_QUOTATION_RECS	(int)0x00001006	// �����¼
#define MSG_SUBSCRIP_RECS   (int)0x00001007 //���ĺ�Լ�б�
#define MSG_SUBSCRIP_TYPE   (int)0x00001008 //��������

#define MSG_HELLO_CONTENT   (int)0x00001009 //��������

// added by Jerry Lee, 2010-12-21, ������ʷ����
#define MSG_HISTORY_DATA    (int)0x00001010  //��ʷ���� 

// added by Jerry Lee, 2011-1-17, ����tick����
#define MSG_TICK_DATA		(int)0x00001011  //tick���� 

// added by Jerry Lee, 2011-2-13, ������Ѷ����
#define MSG_INFO_DATA		(int)0x00001012  //��Ѷ���� 

// added by Ben, 2011-5-29
#define MSG_BODY_NODEID		(int)0x00001013  //�˳����� 

#define MSG_IFH1_DATA		(int)0x00001014  //ģ��H1���ױ��Ķ����Ʒ�װ
#define MSG_IFH2_DATA		(int)0x00001015  //ģ��H2���ױ��Ķ����Ʒ�װ
#define MSG_SV_SUB_DATA		(int)0x00001016  //����/�˶������Ʒ�װ
#define MSG_DATE			(int)0x00001017  //
#define MSG_TIME			(int)0x00001018  //


#define YL_LOGIN				(unsigned int)0x00000001
#define YL_LOGIN_RSP			(unsigned int)0x80000001
#define YL_LOGOUT				(unsigned int)0x00000002
#define YL_LOGOUT_RSP			(unsigned int)0x80000002
#define YL_SUBSCRIP				(unsigned int)0x00000003
#define	YL_SUBSCRIP_RSP			(unsigned int)0x80000003
#define YL_UNSUBSCRIP			(unsigned int)0x00000004
#define	YL_UNSUBSCRIP_RSP		(unsigned int)0x80000004
#define YL_HELLO				(unsigned int)0x00000005
#define	YL_HELLO_RSP			(unsigned int)0x80000005

#define	YL_QUOTATION			(unsigned int)0x00000006

// added by Jerry Lee, 2010-12-21, ֧����ʷ���ݷ���
#define YL_HISTORYDATA          (unsigned int)0x00000007   

// added by Jerry Lee, 2011-1-17, ֧��tick���ݷ���
#define YL_TICKDATA             (unsigned int)0x00000008   

// added by Jerry Lee, 2011-2-13, ֧����Ѷ���ݷ���
#define YL_INFODATA             (unsigned int)0x00000009   

// added by Ben, 2011-5-29, ֧����Ѷ���ݷ���
#define YL_QUITMSG              (unsigned int)0x00000010 

#define YL_SV_REQ               (unsigned int)0x00000011
#define YL_SV_RSP               (unsigned int)0x80000011
#define YL_SV_NTF               (unsigned int)0x00000012
#define YL_SV_SUB_NTF           (unsigned int)0x00000013
#define YL_SV_UNSUB_NTF         (unsigned int)0x00000014
#define YL_SYNC_TIME			(unsigned int)0x00000015
#endif // _MSG_DEF_H_
