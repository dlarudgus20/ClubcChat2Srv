#pragma once

class no_copyable
{
private:
	no_copyable(const no_copyable &);
	no_copyable &operator =(const no_copyable &);
protected:
	no_copyable() { }
};

class mutex_locker : private no_copyable
{
private:
	std::mutex &m_mutex;
public:
	explicit mutex_locker(std::mutex &mu) : m_mutex(mu) { m_mutex.lock(); }
	~mutex_locker() { m_mutex.unlock(); }
};

struct array_deleter
{
	template <typename T> void operator()(T *p)
	{
		delete [] p;
	}
};
