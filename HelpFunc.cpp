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

