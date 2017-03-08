#include "stdafx.h"
#include "global.h"
#include "utility.h"
#include "log.h"

#include "Client.h"

#include "../Common.h"

int SrvSock;
ClientList lstClient;
std::mutex gMutex;

void ServiceMain()
{
	sockaddr_in SrvAddr, ClntAddr;
	int szClntAddr;
	int ClntSock;

	Logging("Server On");

	SrvSock = socket(AF_INET, SOCK_STREAM, 0);
	if (SrvSock != -1)
	{
		memset(&SrvAddr, 0, sizeof(SrvAddr));
		SrvAddr.sin_family = AF_INET;
		SrvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		SrvAddr.sin_port = htons(SERVER_PORT);
	
		if (bind(SrvSock, (sockaddr *)&SrvAddr, sizeof(SrvAddr)) != -1)
		{
			if (listen(SrvSock, 5) != -1)
			{
				while (1)
				{
					szClntAddr = sizeof(ClntAddr);
					ClntSock = accept(SrvSock, (sockaddr *)&ClntAddr, &szClntAddr);
					if (ClntSock != -1)
					{
						(new Client(ClntSock, ClntAddr))->run();
					}
				}
			}
			else
			{
				Logging("listen() failed");
			}
		}
		else
		{
			Logging("bind() failed");
		}
	
		close(SrvSock);
	}
	else
	{
		Logging("socket() failed");
	}
}
#ifdef CREATE_EXECUTE_FILE
int main() { ServiceMain(); abort(); return 0; }
#endif

void BroadCastString(char *str)
{
	for (auto it = lstClient.begin(); it != lstClient.end(); it++)
	{
		(*it)->SendString(str);
	}
}
