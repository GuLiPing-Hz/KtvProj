//
//  NSString+Regex.h
//  chat_rebot
//
//  Created by lp_show on 14-7-15.
//  Copyright (c) 2014年 Lipeng. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSString (Regex)

/**
 *  判断聊天内容是否符合法 - 字母和数字不能超过5个
 */
- (BOOL)regexMessage;

/**
 *  查找表情
 */
-(NSMutableArray *)regexfaceContent;

/**
 *  用户名由5-12字母和数字组成
 */
- (BOOL)regexUserName;

/**
 *  密码必须由5~20个字符组成
 */
- (BOOL)regexPassword;

@end
