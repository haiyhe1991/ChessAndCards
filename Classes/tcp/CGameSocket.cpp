//http://blog.csdn.net/langresser_king/article/details/8646088
#include "CGameSocket.h"
CGameSocket::CGameSocket()  
{   
	// 初始化  
	memset(m_bufOutput, 0, sizeof(m_bufOutput));  
	memset(m_bufInput, 0, sizeof(m_bufInput));  
}  

void CGameSocket::closeSocket()  
{  
#ifdef WIN32  
	closesocket(m_sockClient);  
	WSACleanup();  
#else  
	close(m_sockClient);  
#endif  
}  

bool CGameSocket::Create(const char* pszServerIP, int nServerPort, int nBlockSec, bool bKeepAlive /*= FALSE*/)  
{  
	// 检查参数  
	if(pszServerIP == 0 || strlen(pszServerIP) > 15) {  
		return false;  
	}  

#ifdef WIN32  
	WSADATA wsaData;  
	WORD version = MAKEWORD(2, 0);  
	int ret = WSAStartup(version, &wsaData);//win sock start up  
	if (ret != 0) {  //检查套接字库是否申请成功
		return false;  
	}  
#endif  

	// 创建主套接字  参数说明：参数1：网络类型，tcp.udp..第二个参数：字节流传输，参数3：tcp协议
	m_sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//如果socket创建失败
	if(m_sockClient == INVALID_SOCKET) {  
		closeSocket();  
		return false;  
	}  

	// 设置SOCKET为KEEPALIVE  
	if(bKeepAlive)  
	{  
		int     optval=1;  
		//http://blog.csdn.net/chary8088/article/details/2486377
		//http://blog.csdn.net/l_yangliu/article/details/7086256
		//参数1：指向一个打开的套接口描述字，参数2：指向一个基本的套接口，参数3：保持连接 int,检 测对方主机是否崩溃，避免（服务器）永远阻塞于TCP连接的输入。
		//返回值：成功执行时，返回0。失败返回-1，
		if(setsockopt(m_sockClient, SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval)))  
		{  
			closeSocket();  
			return false;  
		}  
	}  

#ifdef WIN32  
	DWORD nMode = 1;  
	//http://www.cnblogs.com/endeavour/archive/2011/11/18/2253529.html
	//参数2允许或禁止非阻塞，并且参数3存了该函数的返回值，参数3：0为阻塞，非0 为非阻塞
	int nRes = ioctlsocket(m_sockClient, FIONBIO, &nMode);  
	if (nRes == SOCKET_ERROR) {  
		closeSocket();  
		return false;  
	}  
#else  
	// 设置为非阻塞方式  
	fcntl(m_sockClient, F_SETFL, O_NONBLOCK);  
