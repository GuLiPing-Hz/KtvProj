//
//  User.h
//  SinaKTVAide
//
//  Created by sp on 13-12-5.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AssociatedAccount.h"

@interface User : NSObject <NSCoding>

@property (nonatomic, assign) long uid;             //用户id
@property (nonatomic, retain) NSString *name;       //用户名
@property (nonatomic, retain) NSString *headphoto;  //头像
@property (nonatomic, assign) int gender;           //性别

@property (nonatomic, assign) long gold;           //用户的金币
@property (nonatomic, assign) int accountType;      //账户类型 （1：新浪微博， 2:QQ,3:微信用户）
@property (nonatomic, retain) NSMutableArray *openAccounts;//第三方账户列表
@property (nonatomic, retain) NSString *xgid;  //星光账号
@property (nonatomic, retain) NSString *pwd;    //星光密码
@property (nonatomic, retain) NSString *domin;    //用户域名
@property (nonatomic,assign) int songtotalscore;//演唱累计得分
@property (nonatomic, assign) char type;         //登录形式 0-主播 1-本包厢 2-其他包厢主播 3-大厅

+(User *)initWithDefaultValue;
+(User *)initWithLoginDictionary:(NSDictionary *)data;
+(User *)initWithUserDictionary:(NSDictionary *)data;

-(void) log;
@end
