#include "stdafx.h"
#include "log.h"
#include <stdarg.h>

#include "utility.h"

#include "CharConvert.h"

Log::Log()
		: m_LogFile("./ClubcChat2Srv.txt", std::ios_base::out | std::ios_base::app)
{
}

Log &Log::Get()
{
	static Log g;
	return g;
}

void Log::Logging(const char *str)
{
	mutex_locker ml(m_mutex);

	char buf[512];
	time_t t;

	time(&t);
	snprintf(buf, sizeof(buf), "%s", ctime(&t));
	buf[strlen(buf) - 1] = '\0';


	m_LogFile << str << " [" << buf << ']' << std::endl;
}

void Log::Logging(const wchar_t *str)
{
	const char *astr = ChrCvt_WideCharToMultiByte(str);
	Logging(astr);
	ChrCvt_Free(astr);
}

void Loggingf(const char *format, ...)
{
	char buf[2048];
	va_list va;

	va_start(va, format);

	vsnprintf(buf, sizeof(buf) / sizeof(buf[0]), format, va);
	Log::Get().Logging(buf);

	va_end(va);
}

void Loggingf(const wchar_t *format, ...)
{
	wchar_t buf[2048];
	va_list va;

	va_start(va, format);

	vswprintf(buf, sizeof(buf) / sizeof(buf[0]), format, va);
	Log::Get().Logging(buf);

	va_end(va);
}
