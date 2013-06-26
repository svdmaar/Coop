#pragma once

#include "DataMap.h"

template<typename T>
class CDataMapExt : public CDataMap<T>
{

public:
	int GetCount(const T & val) const
	{
		const T * pData = _GetData();
		int iCount = _GetCount();
		int nFound = 0;

		for(int i = 0; i < iCount; i++)
		{
			if(pData[i] == val)
			{
				nFound++;
			}
		}

		return nFound;
	}
};
