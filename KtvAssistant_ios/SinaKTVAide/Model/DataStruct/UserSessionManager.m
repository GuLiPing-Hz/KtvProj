//
//  UserSessionManager.m
//  SinaKTVAide
//
//  Created by apple on 13-10-29.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "UserSessionManager.h"
#import "User.h"
@implementation UserSessionManager

@synthesize currentRoomInfo = _currentRoomInfo;
@synthesize currentUser = _currentUser;
@synthesize isLoggedIn = _isLoggedIn;

+(UserSessionManager *)getInstance{
    
    static UserSessionManager *instance = nil;
    
    @synchronized(self){
        if (nil == instance) {
            instance = [[self alloc] init];
            instance.currentRoomInfo = [RoomInfo initWithDefaultValue];
            instance.currentUser = [User initWithDefaultValue];
        }
    }
    return instance;
}

+(bool)isUserAlreadyLogin
{
    User *user = [UserSessionManager getInstance].currentUser;
    if( user.uid > 0 && user.name.length > 0 && user.accountType > 0) //&& (user.openAccounts !=nil) && ([user.openAccounts count] > 0 ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

+(bool)isRoomAlreadyLogin
{
    RoomInfo *room = [UserSessionManager getInstance].currentRoomInfo;
    
    if( ((int)[room.roomId intValue]) > 0 && room.roomPassword.length > 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}


-(void)updateCurrentUserInfo:(User *)_user
{
    if(_user){
        self.currentUser.headphoto = _user.headphoto;
        self.currentUser.name = _user.name;
        self.currentUser.gold = _user.gold;
        self.currentUser.domin = _user.domin;
        [self writeUserInfoToFile];
    }else{
        PLog(@"updateCurrentUserInfo Fail-->_user is nil");
    }
    
}

-(void)updateCurrentUserPhoto:(NSString *)_headPhoto
{
    if(_headPhoto){
        self.currentUser.headphoto = _headPhoto;
        [self writeUserInfoToFile];
    }else{
        PLog(@"updateCurrentUserPhoto Fail-->_headPhoto is nil");
    }
    
}


-(void)updateCurrentUserGold:(long)_gold
{
    self.currentUser.gold = _gold;
    [self writeUserInfoToFile];
}

+(void)clearCurrentUserInfo
{
    [UserSessionManager getInstance].currentUser = [User initWithDefaultValue];
    NSString *path = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
    NSString *loginInfoPath = [path stringByAppendingPathComponent:@"login.plist"];
    if(![[NSFileManager defaultManager] fileExistsAtPath:loginInfoPath])
    {
        PLog(@"不存在需要删除的文件");
    }
    else
    {
        bool res = [[NSFileManager defaultManager] removeItemAtPath:loginInfoPath error:nil];
        if(res)
        {
            PLog(@"登录信息文件删除Success");
        }
        else
        {
            PLog(@"登录信息文件删除Fail");
        }
    }
    
}

+(void)clearCurrentRoomInfo
{
    [UserSessionManager getInstance].currentRoomInfo = [RoomInfo initWithDefaultValue];
}


/**
 * 将用户信息保存在本地文件
 *
 */
-(void)writeUserInfoToFile
{
    // 登录信息存放的路径 NSHomeDirectory 根目录下的 Documents/login.plist
    NSString *path = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
    NSString *loginInfoPath = [path stringByAppendingPathComponent:@"login.plist"];
    NSDictionary *dicLogin = [NSDictionary dictionaryWithObject:self.currentUser forKey:@"user"]; // 字典存放自定义对象
    NSData *loginData = [NSKeyedArchiver archivedDataWithRootObject:dicLogin]; // 归档成 NSData
    BOOL res = [loginData writeToFile:loginInfoPath atomically:NO];
    if(res){
        PLog(@"writeUserInfoToFile SUCCESS ,accountType:=%d",self.currentUser.accountType);
    }else{
        PLog(@"writeUserInfoToFile Fail");
    }
}

/**
 * 从本地文件获取用户信息
 *
 */
-(User *)readUserInfoFromFile
{
    NSString *path = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
    NSString *loginInfoPath = [path stringByAppendingPathComponent:@"login.plist"];
    
    if(![[NSFileManager defaultManager] fileExistsAtPath:path]){
        PLog(@"不存在登录信息文件");
        return nil;
    }else{
        PLog(@"存在登录信息文件");
    }
    
    NSDictionary *dicLogin = [NSKeyedUnarchiver unarchiveObjectWithFile:loginInfoPath];
    User *user = [dicLogin objectForKey:@"user"];
    self.currentUser = user;
    return self.currentUser;
}

@end
