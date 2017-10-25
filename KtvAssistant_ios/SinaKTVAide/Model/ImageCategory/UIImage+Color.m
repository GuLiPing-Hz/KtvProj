//
//  UIImage+Color.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-11-3.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "UIImage+Color.h"

@implementation UIImage (Color)


+(UIImage *)imageWithColor:(UIColor *)color Rect:(CGRect)rect
{
    UIGraphicsBeginImageContext(rect.size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    CGContextSetFillColorWithColor(context, [color CGColor]);
    CGContextFillRect(context, rect);
    
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return image;
}

@end
