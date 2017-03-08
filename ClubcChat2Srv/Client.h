#pragma once

#include "utility.h"

#include "CharConvert.h"

#include "../Common.h"

class Client : private no_copyable
{
private:
	int m_sock;
	sockaddr_in m_addr;
	std::thread m_thrd;

	bool bReceivedVersion;
	bool bReceivedId;
	bool bReceivedPassword;
	bool bConnected;

	char strId[CHAT_DATA_BUF];
	char strNick[CHAT_DATA_BUF];

	char ClientIp[16];
	char ClientPort[6];

	char _astr[21];
	ChrCvtStr<wchar_t> _wstr;

public:
	Client(int sock, const sockaddr_in &addr)
		: m_sock(sock), m_addr(addr),
		bReceivedVersion(false), bReceivedId(false), bReceivedPassword(false), bConnected(false)
	{
		strcpy(ClientIp, inet_ntoa(addr.sin_addr));
		_itoa(ntohs(addr.sin_port), ClientPort, 10);

		sprintf(_astr, "%s:%s", ClientIp, ClientPort);
		_wstr = ChrCvt_MultiByteToWideChar(_astr);
	}

	void run()
	{
		m_thrd = std::thread(std::mem_fun(&Client::ClientThread), this);
	}

	bool SendString(const char *str)
	{
		return send(m_sock, str, strlen(str) + 1, 0) > 0;
	}

	const char *astr() { return _astr; }
	const wchar_t *wstr() { return _wstr; }

private:
	void ClientThread();

	bool ProcessString(char *str);
	void LoginProcess(char *str);
};
