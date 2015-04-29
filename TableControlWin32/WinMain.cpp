#include<iostream>
#include<stdio.h>
#include"../HelpFunc.h"
#include"../zlib/zlib.h"
#include"../tinyxml2/tinyxml2.h"

#pragma comment(lib,"zlibwapi.lib")


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
	uLong itemp = 0;
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


	system("pause");
	return 0;
}