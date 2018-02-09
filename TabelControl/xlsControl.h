#ifndef xlsControl_HelpXlsx_h
#define xlsControl_HelpXlsx_h

#include<string>
#include<vector>

enum eXlsxData
{
	eXlsxDataNull = -1,
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

class cXlsControl
{
public:
	cXlsControl();
	~cXlsControl();
	void Release();

	bool LoadColRowData();

	bool ReadFileData();
	const char * GetXlsxDataBuffer(eXlsxData eDatatype);
	void ParserColRowData(char* pData, int& iRow, int & iCol);
public:

	std::string GetJsonFileName();
	bool LoadXlsx(const char* cpFileName);

	bool ReadXlsxData();

	bool DataToJsonFile();


private:
	std::string							m_strFileName;
	char *								m_apXlsxData[eXlsxDataNum];
	int									m_iRow;
	int									m_iCol;
	std::vector<std::string>			m_akTableHead;
	std::vector<std::string>			m_akTableContext;
	std::vector<std::string>			m_akTableId;

};



#endif




