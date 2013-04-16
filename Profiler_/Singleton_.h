#pragma once

template <typename T> class CSingleton_
{
	static T*		m_pThis;

public:

	CSingleton_()
	{
		if (m_pThis) return;

		int offset = (int)(T*)1 - (int)(CSingleton_<T>*)(T*)1;
		m_pThis = (T*)((int)this + offset);

	}

	static T&	GetThis()
	{
		if (!m_pThis) 
		{
			m_pThis = new T();
		}
		return *m_pThis;
	}

	static T*	GetThis_()
	{
		if (!m_pThis) 
		{
			m_pThis = new T();
		}
		return m_pThis;
	}

	void DestroyThis()
	{
		if(m_pThis)
		{
			delete m_pThis;
		}

		m_pThis = 0;
	}
};

template <typename T> T* CSingleton_ <T>::m_pThis;

