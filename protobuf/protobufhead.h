#ifndef PROTOBUF_Head_H_
#define PROTOBUF_Head_H_

#include"testtable.pb.h"
#include"../HcBase/HcSingleton.h"

class sTableMgr : public cSingleton<sTableMgr>
{
public:
	sTableMgr()
	{
		m_TestTableMap.clear();
	}
	~sTableMgr()
	{

	}

private:
	std::map<int, TABLE::Test*> m_TestTableMap;
};



#endif