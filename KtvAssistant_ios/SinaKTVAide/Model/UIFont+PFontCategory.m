//
//  UIFont+PFontCategory.m
//  SinaKtv
//
//  Created by Zhang Chen on 13-10-22.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "UIFont+PFontCategory.h"

@implementation UIFont (PFontCategory)

+(UIFont *)fontName:(NSString *)fontName size:(CGFloat)fontSize{
    return [UIFont fontWithName:fontName size:fontSize];
}

+(UIFont *)fontOfSystem:(CGFloat)fontSize{
    return [UIFont systemFontOfSize:fontSize];
}

/*
 * 20  26   30  34
 * 四种混用
 */
+(UIFont *)fontOfApp:(CGFloat)fontSize{
    return [UIFont fontName:@"MicrosoftYaHei" size:fontSize];
    //    return [UIFont fontName:@"STHeitiTC-Light" size:fontSize];
    //    return [UIFont fontWithName:@"STHeitiTC-Medium" size:fontSize];
}

@end
