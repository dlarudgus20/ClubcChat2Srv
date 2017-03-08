#pragma once

class Client;

typedef std::list<Client *> ClientList;

void BroadCastString(char *str);

extern int SrvSock;
extern ClientList lstClient;
extern std::mutex gMutex;

#include "login.h"
