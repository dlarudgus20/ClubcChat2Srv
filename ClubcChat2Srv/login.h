#pragma once

class login_exception : public std::exception
{
private:
	wchar_t *wstr;
	std::string astr;
protected:
	login_exception() { }
public:
	explicit login_exception(const char *str);
	virtual ~login_exception() throw();
	virtual const char *what() const throw();
	virtual const wchar_t *wwhat() const throw();
};

bool Login(const char *id, const char *pw, char *nick);
