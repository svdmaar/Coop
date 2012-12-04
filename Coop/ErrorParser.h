#pragma once

#include <string>
#include <Windows.h>

class CErrorParser
{
public:
	static std::string ParseError(HRESULT _hr);
};
