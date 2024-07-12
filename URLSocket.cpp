// URLSocket.cpp: implementation of the CURLSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "URLTester.h"
#include "URLSocket.h"
#include "URLTesterDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define TIME_BEGIN() \
    DWORD dwBegin = GetTickCount()
#define TIME_END(dwTime) \
{ \
    DWORD dwEnd = GetTickCount(); \
    if (dwEnd >= dwBegin) \
        dwTime += dwEnd-dwBegin; \
    else \
        dwTime += (DWORD)-1 - dwBegin + dwEnd; \
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CURLSocket::CURLSocket(CURLTesterDlg* pURLTesterDlg)
{
    DoReset();
    m_pURLTesterDlg = pURLTesterDlg;
}

void CURLSocket::DoReset()
{
    m_fd = INVALID_SOCKET;
        
    m_dwDNSTime = 0;
    m_dwConnectTime = 0;
    m_dwSendTime = 0;
    m_dwRecvTime = 0;
    m_dwTotalByteSent = 0;
    m_dwTotalByteReived = 0;
}
CURLSocket::~CURLSocket()
{
}

void CURLSocket::Close()
{
    if (m_fd != INVALID_SOCKET) {
        ::closesocket(m_fd);
        m_fd = INVALID_SOCKET;
    }
}
bool CURLSocket::TimedConnect(const char* szDns, const char* szIp, unsigned short usPort, long lSecond)
{
    int fd;
    int ret;
    fd_set fds;    
    CString strLog;    
    struct timeval timeout = { lSecond,0 };

    DoReset();
    TIME_BEGIN();
    
	m_strIP = szIp;
	m_strDNS = szDns;
	m_usPort = usPort;

	strLog.Format("正在试图与%s(%s):%d建立连接 。。。", (LPCTSTR)m_strDNS, (LPCTSTR)m_strIP, m_usPort);
	m_pURLTesterDlg->AddRequestLog(strLog);

    fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == fd) {
        strLog.Format("创建套接字出错(%s,%s:%d)：", (LPCTSTR)m_strDNS, (LPCTSTR)m_strIP, m_usPort, GetLastErrorString());
        m_pURLTesterDlg->AddRequestLog(strLog);
        TIME_END(m_dwConnectTime);
        return false;
    }

    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(szIp);
    addr.sin_port = htons(usPort);
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

    while (true)
    {    
        if (-1 == ::connect(fd, (const struct sockaddr *)&addr, addrlen)) {
            if (::GetLastError() != WSAEINPROGRESS) {        
                strLog.Format("连接到%s(%s):%d出错：%s", (LPCTSTR)m_strDNS, (LPCTSTR)m_strIP, m_usPort, GetLastErrorString());
                m_pURLTesterDlg->AddRequestLog(strLog);
                ::closesocket(fd);
                TIME_END(m_dwConnectTime);
                return false;
            }

            FD_ZERO(&fds);
            FD_SET(fd, &fds);
            ret = select(fd, &fds, NULL, NULL, &timeout);
            if (-1 == ret) {
                strLog.Format("连接到%s(%s):%d出错：%s", (LPCTSTR)m_strDNS, (LPCTSTR)m_strIP, m_usPort, GetLastErrorString());
                m_pURLTesterDlg->AddRequestLog(strLog);
                ::closesocket(fd);
                TIME_END(m_dwConnectTime);
                return false;
            }
            else if (0 == ret) {
                strLog.Format("连接到%s(%s):%d超时", (LPCTSTR)m_strDNS, (LPCTSTR)m_strIP, m_usPort);
                m_pURLTesterDlg->AddRequestLog(strLog);
                ::closesocket(fd);
                TIME_END(m_dwConnectTime);
                return false;
            }
            else {
                continue;
            }
        }
        else{
            strLog.Format("成功连接到%s(%s):%d", (LPCTSTR)m_strDNS, (LPCTSTR)m_strIP, m_usPort);
            m_pURLTesterDlg->AddRequestLog(strLog);
            break;
        }
    }

    m_fd = fd;
    TIME_END(m_dwConnectTime);
    return true;
}

