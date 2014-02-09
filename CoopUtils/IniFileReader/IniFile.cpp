#include "IniFile.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>

using namespace std;

CIniFile::CIniFile()
{
}

CIniFile::~CIniFile()
{
}

string CIniFile::_TrimString(const string& _sText)
{
	int iFirstNonSpace = _sText.length();
	int iLastNonSpace = _sText.length() - 1;

	for(int i = 0; i < (int)_sText.length(); i++)
	{
		char c = _sText[i];

		if(c != ' ')
		{
			iFirstNonSpace = min(i, iFirstNonSpace);
			iLastNonSpace = i;
		}
	}

	int nLength = iLastNonSpace + 1 - iFirstNonSpace;

	string strOut;

	if (nLength > 0)
	{
		strOut = _sText.substr(iFirstNonSpace, nLength);
	}
	
	return strOut;
}

bool CIniFile::Load(const string& _sFileName)
{
	const int nBufferSize = 256;
	char cBuffer[nBufferSize];

	ifstream ifIn(_sFileName);
	string sBlockTitle;

	while (!ifIn.eof())
	{
		ifIn.getline(cBuffer, sizeof(cBuffer));

		string sLine(cBuffer);
		sLine = _TrimString(sLine);

		if(sLine.empty())
		{
			continue;
		}

		if(sLine[0] == '[')
		{
			int iEndIndex = sLine.find(']');
			sBlockTitle = sLine.substr(1, iEndIndex - 1);
			continue;
		}

		int iSpaceIndex = sLine.find('=');

		if(iSpaceIndex == string::npos)
		{
			continue;
		}

		string sKey = sLine.substr(0, iSpaceIndex);
		string sValue = sLine.substr(iSpaceIndex + 1, sLine.length() - iSpaceIndex - 1);

		sKey = _TrimString(sKey);
		sValue = _TrimString(sValue);

		map<string, SIniBlock>::iterator i;
		i = m_mIniBlocks.find(sBlockTitle);

		if(i == m_mIniBlocks.end())
		{
			SIniBlock block;
			block.m_mLines[sKey] = sValue;

			m_mIniBlocks[sBlockTitle] = block;
		}
		else
		{
			SIniBlock& block = (*i).second;
			block.m_mLines[sKey] = sValue;
		}
	}

	return true;
}

string CIniFile::GetValueString(const string& _sBlock, const string& _sKey) const
{
	map<string, SIniBlock>::const_iterator iBlock;

	iBlock = m_mIniBlocks.find(_sBlock);
	if(iBlock == m_mIniBlocks.end())
	{
		return "";
	}

	const SIniBlock& block = (*iBlock).second;

	map<string, string>::const_iterator iLine;
	iLine = block.m_mLines.find(_sKey);
	if(iLine == block.m_mLines.end())
	{
		return "";
	}

	return (*iLine).second;
}

int CIniFile::GetValueInt(const string& _sBlock, const string& _sKey) const
{
	string sValue = GetValueString(_sBlock, _sKey);

	if(sValue == "")
	{
		return 0;
	}

	int iValue = atoi(sValue.c_str());
	return iValue;
}

void CIniFile::PrintValues() const
{
	map<string, SIniBlock>::const_iterator iBlock;

	for(iBlock = m_mIniBlocks.begin(); iBlock != m_mIniBlocks.end(); iBlock++)
	{
		string sTitle = (*iBlock).first;
		const SIniBlock& block = (*iBlock).second;

		map<string, string>::const_iterator iValue;

		for(iValue = block.m_mLines.begin(); iValue != block.m_mLines.end(); iValue++)
		{
			string sKey = (*iValue).first;
			string sValue = (*iValue).second;

			cout << sTitle << "." << sKey << ": " << sValue << endl;
		}
	}
}

bool CIniFile::BlockExists(const std::string & _sBlock) const
{
	map<string, SIniBlock>::const_iterator iBlock;
	iBlock = m_mIniBlocks.find(_sBlock);

	return iBlock != m_mIniBlocks.cend();
}

void CIniFile::TestTrim()
{
	string strOrg, strTrim;

	strOrg = "test";
	strTrim = _TrimString(strOrg);
	cout << "\"" << strTrim << "\"" << endl;

	strOrg = "  test";
	strTrim = _TrimString(strOrg);
	cout << "\"" << strTrim << "\"" << endl;

	strOrg = "test  ";
	strTrim = _TrimString(strOrg);
	cout << "\"" << strTrim << "\"" << endl;

	strOrg = "  test  ";
	strTrim = _TrimString(strOrg);
	cout << "\"" << strTrim << "\"" << endl;
}

vector<int> CIniFile::GetValueIntVector(const string & _sBlock, const string & _sKey) const
{
	vector<int> vOut;

	vector<string> vStringValues = _GetValueStringVector(_sBlock, _sKey);
	
	for(vector<string>::iterator i = vStringValues.begin(); i != vStringValues.end(); i++)
	{
		string sValue = *i;
		int iValue = atoi(sValue.c_str());
		vOut.push_back(iValue);
	}

	return vOut;
}

vector<string> CIniFile::_GetValueStringVector(const string & _sBlock, const string & _sKey) const
{
	vector<string> vOut;

	string sCompleteString = GetValueString(_sBlock, _sKey);

	if(sCompleteString == "")
	{
		return vOut;
	}

	char cFirstChar = sCompleteString[0];
	char cLastChar = sCompleteString[sCompleteString.length() - 1];

	if((cFirstChar != '[') || (cLastChar != ']'))
	{
		vOut.push_back(sCompleteString);
		return vOut;
	}

	sCompleteString = sCompleteString.substr(1, sCompleteString.length() - 2);

	vOut = _SplitString(sCompleteString, ',');

	vector<string>::iterator i;
	for(i = vOut.begin(); i != vOut.end(); i++)
	{
		string s = *i;
		s = _TrimString(s);
		*i = s;
	}

	return vOut;
}

vector<string> CIniFile::_SplitString(const string & _sText, char _cSplit)
{
	vector<string> vOut;

	istringstream iss(_sText);
	string s;
	
	while(getline(iss, s, _cSplit))
	{
		vOut.push_back(s);
	}

	return vOut;
}

POINT CIniFile::GetValuePoint(const string & _sBlock, const string & _sKey) const
{
	vector<int> vIntValues = GetValueIntVector(_sBlock, _sKey);
	POINT pOut;
	ZeroMemory(&pOut, sizeof(pOut));

	if(vIntValues.size() != 2)
	{
		return pOut;
	}

	pOut.x = vIntValues[0];
	pOut.y = vIntValues[1];

	return pOut;
}
