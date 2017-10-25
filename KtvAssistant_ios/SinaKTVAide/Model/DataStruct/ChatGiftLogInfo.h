//
//  ChatGiftLogInfo.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-20.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ChatGiftLogInfo : NSObject

@property (nonatomic, strong) NSString *nickName;  //
@property (nonatomic, strong) NSString *headPhoto;
@property (nonatomic, strong) NSString *tradeDate;
@property (nonatomic, strong) NSString *giftName;
@property (nonatomic, assign) NSInteger giftNum;
@property (nonatomic, assign) NSInteger gold;
@property (nonatomic, strong) NSString *ktvName;  //在大厅送礼时 ktv和room都为空
@property (nonatomic, strong) NSString *roomName; //

+(ChatGiftLogInfo *)initWithDictionary:(NSDictionary *)data;
-(void)Log;



@end