bool CURLSocket::TimedReceive(char* lpBuf, int nBufLen, long lSecond)
{
    int ret;
    int len;
    fd_set fds;    
    CString strLog;
    char* lpBufp = lpBuf;      
    struct timeval timeout = { lSecond,0 };

    TIME_BEGIN();

    len = nBufLen;
    while (true)
    {    
        ret = ::recv(m_fd, lpBufp, len, 0);
        if (0 == ret) {
            TIME_END(m_dwRecvTime);
            return false;
        }
        else if (ret > 0) {
            lpBufp[ret] = 0;
            m_pURLTesterDlg->AddResponseLog(lpBufp);

            m_dwTotalByteReived += ret;
            lpBufp += ret;
            len -= ret;
            if (0 == len) break;
        }
        else if (-1 == ret)
        {
            if (WSAEWOULDBLOCK == ::WSAGetLastError()) {
                FD_ZERO(&fds);
                FD_SET(m_fd, &fds);
                ret = select(m_fd+1, &fds, NULL, NULL, &timeout);
                if (-1 == ret) {
                    strLog.Format("接收响应出错(%s,%s:%d)：%s", (LPCTSTR)m_strDNS, (LPCTSTR)m_strIP, m_usPort, GetLastErrorString());
                    m_pURLTesterDlg->AddRequestLog(strLog);
                    TIME_END(m_dwRecvTime);
                    return false;
                }
                else if (0 == ret) {
                    strLog.Format("接收响应超时(%s,%s:%d)", (LPCTSTR)m_strDNS, (LPCTSTR)m_strIP, m_usPort);
                    m_pURLTesterDlg->AddRequestLog(strLog);
                    TIME_END(m_dwRecvTime);
                    return false;
                }
                else {
                    continue;
                }
            }
            else {
                strLog.Format("接收响应出错(%s,%s:%d)：%s", (LPCTSTR)m_strDNS, (LPCTSTR)m_strIP, m_usPort, GetLastErrorString());
                m_pURLTesterDlg->AddRequestLog(strLog);
                TIME_END(m_dwRecvTime);
                return false;
            }
        }
    }

    TIME_END(m_dwRecvTime);
    return true;
}
bool CURLSocket::TimedSend(const char* lpBuf, int nBufLen, long lSecond)
{
    int ret;
    int len;
    fd_set fds;    
    CString strLog;
    const char* lpBufp = lpBuf;      
    struct timeval timeout = { lSecond,0 };

    TIME_BEGIN();

    len = nBufLen;
    while (true)
    {    
        ret = ::send(m_fd, lpBufp, len, 0);
        if (0 == ret) {
            break;
        }
        else if (ret > 0) {
            m_dwTotalByteSent += ret;
            lpBufp += ret;
            len -= ret;
            if (0 == len) break;
        }
        else if (-1 == ret)
        {
            if (WSAEWOULDBLOCK == ::WSAGetLastError()) {
                FD_ZERO(&fds);
                FD_SET(m_fd, &fds);
                ret = select(m_fd+1, NULL, &fds, NULL, &timeout);
                if (-1 == ret) {
                    strLog.Format("发送请求出错(%s,%s:%d)：%s", (LPCTSTR)m_strDNS, (LPCTSTR)m_strIP, m_usPort, GetLastErrorString());
                    m_pURLTesterDlg->AddRequestLog(strLog);
                    TIME_END(m_dwSendTime);
                    return false;
                }
                else if (0 == ret) {
                    strLog.Format("发送请求超时(%s,%s:%d)", (LPCTSTR)m_strDNS, (LPCTSTR)m_strIP, m_usPort);
                    m_pURLTesterDlg->AddRequestLog(strLog);
                    TIME_END(m_dwSendTime);
                    return false;
                }
                else {
                    continue;
                }
            }
            else {
                strLog.Format("发送请求出错(%s,%s:%d)：%s", (LPCTSTR)m_strDNS, (LPCTSTR)m_strIP, m_usPort, GetLastErrorString());
                m_pURLTesterDlg->AddRequestLog(strLog);
                TIME_END(m_dwSendTime);
                return false;
            }
        }
    }

    TIME_END(m_dwSendTime);
    return true;
}

int CURLSocket::GetIPByHostname(const char* pszHostname)
{
    TIME_BEGIN();
    struct hostent* host = gethostbyname(pszHostname);
    if (NULL == host) return 0;
    
    unsigned int addr = ((unsigned int *)host->h_addr_list[0])[0];
    if ( (addr & 0xffff) == 0x0a0a ) addr= ((unsigned int *)host->h_addr_list[0])[1];

    DWORD dwEnd = GetTickCount();
    if (dwEnd >= dwBegin)
        m_dwDNSTime = dwEnd-dwBegin;
    else
        m_dwDNSTime = (DWORD)-1 - dwBegin + dwEnd;
    return addr;
}
