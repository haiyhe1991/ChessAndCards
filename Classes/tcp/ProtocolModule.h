#ifndef __PROTOCOL_MODULE_H__
#define __PROTOCOL_MODULE_H__


typedef unsigned short  UINT16;
typedef unsigned char  BYTE;

#define DEFAULT_TOKEN_KEY	"CYA_DEFAULT_TOKEN_KEY_1234567890"	//Ĭ����Կ
#define DEFAULT_KEY_SIZE	32		//��Կ����
#define DEFAULT_TOKEN_SIZE  4		//���Ƴ���

#pragma pack(push, 1)


/*********************************************************************
�������ݽṹ
**********************************************************************/
enum ESGSPacketType
{
	DATA_PACKET = 0,				//���ݰ�
	DATA_LINKER_GROUP_PACKET,		//linkerȺ����
	DATA_RELAY_FAILED_PACKET,		//ת������ʧ��
	DATA_HEARTBEAT_PACKET,			//������
	DATA_PING_PACKET,				//ping (�ⲿЭ������)
	DATA_CONFIRM_PACKET,			//������֤��(�ڲ�ͨ��Э��)
	DATA_TOKEN_KEY_PACKET,			//���ư�����Կ
	DATA_CTRL_PACKET,				//���ư�(�ڲ�ʹ��,�ⲿ��ֹʹ��)
};

struct SGSProtocolHead	//����Э��ͷ
{
	UINT16 pduLen : 12;	//���ݳ���
	UINT16 pktType : 3;	//������
	UINT16 isCrypt : 1;	//�Ƿ����
	void hton()
	{
		BYTE* pThis = (BYTE*)this;
		*(UINT16*)pThis = htons(*(UINT16*)pThis);
	}

	void ntoh()
	{
		BYTE* pThis = (BYTE*)this;
		*(UINT16*)pThis = ntohs(*(UINT16*)pThis);
	}
};

struct SGSReqPayload	//��������
{
	UINT16 rawDataBytes;	//ԭʼ����(δ����)����
	UINT32 token;			//����
	UINT16 cmdCode;			//������
	BYTE data[1];			//������

	void hton()
	{
		rawDataBytes = htons(rawDataBytes);
		token = htonl(token);
		cmdCode = htons(cmdCode);
	}

	void ntoh()
	{
		rawDataBytes = ntohs(rawDataBytes);
		token = ntohl(token);
		cmdCode = ntohs(cmdCode);
	}
};

struct SGSResPayload	//��Ӧ����
{
	UINT16 rawDataBytes;	//ԭʼ����(δ����)����
	UINT32 token;			//����
	UINT16 cmdCode;			//������
	UINT16 retCode;			//������
	BYTE   data[1];			//������

	void hton()
	{
		rawDataBytes = htons(rawDataBytes);
		token = htonl(token);
		cmdCode = htons(cmdCode);
		retCode = htons(retCode);
	}

	void ntoh()
	{
		rawDataBytes = ntohs(rawDataBytes);
		token = ntohl(token);
		cmdCode = ntohs(cmdCode);
		retCode = ntohs(retCode);
	}
};

#pragma pack(pop)

#define SGS_PROTO_HEAD_LEN	(sizeof(SGSProtocolHead))				//SGSЭ��ͷ��
#define SGS_REQ_HEAD_LEN	(sizeof(SGSReqPayload) - sizeof(BYTE))	//SGS����ͷ��
#define SGS_RES_HEAD_LEN	(sizeof(SGSResPayload) - sizeof(BYTE))	//SGS��Ӧͷ��
#define SGS_BASIC_REQ_LEN	(SGS_PROTO_HEAD_LEN + SGS_REQ_HEAD_LEN)	//SGS�����������ݳ���
#define SGS_BASIC_RES_LEN	(SGS_PROTO_HEAD_LEN + SGS_RES_HEAD_LEN)	//SGS������Ӧ���ݳ���
#define MAX_RES_USER_BYTES	(2048 - SGS_PROTO_HEAD_LEN - SGS_RES_HEAD_LEN - 15)	//�ͻ������������ݳ���
#define MAX_RES_INNER_USER_BYTES	(4096 - SGS_PROTO_HEAD_LEN - SGS_RES_HEAD_LEN - 15)	//�ͻ������������ݳ���

#endif