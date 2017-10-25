//
//  AssociatedAccount.m
//  SinaKTVAide
//
//  Created by sp on 13-12-18.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "AssociatedAccount.h"
#import "PCommonUtil.h"


#define TYPE            @"type"
#define OPENID          @"openID"
#define TOKEN           @"accessToken"
#define DATE            @"expirationdate"
#define NICKNAME        @"nickName"

@implementation AssociatedAccount

@synthesize type;
@synthesize openID;
@synthesize accessToken;
@synthesize expirationdate;
@synthesize nickName;


+(AssociatedAccount *)initWithDefaultValue{
    AssociatedAccount *account = [[AssociatedAccount alloc] init];
    account.type = 0;
    account.openID = @"";
    account.accessToken = @"";
    account.expirationdate = @"";
    account.nickName = @"";
    
    return account;
    
}


+(AssociatedAccount *)initWithDictionary:(NSDictionary *)data{
    AssociatedAccount *account = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            account = [[AssociatedAccount alloc] init];
            
            account.type = [[data objectForKey:@"type_id"] intValue];
            account.openID = [NSString stringWithFormat:@"%@",[data objectForKey:@"uid"]];
            account.accessToken = [NSString stringWithFormat:@"%@",[data objectForKey:@"accesstoken"]];
            account.expirationdate = [PCommonUtil decodeBase64:[data objectForKey:@"expirationdate"]];
            account.nickName = [PCommonUtil decodeBase64:[data objectForKey:@"nickname"]];
            
        }
        
    }@catch (NSException *exception) {
        PLog(@"create Account failed...please check");
    }
    
    return account;
    
    
}


- (id)init{
    self = [super init];
    if(self != nil){
        self.type = 0;
        self.openID = @"";
        self.accessToken = @"";
        self.expirationdate = @"";
        self.nickName = @"";
    }
    
    return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder{
    [aCoder encodeInt:type forKey:TYPE];
    [aCoder encodeObject:openID forKey:OPENID];
    [aCoder encodeObject:accessToken forKey:TOKEN];
    [aCoder encodeObject:expirationdate forKey:DATE];
    [aCoder encodeObject:nickName forKey:NICKNAME];
}


- (id)initWithCoder:(NSCoder *)aDecoder{
    self = [super init];
    if(self != nil){
        self.type = [aDecoder decodeIntForKey:TYPE];
        self.openID = [aDecoder decodeObjectForKey:OPENID];
        self.accessToken = [aDecoder decodeObjectForKey:TOKEN];
        self.expirationdate = [aDecoder decodeObjectForKey:DATE];
        self.nickName = [aDecoder decodeObjectForKey:NICKNAME];
    }
    return self;
}


- (void) log{
    PLog(@"account [type=%d, openID=%@, accessToken=%@, expirationdate=%@, nickName=%@]", type, openID, accessToken, expirationdate, nickName);
}

- (NSString *)description{
    return [NSString stringWithFormat:@"account [type=%d, openID=%@, accessToken=%@, expirationdate=%@, nickName=%@]", type, openID, accessToken, expirationdate, nickName];
}

@end