#endif  

	unsigned long serveraddr = inet_addr(pszServerIP);  
	if(serveraddr == INADDR_NONE)   // 检查IP地址格式错误  
	{  
		closeSocket();  
		return false;  
	}  

	sockaddr_in addr_in;  
	memset((void *)&addr_in, 0, sizeof(addr_in));  
	addr_in.sin_family = AF_INET;  
	addr_in.sin_port = htons(nServerPort);  
	addr_in.sin_addr.s_addr = serveraddr;  

	if(connect(m_sockClient, (sockaddr *)&addr_in, sizeof(addr_in)) == SOCKET_ERROR) {  
		if (hasError()) {  
			closeSocket();  
			return false;  
		}  
		else    // WSAWOLDBLOCK  
		{  
			timeval timeout;  
			timeout.tv_sec  = nBlockSec;  
			timeout.tv_usec = 0;  
			fd_set writeset, exceptset;  
			FD_ZERO(&writeset);  
			FD_ZERO(&exceptset);  
			FD_SET(m_sockClient, &writeset);  
			FD_SET(m_sockClient, &exceptset);  

			int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);  
			if (ret == 0 || ret < 0) {  
				closeSocket();  
				return false;  
			} else  // ret > 0  
			{  
				ret = FD_ISSET(m_sockClient, &exceptset);  
				if(ret)     // or (!FD_ISSET(m_sockClient, &writeset)  
				{  
					closeSocket();  
					return false;  
				}  
			}  
		}  
	}  

	m_nInbufLen     = 0;  
	m_nInbufStart   = 0;  
	m_nOutbufLen    = 0;  

	struct linger so_linger;  
	so_linger.l_onoff = 1;  
	so_linger.l_linger = 500;  
	//若设置了SO_LINGER并确定了非零的超时间 隔，则closesocket()调用阻塞进程，直到所剩数据发送完毕或超时。
	setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));  

	return true;  
}  
/************************************
函数功能：将要发送的数据拷贝在发送缓冲区里，如果缓冲区放不下了，那么马上提交一次数据，将缓冲区空间清理好容纳即将进来的字节
参数1：将要发送的数据的数组首地址，参数2：将要发送的字节长度
返回值：拷贝成功：返回真，如果拷贝不成功，那么返回false
注意：此方法不是真正的向服务器提交需要发送的数据，只是拷贝到本地缓存。（缓冲区装不下除外）
*************************************/
bool CGameSocket::SendMsg(void* pBuf, int nSize)  
{  
	if(pBuf == 0 || nSize <= 0) {  
		return false;  
	}  

	if (m_sockClient == INVALID_SOCKET) {  
		return false;  
	}  

	// 检查通讯消息包长度  
	int packsize = 0;  
	packsize = nSize;  

	// 检测BUF溢出  
	if(m_nOutbufLen + nSize > OUTBUFSIZE) {  
		// 立即发送OUTBUF中的数据，以清空OUTBUF。  
	//m_nOutbufLen += nSize;  
		Flush();  
		if(m_nOutbufLen + nSize > OUTBUFSIZE) {  
			// 出错了  
			Destroy();  
			return false;  
		}  
	}  
	// 数据添加到BUF尾  
	memcpy(m_bufOutput + m_nOutbufLen, pBuf, nSize);  
	m_nOutbufLen += nSize;  //需要发送的字节又长了
	return true;  
}  
/************************************
函数功能：向服务器请求数据
返回值：拷贝成功：返回真，如果拷贝不成功，那么返回false
注意：此方法是将服务器数据请求到本地缓存，然后再拷贝到用户缓存
*************************************/
bool CGameSocket::ReceiveMsg(void* pBuf, int& nSize)  
{  
	//检查参数  
	if(pBuf == NULL || nSize <= 0) {  
		return false;  
	}  

	if (m_sockClient == INVALID_SOCKET) {  
		return false;  
	}  

	// 检查是否有一个消息(小于2则无法获取到消息长度)  
	if(m_nInbufLen < 2) {  
		//  如果没有请求成功  或者   如果没有数据则直接返回  
		if(!recvFromSock() || m_nInbufLen < 2) {     // 这个m_nInbufLen更新了  
			return false;  
		}  
	}  

	//// 计算要拷贝的消息的大小（一个消息，大小为整个消息的第一个16字节），因为环形缓冲区，所以要分开计算  
	//int packsize = (unsigned char)m_bufInput[m_nInbufStart] +  
	//	(unsigned char)m_bufInput[(m_nInbufStart + 1) % INBUFSIZE] * 256; // 注意字节序，高位+低位  
	//项目不一样，暂时替换。0-12bit是消息长度，12-15是消息类型，15-16是消息是否加密，而且消息长度里面不带这个协议头长度，所以要+2。而且经过了字节顺序转换
	int packsize = (unsigned char)m_bufInput[(m_nInbufStart + 1) % INBUFSIZE] + ((unsigned char)m_bufInput[m_nInbufStart] & 0x0f) * 256 + 2;

	// 检测消息包尺寸错误 暂定最大16k  
	if (packsize <= 0 || packsize > _MAX_MSGSIZE) {  
		m_nInbufLen = 0;        // 直接清空INBUF  
		m_nInbufStart = 0;  
		return false;  
	}  

	// 检查消息是否完整(如果将要拷贝的消息大于此时缓冲区数据长度，需要再次请求接收剩余数据)  
	if (packsize > m_nInbufLen) {  
		// 如果没有请求成功   或者    依然无法获取到完整的数据包  则返回，直到取得完整包  
		if (!recvFromSock() || packsize > m_nInbufLen) { // 这个m_nInbufLen已更新  
			return false;  
		}  
	}  

	// 复制出一个消息  
	if(m_nInbufStart + packsize > INBUFSIZE) {  
		// 如果一个消息有回卷（被拆成两份在环形缓冲区的头尾）  
		// 先拷贝环形缓冲区末尾的数据  
		int copylen = INBUFSIZE - m_nInbufStart;  
		memcpy(pBuf, m_bufInput + m_nInbufStart, copylen);  

		// 再拷贝环形缓冲区头部的剩余部分  
		memcpy((unsigned char *)pBuf + copylen, m_bufInput, packsize - copylen);  
		nSize = packsize;  
	} else {  
		// 消息没有回卷，可以一次拷贝出去  
		memcpy(pBuf, m_bufInput + m_nInbufStart, packsize);  
		nSize = packsize;  
	}  

	// 重新计算环形缓冲区头部位置  
	m_nInbufStart = (m_nInbufStart + packsize) % INBUFSIZE;  
	m_nInbufLen -= packsize;  
	return  true;  
}  

