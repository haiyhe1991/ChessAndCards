#ifndef __CGameSocket_H__
#define __CGameSocket_H__

#ifdef WIN32  
#include <windows.h>  
#include <WinSock.h> 
#pragma comment(lib,"ws2_32.lib")
#else
#include <sys/socket.h>  
#include <fcntl.h>  
#include <errno.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  

#define SOCKET int  
#define SOCKET_ERROR -1  
#define INVALID_SOCKET -1  
#endif
//#ifndef CHECKF  
//#define CHECKF(x)  do   {   if (!(x)) { log_msg("CHECKF", #x, __FILE__, __LINE__); return 0;   } } while (0)
//#endif
#define _MAX_MSGSIZE 4 * 1024      // �ݶ�һ����Ϣ���Ϊ4k  
#define BLOCKSECONDS    30          // INIT��������ʱ��  
#define INBUFSIZE   (64*1024)       //����ߴ�������汨�����  �������ݵĻ���  
#define OUTBUFSIZE  (8*1024)        // ����ߴ�������汨������� �������ݵĻ��棬��������8Kʱ��FLUSHֻ��ҪSENDһ��  
class CGameSocket {  
public:  
	CGameSocket(void); 
	/*************************************
	�������ܣ�����socket,�������ӷ�����,����������
	����������1��������ip��ַ������2���������˿ںţ�����3�����ӷ�������������ʱ�䣬����4��
	����ֵ����Ϊ�������ӳɹ�����Ϊ��������ʧ��
	**************************************/
	bool    Create(const char* pszServerIP, int nServerPort, int nBlockSec = BLOCKSECONDS, bool bKeepAlive = false);  
	bool    SendMsg(void* pBuf, int nSize);  
	bool    ReceiveMsg(void* pBuf, int& nSize);  
	bool    Flush(void);  
	bool    Check(void);  
	void    Destroy(void);  
	SOCKET  GetSocket(void) const { return m_sockClient; }  
private:  
	bool    recvFromSock(void);     // �������ж�ȡ�����ܶ������  
	bool    hasError();         // �Ƿ�������ע�⣬�첽ģʽδ��ɷǴ���  
	void    closeSocket();  

	SOCKET  m_sockClient;  

	// �������ݻ���  
	char    m_bufOutput[OUTBUFSIZE];    //? ���Ż�Ϊָ������  
	int     m_nOutbufLen;  

	// ���λ�����  
	char    m_bufInput[INBUFSIZE];  
	int     m_nInbufLen;  
	int     m_nInbufStart;              // INBUFʹ��ѭ��ʽ���У��ñ���Ϊ������㣬0 - (SIZE-1)  
};  
#endif