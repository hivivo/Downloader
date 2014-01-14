//
//  main.cpp
//  Downloader
//
//  Created by Xu Xu on 1/8/14.
//  Copyright (c) 2014 Xu Xu. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include "Common.h"
#include "Downloader.h"

int main(int argc, const char * argv[])
{
    Downloader::instance()->download("http://coltsauthority.com/images/stories/merry-christmas4.jpg", "./");
    Downloader::instance()->download("http://static.tumblr.com/431d1861ed4b19e771839ed0f2488534/rk90lhh/cPjmqrglu/tumblr_static_002.jpg", "./");
    
    Downloader::instance()->download("http://static2.wikia.nocookie.net/__cb20091105052914/muppet/images/c/c9/Zoe-smaller-puppet.jpg", "./", [](string url)
    {
        cout << "Downloaded: " << url << endl;
    });
    
    Downloader::instance()->download("http://upload.wikimedia.org/wikipedia/commons/6/65/View_Point_Inn_2007.05.04.jpg", "./");
    Downloader::instance()->download("http://upload.wikimedia.org/wikipedia/commons/thumb/f/fc/Yosemite_Valley_from_Wawona_Tunnel_view,_vista_point..JPG/1280px-Yosemite_Valley_from_Wawona_Tunnel_view,_vista_point..JPG", "./");
    Downloader::instance()->download("http://th01.deviantart.net/fs70/PRE/i/2012/280/5/9/smile__by_kp_shadowsquirrel-d5fw346.jpg", "./");
    
    sleep(10);
    
    return 0;
}

