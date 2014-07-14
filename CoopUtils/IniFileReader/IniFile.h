#pragma once

#include <string>
#include <vector>
#include <map>

// TODO: make independent from windows (POINT -> SIntPoint).
#include <Windows.h>

struct SIniBlock
{
	std::map<std::string, std::string> m_mLines;
};

class CIniFile
{
	std::map<std::string, SIniBlock> m_mIniBlocks;

	static std::string _TrimString(const std::string & _sText);
	std::vector<std::string> _GetValueStringVector(const std::string & _sBlock, const std::string & _sKey) const;
	
	static std::vector<std::string> _SplitString(const std::string & _sText, char _cSplit);

public:
	CIniFile();
	~CIniFile();

	bool Load(const std::string& _sFileName);
   bool Save(const std::string& _sFileName);

	std::string GetValueString(const std::string& _sBlock, const std::string& _sKey) const;
	int GetValueInt(const std::string& _sBlock, const std::string& _sKey) const;
	std::vector<int> GetValueIntVector(const std::string& _sBlock, const std::string& _sKey) const;
	POINT GetValuePoint(const std::string& _sBlock, const std::string& _sKey) const;

   void SetValueInt(const std::string& _sBlock, const std::string& _sKey, int _iValue);

	bool BlockExists(const std::string & _sBlock) const;

	void PrintValues() const;

	static void TestTrim();
};
