#include "stdafx.h"
#include "login.h"

#include "CharConvert.h"
login_exception::login_exception(const char *str) { }
login_exception::~login_exception() throw() { }
const char *login_exception::what() const throw() { return ""; }
const wchar_t *login_exception::wwhat() const throw() { return L""; }

bool Login(const char *id, const char *pw, char *nick)
{
	strcpy(nick, id);
	return true;
}

