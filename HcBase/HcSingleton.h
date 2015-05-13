//单键的实现
#ifndef Hc_Singletion_h
#define Hc_Singletion_h

#define INI_SINGLETION(type) \
	template <> type * cSingleton< type >::m_spSingleton = 0; \
	type the##type

template<typename T>
class cSingleton
{
public:
	cSingleton()
	{
		m_spSingleton = static_cast<T *>(this);
	}
	virtual ~cSingleton()
	{
		m_spSingleton = 0;
	}

	static T& GetSingleton()
	{
		return *m_spSingleton;
	}
	static T* GetSingletonPtr()
	{
		return m_spSingleton;
	}
protected:
	static T *		m_spSingleton;

};

#endif