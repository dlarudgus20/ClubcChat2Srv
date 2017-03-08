#pragma once

class Log
{
private:
	std::fstream m_LogFile;
	std::mutex m_mutex;

	Log();
public:
	static Log &Get();
	void Logging(const char *str);
	void Logging(const wchar_t *str);
};

inline void Logging(const char *str)
{
	Log::Get().Logging(str);
}

inline void Logging(const wchar_t *str)
{
	Log::Get().Logging(str);
}

void Loggingf(const char *format, ...);
void Loggingf(const wchar_t *format, ...);
