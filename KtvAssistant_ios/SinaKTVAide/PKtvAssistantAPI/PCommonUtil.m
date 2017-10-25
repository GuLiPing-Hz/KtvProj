//
//  PCommonUtil.m
//  MobileKtv
//
//  Created by Ming Jianhua on 12-9-4.
//  Copyright (c) 2012年 9158.com. All rights reserved.
//

#import "PCommonUtil.h"
#import <CommonCrypto/CommonDigest.h>
#import "GTMBase64.h"
#import "NSData+AES256.h"
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#import "JSONKit.h"

//4 file cache
#define SECRET_KEY @"192c96beaec59d367329c70016e7a50f"

@implementation PCommonUtil

+(NSString *)md5Encode:(NSString *)str{
    @try {
        if(str && [str isKindOfClass:[NSString class]]){
            const char *cStr = [str UTF8String];
            unsigned char result[16];
            CC_MD5(cStr, strlen(cStr), result);
            
            return [NSString stringWithFormat:
                    @"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
                    result[0], result[1], result[2], result[3],
                    result[4], result[5], result[6], result[7],
                    result[8], result[9], result[10], result[11],
                    result[12], result[13], result[14], result[15]
                    ];
        }
    }
    @catch (NSException *exception) {
        //NSLog(@"PCommonUtil md5 encode error...please check: %@", str);
    }
    
    return str;
}

#pragma mark MAC
// Return the local MAC addy
// Courtesy of FreeBSD hackers email list
// Accidentally munged during previous update. Fixed thanks to mlamb.
+(NSString *)GetMacAddress
{
	int                    mib[6];
	size_t                len;
	char                *buf;
	unsigned char        *ptr;
	struct if_msghdr    *ifm;
	struct sockaddr_dl    *sdl;
	
	mib[0] = CTL_NET;
	mib[1] = AF_ROUTE;
	mib[2] = 0;
	mib[3] = AF_LINK;
	mib[4] = NET_RT_IFLIST;
	
	if ((mib[5] = if_nametoindex("en0")) == 0) {
		//printf("Error: if_nametoindex error/n");
		return NULL;
	}
	
	if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
		//printf("Error: sysctl, take 1/n");
		return NULL;
	}
	
	if ((buf = malloc(len)) == NULL) {
		//printf("Could not allocate memory. error!/n");
		return NULL;
	}
	
	if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
		//printf("Error: sysctl, take 2");
		return NULL;
	}
	
	ifm = (struct if_msghdr *)buf;
	sdl = (struct sockaddr_dl *)(ifm + 1);
	ptr = (unsigned char *)LLADDR(sdl);
	// NSString *outstring = [NSString stringWithFormat:@"%02x:%02x:%02x:%02x:%02x:%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
	NSString *outstring = [NSString stringWithFormat:@"%02x%02x%02x%02x%02x%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
	free(buf);
	return [outstring uppercaseString];
	
}

+(NSString *)generateSecretString:(NSString *)str{
    NSString *tempStr = [NSString stringWithFormat:@"%@*(!%@%@*(!", @"%", str, @"%"];
    return [self md5Encode:tempStr];
}

+(NSString *)encodeUrlParameter:(NSString *)param{
    if (param) {
        return (NSString *)
        CFBridgingRelease(CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault,
                                                                  (__bridge CFStringRef)param,
                                                                  NULL,
                                                                  (CFStringRef)@"!*'();@&=+$,/?%#[]",
                                                                  kCFStringEncodingUTF8));        
    }
    
    return param;
}



+(NSString *)decodeUrlParameter:(NSString *)param{
    if (param) {
        return [param stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    }
    
    return param;
}

+(NSString *)generateAPIStringWithSecret:(NSString *)baseUrl param:(NSString *)param{
    NSString *srcUrl = [NSString stringWithFormat:@"%@%@", baseUrl, param];
    NSString *secret = [self generateSecretString:srcUrl];
    NSString *encodeUrl = [NSString stringWithFormat:@"%@&secret=%@", srcUrl, secret];
    NSString *strMacAddress = [self GetMacAddress];
    strMacAddress = (strMacAddress != nil) ? [self encodeBase64:strMacAddress] : @"";
    strMacAddress = (strMacAddress != nil && strMacAddress.length > 0) ? [self encodeUrlParameter:strMacAddress] : @"";
    NSString *strUnique = [NSString stringWithFormat:@"unique=%@", strMacAddress];
    
    encodeUrl = [NSString stringWithFormat:@"%@&%@&%@", encodeUrl, KTV_ASSISTANT_API_VERSION, strUnique];
    PLog(@"encode url: %@", encodeUrl);
    
    return encodeUrl;
}

+(NSString *)encodeBase64:(NSString *)str{
    if (str && [str isKindOfClass:[NSString class]]) {
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding allowLossyConversion:YES];
        return [[NSString alloc] initWithData:[GTMBase64 encodeData:data] encoding:NSUTF8StringEncoding];
    }
    return str;
}

