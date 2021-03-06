// TabelControl.cpp: 定义控制台应用程序的入口点。
//

#include"xlsControl.h"
#include"HelpFunc.h"

const std::string cstrComment1 = "xl/sharedStrings.xml";

int main()
{
	std::vector<std::string> akFileName;
	FilePathAllFile("cehuaxlsx", akFileName);
	cXlsControl ioXlsx;
	for (int iloop = 0; iloop < akFileName.size();iloop++)
	{
		std::string tempFileName = "cehuaxlsx/" + akFileName[iloop];
		if (tempFileName.find(".xlsx") != std::string::npos)
		{
			ioXlsx.LoadXlsx(tempFileName.c_str());
			ioXlsx.ReadXlsxData();
			ioXlsx.DataToJsonFile();
			ioXlsx.Release();
		}
	}	
	system("pause");
    return 0;
}

