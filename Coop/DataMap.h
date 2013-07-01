#pragma once

template<typename T>
class CDataMap
{
	int m_iWidth;
	int m_iHeight;
	T * m_pData;

	int _CalcIndex(int _iRowIndex, int _iColumnIndex)
	{
		return _iColumnIndex + _iRowIndex * m_iWidth;
	}

protected:
	int _GetCount() const
	{
		return m_iWidth * m_iHeight;
	}

	const T * _GetData() const
	{
		return m_pData;
	}

public:
	CDataMap()
	{
		m_iWidth = 1;
		m_iHeight = 1;
		m_pData = new T[1];
	}

	CDataMap(int _iWidth, int _iHeight)
	{
		m_iWidth = _iWidth;
		m_iHeight = _iHeight;

		m_pData = new T[m_iWidth * m_iHeight];
	}

	CDataMap(int _iWidth, int _iHeight, T _value)
	{
		m_iWidth = _iWidth;
		m_iHeight = _iHeight;

		m_pData = new T[m_iWidth * m_iHeight];

		for(int i = 0; i < (m_iWidth * m_iHeight); i++)
		{
			T[i] = _value;
		}
	}

	~CDataMap()
	{
		delete [] m_pData;

		m_pData = NULL;
		m_iWidth = 0;
		m_iHeight = 0;
	}
	
	int GetWidth() const 
	{
		return m_iWidth;
	}

	int GetHeight() const
	{
		return m_iHeight;
	}

	T GetValue(int _iRowIndex, int _iColumnIndex)
	{
		int iIndex = _CalcIndex(_iRowIndex, _iColumnIndex);
		return m_pData[iIndex];
	}

	void SetValue(int _iRowIndex, int _iColumnIndex, T _value)
	{
		int iIndex = _CalcIndex(_iRowIndex, _iColumnIndex);
		m_pData[iIndex] = _value;
	}

	void SetValue(T _value)
	{
		for(int iIndex = 0; iIndex < _GetCount(); iIndex)
		{
			m_pData[iIndex] = _value;
		}
	}

	void Resize(int _iWidth, int _iHeight)
	{
		delete [] m_pData;

		m_iWidth = _iWidth;
		m_iHeight = _iHeight;
		m_pData = new T[m_iWidth * m_iHeight];
	}

	CDataMap<T> operator=(const CDataMap<T> & _rhs)
	{
		if(this != &_rhs)
		{
			Resize(_rhs.GetWidth(), _rhs.GetHeight());
			
			for(int iIndex = 0; iIndex < (m_iWidth * m_iHeight); iIndex++)
			{
				m_pData[iIndex] = _rhs.m_pData[iIndex];
			}
		}

		return *this;
	}
};
