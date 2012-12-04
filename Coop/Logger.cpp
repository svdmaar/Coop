#include "Logger.h"

#include "ErrorParser.h"

#include <assert.h>
#include <ctime>

using namespace std;

CLogger::CLogger()
{
	assert(m_pOut == NULL);

	string fileName = "log.txt";
	string fullPath = string("log/").append(fileName);

	m_pOut = new ofstream(fullPath);

	ofstream& out = *m_pOut;

	char timeBuf[128], dateBuf[128];
    _strtime_s(timeBuf, 128);
    _strdate_s(dateBuf, 128);

	out << "Logging is on." << endl;
	out << "Build date and time of application: " << __DATE__ << " - " << __TIME__ << endl;
	out << "Date and time of run: " << dateBuf << " - " << timeBuf << endl;
	out << endl;

	m_sLastError = "";
}

void CLogger::WriteError(const std::string& _file, long _line, const std::string& _message)
{
	ofstream& out = *m_pOut;

	char lineBuffer[32];
	sprintf_s(lineBuffer, "%d", _line);

	int iStartFileName = _file.find_last_of('\\');
	string fileSubString = _file;

	if((iStartFileName != -1) && ((iStartFileName + 1) < (int)_file.length()))
	{
		fileSubString = _file.substr(iStartFileName + 1);
	}
	
	out << fileSubString.c_str() << ":" << lineBuffer << ": " << _message.c_str() << endl;

	m_sLastError = _message;
}

void CLogger::WriteError(const std::string& _file, long _line, const std::string& _message, HRESULT _hr)
{
	char hrBuffer[40];
	sprintf_s(hrBuffer, ": 0x%08X (", _hr);
	
	string sNewMessage = _message;
	sNewMessage += hrBuffer;
	sNewMessage += CErrorParser::ParseError(_hr);
	sNewMessage += ")";

	WriteError(_file, _line, sNewMessage);
}

void CLogger::WriteMessage(const std::string& _message)
{
	ofstream& out = *m_pOut;
	out << _message.c_str() << endl;
}

bool CLogger::ErrorHappened()
{
	return !m_sLastError.empty();
}

string CLogger::GetErrorString()
{
	return m_sLastError;
}

ofstream *CLogger::m_pOut = NULL;
string CLogger::m_sLastError;