bool CGameSocket::hasError()  
{  
#ifdef WIN32  
	int err = WSAGetLastError();  
	if(err != WSAEWOULDBLOCK) {  
#else  
	int err = errno;  
	if(err != EINPROGRESS && err != EAGAIN) {  
#endif  
		return true;  
	}  

	return false;  
}  

/************************************
函数功能：从网络中读取尽可能多的数据
返回值：接收数据成功：返回真，如果接收不成功，那么返回false
注意：此方法是实际向服务器请求数据的地方，将服务器数据放到本地缓存 
*************************************/
//  
bool CGameSocket::recvFromSock(void)  
{  
	if (m_nInbufLen >= INBUFSIZE || m_sockClient == INVALID_SOCKET) {  
		return false;  
	}  

	// 接收第一段数据  
	int savelen, savepos;           // 数据要保存的长度和位置  
	if(m_nInbufStart + m_nInbufLen < INBUFSIZE)  {   // INBUF中的剩余空间无回绕  
		savelen = INBUFSIZE - (m_nInbufStart + m_nInbufLen);        // 后部空间长度，最大接收数据的长度  
	} else {  
		savelen = INBUFSIZE - m_nInbufLen;   // INBUF中的剩余空间有回绕，如开始5，最大20，而现在已经有18 了，那么还剩2个字节的空间，并且从18+5-20开始，当然%20最好  
	}  

	// 缓冲区数据的末尾  
	savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;  //有无回绕都是这个位置
	if(savepos + savelen > INBUFSIZE)
		return false;  
	int inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);  //实际从服务器发过来的socket里取得的字节数
	if(inlen > 0) {  
		// 有接收到数据  
		m_nInbufLen += inlen;  
		//缓冲区装不下了
		if (m_nInbufLen > INBUFSIZE) {  
			return false;  
		}  

		// 接收第二段数据(一次接收没有完成，接收第二段数据)  
		if(inlen == savelen && m_nInbufLen < INBUFSIZE) {  
			int savelen = INBUFSIZE - m_nInbufLen;  
			int savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;  
			//CHECKF(savepos + savelen <= INBUFSIZE);  
			if(savepos + savelen > INBUFSIZE)
				return false;  
			inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);  
			if(inlen > 0) {  
				m_nInbufLen += inlen;  
				if (m_nInbufLen > INBUFSIZE) {  
					return false;  
				}     
			} else if(inlen == 0) {  
				Destroy();  
				return false;  
			} else {  
				// 连接已断开或者错误（包括阻塞）  
				if (hasError()) {  
					Destroy();  
					return false;  
				}  
			}  
		}  
	} else if(inlen == 0) {  
		Destroy();  
		return false;  
	} else {  
		// 连接已断开或者错误（包括阻塞）  
		if (hasError()) {  
			Destroy();  
			return false;  
		}  
	}  

	return true;  
}  
/************************************
函数功能：将缓冲区数据提交给服务器
返回值：提交成功：返回真，如果提交不成功，那么返回false
注意：此方法是真正的向服务器提交缓冲区里的数据.
*************************************/
bool CGameSocket::Flush(void)       //? 如果 OUTBUF > SENDBUF 则需要多次SEND（）  
{  
	if (m_sockClient == INVALID_SOCKET) {  
		return false;  
	}  

	if(m_nOutbufLen <= 0) {  
		return true;  
	}  

	// 发送一段数据  
	int outsize;  
	//返回值为发送了的字节长度，若上来发送缓冲区的数据超过总长度，那么就会全部发送。
	outsize = send(m_sockClient, m_bufOutput, m_nOutbufLen, 0);  
	if(outsize > 0) {  
		// 删除已发送的部分  
		if(m_nOutbufLen - outsize > 0) {  
			//发送缓冲区的数据往该队列前面移动发送了的字节长度，移动到队列首位置
			memcpy(m_bufOutput, m_bufOutput + outsize, m_nOutbufLen - outsize);  
		}  

		m_nOutbufLen -= outsize;  
		//异常检测，查看是否多发了数据
		if (m_nOutbufLen < 0) {  
			return false;  
		}  
	} else {  
		if (hasError()) {  
			Destroy();  
			return false;  
		}  
	}  

	return true;  
}  

bool CGameSocket::Check(void)  
{  
	// 检查状态  
	if (m_sockClient == INVALID_SOCKET) {  
		return false;  
	}  
	//取第一个字节出来检查
	char buf[1];  
	int ret = recv(m_sockClient, buf, 1, MSG_PEEK);  
	if(ret == 0) {  
		Destroy();  
		return false;  
	} else if(ret < 0) {  
		if (hasError()) {  
			Destroy();  
			return false;  
		} else {    // 阻塞  
			return true;  
		}  
	} else {    // 有数据  
		return true;  
	}  

	return true;  
}  

void CGameSocket::Destroy(void)  
{  
	// 关闭  
	struct linger so_linger;  
	so_linger.l_onoff = 1;  //关
	so_linger.l_linger = 500; //延迟时间 
	int ret = setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));  

	closeSocket();  

	m_sockClient = INVALID_SOCKET;  
	m_nInbufLen = 0;  
	m_nInbufStart = 0;  
	m_nOutbufLen = 0;  

	memset(m_bufOutput, 0, sizeof(m_bufOutput));  
	memset(m_bufInput, 0, sizeof(m_bufInput));  
}  