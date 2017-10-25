//
//  UIFont+PFontCategory.h
//  SinaKtv
//
//  Created by Zhang Chen on 13-10-22.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIFont (PFontCategory)

+(UIFont *)fontName:(NSString *)fontName size:(CGFloat)fontSize;
+(UIFont *)fontOfSystem:(CGFloat)fontSize;
+(UIFont *)fontOfApp:(CGFloat)fontSize;
@end
