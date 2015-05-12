#include"HelpXlsx.h"
#include"zlib/Unzipper.h"
#include"protobuf/protobufhead.h"
#include"tinyxml2/tinyxml2.h"


cHelpXlsx::cHelpXlsx()
{
	for (int iLoop = eXlsxComments1; iLoop < eXlsxDataNum; ++iLoop)
	{
		m_apXlsxData[iLoop] = NULL;
	}
	m_iRow = 0;
	m_iCol = 0;
}

cHelpXlsx::~cHelpXlsx()
{
	Release();
}

void cHelpXlsx::Release()
{
	for (int iLoop = eXlsxComments1; iLoop < eXlsxDataNum; ++iLoop)
	{
		char * pkTemp =m_apXlsxData[iLoop];
		if (pkTemp != NULL)
		{
			delete[] pkTemp;
			m_apXlsxData[iLoop] = NULL;
		}
	}
}

bool cHelpXlsx::LoadXlsx(const char* cpFileName)
{
	CUnzipper unZib;
	unZib.OpenZip(cpFileName);	

	for (int iLoop = eXlsxComments1; iLoop < eXlsxDataNum; ++iLoop)
	{
		if (unZib.GotoFile(strXlsxDataFileName[iLoop].c_str()))
		{
			UZ_FileInfo kTempInfo;
			unZib.GetFileInfo(kTempInfo);
			m_apXlsxData[iLoop] = new char[kTempInfo.dwUncompressedSize + 1];
			memset(m_apXlsxData[iLoop], 0, kTempInfo.dwUncompressedSize + 1);
			unZib.UnzipToBuffer(m_apXlsxData[iLoop], kTempInfo.dwUncompressedSize + 1);
		}
	}	
	LoadColRowData();
	return true;
}

//填充行列数据
bool cHelpXlsx::LoadColRowData()
{
	tinyxml2::XMLDocument pkXmlDoc;
	tinyxml2::XMLError eRes = pkXmlDoc.Parse(m_apXlsxData[eXlsxSheet1]);
	if (eRes != tinyxml2::XML_NO_ERROR)
	{
		return false;
	}
	tinyxml2::XMLElement* pkElework = pkXmlDoc.FirstChildElement("worksheet");
	if (pkElework == NULL)
	{
		return false;
	}
	tinyxml2::XMLElement* pkDi = pkElework->FirstChildElement("dimension");
	if (pkDi == NULL)
	{
		return false;
	}
	std::string strCloRow = pkDi->Attribute("ref");
	char * pStr = (char *)strCloRow.c_str();

	char *pDes = std::strstr(pStr, ":");
	if (pDes == NULL)
	{
		return false;
	}
	pDes++;
	ParserColRowData(pDes, m_iRow, m_iCol);
	return true;
}

const char * cHelpXlsx::GetXlsxDataBuffer(eXlsxData eDatatype)
{
	if (eDatatype <= eXlsxDataNull || eDatatype >= eXlsxDataNum)
	{
		return NULL;
	}
	return m_apXlsxData[eDatatype];
}

//得到表格的有效数据
void cHelpXlsx::ParserColRowData(char* pData, int& iRow, int & iCol)
{
	iRow = 0;
	iCol = 0;
	while (*pData && *pData >= 'A' && *pData <= 'Z')
	{
		iCol *= ('Z' - 'A' + 1);
		iCol += (*pData - 'A') + 1;
		++pData;
	}

	while (*pData && *pData >= '0' && *pData <= '9')
	{
		iRow *= ('9' - '0' + 1);
		iRow += (*pData - '0') + 1;
		++pData;
	}
}

bool cHelpXlsx::SavePBData()
{
	int iRow = 0;
	int iCol = 0;

	


	return true;
}