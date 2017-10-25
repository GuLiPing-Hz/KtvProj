//
//  KTVHongBaoSectionInfo.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/22.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "KTVHongBaoSectionInfo.h"

@implementation KTVHongBaoSectionInfo

-(instancetype)init
{
    self = [super init];
    if(self)
    {
        self.array = [[NSMutableArray alloc] init];
        self.expand = YES;
        self.name = @"";
        self.nclass = -1;
    }
    return self;
}

@end
