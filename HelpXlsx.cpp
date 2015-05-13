#include"HelpXlsx.h"
#include"zlib/Unzipper.h"
#include"protobuf/protobufhead.h"
#include"tinyxml2/tinyxml2.h"
#include"HelpFunc.h"



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

//�����������
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

//�õ�������Ч����
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
	if (eRes2 != tinyxml2::XML_NO_ERROR)
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
		UTF_8ToGB2312(strTemp,pkStrT->GetText());
		kvStr.push_back(strTemp);
		pkStr = pkStr->NextSiblingElement();
	}
	cTestTable& kTestTable = cTableMgr::Get()->GetTestTable();
	for (int iLoop = 0; iLoop < m_iRow; iLoop++)
	{		
		TABLE::Test* pKTestTemp = new TABLE::Test();
		tinyxml2::XMLElement* pCol = pRow->FirstChildElement("c");
		if (pCol == NULL)
		{
			continue;
		}
		for (int jLoop = 0; jLoop < m_iCol; jLoop++)
		{
			std::string tStr = pCol->Attribute("t");
			std::string strValue = pCol->FirstChildElement("v")->GetText();			
			if (tStr == "s")
			{				
				int iVale = atoi(strValue.c_str());
				if (iVale < 0 || iVale >= kvStr.size())
				{
					continue;
				}
				strValue = kvStr[iVale];
			}
			if (jLoop == 0)
			{
				pKTestTemp->set_name(strValue.c_str());
			}
			else if (jLoop == 1)
			{
				pKTestTemp->set_id(strValue.c_str());
			}
			else if (jLoop == 2)
			{
				pKTestTemp->set_email(strValue.c_str());
			}
			else if (jLoop == 3)
			{
				pKTestTemp->set_time(atof(strValue.c_str()));
			}
			else if (jLoop == 4)
			{
				pKTestTemp->set_big(atoi(strValue.c_str()));
			}
			pCol = pCol->NextSiblingElement();
		}
		kTestTable.AddElemTable(iLoop, pKTestTemp);
		pRow = pRow->NextSiblingElement();
	}
	return true;
}