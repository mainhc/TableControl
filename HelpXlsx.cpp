#include"HelpXlsx.h"
#include"zlib/Unzipper.h"
#include"protobuf/protobufhead.h"
#include"tinyxml2\tinyxml2.h"


cHelpXlsx::cHelpXlsx()
{
	for (int iLoop = eXlsxComments1; iLoop < eXlsxDataNum; ++iLoop)
	{
		m_apXlsxData[iLoop] = NULL;
	}
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

bool cHelpXlsx::SavePBData()
{
	int iRow = 0;
	int iCol = 0;

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


	return true;
}