+(NSString *)decodeBase64:(NSString *)str{
    if (str && [str isKindOfClass:[NSString class]]) {
        return [[NSString alloc] initWithData:[GTMBase64 decodeString:str] encoding:NSUTF8StringEncoding];
    }
    return str;
}

+(NSString *)decodeStringOfBase64AndUrlEncode:(NSString *)str{
    if (str && [str isKindOfClass:[NSString class]]) {
        
        return [[self decodeBase64:str] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    }
    return str;
}

//1. AES256加解密 2. base64 encode
/*
 * 1. 把普通的str转成data类型
 * 2. 对data做AES256加密，得到加密后的data
 * 3. 对加密后的data做base64编码
 */
+(NSData *)encodeAesAndBase64DataFromStr:(NSString *)str secretKey:(NSString *)secretKey{
    
    NSData *encodeData = nil;
    if (str && secretKey) {
        NSData *tempData = [str dataUsingEncoding:NSUTF8StringEncoding allowLossyConversion:YES];
        NSData *tempAesData = [tempData AES256EncryptWithKey:secretKey];
        encodeData = [GTMBase64 encodeData:tempAesData];
    }
    return encodeData;
}

/*
 * 1. 把普通的str转成data类型
 * 2. 对data做AES256加密，得到加密后的data
 * 3. 对加密后的data做base64编码
 * 4. 把加密并编码后的data转成str，用于输入查看
 */
+(NSString *)encodeAesAndBase64StrFromStr:(NSString *)str secretKey:(NSString *)secretKey{
    NSData *tempEncodeData = [self encodeAesAndBase64DataFromStr:str secretKey:secretKey];
    if (tempEncodeData) {
        return [[NSString alloc] initWithData:tempEncodeData encoding:NSUTF8StringEncoding];;
    }
    return nil;
}

/*
 * 1. 把做了AES256加密，并做过base64编码的str转成data类型
 * 2. 对加密后的data做base64解码，得到新的data
 * 3. 对data做AES256解密，得到解密后的data
 */
+(NSData *)decodeAesAndBase64DataFromStr:(NSString *)str secretKey:(NSString *)secretKey{
    
    NSData *decodeData = nil;
    if (str && secretKey) {
        NSData *tempData = [str dataUsingEncoding:NSUTF8StringEncoding];
        NSData *tempBase64Data = [GTMBase64 decodeData:tempData];
        decodeData = [tempBase64Data AES256DecryptWithKey:secretKey];
    }
    return decodeData;
}

/*
 * 1. 把做了AES256加密，并做过base64编码的str转成data类型
 * 2. 对加密后的data做base64解码，得到新的data
 * 3. 对data做AES256解密，得到解密后的data
 * 4. 把解密后的data转成普通的str
 */
+(NSString *)decodeAesAndBase64StrFromStr:(NSString *)str secretKey:(NSString *)secretKey{
    NSData *tempDecodeData = [self decodeAesAndBase64DataFromStr:str secretKey:secretKey];
    if (tempDecodeData) {
        return [[NSString alloc] initWithData:tempDecodeData encoding:NSUTF8StringEncoding];;
    }
    return nil;
}

//AES加解密, 用于缓存文件的保密处理
+(NSData *)encodeAES:(NSDictionary *)dic{
    NSString *tempString = [dic JSONString];
    NSData *tempData = [tempString dataUsingEncoding:NSUTF8StringEncoding allowLossyConversion:YES];
    return [tempData AES256EncryptWithKey:SECRET_KEY];
}

+(NSDictionary *)decodeAES:(NSData *)data{
    NSData *tempData = [data AES256DecryptWithKey:SECRET_KEY];
    NSString *tempString = [[NSString alloc] initWithData:tempData encoding:NSUTF8StringEncoding];
    return [tempString objectFromJSONString];
}

/**
 *  为了方便查看错误内容，只对result字段数据做加密，其它错误状态和错误提示明码显示，方便查找问题。
 *  （如果要恢复原来的整块数据加密，需要调整接口版本 
 *   #define KTV_ASSISTANT_API_VERSION @"versioncode=1.0.0&client=iphone"，和解密方法）
 *
 *  @param response 服务端返回的加密的整个数据结果
 *
 *  @return 返回解密后的一个数据字典
 */
+(NSDictionary *)NSDictionaryFromJSONString:(NSString *)response{
    
    NSString *tempResponse = response;
//    NSLog(@"tempResponse: %@",tempResponse);
    
    id str = [tempResponse objectFromJSONString];
    if ([str isKindOfClass:[NSDictionary class]]) {
        NSDictionary *dict = (NSDictionary *)str;
        NSString *status = [dict objectForKey:@"status"];
        if ((nil == status) || ((id)[NSNull null] == status) || [status isEqualToString:@""]) {
            
            NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
            [results setObject:@"0" forKey:@"status"];
            [results setObject:kPKtvAssistantErrorMsgSystem forKey:@"msg"];
            
            return results;
            
        } else if ([status compare:@"1"] == NSOrderedSame) {
            
            NSString *srcResult = [dict objectForKey:@"result"];
            NSString *tempResult = [PCommonUtil decodeAesAndBase64StrFromStr:srcResult secretKey:KTV_ASSISTANT_API_SECRET_KEY];
            PLog(@"tempResult: %@",tempResult);
            
            if ([tempResult length] > 2) {
                id strResult = [tempResult objectFromJSONString];
                if ([strResult isKindOfClass:[NSDictionary class]]) {
                    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                    [results setObject:@"1" forKey:@"status"];
                    [results setObject:[dict objectForKey:@"msg"] forKey:@"msg"];
                    [results setObject:[dict objectForKey:@"errorcode"] forKey:@"errorcode"];
                    [results setObject:strResult forKey:@"result"];
                    
                    return results;
                }
                else
                {
                    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                    [results setObject:@"0" forKey:@"status"];
                    [results setObject:kPKtvAssistantErrorMsgJsonError forKey:@"msg"];
                    [results setObject:@"0" forKey:@"errorcode"];
                    
                    return results;
                }
            } else {
                NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                [results setObject:@"1" forKey:@"status"];
                [results setObject:[dict objectForKey:@"msg"] forKey:@"msg"];
                [results setObject:[dict objectForKey:@"errorcode"] forKey:@"errorcode"];
                [results setObject:@"" forKey:@"result"];
                
                return results;
            }
            
        } else if ([status compare:@"0"] == NSOrderedSame) {
            NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
            [results setObject:@"0" forKey:@"status"];
            [results setObject:[dict objectForKey:@"msg"] forKey:@"msg"];
            [results setObject:[dict objectForKey:@"errorcode"] forKey:@"errorcode"];
            [results setObject:@"" forKey:@"result"];
            
            return results;
        }//if
    }//if
    
    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
    [results setObject:@"0" forKey:@"status"];
    [results setObject:kPKtvAssistantErrorMsgSystem forKey:@"msg"];
    [results setObject:@"0" forKey:@"errorcode"];
    
    return results;
    
}

//通过nsdata解析数据
+(NSDictionary *)NSDictionaryFromJSONData:(NSData *)responseData{
    
    @try {
        
        if (responseData) {
            id str = [NSJSONSerialization JSONObjectWithData:responseData options:NSJSONReadingMutableContainers error:nil];
            if ([str isKindOfClass:[NSDictionary class]]) {
                NSDictionary *dict = (NSDictionary *)str;
                NSString *status = [dict objectForKey:@"status"];
                if ((nil == status) || ((id)[NSNull null] == status) || [status isEqualToString:@""]) {
                    
                    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                    [results setObject:@"0" forKey:@"status"];
                    [results setObject:kPKtvAssistantErrorMsgSystem forKey:@"msg"];
                    
                    return results;
            
                } else if ([status compare:@"1"] == NSOrderedSame) {
                    
                    NSString *srcResult = [dict objectForKey:@"result"];
                    NSString *tempResult = [PCommonUtil decodeAesAndBase64StrFromStr:srcResult secretKey:KTV_ASSISTANT_API_SECRET_KEY];
                    //NSLog(@"tempResult: %@",tempResult);
                    
                    if ([tempResult length] > 2) {
                        id strResult = [tempResult objectFromJSONString];
                        if ([strResult isKindOfClass:[NSDictionary class]]) {
                            NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                            [results setObject:@"1" forKey:@"status"];
                            [results setObject:[dict objectForKey:@"msg"] forKey:@"msg"];
                            [results setObject:[dict objectForKey:@"errorcode"] forKey:@"errorcode"];
                            [results setObject:strResult forKey:@"result"];
                            
                            return results;
                        }
                        else
                        {
                            NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                            [results setObject:@"0" forKey:@"status"];
                            [results setObject:kPKtvAssistantErrorMsgJsonError forKey:@"msg"];
                            [results setObject:@"0" forKey:@"errorcode"];
                            
                            return results;
                        }
                    } else {
                        NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                        [results setObject:@"1" forKey:@"status"];
                        [results setObject:[dict objectForKey:@"msg"] forKey:@"msg"];
                        [results setObject:[dict objectForKey:@"errorcode"] forKey:@"errorcode"];
//                        [results setObject:@"" forKey:@"result"];
                        
                        return results;
                    }
                    
                } else if ([status compare:@"0"] == NSOrderedSame) {
                    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                    [results setObject:@"0" forKey:@"status"];
                    [results setObject:[dict objectForKey:@"msg"] forKey:@"msg"];
                    [results setObject:[dict objectForKey:@"errorcode"] forKey:@"errorcode"];
//                    [results setObject:@"" forKey:@"result"];
                    
                    return results;
                }//if
            }//if
            
            
        }
        
    }
    @catch (NSException *exception) {
        PLog(@"NSException...");
    }
    
    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
    [results setObject:@"0" forKey:@"status"];
    [results setObject:kPKtvAssistantErrorMsgSystem forKey:@"msg"];
    [results setObject:@"0" forKey:@"errorcode"];
    
    return results;
}


//通过nsdata解析数据 result 是金币数据，不是json
+(NSDictionary *)NSDictionaryFromJSONData2:(NSData *)responseData{
    
    @try {
        
        if (responseData) {
            //NSString *tempResponse = [[NSString alloc] initWithData:responseData encoding:NSUTF8StringEncoding];
            //NSLog(@"tempResponse: %@",tempResponse);
            
            id str = [NSJSONSerialization JSONObjectWithData:responseData options:NSJSONReadingMutableContainers error:nil];
            if ([str isKindOfClass:[NSDictionary class]]) {
                NSDictionary *dict = (NSDictionary *)str;
                NSString *status = [dict objectForKey:@"status"];
                if ((nil == status) || ((id)[NSNull null] == status) || [status isEqualToString:@""])
                {
                    
                    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                    [results setObject:@"0" forKey:@"status"];
                    [results setObject:kPKtvAssistantErrorMsgSystem forKey:@"msg"];
                    
                    return results;
                    
                }
                else if ([status compare:@"1"] == NSOrderedSame) {
                    
                    NSString *srcResult = [dict objectForKey:@"result"];
                    NSString *tempResult = [PCommonUtil decodeAesAndBase64StrFromStr:srcResult secretKey:KTV_ASSISTANT_API_SECRET_KEY];
                    //NSLog(@"tempResult: %@",tempResult);
                    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                    [results setObject:@"1" forKey:@"status"];
                    [results setObject:[dict objectForKey:@"msg"] forKey:@"msg"];
                    [results setObject:[dict objectForKey:@"errorcode"] forKey:@"errorcode"];
                    [results setObject:tempResult forKey:@"result"];
                    
                    return results;
                    
                } else if ([status compare:@"0"] == NSOrderedSame) {
                    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                    [results setObject:@"0" forKey:@"status"];
                    [results setObject:[dict objectForKey:@"msg"] forKey:@"msg"];
                    [results setObject:[dict objectForKey:@"errorcode"] forKey:@"errorcode"];
                    //                    [results setObject:@"" forKey:@"result"];
                    
                    return results;
                }//if
            }//if
            
            
        }
        
    }
    @catch (NSException *exception) {
        PLog(@"NSException...");
    }
    
    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
    [results setObject:@"0" forKey:@"status"];
    [results setObject:kPKtvAssistantErrorMsgSystem forKey:@"msg"];
    [results setObject:@"0" forKey:@"errorcode"];
    
    return results;
    
}



//通过nsdata解析数据,由于数据庞大，所以是压缩数据不解密数据
+(NSDictionary *)NSDictionaryFromJSONData3:(NSData *)responseData{
    
    @try {
        
        if (responseData) {
            id str = [NSJSONSerialization JSONObjectWithData:responseData options:NSJSONReadingMutableContainers error:nil];
            if ([str isKindOfClass:[NSDictionary class]]) {
                NSDictionary *dict = (NSDictionary *)str;
                NSString *status = [dict objectForKey:@"status"];
                if ((nil == status) || ((id)[NSNull null] == status) || [status isEqualToString:@""])
                {
                    
                    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                    [results setObject:@"0" forKey:@"status"];
                    [results setObject:kPKtvAssistantErrorMsgSystem forKey:@"msg"];
                    
                    return results;
                    
                }
                else if ([status compare:@"1"] == NSOrderedSame) {
                    
                    NSString *srcResult = [dict objectForKey:@"result"];
                    //NSLog(@"tempResult: %@",tempResult);
                    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                    [results setObject:@"1" forKey:@"status"];
                    [results setObject:[dict objectForKey:@"msg"] forKey:@"msg"];
                    [results setObject:[dict objectForKey:@"errorcode"] forKey:@"errorcode"];
                    [results setObject:srcResult forKey:@"result"];
                    
                    return results;
                    
                } else if ([status compare:@"0"] == NSOrderedSame) {
                    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
                    [results setObject:@"0" forKey:@"status"];
                    [results setObject:[dict objectForKey:@"msg"] forKey:@"msg"];
                    [results setObject:[dict objectForKey:@"errorcode"] forKey:@"errorcode"];
                    //                    [results setObject:@"" forKey:@"result"];
                    
                    return results;
                }//if
            }//if
            
            
        }
        
    }
    @catch (NSException *exception) {
        PLog(@"NSException...");
    }
    
    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
    [results setObject:@"0" forKey:@"status"];
    [results setObject:kPKtvAssistantErrorMsgSystem forKey:@"msg"];
    [results setObject:@"0" forKey:@"errorcode"];
    
    return results;
    
}


/**
 *  改方法是对整块数据做解密，出错时，必须调试解密才能知道错误问题，非常不方便
 *
 *  @param response 服务端返回的加密的整个数据结果
 *
 *  @return 返回解密后的一个数据字典
 */
+(NSDictionary *)NSDictionaryFromJSONStringOld:(NSString *)response{
    
    NSString *tempResponse = [PCommonUtil decodeAesAndBase64StrFromStr:response secretKey:KTV_ASSISTANT_API_SECRET_KEY];
    PLog(@"tempResponse: %@",tempResponse);
    
    id str = [tempResponse objectFromJSONString];
    if ([str isKindOfClass:[NSDictionary class]]) {
        NSDictionary *dict = (NSDictionary *)str;
        NSString *status = [dict objectForKey:@"status"];
        if ((nil == status) || ((id)[NSNull null] == status) || [status isEqualToString:@""]) {
            
            NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
            [results setObject:@"0" forKey:@"status"];
            [results setObject:kPKtvAssistantErrorMsgSystem forKey:@"msg"];
            
            return results;
            
        } else {
            return dict;
        }//if
    }//if
    
    NSMutableDictionary *results = [[NSMutableDictionary alloc] initWithCapacity:1];
    [results setObject:@"0" forKey:@"status"];
    [results setObject:kPKtvAssistantErrorMsgSystem forKey:@"msg"];
    
    return results;
}


+(NSString *)getHomeCacheDirectory{
    NSString *cachesDirectory = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *commonDirectory = [[[cachesDirectory stringByAppendingPathComponent:[[NSProcessInfo processInfo] processName]] stringByAppendingPathComponent:@"common"] copy];
    
    return commonDirectory;
    
}

+(NSString *)storeDataToFile:(NSString *)fileName subDir:(NSString *)subDir data:(NSData *)data{
    NSString *floderpath = [[PCommonUtil getHomeCacheDirectory] stringByAppendingPathComponent:subDir];
    NSFileManager *fm = [NSFileManager defaultManager];
    if (![fm fileExistsAtPath:floderpath isDirectory:NULL])
    {
        NSError *error;
        if (![fm createDirectoryAtPath:floderpath withIntermediateDirectories:YES attributes:nil error:&error]) {
            [NSException raise:@"Exception" format:@"%@",[error localizedDescription]];
        }
    }
    NSString *storePath = [floderpath stringByAppendingPathComponent:fileName];
    //NSLog(@"PCommonUtil storeDataToFile storePath: %@", storePath);
    [data writeToFile:storePath atomically:NO];
    
    return storePath;
}


@end
