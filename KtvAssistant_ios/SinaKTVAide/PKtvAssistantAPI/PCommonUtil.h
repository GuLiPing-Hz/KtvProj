//
//  PCommonUtil.h
//  MobileKtv
//
//  Created by Ming Jianhua on 12-9-4.
//  Copyright (c) 2012年 9158.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "KTVBaseConfig.h"

@interface PCommonUtil : NSObject{
    
}

+(NSString *)md5Encode:(NSString *)str;
+(NSString *)generateSecretString:(NSString *)str;

+(NSString *)encodeUrlParameter:(NSString *)param;
+(NSString *)decodeUrlParameter:(NSString *)param;
+(NSString *)generateAPIStringWithSecret:(NSString *)baseUrl param:(NSString *)param;

+(NSString *)encodeBase64:(NSString *)str;
+(NSString *)decodeBase64:(NSString *)str;
+(NSString *)decodeStringOfBase64AndUrlEncode:(NSString *)str;

+(NSData *)encodeAesAndBase64DataFromStr:(NSString *)str secretKey:(NSString *)secretKey;
+(NSString *)encodeAesAndBase64StrFromStr:(NSString *)str secretKey:(NSString *)secretKey;

+(NSData *)decodeAesAndBase64DataFromStr:(NSString *)str secretKey:(NSString *)secretKey;
+(NSString *)decodeAesAndBase64StrFromStr:(NSString *)str secretKey:(NSString *)secretKey;

+(NSData *)encodeAES:(NSDictionary *)dic;
+(NSDictionary *)decodeAES:(NSData *)data;

+(NSDictionary *)NSDictionaryFromJSONString:(NSString *)response;
+(NSDictionary *)NSDictionaryFromJSONData:(NSData *)responseData;
+(NSDictionary *)NSDictionaryFromJSONData2:(NSData *)responseData;
+(NSDictionary *)NSDictionaryFromJSONData3:(NSData *)responseData;

+(NSString *)getHomeCacheDirectory;
+(NSString *)storeDataToFile:(NSString *)fileName subDir:(NSString *)subDir data:(NSData *)data;

@end
