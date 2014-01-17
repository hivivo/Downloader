/*****************************************************************************
 Copyright (c) 2014 Xu Xu (Vivo)
 
 http://git.xuxu.name/downloader
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include <iostream>
#include <unistd.h>
#include "Common.h"
#include "Downloader.h"

int main(int argc, const char * argv[])
{
    Downloader::instance()->setCompletedCallback([](int){
        cout << "All Done!" << endl;
        exit(0);
    });
    
    Downloader::instance()->download("http://coltsauthority.com/images/stories/merry-christmas4.jpg", "./");
    Downloader::instance()->download("http://static.tumblr.com/431d1861ed4b19e771839ed0f2488534/rk90lhh/cPjmqrglu/tumblr_static_002.jpg", "./");
    
    Downloader::instance()->download("http://static2.wikia.nocookie.net/__cb20091105052914/muppet/images/c/c9/Zoe-smaller-puppet.jpg", "./", [](int no)
    {
        cout << "Downloaded: #" << no << endl;
    });
    
    Downloader::instance()->download("http://upload.wikimedia.org/wikipedia/commons/6/65/View_Point_Inn_2007.05.04.jpg", "./");
    Downloader::instance()->download("http://upload.wikimedia.org/wikipedia/commons/thumb/f/fc/Yosemite_Valley_from_Wawona_Tunnel_view,_vista_point..JPG/1280px-Yosemite_Valley_from_Wawona_Tunnel_view,_vista_point..JPG", "./", "yosemite.jpg");
    Downloader::instance()->download("http://th01.deviantart.net/fs70/PRE/i/2012/280/5/9/smile__by_kp_shadowsquirrel-d5fw346.jpg", "./");
    
    sleep(100);
    
    return 0;
}

