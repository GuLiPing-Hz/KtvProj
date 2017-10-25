//
//  AssociatedAccount.h
//  SinaKTVAide
//
//  Created by sp on 13-12-18.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AssociatedAccount : NSObject <NSCoding>

@property (nonatomic, assign) int type;             //账户类型 （1：新浪微博， 2：腾讯）
@property (nonatomic, strong) NSString *openID;     //第三方平台账户的ID
@property (nonatomic, strong) NSString *accessToken;  //Token
@property (nonatomic, strong) NSString *expirationdate;    //
@property (nonatomic, strong) NSString *nickName;      //

+(AssociatedAccount *)initWithDefaultValue;
+(AssociatedAccount *)initWithDictionary:(NSDictionary *)data;

-(void) log;

@end
