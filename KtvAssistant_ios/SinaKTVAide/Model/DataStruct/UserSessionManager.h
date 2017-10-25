//
//  UserSessionManager.h
//  SinaKTVAide
//
//  Created by apple on 13-10-29.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RoomInfo.h"
#import "User.h"
#import "AssociatedAccount.h"

@interface UserSessionManager : NSObject

@property (nonatomic, strong) RoomInfo *currentRoomInfo;
@property (nonatomic, strong) User *currentUser;
@property (nonatomic, assign) BOOL isLoggedIn;

+(UserSessionManager *)getInstance;

/**
 *  判断用户是否已登录
 *
 *  @return
 */
+(bool)isUserAlreadyLogin;

/**
 *  判断房间是否已登录
 *
 *  @return
 */
+(bool)isRoomAlreadyLogin;

/**
 *  更新当前登录用户的信息
 *
 *  @return 
 */
-(void)updateCurrentUserInfo:(User *)_user;

/**
 *  更新当前登录用户的头像
 *
 *  @return
 */
-(void)updateCurrentUserPhoto:(NSString *)_headPhoto;

/**
 *  更新当前登录用户的积分信息
 *
 *  @return
 */
-(void)updateCurrentUserGold:(long)_gold;

/**
 * 将用户信息保存在本地文件
 *
 */
-(void)writeUserInfoToFile;


/**
 * 从本地文件获取用户信息
 *
 */
-(User *)readUserInfoFromFile;


+(void)clearCurrentUserInfo;

+(void)clearCurrentRoomInfo;

@end
