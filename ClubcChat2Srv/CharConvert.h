#pragma once

char *ChrCvt_WideCharToMultiByte(const wchar_t *str);
char *ChrCvt_WideCharToUTF8(const wchar_t *str);

wchar_t *ChrCvt_MultiByteToWideChar(const char *str);
char *ChrCvt_MultiByteToUTF8(const char *str);

wchar_t *ChrCvt_UTF8ToWideChar(const char *str);
char *ChrCvt_UTF8ToMultiByte(const char *str);

void ChrCvt_Free(const void *ptr);

template <typename T> class ChrCvtStr
{
private:
	ChrCvtStr(const ChrCvtStr &);
	ChrCvtStr &operator =(const ChrCvtStr &);

	T *m_p;
public:
	ChrCvtStr(T *p = NULL) : m_p(p) { }
	~ChrCvtStr() { if (m_p != NULL) ChrCvt_Free(m_p); }

	ChrCvtStr &operator =(T *str)
	{
		if (m_p != NULL) ChrCvt_Free(m_p);
		m_p = str;
		return *this;
	}

	operator T *() { return m_p; }
	operator const T *() const { return m_p; }

	T &operator *() { return *m_p; }
	const T &operator *() const { return *m_p; }

	T &operator [](int idx) { return m_p[idx]; }
	const T &operator [](int idx) const { return m_p[idx]; }
};
