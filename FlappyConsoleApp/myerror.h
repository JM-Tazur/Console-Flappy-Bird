#pragma once

#include "headers.h"

class MyError :public std::exception
{
private:
	string error_text;

public:
	MyError(string txt);
	~MyError();
	virtual const char* what() const noexcept;
};