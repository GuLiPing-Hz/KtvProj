//
//  AutoLock.h
//  testnetutil
//
//  Created by guliping_imac on 14-7-3.
//  Copyright (c) 2014年 天格科技. All rights reserved.
//

#import <Foundation/Foundation.h>

class AutoLock
{
public:
    AutoLock(id<NSLocking> lock):locker(lock){
        [locker lock];
    }
    ~AutoLock(){
        [locker unlock];
    }
private:
    id<NSLocking> locker;
};

