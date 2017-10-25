//
//  NetUtilCallBackDelegate.h
//  SinaKTVAide
//
//  Created by cxp on 14-8-5.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol NetUtilCallBackDelegate <NSObject>

@optional

- (void)delegateInfo:(NSInteger)type;
- (void)delegateHoreInfo:(NSMutableDictionary*)dic;
- (void)delegateOnlineInfo;
- (void)delegateRedInfo:(NSMutableDictionary*)dic;
- (void)delegateRedStatue:(NSMutableDictionary*)dic;
- (void)delegateLeaveRoom;
- (void)delegateRedNum;

@optional
- (void)updateMusic;

@end

