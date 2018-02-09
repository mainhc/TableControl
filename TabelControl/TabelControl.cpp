// TabelControl.cpp: 定义控制台应用程序的入口点。
//

#include"xlsControl.h"

const std::string cstrComment1 = "xl/sharedStrings.xml";

int main()
{
	cXlsControl ioXlsx;
	ioXlsx.LoadXlsx("testtable.xlsx");
	ioXlsx.ReadXlsxData();
	ioXlsx.DataToJsonFile();
	system("pause");
    return 0;
}

