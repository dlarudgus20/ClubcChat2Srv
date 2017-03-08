#include "stdafx.h"
#include "global.h"
#include "utility.h"
#include "log.h"

#include "Client.h"

#include "../Common.h"

void Client::ClientThread()
{
	char RecvBuffer[CHAT_DATA_BUF];
	char RecvString[CHAT_DATA_BUF + 1];
	int rcv, i, j;

	Loggingf("Connect : %s", astr());

	while (1)
	{
		j = 0;
		rcv = recv(m_sock, RecvBuffer, CHAT_DATA_BUF, 0);
		if (rcv > 0)
		{
			for (i = 0; i < rcv; i++)
			{
				RecvString[j] = RecvBuffer[i];
				if (RecvBuffer[i] == NULL)
				{
					if (!ProcessString(RecvString))
					{
						goto DoCommunicate_End;
					}
					j = 0;
				}
				else
				{
					j++;
				}
			}
			if (j != 0)
			{
				RecvString[j] = NULL;
				if(!ProcessString(RecvString))
				{
					goto DoCommunicate_End;
				}
			}
		}
		else
		{
			break;
		}
	}
DoCommunicate_End:

	{
		mutex_locker ml(gMutex);

		Loggingf("Close : %s", astr());
		close(m_sock);

		if (bConnected)
		{
			lstClient.remove(this);

			char strSend[CHAT_DATA_BUF];
			sprintf(strSend, "%s%s", CMD_CNTLIST_REMOVE, strNick);
			BroadCastString(strSend);

			std::wstringstream stream; stream << lstClient.size();

			Loggingf(L"Logout : %s Count : %s (%s)",
				(wchar_t *)ChrCvtStr<wchar_t>(ChrCvt_UTF8ToWideChar(strNick)),
				stream.str().c_str(),
				wstr());
		}

		Loggingf("End : %s", astr());
		m_thrd.detach();
		delete this;
	}
}

bool Client::ProcessString(char *str)
{
	if (!bReceivedVersion)
	{
		if (strcmp(str, CLUBCCHATINGPROC_VERSION_STR) != 0)
		{
			Loggingf("Invalid Client : %s", astr());
			return false;
		}
		else
		{
			Loggingf("Client Checked : %s", astr());
			bReceivedVersion = true;
		}
	}
	else if (!bReceivedId)
	{
		strcpy(strId, str);
		bReceivedId = true;
	}
	else if (!bReceivedPassword)
	{
		mutex_locker ml(gMutex);

		try
		{
			if (Login(strId, str, strNick))
			{
				LoginProcess(str);

				std::wstringstream stream; stream << lstClient.size();

				Loggingf(L"Login Successed : %s %s %s Count : %s (%s)",
					(wchar_t *)ChrCvtStr<wchar_t>(ChrCvt_UTF8ToWideChar(strId)),
					(wchar_t *)ChrCvtStr<wchar_t>(ChrCvt_UTF8ToWideChar(str)),
					(wchar_t *)ChrCvtStr<wchar_t>(ChrCvt_UTF8ToWideChar(strNick)),
					stream.str().c_str(),
					wstr());
			}
			else
			{
				Loggingf("Login failed : %s", astr());
				return false;
			}
		}
		catch (login_exception &e)
		{
			Loggingf(L"Login exception : %s", e.wwhat());
			return false;
		}
	}
	else
	{
		mutex_locker ml(gMutex);

		Loggingf(L"Say BroadCast : %s (%s) Message : %s",
			(wchar_t *)ChrCvtStr<wchar_t>(ChrCvt_UTF8ToWideChar(strNick)),
			wstr(),
			(wchar_t *)ChrCvtStr<wchar_t>(ChrCvt_UTF8ToWideChar(str)));

		BroadCastString(str);
	}

	return true;
}

void Client::LoginProcess(char *str)
{
	char strSend[CHAT_DATA_BUF];

	sprintf(strSend, "%s%s", CMD_MY_NICKNAME, strNick);

	SendString(strSend);
	bReceivedPassword = true;
	SendString(CMD_CNT_SUCCEED);

	sprintf(strSend, "%s%s", CMD_CNTLIST_ADD, strNick);
	BroadCastString(strSend);
	bConnected = true;

	lstClient.push_back(this);

	SendString(CMD_CNTLIST_BEGIN);
	for (auto it = lstClient.begin(); it != lstClient.end(); it++)
	{
		if ((*it)->bConnected)
		{
			SendString((*it)->strNick);
		}
	}
	SendString(CMD_CNTLIST_END);
}
