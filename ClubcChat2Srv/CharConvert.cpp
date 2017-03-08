#include "stdafx.h"
#include "CharConvert.h"

char *ChrCvt_WideCharToMultiByte(const wchar_t *str)
{
	int len = wcslen(str) * 4;
	char *ret = (char *)calloc(len, sizeof(char));
	wcstombs(ret, str, len);
	return ret;
}

char *ChrCvt_WideCharToUTF8(const wchar_t *str)
{
	std::vector<char> buf;
	unsigned sur;

	for (const wchar_t *ptr = str; *ptr != L'\0'; ptr++)
	{
		if ((unsigned wchar_t)*ptr <= 0x7f)
		{
			buf.push_back((char)*ptr);
		}
		else if ((unsigned wchar_t)*ptr <= 0x7ff)
		{
			buf.push_back((char)( ((*ptr >> 6) & 0x1f) | 0xc0 ));
			buf.push_back((char)( (*ptr & 0x3f) | 0x80 ));
		}
		else if ((unsigned wchar_t)*ptr <= 0xffff)
		{
			buf.push_back((char)( ((*ptr >> 12) & 0x0f) | 0xe0 ));
			buf.push_back((char)( ((*ptr >> 6) & 0x3f) | 0x80 ));
			buf.push_back((char)( (*ptr & 0x3f) | 0x80 ));
		}
		else if (*ptr & 0xd800 == 0xcc00)
		{
			sur = (*ptr & 0x3ff) << 10;
			ptr++;
			if (*ptr & 0xdc00 == 0xce00)
			{
				sur |= *ptr & 0x3ff;

				buf.push_back((char)( ((sur >> 17) & 0x07) | 0xf0 ));
				buf.push_back((char)( ((sur >> 12) & 0x3f) | 0x80 ));
				buf.push_back((char)( ((sur >> 6) & 0x3f) | 0x80 ));
				buf.push_back((char)( (sur & 0x3f) | 0x80 ));
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}

	char *ret = (char *)calloc(buf.size() + 1, sizeof(char));
	memcpy(ret, &buf[0], buf.size());
	ret[buf.size()] = '\0';
	return ret;
}

wchar_t *ChrCvt_MultiByteToWideChar(const char *str)
{
	int len = strlen(str);
	wchar_t *ret = (wchar_t *)calloc(len, sizeof(wchar_t));
	mbstowcs(ret, str, len);
	return ret;
}

char *ChrCvt_MultiByteToUTF8(const char *str)
{
	wchar_t *tmp = ChrCvt_MultiByteToWideChar(str);
	char *ret = ChrCvt_WideCharToUTF8(tmp);
	ChrCvt_Free(tmp);
	return ret;
}

wchar_t *ChrCvt_UTF8ToWideChar(const char *str)
{
	std::vector<wchar_t> buf;

	while (*str != '\0')
	{
		if (((unsigned char)*str & 0x80) == 0)
		{
			buf.push_back(*str);
			str += 1;
		}
		else if (((unsigned char)*str & 0xe0) == 0xc0)
		{
			buf.push_back( ((str[0] & 0x1f) << 11) | (str[1] & 0x3f) );
			str += 2;
		}
		else if (((unsigned char)*str & 0xf0) == 0xe0)
		{
			buf.push_back( ((str[0] & 0x0f) << 12) | ((str[1] & 0x3f) << 6) | (str[2] & 0x3f) );
			str += 3;
		}
		else if (((unsigned char)*str & 0xf8) == 0xf0)
		{
			buf.push_back( 0xcc00 | ((str[0] & 0x07) << 7) | ((str[1] & 0x3f) << 1) | ((str[2] & 0x20) >> 5) );
			buf.push_back( 0xce00 | ((str[2] & 0x1f) << 5) | (str[3] & 0x3f) );
			str += 4;
		}
		else
		{
			return NULL;
		}
	}

	wchar_t *ret = (wchar_t *)calloc(buf.size() + 1, sizeof(wchar_t));
	memcpy(ret, &buf[0], buf.size() * sizeof(wchar_t));
	ret[buf.size()] = L'\0';
	return ret;
}

char *ChrCvt_UTF8ToMultiByte(const char *str)
{
	wchar_t *tmp = ChrCvt_UTF8ToWideChar(str);
	char *ret = ChrCvt_WideCharToMultiByte(tmp);
	ChrCvt_Free(tmp);
	return ret;
}

void ChrCvt_Free(const void *ptr)
{
	free((void *)ptr);
}

