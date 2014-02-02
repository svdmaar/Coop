#pragma once

#include <string>
#include <vector>
#include <map>

struct SIniBlock
{
	std::map<std::string, std::string> m_mLines;
};

class CIniFile
{
	std::map<std::string, SIniBlock> m_mIniBlocks;

	static std::string _TrimString(const std::string& _sText);

public:
	CIniFile();
	~CIniFile();

	bool Load(const std::string& _sFileName);

	std::string GetValueString(const std::string& _sBlock, const std::string& _sKey) const;
	int GetValueInt(const std::string& _sBlock, const std::string& _sKey) const;

	bool BlockExists(const std::string & _sBlock) const;

	void PrintValues() const;

	static void TestTrim();
};
