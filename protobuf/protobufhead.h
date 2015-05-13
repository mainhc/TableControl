#ifndef PROTOBUF_Head_H_
#define PROTOBUF_Head_H_

#include"testtable.pb.h"
#include"../HcBase/HcSingleton.h"

class cTestTable
{
public:
	cTestTable()
	{
		m_TestTableMap.clear();
	}
	virtual ~cTestTable()
	{
		Release();
	}

	void Release()
	{
		typedef std::map<int, TABLE::Test*>::iterator itortemp;
		for (itortemp piter = m_TestTableMap.begin(); piter != m_TestTableMap.end(); ++piter)
		{
			TABLE::Test* pkTemp = piter->second;
			if (pkTemp != NULL)
			{
				delete pkTemp;
				pkTemp = NULL;
			}
		}
		m_TestTableMap.clear();
	}

	void AddElemTable(int iID, TABLE::Test* pkTable)
	{
		if (m_TestTableMap.find(iID) != m_TestTableMap.end())
		{
			return;
		}
		m_TestTableMap[iID] = pkTable;
	}

	TABLE::Test* GetTableElem(int iID)
	{
		if (m_TestTableMap.find(iID) != m_TestTableMap.end())
		{
			return m_TestTableMap[iID];
		}
		return NULL;
	}
private:
	std::map<int, TABLE::Test*> m_TestTableMap;
	
};



class cTableMgr : public cSingleton<cTableMgr>
{
public:
	cTableMgr(){		
	}
	virtual ~cTableMgr(){
	}
	static cTableMgr* Get(){
		return &GetSingleton();
	}
	cTestTable& GetTestTable()
	{
		return m_kTestTable;
	}
private:
	cTestTable			m_kTestTable;
	
};







#endif