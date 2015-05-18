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

	while (*pData && *pData >= '1' && *pData <= '9')
	{
		iRow *= ('9' - '1' + 1);
		iRow += (*pData - '1') + 1;
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
	int iBufferSize = 2* sizeof(int);
	for (int iLoop = 0; iLoop < kTestTable.GetTableRow(); ++iLoop)
	{
		iBufferSize += sizeof(int);

		TABLE::Test* ptemp = kTestTable.GetTableElem(iLoop);
		iBufferSize += ptemp->ByteSize();
	}
	char* pTempChar = new char[iBufferSize];
	char* pBegin = pTempChar;
	memset(pTempChar, 0, iBufferSize);
	memcpy(pTempChar, &iBufferSize, sizeof(iBufferSize));
	pTempChar += sizeof(iBufferSize);
	int iRowNum = kTestTable.GetTableRow();
	memcpy(pTempChar, &iRowNum, sizeof(int));
	pTempChar += sizeof(int);
	for (int iLoop = 0; iLoop < kTestTable.GetTableRow(); ++iLoop)
	{
		TABLE::Test* ptemp = kTestTable.GetTableElem(iLoop);
		int pbsize = ptemp->ByteSize();
		memcpy(pTempChar, &pbsize, sizeof(int));
		pTempChar += sizeof(int);
		ptemp->SerializeToArray(pTempChar,pbsize);		
		pTempChar += pbsize;
	}
	FILE * pfile = fopen("wwww.txt", "wb+");
	if (pfile != NULL)
	{
		fwrite(pBegin, 1, iBufferSize, pfile);
		fclose(pfile);
	}
	if (pBegin != NULL)
	{
		int iSize = *((int*)pBegin);
		delete[] pBegin;
		pBegin = NULL;
	}
	return true;
}

bool cHelpXlsx::ReadFileData()
{
	int iBufferSize = 0;
	int iPbNum = 0;
	char * pbuffer = NULL;
	FILE * pfile = fopen("wwww.txt", "r");
	if (pfile != NULL)
	{
		

		int iReadSize = fread(&iBufferSize, 1, sizeof(int), pfile);
		iReadSize = fread(&iPbNum, 1, sizeof(int), pfile);
		int pbBuferSize = iBufferSize - 2 * sizeof(int);
		pbuffer = new char[pbBuferSize];
		memset(pbuffer, 0, pbBuferSize);
		iReadSize = fread(pbuffer, 1, pbBuferSize, pfile);
		pbuffer += iReadSize;
		iReadSize = fread(pbuffer, 1, pbBuferSize, pfile);
		fclose(pfile);
	}
	for (int iLoop = 0; iLoop < iPbNum; ++iLoop)
	{
		int ipbsize = 0;
		ipbsize = *(int *)pbuffer;
		pbuffer += sizeof(int);
		TABLE::Test* pKTestTemp = new TABLE::Test();
		pKTestTemp->ParseFromArray(pbuffer, ipbsize);
		pbuffer += ipbsize;
		printf("iLoop   %d  name %s email %s  /n ", iLoop, pKTestTemp->name().c_str(), pKTestTemp->email().c_str());
	}
	

	return true;

}