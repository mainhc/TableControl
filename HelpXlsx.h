#ifndef TableControl_HelpXlsx_h
#define TableControl_HelpXlsx_h

#include<string>

enum eXlsxData
{
	eXlsxDataNull =-1,
	eXlsxComments1,
	eXlsxSharedStrings,
	eXlsxSheet1,
	eXlsxDataNum
};

const std::string  strXlsxDataFileName[eXlsxDataNum] =
{
	"xl/comments1.xml",
	"xl/sharedStrings.xml",
	"xl/worksheets/sheet1.xml",
};

class cHelpXlsx
{
public:
	cHelpXlsx();
	~cHelpXlsx();
	void Release();

	bool LoadXlsx(const char* cpFileName);
	bool SavePBData();
	const char * GetXlsxDataBuffer(eXlsxData eDatatype);
private:
	char *				m_apXlsxData[eXlsxDataNum];


};



#endif