//
//  main.cpp
//  Downloader
//
//  Created by Xu Xu on 1/8/14.
//  Copyright (c) 2014 Xu Xu. All rights reserved.
//

#include "Common.h"
#include "Downloader.h"

int main(int argc, const char * argv[])
{
    Downloader::instance()->download("http://coltsauthority.com/images/stories/merry-christmas4.jpg");
    Downloader::instance()->wait();
    return 0;
}

