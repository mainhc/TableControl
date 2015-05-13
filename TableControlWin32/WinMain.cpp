
#include "stdafx.h"
#include<iostream>
#include<string>
#include<stdio.h>
#include"../HelpFunc.h"
#include"../HelpXlsx.h"
#include"../zlib/zlib.h"
#include"../zlib/Unzipper.h"
#include"../tinyxml2/tinyxml2.h"



#pragma comment(lib,"zlibwapi.lib")
#pragma comment(lib,"libprotobuf.lib")
#pragma comment(lib,"libprotoc.lib")

#if defined(_WIN32)
#include"../vld/include/vld.h"
#pragma comment(lib,"vld.lib")
#endif


const std::string cstrComment1 = "xl/sharedStrings.xml";


int main()
{
	FILE* fp = fopen("../resources/test7.xml", "r");
	if (!fp) {
		printf("Error opening test file 'test7.xml'.\n"
			"Is your working directory the same as where \n"
			"the xmltest.cpp and test7.xml file are?\n\n"
#if defined( _MSC_VER )
			"In windows Visual Studio you may need to set\n"
			"Properties->Debugging->Working Directory to '..'\n"
#endif
			);
		exit(1);
	}
	int isize = GetFileSize(fp);
	Byte* pMem = new Byte[isize + 1];
	memset(pMem, 0, isize + 1);
	LoadFileToMemory(fp, (char*)pMem, isize);
	Byte atemp[1024 * 10] = { 0 };
	uLong itemp = sizeof(atemp);
	uLong iLen = sizeof(pMem);

	int errtemp = compress(atemp, &itemp, (const Bytef*)pMem, isize + 1);

	if (errtemp != Z_OK) {

		exit(1);
	}


	int err;
	Byte compr[200], uncompr[200];    // big enough
	uLong comprLen, uncomprLen;
	const char* hello = "12345678901234567890123456789012345678901234567890";

	uLong len = strlen(hello) + 1;
	comprLen = sizeof(compr) / sizeof(compr[0]);

	err = compress(compr, &comprLen, (const Bytef*)hello, len);

	if (err != Z_OK) {

		exit(1);
	}
	strcpy((char*)uncompr, "garbage");

	err = uncompress(uncompr, &uncomprLen, compr, comprLen);

	if (err != Z_OK) {
		exit(1);
	}

	//compress(atemp, &ilenght, pMem, isize+1);
	//int temp = compress(atemp,ilenght,pMem,isize+1);

	fclose(fp);

	tinyxml2::XMLDocument doc;
	doc.LoadFile("../resources/test7.xml");

	printf("loadfile erroe Id is %d ", doc.ErrorID());

	//tutorial::Person temp;
	//tutorial::Person_PhoneNumber tempnum;
	//tempnum.set_number("1389765746");

	//tutorial::Person_PhoneNumber* pPhone = temp.add_phone();
	//pPhone->set_number("1387667889");
	//temp.set_email("wwww.com");	
	//temp.set_id(342342);
	//temp.set_name("hahha");

	//std::string tempstr;

	//temp.SerializeToString(&tempstr);

	//

	////temp.AppendToString(&tempstr);

	//printf("Person_PhoneNumber  %s ", tempstr.c_str());

	cHelpXlsx ioXlsx;
	ioXlsx.LoadXlsx("../resources/testtable.xlsx");
	ioXlsx.SavePBData();

	//
	CUnzipper unZib;

	unZib.OpenZip("../resources/testtable.xlsx");

	printf("testtable.xlsx has file count %d ", unZib.GetFileCount());

	if (unZib.GotoFile(cstrComment1.c_str()))
	{		
		UZ_FileInfo kTempInfo;	
		unZib.GetFileInfo(kTempInfo);
		char * pBuffer = new char[kTempInfo.dwUncompressedSize+1];
		memset(pBuffer, 0, kTempInfo.dwUncompressedSize + 1);
		unZib.UnzipToBuffer(pBuffer, kTempInfo.dwUncompressedSize + 1);
		printf("xl/sharedStrings.xml is %s  ", pBuffer);
		printf("testtable.xlsx has file success ! ");
	}
	system("pause");
	return 0;
}