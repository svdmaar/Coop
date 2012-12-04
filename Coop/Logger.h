#pragma once

#include <Windows.h>
#include <fstream>

class CLogger
{
	static std::ofstream *m_pOut;
	static std::string m_sLastError;

public:
	CLogger();

	static void WriteError(const std::string& _file, long _line, const std::string& _message);
	static void WriteError(const std::string& _file, long _line, const std::string& _message, HRESULT _hr);

	static void WriteMessage(const std::string& _message);

	static bool ErrorHappened();
	static std::string GetErrorString();
};

#define LogError(message) { \
	CLogger::WriteError(__FILE__, __LINE__, message); \
}

#define LogErrorHr(message, hr) { \
	CLogger::WriteError(__FILE__, __LINE__, message, hr); \
}
