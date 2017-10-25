//
//  User.m
//  SinaKTVAide
//
//  Created by sp on 13-12-5.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "User.h"
#import "PCommonUtil.h"

#define UID             @"UID"
#define NAME            @"name"
#define PWD             @"pwd"
#define HEADPHOTO       @"headphoto"
#define GENDER          @"gender"
#define SCORE           @"score"
#define ACCOUNTTYPE     @"accountType"
#define OPENACCOUNTS    @"openAccounts"
#define XGID            @"xgid"
#define Domin           @"domin"


@implementation User

@synthesize uid;
@synthesize name;
@synthesize headphoto;
@synthesize gender;
@synthesize gold;
@synthesize accountType;
@synthesize openAccounts;
@synthesize xgid;
@synthesize domin;
@synthesize type;

+(User *)initWithDefaultValue{
    
    User *user = [[User alloc] init];
    user.uid = 0L;
    user.name = @"";
    user.headphoto = @"";
    user.gender = 0;
    user.gold = 0L;
    user.accountType = 0;
    user.openAccounts = [[NSMutableArray alloc] init];
    
    return user;

}


+(User *)initWithLoginDictionary:(NSDictionary *)data{
    User *user = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            user = [[User alloc] init];
            
            NSDictionary *userData = [data objectForKey:@"User"];
            
            user.uid = [[userData objectForKey:@"uidx"] longValue];
            user.name = [PCommonUtil decodeBase64:[userData objectForKey:@"name"]];
            user.headphoto = [PCommonUtil decodeBase64:[userData objectForKey:@"headphoto"]];//[NSString stringWithFormat:@"%@",[data objectForKey:@"headphoto"]];
            user.gender = [[userData objectForKey:@"gender"] intValue];
            
            if([userData objectForKey:@"gold"]){
                user.gold = [[userData objectForKey:@"gold"] longValue];
            }
            
            if([userData objectForKey:@"account_type_id"]){
                user.accountType = [[userData objectForKey:@"account_type_id"] intValue];
            }
            
            if([data objectForKey:@"relevance_account"]){
                NSArray *accountsList = [data objectForKey:@"relevance_account"];
                int count = [accountsList count];
                for (int i = 0; i< count; i++) {
                    AssociatedAccount *account = [AssociatedAccount initWithDictionary:[accountsList objectAtIndex:i]];
                    [user.openAccounts addObject:account];
                    [account log];
                }
            }
            if([userData objectForKey:@"pwd"]){
                user.pwd = [userData objectForKey:@"pwd"];
            }
            if([userData objectForKey:@"xgid"]){
                user.xgid = [userData objectForKey:@"xgid"];
            }
            if([userData objectForKey:@"domin"]){
                user.domin = [userData objectForKey:@"domin"];
            }
            
        }
        
    }@catch (NSException *exception) {
        PLog(@"create User failed...please check");
    }
    
    return user;
}

+(User *)initWithUserDictionary:(NSDictionary *)data{
    User *user = nil;
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            user = [[User alloc] init];
            
            user.uid = [[data objectForKey:@"uidx"] longValue];
            
            if([data objectForKey:@"name"]){
                user.name = [PCommonUtil decodeBase64:[data objectForKey:@"name"]];
            }
            
            if([data objectForKey:@"headphoto"]){
                user.headphoto = [PCommonUtil decodeBase64:[data objectForKey:@"headphoto"]];//[NSString stringWithFormat:@"%@",[data objectForKey:@"headphoto"]];
            }
            
            if([data objectForKey:@"gender"]){
                user.gender = [[data objectForKey:@"gender"] intValue];
            }
            
            if([data objectForKey:@"gold"]){
                user.gold = [[data objectForKey:@"gold"] longValue];
            }
            
            if([data objectForKey:@"pwd"]){
                user.pwd = [data objectForKey:@"pwd"];
            }
            if([data objectForKey:@"xgid"]){
                user.xgid = [data objectForKey:@"xgid"];
            }
            if([data objectForKey:@"domin"]){
                user.domin = [data objectForKey:@"domin"];
            }
            if ([data objectForKey:@"TotalScore"]) {
                user.songtotalscore = [[data objectForKey:@"TotalScore"] intValue];
            }
        }
        
    }@catch (NSException *exception) {
        PLog(@"create User failed...please check");
    }
    
    return user;
}



- (id)init{
    self = [super init];
    if(self !=nil){
        self.uid = 0L;
        self.name = @"";
        self.headphoto = @"";
        self.gender = 0;
        self.gold = 0L;
        self.openAccounts = [[NSMutableArray alloc] init];
        self.pwd = @"";
        self.xgid = @"";
        self.domin = @"";
        self.songtotalscore = 0;
    }
    
    return self;
}

// 归档编码
- (void)encodeWithCoder:(NSCoder *)aCoder
{
    [aCoder encodeInt32:uid forKey:UID];
    [aCoder encodeObject:name forKey:NAME];
    [aCoder encodeObject:headphoto forKey:HEADPHOTO];
    [aCoder encodeObject:_pwd forKey:PWD];
    [aCoder encodeInt:gender forKey:GENDER];
    [aCoder encodeInt32:gold forKey:SCORE];
    [aCoder encodeInt:accountType forKey:ACCOUNTTYPE];
    [aCoder encodeObject:openAccounts forKey:OPENACCOUNTS];
    [aCoder encodeObject:xgid forKey:XGID];
    [aCoder encodeObject:domin forKey:Domin];
}

// 归档解码
- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super init];
    if(self != nil)
    {
        self.uid = [aDecoder decodeInt32ForKey:UID];
        self.name = [aDecoder decodeObjectForKey:NAME];
        self.pwd = [aDecoder decodeObjectForKey:PWD];
        self.headphoto = [aDecoder decodeObjectForKey:HEADPHOTO];
        self.gender = [aDecoder decodeIntForKey:GENDER];
        self.gold = [aDecoder decodeInt32ForKey:SCORE];
        self.accountType = [aDecoder decodeIntForKey:ACCOUNTTYPE];
        //self.openAccounts = [aDecoder decodeObjectForKey:OPENACCOUNTS];
        NSArray *array = [aDecoder decodeObjectForKey:OPENACCOUNTS];
        //NSLog(@"array-->%@", array);
        self.openAccounts = [NSMutableArray arrayWithArray:array];
        self.xgid = [aDecoder decodeObjectForKey:XGID];
        self.domin = [aDecoder decodeObjectForKey:Domin];
    }
    return self;
}

- (void) log
{
    PLog(@"user [uid=%ld, name=%@, headphoto=%@, gender=%d, gold=%ld, accountType=%d]", uid, name, headphoto, gender, gold, accountType);
}


-(NSString *)description{
    return [NSString stringWithFormat:@"User [uid=%ld, name=%@, headphoto=%@, gender=%d, gold=%ld, accountType=%d]", uid, name, headphoto, gender, gold, accountType];
}

@end
