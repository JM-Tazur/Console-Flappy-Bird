#pragma once
#include "headers.h"
#include "myerror.h"

MyError::MyError(string txt)
{
	error_text = txt;
}

MyError::~MyError()
{
	//pusty destruktor
}

const char* MyError::what() const noexcept
{
	return error_text.c_str();
}