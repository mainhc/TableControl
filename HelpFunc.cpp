//
//  HelpFunc.cpp
//  TableControl
//
//  Created by 寒春 郭 on 15-4-26.
//  Copyright (c) 2015年 寒春 郭. All rights reserved.
//

#include "HelpFunc.h"


int GetFileSize(FILE * pfile)
{
    if(pfile == nullptr)
    {
        return 0;
    }
    fseek( pfile, 0, SEEK_SET );
    fseek( pfile, 0, SEEK_END );
    int filelength = ftell( pfile );
    fseek( pfile, 0, SEEK_SET );
    return filelength;
}


bool LoadFileToMemory(FILE* pfile,char* pMem, int iSize)
{
    
    fseek( pfile, 0, SEEK_SET );
    fseek( pfile, 0, SEEK_END );
    const long filelength = ftell( pfile );
    fseek( pfile, 0, SEEK_SET );
    if ( filelength == -1L ) {
        return false;
    }
    
    const size_t size = filelength;
    if ( size == 0 || size!=iSize) {
        return false;
    }
   
    size_t read = fread( pMem, 1, size, pfile );
    if ( read != size ) {
        return false;
    }    
    return true;
}

#if defined(_WIN32)
#include <windows.h>
#include <wchar.h>

void UTF_8ToGB2312(std::string & pOut, const char* p)
{
	char acChar[2];
	char acWChar[2];
	while (*p)
	{
		if (*p > 0)
		{
			pOut += *p;
			p++;
		}
		else
		{
			if (p[1] == 0xff)
			{
				if (((p[0] >= 0x21) && (p[0] < 0x5a)) || ((p[0] >= 0x10) && (p[0] < 0x19)) )
				{
					char buffer[1024] = { 0 };
					sprintf_s(buffer, sizeof(buffer), "存在全角字符 %x", p[0]);
					printf(buffer);
				}
			}
			acWChar[1] = ((p[0] & 0x0f) << 4) + ((p[1] >> 2) & 0x0f);
			acWChar[0] = ((p[1] & 0x03) << 6) + (p[2] & 0x3f);
			WideCharToMultiByte(CP_ACP, NULL, (LPCWSTR)acWChar, 1, acChar, sizeof(acChar), NULL, NULL);
			pOut += acChar[0];
			pOut += acChar[1];
			p += 3;
		}
	}
	
}
#endif

