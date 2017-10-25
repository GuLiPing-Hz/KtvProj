//
//  NSString+Regex.m
//  chat_rebot
//
//  Created by lp_show on 14-7-15.
//  Copyright (c) 2014年 Lipeng. All rights reserved.
//

#import "NSString+Regex.h"

@implementation NSString (Regex)

- (NSString *)filterFace
{
    NSString *regula = @"\\[[abc]/[a-z0-9]{1,}\\]";
    NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:regula options:NSRegularExpressionCaseInsensitive error:nil];
    NSArray *arrayOfAllMatches = [regex matchesInString:self options:0 range:NSMakeRange(0, [self length])];
    
    if( arrayOfAllMatches != nil && [arrayOfAllMatches count] > 0 )
    {
        NSString* result = @"";
        NSRange range = NSMakeRange(0,0);
        for(NSTextCheckingResult *match in arrayOfAllMatches)
        {
            if( match.range.location != range.location )
            {
                range.length = match.range.location - range.location;
                result = [result stringByAppendingString:[self substringWithRange:range]];
            }
            range.location = match.range.location + match.range.length;
            //face
            if ( match.range.length > 0 )
            {
                PLog(@"%@",[self substringWithRange:NSMakeRange(match.range.location,match.range.length)]);
            }
        }
        if ( range.location != [self length] ) {
            range.length = [self length] - range.location;
            result = [result stringByAppendingString:[self substringWithRange:range]];
        }
        return result;
    }
    return nil;
}

/**
 *  判断聊天内容是否符合法 - 字母和数字不能超过5个
 */
- (BOOL)regexMessage
{
    NSString* message = [self filterFace];
    if (!message) {
        message = self;
    } else if (message.length == 0) {
        return YES;
    }
    NSString *regula = @"[\u62630-9\uff11\uff12\uff13\uff14\uff15\uff16\uff17\uff18\uff19\uff10\u4e00\u4e8c\u4e09\u56db\u4e94\u516d\u4e03\u516b\u4e5d\u96f6\u2460\u2461\u2462\u2463\u2464\u2465\u2466\u2467\u2468\u3220\u3221\u3222\u3223\u3224\u3225\u3226\u3227\u3228\u2474\u2475\u2476\u2477\u2478\u2479\u247a\u247b\u247c\u58f9\u8d30\u53c1\u8086\u4f0d\u9646\u67d2\u634c\u7396./\uff0e\u70b9\u3002a-z\uff41\uff42\uff43\uff44\uff45\uff46\uff47\uff48\uff49\uff47\uff4b\uff4c\uff4d\uff4e\uff4f\uff50\uff51\uff52\uff53\uff54\uff55\uff56\uff57\uff58\uff59\uff5a\uff21\uff22\uff23\uff24\uff25\uff26\uff27\uff28\uff29\uff27\uff2b\uff2c\uff2d\uff2e\uff2f\uff30\uff31\uff32\uff33\uff34\uff35\uff36\uff37\uff38\uff39\uff3a\u3089\u310b\u2488\u2489\u248b\u248c\u248d\u248e\u248f\u2490\u0392\u0421\u039f\u039a\u041a\u041c\u0422\u03a7\u0399\u039d\u0442\u043a\u03c4\u03ba\u2530\u03a4]";
    
    NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:regula options:NSRegularExpressionCaseInsensitive error:nil];
    NSArray *arrayOfAllMatches = [regex matchesInString:message options:0 range:NSMakeRange(0,[message length])];
    if( arrayOfAllMatches != nil && [arrayOfAllMatches count] > 0 )
    {
        NSMutableArray* array = [NSMutableArray array];
        NSRange range = NSMakeRange(0,0);
        for(NSTextCheckingResult *match in arrayOfAllMatches)
        {
            range.location = match.range.location + match.range.length;
            if ( match.range.length > 0 )
            {
                //image
                NSString* image = [message substringWithRange:NSMakeRange(match.range.location,match.range.length)];
                [array addObject:image];
            }
        }
        return (array.count > 5) ? NO : YES;
    }
    return YES;
}

/**
 *  查找表情
 */
-(NSMutableArray *)regexfaceContent
{
    NSString *regula = @"\\[[abc]/[a-z0-9]{1,}\\]";
    NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:regula options:NSRegularExpressionCaseInsensitive error:nil];
    NSArray *arrayOfAllMatches = [regex matchesInString:self options:0 range:NSMakeRange(0,[self length])];
    if( arrayOfAllMatches != nil && [arrayOfAllMatches count] > 0 )
    {
        NSMutableArray* array = [NSMutableArray array];
        NSRange range = NSMakeRange(0,0);
        for(NSTextCheckingResult *match in arrayOfAllMatches)
        {
            if(match.range.location != range.location)
            {
                range.length = match.range.location - range.location;
                [array addObject:[self substringWithRange:range]];
            }
            range.location = match.range.location + match.range.length;
            if (match.range.length > 0)
            {
                //image
                [array addObject:[self substringWithRange:NSMakeRange(match.range.location,match.range.length)]];
            }
        }
        if (range.location != [self length]) {
            range.length = [self length] - range.location;
            [array addObject:[self substringWithRange:range]];
        }
        return array;
    }
    return nil;
}

/**
 *  用户名由5-12字母和数字组成
 */
- (BOOL)regexUserName
{
    NSString *useridRegex = @"^[a-z|A-Z]{1}[a-z|A-Z|0-9]{4,11}$";
    NSPredicate *useridTest = [NSPredicate predicateWithFormat:@"SELF MATCHES %@", useridRegex];
    return [useridTest evaluateWithObject:self];
}

/**
 *  密码必须由5~20个字符组成
 */
- (BOOL)regexPassword
{
    NSString *pwdRegex = @"^[\\w@$]{5,20}$";//@"^[\\w@$]{5,20}$"
    NSPredicate *pwdTest = [NSPredicate predicateWithFormat:@"SELF MATCHES %@", pwdRegex];
    return [pwdTest evaluateWithObject:self];
}

@end
