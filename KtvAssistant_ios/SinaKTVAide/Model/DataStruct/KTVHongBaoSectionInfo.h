//
//  KTVHongBaoSectionInfo.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/22.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface KTVHongBaoSectionInfo : NSObject

@property (nonatomic,strong) NSMutableArray* array;
//0新人奖励 ; 1每日奖励 ; 2个人红包
@property (nonatomic,assign) int nclass;
@property (nonatomic,strong) NSString* name;
@property (nonatomic,assign) BOOL expand;

@end
