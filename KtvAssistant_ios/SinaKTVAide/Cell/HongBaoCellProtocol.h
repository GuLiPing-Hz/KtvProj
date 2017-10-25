//
//  HongBaoCellProtocol.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/21.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "KTVHongBaoInfo.h"

@protocol HongBaoCellProtocol <NSObject>

@optional
-(void)onTouchGetHB:(KTVHongBaoInfo*)info;

@end
