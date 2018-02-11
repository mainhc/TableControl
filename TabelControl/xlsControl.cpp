#include"xlsControl.h"
#include"tinyxml2.h"
#include"Unzipper.h"
#include"HelpFunc.h"
#include<vector>


cXlsControl::cXlsControl()
{
	for (int iLoop = eXlsxComments1; iLoop < eXlsxDataNum; ++iLoop)
	{
		m_apXlsxData[iLoop] = NULL;
	}
	m_iRow = 0;
	m_iCol = 0;

	m_akTableHead.clear();
	m_akTableContext.clear();
	m_akTableId.clear();

}

cXlsControl::~cXlsControl()
{
	Release();
}

void cXlsControl::Release()
{
	for (int iLoop = eXlsxComments1; iLoop < eXlsxDataNum; ++iLoop)
	{
		char * pkTemp = m_apXlsxData[iLoop];
		if (pkTemp != NULL)
		{
			delete[] pkTemp;
			m_apXlsxData[iLoop] = NULL;
		}
	}
}

//填充行列数据
bool cXlsControl::LoadColRowData()
{
	tinyxml2::XMLDocument pkXmlDoc;
	tinyxml2::XMLError eRes = pkXmlDoc.Parse(m_apXlsxData[eXlsxSheet1]);
	if (eRes != tinyxml2::XML_SUCCESS)
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

const char * cXlsControl::GetXlsxDataBuffer(eXlsxData eDatatype)
{
	if (eDatatype <= eXlsxDataNull || eDatatype >= eXlsxDataNum)
	{
		return NULL;
	}
	return m_apXlsxData[eDatatype];
}

//得到表格的有效数据
void cXlsControl::ParserColRowData(char* pData, int& iRow, int & iCol)
{
	iRow = 0;
	iCol = 0;
	while (*pData && *pData >= 'A' && *pData <= 'Z')
	{
		iCol *= ('Z' - 'A' + 1);
		iCol += (*pData - 'A') + 1;
		++pData;
	}

	while (*pData && *pData >= '1' && *pData <= '9')
	{
		iRow *= ('9' - '1' + 1);
		iRow += (*pData - '1') + 1;
		++pData;
	}
}


bool cXlsControl::ReadFileData()
{
	int iBufferSize = 0;
	int iPbNum = 0;
	char * pbuffer = NULL;
	char * pBegin = NULL;
	FILE * pfile = fopen("../resources/wwww.txt", "r");
	if (pfile != NULL)
	{


		int iReadSize = fread(&iBufferSize, 1, sizeof(int), pfile);
		iReadSize = fread(&iPbNum, 1, sizeof(int), pfile);
		int pbBuferSize = iBufferSize - 2 * sizeof(int);
		pbuffer = new char[pbBuferSize];
		pBegin = pbuffer;
		memset(pbuffer, 0, pbBuferSize);
		iReadSize = fread(pbuffer, 1, pbBuferSize, pfile);
		//pbuffer += iReadSize;
		//iReadSize = fread(pbuffer, 1, pbBuferSize, pfile);
		fclose(pfile);
	}


	return true;

}

std::string cXlsControl::GetJsonFileName()
{
	std::string res;
	int iIndex = m_strFileName.find_first_of('.');
	if (iIndex != std::string::npos)
	{
		res = m_strFileName.substr(0, iIndex);
		res.append(".json");
	}
	return res;
}


bool cXlsControl::LoadXlsx(const char* cpFileName)
{
	CUnzipper unZib;
	unZib.OpenZip(cpFileName);
	m_strFileName = cpFileName;

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


bool cXlsControl::ReadXlsxData()
{
	tinyxml2::XMLDocument pkXmlDoc;
	tinyxml2::XMLError eRes = pkXmlDoc.Parse(m_apXlsxData[eXlsxSheet1]);
	if (eRes != tinyxml2::XML_SUCCESS)
	{
		return false;
	}
	tinyxml2::XMLElement* pkElework = pkXmlDoc.FirstChildElement("worksheet");
	if (pkElework == NULL)
	{
		return false;
	}
	tinyxml2::XMLElement* pkSheet1 = pkElework->FirstChildElement("sheetData");
	if (pkSheet1 == NULL)
	{
		return false;
	}
	tinyxml2::XMLElement* pRow = pkSheet1->FirstChildElement("row");
	if (pRow == NULL)
	{
		return false;
	}
	tinyxml2::XMLDocument pkXmlDoc2;
	tinyxml2::XMLError eRes2 = pkXmlDoc2.Parse(m_apXlsxData[eXlsxSharedStrings]);
	if (eRes2 != tinyxml2::XML_SUCCESS)
	{
		return false;
	}
	tinyxml2::XMLElement* pkSharedStrings = pkXmlDoc2.FirstChildElement("sst");
	if (pkSharedStrings == NULL)
	{
		return false;
	}
	std::vector<std::string>   kvStr;
	tinyxml2::XMLElement* pkStr = pkSharedStrings->FirstChildElement("si");
	if (pkStr == NULL)
	{
		return false;
	}
	while (pkStr)
	{
		tinyxml2::XMLElement* pkStrT = pkStr->FirstChildElement("t");
		std::string strTemp;
		UTF_8ToGB2312(strTemp, pkStrT->GetText());
		kvStr.push_back(strTemp);
		pkStr = pkStr->NextSiblingElement();
	}
	for (int iLoop = 0; iLoop < m_iRow; iLoop++)
	{
		tinyxml2::XMLElement* pCol = pRow->FirstChildElement("c");
		if (pCol == NULL)
		{
			continue;
		}
		for (int jLoop = 0; jLoop < m_iCol; jLoop++)
		{
			std::string tStr = "";
			const char* cpTemp = pCol->Attribute("t");
			if (cpTemp != NULL)
			{
				tStr = cpTemp;
			}
			std::string strRowCol = "";
			cpTemp = pCol->Attribute("r");
			if (cpTemp != NULL)
			{
				strRowCol = cpTemp;
				printf("SavePBData  ++++  %s \n ", strRowCol.c_str());
			}
			std::string strValue = pCol->FirstChildElement("v")->GetText();
			if (tStr == "s")
			{
				int iVale = atoi(strValue.c_str());
				if (iVale < 0 || iVale >= kvStr.size())
				{
					continue;
				}
				strValue = kvStr[iVale];
				//第一行默认是表头
				if (iLoop == 0)
				{
					m_akTableHead.push_back(strValue);
				}
				else
				{
					if (jLoop == 0)
					{
						m_akTableId.push_back(strValue);
					}
					m_akTableContext.push_back(strValue);
				}
				printf(" %s \n ", strValue.c_str());
			}
			else
			{
				if (jLoop == 0)
				{
					m_akTableId.push_back(strValue);
				}
				m_akTableContext.push_back(strValue);
			}

			pCol = pCol->NextSiblingElement();
		}
		pRow = pRow->NextSiblingElement();
	}
	return true;
}

bool cXlsControl::DataToJsonFile()
{
	std::string strFileName = GetJsonFileName();

	FILE * pfile = fopen(strFileName.c_str(), "w");
	if (pfile != nullptr)
	{
		fwrite("{",1,1, pfile);
		fwrite("\n", 1, 1, pfile);
		fwrite("\t", 1, 1, pfile);
		for (int iLoop = 0; iLoop < m_akTableId.size();iLoop++)
		{
			fwrite("\n", 1, 1, pfile);
			fwrite("\t", 1, 1, pfile);
			fwrite("\"", 1, 1, pfile);
			std::string strTemp = m_akTableId[iLoop];
			fwrite(strTemp.c_str(), 1, strTemp.length(), pfile);
			fwrite("\"", 1, 1, pfile);
			fwrite(":", 1, 1, pfile);
			fwrite("\n", 1, 1, pfile);
			fwrite("\t", 1, 1, pfile);
			fwrite("{", 1, 1, pfile);
			fwrite("\n", 1, 1, pfile);
			fwrite("\t", 1, 1, pfile);
			for (int jLoop = 0; jLoop < m_akTableHead.size();jLoop++)
			{
				fwrite("\"", 1, 1, pfile);
				std::string tempStr = m_akTableHead[jLoop];
				fwrite(tempStr.c_str(), 1, tempStr.length(), pfile);
				fwrite("\"", 1, 1, pfile);
				fwrite(":", 1, 1, pfile);
				fwrite("\"", 1, 1, pfile);
				int iIndex = iLoop * m_akTableHead.size() + jLoop;
				std::string tempStr02 = m_akTableContext[iIndex];
				fwrite(tempStr02.c_str(), 1, tempStr02.length(), pfile);
				fwrite("\"", 1, 1, pfile);
				if (jLoop != m_akTableHead.size() - 1)
				{
					fwrite(",", 1, 1, pfile);
				}				
			}
			fwrite("\n", 1, 1, pfile);
			fwrite("\t", 1, 1, pfile);
			fwrite("}", 1, 1, pfile);
			if (iLoop != m_akTableId.size() - 1)
			{
				fwrite(",", 1, 1, pfile);
			}
		}
		fwrite("\n", 1, 1, pfile);
		fwrite("}", 1, 1, pfile);
		fclose(pfile);
	}

	return true;
}





