#include"HelpXlsx.h"
#include"zlib/Unzipper.h"
#include"protobuf/protobufhead.h"


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

bool cHelpXlsx::SavePBData()
{
	int iRow = 0;
	int iCol = 0;

	return true;
}