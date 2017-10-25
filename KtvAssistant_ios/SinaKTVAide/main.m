//
//  main.m
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-10-23.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "KTVAppDelegate.h"
#import "signal.h"

int main(int argc, char * argv[])
{
    //防止服务器异常，导致APP异常退出
    signal(SIGPIPE,SIG_IGN);
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([KTVAppDelegate class]));
    }
}
