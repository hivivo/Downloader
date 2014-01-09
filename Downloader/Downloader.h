//
//  Downloader.h
//  Downloader
//
//  Created by Xu Xu on 1/8/14.
//  Copyright (c) 2014 Xu Xu. All rights reserved.
//

#ifndef __Downloader__Downloader__
#define __Downloader__Downloader__

#include "Common.h"

class Downloader
{
public:
    Downloader();
    virtual ~Downloader();
    
    /**
     Download a file from url.
     @return: task ID. If failed, return -1
     */
    int download(string url);
    
};

#endif /* defined(__Downloader__Downloader__) */
