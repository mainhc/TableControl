//
//  HelpFunc.h
//  TableControl
//
//  Created by 寒春 郭 on 15-4-26.
//  Copyright (c) 2015年 寒春 郭. All rights reserved.
//

#ifndef TableControl_HelpFunc_h
#define TableControl_HelpFunc_h
#include <iostream>

int GetFileSize(FILE * pfile);

bool LoadFileToMemory(FILE* pfile,char* pMem, int iSize);

#if defined(_WIN32)
void UTF_8ToGB2312(std::string & pOut, const char* p);
#endif


#endif
