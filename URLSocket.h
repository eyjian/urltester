// URLSocket.h: interface for the CURLSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_URLSOCKET_H__182A0283_D017_47A7_AD01_9CDC36EC4994__INCLUDED_)
#define AFX_URLSOCKET_H__182A0283_D017_47A7_AD01_9CDC36EC4994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CURLTesterDlg;
class CURLSocket  
{
public:
	CURLSocket(CURLTesterDlg* pURLTesterDlg);
	~CURLSocket();
    
    int GetIPByHostname(const char* pszHostname);

    void Close();
    bool TimedConnect(const char* szDns, const char* szIp, unsigned short usPort, long lSecond);
    bool TimedReceive(char* lpBuf, int nBufLen, long lSecond);
	bool TimedSend(const char* lpBuf, int nBufLen, long lSecond);
    
    DWORD GetDNSTime() const { return m_dwDNSTime; }
    DWORD GetConnectTime() const { return m_dwConnectTime; }
    DWORD GetSendTime() const { return m_dwSendTime; }
    DWORD GetRecvTime() const { return m_dwRecvTime; }
    DWORD GetTotalByteSent() const { return m_dwTotalByteSent; }
    DWORD GetTotalByteReived() const { return m_dwTotalByteReived; }    

private:
    void DoReset();

private:
    SOCKET m_fd;
    CURLTesterDlg* m_pURLTesterDlg;
	CString m_strIP;
	CString m_strDNS;
	unsigned short m_usPort;

private:
    DWORD m_dwDNSTime;
    DWORD m_dwConnectTime;
    DWORD m_dwSendTime;
    DWORD m_dwRecvTime;    
    DWORD m_dwTotalByteSent;
    DWORD m_dwTotalByteReived;
};

#endif // !defined(AFX_URLSOCKET_H__182A0283_D017_47A7_AD01_9CDC36EC4994__INCLUDED_)
