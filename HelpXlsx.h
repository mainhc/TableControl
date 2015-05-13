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
	//填充行列数据
	bool LoadColRowData();
	bool SavePBData();
	const char * GetXlsxDataBuffer(eXlsxData eDatatype);
	void ParserColRowData(char* pData, int& iRow, int & iCol);


private:
	char *				m_apXlsxData[eXlsxDataNum];
	//行数
	int					m_iRow;
	//列数
	int					m_iCol;

};



#endif